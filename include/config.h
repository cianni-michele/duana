#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stddef.h>

// Parsed CLI configuration
typedef struct {
    bool filterExtensions;  // true if --types passed
    char** extensions;      // array of ".ext" strings
    size_t extensionsCount; // number of extensions
} Config;

// Initialize config to defaults
void config_init(Config* cfg);

// Release dynamic memory in config
void config_free(Config* cfg);

#endif // CONFIG_H