#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "stats_collector.h"
#include "unity.h"

static DuStats *s = NULL;
static DirStatsArr *dirs = NULL;
static ExtStatsArr *exts = NULL;
static size_t dir_count = 0;
static size_t ext_count = 0;

static void free_dirs_array(void) {
    if (dirs) {
        free(dirs);
        dirs = NULL;
    }
}

static void free_exts_array(void) {
    if (exts) {
        free(exts);
        exts = NULL;
    }
}

void setUp(void) {
    s = du_stats_init();
    dirs = NULL;
    exts = NULL;
    dir_count = 0;
    ext_count = 0;
}

void tearDown(void) {
    free_dirs_array();
    free_exts_array();
    if (s) {
        du_stats_destroy(s);
        s = NULL;
    }
}

void test_initialization_returns_valid_instance(void) {
    TEST_ASSERT_NOT_NULL(s);
}

void test_empty_stats_has_no_dirs_or_exts(void) {
    dir_count = du_stats_get_dirs(s, &dirs);
    ext_count = du_stats_get_exts(s, &exts);

    TEST_ASSERT_EQUAL_INT(0, dir_count);
    TEST_ASSERT_EQUAL_INT(0, ext_count);
}

void test_adding_single_file_updates_dir_stats(void) {
    du_stats_update_file(s, "/tmp", ".txt", 100);
    du_stats_finalize(s);

    dir_count = du_stats_get_dirs(s, &dirs);
    TEST_ASSERT_EQUAL_INT(1, dir_count);
    TEST_ASSERT_EQUAL_STRING("/tmp", dirs[0].path);
    TEST_ASSERT_EQUAL_INT(1, dirs[0].file_count);
    TEST_ASSERT_EQUAL_INT(100, dirs[0].total_size);
}

void test_adding_single_file_updates_ext_stats(void) {
    du_stats_update_file(s, "/tmp", ".txt", 100);
    du_stats_finalize(s);

    ext_count = du_stats_get_exts(s, &exts);
    TEST_ASSERT_EQUAL_INT(1, ext_count);
    TEST_ASSERT_EQUAL_STRING(".txt", exts[0].ext);
    TEST_ASSERT_EQUAL_INT(1, exts[0].file_count);
    TEST_ASSERT_EQUAL_INT(100, exts[0].total_size);
}

void test_single_file_average_size(void) {
    du_stats_update_file(s, "/tmp", ".txt", 100);
    du_stats_finalize(s);

    const double avg = du_stats_get_avg_size(s);
    TEST_ASSERT_EQUAL_FLOAT(100.0, avg);
}

void test_multiple_dirs_stats(void) {
    du_stats_update_file(s, "/a", ".c", 50);
    du_stats_update_file(s, "/b", ".h", 70);
    du_stats_update_file(s, "/a", ".c", 30);
    du_stats_update_file(s, "/b", ".c", 20);
    du_stats_finalize(s);

    dir_count = du_stats_get_dirs(s, &dirs);
    TEST_ASSERT_EQUAL_INT(2, dir_count);

    int found_a = 0, found_b = 0;
    for (size_t i = 0; i < dir_count; i++) {
        if (strcmp(dirs[i].path, "/a") == 0) {
            TEST_ASSERT_EQUAL_INT(80, dirs[i].total_size);
            TEST_ASSERT_EQUAL_INT(2, dirs[i].file_count);
            found_a = 1;
        }
        if (strcmp(dirs[i].path, "/b") == 0) {
            TEST_ASSERT_EQUAL_INT(90, dirs[i].total_size);
            TEST_ASSERT_EQUAL_INT(2, dirs[i].file_count);
            found_b = 1;
        }
    }
    TEST_ASSERT_TRUE(found_a && found_b);
}

void test_multiple_exts_stats(void) {
    du_stats_update_file(s, "/a", ".c", 50);
    du_stats_update_file(s, "/b", ".h", 70);
    du_stats_update_file(s, "/a", ".c", 30);
    du_stats_update_file(s, "/b", ".c", 20);
    du_stats_finalize(s);

    ext_count = du_stats_get_exts(s, &exts);
    TEST_ASSERT_EQUAL_INT(2, ext_count);

    int found_c = 0, found_h = 0;
    for (size_t i = 0; i < ext_count; i++) {
        if (strcmp(exts[i].ext, ".c") == 0) {
            TEST_ASSERT_EQUAL_INT(100, exts[i].total_size);
            TEST_ASSERT_EQUAL_INT(3, exts[i].file_count);
            found_c = 1;
        }
        if (strcmp(exts[i].ext, ".h") == 0) {
            TEST_ASSERT_EQUAL_INT(70, exts[i].total_size);
            TEST_ASSERT_EQUAL_INT(1, exts[i].file_count);
            found_h = 1;
        }
    }
    TEST_ASSERT_TRUE(found_c && found_h);
}

void test_multiple_files_average_size(void) {
    du_stats_update_file(s, "/a", ".c", 50);
    du_stats_update_file(s, "/b", ".h", 70);
    du_stats_update_file(s, "/a", ".c", 30);
    du_stats_update_file(s, "/b", ".c", 20);
    du_stats_finalize(s);

    const double avg = du_stats_get_avg_size(s);
    TEST_ASSERT_EQUAL_FLOAT(170.0/4.0, avg);
}

void test_division_by_zero(void) {
    du_stats_finalize(s);
    const double avg = du_stats_get_avg_size(s);
    TEST_ASSERT_EQUAL_FLOAT(0.0, avg);
}

void test_finalize_makes_stats_immutable(void) {
    du_stats_update_file(s, "/x", ".log", 10);
    du_stats_finalize(s);
    du_stats_update_file(s, "/x", ".log", 1000);

    dir_count = du_stats_get_dirs(s, &dirs);
    TEST_ASSERT_EQUAL_INT(1, dir_count);
    TEST_ASSERT_EQUAL_INT(10, dirs[0].total_size);
}

void test_null_dir_parameter_is_ignored(void) {
    du_stats_update_file(s, NULL, ".txt", 5);
    du_stats_finalize(s);

    dir_count = du_stats_get_dirs(s, &dirs);
    TEST_ASSERT_EQUAL_INT(0, dir_count);
}

void test_null_ext_parameter_is_ignored(void) {
    du_stats_update_file(s, "/tmp", NULL, 5);
    du_stats_finalize(s);

    ext_count = du_stats_get_exts(s, &exts);
    TEST_ASSERT_EQUAL_INT(0, ext_count);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_initialization_returns_valid_instance);
    RUN_TEST(test_empty_stats_has_no_dirs_or_exts);
    RUN_TEST(test_adding_single_file_updates_dir_stats);
    RUN_TEST(test_adding_single_file_updates_ext_stats);
    RUN_TEST(test_single_file_average_size);
    RUN_TEST(test_multiple_dirs_stats);
    RUN_TEST(test_multiple_exts_stats);
    RUN_TEST(test_multiple_files_average_size);
    RUN_TEST(test_division_by_zero);
    RUN_TEST(test_finalize_makes_stats_immutable);
    RUN_TEST(test_null_dir_parameter_is_ignored);
    RUN_TEST(test_null_ext_parameter_is_ignored);
    return UNITY_END();
}