#ifndef DUANA_UTILS_H
#define DUANA_UTILS_H

#include <sys/types.h>

/**
 * @brief Format a byte counts into a human-readable string.
 *
 * Converts the given number of bytes into a string with units B, KiB, MiB, or GiB,
 * formatted with two decimal places. The returned string is heap-allocated and
 * must be freed by the caller.
 *
 * @param bytes Number of bytes to format.
 * @return A malloc-allocated string representing the formatted size, or NULL on allocation failure.
 */
char *du_format_size(off_t bytes);

/**
 * @brief Extract the file extension from a filename.
 *
 * Returns lowercase, malloc-allocated string containing the extension (without the dot),
 * or NULL if the filename has no extension or on allocation failure.
 * The caller is responsible for freeing the returned string.
 *
 * @param filename The filename or path to examine (may be NULL).
 * @return A malloc-allocated lowercase extension string, or NULL.
 */
char *du_get_extension(const char *filename);

/**
 * @brief Print an error message with project prefix to stderr.
 *
 * Wrapper around perror() that prefixes messages with "duana: <ctx>: ".
 * Use the current errno value.
 *
 * @param ctx Context string to include in the error message.
 */
void du_perror(const char *ctx);

/**
 * @brief Safe wrapper for malloc.
 *
 * Allocates memory of the given size. If allocation fails, prints an error
 * message to stderr and exits the program with code 1.
 *
 * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory (never NULL on success).
 */
void *du_xmalloc(size_t size);

#endif // DUANA_UTILS_H
