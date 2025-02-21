#ifndef BLISS_INDEX_TESTS_H
#define BLISS_INDEX_TESTS_H
#include <alex.h>
#include <gtest/gtest.h>
#include <lipp.h>
#include <spdlog/common.h>

#include <algorithm>
#include <cxxopts.hpp>
#include <iostream>
#include <random>
#include <string>

#include "bliss/bench_alex.h"
#include "bliss/bench_art.h"
#include "bliss/bench_btree.h"
#include "bliss/bench_leveldb.h"
#include "bliss/bench_lipp.h"
#include "bliss/bench_imprints.h"
#include "bliss/bench_skiplist.h"
#include "bliss/bench_pgm.h"
#include "bliss/bench_skiplist.h"
#include "bliss/bliss_index.h"
#include "bliss/util/args.h"
#include "bliss/util/config.h"
#include "bliss/util/execute.h"
#include "bliss/util/reader.h"
#include "bliss/util/timer.h"
#include "include/pgm/pgm_index_dynamic.hpp"

using namespace bliss::utils;

using key_type = unsigned long;
using value_type = unsigned long;

class BlissIndexTest : public testing::Test {
   protected:
    std::unique_ptr<bliss::BlissIndex<key_type, value_type>> index;
    std::string indexes[7] = {"alex", "lipp", "btree",  "skiplist",
                              "pgm",  "art",  "leveldb"};

    int num_keys = 100000;

    void SetUp() {}

    void GenerateData(std::vector<key_type> &data, int num_keys,
                      bool sorted = true) {
        for (int i = 0; i < num_keys; i++) {
            data.push_back(i);
        }
        if (!sorted) {
            std::random_device rd;
            std::mt19937 g(rd());
            std::shuffle(data.begin(), data.end(), g);
        }
    }
};

#endif