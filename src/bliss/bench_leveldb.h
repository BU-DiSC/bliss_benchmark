#ifndef BLISS_BENCH_LEVELDB
#define BLISS_BENCH_LEVELDB

#include "leveldb/db.h"
#include "leveldb/write_batch.h"

#include <vector>

#include "bliss/bliss_index.h"
#include "spdlog/spdlog.h"

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissLevelDBIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
   public:
    leveldb::DB* _index;
    leveldb::Options _options;
    leveldb::WriteOptions _write_options;
    leveldb::ReadOptions _read_options;
    
    BlissLevelDBIndex() : _index(nullptr) {

        _options.create_if_missing = true;
        _options.error_if_exists = false;
        _options.compression = leveldb::kSnappyCompression;
        _write_options.sync = false;  // Async writes by default

        leveldb::Status status = leveldb::DB::Open(_options, nullptr, &_index);
        if (!status.ok()) {
            spdlog::error("Failed to open LevelDB: {}", status.ToString());
            throw std::runtime_error("Failed to initialize LevelDB");
        }
        spdlog::info("LevelDB initialized at path: {}", nullptr);

    };

    void bulkload(
        std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        
        if (!_index) {
            spdlog::error("Database is not initialized!");
            return;
        }
        leveldb::WriteBatch batch;
        for (const auto& kv : values) {
            batch.Put(std::to_string(kv.first), std::to_string(kv.second));
        }
        leveldb::Status status = _index->Write(_write_options, &batch);
        if (!status.ok()) {
            spdlog::error("Bulkload failed: {}", status.ToString());
        } else {
            spdlog::info("Bulkload completed with {} entries", values.size());
        }

    }

    bool get(KEY_TYPE key) override {

        if (!_index) {
            spdlog::error("Database is not initialized!");
            return false;
        }
        std::string value;
        leveldb::Status status =
            _index->Get(_read_options, std::to_string(key), &value);
        if (!status.ok()) {
            if (status.IsNotFound()) {
                spdlog::debug("Key {} not found", key);
            } else {
                spdlog::error("Get operation failed: {}", status.ToString());
            }
            return false;
        }
        spdlog::debug("Key {} found with value: {}", key, value);
        return true;

    }

    void put(KEY_TYPE key, VALUE_TYPE value) override {

       if (!_index) {
            spdlog::error("Database is not initialized!");
            return;
        }
        leveldb::Status status =
            _index->Put(_write_options, std::to_string(key), std::to_string(value));
        if (!status.ok()) {
            spdlog::error("Put operation failed: {}", status.ToString());
        } else {
            spdlog::debug("Inserted key: {}, value: {}", key, value);
        }

    }

    void end_routine() override {
        if (_index) {
            delete _index;
        }
    }
};

}  // namespace bliss

#endif  // !BLISS_BENCH_LEVELDB
