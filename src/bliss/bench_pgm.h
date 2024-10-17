#ifndef BLISS_BENCH_PGM
#define BLISS_BENCH_PGM

#include <vector>

#include "bliss/bliss_index.h"
#include "include/pgm/pgm_index_dynamic.hpp"


namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissPGMIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    // pgm::PGMIndex<int, epsilon> _index();
    pgm::DynamicPGMIndex<KEY_TYPE, VALUE_TYPE> _index;
    BlissPGMIndex() : _index(){};

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        for (const auto& pair : values) {
            this->put(pair.first, pair.second);
        }
    }

    bool get(KEY_TYPE key) override {
        auto it = _index.find(key);
        return it != _index.end();
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override { _index.insert_or_assign(key, value); }

    void end_routine() override {}
};

}  // namespace bliss

#endif  // !BLISS_BENCH_PGM
