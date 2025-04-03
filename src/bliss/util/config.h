#ifndef BLISS_CONFIG_H
#define BLISS_CONFIG_H

#include <spdlog/common.h>

#include <string>

namespace bliss {
namespace utils {
namespace config {
struct BlissConfig {
    std::string data_file;
    double preload_factor;
    double write_factor;
    double read_factor;
    double mixed_read_write_ratio;
    int seed;
    int verbosity;
    std::string index;
    std::string file_type;
    bool use_preload;
    double range_query_factor = 0.0;
    double selectivity_factor = 0.01;
};

void display_config(BlissConfig config) {
    spdlog::trace("Data File: {}", config.data_file);
    spdlog::trace("Preload Factor: {}", config.preload_factor);
    spdlog::trace("Write Factor: {}", config.write_factor);
    spdlog::trace("Read Factor: {}", config.read_factor);
    spdlog::trace("Read Write Ratio: {}", config.mixed_read_write_ratio);
    spdlog::trace("Verbosity {}", config.verbosity);
    spdlog::trace("Index: {}", config.index);
    spdlog::trace("File type: {}", config.file_type);
    spdlog::trace("Use Preload: {}", config.use_preload);
    spdlog::trace("Range Query Factor: {}", config.range_query_factor);
    spdlog::trace("Selectivity Factor: {}", config.selectivity_factor);
}
}  // namespace config
}  // namespace utils
}  // namespace bliss

#endif