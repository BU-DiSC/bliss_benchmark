#include "bliss_index_tests.h"

class SkipListTest : public BlissIndexTest {};


TEST_F(SkipListTest, TestSkipList_Sanity) {
    index.reset(new bliss::BlissSkipListIndex<key_type, key_type>());
    std::vector<key_type> data;
    int key = 100'000;
    int value = 123'456;
    index->put(key, value);
    EXPECT_TRUE(index->get(key));
}


TEST_F(SkipListTest, TestSkipList_Sorted) {
    index.reset(new bliss::BlissSkipListIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    for (auto key : data) {
        EXPECT_TRUE(index->get(key));
    }
}


TEST_F(SkipListTest, TestSkipList_Random) {
    index.reset(new bliss::BlissSkipListIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys, false);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    for (auto key : data) {
        EXPECT_TRUE(index->get(key));
    }
}