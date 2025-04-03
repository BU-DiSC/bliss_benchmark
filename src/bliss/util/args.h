#ifndef BLISS_ARGS_H
#define BLISS_ARGS_H
#include <cxxopts.hpp>
#include <iostream>
#include <string>

#include "bliss/util/config.h"

using namespace bliss::utils::config;

namespace bliss {
namespace utils {
namespace args {
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
            "i,index", "Index type [alex | lipp | btree | bepstree | lsm]",
            cxxopts::value<std::string>()->default_value("btree"))(
            "file_type", "Input file type [binary | txt]",
            cxxopts::value<std::string>()->default_value("txt"))(
            "use_preload", "Use index defined preload",
            cxxopts::value<bool>()->default_value("false"))(
            "range-query", "Range query factor",
            cxxopts::value<double>()->default_value("0.0"))(
            "selectivity", "Selectivity factor (percentage of domain)",
            cxxopts::value<double>()->default_value("0.01"));

        auto result = options.parse(argc, argv);
        config = {
            .data_file = result["data_file"].as<std::string>(),
            .preload_factor = result["preload_factor"].as<double>(),
            .write_factor = result["write_factor"].as<double>(),
            .read_factor = result["read_factor"].as<double>(),
            .mixed_read_write_ratio =
                result["mixed_read_write_ratio"].as<double>(),
            .seed = result["seed"].as<int>(),
            .verbosity = result["verbosity"].as<int>(),
            .index = result["index"].as<std::string>(),
            .file_type = result["file_type"].as<std::string>(),
            .use_preload = result["use_preload"].as<bool>(),
            .range_query_factor = result["range-query"].as<double>(),
            .selectivity_factor = result["selectivity"].as<double>(),
        };
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << options.help() << std::endl;
        exit(1);
    }
    return config;
}
}  // namespace args
}  // namespace utils
}  // namespace bliss
#endif