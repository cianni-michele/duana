#include "options.h"
#include "report_writer.h"
#include "stats_collector.h"
#include "unity.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *captured_output = NULL;

// Helper per catturare l'output di stdout
static char *capture_stdout(int (*test_fn)(void)) {
    fflush(stdout);
    const int fd_old = dup(fileno(stdout));
    if (fd_old == -1) {
        return NULL;
    }

    FILE *tmp = tmpfile();
    if (!tmp) {
        close(fd_old);
        return NULL;
    }

    dup2(fileno(tmp), fileno(stdout));

    const int ret = test_fn();
    fflush(stdout);

    fseek(tmp, 0, SEEK_END);
    const long len = ftell(tmp);
    fseek(tmp, 0, SEEK_SET);
    char *buf = du_xmalloc((size_t)len + 1);
    if (buf) {
        fread(buf, 1, (size_t)len, tmp);
        buf[len] = '\0';
    }

    dup2(fd_old, fileno(stdout));
    close(fd_old);
    fclose(tmp);

    if (ret != 0 || !buf) {
        free(buf);
        return NULL;
    }

    return buf;
}

// Dati di test condivisi
static const DirStatsArr test_dirs[] = {{"dir1", 2, 2048}, {NULL, 0, 0}};
static const ExtStatsArr test_exts[] = {{".txt", 2, 2048}, {NULL, 0, 0}};
static const DirStatsArr empty_dirs[] = {{NULL, 0, 0}};
static const ExtStatsArr empty_exts[] = {{NULL, 0, 0}};

// Funzioni helper per i test
static int call_print_single(void) {
    return du_report_print(test_dirs, test_exts, 1024.0);
}

static int call_print_empty(void) {
    return du_report_print(empty_dirs, empty_exts, 0.0);
}

void setUp(void) {
    captured_output = NULL;
}

void tearDown(void) {
    if (captured_output) {
        free(captured_output);
        captured_output = NULL;
    }
}

void test_report_print_returns_zero_on_success(void) {
    const int res = du_report_print(test_dirs, test_exts, 1024.0);
    TEST_ASSERT_EQUAL_INT(0, res);
}

void test_report_contains_directory_name(void) {
    captured_output = capture_stdout(call_print_single);
    TEST_ASSERT_NOT_NULL(captured_output);
    TEST_ASSERT_NOT_NULL(strstr(captured_output, "dir1"));
}

void test_report_contains_file_count(void) {
    captured_output = capture_stdout(call_print_single);
    TEST_ASSERT_NOT_NULL(captured_output);
    TEST_ASSERT_NOT_NULL(strstr(captured_output, "2"));
}

void test_report_contains_directory_size(void) {
    captured_output = capture_stdout(call_print_single);
    TEST_ASSERT_NOT_NULL(captured_output);
    TEST_ASSERT_NOT_NULL(strstr(captured_output, "2.00 KiB"));
}

void test_report_contains_extension(void) {
    captured_output = capture_stdout(call_print_single);
    TEST_ASSERT_NOT_NULL(captured_output);
    TEST_ASSERT_NOT_NULL(strstr(captured_output, ".txt"));
}

void test_report_contains_media_size(void) {
    captured_output = capture_stdout(call_print_single);
    TEST_ASSERT_NOT_NULL(captured_output);
    TEST_ASSERT_NOT_NULL(strstr(captured_output, "Media: 1.00 KiB"));
}

void test_report_returns_error_with_null_arrays(void) {
    const int res = du_report_print(NULL, NULL, 0.0);
    TEST_ASSERT_EQUAL_INT(-1, res);
}

void test_report_handles_empty_arrays(void) {
    const int res = call_print_empty();
    TEST_ASSERT_EQUAL_INT(0, res);

    captured_output = capture_stdout(call_print_empty);
    TEST_ASSERT_NOT_NULL(captured_output);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_report_print_returns_zero_on_success);
    RUN_TEST(test_report_contains_directory_name);
    RUN_TEST(test_report_contains_file_count);
    RUN_TEST(test_report_contains_directory_size);
    RUN_TEST(test_report_contains_extension);
    RUN_TEST(test_report_contains_media_size);
    RUN_TEST(test_report_returns_error_with_null_arrays);
    RUN_TEST(test_report_handles_empty_arrays);

    return UNITY_END();
}