#include "io.h"

FILE *load_file (const char * const name, const char * const mode)
{
    FILE* f = fopen(name, mode);
    if (!f)
    {
        perror("Error openning file!");
    }

    return f;
}

ssize_t get_file_size_stat(const char * const filename) {
    if (!filename) return -1;

    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    } else {
        perror("Error getting file status");
        return -1;
    }
}

static int is_ignore(const char ch)
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

static size_t clean_str(char* dest, const char* src)
{
    if (!dest || !src) return 0;

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

size_t read_file(FILE *file, char * const buffer, const size_t buffer_size)
{
    if (!file || !buffer || buffer_size == 0) return 0;
    size_t read_bytes = fread(buffer, sizeof(char), buffer_size, file);
    if (read_bytes == 0) return 0;
    buffer[buffer_size - 1] = '\0';
    return read_bytes;
}

static size_t prepare_buffers(char * const buffer, char * const clean_buffer, const size_t buffer_size)
{   
    if (!buffer || !clean_buffer || buffer_size == 0) return 0;

    memcpy(clean_buffer, buffer, buffer_size);

    size_t index_size = 0;

    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer[i] == '\n')
        {
            index_size++;
            buffer[i] = '\0';
        }
    }
    index_size++;

    return index_size;
}

static size_t parse_buffer(char * const buffer, line_t * const index, const size_t buffer_size)
{
    if (!buffer || !index || buffer_size == 0) return 0;

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

    return 0;
}

static size_t handle_clean_buffer(char* clean_buffer, line_t * const index, const size_t index_size)
{
    if (!clean_buffer || !index || index_size == 0) return 0;

    char* new_start_ptr = (char*)clean_buffer;
    for (size_t i = 0; i < index_size; i++)
    {
        size_t new_len = clean_str(new_start_ptr, index[i].start_ptr);
        index[i].clean_str_ptr = new_start_ptr;
        index[i].clean_str_len = new_len;
        new_start_ptr += new_len;
    }
    return 0;
}

size_t parse_file(char* buffer, char * const clean_buffer, line_t** index, const size_t buffer_size)
{
    if (!buffer || !clean_buffer || !index || buffer_size == 0) return 0;
    
    size_t index_size = prepare_buffers(buffer, clean_buffer, buffer_size);
 
    *index = calloc(index_size, sizeof(line_t));

    parse_buffer(buffer, *index, buffer_size);

    handle_clean_buffer(clean_buffer, *index, index_size);  

    return index_size;
}
