#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

static const size_t SIZE_BUFFER_LEN = 32;

char *du_format_size(const off_t bytes) {
    const char *units[] = {"B", "KiB", "MiB", "GiB"};
    double size = (double) bytes;
    int unit = 0;
    while (size >= 1024.0 && unit < 3) {
        size /= 1024.0;
        unit++;
    }
    
    char *buf = du_xmalloc(SIZE_BUFFER_LEN);
    snprintf(buf, SIZE_BUFFER_LEN, "%.2f %s", size, units[unit]);
    return buf;
}

char *du_get_extension(const char *filename) {
    if (!filename) return NULL;

    const char *base = strrchr(filename, '/');
    const char *start = base ? base + 1 : filename;
    const char *ext = strrchr(start, '.');

    if (!ext || ext == start || *(ext + 1) == '\0') return NULL;

    const size_t len = strlen(ext);
    char *out = du_xmalloc(len + 1);

    for (size_t i = 0; i < len; i++) {
        out[i] = tolower((unsigned char)ext[i]);
    }
    out[len] = '\0';
    return out;
}

void du_perror(const char *ctx) {
    if (!ctx) ctx = "";
    fprintf(stderr, "duana: %s: ", ctx);
    perror(NULL);
}

void *du_xmalloc(const size_t size) {
    void *p = malloc(size);
    if (!p) {
        fprintf(stderr, "duana: malloc failed\n");
        exit(1);
    }
    return p;
}