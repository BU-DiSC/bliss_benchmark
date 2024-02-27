#ifndef BLISS_BENCH_LIPP
#define BLISS_BENCH_LIPP

#include <lipp.h>

#include <random>

#include "bliss/bliss_index.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissLippIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    LIPP<KEY_TYPE, VALUE_TYPE> _index;
    BlissLippIndex() : _index(){};

    void preload(typename std::vector<KEY_TYPE>::iterator &start,
                 typename std::vector<KEY_TYPE>::iterator &end,
                 int seed = 0) override {
        auto num_keys = end - start;
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> distribution(0, 1);
        std::pair<KEY_TYPE, VALUE_TYPE> values[num_keys];
        size_t idx = 0;
        for (auto curr = start; curr != end; ++curr) {
            values[idx].first = *curr;
            values[idx].second = distribution(generator);
            idx++;
        }
        this->_index.bulk_load(values, num_keys);
    }

    bool get(KEY_TYPE key) override {
        return _index.exists(key) ? "true" : "false";
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        _index.insert(key, value);
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_LIPP
