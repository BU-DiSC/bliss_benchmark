#include "bliss_index_tests.h"

class ImprintsTest : public BlissIndexTest {};

TEST_F(ImprintsTest, TestImprint_Random) {
    index.reset(new bliss::BlissImprintsIndex<size_t, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys, false);
    std::vector < std::pair<size_t, key_type> > bulkload_data;
    for (size_t i = 0; i < data.size(); i ++) {
        bulkload_data.push_back(std::make_pair(i, data[i]));
    }
    index->bulkload(bulkload_data);
    auto minimum = 0;
    // auto insert_start = data.begin();
    // auto insert_end = data.end();
    // executor::execute_inserts(*index, insert_start, insert_end);

    for (size_t key = 0; key < num_keys; key += std::max(1,num_keys / 20)) {
        uint32_t *result = nullptr;
        result = index->get(minimum, data[key]);
        for(size_t i = 0; i < num_keys; i ++) {
            // if((data[i] <= data[key] && data[i] > minimum) != ((result[i / 32] >> (i & 31)) & 1u)) {
            //     std::cout << minimum << " < " << data[i] << " <= " << data[key] << " " << ((result[i / 32] >> (i & 31)) & 1u) << " :: idx = " << i << std::endl;
            // }
            if((result[i / 32] >> (i & 31)) & 1u) {
                EXPECT_TRUE(data[i] <= data[key] && data[i] > minimum);
            } else {
                EXPECT_FALSE(data[i] <= data[key] && data[i] > minimum);
            }
        }
        free(result);
    }
}