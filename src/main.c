#include "sorting/sorting.h"
#include "io/io.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Function to automatically free provided buffers
*/
void free_buffers(char* buffer, char* clean_buffer, line_t* index)
{
    if (buffer)       free(buffer);
    if (clean_buffer) free(clean_buffer);
    if (index)        free(index);
}

int main()
{
    // Prepare data
    FILE*          file = load_file("dist/onegin_cleaned.txt", "r");
    ssize_t   read_size = get_file_size_stat("dist/onegin_cleaned.txt");
    size_t  buffer_size = ((size_t)read_size) + 1;

    char*        buffer = calloc(buffer_size, sizeof(char));
    if (!buffer) return 1;

    char*  clean_buffer = calloc(buffer_size, sizeof(char));
    if (!clean_buffer) { free(buffer); return 1; }

    line_t* index = NULL;
    
    // Read file
    size_t bytes_read = read_file(file, buffer, buffer_size);
    fclose(file);
    if (bytes_read == 0) { free_buffers(buffer, clean_buffer, index); return 0; }

    // Parse file
    size_t index_size = parse_file(buffer, clean_buffer, &index, buffer_size);
    if (index_size == 0) { free_buffers(buffer, clean_buffer, index); return 0; }

    // Open file to print output
    FILE* file_out = load_file("dist/onegin_output.txt", "w");

    // Sort A-Z start->end
    fprintf(file_out, "\n\nOnegin sorted A-Z start->end\n\n");
    radix_sort(index, index_size, -1);
    for(size_t i = 0; i < index_size; i++)
    {
        if (index[i].clean_str_ptr) fprintf(file_out, "%s\n", index[i].str_ptr);
    }

    // Sort A-Z start<-end
    fprintf(file_out, "\n\nOnegin sorted A-Z start<-end\n\n");
    radix_sort(index, index_size, 1);
    for(size_t i = 0; i < index_size; i++)
    {
        if (index[i].clean_str_ptr) fprintf(file_out, "%s\n", index[i].str_ptr);
    }

    // Restore original
    fprintf(file_out, "\n\nOriginal Onegin\n\n");
    for(size_t i = 0; i < buffer_size; i++)
    {
        if (buffer[i] == '\0')
        {
            fprintf(file_out, "\n");
            continue;
        }

        fprintf(file_out, "%c", buffer[i]);
    }

    // Clear memory
    fclose(file_out);
    free_buffers(buffer, clean_buffer, index);

    return 0;
}
