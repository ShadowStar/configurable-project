/*
 * =============================================================================
 *
 *       Filename:  bm.c
 *
 *    Description:  Boyer-Moore text search implementation
 *
 *        Version:  1.0
 *        Created:  2012/05/30 11:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ShadowStar, <orphen.leiliu@gmail.com>
 *   Organization:  Gmail
 *
 * =============================================================================
 */
#include <stdlib.h>
#include <string.h>
#include "bm.h"

static inline int subpattern(uint8_t *pattern, int i, int j, int g)
{
    int x = i + g - 1, y = j + g - 1, ret = 0;

    while (pattern[x--] == pattern[y--]) {
        if (y < 0) {
            ret = 1;
            break;
        }
        if (--g == 0) {
            ret = pattern[i - 1] != pattern[j - 1];
            break;
        }
    }

    return ret;
}

static inline void compute_prefix_tbl(struct ts_bm *bm)
{
    int i, j, g;

    for (i = 0; i < ASIZE; i++)
        bm->bad_shift[i] = bm->patlen;
    for (i = 0; i < bm->patlen - 1; i++) {
        bm->bad_shift[bm->pattern[i]] = bm->patlen - 1 - i;
        if (bm->icase)
            bm->bad_shift[tolower(bm->pattern[i])] =
                bm->patlen - 1 - i;
    }

    /* Compute the good shift array, used to match reocurrences
     * of a subpattern */
    bm->good_shift[0] = 1;
    for (i = 1; i < bm->patlen; i++)
        bm->good_shift[i] = bm->patlen;
    for (i = bm->patlen - 1, g = 1; i > 0; g++, i--) {
        for (j = i - 1; j >= 1 - g; j--)
            if (subpattern(bm->pattern, i, j, g)) {
                bm->good_shift[g] = bm->patlen - j - g;
                break;
            }
    }
}

struct ts_bm *bm_init(const uint8_t *pattern, uint8_t len, int icase)
{
    int i;
    struct ts_bm *bm = calloc(1, sizeof(struct ts_bm));
    if (bm == NULL)
        return NULL;
    bm->patlen = len;
    bm->icase = icase;
    if (icase)
        for (i = 0; i < len; i++)
            bm->pattern[i] = toupper(pattern[i]);
    else
        memcpy(bm->pattern, pattern, len);
    compute_prefix_tbl(bm);

    return bm;
}

