#ifndef BLISS_EXECUTE_H
#define BLISS_EXECUTE_H
#include <iostream>
#include <random>
#include <vector>

#include "bliss/bliss_index.h"

typedef unsigned long key_type;
typedef unsigned long value_type;

namespace bliss {
namespace utils {
namespace executor {
void execute_inserts(bliss::BlissIndex<key_type, value_type> &tree,
                     std::vector<key_type>::iterator &start,
                     std::vector<key_type>::iterator &end, int seed = 0) {
    spdlog::trace("Executing Inserts");
    std::mt19937 gen(seed);
    std::uniform_int_distribution<value_type> dist(0, 1);

    auto num_keys = end - start;
    for (auto &curr = start; curr != end; ++curr) {
        tree.put(*curr, std::round(dist(gen) * num_keys));
    }
}

void execute_non_empty_reads(bliss::BlissIndex<key_type, value_type> &tree,
                             std::vector<key_type> &data, int num_reads,
                             int seed = 0) {
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> dist(0, 1);

    size_t key_idx;
    for (auto blank = 0; blank < num_reads; blank++) {
        key_idx = std::round(dist(gen) * (data.size() - 1));
        tree.get(data.at(key_idx));
    }
}

}  // namespace executor
}  // namespace utils
}  // namespace bliss
#endif