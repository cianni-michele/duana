#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include "scanner.h"

int main(void) {
    // Create a temporary directory
    char template[] = "/tmp/duana_testXXXXXX";
    char *dir = mkdtemp(template);
    if (!dir) {
        perror("mkdtemp");
        return 1;
    }

    // Create two regular files
    char pathbuf[PATH_MAX];
    snprintf(pathbuf, PATH_MAX, "%s/file1.txt", dir);
    FILE *f1 = fopen(pathbuf, "w");
    assert(f1);
    fputs("hello", f1);
    fclose(f1);

    snprintf(pathbuf, PATH_MAX, "%s/file2.log", dir);
    FILE *f2 = fopen(pathbuf, "w");
    assert(f2);
    fputs("world", f2);
    fclose(f2);

    // Create a subdirectory (should be ignored)
    snprintf(pathbuf, PATH_MAX, "%s/subdir", dir);
    mkdir(pathbuf, 0755);

    // Perform scan
    DirectoryInfo info = { .path = dir, .totalFiles = 0 };
    int ret = scan_directory(info.path, &info);
    assert(ret == 0);
    assert(info.totalFiles == 2);

    // Cleanup
    snprintf(pathbuf, PATH_MAX, "%s/file1.txt", dir);
    remove(pathbuf);
    snprintf(pathbuf, PATH_MAX, "%s/file2.log", dir);
    remove(pathbuf);
    snprintf(pathbuf, PATH_MAX, "%s/subdir", dir);
    rmdir(pathbuf);
    rmdir(dir);

    printf("All tests passed\n");
    return 0;
}