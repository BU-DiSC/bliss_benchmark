#ifndef BLISS_INDEX
#define BLISS_INDEX

namespace bliss {

template <typename KEY_TYPE, typename VALUE_TYPE>
class BlissIndex {
   public:
    VALUE_TYPE get(KEY_TYPE key);
    void put(KEY_TYPE key, VALUE_TYPE value);
};

}  // namespace bliss
#endif