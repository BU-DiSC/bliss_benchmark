#ifndef BLISS_INDEX_TESTS_H
#define BLISS_INDEX_TESTS_H
#include <alex.h>
#include <gtest/gtest.h>
#include <lipp.h>
#include <spdlog/common.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>

#include "bliss/bench_alex.h"
#include "bliss/bench_btree.h"
#include "bliss/bench_lipp.h"
#include "bliss/bliss_index.h"
#include "bliss/util/args.h"
#include "bliss/util/config.h"
#include "bliss/util/execute.h"
#include "bliss/util/reader.h"
#include "bliss/util/timer.h"

using namespace bliss::utils;

using key_type = unsigned long;
using value_type = unsigned long;

class BlissIndexTest : public testing::Test {
   protected:
    std::unique_ptr<bliss::BlissIndex<key_type, value_type>> index;
    std::string indexes[3] = {"alex", "lipp", "btree"};
    int num_keys = 100000;

    void SetUp() {}

    void GenerateData(std::vector<key_type> &data, int num_keys,
                      bool sorted = true) {
        for (int i = 0; i < num_keys; i++) {
            data.push_back(i);
        }
        if (!sorted) {
            std::random_shuffle(data.begin(), data.end());
        }
    }
};

#endif