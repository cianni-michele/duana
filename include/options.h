#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Sorting fields for disk usage report.
 */
typedef enum {
    DU_SORT_SIZE,
    DU_SORT_COUNT
} DuSortField;

/**
 * Command-line options structure.
 */
typedef struct {
    char *path;             // Directory path to analyze
    DuSortField sort_field; // Field to sort by (size or count)
    bool sort_desc;         // Sort descending if true
    bool json;              // Export JSON if true
    char *json_file;        // JSON output file (NULL -> stdout)
    char **filter_exts;     // Array of file extensions to include
    size_t filter_count;    // Number of extensions in filter_exts
} DuOptions;

/**
 * Initialize DuOptions with default values.
 * This function should be called before using DuOptions.
 */
void du_options_init(DuOptions *opt);

/**
 * Free resources allocated in DuOptions.
 */
void du_options_free(DuOptions *opt);

#endif // OPTIONS_H