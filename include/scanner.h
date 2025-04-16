#ifndef SCANNER_H
#define SCANNER_H

#include <stddef.h>

typedef struct {
    const char *path;         // Directory path being scanned
    size_t totalFiles;        // Count of regular files
    unsigned long totalSize;  // Sum of st_size for all regular files
} DirectoryInfo;

/**
 * scan_directory:
 *   Scans the given directory (non-recursively) and:
 *     - increments info->totalFiles for each regular file
 *     - adds st_size to info->totalSize
 *   On failure to open directory: logs a warning and returns -1.
 *   On success returns 0.
 */
int scan_directory(const char *path, DirectoryInfo *info);

#endif // SCANNER_H