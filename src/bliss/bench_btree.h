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
        // Sort the values by key before bulk loading
        std::sort(values.begin(), values.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        // Insert sorted values into the B+ tree
        for (const auto& pair : values) {
            _index.insert(pair);
        }
    }

    bool get(KEY_TYPE key) override {
        auto it = _index.find(key);
        return it != _index.end();
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        _index[key] = value;
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE
