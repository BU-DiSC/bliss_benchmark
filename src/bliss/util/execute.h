// #ifndef BLISS_EXECUTE_H
// #define BLISS_EXECUTE_H
// #include <iostream>
// #include <random>
// #include <vector>

// #include "bliss/bliss_index.h"

// typedef unsigned long key_type;
// typedef unsigned long value_type;

// namespace bliss {
// namespace utils {
// namespace executor {
// void execute_inserts(bliss::BlissIndex<key_type, value_type> &tree,
//                      std::vector<key_type>::iterator &start,
//                      std::vector<key_type>::iterator &end, int seed = 0) {
//     spdlog::trace("Executing Inserts");
//     std::mt19937 gen(seed);
//     std::uniform_int_distribution<value_type> dist(0, 1);

//     auto num_keys = end - start;
//     for (auto &curr = start; curr != end; ++curr) {
//         tree.put(*curr, std::round(dist(gen) * num_keys));
//     }
// }

// void execute_non_empty_reads(bliss::BlissIndex<key_type, value_type> &tree,
//                              std::vector<key_type> &data, int num_reads,
//                              int seed = 0) {
//     std::mt19937 gen(seed);
//     std::uniform_int_distribution<int> dist(0, 1);

//     size_t key_idx;
//     for (auto blank = 0; blank < num_reads; blank++) {
//         key_idx = std::round(dist(gen) * (data.size() - 1));
//         tree.get(data.at(key_idx));
//     }
// }

// }  // namespace executor
// }  // namespace utils
// }  // namespace bliss
// #endif



#ifndef BLISS_EXECUTE_H
#define BLISS_EXECUTE_H

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <spdlog/spdlog.h>

#include "bliss/bliss_index.h"

typedef unsigned long key_type;
typedef unsigned long value_type;

namespace bliss {
namespace utils {
namespace executor {

void execute_inserts(bliss::BlissIndex<key_type, value_type> &tree,
                     std::vector<key_type>::iterator start,
                     std::vector<key_type>::iterator end, int seed = 0) {
    spdlog::trace("Executing Inserts");
    std::mt19937 gen(seed);
    auto num_keys = std::distance(start, end);
    std::uniform_int_distribution<value_type> dist(0, num_keys - 1);

    for (auto curr = start; curr != end; ++curr) {
        tree.put(*curr, dist(gen));
    }
}

void execute_non_empty_reads(bliss::BlissIndex<key_type, value_type> &tree,
                             const std::vector<key_type> &data, int num_reads,
                             int seed = 0) {
    spdlog::trace("Executing Non-Empty Reads");
    std::mt19937 gen(seed);
    std::uniform_int_distribution<size_t> dist(0, data.size() - 1);

    for (int i = 0; i < num_reads; ++i) {
        size_t key_idx = dist(gen);
        tree.get(data.at(key_idx));
    }
}

}  // namespace executor
}  // namespace utils
}  // namespace bliss

#endif
