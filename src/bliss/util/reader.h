#ifndef BLISS_UTIL_READER
#define BLISS_UTIL_READER

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

namespace bliss {

template <typename KEY_TYPE>
std::vector<KEY_TYPE> read_file(const char *filename) {
    std::ifstream fid(filename);

    if (!fid.is_open()) {
        spdlog::error("Error opening file {}", filename);
        exit(EXIT_FAILURE);
    }

    std::vector<KEY_TYPE> data;
    std::string line;

    while (std::getline(fid, line)) {
        KEY_TYPE key = std::stoul(line);
        data.push_back(key);
    }

    return data;
}

template <typename KEY_TYPE>
std::vector<KEY_TYPE> read_file_binary(const char *filename) {
    std::ifstream fid(filename, std::ios::binary);

    if (!fid.is_open()) {
        spdlog::error("Error opening file {}", filename);
        exit(EXIT_FAILURE);
    }

    fid.seekg(0, std::ios::end);
    std::streampos fileSize = fid.tellg();
    fid.seekg(0, std::ios::beg);
    std::vector<KEY_TYPE> data(fileSize / sizeof(KEY_TYPE));
    fid.read(reinterpret_cast<char *>(data.data()), fileSize);

    return data;
}

}  // namespace bliss

#endif
