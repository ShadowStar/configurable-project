/*
 * =============================================================================
 *
 *       Filename:  bm.h
 *
 *    Description:  Boyer-Moore text search implementation
 *
 *        Version:  1.0
 *        Created:  2012/05/30 11:47
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: ShadowStar, <orphen.leiliu@gmail.com>
 *   Organization: Gmail
 *
 * =============================================================================
 */

#ifndef  BM_INC
#define  BM_INC
#include <stdint.h>
#include <ctype.h>

/* Alphabet size, use ASCII */
#define ASIZE 256

#define max_t(type, x, y)                                                   \
    ({ type __max1 = (x); type __max2 = (y);                                \
     __max1 > __max2 ? __max1 : __max2; })

struct ts_bm {
    uint8_t pattern[127];    // Limited to align structure size
    uint8_t patlen;
    uint8_t good_shift[127];// The same reason above
    uint8_t icase;
    uint8_t bad_shift[ASIZE];
};

static inline uint8_t *bm_find(struct ts_bm *bm, const uint8_t *text,
                               int32_t text_len)
{
    int i, shift = bm->patlen - 1, bs;
    const int icase = bm->icase;

    while (shift < text_len) {
        for (i = 0; i < bm->patlen; i++)
            if ((icase ? toupper(text[shift - i]) : text[shift - i])
                != bm->pattern[bm->patlen - 1 - i])
                goto next;

        return (uint8_t *) text + (shift - (bm->patlen - 1));

next:
        bs = bm->bad_shift[text[shift - i]];

        /* Now jumping to... */
        shift = max_t(int, shift - i + bs, shift + bm->good_shift[i]);
    }

    return NULL;
}

extern struct ts_bm *bm_init(const uint8_t * pattern, uint8_t len, int icase);

#endif /* ----- #ifndef BM_INC  ----- */
