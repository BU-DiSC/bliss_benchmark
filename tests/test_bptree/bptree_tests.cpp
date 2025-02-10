#include "bliss_index_tests.h"

class BPTreeTest : public BlissIndexTest {};

TEST_F(BPTreeTest, TestBPTree_Sorted) {
    index.reset(new bliss::BlissBPTreeIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    for (auto key : data) {
        EXPECT_TRUE(index->get(key));
    }
}