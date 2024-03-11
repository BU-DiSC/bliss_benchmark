#ifndef BLISS_BENCH_LIPP
#define BLISS_BENCH_LIPP

#include <lipp.h>

#include "bliss/bliss_index.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissLippIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    LIPP<KEY_TYPE, VALUE_TYPE> _index;
    BlissLippIndex() : _index(){};

    void bulkload(std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        this->_index.bulk_load(values.data(), values.size());
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
