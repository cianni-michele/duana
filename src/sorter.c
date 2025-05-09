#include "sorter.h"
#include <stddef.h>

static int compare_dirs(const DirStatsArr *a,
                        const DirStatsArr *b,
                        const DuOptions *opt) {
    if (opt->sort_field == DU_SORT_SIZE) {
        if (a->total_size < b->total_size)
            return opt->sort_desc ? 1 : -1;
        if (a->total_size > b->total_size)
            return opt->sort_desc ? -1 : 1;
        return 0;
    }
    if (opt->sort_field == DU_SORT_COUNT) {
        if (a->file_count < b->file_count)
            return opt->sort_desc ? 1 : -1;
        if (a->file_count > b->file_count)
            return opt->sort_desc ? -1 : 1;
        return 0;
    }
    return 0;
}

static int compare_exts(const ExtStatsArr *a,
                        const ExtStatsArr *b,
                        const DuOptions *opt) {
    if (opt->sort_field == DU_SORT_SIZE) {
        if (a->total_size < b->total_size)
            return opt->sort_desc ? 1 : -1;
        if (a->total_size > b->total_size)
            return opt->sort_desc ? -1 : 1;
        return 0;
    }
    if (opt->sort_field == DU_SORT_COUNT) {
        if (a->file_count < b->file_count)
            return opt->sort_desc ? 1 : -1;
        if (a->file_count > b->file_count)
            return opt->sort_desc ? -1 : 1;
        return 0;
    }
    return 0;
}

void du_sort_dirs(DirStatsArr *arr, const size_t count, const DuOptions *opt) {
    if (!arr || !opt || count < 2) return;
    /* insertion sort: stable, in-place, no extra allocation */
    for (size_t i = 1; i < count; ++i) {
        DirStatsArr key = arr[i];
        ssize_t j = (ssize_t)i - 1;
        while (j >= 0 && compare_dirs(&arr[j], &key, opt) > 0) {
            arr[j+1] = arr[j];
            --j;
        }
        arr[j+1] = key;
    }
}

void du_sort_exts(ExtStatsArr *arr, const size_t count, const DuOptions *opt) {
    if (!arr || !opt || count < 2) return;
    for (size_t i = 1; i < count; ++i) {
        ExtStatsArr key = arr[i];
        ssize_t j = (ssize_t)i - 1;
        while (j >= 0 && compare_exts(&arr[j], &key, opt) > 0) {
            arr[j+1] = arr[j];
            --j;
        }
        arr[j+1] = key;
    }
}
