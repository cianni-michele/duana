#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "duana_config.h"

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        fprintf(stderr, "Usage: %s <directory> [--types=.ext1,.ext2]\n", argv[0]);
        return EXIT_FAILURE;
    }

    Config config = { .filterExtensions = false,
                      .extensions = NULL,
                      .extensionsCount = 0 };
                      
    // Parse --types
    for (int i = 2; i < argc; i++) 
    {
        if (strncmp(argv[i], "--types=", 8) == 0) 
        {
            config.filterExtensions = true;
            char *list = argv[i] + 8;
            // Count commas
            size_t count = 1;
            for (char *p = list; *p; p++) 
            {
                if (*p == ',') count++;
            }
            config.extensionsCount = count;
            config.extensions = malloc(count * sizeof(char*));
            size_t idx = 0;
            char *token = strtok(list, ",");
            while (token) 
            {
                if (token[0] != '.') 
                {
                    // prepend dot
                    size_t len = strlen(token) + 2;
                    config.extensions[idx] = malloc(len);
                    snprintf(config.extensions[idx], len, ".%s", token);
                } 
                else 
                {
                    config.extensions[idx] = strdup(token);
                }
                idx++;
                token = strtok(NULL, ",");
            }
        }
    }

    DirectoryInfo info = { .path = argv[1],
                           .totalFiles = 0,
                           .totalSize = 0 };

    if (scan_directory(info.path, &info, &config) < 0) 
    {
        fprintf(stderr, "Error scanning directory '%s'\n", info.path);
        return EXIT_FAILURE;
    }

    printf("Directory: %s\n", info.path);
    printf("Total files: %zu\n", info.totalFiles);
    printf("Total size: %lu bytes\n", info.totalSize);

    // Free extensions
    for (size_t i = 0; i < config.extensionsCount; i++) 
    {
        free(config.extensions[i]);
    }
    free(config.extensions);
    return EXIT_SUCCESS;
}