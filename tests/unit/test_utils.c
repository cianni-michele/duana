#include "utils.h"
#include "unity.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

static char* allocated_memory;

void setUp(void) {
    allocated_memory = NULL;
    errno = 0;
}

void tearDown(void) {
    if (allocated_memory) {
        free(allocated_memory);
        allocated_memory = NULL;
    }
}


// Test cases for du_format_size
void test_du_format_size_zero(void) {
    allocated_memory = du_format_size(0);
    TEST_ASSERT_EQUAL_STRING("0.00 B", allocated_memory);
}

void test_du_format_size_bytes(void) {
    allocated_memory = du_format_size(512);
    TEST_ASSERT_EQUAL_STRING("512.00 B", allocated_memory);
}

void test_du_format_size_kib(void) {
    allocated_memory = du_format_size(1024);
    TEST_ASSERT_EQUAL_STRING("1.00 KiB", allocated_memory);
}

void test_du_format_size_kib_decimal(void) {
    allocated_memory = du_format_size(1536);
    TEST_ASSERT_EQUAL_STRING("1.50 KiB", allocated_memory);
}

void test_du_format_size_mib(void) {
    allocated_memory = du_format_size(1048576);
    TEST_ASSERT_EQUAL_STRING("1.00 MiB", allocated_memory);
}

void test_du_format_size_gib(void) {
    allocated_memory = du_format_size(1073741824ULL);
    TEST_ASSERT_EQUAL_STRING("1.00 GiB", allocated_memory);
}


// Test cases for du_get_extension
void test_du_get_extension_simple(void) {
    allocated_memory = du_get_extension("file.txt");
    TEST_ASSERT_EQUAL_STRING("txt", allocated_memory);
}

void test_du_get_extension_uppercase(void) {
    allocated_memory = du_get_extension("photo.JPG");
    TEST_ASSERT_EQUAL_STRING("jpg", allocated_memory);
}

void test_du_get_extension_multiple_dots(void) {
    allocated_memory = du_get_extension("archive.tar.gz");
    TEST_ASSERT_EQUAL_STRING("gz", allocated_memory);
}

void test_du_get_extension_hidden_file(void) {
    allocated_memory = du_get_extension(".bashrc");
    TEST_ASSERT_NULL(allocated_memory);
}

void test_du_get_extension_empty_extension(void) {
    allocated_memory = du_get_extension("file.");
    TEST_ASSERT_NULL(allocated_memory);
}

void test_du_get_extension_with_path(void) {
    allocated_memory = du_get_extension("/path/to/FILE.TxT");
    TEST_ASSERT_EQUAL_STRING("txt", allocated_memory);
}

void test_du_get_extension_null(void) {
    allocated_memory = du_get_extension(NULL);
    TEST_ASSERT_NULL(allocated_memory);
}

int main(void) {
    UNITY_BEGIN();

    // Test du_format_size
    RUN_TEST(test_du_format_size_zero);
    RUN_TEST(test_du_format_size_bytes);
    RUN_TEST(test_du_format_size_kib);
    RUN_TEST(test_du_format_size_kib_decimal);
    RUN_TEST(test_du_format_size_mib);
    RUN_TEST(test_du_format_size_gib);

    // Test du_get_extension
    RUN_TEST(test_du_get_extension_simple);
    RUN_TEST(test_du_get_extension_uppercase);
    RUN_TEST(test_du_get_extension_multiple_dots);
    RUN_TEST(test_du_get_extension_hidden_file);
    RUN_TEST(test_du_get_extension_empty_extension);
    RUN_TEST(test_du_get_extension_with_path);
    RUN_TEST(test_du_get_extension_null);

    return UNITY_END();
}