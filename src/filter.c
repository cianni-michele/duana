#include "filter.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "utils.h"
#include "options.h"

static char* str_tolower(const char *str) {
    const size_t len = strlen(str);
    char *lower = du_xmalloc(len + 1);
    for (size_t i = 0; i < len; ++i) {
        lower[i] = tolower((unsigned char)str[i]);
    }
    lower[len] = '\0';
    return lower;
}

static void free_extension_array(const size_t count, char **exts) {
    if (!exts) return;
    for (size_t i = 0; i < count; ++i) {
        free(exts[i]);
    }
    free(exts);
}

static int handle_filter_error(const size_t count, char **exts) {
    errno = EINVAL;
    du_perror("du_filter_init");
    free_extension_array(count, exts);
    return -1;
}

int du_filter_init(const char *csv, DuOptions *opt) {
    if (!opt) {
        errno = EINVAL;
        du_perror("du_filter_init");
        return -1;
    }

    du_filter_clear(opt);

    if (csv == NULL) {
        opt->filter_exts = NULL;
        opt->filter_count = 0;
        return 0;
    }
    if (csv[0] == '\0') {
        return handle_filter_error(0, NULL);
    }

    const size_t len = strlen(csv);
    size_t count = 1;
    for (size_t i = 0; i < len; ++i) {
        if (csv[i] == ',') count++;
    }

    char **exts = du_xmalloc(count * sizeof(char *));
    size_t idx = 0;
    const char *start = csv;

    for (size_t i = 0; i <= len; ++i) {
        if (csv[i] == ',' || csv[i] == '\0') {
            const size_t seglen = &csv[i] - start;
            if (seglen == 0) {
                return handle_filter_error(idx, exts);
            }

            char *token = du_xmalloc(seglen + 1);
            memcpy(token, start, seglen);
            token[seglen] = '\0';

            for (size_t k = 0; k < seglen; ++k) {
                token[k] = tolower((unsigned char)token[k]);
            }

            if (token[0] != '.') {
                free(token);
                return handle_filter_error(idx, exts);
            }
            exts[idx++] = token;
            start = csv + i + 1;
        }
    }

    opt->filter_exts = exts;
    opt->filter_count = idx;
    return 0;
}

bool du_filter_match(const DuOptions *opt, const char *ext) {
    if (!opt || !ext) return false;
    if (opt->filter_count == 0) {
        return true;
    }

    char *lower = str_tolower(ext);
    bool found = false;

    for (size_t i = 0; i < opt->filter_count; ++i) {
        if (strcmp(lower, opt->filter_exts[i]) == 0) {
            found = true;
            break;
        }
    }

    free(lower);
    return found;
}

void du_filter_clear(DuOptions *opt) {
    if (!opt) return;
    if (opt->filter_exts) {
        free_extension_array(opt->filter_count, opt->filter_exts);
        opt->filter_exts = NULL;
    }
    opt->filter_count = 0;
}
