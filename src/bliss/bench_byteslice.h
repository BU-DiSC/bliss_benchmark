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
        // std::cout << "column initiated" << std::endl;
        byteslice_ = new ByteSlice(numrows, numbits);
    };

    ~BlissByteSliceIndex()  {
        delete byteslice_;
    }
    
    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        // this->_index.bulk_load(values.begin(), values.end());
        // binning()
        // std::vector<VALUE_TYPE> vals;
        int i = 0;
        for(auto x: values) {
            byteslice_->setTuple(i, static_cast<unsigned long long>(x.second));
            // vals.push_back(x.second);
        }
        // imprints_->bulkload(vals);
    }


    unsigned int * get(VALUE_TYPE low, VALUE_TYPE high) {
        return byteslice_->query(static_cast<unsigned long long>(low), static_cast<unsigned long long>(high));
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
    ByteSlice *byteslice_;
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE