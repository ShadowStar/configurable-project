#ifndef __COMMON_H
#define __COMMON_H
#ifdef __KERNEL__
#include <linux/string.h>
#include <linux/ctype.h>
#elif !defined(__BPF__)
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#else
typedef __INT8_TYPE__ int8_t;
typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT64_TYPE__ int64_t;
typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
#ifdef __LP64__
#define __BPF64__
#elif defined(__ILP32__)
#define __BPF32__
#endif
#endif

#ifndef CACHE_LINE_SZIE
#define CACHE_LINE_SZIE     64
#endif

#define CACHE_LINE_ALIGNED  __attribute__((aligned(CACHE_LINE_SZIE)))

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN        4321
#endif
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN     1234
#endif

#if !defined(__BYTE_ORDER) && defined(__BYTE_ORDER__)
#define __BYTE_ORDER        __BYTE_ORDER__
#endif

#ifndef __KERNEL__
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
#endif

#ifndef BIT
#define BIT(nr)             ((uint64_t)1UL << (nr))
#endif

#ifndef likely
#define likely(x)           __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)         __builtin_expect(!!(x), 0)
#endif

#define PRE_MASK_R(x)                                                       \
    ({ unsigned _x = (x); (_x > 63 ? 0ULL : ~0ULL >> _x); })
#define SUF_MASK_R(x)                                                       \
    ({ unsigned _x = (x); (_x > 63 ? 0ULL : ~0ULL << _x); })

#define PRE_MASK(x)         ~PRE_MASK_R(x)
#define SUF_MASK(x)         ~SUF_MASK_R(x)

#define RANGE_MASK(x, y)    (PRE_MASK(64 - (x)) & SUF_MASK((y) + 1))
#define RANGE_MASK_R(x, y)  (PRE_MASK(63 - (y)) | SUF_MASK(x))

#ifndef UNUSED
#define UNUSED(x)           (x = x)
#endif

#ifndef ACCESS_ONCE
#define ACCESS_ONCE(x)      (*(volatile typeof(x) *)&(x))
#endif    /* end of ACCESS_ONCE */

#define VOLATILE(x)         (*(volatile typeof(x) *)&(x))

#define SUF_FUNC(NAME, SUFFIX, ...)    NAME##SUFFIX(__VA_ARGS__)
#define CALL_FUNC_VA(NAME, VA, ...)    SUF_FUNC(NAME, VA, ##__VA_ARGS__)

#ifndef ALIGN
#define __ALIGN_MASK(x, mask)   (((x) + (mask)) & ~(mask))
#define ALIGN(x, a)             __ALIGN_MASK(x, (typeof(x))(a)-1)
#endif

#define ALIGN_INC_POW2(x)   (((x) & ((x) - 1)) == 0 ?                       \
    (x) + 2 * ((x) == 0) : (typeof(x))(1ULL << (64 - clz64(x))))

#define ALIGN_DEC_POW2(x)   (((x) & ((x) - 1)) == 0 ?                       \
    (x) + 2 * ((x) == 0) : (typeof(x))(1ULL << (63 - clz64(x))))

#define ALIGN_INC(x, a)                                                     \
    (((x) + ((x) == 0) + (a) - 1) & ~((typeof(x))(a) - 1))
#define ALIGN_DEC(x, a)     ((x) & ~((typeof(x))(a) - 1))

#ifndef PTR_ALIGN
#define PTR_ALIGN(p, a)     ((typeof(p))ALIGN((unsigned long)(p), (a)))
#endif

#ifndef IS_ALIGNED
#define IS_ALIGNED(x, a)    (((x) & ((typeof(x))(a) - 1)) == 0)
#endif

#ifndef bit_mask_to_strip
#define bit_mask_to_strip(x, y) ({                                          \
    int _ex = (y);                                                          \
    typeof(x) _mask = (x);                                                  \
    typeof(x) _stp = _mask & SUF_MASK(_ex);                                 \
    _stp |= (_mask & PRE_MASK(63 - _ex)) >> 1; })
#endif

#ifndef bit_strip_to_mask
#define bit_strip_to_mask(x, y) ({                                          \
    int _ex = (y);                                                          \
    typeof(x) _stp = (x);                                                   \
    typeof(x) _mask = _stp & SUF_MASK(_ex);                                 \
    _mask |= (_stp << 1) & PRE_MASK(63 - _ex); })
#endif

/*
 * min()/max()/clamp() macros that also do
 * strict type-checking.. See the
 * "unnecessary" pointer comparison.
 */
#ifndef min
#define min(x, y) ({                                                        \
    typeof(x) _min1 = (x);                                                  \
    typeof(y) _min2 = (y);                                                  \
    (void) (&_min1 == &_min2);                                              \
    _min1 < _min2 ? _min1 : _min2; })
#endif

#ifndef max
#define max(x, y) ({                                                        \
    typeof(x) _max1 = (x);                                                  \
    typeof(y) _max2 = (y);                                                  \
    (void) (&_max1 == &_max2);                                              \
    _max1 > _max2 ? _max1 : _max2; })
#endif

/**
 * clamp - return a value clamped to a given range with strict typechecking
 * @val: current value
 * @min: minimum allowable value
 * @max: maximum allowable value
 *
 * This macro does strict typechecking of min/max to make sure they are of the
 * same type as val.  See the unnecessary pointer comparisons.
 */
#ifndef clamp
#define clamp(val, min, max) ({                                             \
    typeof(val) __val = (val);                                              \
    typeof(min) __min = (min);                                              \
    typeof(max) __max = (max);                                              \
    (void) (&__val == &__min);                                              \
    (void) (&__val == &__max);                                              \
    __val = __val < __min ? __min: __val;                                   \
    __val > __max ? __max: __val; })
#endif

/*
 * ..and if you can't take the strict
 * types, you can specify one yourself.
 *
 * Or not use min/max/clamp at all, of course.
 */
#ifndef min_t
#define min_t(type, x, y) ({                                                \
    type __min1 = (x);                                                      \
    type __min2 = (y);                                                      \
    __min1 < __min2 ? __min1: __min2; })
#endif

#ifndef max_t
#define max_t(type, x, y) ({                                                \
    type __max1 = (x);                                                      \
    type __max2 = (y);                                                      \
    __max1 > __max2 ? __max1: __max2; })
#endif

/**
 * clamp_t - return a value clamped to a given range using a given type
 * @type: the type of variable to use
 * @val: current value
 * @min: minimum allowable value
 * @max: maximum allowable value
 *
 * This macro does no typechecking and uses temporary variables of type
 * 'type' to make all the comparisons.
 */
#ifndef clamp_t
#define clamp_t(type, val, min, max) ({                                     \
    type __val = (val);                                                     \
    type __min = (min);                                                     \
    type __max = (max);                                                     \
    __val = __val < __min ? __min: __val;                                   \
    __val > __max ? __max: __val; })
#endif

/**
 * clamp_val - return a value clamped to a given range using val's type
 * @val: current value
 * @min: minimum allowable value
 * @max: maximum allowable value
 *
 * This macro does no typechecking and uses temporary variables of whatever
 * type the input argument 'val' is.  This is useful when val is an unsigned
 * type and min and max are literals that will otherwise be assigned a signed
 * integer type.
 */
#ifndef clamp_val
#define clamp_val(val, min, max) ({                                         \
    typeof(val) __val = (val);                                              \
    typeof(val) __min = (min);                                              \
    typeof(val) __max = (max);                                              \
    __val = __val < __min ? __min: __val;                                   \
    __val > __max ? __max: __val; })
#endif

#ifndef offsetof
#ifdef __compiler_offsetof
#define offsetof(TYPE, MEMBER) __compiler_offsetof(TYPE, MEMBER)
#else
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
#endif /* offsetof */

#ifndef rangeof
#define rangeof(TYPE, FROM, TO) ({                                          \
    BUILD_BUG_ON(offsetof(TYPE, FROM) > offsetof(TYPE, TO));                \
    (offsetof(TYPE, TO) - offsetof(TYPE, FROM) + sizeof(((TYPE *)0)->TO));})
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({                                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);                    \
    (type *)( (char *)__mptr - offsetof(type, member) );})
#endif /* container_of */

#ifndef FALL_THROUGH
#if defined(__GNUC__)
#if defined(fallthrough)
#define FALL_THROUGH fallthrough
#elif ((__GNUC__ > 7) || ((__GNUC__ == 7) && (__GNUC_MINOR__ >= 1)))
#define FALL_THROUGH ; __attribute__ ((fallthrough))
#elif defined(__clang__) && defined(__clang_major__) && (__clang_major__ >= 12)
#define FALL_THROUGH ; __attribute__ ((fallthrough))
#else
#define FALL_THROUGH do {} while (0)
#endif
#else
#define FALL_THROUGH do {} while (0)
#endif  /** __GNUC__ */
#endif  /** FALL_THROUGH */

/* Force a compilation error if condition is true, but also produce a
   result (of value 0 and type size_t), so the expression can be used
   e.g. in a structure initializer (or where-ever else comma expressions
   aren't permitted). */
#ifndef BUILD_BUG_NOT_ZERO
#define BUILD_BUG_NOT_ZERO(e)    (sizeof(struct { int:-!!(e); }))
#endif

/* Force a compilation error if condition is true */
#ifndef BUILD_BUG_ON
#define BUILD_BUG_ON(e)        ((void)sizeof(char[1 - 2 * !!(e)]))
#endif

#ifndef BUILD_BUG_NOT_NULL
#define BUILD_BUG_NOT_NULL(e)    ((void *)BUILD_BUG_NOT_ZERO(e))
#endif

/* Force a compilation error if a constant expression is not a power of 2 */
#define BUILD_BUG_NOT_POWER_OF_2(n)                                         \
    BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))

#define BUILD_EXPR(v, b0, b1)   __builtin_choose_expr((v), b0, b1)

#define BUILD_EXPR32(v, b32)                                                \
    __builtin_choose_expr(sizeof(v) == 4, b32, (void)0)

#define BUILD_EXPR64(v, b64)                                                \
    __builtin_choose_expr(sizeof(v) == 8, b64, (void)0)

#define BUILD_EXPR_64or32(v, b64, b32)                                      \
    __builtin_choose_expr(sizeof(v) == 8, b64, BUILD_EXPR32(v, b32))

#define BUILD_EXPR_CONST(v, bconst, bdynamic)                               \
    (__builtin_constant_p(v) ? bconst : bdynamic)

#define BUILD_CONST(v, V)   (__builtin_constant_p(v) && (v) == V)

/* Are two types/vars the same type (ignoring qualifiers)? */
#define SAME_TYPE(a, b)     __builtin_types_compatible_p(typeof(a), typeof(b))

#define BUILD_SAME_TYPE(a, b)   BUILD_BUG_ON(!SAME_TYPE(a, b))

/* &a[0] degrades to a pointer: a different type from an array */
#define BUILD_BE_ARRAY(a)       BUILD_BUG_NOT_ZERO(SAME_TYPE((a), &(a)[0]))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + BUILD_BE_ARRAY(arr))

#ifndef NIPQUAD
#define NIPQUAD_FMT    "%u.%u.%u.%u"
#define NIPQUAD(value)                                                      \
    (value & 0xFF000000) >> 24, (value & 0x00FF0000) >> 16,                 \
    (value & 0x0000FF00) >> 8, (value & 0x000000FF)
#endif

#define NIP6QUAD_FMT    "%x:%x:%x:%x:%x:%x:%x:%x"
#define NIP6QUAD(v)                                                         \
    ((uint16_t *)v)[0], ((uint16_t *)v)[1], ((uint16_t *)v)[2],             \
    ((uint16_t *)v)[3], ((uint16_t *)v)[4], ((uint16_t *)v)[5],             \
    ((uint16_t *)v)[6], ((uint16_t *)v)[7]

#define MACQUAD_FMT    "%02X:%02X:%02X:%02X:%02X:%02X"
#define MACQUAD(addr)                                                       \
    ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2],       \
    ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5]

#define Ki              1024ULL
#define Mi              (1024 * Ki)
#define Gi              (1024 * Mi)
#define Ti              (1024 * Gi)

#define MS_PER_SECOND   1000
#define US_PER_SECOND   1000000

#define U64_MAX         0xFFFFFFFFFFFFFFFF
#define U32_MAX         0xFFFFFFFF
#define U16_MAX         0xFFFF
#define U8_MAX          0xFF

#define TGMK_FMT "%luTi %luGi %luMi %luKi %lu"
#define TGMK(x)                                                             \
    ((x) >> 40), (((x) & (Ti - 1)) >> 30), (((x) & (Gi - 1)) >> 20),        \
    (((x) & (Mi - 1)) >> 10), ((x) & (Ki - 1))

#define ___rol(TYPE, w, s)                                                  \
({ TYPE##_t __w = (w); int __s = (s);                                       \
 (__w << __s) | (__w >> (sizeof(TYPE##_t) * 8 - __s)); })

#define ___ror(TYPE, w, s)                                                  \
({ TYPE##_t __w = (w); int __s = (s);                                       \
 (__w >> __s) | (__w << (sizeof(TYPE##_t) * 8 - __s)); })

#define ___constant_rol64(w, s)     ___rol(uint64, w, s)
#define ___constant_rol32(w, s)     ___rol(uint32, w, s)
#define ___constant_rol16(w, s)     ___rol(uint16, w, s)
#define ___constant_rol8(w, s)      ___rol(uint8, w, s)

#define ___constant_ror64(w, s)     ___ror(uint64, w, s)
#define ___constant_ror32(w, s)     ___ror(uint32, w, s)
#define ___constant_ror16(w, s)     ___ror(uint16, w, s)
#define ___constant_ror8(w, s)      ___ror(uint8, w, s)

static inline int ___constant_popcnt16(uint16_t x)
{
    x -= ((x >> 1) & 0x5555U);
    x = (x & 0x3333U) + ((x >> 2) & 0x3333U);
    x = (x + (x >> 4)) & 0x0F0FU;
    x += x >> 8;
    return x & 0x1F;
}

static inline int ___constant_popcnt32(uint32_t x)
{
    x -= ((x >> 1) & 0x55555555ULL);
    x = (x & 0x33333333ULL) + ((x >> 2) & 0x33333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0FULL;
    x += x >> 8;
    x += x >> 16;
    return x & 0x3F;
}

static inline int ___constant_popcnt64(uint64_t x)
{
    x -= ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    x += x >> 8;
    x += x >> 16;
    x += x >> 32;
    return x & 0x7F;
}

static inline int ___constant_clz64(uint64_t x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    x |= (x >> 32);

    return 64 - ___constant_popcnt64(x);
}

static inline int ___constant_clz32(uint32_t x)
{
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);

    return 32 - ___constant_popcnt32(x);
}

static inline int ___constant_ctz64(uint64_t x)
{
    return ___constant_popcnt64((x & -x) - 1);
}

static inline int ___constant_ctz32(uint32_t x)
{
    return ___constant_popcnt32((x & -x) - 1);
}

#define ___constant_clo64(x)    ___constant_clz64(~(x))
#define ___constant_clo32(x)    ___constant_clz32(~(x))

#define ___constant_cto64(x)    ___constant_ctz64(~(x))
#define ___constant_cto32(x)    ___constant_ctz32(~(x))

#define ___constant_swap16(x)    ((uint16_t)(                               \
    (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) |                            \
    (((uint16_t)(x) & (uint16_t)0xff00U) >> 8)))

#define ___constant_swap32(x)    ((uint32_t)(                               \
    (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) |                      \
    (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) |                      \
    (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) |                      \
    (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24)))

#define ___constant_swap64(x)    ((uint64_t)(                               \
    (((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) |             \
    (((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) |             \
    (((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) |             \
    (((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) |             \
    (((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) |             \
    (((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) |             \
    (((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) |             \
    (((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56)))

#if defined(__x86_64__) || defined(__BPF64__)
#ifndef __BYTE_ORDER
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif
static inline int arch_popcnt32(uint32_t x)
{
    __asm__(
    "popcntl    %1, %0"
    : "=r" (x)
    : "0" (x));
    return x;
}

static inline int arch_popcnt64(uint64_t x)
{
    __asm__(
    "popcntq    %1, %0"
    : "=r" (x)
    : "0" (x));
    return x;
}

static inline int arch_clz32(uint32_t x)
{
    if (x == 0)
        return 32;
    __asm__(
    "bsrl    %1, %0"
    : "=r" (x)
    : "0" (x));
    return 31 - x;
}

static inline int arch_clo32(uint32_t x)
{
    if (x == -1U)
        return 32;
    __asm__(
    "notl    %0\n\t"
    "bsrl    %1, %0"
    : "=r" (x)
    : "0" (x));
    return 31 - x;
}

static inline int arch_clz64(uint64_t x)
{
    if (x == 0)
        return 64;
    __asm__(
    "bsrq    %1, %0"
    : "=r" (x)
    : "0" (x));
    return 63 - x;
}

static inline int arch_clo64(uint64_t x)
{
    if (x == -1ULL)
        return 64;
    __asm__(
    "notq    %0\n\t"
    "bsrq    %1, %0"
    : "=r" (x)
    : "0" (x));
    return 63 - x;
}

static inline uint16_t arch_swap16(uint16_t x)
{
    return ((x & 0x00FFU) << 8) | ((x & 0xFF00U) >> 8);
}

static inline uint32_t arch_swap32(uint32_t x)
{
    __asm__(
    "bswapl    %0"
    : "=r" (x)
    : "0" (x));
    return x;
}

static inline uint64_t arch_swap64(uint64_t x)
{
    __asm__(
    "bswapq    %0"
    : "=r" (x)
    : "0" (x));
    return x;
}
#elif defined(__i386__) || defined(__BPF32__)
#ifndef __BYTE_ORDER
#define __BYTE_ORDER        __LITTLE_ENDIAN
#endif
static inline int arch_popcnt32(uint32_t x)
{
    __asm__(
    "popcntl    %1, %0"
    : "=r" (x)
    : "0" (x));
    return x;
}

static inline int arch_popcnt64(uint64_t x)
{
    union {
        struct {
            uint32_t a;
            uint32_t b;
        } s;
        uint64_t u;
    } v = { .u = x };

    __asm__(
    "popcntl    %2, %0\n\t"
    "popcntl    %3, %1\n\t"
    "addl        %1, %0"
    : "=r" (v.s.a), "=r" (v.s.b)
    : "0" (v.s.a), "1" (v.s.b));
    return v.s.a;
}

static inline int arch_clz32(uint32_t x)
{
    if (x == 0)
        return 32;
    __asm__(
    "bsrl    %1, %0"
    : "=r" (x)
    : "0" (x));
    return 31 - x;
}

static inline int arch_clo32(uint32_t x)
{
    if (x == -1U)
        return 32;
    __asm__(
    "notl    %0\n\t"
    "bsrl    %1, %0"
    : "=r" (x)
    : "0" (x));
    return 31 - x;
}

static inline int arch_clz64(uint64_t x)
{
    if (x == 0)
        return 64;
    union {
        struct {
            uint32_t b;
            uint32_t a;
        } s;
        uint64_t u;
    } v = { .u = x };
    __asm__(
    "cmpl    $0, %0\n\t"
    "je    1f\n\t"
    "bsrl    %1, %0\n\t"
    "addl    $32, %0\n\t"
    "jmp    2f\n\t"
    "1:"
    "bsrl    %2, %0\n\t"
    "2:"
    : "=r" (v.s.a)
    : "0" (v.s.a), "r" (v.s.b));
    return 63 - v.s.a;
}

static inline int arch_clo64(uint64_t x)
{
    if (x == -1ULL)
        return 64;
    union {
        struct {
            uint32_t b;
            uint32_t a;
        } s;
        uint64_t u;
    } v = { .u = x };
    __asm__(
    "notl    %0\n\t"
    "notl    %2\n\t"
    "cmpl    $0, %0\n\t"
    "je    1f\n\t"
    "bsrl    %1, %0\n\t"
    "addl    $32, %0\n\t"
    "jmp    2f\n\t"
    "1:"
    "bsrl    %2, %0\n\t"
    "2:"
    : "=r" (v.s.a)
    : "0" (v.s.a), "r" (v.s.b));
    return 63 - v.s.a;
}

static inline uint16_t arch_swap16(uint16_t x)
{
    return ((x & 0x00FFU) << 8) | ((x & 0xFF00U) >> 8);
}

static inline uint32_t arch_swap32(uint32_t x)
{
    __asm__(
    "bswapl    %0"
    : "=r" (x)
    : "0" (x));
    return x;
}

static inline uint64_t arch_swap64(uint64_t x)
{
    union {
        struct {
            uint32_t a;
            uint32_t b;
        } s;
        uint64_t u;
    } v = { .u = x };

    __asm__(
    "bswapl    %0\n\t"
    "bswapl    %1\n\t"
    "xchgl    %0, %1"
    : "=r" (v.s.a), "=r" (v.s.b)
    : "0" (v.s.a), "1"(v.s.b));
    return v.u;
}
#elif defined(__mips64)

#if defined(__MIPSEB) && !defined(__BYTE_ORDER)
#define __BYTE_ORDER    __BIG_ENDIAN
#elif defined(__MIPSEL) && !defined(__BYTE_ORDER)
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif

#define _dup_16_to_32(x)        ({                                          \
    uint32_t _x = (uint16_t)(x); (_x) | ((_x) << 16); })

#define _dup_8_to_32(x)         ({                                          \
    uint32_t _x = (uint8_t)(x);                                             \
    (_x) | ((_x) << 8) | ((_x) << 16) | ((_x) << 24); })

#define __arch_ror(S, D)        ({                                          \
    if (__builtin_constant_p(S)) {                                          \
        __asm__ (                                                           \
        "\t"#D"rotr    %0, %0, "#S"\n"                                      \
        : "+r" (__w));                                                      \
    } else {                                                                \
        __asm__ (                                                           \
        "\t"#D"rotrv    %0, %0, %1\n"                                       \
        : "+r" (__w) : "r" (S));                                            \
    } __w; })

#define _arch_ror(TYPE, w, s)   ({                                          \
    TYPE##_t __w = (w);                                                     \
    BUILD_EXPR_64or32(__w, __arch_ror(s, d), __arch_ror(s,)); })

#define arch_ror64(w, s)    _arch_ror(uint64, w, s)
#define arch_ror32(w, s)    _arch_ror(uint32, w, s)
#define arch_ror16(w, s)    (uint16_t)_arch_ror(uint32, _dup_16_to_32(w), s)
#define arch_ror8(w, s)     (uint8_t)_arch_ror(uint32, _dup_8_to_32(w), s)

#define arch_rol64(w, s)    arch_ror64(w, (64 - s))
#define arch_rol32(w, s)    arch_ror32(w, (32 - s))
#define arch_rol16(w, s)    arch_ror16(w, (16 - s))
#define arch_rol8(w, s)     arch_ror8(w, (8 - s))

static inline int arch_popcnt32(uint32_t x)
{
    __asm__ (
    "pop    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline int arch_popcnt64(uint64_t x)
{
    __asm__ (
    "dpop    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline int arch_clz32(uint32_t x)
{
    __asm__(
    "clz    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline int arch_clo32(uint32_t x)
{
    __asm__(
    "clo    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline int arch_clz64(uint64_t x)
{
    __asm__(
    "dclz    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline int arch_clo64(uint64_t x)
{
    __asm__(
    "dclo    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline uint16_t arch_swap16(uint16_t x)
{
    __asm__(
    "wsbh    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline uint32_t arch_swap32(uint32_t x)
{
    __asm__(
    "wsbh    %0, %1\n\t"
    "rotr    %0, %0, 16"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline uint64_t arch_swap64(uint64_t x)
{
    __asm__(
    "dsbh    %0, %1\n\t"
    "dshd    %0, %0"
    : "=r" (x)
    : "r" (x));

    return x;
}
#elif defined(__arm__)

#if defined(__ARMEB__) && !defined(__BYTE_ORDER)
#define __BYTE_ORDER    __BIG_ENDIAN
#elif defined(__ARMEL__) && !defined(__BYTE_ORDER)
#define __BYTE_ORDER    __LITTLE_ENDIAN
#endif

#if defined(__thumb2__)
static inline int arch_popcnt32(uint32_t x)
{
    uint32_t tmp;
    __asm__ (
    "and    %1, %0, #0xAAAAAAAA\n\t"
    "sub    %0, %0, %1, lsr #1\n\t"
    "and    %1, %0, #0xCCCCCCCC\n\t"
    "and    %0, %0, #0x33333333\n\t"
    "add    %0, %0, %1, lsr #2\n\t"
    "add    %0, %0, %0, lsr #4\n\t"
    "and    %0, %0, #0x0F0F0F0F\n\t"
    "add    %0, %0, %0, lsr #8\n\t"
    "add    %0, %0, %0, lsr #16\n\t"
    "and    %0, %0, #63"
    : "+r" (x), "=&r" (tmp));
    return x;
}
#elif defined(__thumb__)
#define arch_popcnt32(x)    ___constant_popcnt32(x)
#else
static inline int arch_popcnt32(uint32_t x)
{
    uint32_t t0, t1, t2, t3, t4;
    __asm__ (
    "mov    %2, #0xFF\n\t"
    "orr    %2, %2, #0xFF<<16\n\t"
    "eor    %3, %2, %2, lsl #4\n\t"
    "eor    %4, %3, %3, lsl #2\n\t"
    "eor    %5, %4, %4, lsl #1\n\t"
    "and    %1, %5, %0, lsr #1\n\t"
    "sub    %0, %0, %1\n\t"
    "and    %1, %4, %0, lsr #2\n\t"
    "and    %0, %4, %0\n\t"
    "add    %0, %0, %1\n\t"
    "add    %0, %0, %0, lsr #4\n\t"
    "and    %0, %0, %3\n\t"
    "add    %0, %0, %0, lsr #8\n\t"
    "add    %0, %0, %0, lsr #16\n\t"
    "and    %0, %0, #63\n\t"
    : "+r" (x), "=&r" (t0), "=&r" (t1), "=&r" (t2), "=&r" (t3), "=&r" (t4));
    return x;
}
#endif

static inline int arch_popcnt64(uint64_t x)
{
    union {
        struct {
            uint32_t a;
            uint32_t b;
        } s;
        uint64_t u;
    } v = { .u = x };
    return arch_popcnt32(v.s.a) + arch_popcnt32(v.s.b);
}

static inline int arch_clz32(uint32_t x)
{
    __asm__ (
    "clz    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline int arch_clz64(uint64_t x)
{
    if (x == 0)
        return 64;
    union {
        struct {
            uint32_t b;
            uint32_t a;
        } s;
        uint64_t u;
    } v = { .u = x };
    __asm__ (
    "clz    %0, %1\n\t"
    "teq    %0, #32\n\t"
    "clz    %2, %2\n\t"
    "addeq    %0, %2, #32"
    : "=r" (v.s.a)
    : "0" (v.s.a), "r" (v.s.b));
    return v.s.a;
}

static inline int arch_clo32(uint32_t x)
{
    return arch_clz32(~x);
}

static inline int arch_clo64(uint64_t x)
{
    return arch_clz64(~x);
}

static inline uint16_t arch_swap16(uint32_t x)
{
    __asm__ (
    "rev16    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline uint32_t arch_swap32(uint32_t x)
{
    __asm__ (
    "rev    %0, %1"
    : "=r" (x)
    : "r" (x));
    return x;
}

static inline uint64_t arch_swap64(uint64_t x)
{
    uint32_t h = x >> 32;
    uint32_t l = x & (0xFFFFFFFFU);
    return ((uint64_t)arch_swap32(l) << 32) | ((uint64_t)arch_swap32(h));
}
#endif

#ifndef arch_ror64
#define arch_ror64(w, s)    ___constant_ror64(w, s)
#endif
#ifndef arch_ror32
#define arch_ror32(w, s)    ___constant_ror32(w, s)
#endif
#ifndef arch_ror16
#define arch_ror16(w, s)    ___constant_ror16(w, s)
#endif
#ifndef arch_ror8
#define arch_ror8(w, s)     ___constant_ror8(w, s)
#endif

#ifndef arch_rol64
#define arch_rol64(w, s)    ___constant_rol64(w, s)
#endif
#ifndef arch_rol32
#define arch_rol32(w, s)    ___constant_rol32(w, s)
#endif
#ifndef arch_rol16
#define arch_rol16(w, s)    ___constant_rol16(w, s)
#endif
#ifndef arch_rol8
#define arch_rol8(w, s)     ___constant_rol8(w, s)
#endif

#ifndef arch_ctz64
#define arch_ctz64(x)       __builtin_ctzl(x)
#endif
#ifndef arch_ctz32
#define arch_ctz32(x)       __builtin_ctz(x)
#endif

#ifndef arch_cto64
#define arch_cto64(x)       ___constant_cto64(x)
#endif
#ifndef arch_cto32
#define arch_cto32(x)       ___constant_cto32(x)
#endif

#ifndef arch_popcnt64
#define arch_popcnt64(x)    __builtin_popcountl(x)
#endif
#ifndef arch_popcnt32
#define arch_popcnt32(x)    __builtin_popcount(x)
#endif
#ifndef arch_popcnt16
#define arch_popcnt16(x)    __builtin_popcount(x)
#endif

#ifndef arch_clz64
#define arch_clz64(x)       __builtin_clzl(x)
#endif
#ifndef arch_clz32
#define arch_clz32(x)       __builtin_clz(x)
#endif

#ifndef arch_clo64
#define arch_clo64(x)       ___constant_clo64(x)
#endif
#ifndef arch_clo32
#define arch_clo32(x)       ___constant_clo32(x)
#endif

#ifndef arch_swap64
#define arch_swap64(x)      __builtin_bswap64(x)
#endif
#ifndef arch_swap32
#define arch_swap32(x)      __builtin_bswap32(x)
#endif
#ifndef arch_swap16
#define arch_swap16(x)      ___constant_swap16(x)
#endif

#define __bit_ro(bs, w, s)                                                  \
    ((__builtin_constant_p(w) && __builtin_constant_p(s)) ?                 \
     ___constant_ro##bs(w, s) : arch_ro##bs(w, s))

#define rol64(word, shift)  __bit_ro(l64, word, shift)
#define rol32(word, shift)  __bit_ro(l32, word, shift)
#define rol16(word, shift)  __bit_ro(l16, word, shift)
#define rol8(word, shift)   __bit_ro(l8, word, shift)

#define ror64(word, shift)  __bit_ro(r64, word, shift)
#define ror32(word, shift)  __bit_ro(r32, word, shift)
#define ror16(word, shift)  __bit_ro(r16, word, shift)
#define ror8(word, shift)   __bit_ro(r8, word, shift)

/* Count Ones in 16-Bits */
#define popcnt16(x) (__builtin_constant_p(x) ?                              \
    ___constant_popcnt16(x) : arch_popcnt16(x))

/* Count Ones in 32-Bits */
#define popcnt32(x) (__builtin_constant_p(x) ?                              \
    ___constant_popcnt32(x) : arch_popcnt32(x))

/* Count Ones in 64-Bits */
#define popcnt64(x) (__builtin_constant_p(x) ?                              \
    ___constant_popcnt64(x) : arch_popcnt64(x))

/* Count Leading Zeros in 64-Bits */
#define clz64(x) (__builtin_constant_p(x) ?                                 \
    ___constant_clz64(x) : arch_clz64(x))

/* Count Leading Ones in 64-Bits */
#define clo64(x) (__builtin_constant_p(x) ?                                 \
    ___constant_clo64(x) : arch_clo64(x))

/* Count Leading Zeros in 32-Bits */
#define clz32(x) (__builtin_constant_p(x) ?                                 \
    ___constant_clz32(x) : arch_clz32(x))

/* Count Leading Ones in 32-Bits */
#define clo32(x) (__builtin_constant_p(x) ?                                 \
    ___constant_clo32(x) : arch_clo32(x))

/* Count Trailing Zeros in 64-Bits */
#define ctz64(x) (__builtin_constant_p(x) ?                                 \
    ___constant_ctz64(x) : arch_ctz64(x))

/* Count Trailing Ones in 64-Bits */
#define cto64(x) (__builtin_constant_p(x) ?                                 \
    ___constant_cto64(x) : arch_cto64(x))

/* Count Trailing Zeros in 32-Bits */
#define ctz32(x) (__builtin_constant_p(x) ?                                 \
    ___constant_ctz32(x) : arch_ctz32(x))

/* Count Trailing Ones in 32-Bits */
#define cto32(x) (__builtin_constant_p(x) ?                                 \
    ___constant_cto32(x) : arch_cto32(x))

/* Swap Byte in 16-Bits */
#define swap16(x) (__builtin_constant_p((uint16_t)(x)) ?                    \
    ___constant_swap16(x) : arch_swap16(x))

/* Swap Byte in 32-Bits */
#define swap32(x) (__builtin_constant_p((uint32_t)(x)) ?                    \
    ___constant_swap32(x) : arch_swap32(x))

/* Swap Byte in 64-Bits */
#define swap64(x) (__builtin_constant_p((uint64_t)(x)) ?                    \
    ___constant_swap64(x) : arch_swap64(x))

#define bitmap_foreach(pos, mask, t)                                        \
    for (t = mask; t && ({ pos = 63 - clz64(t); 1; }); t &= ~(1ULL << pos))

#ifndef __KERNEL__
#if (__BYTE_ORDER == __BIG_ENDIAN)
#define cpu_to_be64(x)      (x)
#define cpu_to_be32(x)      (x)
#define cpu_to_be16(x)      (x)
#define cpu_to_le64(x)      swap64(x)
#define cpu_to_le32(x)      swap32(x)
#define cpu_to_le16(x)      swap16(x)
#else    /* __BYTE_ORDER == __LITTLE_ENDIAN */
#define cpu_to_be64(x)      swap64(x)
#define cpu_to_be32(x)      swap32(x)
#define cpu_to_be16(x)      swap16(x)
#define cpu_to_le64(x)      (x)
#define cpu_to_le32(x)      (x)
#define cpu_to_le16(x)      (x)
#endif
#define be64_to_cpu(x)      cpu_to_be64(x)
#define be32_to_cpu(x)      cpu_to_be32(x)
#define be16_to_cpu(x)      cpu_to_be16(x)
#define le64_to_cpu(x)      cpu_to_le64(x)
#define le32_to_cpu(x)      cpu_to_le32(x)
#define le16_to_cpu(x)      cpu_to_le16(x)
#endif

#define load_u8(x)          (*(uint8_t *)(x))
#define load_u16(x)         (*(uint16_t *)(x))
#define load_u32(x)         (*(uint32_t *)(x))
#define load_u64(x)         (*(uint64_t *)(x))

#if (__BYTE_ORDER == __BIG_ENDIAN)
#define load_be16(x)        load_u16(x)
#define load_be32(x)        load_u32(x)
#define load_be64(x)        load_u64(x)
#define load_le16(x)        le16_to_cpu(load_u16(x))
#define load_le32(x)        le32_to_cpu(load_u32(x))
#define load_le64(x)        le64_to_cpu(load_u64(x))
#define load_h16(x)         load_be16(x)
#define load_h32(x)         load_be32(x)
#define load_h64(x)         load_be64(x)
#define load_n16(x)         load_be16(x)
#define load_n32(x)         load_be32(x)
#define load_n64(x)         load_be64(x)
#else    /* __BYTE_ORDER == __LITTLE_ENDIAN */
#define load_be16(x)        be16_to_cpu(load_u16(x))
#define load_be32(x)        be32_to_cpu(load_u32(x))
#define load_be64(x)        be64_to_cpu(load_u64(x))
#define load_le16(x)        load_u16(x)
#define load_le32(x)        load_u32(x)
#define load_le64(x)        load_u64(x)
#define load_h16(x)         load_le16(x)
#define load_h32(x)         load_le32(x)
#define load_h64(x)         load_le64(x)
#define load_n16(x)         load_be16(x)
#define load_n32(x)         load_be32(x)
#define load_n64(x)         load_be64(x)
#endif

#define ATOM_LOAD_RELAXED(x)    __atomic_load_n(x, __ATOMIC_RELAXED)
#define ATOM_LOAD(x)            __atomic_load_n(x, __ATOMIC_ACQUIRE)
#define ATOM_LOAD_TO(x, p)      __atomic_load(x, p, __ATOMIC_ACQUIRE)

#define ATOM_CLR(x)             __atomic_clear(x, __ATOMIC_RELEASE)
#define ATOM_STORE_RELAXED(x, v)    __atomic_store_n(x, v, __ATOMIC_RELAXED)
#define ATOM_STORE(x, v)        __atomic_store_n(x, v, __ATOMIC_RELEASE)
#define ATOM_STORE_FROM(x, p)   __atomic_store(x, p, __ATOMIC_RELEASE)

#define ATOM_ADD_RELAXED(x, v)  __atomic_add_fetch(x, v, __ATOMIC_RELAXED)
#define ATOM_ADD(x, v)          __atomic_add_fetch(x, v, __ATOMIC_RELEASE)
#define ATOM_ADD_LOAD(x, v)     __atomic_add_fetch(x, v, __ATOMIC_ACQ_REL)
#define ATOM_LOAD_ADD(x, v)     __atomic_fetch_add(x, v, __ATOMIC_ACQ_REL)

#define ATOM_SUB_RELAXED(x, v)  __atomic_sub_fetch(x, v, __ATOMIC_RELAXED)
#define ATOM_SUB(x, v)          __atomic_sub_fetch(x, v, __ATOMIC_RELEASE)
#define ATOM_SUB_LOAD(x, v)     __atomic_sub_fetch(x, v, __ATOMIC_ACQ_REL)
#define ATOM_LOAD_SUB(x, v)     __atomic_fetch_sub(x, v, __ATOMIC_ACQ_REL)

#define ATOM_AND(x, v)          __atomic_and_fetch(x, v, __ATOMIC_RELEASE)
#define ATOM_AND_LOAD(x, v)     __atomic_and_fetch(x, v, __ATOMIC_ACQ_REL)
#define ATOM_LOAD_AND(x, v)     __atomic_fetch_and(x, v, __ATOMIC_ACQ_REL)

#define ATOM_OR(x, v)           __atomic_or_fetch(x, v, __ATOMIC_RELEASE)
#define ATOM_OR_LOAD(x, v)      __atomic_or_fetch(x, v, __ATOMIC_ACQ_REL)
#define ATOM_LOAD_OR(x, v)      __atomic_fetch_or(x, v, __ATOMIC_ACQ_REL)

#define ATOM_XOR(x, v)          __atomic_xor_fetch(x, v, __ATOMIC_RELEASE)
#define ATOM_XOR_LOAD(x, v)     __atomic_xor_fetch(x, v, __ATOMIC_ACQ_REL)
#define ATOM_LOAD_XOR(x, v)     __atomic_fetch_xor(x, v, __ATOMIC_ACQ_REL)

#define ATOM_XCHG(x, v)         __atomic_exchange_n(x, v, __ATOMIC_ACQ_REL)
#define ATOM_CMP_XCHG(x, e, v)  \
__atomic_compare_exchange_n(x, e, v, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)

#define GOLDEN_RATIO_32         0x61C88647
#define GOLDEN_RATIO_64         0x61C8864680B583EBULL

static inline uint32_t hash32(uint32_t val, unsigned int bits)
{
       return val * GOLDEN_RATIO_32 >> (32 - bits);
}

static inline uint32_t hash64(uint64_t val, unsigned int bits)
{
       return val * GOLDEN_RATIO_64 >> (64 - bits);
}

static inline void swap_dat(uint8_t *buf, int len)
{
    while (len >= 16) {
        *(uint64_t *)buf ^= *(uint64_t *)(buf + len - 8);
        *(uint64_t *)(buf + len - 8) ^= *(uint64_t *)buf;
        *(uint64_t *)buf ^= *(uint64_t *)(buf + len - 8);
        *(uint64_t *)buf = swap64(*(uint64_t *)buf);
        *(uint64_t *)(buf + len - 8) =
            swap64(*(uint64_t *)(buf + len - 8));
        buf += 8;
        len -= 16;
    }
    switch (len) {
    case 8 ... 15:
        *(uint32_t *)buf ^= *(uint32_t *)(buf + len - 4);
        *(uint32_t *)(buf + len - 4) ^= *(uint32_t *)buf;
        *(uint32_t *)buf ^= *(uint32_t *)(buf + len - 4);
        *(uint32_t *)buf = swap32(*(uint32_t *)buf);
        *(uint32_t *)(buf + len - 4) =
            swap32(*(uint32_t *)(buf + len - 4));
        buf += 4;
        len -= 8;
        break;
    }
    switch (len) {
    case 4 ... 7:
        *(uint16_t *)buf ^= *(uint16_t *)(buf + len - 2);
        *(uint16_t *)(buf + len - 2) ^= *(uint16_t *)buf;
        *(uint16_t *)buf ^= *(uint16_t *)(buf + len - 2);
        *(uint16_t *)buf = swap16(*(uint16_t *)buf);
        *(uint16_t *)(buf + len - 2) =
            swap16(*(uint16_t *)(buf + len - 2));
        buf += 2;
        len -= 4;
        break;
    }
    switch (len) {
    case 2 ... 3:
        buf[0] ^= buf[len - 1];
        buf[len - 1] ^= buf[0];
        buf[0] ^= buf[len - 1];
        break;
    }
}

static inline int strmatch(char *dst, const char *src)
{
    if (strlen(dst) < strlen(src))
        return 0;
    return memcmp(dst, src, strlen(src)) == 0;
}

static inline uint8_t __str2u8(char **str, int *err)
{
    uint16_t ret = 0;
    *err = 1;
    while (isdigit((int)**str)) {
        *err = 0;
        ret = ret * 10 + **str - '0';
        if (ret > U8_MAX) {
            *err = 1;
            return 0;
        }
        (*str)++;
    }
    return ret & U8_MAX;
}

static inline uint8_t str2u8(char *str, int *err)
{
    return __str2u8(&str, err);
}

static inline uint16_t __str2u16(char **str, int *err)
{
    uint32_t ret = 0;
    *err = 1;
    while (isdigit((int)**str)) {
        *err = 0;
        ret = ret * 10 + **str - '0';
        if (ret > U16_MAX) {
            *err = 1;
            return 0;
        }
        (*str)++;
    }
    return ret & U16_MAX;
}

static inline uint16_t str2u16(char *str, int *err)
{
    return __str2u16(&str, err);
}

static inline uint32_t __str2u32(char **str, int *err)
{
    uint64_t ret = 0;
    *err = 1;
    while (isdigit((int)**str)) {
        *err = 0;
        ret = ret * 10 + **str - '0';
        if (ret > U32_MAX) {
            *err = 1;
            return 0;
        }
        (*str)++;
    }
    return ret & U32_MAX;
}

static inline uint32_t str2u32(char *str, int *err)
{
    return __str2u32(&str, err);
}

static inline uint32_t __dotted2u32(char **str, int *err)
{
    uint64_t ret = 0;
    int i = 4;
    *err = 0;
    do {
        i--;
        ret = __str2u8(str, err) + ret * 256;
        if (*err || ret > U32_MAX)
            return 0;
        if (**str == '.')
            (*str)++;
    } while (i);
    if (i != 0)
        *err = 1;
    return ret & U32_MAX;
}

static inline uint32_t dotted2u32(char *str, int *err)
{
    return __dotted2u32(&str, err);
}

static inline uint32_t str2maskip(uint32_t *mask, char *str, int *err)
{
    uint32_t ip = __dotted2u32(&str, err);

    if (*err)
        return 0;
    if (*str != '/') {
        *err = 1;
        return 0;
    }
    ++str;
    if (strlen(str) < 3) {
        uint8_t bit_width = str2u8(str, err);
        if (*err || bit_width > 32)
            return 0;
        *mask = ~0U << (32 - bit_width);
    } else {
        *mask = dotted2u32(str, err);
        if (*err)
            return 0;
    }
    return ip;
}

static inline uint8_t __hextou8(char *str, int *err)
{
    uint8_t ret = 0;
    *err = 0;
    switch (*str) {
    case '0' ... '9':
        ret = (*str - '0') << 4;
        break;
    case 'A' ... 'F':
        ret = (*str - 'A' + 10) << 4;
        break;
    case 'a' ... 'f':
        ret = (*str - 'a' + 10) << 4;
        break;
    default:
        *err = 1;
        return 0;
    }
    ++str;
    switch (*str) {
    case '0' ... '9':
        ret |= (*str - '0');
        break;
    case 'A' ... 'F':
        ret |= (*str - 'A' + 10);
        break;
    case 'a' ... 'f':
        ret |= (*str - 'a' + 10);
        break;
    default:
        *err = 1;
        return 0;
    }
    return ret;
}

static inline void str2mac(uint8_t *dst, char *str, int *err)
{
    int i;

    *err = 0;
    for (i = 0; i < 6; i++) {
        switch (*str) {
        case ':':
        case '-':
            ++str;
            break;
        case '0' ... '9':
        case 'A' ... 'F':
        case 'a' ... 'f':
            break;
        default:
            *err = 1;
            return;
        }
        dst[i] = __hextou8(str, err);
        if (*err)
            return;
        str += 2;
    }
}

static inline uint64_t get_gcd(uint64_t x, uint64_t y)
{
    if (x < y) {
        x ^= y;
        y ^= x;
        x ^= y;
    }
    while (y) {
        uint64_t r = x % y;
        x = y;
        y = r;
    }
    return x;
}

static inline uint64_t get_lcm(uint64_t x, uint64_t y)
{
    return (x * y) / get_gcd(x, y);
}

#define bits_reverse(x)       ({                                            \
    typeof(x) _x = (x), _m = (typeof(x))~0ULL;                              \
    unsigned _s = sizeof(_x) * 8;                                           \
    while ((_s >>= 1)) {                                                    \
        _m ^= (_m << _s);                                                   \
        _x = ((_x >> _s) & _m) | ((_x << _s) & ~_m);                        \
    } _x; })

#endif /* __COMMON_H */

