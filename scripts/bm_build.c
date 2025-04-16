#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#ifndef PRJ_VERSION
#define PRJ_VERSION     "(Build "__DATE__" "__TIME__")"
#endif

static int __verbose;
static int ignorecase;
static char *in_pattern;

/* Alphabet size, use ASCII */
#define ASIZE   256

#define max_t(type, x, y)                                                   \
    ({ type __max1 = (x); type __max2 = (y);                                \
     __max1 > __max2 ? __max1 : __max2; })

static inline uint8_t __hextou8(const char *str, int *err)
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

static int parse_char(uint8_t *dst, const char *src)
{
    int err, len = 0;
    while (isprint(*src)) {
        if (*src == '\\') {
            switch (*(++src)) {
            case 'a':
                *dst = 0x07;
                break;
            case 'b':
                *dst = 0x08;
                break;
            case 'f':
                *dst = 0x0C;
                break;
            case 'n':
                *dst = 0x0A;
                break;
            case 'r':
                *dst = 0x0D;
                break;
            case 't':
                *dst = 0x09;
                break;
            case 'v':
                *dst = 0x0B;
                break;
            case 'x':
                *dst = __hextou8((++src), &err);
                if (err)
                    return 0;
                ++src;
                break;
            default:
                return 0;
            }
        } else if (ignorecase)
            *dst = tolower(*src);
        else
            *dst = *src;

        src++;
        dst++;
        len++;
    }
    return len;
}

struct ts_bm
{
    uint8_t *pattern;
    uint32_t patlen;
    uint32_t bad_shift[ASIZE];
    uint32_t good_shift[0];
};

#define PATTERN_STR                                                         \
    ({ int i;                                                               \
     for (i = 0; i < bm->patlen; i++) {                                     \
        if (isalnum(bm->pattern[i]) || bm->pattern[i] == '_')               \
            printf("%c", bm->pattern[i]);                                   \
        else                                                                \
            printf("%02x", bm->pattern[i]); } })

static void build_file_pre(struct ts_bm *bm)
{
    printf("#ifndef __BM_");
    PATTERN_STR;
    printf("_INC\n");
    printf("#define __BM_");
    PATTERN_STR;
    printf("_INC\n\n");
    printf("#include <stdio.h>\n");
    printf("#include <stdlib.h>\n");
    printf("#include <stdint.h>\n");
    printf("#include <string.h>\n");
    printf("#include <ctype.h>\n\n");
}

static void build_file_post(struct ts_bm *bm)
{
    printf("static inline uint8_t *bm_find%s", ignorecase ? "_icase_" : "_");
    PATTERN_STR;
    printf("(const void *in, uint32_t *len)\n");
    printf("{\n");
    printf("\tuint32_t i, shift = %d, bs, gs;\n", bm->patlen - 1);
    printf("\tconst uint8_t *text = in, pattern[] = {");
    int i;
    for (i = 0; i < bm->patlen; i++)
        printf(" 0x%X,", bm->pattern[i]);
    printf("};\n");
    printf("\twhile (shift < *len) {\n");
    printf("\t\tfor (i = 0; i < %d; i++)\n", bm->patlen);
    if (ignorecase)
        printf("\t\t\tif (tolower(text[shift - i]) != pattern[%d - 1 - i])\n",
               bm->patlen);
    else
        printf("\t\t\tif (text[shift - i] != pattern[%d - 1 - i])\n",
               bm->patlen);
    printf("\t\t\t\tgoto next;\n");
    printf("\t\t*len = sizeof(pattern);\n");
    printf("\t\treturn (uint8_t *)text + shift - %d + 1;\n", bm->patlen);
    printf("next:\n");
    printf("\t\tbs = shift - i + get_bs_");
    PATTERN_STR;
    printf("(text[shift - i]);\n");
    printf("\t\tgs = shift + get_gs_");
    PATTERN_STR;
    printf("(i);\n");
    printf("\t\tshift = bs > gs ? bs : gs;\n");
    printf("\t}\n");
    printf("\treturn NULL;\n");
    printf("}\n");
    printf("#endif\n");
}

static void build_gs_pre(struct ts_bm *bm)
{
    printf("static inline int get_gs_");
    PATTERN_STR;
    printf("(int i)\n");
    printf("{\n");
    printf("\tswitch (i) {\n");
    printf("\tdefault: return %d;\n", bm->patlen);
    printf("\tcase 0: return 1;\n");
}

static void build_gs_post(void)
{
    printf("\t}\n");
    printf("}\n\n");
}

static void build_bs_pre(struct ts_bm *bm)
{
    printf("static inline int get_bs_");
    PATTERN_STR;
    printf("(int i)\n");
    printf("{\n");
    printf("\tswitch (i) {\n");
    printf("\tdefault: return %d;\n", bm->patlen);
}

static void build_bs_post(void)
{
    printf("\t}\n");
    printf("}\n\n");
}

static void build_shift(int i, int r)
{
    printf("\tcase %d: return %d;\n", i, r);
}

static int subpattern(uint8_t *pat, int i, int j, int g)
{
    int x = i+g-1, y = j+g-1, ret = 0;

    while(pat[x--] == pat[y--]) {
        if (y < 0) {
            ret = 1;
            break;
        }
        if (--g == 0) {
            ret = pat[i-1] != pat[j-1];
            break;
        }
    }

    return ret;
}

static void compute_prefix_tbl(struct ts_bm *bm)
{
    int i, j, g;

    for (i = 0; i < ASIZE; i++)
        bm->bad_shift[i] = bm->patlen;
    for (i = 0; i < bm->patlen - 1; i++) {
        bm->bad_shift[bm->pattern[i]] = bm->patlen - 1 - i;
        if (ignorecase)
            bm->bad_shift[toupper(bm->pattern[i])]
                = bm->patlen - 1 - i;
    }

    /* Compute the good shift array, used to match reocurrences
     * of a subpattern */
    bm->good_shift[0] = 1;
    for (i = 1; i < bm->patlen; i++)
        bm->good_shift[i] = bm->patlen;
    for (i = bm->patlen-1, g = 1; i > 0; g++, i--) {
        for (j = i-1; j >= 1-g ; j--)
            if (subpattern(bm->pattern, i, j, g)) {
                bm->good_shift[g] = bm->patlen-j-g;
                break;
            }
    }
}

static void build_file(struct ts_bm *bm)
{
    int i;

    build_file_pre(bm);
    build_bs_pre(bm);

    for (i = 0; i < ASIZE; i++) {
        if (bm->bad_shift[i] != bm->patlen)
            build_shift(i, bm->bad_shift[i]);
    }
    build_bs_post();

    /* Compute the good shift array, used to match reocurrences
     * of a subpattern */
    build_gs_pre(bm);
    for (i = 1; i < bm->patlen; i++) {
        if (bm->good_shift[i] != bm->patlen)
            build_shift(i, bm->good_shift[i]);
    }
    build_gs_post();
    build_file_post(bm);
}

static struct ts_bm *bm_init(const char *pattern)
{
    struct ts_bm *bm;
    int i, len = strlen(pattern) + 1;
    unsigned int prefix_tbl_len = len * sizeof(uint32_t);
    size_t priv_size = sizeof(*bm) + len + prefix_tbl_len;

    bm = calloc(1, priv_size);
    bm->pattern = (uint8_t *) bm->good_shift + prefix_tbl_len;
    memcpy(bm->pattern, pattern, len);
    bm->patlen = parse_char(bm->pattern, pattern);

    if (bm->patlen == 0) {
        free(bm);
        return NULL;
    }

    compute_prefix_tbl(bm);

    if (__verbose) {
        fprintf(stderr, "%u-%s[%zu]\n", bm->patlen, bm->pattern, priv_size);
        fprintf(stderr, "Bad Shift:\n");
        for (i = 0; i < ASIZE; i++) {
            if ((i % 16) == 0)
                fprintf(stderr, "\n");
            fprintf(stderr, "%d ", bm->bad_shift[i]);
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "Good Shift:\n");
        for (i = 0; i < len; i++) {
            if ((i % 16) == 0)
                fprintf(stderr, "\n");
            fprintf(stderr, "%d ", bm->good_shift[i]);
        }
        fprintf(stderr, "\n");
    }
    return bm;
}

static void usage(void)
{
    printf("bm_build V%s usage:\n"
           "\t--help|-h            Usage\n"
           "\t--version|-V         Version\n"
           "\t--verbose|-v         Verbose\n"
           "\t--ignorecase|-i      Ignore Case in Pattern String\n"
           "\t<Pattern String>     Pattern String\n"
           , PRJ_VERSION);
    exit(-1);
}

static void parse_options(int argc, char **argv)
{
    int c;
    struct option longopts[] = {
        { "help", no_argument, NULL, 'h' },
        { "version", no_argument, NULL, 'V' },
        { "verbose", no_argument, NULL, 'v' },
        { "ignorecase", no_argument, NULL, 'i' },
        { NULL, 0, NULL, 0 }
    };

get_next_arg:
    while ((c = getopt_long(argc, argv, "hVvi", longopts, NULL)) != -1) {
        switch (c) {
        case 'i':
            ignorecase = 1;
            break;
        case 'v':
            __verbose = 1;
            break;
        case 'V':
            printf("bm_build Version: %s\n", PRJ_VERSION);
            exit(0);
        case 'h':
            usage();
        default:
            break;
        }
    }
    if (optind < argc && !in_pattern) {
        in_pattern = strdup(argv[optind]);
        goto get_next_arg;
    }
}

int main(int argc, char *argv[])
{
    struct ts_bm *bm;
    if (argc < 2)
        usage();
    parse_options(argc, argv);
    bm = bm_init(in_pattern);
    if (bm) {
        build_file(bm);
        fprintf(stderr, "Build BM for `%s' Done\n", in_pattern);
        return EXIT_SUCCESS;
    }
    fprintf(stderr, "Pattern Error.\n");
    return EXIT_FAILURE;
}
