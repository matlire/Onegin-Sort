#include <stddef.h>
#include "logging/logging.h"

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
#include <ctype.h>

typedef struct
{
    FILE* file;
    
    char*  buffer;
    char*  clean_buffer;
    size_t buffer_size;

    line_t* index;
    size_t  index_size;
} operational_data_t;

/*
    Function to parse shell arguments (files to interact with)
*/
void parse_arguments(const int argc, char* const argv[], \
                     const char** in_file, const char** out_file);

/*
    Function to load file under name in mode (r, w, a, etc)
*/
FILE   *load_file (const char * const name, const char * const mode);

/*
    Function to read buffer_size bytes from file into buffer 
*/
size_t  read_file (FILE *file, operational_data_t* op_data);

/*
    Function to get file size by file's name
*/
ssize_t get_file_size_stat (const char * const filename);

/*
    Function to parse raw input file into buffer, clean_buffer and index
*/
size_t  parse_file (operational_data_t* op_data);

/*
    Function to clear file by it's filename
*/
size_t  clean_file(const char * const filename);

/*
    Function to prepare data: read from file, parse into buffer and clean_buffer
*/
size_t prepare_data(const char * const filename, operational_data_t* op_data);

/*
    Functions to print parsed data into file
    Parameters:
        label    - label of data segment we writing
        filename - name of output file
        op_data  - operational data
*/
size_t  print_parsed_to_file (const char * const filename, const char * const label, \
                              operational_data_t* op_data);

size_t  print_original_to_file (const char * const filename,                         \
                                operational_data_t* op_data);


/*
    Function to convert raw str to sortable one (is_ignore and to lower)
*/
static size_t clean_str (char* dest, const char* src);

/*
    Function to prepare raw buffers for sorting (replace \n with \0)
*/
static size_t prepare_buffers (operational_data_t* op_data);

/*
    Function to parse raw buffer into index, save ptrs to strs start and str lens
*/
static size_t parse_buffer (operational_data_t* op_data);

/*
    Function to parse from index(pointing only to buffer for now) to clean_buffer with cleaning strs
    and updating lens.
*/
static size_t handle_clean_buffer (operational_data_t* op_data);

#endif
