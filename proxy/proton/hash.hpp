#pragma once
#include <stdint.h>

#pragma warning(disable : 4307)

#ifdef _WIN32
#define INLINE __forceinline
#else //for gcc/clang
#define INLINE inline
#endif

template <typename t, t val>
class cexpr {
   public:
    static constexpr t value = val;
};
namespace hs {
    constexpr uint32_t val_32_const = 0x811c9dc5u;
    constexpr uint32_t prime_32_const = 0x1000193u;
    constexpr uint64_t val_64_const = 0xcbf29ce484222325u;
    constexpr uint64_t prime_64_const = 0x100000001b3u;
    INLINE constexpr uint32_t hash32(const char* const str, const uint32_t value = val_32_const) noexcept {
        return (str[0] == '\0') ? value : hash32(&str[1], (value ^ uint32_t(str[0])) * prime_32_const);
    }
    INLINE constexpr uint64_t hash64(const char* const str, const uint64_t value = val_64_const) noexcept {
        return (str[0] == '\0') ? value : hash64(&str[1], (value ^ uint64_t(str[0])) * prime_64_const);
    }
} // namespace hs
#define fnv32(s) (cexpr<uint32_t, hs::hash32(s)>::value)
#define fnv64(s) (cexpr<uint64_t, hs::hash64(s)>::value)
