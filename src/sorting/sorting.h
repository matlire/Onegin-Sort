#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

typedef struct
{
    char*  start_ptr;
    size_t str_len;

    char*  clean_str_ptr;
    size_t clean_str_len; 
} line_t;

void radix_LSD_sort(line_t* data, const size_t data_size);

#endif
