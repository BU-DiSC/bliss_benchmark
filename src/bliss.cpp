#include <alex.h>
#include <lipp.h>
#include <spdlog/spdlog.h>

#include <cxxopts.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

struct BlissConfig {
    std::string data_file;
    double preload_factor;
    double raw_write_factor;
    double raw_read_factor;
    double mixed_read_write_ratio;
    std::string index;
};

BlissConfig parse_args(int argc, char* argv[]) {
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

    } catch (const std::exception& e) {
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

//read test file
std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::vector<int> integers;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return integers;
    }

    int value;
    while (file >> value) {
        integers.push_back(value);
    }

    file.close();
    return integers;
}
//read integer as string
std::vector<std::string> readIntegersAsStringFromFile(const std::string& filename) {
    std::vector<std::string> integerStrings;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return integerStrings;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Optionally, you can add a check here to verify that the line is a valid integer
        integerStrings.push_back(line);
    }

    file.close();
    return integerStrings;
}

// Separate functions for each index
void processIndexAlex() {
    // Placeholder for "alex"
    spdlog::info("Processing index: ALEX");
}

void processIndexLipp() {
    // Placeholder for "lipp"
    spdlog::info("Processing index: LIPP");
}

int main(int argc, char* argv[]) {
    // bool index_set = false;
    auto config = parse_args(argc, argv);

    // display the config
    display_config(config);

    //std::vector<int> integers = readIntegersFromFile(config.data_file);
    std::vector<std::string> integerStrings = readIntegersAsStringFromFile(config.data_file);
    // Call the respective function based on the index value
    if (config.index == "alex") {
        processIndexAlex();
    } else if (config.index == "lipp") {
        processIndexLipp();
    }

    //for (int i : integers) {
    //std::cout << i << std::endl;
    //}
    for (const std::string& str : integerStrings) {
        std::cout << str << std::endl;
    }

    return 0;
}
