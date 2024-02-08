#ifndef BLISS_BENCH_ALEX
#define BLISS_BENCH_ALEX

#include <alex.h>

#include "bliss/bliss_index.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissAlexIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    alex::Alex<KEY_TYPE, VALUE_TYPE> _index;
    BlissAlexIndex() { _index = alex::Alex<KEY_TYPE, VALUE_TYPE>(); }

    bool get(KEY_TYPE key) override {
        auto it = _index.find(key);
        return it != _index.end();
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        _index.insert(key, value);
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_ALEX
