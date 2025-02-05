#include "bliss_index_tests.h"

class BPTreeTest : public BlissIndexTest {};

TEST_F(BPTreeTest, TestBPTree_Sorted) {
    index.reset(new bliss::BlissBPTreeIndex<key_type, key_type>());
}