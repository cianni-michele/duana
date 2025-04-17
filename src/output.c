#include "output.h"
#include <stdio.h>

void print_summary(const DirectoryInfo* info) {
    printf("Directory: %s\n", info->path);
    printf("Total files: %zu\n", info->totalFiles);
    printf("Total size: %lu bytes\n", info->totalSize);
}