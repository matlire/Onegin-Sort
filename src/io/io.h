#include <stddef.h>

#ifndef LINE_T
#define LINE_T
typedef struct
{
    char*  start_ptr;
    size_t str_len;

    char*  clean_str_ptr;
    size_t clean_str_len; 
} line_t;
#endif

#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

FILE   *load_file (const char * const name, const char * const mode);
size_t  read_file (FILE *file, char * const buffer, const size_t buffer_size);

ssize_t get_file_size_stat (const char * const filename);

size_t  parse_file (char* buffer, char * const clean_buffer, line_t** index, const size_t buffer_size);

#endif
