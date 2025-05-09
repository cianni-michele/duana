#include "unity.h"
#include "sorter.h"
#include <string.h>

// Variabili globali per i test
DuOptions opt;
DirStatsArr dirs[5];
ExtStatsArr exts[5];
size_t n;

/* Helpers per creare dati di test */
static DirStatsArr make_dir(const char *path, size_t count, off_t size) {
    DirStatsArr d;
    d.path = (char *)path;
    d.file_count = count;
    d.total_size = size;
    return d;
}

static ExtStatsArr make_ext(const char *ext, size_t count, off_t size) {
    ExtStatsArr e;
    e.ext = (char *)ext;
    e.file_count = count;
    e.total_size = size;
    return e;
}

// Funzione di setup eseguita prima di ogni test
void setUp(void) {
    // Inizializza opzioni con valori predefiniti
    opt.sort_field = DU_SORT_SIZE;
    opt.sort_desc = false;
}

// Funzione di teardown eseguita dopo ogni test
void tearDown(void) {
    // Non necessario in questo caso
}

/* ====== TEST PER du_sort_dirs ====== */

void test_sort_dirs_empty(void) {
    DirStatsArr empty_dirs[1];
    du_sort_dirs(empty_dirs, 0, &opt);
    TEST_PASS(); // Verifica che non ci siano crash
}

void test_sort_dirs_single_element(void) {
    dirs[0] = make_dir("a", 3, 300);
    du_sort_dirs(dirs, 1, &opt);
    TEST_ASSERT_EQUAL_STRING("a", dirs[0].path);
}

void test_sort_dirs_asc_by_size(void) {
    dirs[0] = make_dir("a", 1, 100);
    dirs[1] = make_dir("b", 2, 50);
    dirs[2] = make_dir("c", 3, 150);

    opt.sort_field = DU_SORT_SIZE;
    opt.sort_desc = false;

    du_sort_dirs(dirs, 3, &opt);

    TEST_ASSERT_EQUAL_STRING("b", dirs[0].path);
    TEST_ASSERT_EQUAL_STRING("a", dirs[1].path);
    TEST_ASSERT_EQUAL_STRING("c", dirs[2].path);
}

void test_sort_dirs_desc_by_size(void) {
    dirs[0] = make_dir("a", 1, 100);
    dirs[1] = make_dir("b", 2, 50);
    dirs[2] = make_dir("c", 3, 150);

    opt.sort_field = DU_SORT_SIZE;
    opt.sort_desc = true;

    du_sort_dirs(dirs, 3, &opt);

    TEST_ASSERT_EQUAL_STRING("c", dirs[0].path);
    TEST_ASSERT_EQUAL_STRING("a", dirs[1].path);
    TEST_ASSERT_EQUAL_STRING("b", dirs[2].path);
}

void test_sort_dirs_asc_by_count(void) {
    dirs[0] = make_dir("a", 10, 100);
    dirs[1] = make_dir("b", 5, 200);
    dirs[2] = make_dir("c", 20, 50);

    opt.sort_field = DU_SORT_COUNT;
    opt.sort_desc = false;

    du_sort_dirs(dirs, 3, &opt);

    TEST_ASSERT_EQUAL_STRING("b", dirs[0].path);
    TEST_ASSERT_EQUAL_STRING("a", dirs[1].path);
    TEST_ASSERT_EQUAL_STRING("c", dirs[2].path);
}

void test_sort_dirs_stable_sorting(void) {
    dirs[0] = make_dir("a", 1, 100);
    dirs[1] = make_dir("b", 2, 100);
    dirs[2] = make_dir("c", 3, 100);

    opt.sort_field = DU_SORT_SIZE;
    opt.sort_desc = false;

    du_sort_dirs(dirs, 3, &opt);

    TEST_ASSERT_EQUAL_STRING("a", dirs[0].path);
    TEST_ASSERT_EQUAL_STRING("b", dirs[1].path);
    TEST_ASSERT_EQUAL_STRING("c", dirs[2].path);
}

void test_sort_dirs_null_pointers(void) {
    DirStatsArr original[] = {make_dir("x", 1, 10), make_dir("y", 2, 20)};
    DirStatsArr copy[2];
    memcpy(copy, original, sizeof(copy));

    // Test con array NULL
    du_sort_dirs(NULL, 2, &opt);
    TEST_PASS();

    // Test con opzioni NULL
    du_sort_dirs(copy, 2, NULL);
    TEST_ASSERT_EQUAL_MEMORY(original, copy, sizeof(copy));
}

/* ====== TEST PER du_sort_exts ====== */

void test_sort_exts_empty(void) {
    ExtStatsArr empty_exts[1];
    du_sort_exts(empty_exts, 0, &opt);
    TEST_PASS();
}

void test_sort_exts_single_element(void) {
    exts[0] = make_ext(".c", 4, 400);
    du_sort_exts(exts, 1, &opt);
    TEST_ASSERT_EQUAL_STRING(".c", exts[0].ext);
}

void test_sort_exts_asc_by_size(void) {
    exts[0] = make_ext(".a", 1, 100);
    exts[1] = make_ext(".b", 2, 50);
    exts[2] = make_ext(".c", 3, 150);

    opt.sort_field = DU_SORT_SIZE;
    opt.sort_desc = false;

    du_sort_exts(exts, 3, &opt);

    TEST_ASSERT_EQUAL_STRING(".b", exts[0].ext);
    TEST_ASSERT_EQUAL_STRING(".a", exts[1].ext);
    TEST_ASSERT_EQUAL_STRING(".c", exts[2].ext);
}

void test_sort_exts_desc_by_count(void) {
    exts[0] = make_ext(".x", 10, 100);
    exts[1] = make_ext(".y", 5, 200);
    exts[2] = make_ext(".z", 20, 50);

    opt.sort_field = DU_SORT_COUNT;
    opt.sort_desc = true;

    du_sort_exts(exts, 3, &opt);

    TEST_ASSERT_EQUAL_STRING(".z", exts[0].ext);
    TEST_ASSERT_EQUAL_STRING(".x", exts[1].ext);
    TEST_ASSERT_EQUAL_STRING(".y", exts[2].ext);
}

void test_sort_exts_stable_sorting(void) {
    exts[0] = make_ext(".a", 1, 100);
    exts[1] = make_ext(".b", 1, 100);
    exts[2] = make_ext(".c", 1, 100);

    du_sort_exts(exts, 3, &opt);

    TEST_ASSERT_EQUAL_STRING(".a", exts[0].ext);
    TEST_ASSERT_EQUAL_STRING(".b", exts[1].ext);
    TEST_ASSERT_EQUAL_STRING(".c", exts[2].ext);
}

void test_sort_exts_null_pointers(void) {
    du_sort_exts(NULL, 2, &opt);
    du_sort_exts(exts, 2, NULL);
    TEST_PASS();
}

// Funzione principale
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_sort_dirs_empty);
    RUN_TEST(test_sort_dirs_single_element);
    RUN_TEST(test_sort_dirs_asc_by_size);
    RUN_TEST(test_sort_dirs_desc_by_size);
    RUN_TEST(test_sort_dirs_asc_by_count);
    RUN_TEST(test_sort_dirs_stable_sorting);
    RUN_TEST(test_sort_dirs_null_pointers);

    RUN_TEST(test_sort_exts_empty);
    RUN_TEST(test_sort_exts_single_element);
    RUN_TEST(test_sort_exts_asc_by_size);
    RUN_TEST(test_sort_exts_desc_by_count);
    RUN_TEST(test_sort_exts_stable_sorting);
    RUN_TEST(test_sort_exts_null_pointers);

    return UNITY_END();
}