#ifndef BLISS_BENCH_LEVELDB
#define BLISS_BENCH_LEVELDB

#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include "bliss/bliss_index.h"
#include "leveldb/db.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissLevelDBIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
 public:
  leveldb::DB* _db;


  BlissLevelDBIndex() {
    leveldb::Options options;
    options.create_if_missing = true;

    std::string db_path = "/tmp/bliss_leveldb_index";
    leveldb::Status status = leveldb::DB::Open(options, db_path, &_db);
    if (!status.ok()) {
      std::cerr << "Failed to open LevelDB at " << db_path << ": "
                << status.ToString() << std::endl;
      std::exit(1);
    }
  }


  void bulkload(std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
    for (const auto& kv : values) {
      put(kv.first, kv.second);
    }
  }


  bool get(KEY_TYPE key) override {
    std::string key_str = to_string(key);
    std::string value;
    leveldb::Status status = _db->Get(leveldb::ReadOptions(), key_str, &value);
    return status.ok();
  }


  void put(KEY_TYPE key, VALUE_TYPE value) override {
    std::string key_str = to_string(key);
    std::string value_str = to_string(value);
    leveldb::Status status = _db->Put(leveldb::WriteOptions(), key_str, value_str);
    if (!status.ok()) {
      std::cerr << "LevelDB put error: " << status.ToString() << std::endl;
    }
  }


  void end_routine() override {
    delete _db;
  }

 private:
  template <typename T>
  std::string to_string(const T& t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
  }


  std::string to_string(const std::string& s) {
    return s;
  }
};

}  // namespace bliss

#endif  // BLISS_BENCH_LEVELDB
