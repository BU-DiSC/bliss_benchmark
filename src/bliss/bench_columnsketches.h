#ifndef BLISS_BENCH_COLUMNSKETCHES
#define BLISS_BENCH_COLUMNSKETCHES
#ifdef COMPILE_COLUMNSKETCHES
#include <vector>

#include "bliss/bliss_index.h"
#include "column_sketches.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissColumnSketchesIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    BlissColumnSketchesIndex() {
        columnsketches_ = new ColumnSketchesInteger();
    };

    ~BlissColumnSketchesIndex()  {
        delete columnsketches_;
    }
    
    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        std::vector<int32_t> vals;
        for(auto x: values) {
            vals.push_back(x.second);
        }
        columnsketches_->create(&vals[0], vals.size(), vals.size());
    }

    bool get(VALUE_TYPE start, VALUE_TYPE end) {
        uint32_t *res = columnsketches_->query_ge_lt(start, end);
        bool succeed = res != nullptr;
        if(succeed) delete res;
        return succeed;
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
    
    ColumnSketchesInteger *columnsketches_;
};

}  // namespace bliss
#endif
#endif  // !BLISS_BENCH_BTREE
