#ifndef BLISS_BENCH_ART
#define BLISS_BENCH_ART

#include <ART.h>

#include <vector>

#include "bliss/bliss_index.h"
#include "spdlog/spdlog.h"


namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissARTIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
   ART::Node* _index;
    BlissARTIndex() {
        _index = nullptr;
    };

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        for (const auto& pair : values) {
            put(pair.first, pair.second);
        }
    }

    bool get(KEY_TYPE key) override { 
        uint8_t ARTkey[8];
        ART::loadKey(key, ARTkey);

        ART::Node* leaf = ART::lookup(_index, ARTkey, 8, 0, 8);
        return ART::isLeaf(leaf) && ART::getLeafValue(leaf) == key;
     }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        uint8_t ARTkey[8];
        ART::loadKey(key, ARTkey);
        ART::insert(_index, &_index, ARTkey, 0, key, 8);
    }

    void end_routine() override {}
};

}  // namespace bliss

#endif  // !BLISS_BENCH_ART
