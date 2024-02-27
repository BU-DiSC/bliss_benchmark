#ifndef BLISS_INDEX
#define BLISS_INDEX

#include <vector>

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissIndex {
   public:
    virtual void preload(typename std::vector<KEY_TYPE>::iterator &start,
                         typename std::vector<KEY_TYPE>::iterator &end,
                         int seed = 0);
    virtual bool get(KEY_TYPE key) = 0;
    virtual void put(KEY_TYPE key, VALUE_TYPE value) = 0;
};

}  // namespace bliss

#endif  // !BLISS_INDEX
