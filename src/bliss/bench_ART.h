#ifndef BLISS_BENCH_ART
#define BLISS_BENCH_ART

#include "ART.h"

#include <vector>

#include "bliss/bliss_index.h"
#include "spdlog/spdlog.h"


namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissARTIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
   static constexpr size_t KEY_SIZE = sizeof(KEY_TYPE);
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
        uint8_t ARTkey[KEY_SIZE];
        ART::loadKey(key, ARTkey);

        uint8_t depth = 0;
        ART::Node* leaf = ART::lookup(_index, ARTkey, KEY_SIZE, depth, KEY_SIZE);
        return ART::isLeaf(leaf) && ART::getLeafValue(leaf) == key;
     }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        uint8_t ARTkey[KEY_SIZE];
        ART::loadKey(key, ARTkey);

        uint8_t depth = 0;
        ART::insert(_index, &_index, ARTkey, depth, key, KEY_SIZE);
    }

    void end_routine() override {}
};

}  // namespace bliss

#endif  // !BLISS_BENCH_ART
