#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

bool validateIntRange(int value, int min, int max) {
    return (value >= min && value <= max);
}

bool validateFloatRange(float value, float min, float max) {
    return (value >= min && value <= max);
}

bool validateIndex(const std::string& value) {
    return (value == "alex" || value == "lipp");
}

template <typename T>
T parseArgument(const char* arg) {
    std::istringstream iss(arg);
    T value;
    iss >> value;
    if (iss.fail() || !iss.eof()) {
        throw std::invalid_argument("Invalid argument format");
    }
    return value;
}

// Separate functions for each index
void processIndexAlex() {
    // Placeholder for "alex"
    std::cout << "Processing index: ALEX" << std::endl;
}

void processIndexLipp() {
    // Placeholder for "lipp"
    std::cout << "Processing index: LIPP" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string data_file;
    bool data_file_set = false;
    int preload_factor = -1;
    int raw_write_factor = -1;
    int raw_read_factor = -1;
    float read_write_ratio = -1.0f;
    std::string index;
    bool index_set = false;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--data_file" && i + 1 < argc) {
                data_file = argv[++i];
                data_file_set = true;
            } else if (arg == "--preload_factor" && i + 1 < argc) {
                preload_factor = parseArgument<int>(argv[++i]);
                if (!validateIntRange(preload_factor, 0, 100)) {
                    throw std::runtime_error("Preload factor must be between 0 and 100");
                }
            } else if (arg == "--raw_write_factor" && i + 1 < argc) {
                raw_write_factor = parseArgument<int>(argv[++i]);
                if (!validateIntRange(raw_write_factor, 0, 100)) {
                    throw std::runtime_error("Raw write factor must be between 0 and 100");
                }
            } else if (arg == "--raw_read_factor" && i + 1 < argc) {
                raw_read_factor = parseArgument<int>(argv[++i]);
                if (!validateIntRange(raw_read_factor, 0, 100)) {
                    throw std::runtime_error("Raw read factor must be between 0 and 100");
                }
            } else if (arg == "--read_write_ratio" && i + 1 < argc) {
                read_write_ratio = parseArgument<float>(argv[++i]);
                if (!validateFloatRange(read_write_ratio, 0.0f, 1.0f)) {
                    throw std::runtime_error("Read write ratio must be between 0.0 and 1.0");
                }
            } else if (arg == "--index" && i + 1 < argc) {
                index = argv[++i];
                if (!validateIndex(index)) {
                    throw std::runtime_error("Index must be either 'alex' or 'lipp'");
                }
                index_set = true;
            } else {
                throw std::runtime_error("Unknown or incomplete argument: " + arg);
            }
        }

        if (!data_file_set || preload_factor == -1 || raw_write_factor == -1 || 
            raw_read_factor == -1 || read_write_ratio == -1.0f || !index_set) {
            throw std::runtime_error("Not all required arguments were provided");
        }

        // Call the respective function based on the index value
        if (index == "alex") {
            processIndexAlex();
        } else if (index == "lipp") {
            processIndexLipp();
        }

        // Display the values 
        std::cout << "Data File: " << data_file << std::endl;
        std::cout << "Preload Factor: " << preload_factor << std::endl;
        std::cout << "Raw Write Factor: " << raw_write_factor << std::endl;
        std::cout << "Raw Read Factor: " << raw_read_factor << std::endl;
        std::cout << "Read Write Ratio: " << read_write_ratio << std::endl;
        std::cout << "Index: " << index << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Usage: --data_file <path> --preload_factor <int> --raw_write_factor <int> --raw_read_factor <int> --read_write_ratio <float> --index <alex|lipp>" << std::endl;
        return 1;
    }

    return 0;
}
