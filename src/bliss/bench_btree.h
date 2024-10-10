#ifndef BLISS_BENCH_BTREE
#define BLISS_BENCH_BTREE

#include <vector>

#include "bliss/bliss_index.h"
#include "tlx/container/btree_map.hpp"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissBTreeIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    tlx::btree_map<KEY_TYPE, VALUE_TYPE> _index;
    BlissBTreeIndex() : _index(){};

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        this->_index.bulk_load(values.begin(), values.end());
    }

    bool get(KEY_TYPE key) override {
        std::cout << key << " <- get " << this->_index.exists(key) << std::endl;
        return this->_index.exists(key); }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        std::cout << key << " <- put" << std::endl;
        this->_index.insert(std::make_pair(key, value));
    }

    void end_routine() override {}
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE
