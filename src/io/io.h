#ifndef IO_H
#define IO_H

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "../sorting/sorting.h"

/*
    Load file tries to open file 
    Parameters:
        name  - name of file
    Output:
        FILE  - pointer to FILE
*/
FILE *load_file  (const char *const name, const char *const mode);
ssize_t get_file_size_stat(const char* filename);
size_t parse_file(FILE *file, char* buffer, char* clean_buffer, size_t buffer_size);


#endif
