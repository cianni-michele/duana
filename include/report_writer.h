#ifndef REPORT_WRITER_H
#define REPORT_WRITER_H

#include "options.h"
#include "stats_collector.h"

/**
 * Print the formatted report to stdout.
 * @param dirs    Array of directory statistics, terminated by an entry with path=NULL.
 * @param exts    Array of extension statistics, terminated by an entry with ext=NULL.
 * @param avg     Average file size in bytes.
 * @return 0 on success, -1 on failure (with errno set).
 */
int du_report_print(const DirStatsArr *dirs, const ExtStatsArr *exts, double avg);

#endif //REPORT_WRITER_H
