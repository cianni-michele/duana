#include "scanner.h"
#include "utils.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

int scan_directory(const char *path, DirectoryInfo *info, const Config *config)
{
    DIR *dir = opendir(path);
    if (dir == NULL) 
    {
        fprintf(stderr, "Warning: cannot open '%s': %s\n",path, strerror(errno));
        return -1;
    }

    struct dirent *entry;
    char fullpath[PATH_MAX];
    while ((entry = readdir(dir))) 
    {
        const char *name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) 
        {
            continue;
        }

        if (entry->d_type == DT_REG) 
        {
            if (!match_extension(name, config->extensions, config->extensionsCount)) 
            {
                continue;
            }

            info->totalFiles++;

            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, name);

            struct stat st;
            if (stat(fullpath, &st) == 0) 
            {
                info->totalSize += (unsigned long)st.st_size;
            } 
            else 
            {
                fprintf(stderr, "Warning: cannot stat '%s': %s\n", fullpath, strerror(errno));
            }
        }
    }
    closedir(dir);
    return 0;
}