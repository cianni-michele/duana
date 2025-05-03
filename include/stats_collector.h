#ifndef STATS_COLLECTOR_H
#define STATS_COLLECTOR_H

#include <stddef.h>
#include <sys/types.h>

/**
 * Directory statistics struct for output.
 * 'Path' points to an internal string; user should only free the array.
 */
typedef struct {
    char *path;
    size_t file_count;
    off_t total_size;
} DirStatsArr;

/**
 * Extension statistics struct for output.
 */
typedef struct {
    char *ext;
    size_t file_count;
    off_t total_size;
} ExtStatsArr;

/**
 * Opaque type for collecting disk usage statistics.
 */
typedef struct DuStats DuStats;

/**
 * Initialize a DuStats instance.
 * Allocates internal tables.
 */
DuStats *du_stats_init(void);

/**
 * Update statistics with a single file.
 * Ignored if s is NULL, already finalized, or dirpath/ext is NULL.
 */
void du_stats_update_file(DuStats *s, const char *dirpath, const char *ext, off_t size);

/**
 * Finalize statistics: compute average and prevent further updates.
 */
void du_stats_finalize(DuStats *s);

/**
 * Export directory stats into a newly malloc'd array. Returns count.
 * Caller must free(*out).
 */
size_t du_stats_get_dirs(const DuStats *s, DirStatsArr **out);

/**
 * Export extension stats into a newly malloc'd array. Returns count.
 * Caller must free(*out).
 */
size_t du_stats_get_exts(const DuStats *s, ExtStatsArr **out);

/**
 * Retrieve average file size (0.0 if none).
 */
double du_stats_get_avg_size(const DuStats *s);

/**
 * Destroy DuStats and free all internal memory.
 */
void du_stats_destroy(DuStats *s);

#endif //STATS_COLLECTOR_H
