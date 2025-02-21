#ifndef BLISS_BENCH_IMPRINTS
#define BLISS_BENCH_IMPRINTS

#include <vector>

#include "bliss/bliss_index.h"
#include "column_imprints.h"
#include <string>

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissImprintsIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    BlissImprintsIndex(int blocksize = 64, int maxbins = 64, std::string type_name = std::string("unsigned long")) : blocksize_(blocksize), maxbins_(maxbins) {
        imprints_ = new ColumnImprints::Imprints<VALUE_TYPE>(blocksize, maxbins, type_name);
    };

    ~BlissImprintsIndex()  {
        delete imprints_;
    }
    
    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        std::vector<VALUE_TYPE> vals;
        for(auto x: values) {
            vals.push_back(x.second);
        }
        imprints_->bulkload(vals);
    }


    bool get(VALUE_TYPE start, VALUE_TYPE end) {
        unsigned int * res = imprints_->range_scan(start, end);
        if(res != nullptr) {
            delete res;
            return true;
        } else
            return false;
    }

    uint32_t * get_bitmask(VALUE_TYPE start, VALUE_TYPE end) {
        return imprints_->range_scan(start, end);
    }

    bool get(KEY_TYPE key) override { 
        std::runtime_error("Column Imprints does not support get(key, value).");
        return false; 
    }
    
    void put(KEY_TYPE key, VALUE_TYPE value) {
        std::runtime_error("Column Imprints does not support put(key, value).");
        return;
    }

    void end_routine() override {}
  private:
    ColumnImprints::Imprints<VALUE_TYPE> *imprints_;
    int blocksize_, maxbins_;
    std::string type_name;
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE
