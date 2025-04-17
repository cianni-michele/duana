#ifndef SCANNER_H
#define SCANNER_H

#include "duana_config.h"
#include <stddef.h>

typedef struct {
    const char *path;         // Directory path being scanned
    size_t totalFiles;        // Count of regular files
    unsigned long totalSize;  // Sum of st_size for all regular files
} DirectoryInfo;

/**
 * Scans directory non-recursively, counts files, sizes, applying extension filter.
 * Returns -1 on opendir() failure, 0 otherwise.
 */
int scan_directory(const char *path, DirectoryInfo *info, const Config *config);

#endif // SCANNER_H