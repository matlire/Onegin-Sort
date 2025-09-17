#include "sorting/sorting.h"
#include "io/io.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void free_buffers(char* buffer, char* clean_buffer, line_t* index)
{
    if (buffer)       free(buffer);
    if (clean_buffer) free(clean_buffer);
    if (index)        free(index);
}

int main()
{
    FILE*          file = load_file("dist/onegin_cleaned.txt", "r");
    ssize_t   read_size = get_file_size_stat("dist/onegin_cleaned.txt");
    size_t  buffer_size = ((size_t)read_size) + 1;

    char*       buffer = calloc(buffer_size, sizeof(char));
    if (!buffer) return 1;

    char* clean_buffer = calloc(buffer_size, sizeof(char));
    if (!clean_buffer) { free(buffer); return 1; }

    line_t* index = NULL;
    
    size_t bytes_read = read_file(file, buffer, buffer_size);
    fclose(file);
    if (bytes_read == 0) { free_buffers(buffer, clean_buffer, index); return 0; }

    size_t index_size = parse_file(buffer, clean_buffer, &index, buffer_size);
    if (index_size == 0) { free_buffers(buffer, clean_buffer, index); return 0; }

    for(size_t i = 0; i < index_size; i++)
    {
        printf("%lu. ", i);
        printf("%ld|%s|%s\n", (size_t)index[i].start_ptr, (index[i].start_ptr), (index[i].clean_str_ptr));
    }

    radix_LSD_sort(index, index_size);

    for(size_t i = 0; i < index_size; i++)
    {
        if (index[i].clean_str_ptr) printf("%li. %s|%s\n", i, index[i].start_ptr, index[i].clean_str_ptr);
    }

    free_buffers(buffer, clean_buffer, index);

    return 0;
}
