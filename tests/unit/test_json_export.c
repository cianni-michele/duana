#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "json_export.h"
#include "unity.h"

// Global test data
DirStatsArr test_dirs[2];
ExtStatsArr test_exts[2];
const char *test_file_path = "test_output.json";
const char *temp_stdout_file = "stdout_capture.tmp";

// Variables for stdout tests
static DirStatsArr *test_stdout_dirs;
static ExtStatsArr *test_stdout_exts;
static size_t test_stdout_dcount;
static size_t test_stdout_ecount;
static double test_stdout_avg;

void setUp(void) {
    // Initialize structures to zero
    memset(test_dirs, 0, sizeof(test_dirs));
    memset(test_exts, 0, sizeof(test_exts));

    // Initialize stdout test variables
    test_stdout_dirs = NULL;
    test_stdout_exts = NULL;
    test_stdout_dcount = 0;
    test_stdout_ecount = 0;
    test_stdout_avg = 0.0;
}

void tearDown(void) {
    // Free allocated memory
    for (int i = 0; i < 2; i++) {
        if (test_dirs[i].path != NULL) {
            free(test_dirs[i].path);
            test_dirs[i].path = NULL;
        }
        if (test_exts[i].ext != NULL) {
            free(test_exts[i].ext);
            test_exts[i].ext = NULL;
        }
    }

    // Remove temporary files
    remove(test_file_path);
    remove("test_output.json.tmp");
    remove(temp_stdout_file);
}

// Helper to read file into string
char* read_file_to_string(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    const long sz = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(sz + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    fread(buf, 1, sz, f);
    buf[sz] = '\0';
    fclose(f);

    return buf;
}

// Helper to capture stdout output
char* capture_stdout(void (*fn)(void)) {
    // Save original file descriptor
    const int stdout_fd = dup(STDOUT_FILENO);

    // Redirect stdout to temp file
    freopen(temp_stdout_file, "w", stdout);

    fn();
    fflush(stdout);

    // Restore stdout
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdout_fd);

    // Read and trim content
    char* content = read_file_to_string(temp_stdout_file);
    if (content) {
        const size_t len = strlen(content);
        if (len > 0 && content[len-1] == '\n') {
            content[len-1] = '\0';
        }
    }

    return content;
}

// Function for stdout tests
static void call_export_stdout(void) {
    du_json_export_stdout(test_stdout_dirs, test_stdout_dcount,
                          test_stdout_exts, test_stdout_ecount,
                          test_stdout_avg, false);
}

// Test 1: Export with empty inputs
void test_export_empty_inputs(void) {
    const int rc = du_json_export_file(test_file_path, NULL, 0, NULL, 0, 0.0, false);
    TEST_ASSERT_EQUAL_INT(0, rc);

    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);

    const char *expected = "{\"directories\":[],\"extensions\":[],\"avgFileSize\":0}";
    TEST_ASSERT_EQUAL_STRING(expected, content);
    free(content);
}

// Test 2: Export with single entries
void test_export_file_with_single_entries(void) {
    test_dirs[0].path = strdup("test/dir");
    test_dirs[0].file_count = 2;
    test_dirs[0].total_size = 1024;

    test_exts[0].ext = strdup(".txt");
    test_exts[0].file_count = 3;
    test_exts[0].total_size = 2048;

    const int rc = du_json_export_file(test_file_path,
        test_dirs, 1, test_exts, 1, 512.0, false);
    TEST_ASSERT_EQUAL_INT(0, rc);

    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);

    const char *expected =
        "{\"directories\":[{\"path\":\"test/dir\",\"fileCount\":2,\"totalSize\":1024}],"
        "\"extensions\":[{\"ext\":\".txt\",\"fileCount\":3,\"totalSize\":2048}],"
        "\"avgFileSize\":512}";
    TEST_ASSERT_EQUAL_STRING(expected, content);
    free(content);
}

// Test 3: Special character handling
void test_export_special_characters(void) {
    test_dirs[0].path = strdup("dír/üñîçøde\"\\path");
    test_dirs[0].file_count = 1;
    test_dirs[0].total_size = 123;

    const int rc = du_json_export_file(test_file_path, test_dirs, 1, NULL, 0, 123.0, false);
    TEST_ASSERT_EQUAL_INT(0, rc);

    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);
    TEST_ASSERT_NOT_NULL(strstr(content, "dír/üñîçøde\\\"\\\\path"));
    free(content);
}

// Test 4: Multiple directories export
void test_export_multiple_directories(void) {
    test_dirs[0].path = strdup("/path/one");
    test_dirs[0].file_count = 5;
    test_dirs[0].total_size = 1000;

    test_dirs[1].path = strdup("/path/two");
    test_dirs[1].file_count = 3;
    test_dirs[1].total_size = 2000;

    const int rc = du_json_export_file(test_file_path, test_dirs, 2, NULL, 0, 500.0, false);
    TEST_ASSERT_EQUAL_INT(0, rc);

    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);
    TEST_ASSERT_NOT_NULL(strstr(content, "\"/path/one\""));
    TEST_ASSERT_NOT_NULL(strstr(content, "\"/path/two\""));
    free(content);
}

// Test 5: Multiple extensions export
void test_export_multiple_extensions(void) {
    test_exts[0].ext = strdup(".c");
    test_exts[0].file_count = 10;
    test_exts[0].total_size = 5000;

    test_exts[1].ext = strdup(".h");
    test_exts[1].file_count = 8;
    test_exts[1].total_size = 2000;

    const int rc = du_json_export_file(test_file_path, NULL, 0, test_exts, 2, 437.5, false);
    TEST_ASSERT_EQUAL_INT(0, rc);

    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);
    TEST_ASSERT_NOT_NULL(strstr(content, "\".c\""));
    TEST_ASSERT_NOT_NULL(strstr(content, "\".h\""));
    free(content);
}

// Test 6: Error handling
void test_export_file_error_handling(void) {
    const int rc = du_json_export_file("/invalid/path/test.json", NULL, 0, NULL, 0, 0.0, false);
    TEST_ASSERT_NOT_EQUAL(0, rc);
}

// Test 7: Stdout empty export
void test_stdout_empty_inputs(void) {
    char *output = capture_stdout(call_export_stdout);
    TEST_ASSERT_NOT_NULL(output);
    TEST_ASSERT_EQUAL_STRING("{\"directories\":[],\"extensions\":[],\"avgFileSize\":0}", output);
    free(output);
}

// Test 8: Stdout single entry export
void test_stdout_with_single_entries(void) {
    test_dirs[0].path = strdup("test/dir");
    test_dirs[0].file_count = 2;
    test_dirs[0].total_size = 1024;

    test_exts[0].ext = strdup(".txt");
    test_exts[0].file_count = 3;
    test_exts[0].total_size = 2048;

    test_stdout_dirs = test_dirs;
    test_stdout_exts = test_exts;
    test_stdout_dcount = 1;
    test_stdout_ecount = 1;
    test_stdout_avg = 512.0;

    char *output = capture_stdout(call_export_stdout);
    TEST_ASSERT_NOT_NULL(output);
    TEST_ASSERT_NOT_NULL(strstr(output, "\"path\":\"test/dir\""));
    TEST_ASSERT_NOT_NULL(strstr(output, "\"ext\":\".txt\""));
    free(output);
}

// Test 9: Stdout multiple entries
void test_stdout_multiple_entries(void) {
    test_dirs[0].path = strdup("/path/one");
    test_dirs[0].file_count = 5;
    test_dirs[0].total_size = 1000;

    test_dirs[1].path = strdup("/path/two");
    test_dirs[1].file_count = 3;
    test_dirs[1].total_size = 2000;

    test_exts[0].ext = strdup(".c");
    test_exts[0].file_count = 10;
    test_exts[0].total_size = 5000;

    test_exts[1].ext = strdup(".h");
    test_exts[1].file_count = 8;
    test_exts[1].total_size = 2000;

    test_stdout_dirs = test_dirs;
    test_stdout_exts = test_exts;
    test_stdout_dcount = 2;
    test_stdout_ecount = 2;
    test_stdout_avg = 437.5;

    char *output = capture_stdout(call_export_stdout);
    TEST_ASSERT_NOT_NULL(output);
    TEST_ASSERT_NOT_NULL(strstr(output, "\"/path/one\""));
    TEST_ASSERT_NOT_NULL(strstr(output, "\"/path/two\""));
    TEST_ASSERT_NOT_NULL(strstr(output, "\".c\""));
    free(output);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_export_empty_inputs);
    RUN_TEST(test_export_file_with_single_entries);
    RUN_TEST(test_export_special_characters);
    RUN_TEST(test_export_multiple_directories);
    RUN_TEST(test_export_multiple_extensions);
    RUN_TEST(test_export_file_error_handling);
    RUN_TEST(test_stdout_empty_inputs);
    RUN_TEST(test_stdout_with_single_entries);
    RUN_TEST(test_stdout_multiple_entries);

    return UNITY_END();
}