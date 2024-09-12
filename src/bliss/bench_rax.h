#ifndef BLISS_BENCH_RAX
#define BLISS_BENCH_RAX

#include <vector>

#include "bliss/bliss_index.h"

#include "rax.h"


namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissRaxIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    rax* _index;
    BlissRaxIndex() {
        _index = raxNew();
    };

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        for (const auto& pair : values) {
            put(pair.first, pair.second);
        }
    }

    bool get(KEY_TYPE key) override { 
        std::string keyStr = std::to_string(key);
        void* result = raxFind(_index, (unsigned char*)keyStr.c_str(), keyStr.size());
        return (result == raxNotFound) ? false : true;
     }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        std::string keyStr = std::to_string(key);
        raxInsert(_index, (unsigned char*)keyStr.c_str(), keyStr.size(), (void*)new VALUE_TYPE(value), NULL);
    }

    void end_routine() override {
        raxFree(_index);
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_RAX
