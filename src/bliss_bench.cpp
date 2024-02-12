#include <alex.h>
#include <lipp.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <cxxopts.hpp>
#include <functional>
#include <iostream>
#include <string>

#include "bliss/bench_alex.h"
#include "bliss/bliss_index.h"
#include "bliss/util/reader.h"
#include "bliss/util/timer.h"

typedef unsigned key_type;
typedef unsigned value_type;

using namespace bliss;

struct BlissConfig {
    std::string data_file;
    double preload_factor;
    double write_factor;
    double read_factor;
    double mixed_read_write_ratio;
    int seed;
    int verbosity;
    std::string index;
};

BlissConfig parse_args(int argc, char *argv[]) {
    BlissConfig config;
    cxxopts::Options options(
        "bliss", "BLISS: Benchmarking Learned Index Structures for Sortedness");

    try {
        options.add_options()("d,data_file", "Path to the data file",
                              cxxopts::value<std::string>())(
            "p,preload_factor", "Preload factor",
            cxxopts::value<double>()->default_value("0.5"))(
            "w,write_factor", "Write factor",
            cxxopts::value<double>()->default_value("0.25"))(
            "r,read_factor", "Read factor",
            cxxopts::value<double>()->default_value("0.1"))(
            "m,mixed_read_write_ratio", "Read write ratio",
            cxxopts::value<double>()->default_value("0.5"))(
            "s,seed", "Random Seed value",
            cxxopts::value<int>()->default_value("0"))(
            "v,verbosity", "Verbosity [0: Info| 1: Debug | 2: Trace]",
            cxxopts::value<int>()->default_value("0")->implicit_value("1"))(
            "i,index", "Index type (alex|lipp)",
            cxxopts::value<std::string>()->default_value("btree"));

        auto result = options.parse(argc, argv);
        config = {result["data_file"].as<std::string>(),
                  result["preload_factor"].as<double>(),
                  result["write_factor"].as<double>(),
                  result["read_factor"].as<double>(),
                  result["mixed_read_write_ratio"].as<double>(),
                  result["seed"].as<int>(),
                  result["verbosity"].as<int>(),
                  result["index"].as<std::string>()};
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        exit(1);
    }
    return config;
}

void display_config(BlissConfig config) {
    spdlog::trace("Data File: {}", config.data_file);
    spdlog::trace("Preload Factor: {}", config.preload_factor);
    spdlog::trace("Write Factor: {}", config.write_factor);
    spdlog::trace("Read Factor: {}", config.read_factor);
    spdlog::trace("Read Write Ratio: {}", config.mixed_read_write_ratio);
    spdlog::trace("Verbosity {}", config.verbosity);
    spdlog::trace("Index: {}", config.index);
}

void execute_non_empty_reads(BlissIndex<key_type, value_type> &tree,
                             std::vector<key_type> &data, int num_reads,
                             std::function<double()> rng) {
    size_t key_idx;
    for (auto blank = 0; blank < num_reads; blank++) {
        key_idx = std::round(rng() * data.size());
        tree.get(data[key_idx]);
    }
}

void execute_inserts(BlissIndex<key_type, value_type> &tree,
                     std::vector<key_type>::iterator &start,
                     std::vector<key_type>::iterator &end) {
    for (auto curr = start; curr != end; ++curr) {
        tree.put(*curr, *curr);
    }
}

void execute_mixed_workload(BlissIndex<key_type, key_type> &tree,
                            std::vector<key_type>::iterator &start,
                            std::vector<key_type>::iterator &end,
                            double mixed_ratio, std::function<double()> rng) {
    auto num_items = end - start;
    auto num_reads = std::round(num_items * mixed_ratio);
    auto num_writes = num_items - num_reads;
    auto current = start;
    bool stop_coin_flipping = false;
    while (num_reads > 0 && num_writes > 0) {
        if ((num_reads == 0 || stop_coin_flipping || rng() > 0.5) &&
            num_writes > 0) {
            tree.put(*current, *current);
            num_writes--;
            current++;
        } else if (num_reads > 0) {
            size_t key_idx = std::round(rng() * num_items);
            tree.get(*(start + key_idx));
            num_reads--;
        } else {
            // Number of reads is 0, but we coin flipped wrong, so stop flipping
            stop_coin_flipping = true;
        }
    }
}

void workload_executor(BlissIndex<key_type, value_type> &tree,
                       std::vector<key_type> &data, const BlissConfig &config,
                       const int seed) {
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int> distribution(0, 1);

    size_t num_inserts = data.size();
    size_t num_preload = std::round(config.preload_factor * num_inserts);
    size_t num_writes = std::round(config.write_factor * data.size());
    size_t num_mixed = num_inserts - (num_preload + num_writes);
    size_t num_reads = std::round(config.read_factor * data.size());

    // Timing for preloading index
    spdlog::debug("Preloading {} items", num_preload);
    auto preload_start = data.begin();
    auto preload_end = preload_start + num_preload;
    auto preload_time = time_function(
        [&]() { execute_inserts(tree, preload_start, preload_end); });
    spdlog::info("Preload Time (ns): {}", preload_time);

    // Timing for writes on index
    spdlog::debug("Writing {} items", num_writes);
    auto write_start = preload_end;
    auto write_end = write_start + num_writes;
    auto write_time =
        time_function([&]() { execute_inserts(tree, write_start, write_end); });
    spdlog::info("Write Time (ns): {}", write_time);

    // Timing for mixed workloads running
    spdlog::debug("Running Mixed {} items", num_mixed);
    auto mix_start = write_end;
    auto mix_end = mix_start + num_mixed;
    auto mix_time = time_function([&]() {
        execute_mixed_workload(tree, mix_start, mix_end,
                               config.mixed_read_write_ratio,
                               [&]() { return distribution(generator); });
    });
    spdlog::info("Mix Time (ns): {}", mix_time);

    // Timing for reads on index
    spdlog::debug("Reading {} items", num_reads);
    auto read_time = time_function([&]() {
        execute_non_empty_reads(tree, data, num_reads,
                                [&]() { return distribution(generator); });
    });
    spdlog::info("Read Time (ns): {}", read_time);
}

int main(int argc, char *argv[]) {
    auto config = parse_args(argc, argv);
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

    auto in_data = bliss::read_file<key_type>(config.data_file.c_str());

    std::unique_ptr<BlissIndex<key_type, value_type>> index;
    // Call the respective function based on the index value
    if (config.index == "alex") {
        index.reset(new BlissAlexIndex<key_type, value_type>());
    } else if (config.index == "lipp") {
        return 0;
    }

    workload_executor(*index, in_data, config, 0);

    return 0;
}
