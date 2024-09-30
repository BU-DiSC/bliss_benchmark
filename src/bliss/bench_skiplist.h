#ifndef BLISS_BENCH_SKIPLIST
#define BLISS_BENCH_SKIPLIST

#include <vector>

#include "bliss/bliss_index.h"
#include "skiplist/skiplist.hpp"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissSkipListIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    skiplist::skiplist<KEY_TYPE, VALUE_TYPE> _index;
    BlissSkipListIndex() : _index(){};

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        for (const auto& pair : values) {
            _index.insert(pair.first, pair.second);
        }
    }

    bool get(KEY_TYPE key) override { return _index.find(key) != _index.end(); }

    void put(KEY_TYPE key, VALUE_TYPE value) override { _index.insert(key, value); }

    void end_routine() override {}
};

}  // namespace bliss

#endif  // !BLISS_BENCH_SKIPLIST
