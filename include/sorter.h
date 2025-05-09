#ifndef SORTER_H
#define SORTER_H

#include "options.h"
#include "stats_collector.h"
#include <stddef.h>

/**
 * Sort an array of directory statistics in-place.
 *
 * Stable, insertion-sort based on the options:
 *   - opt->sort_field == DU_SORT_SIZE  → sort by total_size
 *   - opt->sort_field == DU_SORT_COUNT → sort by file_count
 *   - opt->sort_desc                  → descending if true, ascending otherwise
 *
 * @param arr   Array of DirStatsArr elements to sort.
 * @param count Number of elements in arr.
 * @param opt   Sorting options (must not be NULL).
 */
void du_sort_dirs(DirStatsArr* arr, size_t count, const DuOptions* opt);

/**
 * Sort an array of extension statistics in-place.
 *
 * Same semantics as du_sort_dirs, but operates on ExtStatsArr.
 *
 * @param arr   Array of ExtStatsArr elements to sort.
 * @param count Number of elements in arr.
 * @param opt   Sorting options (must not be NULL).
 */
void du_sort_exts(ExtStatsArr* arr, size_t count, const DuOptions* opt);

#endif /* SORTER_H */
