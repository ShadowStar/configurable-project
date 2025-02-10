/*
 * =============================================================================
 *
 *       Filename:  tlv.h
 *
 *    Description:  Tag-Length-Value
 *
 *        Version:  1.0
 *        Created:  01/15/2025 10:23:39
 *    Last Change:  02/10/2025 13:29:40
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ShadowStar, <orphen.leiliu@gmail.com>
 *   Organization:  Gmail
 *
 * =============================================================================
 */
#ifndef __TLV_INC__
#define __TLV_INC__
#include <stdint.h>
#include "common.h"

typedef struct {
    uint16_t tag;
    uint32_t len;
    uint8_t value[0];
} __attribute__ ((packed)) tlv_t;
#define TLV_HDR_SIZE        sizeof(tlv_t)

#define TLV_BE

#ifdef TLV_BE
#define _L(x)                                                               \
    __builtin_choose_expr(sizeof(x) == 8, be64_to_cpu(x),                   \
    __builtin_choose_expr(sizeof(x) == 4, be32_to_cpu(x),                   \
    __builtin_choose_expr(sizeof(x) == 2, be16_to_cpu(x), (x))))
#define _S(x, v)                                                            \
    __builtin_choose_expr(sizeof(x) == 8, (x) = cpu_to_be64(v),             \
    __builtin_choose_expr(sizeof(x) == 4, (x) = cpu_to_be32(v),             \
    __builtin_choose_expr(sizeof(x) == 2, (x) = cpu_to_be16(v), (x) = (v))))
#else
#define _L(x)                                                               \
    __builtin_choose_expr(sizeof(x) == 8, le64_to_cpu(x),                   \
    __builtin_choose_expr(sizeof(x) == 4, le32_to_cpu(x),                   \
    __builtin_choose_expr(sizeof(x) == 2, le16_to_cpu(x), (x))))
#define _S(x, v)                                                            \
    __builtin_choose_expr(sizeof(x) == 8, (x) = cpu_to_le64(v),             \
    __builtin_choose_expr(sizeof(x) == 4, (x) = cpu_to_le32(v),             \
    __builtin_choose_expr(sizeof(x) == 2, (x) = cpu_to_le16(v), (x) = (v))))
#endif

static inline uint16_t tlv_tag(tlv_t *t)
{
    return _L(t->tag);
}

static inline uint32_t tlv_len(tlv_t *t)
{
    return _L(t->len);
}

static inline uint8_t *tlv_value(tlv_t *t)
{
    return t->value;
}

static inline uint32_t tlv_size(tlv_t *t)
{
    return TLV_HDR_SIZE + _L(t->len);
}

static inline int tlv_valid(void *ptr, uint32_t size)
{
    tlv_t *t = ptr;
    return TLV_HDR_SIZE <= size && (tlv_size(t)) <= size;
}

static inline uint32_t tlv_remain(void *ptr, uint32_t size)
{
    tlv_t *t = ptr;

    if (size < TLV_HDR_SIZE)
        return TLV_HDR_SIZE - size;
    else if (size < tlv_size(t))
        return tlv_size(t) - size;
    else
        return 0;
}

static inline tlv_t *tlv_next(tlv_t *t)
{
    return (void *)((uint8_t *)t + tlv_size(t));
}

static inline tlv_t *tlv_search(tlv_t *t, uint16_t tag, uint32_t size)
{
    while (tlv_valid(t, size)) {
        if (_L(t->tag) == tag)
            return t;
        size -= tlv_size(t);
        t = tlv_next(t);
    }
    return NULL;
}

static inline void tlv_store_len(tlv_t *t, uint32_t len)
{
    _S(t->len, len);
}

static inline void tlv_store_tag(tlv_t *t, uint16_t tag)
{
    _S(t->tag, tag);
}

static inline uint32_t tlv_store(void *ptr, uint32_t size, uint16_t tag,
                                 uint32_t len, void *dat)
{
    if (size >= (TLV_HDR_SIZE + len)) {
        tlv_t *t = ptr;
        tlv_store_tag(t, tag);
        tlv_store_len(t, len);
        if (len)
            memcpy(t->value, dat, len);
        return TLV_HDR_SIZE + len;
    }
    return 0;
}
#endif   /* ----- end of header file __TLV_INC__  ----- */

