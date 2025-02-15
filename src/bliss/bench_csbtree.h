#ifndef BLISS_BENCH_CSBTREE_H
#define BLISS_BENCH_CSBTREE_H

#include <vector>
#include "bliss/bliss_index.h"
#include "src/vanilla_implementation/csbtree.h"

namespace bliss {

// Concrete implementation of CSBTree
class ConcreteCSBTree : public CSBTree {
public:
    ConcreteCSBTree(int order) : CSBTree(order) {
        root = new CSBNode(order);
    }

    int insert(uint64_t key) override {
        if (!root->checkPresenceOfKey(key)) {
            int index = root->getIndex(key);
            return root->addKeyToNode(key, index);
        }
        return 0;
    }

    int del(uint64_t key) override {
        return root->deleteKeyFromNode(key);
    }
};

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissCSBTreeIndex : public BlissIndex<KEY_TYPE, VALUE_TYPE> {
public:
    BlissCSBTreeIndex() : _index(16) {}

    void bulkload(std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values) override {
        for (const auto& kv : values) {
            put(kv.first, 0);
        }
    }

    bool get(KEY_TYPE key) override {
        uint64_t k = static_cast<uint64_t>(key);
        return _index.root->checkPresenceOfKey(k);
    }

    bool get(KEY_TYPE start, KEY_TYPE end) override {
        throw std::runtime_error("Not implemented");
    }

    void put(KEY_TYPE key, VALUE_TYPE) override {
        uint64_t k = static_cast<uint64_t>(key);
        _index.insert(k);
    }

    void end_routine() override {}

private:
    ConcreteCSBTree _index;
};

} // namespace bliss
#endif // BLISS_BENCH_CSBTREE_H
