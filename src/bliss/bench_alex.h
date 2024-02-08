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

    VALUE_TYPE get(KEY_TYPE key) { return _index.find(key); }

    void put(KEY_TYPE key, VALUE_TYPE value) { _index.insert(key, value); }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_ALEX
