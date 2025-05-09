#include "cli_parser.h"
#include "fs_analyzer.h"
#include "sig_handler.h"
#include "stats_collector.h"
#include <stdio.h>
#include <stdlib.h>
#include "sorter.h"
#include "json_export.h"
#include "options.h"
#include "report_writer.h"
#include "utils.h"

int main(const int argc, char** argv) {
    DuOptions options;
    du_options_init(&options);
    int exit_code = EXIT_SUCCESS;

    /* 1. Parse CLI arguments */
    if (du_parse_args(argc, argv, &options) != 0) {
        du_perror("du_parse_args");
        du_options_free(&options);
        return EXIT_FAILURE;
    }

    /* 2. Setup signal handling for SIGINT */
    if (du_setup_signals() != 0) {
        du_perror("du_setup_signals");
        du_options_free(&options);
        return EXIT_FAILURE;
    }

    /* 3. Initialize statistics */
    DuStats* stats = du_stats_init();
    if (stats == NULL) {
        du_perror("du_stats_init");
        du_signal_cleanup();
        du_options_free(&options);
        return EXIT_FAILURE;
    }

    /* 4. Scan filesystem (interruptible via SIGINT) */
    if (du_scan_directory(options.path, &options, stats) != 0) {
        if (du_is_cancelled()) {
            fprintf(stderr, "Scan cancelled by user\n");
        } else {
            du_perror("du_scan_directory");
        }
        du_stats_destroy(stats);
        du_signal_cleanup();
        du_options_free(&options);
        return EXIT_FAILURE;
    }

    /* 5. Finalize stats (compute averages) */
    du_stats_finalize(stats);

    /* 6. Extract and sort directory and extension stats */
    DirStatsArr* dirs = NULL;
    const size_t ndirs = du_stats_get_dirs(stats, &dirs);
    ExtStatsArr* exts = NULL;
    const size_t nexts = du_stats_get_exts(stats, &exts);
    du_sort_dirs(dirs, ndirs, &options);
    du_sort_exts(exts, nexts, &options);

    /* 7. Output: JSON or formatted report */
    if (options.json) {
        if (options.json_file) {
            if (du_json_export_file(options.json_file,
                dirs, ndirs,
                exts, nexts,
                du_stats_get_avg_size(stats), options.json_formatted
                ) != 0) {
                du_perror("du_json_export_file");
                exit_code = EXIT_FAILURE;
            }
        } else {
            if (du_json_export_stdout(
                dirs, ndirs,
                exts, nexts,
                du_stats_get_avg_size(stats), options.json_formatted
                ) != 0) {
                du_perror("du_json_export_stdout");
                exit_code = EXIT_FAILURE;
            }
        }
    } else {
        if (du_report_print(
            dirs, ndirs,
            exts, nexts,
            du_stats_get_avg_size(stats)) != 0) {
            du_perror("du_report_print");
            exit_code = EXIT_FAILURE;
        }
    }

    /* 8. Cleanup resources */
    free(dirs);
    free(exts);
    du_stats_destroy(stats);
    du_options_free(&options);
    du_signal_cleanup();

    return exit_code;
}
