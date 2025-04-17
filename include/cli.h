#ifndef CLI_H
#define CLI_H

#include "config.h"

// Parse command-line arguments;
// sets cfg and outputs path in 'rootPath'
int parse_cli(int argc, char* argv[], char** rootPath, Config* cfg);

#endif // CLI_H