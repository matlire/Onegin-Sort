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

#ifndef SORTING_H
#define SORTING_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/*
    Function to sort data with data_size in direction:
                                                -1 - start->end
                                                1  - start<-end
*/
void radix_sort(line_t * const data, const size_t data_size, const int direction);

#endif
