#ifndef BLISS_BENCH_BYTESLICE
#define BLISS_BENCH_BYTESLICE

#include <vector>

#include "bliss/bliss_index.h"
#include "src/byteslice.h"
#include <string>

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissByteSliceIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    BlissByteSliceIndex(int numrows = 1000000, int numbits = 32) {
        byteslice_ = new byteslice::ByteSlice(numrows, numbits);
    };

    ~BlissByteSliceIndex()  {
        delete byteslice_;
    }
    
    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        int i = 0;
        for(auto x: values) {
            byteslice_->setTuple(i, static_cast<unsigned long long>(x.second));
        }
    }


    bool get(VALUE_TYPE start, VALUE_TYPE end) {
        uint32_t * result = byteslice_->query(static_cast<unsigned long long>(start), static_cast<unsigned long long>(end));
        bool check_ = result != nullptr;
        if(check_) delete result;
        return check_;
    }

    bool get(KEY_TYPE key) override { 
        std::runtime_error("ByteSlice does not support get(key, value).");
        return false; 
    }
    
    void put(KEY_TYPE key, VALUE_TYPE value) {
        std::runtime_error("ByteSlice does not support put(key, value).");
        return;
    }

    void end_routine() override {}
  private:
    byteslice::ByteSlice *byteslice_;
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE