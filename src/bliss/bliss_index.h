#ifndef BLISS_INDEX
#define BLISS_INDEX

#include <utility>
#include <vector>

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissIndex {
   public:
    virtual void bulkload(std::vector<std::pair<KEY_TYPE, VALUE_TYPE>> values);
    virtual bool get(KEY_TYPE key) = 0;
    virtual void put(KEY_TYPE key, VALUE_TYPE value) = 0;
};

}  // namespace bliss

#endif  // !BLISS_INDEX
