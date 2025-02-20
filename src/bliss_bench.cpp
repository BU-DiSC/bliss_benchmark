#include <alex.h>
#include <lipp.h>
#include <spdlog/common.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>

#include "bliss/bench_alex.h"
#include "bliss/bench_art.h"
#include "bliss/bench_btree.h"
#include "bliss/bench_leveldb.h"
#include "bliss/bench_lipp.h"
#include "bliss/bench_pgm.h"
#include "bliss/bench_skiplist.h"
#include "bliss/bliss_index.h"
#include "bliss/util/args.h"
#include "bliss/util/config.h"
#include "bliss/util/execute.h"
#include "bliss/util/reader.h"
#include "bliss/util/timer.h"
#include "include/pgm/pgm_index_dynamic.hpp"
#include "skip_list.h"

using namespace bliss::utils;

void execute_bulkload(bliss::BlissIndex<key_type, value_type> &tree,
                      std::vector<std::pair<key_type, value_type>> &values) {
    spdlog::trace("Bulkloading values");
    tree.bulkload(values);
}

void execute_mixed_workload(bliss::BlissIndex<key_type, key_type> &tree,
                            std::vector<key_type>::iterator &start,
                            std::vector<key_type>::iterator &end,
                            double mixed_ratio, int seed = 0) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, 1);

    auto num_items = end - start;
    auto num_reads = std::round(num_items * mixed_ratio);
    auto num_writes = num_items - num_reads;
    auto current = start;
    bool stop_coin_flipping = false;
    while (num_reads > 0 && num_writes > 0) {
        if ((num_reads == 0 || stop_coin_flipping || dist(gen) > 0.5) &&
            num_writes > 0) {
            tree.put(*current, *current);
            num_writes--;
            current++;
        } else if (num_reads > 0) {
            size_t key_idx = std::round(dist(gen) * num_items);
            tree.get(*(start + key_idx));
            num_reads--;
        } else {
            // Number of reads is 0, but we coin flipped wrong, so stop flipping
            stop_coin_flipping = true;
        }
    }
}
std::vector<std::pair<key_type, value_type>> create_preload_vec(
    std::vector<key_type>::iterator &start,
    std::vector<key_type>::iterator &end, bool sort_values = true,
    int value_generator_seed = 0) {
    std::mt19937 gen(value_generator_seed);
    std::uniform_int_distribution<key_type> dist(0, 2 << 16);
    std::vector<std::pair<key_type, value_type>> vec;

    if (sort_values) {
        spdlog::trace("Sorting values");
        std::stable_sort(start, end);
    }

    spdlog::trace("Creating key-value pairs");
    for (auto curr = start; curr != end; ++curr) {
        vec.push_back(std::make_pair(*curr, dist(gen)));
    }

    return vec;
}

void workload_executor(bliss::BlissIndex<key_type, value_type> &tree,
                       std::vector<key_type> &data,
                       const config::BlissConfig &config, const int seed) {
    size_t num_inserts = data.size();
    size_t num_preload = std::round(config.preload_factor * num_inserts);
    size_t num_writes = std::round(config.write_factor * data.size());
    size_t num_mixed = num_inserts - (num_preload + num_writes);
    size_t num_reads = std::round(config.read_factor * data.size());

    // Timing for preloading index
    spdlog::debug("Preloading {} items", num_preload);
    auto preload_start = data.begin();
    auto preload_end = preload_start + num_preload;
    unsigned long long preload_time;
    unsigned long long preload_creation_time;
    if (config.use_preload) {
        auto start = std::chrono::high_resolution_clock::now();
        auto vec = create_preload_vec(preload_start, preload_end);
        preload_creation_time =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start)
                .count();
        preload_time = time_function([&]() { execute_bulkload(tree, vec); });
    } else {
        auto start = std::chrono::high_resolution_clock::now();
        auto vec = create_preload_vec(preload_start, preload_end, false);
        preload_creation_time =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - start)
                .count();
        preload_time = time_function([&]() {
            executor::execute_inserts(tree, preload_start, preload_end);
        });
    }
    spdlog::info("Preload Creation Time (ns): {}", preload_creation_time);
    spdlog::info("Preload Time (ns): {}", preload_time);

    // Timing for writes on index
    spdlog::debug("Writing {} items", num_writes);
    auto write_start = preload_end;
    auto write_end = write_start + num_writes;
    auto write_time = time_function(
        [&]() { executor::execute_inserts(tree, write_start, write_end); });
    spdlog::info("Write Time (ns): {}", write_time);

    // Timing for mixed workloads running
    spdlog::debug("Running Mixed {} items", num_mixed);
    auto mix_start = write_end;
    auto mix_end = mix_start + num_mixed;
    auto mix_time = time_function([&]() {
        execute_mixed_workload(tree, mix_start, mix_end,
                               config.mixed_read_write_ratio, seed);
    });
    spdlog::info("Mix Time (ns): {}", mix_time);

    // Timing for reads on index
    spdlog::debug("Reading {} items", num_reads);
    auto read_time = time_function([&]() {
        executor::execute_non_empty_reads(tree, data, num_reads, seed);
    });
    spdlog::info("Read Time (ns): {}", read_time);
}

int main(int argc, char *argv[]) {
    auto config = args::parse_args(argc, argv);
    switch (config.verbosity) {
        case 1:
            spdlog::set_level(spdlog::level::debug);
            break;
        case 2:
            spdlog::set_level(spdlog::level::trace);
            break;
        default:
            spdlog::set_level(spdlog::level::info);
    }
    display_config(config);

    std::vector<key_type> data;
    if (config.file_type == "binary") {
        data = bliss::read_file_binary<key_type>(config.data_file.c_str());
    } else {
        data = bliss::read_file<key_type>(config.data_file.c_str());
    }
    spdlog::debug("data.at(0) = {}", data.at(0));
    spdlog::debug("data.at({}) = {}", data.size() - 1,
                  data.at(data.size() - 1));

    std::unique_ptr<bliss::BlissIndex<key_type, value_type>> index;
    // Call the respective function based on the index value
    if (config.index == "alex") {
        index.reset(new bliss::BlissAlexIndex<key_type, value_type>());
    } else if (config.index == "lipp") {
        index.reset(new bliss::BlissLippIndex<key_type, value_type>());
    } else if (config.index == "btree") {
        index.reset(new bliss::BlissBTreeIndex<key_type, value_type>());
    } else if (config.index == "skiplist") {
        index.reset(new bliss::BlissSkipListIndex<key_type, value_type>());
    } else if (config.index == "art") {
        index.reset(new bliss::BlissARTIndex<key_type, value_type>());
    } else if (config.index == "pgm") {
        index.reset(new bliss::BlissPGMIndex<key_type, value_type>());
    } else if (config.index == "leveldb") {
        index.reset(new bliss::BlissLevelDBIndex<key_type, value_type>());
    } else {
        spdlog::error(config.index + " not implemented yet", 1);
    }

    workload_executor(*index, data, config, 0);

    index->end_routine();

    return 0;
}
