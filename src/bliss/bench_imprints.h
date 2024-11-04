#ifndef BLISS_BENCH_IMPRINTS
#define BLISS_BENCH_IMPRINTS

#include <vector>

#include "bliss/bliss_index.h"
#include <column_imprints.h>
#include <boost/type_index.hpp>
#include <string>

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissImprintsIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    BlissImprintsIndex(int blocksize = 64, int maxbins = 64, std::string type_name = std::string("unsigned int")) : blocksize_(blocksize), maxbins_(maxbins) {
        imprints_ = new Imprints<VALUE_TYPE>(blocksize, maxbins, type_name);
        // std::cout << "column initiated" << std::endl;
    };

    ~BlissImprintsIndex()  {
        delete imprints_;
    }
    
    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        // this->_index.bulk_load(values.begin(), values.end());
        // binning()
        std::vector<VALUE_TYPE> vals;
        for(auto x: values) {
            vals.push_back(x.second);
        }
        imprints_->bulkload(vals);
    }


    unsigned int * get(VALUE_TYPE low, VALUE_TYPE high) {
        return imprints_->range_scan(low, high);
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
    Imprints<VALUE_TYPE> *imprints_;
    int blocksize_, maxbins_;
    std::string type_name;
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE
