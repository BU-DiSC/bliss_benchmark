#include <alex.h>
#include <lipp.h>
#include <spdlog/spdlog.h>

#include <cxxopts.hpp>
#include <iostream>
#include <iterator>
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
    double raw_write_factor;
    double raw_read_factor;
    double mixed_read_write_ratio;
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
            "w,raw_write_factor", "Raw write factor",
            cxxopts::value<double>()->default_value("1.0"))(
            "r,raw_read_factor", "Raw read factor",
            cxxopts::value<double>()->default_value("0.1"))(
            "m,mixed_read_write_ratio", "Read write ratio",
            cxxopts::value<double>()->default_value("0.5"))(
            "i,index", "Index type (alex|lipp)",
            cxxopts::value<std::string>()->default_value("btree"));

        auto result = options.parse(argc, argv);
        config.data_file = result["data_file"].as<std::string>();
        config.preload_factor = result["preload_factor"].as<double>();
        config.raw_write_factor = result["raw_write_factor"].as<double>();
        config.raw_read_factor = result["raw_read_factor"].as<double>();
        config.mixed_read_write_ratio =
            result["mixed_read_write_ratio"].as<double>();
        config.index = result["index"].as<std::string>();

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        exit(1);
    }
    return config;
}

void display_config(BlissConfig config) {
    spdlog::info("Data File: {}", config.data_file);
    spdlog::info("Preload Factor: {}", config.preload_factor);
    spdlog::info("Raw Write Factor: {}", config.raw_write_factor);
    spdlog::info("Raw Read Factor: {}", config.raw_read_factor);
    spdlog::info("Read Write Ratio: {}", config.mixed_read_write_ratio);
    spdlog::info("Index: {}", config.index);
}

void execute_inserts(BlissIndex<key_type, value_type> &tree,
                     std::vector<key_type> &data, unsigned num_load) {
    // iterate through the data and insert into the tree
    for (unsigned i = 0; i < num_load; i++) {
        tree.put(data[i], data[i]);
    }
}

void workload_executor(BlissIndex<key_type, value_type> &tree,
                       std::vector<key_type> &data, const BlissConfig &conf) {
    unsigned num_inserts = data.size();
    // unsigned raw_queries = conf.raw_read_factor / 100.0 * num_inserts;
    unsigned raw_writes = conf.raw_write_factor / 100.0 * num_inserts;
    unsigned mixed_size = conf.mixed_read_write_ratio / 100.0 * num_inserts;
    unsigned num_load = num_inserts - raw_writes - mixed_size;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, 1);

    // unsigned mix_inserts = 0;
    // unsigned mix_queries = 0;
    // uint32_t ctr_empty = 0;
    // value_type idx = 0;
    //
    // auto it = data.cbegin();
    // key_type offset = 0;

    spdlog::info("Preloading...");
    auto preload_time =
        time_function([&]() { execute_inserts(tree, data, num_load); });

    spdlog::info("Preload Time (ns): {}", preload_time);

    spdlog::info("Executing raw writes...");

    // auto raw_write_time = time_function([&]() {
    //     execute_inserts(tree, data, raw_writes);
    // });  // TODO: this still needs to skip num_load entries btw

    // std::cerr << "Mixed load (2*" << mixed_size << "/" << num_inserts <<
    // ")\n"; auto insert_time = start; auto query_time = start; while
    // (mix_inserts < mixed_size || mix_queries < mixed_size) {
    //     if (mix_queries >= mixed_size ||
    //         (mix_inserts < mixed_size && distribution(generator))) {
    //         auto _start = std::chrono::high_resolution_clock::now();
    //         tree.put(*it++ + offset, idx++);
    //         insert_time += std::chrono::high_resolution_clock::now() -
    //         _start; mix_inserts++;
    //     } else {
    //         key_type query_index = generator() % idx + offset;
    //         auto _start = std::chrono::high_resolution_clock::now();
    //         key_type res = tree.get(query_index);
    //         query_time += std::chrono::high_resolution_clock::now() - _start;
    //         ctr_empty += !res;
    //         mix_queries++;
    //     }
    // }
    // duration = (insert_time - start);
    // results << ", " << duration.count();
    // duration = (query_time - start);
    // results << ", " << duration.count();

    // std::cerr << "Raw read (" << raw_queries << "/" << num_inserts << ")\n";
    // std::uniform_int_distribution<unsigned> range_distribution(0,
    //                                                            num_inserts -
    //                                                            1);
    // start = std::chrono::high_resolution_clock::now();
    // for (unsigned int i = 0; i < raw_queries; i++) {
    //     tree.get(data[range_distribution(generator) % data.size()] + offset);
    // }
    // duration = std::chrono::duration_cast<std::chrono::nanoseconds>(
    //     std::chrono::high_resolution_clock::now() - start);
    // results << ", " << duration.count();
}

int main(int argc, char *argv[]) {
    // bool index_set = false;
    auto config = parse_args(argc, argv);

    // display the config
    display_config(config);

    auto in_data = bliss::read_file<key_type>(config.data_file.c_str());

    std::unique_ptr<BlissIndex<key_type, value_type>> index;
    // Call the respective function based on the index value
    if (config.index == "alex") {
        index.reset(new BlissAlexIndex<key_type, value_type>());
    } else if (config.index == "lipp") {
        return 0;
    }
    workload_executor(*index, in_data, config);
    return 0;
}
