#include "stats_collector.h"
#include <stdlib.h>
#include <string.h>

extern void *du_xmalloc(size_t size);

#define INITIAL_CAP 16
#define LOAD_FACTOR 0.75

// Internal entry for hash tables
typedef struct {
    char *key;
    size_t file_count;
    off_t total_size;
} Entry;

struct DuStats {
    Entry *dir_table;
    size_t dir_capacity;
    size_t dir_count;
    Entry *ext_table;
    size_t ext_capacity;
    size_t ext_count;
    off_t total_size;
    size_t file_count;
    double avg_size;
    int finalized;
};

// djb2 string hash
static unsigned long hash_str(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// Resize a hash table when a load factor exceeded
static void resize_table(Entry **table, size_t *capacity) {
    const size_t old_cap = *capacity;
    const size_t new_cap = old_cap * 2;
    Entry *old_table = *table;
    Entry *new_table = du_xmalloc(new_cap * sizeof(Entry));
    for (size_t i = 0; i < new_cap; i++) new_table[i].key = NULL;
    // Rehash entries
    for (size_t i = 0; i < old_cap; i++) {
        const Entry *e = &old_table[i];
        if (e->key) {
            const unsigned long h = hash_str(e->key);
            size_t idx = h % new_cap;
            while (new_table[idx].key) idx = (idx + 1) % new_cap;
            new_table[idx] = *e;
        }
    }
    free(old_table);
    *table = new_table;
    *capacity = new_cap;
}

// Update or insert an entry in a hash table
static void update_entry(Entry **table, size_t *capacity, size_t *count,
                         const char *key,
                         const off_t size) {
    if ((*count + 1) > (size_t)(*capacity * LOAD_FACTOR)) {
        resize_table(table, capacity);
    }
    const unsigned long h = hash_str(key);
    size_t idx = h % *capacity;
    // Search for existing
    while ((*table)[idx].key) {
        if (strcmp((*table)[idx].key, key) == 0) {
            (*table)[idx].file_count++;
            (*table)[idx].total_size += size;
            return;
        }
        idx = (idx + 1) % *capacity;
    }
    // Insert new
    char *dup = du_xmalloc(strlen(key) + 1);
    strcpy(dup, key);
    (*table)[idx].key = dup;
    (*table)[idx].file_count = 1;
    (*table)[idx].total_size = size;
    (*count)++;
}

DuStats *du_stats_init(void) {
    DuStats *s = du_xmalloc(sizeof(DuStats));
    s->dir_capacity = INITIAL_CAP;
    s->dir_count = 0;
    s->dir_table = du_xmalloc(s->dir_capacity * sizeof(Entry));
    for (size_t i = 0; i < s->dir_capacity; i++) s->dir_table[i].key = NULL;
    s->ext_capacity = INITIAL_CAP;
    s->ext_count = 0;
    s->ext_table = du_xmalloc(s->ext_capacity * sizeof(Entry));
    for (size_t i = 0; i < s->ext_capacity; i++) s->ext_table[i].key = NULL;
    s->total_size = 0;
    s->file_count = 0;
    s->avg_size = 0.0;
    s->finalized = 0;
    return s;
}

void du_stats_update_file(DuStats *s, const char *dirpath, const char *ext, const off_t size) {
    if (!s || s->finalized || !dirpath || !ext) return;
    update_entry(&s->dir_table, &s->dir_capacity, &s->dir_count, dirpath, size);
    update_entry(&s->ext_table, &s->ext_capacity, &s->ext_count, ext, size);
    s->total_size += size;
    s->file_count++;
}

void du_stats_finalize(DuStats *s) {
    if (!s || s->finalized) return;
    if (s->file_count > 0)
        s->avg_size = (double)s->total_size / s->file_count;
    else
        s->avg_size = 0.0;
    s->finalized = 1;
}

size_t du_stats_get_dirs(const DuStats *s, DirStatsArr **out) {
    if (!s || !out)
        return 0;
    const size_t n = s->dir_count;
    *out = malloc(n * sizeof(DirStatsArr));
    size_t idx = 0;
    for (size_t i = 0; i < s->dir_capacity; i++) {
        if (s->dir_table[i].key) {
            (*out)[idx].path = s->dir_table[i].key;
            (*out)[idx].file_count = s->dir_table[i].file_count;
            (*out)[idx].total_size = s->dir_table[i].total_size;
            idx++;
        }
    }
    return n;
}

size_t du_stats_get_exts(const DuStats *s, ExtStatsArr **out) {
    if (!s || !out)
        return 0;
    const size_t n = s->ext_count;
    *out = du_xmalloc(n * sizeof(ExtStatsArr));
    size_t idx = 0;
    for (size_t i = 0; i < s->ext_capacity; i++) {
        if (s->ext_table[i].key) {
            (*out)[idx].ext = s->ext_table[i].key;
            (*out)[idx].file_count = s->ext_table[i].file_count;
            (*out)[idx].total_size = s->ext_table[i].total_size;
            idx++;
        }
    }
    return n;
}

double du_stats_get_avg_size(const DuStats *s) {
    return s ? s->avg_size : 0.0;
}

void du_stats_destroy(DuStats *s) {
    if (!s) return;
    for (size_t i = 0; i < s->dir_capacity; i++) {
        if (s->dir_table[i].key) free(s->dir_table[i].key);
    }
    free(s->dir_table);
    for (size_t i = 0; i < s->ext_capacity; i++) {
        if (s->ext_table[i].key) free(s->ext_table[i].key);
    }
    free(s->ext_table);
    free(s);
}