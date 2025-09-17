#include "io.h"

FILE *load_file (const char *const name, const char *const mode)
{
    FILE* f = fopen(name, mode);
    if (!f)
    {
        perror("Error openning file!");
    }

    return f;
}

ssize_t get_file_size_stat(const char* filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    } else {
        perror("Error getting file status");
        return -1;
    }
}

static int is_ignore(char ch)
{
    return (ch == ' '  || ch == ',' || ch == '.' ||
            ch == '?'  || ch == '!' || ch == ':' || ch == ';' ||
            ch == '_'  || ch == '-' ||
            ch == '\'' || ch == '"' ||
            ch == '{'  || ch == '}' ||
            ch == '('  || ch == '}' ||
            ch == '['  || ch == ']' ||
            ch == '\\' || ch == '/' || ch == '|' ||
            ch == '\n' || ch == '\0'
            );
}

static size_t clean_str(char* dest, char* src)
{
    size_t len = 0;
    while(*src)
    {
        char ch = *src;
        if (is_ignore(ch)) { *src++; continue; }
        if (ch > 'z')      { *src++; continue; }
        if (ch < 'a')      ch = ch - 'A' + 'a';
        
        *dest++ = ch;
        len++;

        *src++;
    }

    *dest = '\0';
    
    return (len + 1);
}

size_t parse_file(FILE *file, char* buffer, char* clean_buffer, size_t buffer_size)
{
    if (!file || !buffer || !clean_buffer) return 0;
    
    size_t arr_index = 0;
    size_t index_size = 0;

    size_t read_bytes = fread(buffer, sizeof(char), buffer_size, file);
    if (read_bytes == 0) return 0;
    buffer[buffer_size - 1] = '\0';
    
    memcpy(clean_buffer, buffer, buffer_size);

    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer[i] == '\n')
        {
            index_size++;
            buffer[i] = '\0';
        }
    }
    index_size++;

    line_t* index = calloc(index_size, sizeof(line_t));

    size_t curr_len = 0;
    size_t start_byte = 0;
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer[i] == '\0')
        {
            index[start_byte].start_ptr = &(buffer[i - curr_len + 1]);
            index[start_byte].str_len   = curr_len;
            curr_len = 0;
            start_byte++;
        }
        curr_len++;
    }

    char* new_start_ptr = clean_buffer;
    for (size_t i = 0; i < index_size; i++)
    {
        size_t new_len = clean_str(new_start_ptr, index[i].start_ptr);
        index[i].clean_str_ptr = new_start_ptr;
        index[i].clean_str_len = new_len;
        new_start_ptr += new_len;
    }

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

    free(index);

    return arr_index;
}
