#include "options.h"

#include <stdlib.h>

#include "filter.h"

void du_options_init(DuOptions *opt) {
    if (!opt) return;

    opt->path = NULL;
    opt->sort_field = DU_SORT_SIZE;
    opt->sort_desc = false;
    opt->json = false;
    opt->json_file = NULL;
    opt->json_formatted = false;
    opt->filter_exts = NULL;
    opt->filter_count = 0;
}

void du_options_free(DuOptions *opt) {
    if (!opt) return;
    if (opt->path) {
        free(opt->path);
        opt->path = NULL;
    }
    if (opt->json_file) {
        free(opt->json_file);
        opt->json_file = NULL;
    }
    if (opt->filter_count > 0 && opt->filter_exts) {
        du_filter_clear(opt);
    }
}