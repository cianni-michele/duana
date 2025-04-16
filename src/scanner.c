#include "scanner.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int scan_directory(const char *path, DirectoryInfo *info) 
{
    DIR *dir = opendir(path);
    if (dir == NULL) 
    {
        fprintf(stderr, "Warning: cannot open '%s': %s\n", path, strerror(errno));
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) 
    {
        const char *name = entry->d_name;
        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) 
        {
            continue;
        }
           
        if (entry->d_type == DT_REG) 
        {
            // Count files
            info->totalFiles++;

            // Build full path
            char fullpath[PATH_MAX];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, name);

            // Stat to get size
            struct stat st;
            if (stat(fullpath, &st) < 0) 
            {
                fprintf(stderr, "Warning: cannot stat '%s': %s\n", fullpath, strerror(errno));
            } 
            else 
            {
                info->totalSize += (unsigned long)st.st_size;
            }
        }
    }

    closedir(dir);
    return 0;
}