#include "utils.h"
#include <string.h>

bool match_extension(const char *filename, char **ext_list, size_t ext_count) 
{
    if (ext_count == 0) 
    {
        return true;
    }

    const char *dot = strrchr(filename, '.');
    if (dot == NULL) 
    {
        return false;
    }

    for (size_t i = 0; i < ext_count; i++) 
    {
        if (strcmp(dot, ext_list[i]) == 0) 
        {
            return true;
        }
    }
    
    return false;
}