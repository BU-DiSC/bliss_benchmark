#include <fstream>
#include <iostream>

#include "bliss/util/reader.h"

// read test file
std::vector<int> read_data_file(const std::string& filename) {
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

