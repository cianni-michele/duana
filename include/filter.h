#ifndef FILTER_H
#define FILTER_H

#include <stdbool.h>
#include "options.h"

/**
 * @brief Initializes filters for file extensions.
 * @param csv CSV string of extensions (e.g. ".txt,.c"). If NULL, no filter is applied.
 * @param opt Pointer to DuOptions where filters will be saved.
 * @return 0 on success, -1 on error (errno set).
 */
int du_filter_init(const char *csv, DuOptions *opt);

/**
 * @brief Checks if a specific extension should be included according to the filters.
 * @param opt Pointer to DuOptions with initialized filters.
 * @param ext Extension to check (e.g. ".txt").
 * @return true if the extension is allowed or if there are no filters, false otherwise.
 */
bool du_filter_match(const DuOptions *opt, const char *ext);

/**
 * @brief Frees all resources allocated for the filters.
 * @param opt Pointer to DuOptions whose filters need to be freed.
 * 
 * This function is idempotent: it can be safely called multiple times.
 */
void du_filter_clear(DuOptions *opt);

#endif // FILTER_H