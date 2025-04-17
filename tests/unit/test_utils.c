#include <assert.h>
#include <stdio.h>
#include "utils.h"

int main(void) 
{
    // No filter: always true
    assert(match_extension("file.txt", NULL, 0));
    // Single extension
    {
        char *exts[] = { ".txt" };
        assert(match_extension("doc.txt", exts, 1));
        assert(!match_extension("doc.c", exts, 1));
    }
    // Multiple extensions
    {
        char *exts[] = { ".c", ".h" };
        assert(match_extension("foo.c", exts, 2));
        assert(match_extension("bar.h", exts, 2));
        assert(!match_extension("baz.cpp", exts, 2));
    }
    // No dot in filename
    {
        char *exts[] = { ".md" };
        assert(!match_extension("README", exts, 1));
    }
    // Case sensitive: ".TXT" != ".txt"
    {
        char *exts[] = { ".TXT" };
        assert(!match_extension("a.txt", exts, 1));
    }
    printf("All utils tests passed\n");
    return 0;
}