#include "bliss_index_tests.h"

class LevelDBTest : public BlissIndexTest {};

TEST_F(LevelDBTest, TestLevelDB_Sanity) {
    index.reset(new bliss::BlissLevelDBIndex<key_type, key_type>());
    std::vector<key_type> data;
    int key = 100;
    int value = 123;
    index->put(key, value);
    EXPECT_TRUE(index->get(key));
}

TEST_F(LevelDBTest, TestLevelDB_RangeQuery) {
    index.reset(new bliss::BlissLevelDBIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    key_type start_key = data.front();
    key_type end_key = start_key + (data.back() - data.front()) / 4;
    
    try {
        index->get(start_key, end_key);
        SUCCEED();
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ("Not implemented", e.what());
    }
}

TEST_F(LevelDBTest, TestLevelDB_Sorted) {
    index.reset(new bliss::BlissLevelDBIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    for (auto key : data) {
        EXPECT_TRUE(index->get(key));
    }
}

TEST_F(LevelDBTest, TestLevelDB_Random) {
    index.reset(new bliss::BlissLevelDBIndex<key_type, key_type>());
    std::vector<key_type> data;
    GenerateData(data, num_keys, false);

    auto insert_start = data.begin();
    auto insert_end = data.end();
    executor::execute_inserts(*index, insert_start, insert_end);

    for (auto key : data) {
        EXPECT_TRUE(index->get(key));
    }
}