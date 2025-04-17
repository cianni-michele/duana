#include "config.h"
#include "scanner.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    // Create a temporary directory
    char template[] = "/tmp/duana_testXXXXXX";
    char* dir = mkdtemp(template);
    assert(dir && "mkdtemp failed");

    // Create two files with known contents
    char pathbuf[PATH_MAX];
    snprintf(pathbuf, PATH_MAX, "%s/file1.bin", dir);
    FILE* f1 = fopen(pathbuf, "wb");
    assert(f1);
    // Write 10 bytes
    for (int i = 0; i < 10; i++)
        fputc('A', f1);
    fclose(f1);

    snprintf(pathbuf, PATH_MAX, "%s/file2.bin", dir);
    FILE* f2 = fopen(pathbuf, "wb");
    assert(f2);
    // Write 20 bytes
    for (int i = 0; i < 20; i++)
        fputc('B', f2);
    fclose(f2);

    // Create a subdirectory (ignored)
    snprintf(pathbuf, PATH_MAX, "%s/subdir", dir);
    mkdir(pathbuf, 0755);

    // Base config (no filter extensions)
    Config config = {.filterExtensions = false, .extensions = NULL, .extensionsCount = 0};

    // Perform scan
    DirectoryInfo info = {.path = dir, .totalFiles = 0, .totalSize = 0};
    int ret = scan_directory(info.path, &info, &config);
    assert(ret == 0);
    assert(info.totalFiles == 2);
    assert(info.totalSize == 30UL);

    // Cleanup
    snprintf(pathbuf, PATH_MAX, "%s/file1.bin", dir);
    remove(pathbuf);
    snprintf(pathbuf, PATH_MAX, "%s/file2.bin", dir);
    remove(pathbuf);
    snprintf(pathbuf, PATH_MAX, "%s/subdir", dir);
    rmdir(pathbuf);
    rmdir(dir);

    printf("All tests passed\n");
    return 0;
}