#include "bliss_index_tests.h"

class BTreeTest : public BlissIndexTest {};

TEST_F(BTreeTest, TestBtree_Sanity) {
    index.reset(new bliss::BlissBTreeIndex<key_type, key_type>());
    std::vector<key_type> data;
    int key = 100;
    int value = 123;
    index->put(key, value);
    EXPECT_TRUE(index->get(key));
}

TEST_F(BTreeTest, TestBtree_Sorted) {
    index.reset(new bliss::BlissBTreeIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    for (auto key : data) {
        EXPECT_TRUE(index->get(key));
    }
}

TEST_F(BTreeTest, TestBTree_Random) {
    index.reset(new bliss::BlissBTreeIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys, false);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    for (auto key : data) {
        EXPECT_TRUE(index->get(key));
    }
}

TEST_F(BTreeTest, TestBTree_RangeQueries) {
    index.reset(new bliss::BlissBTreeIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);
    
    EXPECT_TRUE(index->get(data[5], data[10]));
}