#include "cli_parser.h"
#include "filter.h"
#include "options.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

static const char* OPT_PREFIX = "--";
static const int OPT_PREFIX_LEN = 2;

static const char* OPT_SORT = "--sort=";
static const int OPT_SORT_LEN = 7;

static const char* OPT_DESC = "--desc";
static const char* OPT_FILTER = "--filter=";
static const int OPT_FILTER_LEN = 9;

static const char* OPT_JSON = "--json";
static const int OPT_JSON_LEN = 6;
static const char* OPT_JSON_FILE = "--json=";
static const int OPT_JSON_FILE_LEN = 7;
static const char* OPT_JSON_FORMATTED = "--formatted";
static const int OPT_JSON_FORMATTED_LEN = 12;

static void print_usage(void) {
    fprintf(stderr, "Usage: duana <path> [--sort={size,count}] [--desc] [--filter=ext1,ext2] "
                    "[--json[=file] [--formatted]]\n");
}

static int report_invalid_argument(void) {
    errno = EINVAL;
    print_usage();
    return -1;
}

static int parse_sort_option(const char* val, DuOptions* out) {
    if (strcmp(val, "size") == 0) {
        out->sort_field = DU_SORT_SIZE;
    } else if (strcmp(val, "count") == 0) {
        out->sort_field = DU_SORT_COUNT;
    } else {
        return -1;
    }
    return 0;
}

static int parse_filter_option(const char* csv, DuOptions* out) {
    if (csv[0] == '\0') {
        return -1;
    }

    return du_filter_init(csv, out);
}

static int parse_json_option(const char* arg, DuOptions* out) {
    if (strcmp(arg, OPT_JSON) == 0) {
        out->json = true;
        return 0;
    }
    if (strncmp(arg, OPT_JSON_FILE, OPT_JSON_FILE_LEN) == 0) {
        const char* file = arg + OPT_JSON_FILE_LEN;
        if (file[0] == '\0') {
            return -1;
        }

        out->json = true;
        out->json_file = strdup(file);
        if (!out->json_file) {
            du_perror("parse_json_option");
            return -1;
        }
        return 0;
    }
    if (strncmp(arg, OPT_JSON_FORMATTED, OPT_JSON_FORMATTED_LEN) == 0) {
        if (!out->json) {
            return -1;
        }
        out->json_formatted = true;
        return 0;
    }

    return -1;
}

static int process_option_argument(const char* arg, DuOptions* out) {
    if (strncmp(arg, OPT_SORT, OPT_SORT_LEN) == 0) {
        if (parse_sort_option(arg + OPT_SORT_LEN, out) != 0) {
            return -1;
        }
    } else if (strcmp(arg, OPT_DESC) == 0) {
        out->sort_desc = true;
    } else if (strncmp(arg, OPT_FILTER, OPT_FILTER_LEN) == 0) {
        if (parse_filter_option(arg + OPT_FILTER_LEN, out) != 0) {
            return -1;
        }
    } else if (strncmp(arg, OPT_JSON, OPT_JSON_LEN) == 0 ||
               strncmp(arg, OPT_JSON_FILE, OPT_JSON_FILE_LEN) == 0 ||
               strncmp(arg, OPT_JSON_FORMATTED, OPT_JSON_FORMATTED_LEN) == 0) {
        if (parse_json_option(arg, out) != 0) {
            return -1;
        }
    } else {
        return -1;
    }

    return 0;
}

static int parse_path_argument(const char* arg, DuOptions* out) {
    if (out->path) {
        return -1;
    }

    out->path = strdup(arg);
    if (!out->path) {
        du_perror("parse_path_argument");
        return -1;
    }

    return 0;
}

int du_parse_args(const int argc, char** argv, DuOptions* out) {
    if (argc < 2 || !out) {
        return report_invalid_argument();
    }

    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (strncmp(arg, OPT_PREFIX, OPT_PREFIX_LEN) == 0) {
            if (process_option_argument(arg, out) != 0) {
                return report_invalid_argument();
            }
        } else {
            if (parse_path_argument(arg, out) != 0) {
                return report_invalid_argument();
            }
        }
    }

    if (!out->path) {
        return report_invalid_argument();
    }

    return 0;
}
