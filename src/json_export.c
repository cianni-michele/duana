#include "json_export.h"
#include "cJSON.h"
#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static size_t count_dir_entries(const DirStatsArr *d) {
    if (!d) return 0;
    size_t count = 0;
    while (d[count].path != NULL) {
        count++;
    }
    return count;
}

static size_t count_ext_entries(const ExtStatsArr *e) {
    if (!e) return 0;
    size_t count = 0;
    while (e[count].ext != NULL) {
        count++;
    }
    return count;
}

/* Costruisce l'oggetto JSON in memoria; restituisce NULL se fallisce */
static cJSON *build_json(const DirStatsArr *d, const size_t dcount,
                         const ExtStatsArr *e,
                         const size_t ecount, const double avg)
{
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    /* directories */
    cJSON *jdirs = cJSON_AddArrayToObject(root, "directories");
    if (!jdirs) { cJSON_Delete(root); return NULL; }
    for (size_t i = 0; d && i < dcount; i++) {
        cJSON *o = cJSON_CreateObject();
        if (!o) { cJSON_Delete(root); return NULL; }
        cJSON_AddStringToObject(o, "path", d[i].path);
        cJSON_AddNumberToObject(o, "fileCount", (double)d[i].file_count);
        cJSON_AddNumberToObject(o, "totalSize", (double)d[i].total_size);
        cJSON_AddItemToArray(jdirs, o);
    }

    /* extensions */
    cJSON *jexts = cJSON_AddArrayToObject(root, "extensions");
    if (!jexts) { cJSON_Delete(root); return NULL; }
    for (size_t i = 0; e && i < ecount; i++) {
        cJSON *o = cJSON_CreateObject();
        if (!o) { cJSON_Delete(root); return NULL; }
        cJSON_AddStringToObject(o, "ext", e[i].ext);
        cJSON_AddNumberToObject(o, "fileCount", (double)e[i].file_count);
        cJSON_AddNumberToObject(o, "totalSize", (double)e[i].total_size);
        cJSON_AddItemToArray(jexts, o);
    }

    /* avgFileSize */
    cJSON_AddNumberToObject(root, "avgFileSize", avg);

    return root;
}

int du_json_export_file(const char *path,
                        const DirStatsArr *d,
                        const ExtStatsArr *e,
                        const double avg)
{
    /* Calcolo conteggi (idem stdout) */
    size_t dcount = count_dir_entries(d);
    size_t ecount = count_ext_entries(e);

    cJSON *root = build_json(d, dcount, e, ecount, avg);
    if (!root) {
        du_perror("json_export: cJSON build");
        return -1;
    }
    char *s = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    if (!s) {
        du_perror("json_export: cJSON print");
        return -1;
    }

    /* File temporaneo */
    size_t pathlen = strlen(path);
    char *tmp = malloc(pathlen + 5);
    if (!tmp) { free(s); return -1; }
    sprintf(tmp, "%s.tmp", path);

    int fd = open(tmp, O_WRONLY|O_CREAT|O_TRUNC, 0644);
    if (fd < 0) {
        du_perror("json_export: open tmp");
        free(tmp); free(s);
        return -1;
    }

    /* Scrittura intera stringa */
    size_t towrite = strlen(s);
    ssize_t written = write(fd, s, towrite);
    if (written < 0 || (size_t)written != towrite) {
        du_perror("json_export: write tmp");
        close(fd);
        unlink(tmp);
        free(tmp); free(s);
        return -1;
    }
    /* Persistenza */
    if (fsync(fd) < 0) {
        du_perror("json_export: fsync tmp");
        close(fd);
        unlink(tmp);
        free(tmp); free(s);
        return -1;
    }
    close(fd);

    /* Atomico: rename */
    if (rename(tmp, path) < 0) {
        du_perror("json_export: rename");
        unlink(tmp);
        free(tmp); free(s);
        return -1;
    }
    free(tmp); free(s);
    return 0;
}

