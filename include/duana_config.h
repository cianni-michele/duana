#ifndef DUANA_CONFIG_H
#define DUANA_CONFIG_H

#include <stddef.h>
#include <stdbool.h>

// Configuration parsed from CLI
typedef struct {
    bool filterExtensions;      // true if --types was specified
    char **extensions;          // list of extensions (each starting with '.')
    size_t extensionsCount;     // number of extensions
} Config;


#endif // DUANA_CONFIG_H