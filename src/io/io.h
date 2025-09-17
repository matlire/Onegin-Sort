#include <stddef.h>

/*
    Struct that describes buffer arrays
    str_ptr, str_len             refer to raw buffer
    clean_str_ptr, clean_str_len refer to cleaned(lowered, ignored ', ", etc)
*/

#ifndef LINE_T
#define LINE_T
typedef struct
{
    char*  str_ptr;
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

/*
    Function to load file under name in mode (r, w, a, etc)
*/
FILE   *load_file (const char * const name, const char * const mode);

/*
    Function to read buffer_size bytes from file into buffer 
*/
size_t  read_file (FILE *file, char * const buffer, const size_t buffer_size);

/*
    Function to get file size by file's name
*/
ssize_t get_file_size_stat (const char * const filename);

/*
    Function to parse raw input file into buffer, clean_buffer and index. Also requires buffer_size
*/
size_t  parse_file (char* buffer, char * const clean_buffer, line_t** index, const size_t buffer_size);

/*
    Function to check if we ignore character(', ", ., etc)
*/
static int    is_ignore           (const char ch);

/*
    Function to convert raw str to sortable one (is_ignore and to lower)
*/
static size_t clean_str           (char* dest, const char* src);

/*
    Function to prepare raw buffers for sorting (replace \n with \0)
*/
static size_t prepare_buffers     (char * const buffer, char * const clean_buffer, const size_t buffer_size);

/*
    Function to parse raw buffer into index, save ptrs to strs start and str lens
*/
static size_t parse_buffer        (char * const buffer, line_t * const index, const size_t buffer_size);

/*
    Function to parse from index(pointing only to buffer for now) to clean_buffer with cleaning strs
    and updating lens.
*/
static size_t handle_clean_buffer (char* clean_buffer, line_t * const index, const size_t index_size);

#endif
