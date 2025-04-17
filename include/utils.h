#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

/**
 * Checks if filename ends with one of ext_list[] entries.
 * If filterExtensions is false (ext_count == 0), returns true.
 */
bool match_extension(const char *filename, char **ext_list, size_t ext_count);

#endif // UTILS_H