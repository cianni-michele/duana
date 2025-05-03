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
    dev_t dev;
    ino_t ino;
} DevIno;

static DevIno *visited     = NULL;
static size_t visited_cnt  = 0;
static size_t visited_cap  = 0;


void du_scan_abort(void) {
    du_set_cancelled();
}

/* Verifica se (dev,ino) è già stato visitato */
static int is_visited(const dev_t dev, const ino_t ino) {
    for (size_t i = 0; i < visited_cnt; ++i) {
        if (visited[i].dev == dev && visited[i].ino == ino)
            return 1;
    }
    return 0;
}

/* Aggiunge una nuova coppia (dev,ino) all’array di visitati */
static int mark_visited(const dev_t dev, const ino_t ino) {
    if (is_visited(dev, ino))
        return 0;
    if (visited_cnt == visited_cap) {
        const size_t newcap = (visited_cap == 0 ? 128 : visited_cap * 2);
        DevIno *tmp = realloc(visited, newcap * sizeof(*tmp));
        if (!tmp) {
            du_perror("realloc");  /* errore fatale */
            return -1;
        }
        visited     = tmp;
        visited_cap = newcap;
    }
    visited[visited_cnt].dev = dev;
    visited[visited_cnt].ino = ino;
    visited_cnt++;
    return 0;
}

/* Funzione ricorsiva interna */
static int scan_dir(const char *path, const DuOptions *opt, DuStats *stats) {
    DIR *d = opendir(path);
    if (!d) {
        if (errno == EACCES)
            du_perror(path);
        else
            du_perror(path);
        return 0;  /* skip senza interrompere */
    }

    struct dirent *entry;
    while (!du_is_cancelled() && (entry = readdir(d)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        /* Componi percorso completo */
        const size_t len = strlen(path) + strlen(entry->d_name) + 2;
        char *full = du_xmalloc(len);
        snprintf(full, len, "%s/%s", path, entry->d_name);

        struct stat st;
        if (lstat(full, &st) < 0) {
            if (errno == EACCES)
                du_perror(full);
            else
                du_perror(full);
            free(full);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            /* Prevenzione loop simbolici */
            if (mark_visited(st.st_dev, st.st_ino) < 0) {
                free(full);
                closedir(d);
                return -1;
            }
            scan_dir(full, opt, stats);
        }
        else if (S_ISREG(st.st_mode)) {
            /* File regolare: applica filtro estensioni */
            char* ext = du_get_extension(entry->d_name);
            const int ok = du_filter_match(opt, ext ? ext : "");
            if (ok) {
                du_stats_update_file(stats, path, ext, st.st_size);
            }
            free(ext);
        }
        /* Altri tipi (link, socket, device, FIFO) vengono ignorati */

        free(full);
    }

    closedir(d);
    return 0;
}

int du_scan_directory(const char *root, const DuOptions *opt, DuStats *stats) {
    if (!root || !opt || !stats) {
        errno = EINVAL;
        return -1;
    }

    struct stat st;
    if (lstat(root, &st) < 0) {
        du_perror(root);
        return -1;
    }
    if (!S_ISDIR(st.st_mode)) {
        du_perror(root);
        errno = ENOTDIR;
        return -1;
    }

    visited     = NULL;
    visited_cnt = 0;
    visited_cap = 0;
    if (mark_visited(st.st_dev, st.st_ino) < 0)
        return -1;

    const int res = scan_dir(root, opt, stats);

    free(visited);
    visited     = NULL;
    visited_cnt = visited_cap = 0;

    return res;
}

