#ifndef BLISS_BENCH_SKIPLIST
#define BLISS_BENCH_SKIPLIST

#include <vector>

#include "bliss/bliss_index.h"
#include "skip_list.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissSkipListIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    goodliffe::skip_list<KEY_TYPE> _index;
    BlissSkipListIndex() : _index() {};

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        for (const auto& key : values) {
            put(key.first, key.second);
        }
    }

    bool get(KEY_TYPE key) override { return _index.find(key) != _index.end(); }

    uint32_t * get(KEY_TYPE start, KEY_TYPE end) override {
        throw std::runtime_error("Not implemented");
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override { _index.insert(key); }

    void end_routine() override {}
};

}  // namespace bliss

#endif  // !BLISS_BENCH_SKIPLIST
