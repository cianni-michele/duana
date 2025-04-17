#ifndef SCANNER_H
#define SCANNER_H

#include "config.h"
#include <stddef.h>

// Directory statistics
typedef struct {
    const char* path;
    size_t totalFiles;
    unsigned long totalSize;
} DirectoryInfo;

/**
 * Scan directory non-recursively, filtering and accumulating stats.
 * @return 0 on success, -1 on opendir failure.
 */
int scan_directory(const char* path, DirectoryInfo* info, const Config* cfg);

#endif // SCANNER_H