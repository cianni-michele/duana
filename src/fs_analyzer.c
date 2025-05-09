#include "fs_analyzer.h"
#include "filter.h"
#include "stats_collector.h"
#include "sig_handler.h"
#include "utils.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
    char *path;
} DirNode;

typedef struct {
    DirNode *nodes;
    size_t count;
    size_t capacity;
} DirStack;

static int stack_init(DirStack *stack) {
    stack->nodes = malloc(128 * sizeof(DirNode));
    if (!stack->nodes)
        return -1;
    stack->count = 0;
    stack->capacity = 128;
    return 0;
}

static void stack_free(DirStack *stack) {
    for (size_t i = 0; i < stack->count; i++) {
        free(stack->nodes[i].path);
    }
    free(stack->nodes);
    stack->nodes = NULL;
    stack->count = stack->capacity = 0;
}

static int stack_push(DirStack *stack, const char *path) {
    if (stack->count == stack->capacity) {
        const size_t newcap = stack->capacity * 2;
        DirNode *tmp = realloc(stack->nodes, newcap * sizeof(DirNode));
        if (!tmp)
            return -1;
        stack->nodes = tmp;
        stack->capacity = newcap;
    }
    stack->nodes[stack->count].path = strdup(path);
    if (!stack->nodes[stack->count].path)
        return -1;
    stack->count++;
    return 0;
}

static int stack_pop(DirStack *stack, char **path) {
    if (stack->count == 0)
        return 0;
    stack->count--;
    *path = stack->nodes[stack->count].path;
    return 1;
}

int du_scan_directory(const char *root, const DuOptions *opt, DuStats *stats) {
    if (!root || !opt || !stats) {
        errno = EINVAL;
        return -1;
    }

    struct stat st;
    if (lstat(root, &st) < 0 || !S_ISDIR(st.st_mode)) {
        du_perror(root);
        errno = (!S_ISDIR(st.st_mode)) ? ENOTDIR : errno;
        return -1;
    }

    DirStack stack;
    if (stack_init(&stack) < 0)
        return -1;

    if (stack_push(&stack, root) < 0) {
        stack_free(&stack);
        return -1;
    }

    while (stack.count > 0 && !du_is_cancelled()) {
        char *current;
        if (!stack_pop(&stack, &current))
            break;

        DIR *d = opendir(current);
        if (!d) {
            du_perror(current);
            free(current);
            continue;
        }
        struct dirent *entry;
        while (!du_is_cancelled() && (entry = readdir(d)) != NULL) {
            if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
                continue;
            size_t len = strlen(current) + strlen(entry->d_name) + 2;
            char *full_path = du_xmalloc(len);
            snprintf(full_path, len, "%s/%s", current, entry->d_name);
            struct stat entry_st;
            if (lstat(full_path, &entry_st) < 0) {
                du_perror(full_path);
                free(full_path);
                continue;
            }
            if (S_ISDIR(entry_st.st_mode)) {
                if (stack_push(&stack, full_path) < 0) {
                    free(full_path);
                    closedir(d);
                    free(current);
                    stack_free(&stack);
                    return -1;
                }
            } else if (S_ISREG(entry_st.st_mode)) {
                char *ext = du_get_extension(entry->d_name);
                if (du_filter_match(opt, ext ? ext : ""))
                    du_stats_update_file(stats, current, ext, entry_st.st_size);
                free(ext);
            }
            free(full_path);
        }
        closedir(d);
        free(current);
    }

    stack_free(&stack);
    return 0;
}