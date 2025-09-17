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

#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

void radix_LSD_sort(line_t * const data, const size_t data_size);

#endif
