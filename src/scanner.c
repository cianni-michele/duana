#include "scanner.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

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
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) 
        {
            continue;
        }
           
        if (entry->d_type == DT_REG) 
        {
            info->totalFiles++;
        }
    }

    closedir(dir);
    return 0;
}