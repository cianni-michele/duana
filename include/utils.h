#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

/// Returns true if filename ends in one of ext_list (size ext_count).
bool match_extension(const char* filename, char** ext_list, size_t ext_count);

#endif // UTILS_H