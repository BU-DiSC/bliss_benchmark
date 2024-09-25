#ifndef CONFIG_H
#define CONFIG_H

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
}
}  // namespace config
}  // namespace utils
}  // namespace bliss

#endif