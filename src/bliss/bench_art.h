#ifndef BLISS_BENCH_ART
#define BLISS_BENCH_ART

#include <vector>

#include "ART.h"
#include "bliss/bliss_index.h"
#include "spdlog/spdlog.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissARTIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    ART::ArtNode* _index;
    static constexpr size_t KEY_SIZE = sizeof(KEY_TYPE);
    BlissARTIndex() { _index = nullptr; };

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        for (const auto pair : values) {
            put(pair.first, pair.second);
        }
    }

    bool get(KEY_TYPE key) override {
        uint8_t ARTkey[KEY_SIZE];
        ART::loadKey(key, ARTkey);
        ART::ArtNode* leaf =
            ART::lookup(_index, ARTkey, KEY_SIZE, 0, KEY_SIZE);
        return leaf != nullptr && ART::isLeaf(leaf);
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        uint8_t ARTkey[KEY_SIZE];
        ART::loadKey(key, ARTkey);
        ART::insert(_index, &_index, ARTkey, 0, key, KEY_SIZE);
    }

    void end_routine() override {}
};

}  // namespace bliss

#endif