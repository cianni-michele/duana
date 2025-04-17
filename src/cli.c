#include "cli.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_cli(int argc, char* argv[], char** rootPath, Config* cfg) {
    config_init(cfg);
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <dir> [--types=.ext1, .ext2,...]\n", argv[0]);
        return -1;
    }
    *rootPath = argv[1];
    for (int i = 2; i < argc; ++i) {
        if (strncmp(argv[i], "--types=", 8) == 0) {
            cfg->filterExtensions = true;
            char* list = strdup(argv[i] + 8);
            // count commas
            size_t cnt = 1;
            for (char* p = list; *p; ++p)
                if (*p == ',')
                    cnt++;
            cfg->extensionsCount = cnt;
            cfg->extensions = calloc(cnt, sizeof(char*));
            size_t idx = 0;
            char* tok = strtok(list, ",");
            while (tok) {
                if (tok[0] != '.') {
                    size_t len = strlen(tok) + 2;
                    cfg->extensions[idx] = malloc(len);
                    snprintf(cfg->extensions[idx], len, ".%s", tok);
                } else {
                    cfg->extensions[idx] = strdup(tok);
                }
                tok = strtok(NULL, ",");
                idx++;
            }
            free(list);
        }
    }
    return 0;
}