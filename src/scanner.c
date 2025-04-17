#include "scanner.h"
#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int scan_directory(const char* path, DirectoryInfo* info, const Config* cfg) {
    DIR* dir = opendir(path);
    if (!dir) {
        fprintf(stderr, "Warning: cannot open '%s': %s\n", path, strerror(errno));
        return -1;
    }
    info->path = path;
    info->totalFiles = 0;
    info->totalSize = 0;
    struct dirent* entry;
    char fullpath[PATH_MAX];
    while ((entry = readdir(dir))) {
        const char* name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;
        if (entry->d_type == DT_REG) {
            if (!match_extension(name, cfg->extensions, cfg->extensionsCount))
                continue;
            info->totalFiles++;
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, name);
            struct stat st;
            if (stat(fullpath, &st) == 0)
                info->totalSize += st.st_size;
            else
                fprintf(stderr, "Warning: cannot stat '%s': %s\n", fullpath, strerror(errno));
        }
    }
    closedir(dir);
    return 0;
}