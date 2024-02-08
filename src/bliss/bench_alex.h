#ifndef BLISS_BENCH_ALEX
#define BLISS_BENCH_ALEX

#include <alex.h>

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class IndexAlex {
   public:
    alex::Alex<KEY_TYPE, VALUE_TYPE> _index;

    IndexAlex();

};

}  // namespace bliss

#endif  // !BLISS_BENCH_ALEX
