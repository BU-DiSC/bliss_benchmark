#ifndef BLISS_BENCH_BETREE
#define BLISS_BENCH_BETREE

#include "bepsilon/betree.h"
#include "bliss/bliss_index.h"

const size_t max_node_size = 64;  // default node size in number of entries
const size_t min_flush_size = max_node_size / 4;
const size_t cache_size = 4;  // in betree nodes

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissBeTreeIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    betree<KEY_TYPE, VALUE_TYPE> *_index;
    BlissBeTreeIndex() {
        one_file_per_object_backing_store ofpobs("betree_data/");
        swap_space sspace(&ofpobs, cache_size);

        _index = new betree<KEY_TYPE, VALUE_TYPE>(&sspace, max_node_size,
                                                  min_flush_size);
    };

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {}

    bool get(KEY_TYPE key) override {
        return _index->query(key) ? "true" : "false";
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        _index->insert(key, value);
    }

    void end_routine() override { delete _index; }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_LIPP
