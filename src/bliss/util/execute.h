#ifndef BLISS_EXECUTE_H
#define BLISS_EXECUTE_H

#include <spdlog/spdlog.h>

#include <cmath>
#include <iostream>
#include <random>
#include <vector>
#include <algorithm>

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
    std::uniform_int_distribution<size_t> dist(0, num_keys - 1);

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

void execute_range_queries(bliss::BlissIndex<key_type, value_type> &tree,
                          const std::vector<key_type> &data, int num_queries,
                          double selectivity = 0.01, int seed = 0) {
    spdlog::trace("Executing Range Queries");
    std::mt19937 gen(seed);
    std::uniform_int_distribution<size_t> key_dist(0, data.size() - 1);
    
    key_type data_range = *std::max_element(data.begin(), data.end()) - 
                          *std::min_element(data.begin(), data.end());
    key_type avg_range_size = static_cast<key_type>(data_range * selectivity);
    
    for (int i = 0; i < num_queries; ++i) {
        size_t start_idx = key_dist(gen);
        key_type start_key = data.at(start_idx);
        key_type end_key = start_key + avg_range_size;
        
        tree.get(start_key, end_key);
    }
}

}  // namespace executor
}  // namespace utils
}  // namespace bliss
#endif
