#ifndef BLISS_BENCH_IMPRINTS
#define BLISS_BENCH_IMPRINTS

#include <vector>

#include "bliss/bliss_index.h"
#include <column_imprints.h>
#include <boost/type_index.hpp>

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissImprintsIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    BlissImprintsIndex(int blocksize = 64, int maxbins = 64) : blocksize_(blocksize), maxbins_(maxbins) {
        column = (Column *) malloc(sizeof(Column));
        strcpy(column->type_name, boost::typeindex::type_id<VALUE_TYPE>().pretty_name().c_str());
        // std::cout << boost::typeindex::type_id<VALUE_TYPE>().pretty_name() << std::endl;
        // printf("typename is: %s\n", column->type_name);
        if (strcmp(column->type_name, "tinyint") == 0 || strcmp(column->type_name, "boolean") == 0) {
            column->coltype  = TYPE_bte;
            column->min.bval = 127;
            column->max.bval = -127;
        } else if (strcmp(column->type_name, "char") == 0 || strcmp(column->type_name,"smallint")== 0 || strcmp(column->type_name, "short")== 0) {
            column->coltype  = TYPE_sht;
            column->min.sval = 32767;
            column->max.sval = -32767;
        } else if (strcmp(column->type_name, "decimal") == 0 || strcmp(column->type_name, "int") == 0 || strcmp(column->type_name, "date") == 0) {
            column->coltype  = TYPE_int;
            column->min.ival = INT_MAX;
            column->max.ival = INT_MIN;
        } else if (strcmp(column->type_name, "long") == 0 || strcmp(column->type_name, "long int") == 0) {
            column->coltype  = TYPE_lng;
            column->min.lval = LONG_MAX;
            column->max.lval = LONG_MIN;
        } else if (strcmp(column->type_name, "float") == 0 || strcmp(column->type_name, "real") == 0) {
            column->coltype= TYPE_flt;
            column->min.fval = FLT_MAX;
            column->max.fval = FLT_MIN;
        } else if (strcmp(column->type_name, "double") == 0 ) {
            column->coltype  = TYPE_dbl;
            column->min.dval = DBL_MAX;
            column->max.dval = -DBL_MAX;
        } else if (strcmp(column->type_name, "oid") == 0 || strcmp(column->type_name, "unsigned long") == 0) {
            column->coltype  = TYPE_oid;
            column->min.ulval = ULONG_MAX;
            column->max.ulval = 0;
        } else {
            printf("error: type [%s] not supported\n", column->type_name);
            std::runtime_error("[column imprints]: type not supported");
        }
        std::cout << "column initiated" << std::endl;
    };

    
    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        // expects the pairs to be pre-sorted before performing bulk load
        // this->_index.bulk_load(values.begin(), values.end());
        // binning()
        column->col = (char *)new VALUE_TYPE[values.size()];
        for(size_t i = 0; i < values.size(); i++) {
            ((VALUE_TYPE *)column->col)[i] = values[i].second;
            // std::cout << i << " " << values[i].second << std::endl;
        }
        const int stride[14]= { 0,0,0,1,2,0,4,8,0,0,4,8,8,0};
        int vpp = PAGESIZE/stride[column->coltype];
        if (vpp == 0) {
            printf("rows per pages is 0\n");
            std::runtime_error("rows per pages is 0");
            // return -1;
        }
        int pages = column->colcount/vpp + 1;
        if (pages > MAX_IMPS) {
            printf("there are too many pages %ld\n", pages);
            std::runtime_error("column imprints: too many pages");
            // return -1;
        }
        column->typesize = stride[column->coltype];
        column->colcount = values.size();
        index = create_imprints(column, blocksize_, maxbins_, 1);
    }

    unsigned int * get(VALUE_TYPE low, VALUE_TYPE high) {
        ValRecord low_, high_;
        switch (column->coltype) {
            case TYPE_bte:
                low_.bval = low;
                high_.bval = high;
                break;
            case TYPE_sht:
                low_.sval = low;
                high_.sval = high;
                break;
            case TYPE_int:
                low_.ival = low;
                high_.ival = high;
                // setqueryrange(ival);
                break;
            case TYPE_lng:
                low_.lval = low;
                high_.lval = high;
                break;
            case TYPE_oid:
                low_.ulval = low;
                high_.ulval = high;
                // setqueryrange(ulval);
                break;
            case TYPE_flt:
                low_.fval = low;
                high_.fval = high;
                // setqueryrange(fval);
                break;
            case TYPE_dbl:
                low_.dval = low;
                high_.dval = high;
                // setqueryrange(dval);
        }
        uint32_t *result_data = new uint32_t[(column->colcount + 31) / 32];
        memset(result_data, 0, sizeof(uint32_t) * ((column->colcount + 31) / 32));
        auto dummy = usec();
        imprints_simd_scan(column, index, low_, high_, &dummy, result_data);
        dummy = usec();
        return result_data;
    }

    bool get(KEY_TYPE key) override { return false; }
    
    void put(KEY_TYPE key, VALUE_TYPE value) {
        std::runtime_error("Column Imprints does not support put(key, value).");
        return;
    }

    void end_routine() override {}
  private:
    Column *column;
    Imprints_index *index;
    int blocksize_, maxbins_;
};

}  // namespace bliss

#endif  // !BLISS_BENCH_BTREE
