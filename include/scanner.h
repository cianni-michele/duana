#ifndef SCANNER_H
#define SCANNER_H

#include <stddef.h>

typedef struct {
    const char *path;
    size_t totalFiles;
} DirectoryInfo;

/**
 * scan_directory:
 *   Scans the given directory (non-recursively) and increments
 *   info->totalFiles for each regular file found.
 *   On directory-open failure, logs a warning and returns -1.
 *   Otherwise returns 0.
 */
int scan_directory(const char *path, DirectoryInfo *info);

#endif // SCANNER_H