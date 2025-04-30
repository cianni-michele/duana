#ifndef CLI_PARSER_H
#define CLI_PARSER_H

#include "options.h"

/**
 * Parse command-line arguments into DuOptions.
 * Returns 0 on success, -1 on error (sets errno).
 */
int du_parse_args(int argc, char **argv, DuOptions *out);

#endif // CLI_PARSER_H