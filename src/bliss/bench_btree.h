#ifndef BLISS_BENCH_BTREE
#define BLISS_BENCH_BTREE

#include "tlx/container/btree.hpp"
#include <vector>
#include "bliss/bliss_index.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissBTreeIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    tlx::btree_default_traits<KEY_TYPE, VALUE_TYPE> _index;
    BlissBTreeIndex() = default;

    void preload(std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        _index.bulk_load(values.begin(), values.end());
    }

    bool get(KEY_TYPE key) override {
        return _index.exists(key);
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        _index.insert(key, value);
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE
