#ifndef BLISS_BENCH_ALEX
#define BLISS_BENCH_ALEX

#include <alex.h>

#include <vector>

#include "bliss/bliss_index.h"
#include "spdlog/spdlog.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissAlexIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    alex::Alex<KEY_TYPE, VALUE_TYPE> _index;
    BlissAlexIndex() : _index() {};

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        this->_index.bulk_load(values.data(), values.size());
    }

    bool get(KEY_TYPE key) override {
        auto it = _index.find(key);
        return it != _index.end();
    }

    bool get(KEY_TYPE start, KEY_TYPE end) override {
        auto it_start = _index.lower_bound(start);
        auto it_end = _index.lower_bound(end);
        // Return true if there are any elements in the range
        return it_start != _index.end() && it_start != it_end;
    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {
        _index.insert(key, value);
    }

    void end_routine() override {
        auto stats = this->_index.stats_;
        spdlog::info("num_keys = {}", stats.num_keys);
        spdlog::info("num_model_nodes = {}", stats.num_model_nodes);
        spdlog::info("num_data_nodes = {}", stats.num_data_nodes);
        spdlog::info("num_expand_and_scales = {}", stats.num_expand_and_scales);
        spdlog::info("num_expand_and_retrains = {}",
                     stats.num_expand_and_retrains);
        spdlog::info("num_downward_splits = {}", stats.num_downward_splits);
        spdlog::info("num_sideways_splits = {}", stats.num_sideways_splits);
        spdlog::info("num_model_node_expansions = {}",
                     stats.num_model_node_expansions);
        spdlog::info("num_model_node_splits = {}", stats.num_model_node_splits);
        spdlog::info("num_downard_split_keys = {}",
                     stats.num_downward_split_keys);
        spdlog::info("num_sideways_split_keys = {}",
                     stats.num_sideways_split_keys);
        spdlog::info("num_model_node_expansion_pointers = {}",
                     stats.num_model_node_expansion_pointers);
        spdlog::info("num_model_node_split_pointers = {}",
                     stats.num_model_node_split_pointers);
        spdlog::info("num_node_lookups = {}", stats.num_node_lookups);
        spdlog::info("num_lookups = {}", stats.num_lookups);
        spdlog::info("num_inserts = {}", stats.num_inserts);
        spdlog::info("splitting_time = {}", stats.splitting_time);
        spdlog::info("cost_computation_time = {}", stats.cost_computation_time);

        spdlog::info("max_fanout = {}",
                     this->_index.derived_params_.max_fanout);
        spdlog::info("max_data_node_slots = {}",
                     this->_index.derived_params_.max_data_node_slots);
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_ALEX
