#include "cli_parser.h"
#include "unity.h"
#include "options.h"

#include <errno.h>
#include <string.h>

static DuOptions opt;

void setUp(void) {
    du_options_init(&opt);
    errno = 0;
}

void tearDown(void) {
    du_options_free(&opt);
}

void test_minimal(void) {
    char *argv[] = {"duana", "/tmp"};
    const int ret = du_parse_args(2, argv, &opt);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_STRING("/tmp", opt.path);
    TEST_ASSERT_EQUAL_INT(DU_SORT_SIZE, opt.sort_field);
    TEST_ASSERT_FALSE(opt.sort_desc);
    TEST_ASSERT_FALSE(opt.json);
    TEST_ASSERT_NULL(opt.json_file);
    TEST_ASSERT_EQUAL_INT(0, opt.filter_count);

    du_options_free(&opt);
}

void test_sort_count(void) {
    char *argv[] = {"duana", "--sort=count", "/tmp"};
    const int ret = du_parse_args(3, argv, &opt);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT(DU_SORT_COUNT, opt.sort_field);
}

void test_desc_flag(void) {
    char *argv[] = {"duana", "/tmp", "--desc", "--sort=size"};
    const int ret = du_parse_args(4, argv, &opt);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_TRUE(opt.sort_desc);
    TEST_ASSERT_EQUAL_INT(DU_SORT_SIZE, opt.sort_field);
}

void test_filter(void) {
    char *argv[] = {"duana", "--filter=.c,.h", "/tmp"};
    const int ret = du_parse_args(3, argv, &opt);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT(2, opt.filter_count);
    TEST_ASSERT_EQUAL_STRING(".c", opt.filter_exts[0]);
    TEST_ASSERT_EQUAL_STRING(".h", opt.filter_exts[1]);
}

void test_json_stdout(void) {
    char *argv[] = {"duana", "--json", "/tmp"};
    const int ret = du_parse_args(3, argv, &opt);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_TRUE(opt.json);
    TEST_ASSERT_NULL(opt.json_file);
}

void test_json_file(void) {
    char *argv[] = {"duana", "--json=out.json", "/tmp"};
    const int ret = du_parse_args(3, argv, &opt);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_TRUE(opt.json);
    TEST_ASSERT_NOT_NULL(opt.json_file);
    TEST_ASSERT_EQUAL_STRING("out.json", opt.json_file);
}

void test_combined(void) {
    char *argv[] = {"duana", "--filter=.md", "--json=out.json", "--sort=count", "--desc", "/tmp"};
    const int ret = du_parse_args(6, argv, &opt);

    TEST_ASSERT_EQUAL_INT(0, ret);
    TEST_ASSERT_EQUAL_INT(1, opt.filter_count);
    TEST_ASSERT_EQUAL_STRING(".md", opt.filter_exts[0]);
    TEST_ASSERT_TRUE(opt.json);
    TEST_ASSERT_EQUAL_STRING("out.json", opt.json_file);
    TEST_ASSERT_EQUAL_INT(DU_SORT_COUNT, opt.sort_field);
    TEST_ASSERT_TRUE(opt.sort_desc);
}

void test_unknown_opt(void) {
    char *argv[] = {"duana", "--unknown", "/tmp"};
    const int ret = du_parse_args(3, argv, &opt);

    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

void test_missing_value(void) {
    char *argv[] = {"duana", "--sort", "/tmp"};
    const int ret = du_parse_args(3, argv, &opt);

    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

void test_multiple_path(void) {
    char *argv[] = {"duana", "/tmp", "/var", "--sort=size"};
    const int ret = du_parse_args(4, argv, &opt);

    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

void test_no_path(void) {
    char *argv[] = {"duana", "--sort=size"};
    const int ret = du_parse_args(2, argv, &opt);

    TEST_ASSERT_EQUAL_INT(-1, ret);
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_minimal);
    RUN_TEST(test_sort_count);
    RUN_TEST(test_desc_flag);
    RUN_TEST(test_filter);
    RUN_TEST(test_json_stdout);
    RUN_TEST(test_json_file);
    RUN_TEST(test_combined);
    RUN_TEST(test_unknown_opt);
    RUN_TEST(test_missing_value);
    RUN_TEST(test_multiple_path);
    RUN_TEST(test_no_path);

    return UNITY_END();
}