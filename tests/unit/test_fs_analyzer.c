#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <math.h>

#include "unity.h"
#include "filter.h"
#include "fs_analyzer.h"
#include "sig_handler.h"
#include "stats_collector.h"

static char *test_dir = NULL;
static DuStats *stats = NULL;
static DuOptions opt;

static char *create_temp_dir(void) {
    char template[] = "/tmp/duana_testXXXXXX";
    const char *dir = mkdtemp(template);
    return strdup(dir);
}

static void cleanup_dir(const char *path) {
    char cmd[PATH_MAX + 16];
    snprintf(cmd, sizeof(cmd), "rm -rf '%s'", path);
    const int ret = system(cmd);
    (void)ret;
}

static void write_file(const char *dir, const char *name, const char *content) {
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    const int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    write(fd, content, strlen(content));
    close(fd);
}

void setUp(void) {
    du_setup_signals();

    test_dir = create_temp_dir();

    memset(&opt, 0, sizeof(opt));

    stats = du_stats_init();
}

void tearDown(void) {
    du_signal_cleanup();

    if (stats) {
        du_stats_destroy(stats);
        stats = NULL;
    }

    du_filter_clear(&opt);

    if (test_dir) {
        cleanup_dir(test_dir);
        free(test_dir);
        test_dir = NULL;
    }
}

void test_scan_without_filter(void) {
    write_file(test_dir, "a.txt", "hello");       // 5 byte
    write_file(test_dir, "b.txt", "world!!");     // 7 byte
    write_file(test_dir, "c.c",   "code");        // 4 byte

    const int ret = du_scan_directory(test_dir, &opt, stats);
    TEST_ASSERT_EQUAL_INT(0, ret);

    du_stats_finalize(stats);

    DirStatsArr* dirs = NULL;
    const size_t dir_count = du_stats_get_dirs(stats, &dirs);
    TEST_ASSERT_EQUAL_size_t(1, dir_count);
    TEST_ASSERT_EQUAL_UINT(3, dirs[0].file_count);
    free(dirs);
}

void test_total_file_size(void) {
    write_file(test_dir, "a.txt", "hello");       // 5 byte
    write_file(test_dir, "b.txt", "world!!");     // 7 byte
    write_file(test_dir, "c.c",   "code");        // 4 byte


    const int ret = du_scan_directory(test_dir, &opt, stats);
    TEST_ASSERT_EQUAL_INT(0, ret);
    du_stats_finalize(stats);

    DirStatsArr* dirs = NULL;
    du_stats_get_dirs(stats, &dirs);
    const off_t expected_total = 5 + 7 + 4;
    TEST_ASSERT_EQUAL_INT64(expected_total, dirs[0].total_size);
    free(dirs);
}

void test_extension_stats(void) {
    write_file(test_dir, "a.txt", "hello");       // 5 byte
    write_file(test_dir, "b.txt", "world!!");     // 7 byte
    write_file(test_dir, "c.c",   "code");        // 4 byte

    du_scan_directory(test_dir, &opt, stats);
    du_stats_finalize(stats);

    ExtStatsArr* exts = NULL;
    const size_t ext_count = du_stats_get_exts(stats, &exts);
    TEST_ASSERT_EQUAL_size_t(2, ext_count);

    for (size_t i = 0; i < ext_count; ++i) {
        if (strcmp(exts[i].ext, ".txt") == 0) {
            TEST_ASSERT_EQUAL_UINT(2, exts[i].file_count);
            TEST_ASSERT_EQUAL_INT64(12, exts[i].total_size);
        } else if (strcmp(exts[i].ext, ".c") == 0) {
            TEST_ASSERT_EQUAL_UINT(1, exts[i].file_count);
            TEST_ASSERT_EQUAL_INT64(4, exts[i].total_size);
        } else {
            TEST_FAIL_MESSAGE("Estensione inaspettata");
        }
    }
    free(exts);
}

void test_average_file_size(void) {
    write_file(test_dir, "a.txt", "hello");       // 5 byte
    write_file(test_dir, "b.txt", "world!!");     // 7 byte
    write_file(test_dir, "c.c",   "code");        // 4 byte

    du_scan_directory(test_dir, &opt, stats);
    du_stats_finalize(stats);

    const double avg = du_stats_get_avg_size(stats);
    const double expected_avg = (double)(5 + 7 + 4) / 3.0;

    TEST_ASSERT_EQUAL_INT((int)(expected_avg * 100), (int)(avg * 100));
}

void test_extension_filtering(void) {
    write_file(test_dir, "one.log", "1234");    // 4 byte
    write_file(test_dir, "two.log", "abcd");    // 4 byte
    write_file(test_dir, "skip.data", "xyz");   // 3 byte da filtrare

    du_filter_init(".log", &opt);

    du_scan_directory(test_dir, &opt, stats);
    du_stats_finalize(stats);

    DirStatsArr* dirs = NULL;
    const size_t dir_count = du_stats_get_dirs(stats, &dirs);
    TEST_ASSERT_EQUAL_size_t(1, dir_count);
    TEST_ASSERT_EQUAL_UINT(2, dirs[0].file_count);
    TEST_ASSERT_EQUAL_INT64(8, dirs[0].total_size);
    free(dirs);
}

void test_filtered_extension_stats(void) {
    write_file(test_dir, "one.log", "1234");    // 4 byte
    write_file(test_dir, "two.log", "abcd");    // 4 byte
    write_file(test_dir, "skip.data", "xyz");   // 3 byte da filtrare

    du_filter_init(".log", &opt);

    du_scan_directory(test_dir, &opt, stats);
    du_stats_finalize(stats);

    ExtStatsArr* exts = NULL;
    const size_t ext_count = du_stats_get_exts(stats, &exts);
    TEST_ASSERT_EQUAL_size_t(1, ext_count);
    TEST_ASSERT_EQUAL_STRING(".log", exts[0].ext);
    TEST_ASSERT_EQUAL_UINT(2, exts[0].file_count);
    TEST_ASSERT_EQUAL_INT64(8, exts[0].total_size);
    free(exts);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_scan_without_filter);
    RUN_TEST(test_total_file_size);
    RUN_TEST(test_extension_stats);
    RUN_TEST(test_average_file_size);
    RUN_TEST(test_extension_filtering);
    RUN_TEST(test_filtered_extension_stats);

    return UNITY_END();
}