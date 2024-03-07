#ifndef BLISS_BENCH_BTREE
#define BLISS_BENCH_BTREE

#include "tlx/container/btree_map.hpp"
#include <vector>
#include <algorithm>
#include "bliss/bliss_index.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissBTreeIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    tlx::btree_map<KEY_TYPE, VALUE_TYPE> _index;
    BlissBTreeIndex() = default;

    // bulk load
    void preload(std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // Value has been pre-sorted
        // Insert sorted values into the B+ tree
        _index.bulk_load(values.begin(), values.end());
    }

    bool get(KEY_TYPE key) override {
        auto it = _index.find(key);
        return it != _index.end();
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
       _index.insert(std::make_pair(key, value));
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE
