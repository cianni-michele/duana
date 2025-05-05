#include "report_writer.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>

// Helper to format a double byte size into human-readable string
static char *double_format_size(const double bytes) {
    const char *unit = "B";
    double value = bytes;
    if (value >= 1024) {
        value /= 1024;
        unit = "KiB";
        if (value >= 1024) {
            value /= 1024;
            unit = "MiB";
        }
    }
    char buf[64];
    const int len = snprintf(buf, sizeof(buf), "%.2f %s", value, unit);
    if (len < 0) {
        return NULL;
    }
    char *res = malloc((size_t)len + 1);
    if (!res) {
        du_perror("double_format_size");
        return NULL;
    }
    strcpy(res, buf);
    return res;
}

int du_report_print(const DirStatsArr *dirs, const ExtStatsArr *exts, const double avg) {
    if (!dirs || !exts) {
        errno = EINVAL;
        return -1;
    }
    // Section: directories
    if (fprintf(stdout, "Directories:\n") < 0) return -1;
    for (size_t i = 0; dirs[i].path != NULL; i++) {
        char *size_str = du_format_size(dirs[i].total_size);
        if (!size_str) return -1;
        if (fprintf(stdout, "%-30s %10zu %10s\n",
                    dirs[i].path, dirs[i].file_count, size_str) < 0) {
            free(size_str);
            return -1;
                    }
        free(size_str);
    }
    // Section: extensions
    if (fprintf(stdout, "\nExtensions:\n") < 0) return -1;
    for (size_t i = 0; exts[i].ext != NULL; i++) {
        char *size_str = du_format_size(exts[i].total_size);
        if (!size_str) return -1;
        if (fprintf(stdout, "%-30s %10zu %10s\n",
                    exts[i].ext, exts[i].file_count, size_str) < 0) {
            free(size_str);
            return -1;
                    }
        free(size_str);
    }
    // Average line
    if (fprintf(stdout, "\n") < 0) return -1;
    char *avg_str = double_format_size(avg);
    if (!avg_str) return -1;
    if (fprintf(stdout, "Media: %s\n", avg_str) < 0) {
        free(avg_str);
        return -1;
    }
    free(avg_str);
    return 0;
}