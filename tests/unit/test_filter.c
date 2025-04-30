#include "filter.h"
#include "unity.h"
#include "options.h"
#include <errno.h>

static DuOptions opt;

void setUp(void) {
    du_options_init(&opt);
    errno = 0;
}

void tearDown(void) {
    du_filter_clear(&opt);
    du_options_free(&opt);
}

void test_filter_init_null(void) {
    TEST_ASSERT_EQUAL_INT(0, du_filter_init(NULL, &opt));
    TEST_ASSERT_EQUAL_INT(0, opt.filter_count);
}

void test_filter_match_without_filters(void) {
    du_filter_init(NULL, &opt);
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".txt"));
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".any"));
    TEST_ASSERT_TRUE(du_filter_match(&opt, ""));
}

void test_filter_init_multiple_extensions(void) {
    TEST_ASSERT_EQUAL_INT(0, du_filter_init(".txt,.C,.Md", &opt));
    TEST_ASSERT_EQUAL_INT(3, opt.filter_count);
    TEST_ASSERT_EQUAL_STRING(".txt", opt.filter_exts[0]);
    TEST_ASSERT_EQUAL_STRING(".c", opt.filter_exts[1]);
    TEST_ASSERT_EQUAL_STRING(".md", opt.filter_exts[2]);
}

void test_filter_match_case_insensitive(void) {
    du_filter_init(".txt", &opt);
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".txt"));
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".TXT"));
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".tXt"));
}

void test_filter_match_multiple_extensions(void) {
    du_filter_init(".txt,.c,.md", &opt);
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".txt"));
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".c"));
    TEST_ASSERT_TRUE(du_filter_match(&opt, ".md"));
}

void test_filter_nomatch(void) {
    du_filter_init(".txt,.c,.md", &opt);
    TEST_ASSERT_FALSE(du_filter_match(&opt, ".pdf"));
    TEST_ASSERT_FALSE(du_filter_match(&opt, ".jpg"));
    TEST_ASSERT_FALSE(du_filter_match(&opt, ".doc"));
}

void test_filter_init_empty_csv(void) {
    TEST_ASSERT_EQUAL_INT(-1, du_filter_init("", &opt));
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

void test_filter_init_malformed_csv(void) {
    TEST_ASSERT_EQUAL_INT(-1, du_filter_init(".,,txt, \\", &opt));
    TEST_ASSERT_EQUAL_INT(EINVAL, errno);
}

void test_filter_clear(void) {
    du_filter_init(".txt,.c", &opt);
    TEST_ASSERT_EQUAL_INT(2, opt.filter_count);

    du_filter_clear(&opt);
    TEST_ASSERT_EQUAL_INT(0, opt.filter_count);
    TEST_ASSERT_NULL(opt.filter_exts);
}

void test_filter_clear_idempotent(void) {
    du_filter_clear(&opt);
    du_filter_clear(&opt);
    TEST_ASSERT_EQUAL_INT(0, opt.filter_count);
    TEST_ASSERT_NULL(opt.filter_exts);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_filter_init_null);
    RUN_TEST(test_filter_match_without_filters);
    RUN_TEST(test_filter_init_multiple_extensions);
    RUN_TEST(test_filter_match_case_insensitive);
    RUN_TEST(test_filter_match_multiple_extensions);
    RUN_TEST(test_filter_nomatch);
    RUN_TEST(test_filter_init_empty_csv);
    RUN_TEST(test_filter_init_malformed_csv);
    RUN_TEST(test_filter_clear);
    RUN_TEST(test_filter_clear_idempotent);

    return UNITY_END();
}