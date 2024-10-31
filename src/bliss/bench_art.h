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
        std::cout << key << "" << std::endl;
        for (size_t i = 0; i < KEY_SIZE; ++i) {
            std::cout << static_cast<int>(ARTkey[i]) << " ";
        }
        std::cout << std::endl;
        size_t testing = 2;
        ART::ArtNode* leaf =
            ART::lookup(_index, ARTkey, testing, 0, KEY_SIZE);
        std::cout << ART::getLeafValue(leaf) << " <- leaf value" << std::endl << std::endl;
        return leaf != nullptr && ART::isLeaf(leaf);
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        uint8_t ARTkey[KEY_SIZE];
        ART::loadKey(key, ARTkey);
        // function signature for insert:
        // void insert(ArtNode* ArtNode, ArtNode** nodeRef, uint8_t key[],
        // unsigned depth, uintptr_t value, unsigned maxKeyLength)
        ART::insert(_index, &_index, ARTkey, 0, value, KEY_SIZE);
    }

    void end_routine() override {}
};

}  // namespace bliss

#endif