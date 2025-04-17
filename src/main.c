#include "cli.h"
#include "output.h"
#include "scanner.h"
#include <stdlib.h>

int main(int argc, char* argv[]) {
    char* rootPath = NULL;
    Config cfg;
    if (parse_cli(argc, argv, &rootPath, &cfg) < 0)
        return EXIT_FAILURE;

    DirectoryInfo info;
    if (scan_directory(rootPath, &info, &cfg) < 0)
        return EXIT_FAILURE;

    print_summary(&info);
    config_free(&cfg);
    return EXIT_SUCCESS;
}