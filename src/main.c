#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"

int main(int argc, char *argv[]) {
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    DirectoryInfo info = { .path = argv[1], .totalFiles = 0 };
    if (scan_directory(info.path, &info) < 0) 
    {
        fprintf(stderr, "Error scanning directory '%s'\n", info.path);
        return EXIT_FAILURE;
    }

    printf("Directory: %s\n", info.path);
    printf("Total files: %zu\n", info.totalFiles);
    return EXIT_SUCCESS;
}