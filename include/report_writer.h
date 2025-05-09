#ifndef REPORT_WRITER_H
#define REPORT_WRITER_H

#include "options.h"
#include "stats_collector.h"

/**
 * Print the formatted report to stdout.
 * @param dirs      Array of directory statistics.
 * @param dir_count Number of elements in the dirs array.
 * @param exts      Array of extension statistics.
 * @param ext_count Number of elements in the exts array.
 * @param avg       Average file size in bytes.
 * @return 0 on success, -1 on failure (with errno set).
 */
int du_report_print(const DirStatsArr *dirs, size_t dir_count, 
                    const ExtStatsArr *exts, size_t ext_count, double avg);

#endif //REPORT_WRITER_H
