#include "config.h"
#include <stdlib.h>
#include <string.h>

void config_init(Config* cfg) {
    cfg->filterExtensions = false;
    cfg->extensions = NULL;
    cfg->extensionsCount = 0;
}

void config_free(Config* cfg) {
    for (size_t i = 0; i < cfg->extensionsCount; ++i) {
        free(cfg->extensions[i]);
    }
    free(cfg->extensions);
}