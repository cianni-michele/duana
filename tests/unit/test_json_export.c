#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "json_export.h"
#include "unity.h"

DirStatsArr test_dirs[2];
ExtStatsArr test_exts[2];
const char *test_file_path = "test_output.json";

void setUp(void) {
    memset(test_dirs, 0, sizeof(test_dirs));
    memset(test_exts, 0, sizeof(test_exts));
}

void tearDown(void) {
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

    remove(test_file_path);
    remove("test_output.json.tmp");
}

// Helper per leggere un file in una stringa
char* read_file_to_string(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return NULL;
    
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
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

void test_export_empty_inputs(void) {
    const DirStatsArr* dirs = NULL;
    const ExtStatsArr* exts = NULL;
    const double avg = 0.0;

    const int rc = du_json_export_file(test_file_path, dirs, exts, avg);

    TEST_ASSERT_EQUAL_INT(0, rc);
    
    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);
    
    const char *expected = "{\"directories\":[],\"extensions\":[],\"avgFileSize\":0}";
    TEST_ASSERT_EQUAL_STRING(expected, content);
    
    free(content);
}

// Test case 2: Export su file con un elemento per array
void test_export_file_with_single_entries(void) {
    test_dirs[0].path = strdup("test/dir");
    test_dirs[0].file_count = 2;
    test_dirs[0].total_size = 1024;

    test_exts[0].ext = strdup(".txt");
    test_exts[0].file_count = 3;
    test_exts[0].total_size = 2048;

    double avg = 512.0;

    int rc = du_json_export_file(test_file_path, test_dirs, test_exts, avg);
    TEST_ASSERT_EQUAL_INT(0, rc);

    // Verifica contenuto file
    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);

    const char *expected =
        "{\"directories\":[{\"path\":\"test/dir\",\"fileCount\":2,\"totalSize\":1024}],"
        "\"extensions\":[{\"ext\":\".txt\",\"fileCount\":3,\"totalSize\":2048}],"
        "\"avgFileSize\":512}";
    TEST_ASSERT_EQUAL_STRING(expected, content);
    free(content);

    // Verifica che il file temporaneo sia stato rimosso
    TEST_ASSERT_NOT_EQUAL(0, access("test_output.json.tmp", F_OK));
}

// Test case 3: Gestione caratteri speciali
void test_export_special_characters(void) {
    test_dirs[0].path = strdup("dír/üñîçøde\"\\path");
    test_dirs[0].file_count = 1;
    test_dirs[0].total_size = 123;
    double avg = 123.0;

    int rc = du_json_export_file(test_file_path, test_dirs, NULL, avg);
    
    TEST_ASSERT_EQUAL_INT(0, rc);
    
    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);
    TEST_ASSERT_NOT_NULL(strstr(content, "dír/üñîçøde\\\"\\\\path"));
    
    free(content);
}

// Test case 4: Formato directory multiple
void test_export_multiple_directories(void) {
    test_dirs[0].path = strdup("/path/one");
    test_dirs[0].file_count = 5;
    test_dirs[0].total_size = 1000;

    test_dirs[1].path = strdup("/path/two");
    test_dirs[1].file_count = 3;
    test_dirs[1].total_size = 2000;

    double avg = 500.0;

    int rc = du_json_export_file(test_file_path, test_dirs, NULL, avg);
    
    TEST_ASSERT_EQUAL_INT(0, rc);
    
    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);
    TEST_ASSERT_NOT_NULL(strstr(content, "\"path\":\"/path/one\""));
    TEST_ASSERT_NOT_NULL(strstr(content, "\"fileCount\":5"));
    TEST_ASSERT_NOT_NULL(strstr(content, "\"path\":\"/path/two\""));
    
    free(content);
}

// Test case 5: Formato estensioni multiple
void test_export_multiple_extensions(void) {
    test_exts[0].ext = strdup(".c");
    test_exts[0].file_count = 10;
    test_exts[0].total_size = 5000;

    test_exts[1].ext = strdup(".h");
    test_exts[1].file_count = 8;
    test_exts[1].total_size = 2000;

    double avg = 437.5;

    int rc = du_json_export_file(test_file_path, NULL, test_exts, avg);
    
    TEST_ASSERT_EQUAL_INT(0, rc);
    
    char *content = read_file_to_string(test_file_path);
    TEST_ASSERT_NOT_NULL(content);
    TEST_ASSERT_NOT_NULL(strstr(content, "\"ext\":\".c\""));
    TEST_ASSERT_NOT_NULL(strstr(content, "\"fileCount\":10"));
    TEST_ASSERT_NOT_NULL(strstr(content, "\"ext\":\".h\""));
    
    free(content);
}

// Test case 6: Gestione errori
void test_export_file_error_handling(void) {
    const char *invalid_path = "/percorso/inesistente/test.json";
    int rc = du_json_export_file(invalid_path, NULL, NULL, 0.0);
    TEST_ASSERT_NOT_EQUAL(0, rc);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_export_empty_inputs);
    RUN_TEST(test_export_file_with_single_entries);
    RUN_TEST(test_export_special_characters);
    RUN_TEST(test_export_multiple_directories);
    RUN_TEST(test_export_multiple_extensions);
    RUN_TEST(test_export_file_error_handling);

    return UNITY_END();
}
