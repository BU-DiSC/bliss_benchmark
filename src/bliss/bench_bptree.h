#ifndef BLISS_BENCH_BTREE
#define BLISS_BENCH_BTREE

#include <bptree/container/btree_map.hpp>
#include <vector>

#include "bliss/bliss_index.h"
#include "spdlog/spdlog.h"

// BP-Tree follows the tlx::btree_map implementation
namespace bliss {
template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissBPTreeIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    tlx::btree_map<KEY_TYPE, VALUE_TYPE> _index;
    BlissBPTreeIndex() : _index() {};

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        this->_index.bulk_load(values.begin(), values.end());
    }

    bool get(KEY_TYPE key) override { return this->_index.exists(key); }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        this->_index.insert(std::make_pair(key, value));
    }

    void end_routine() override {}
};
}  // namespace bliss

#endif  // !BLISS_BENCH_BPTREE