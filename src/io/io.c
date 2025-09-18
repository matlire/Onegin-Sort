#include "io.h"

FILE *load_file (const char * const name, const char * const mode)
{
    FILE* f = fopen(name, mode);
    CHECK(FATAL, !f, \
          "Can't open file %s in mode %s", name, mode);

    return f;
}

ssize_t get_file_size_stat(const char * const filename) {
    CHECK(FATAL, !filename, \
          "No filename provided!");

    struct stat st = {  };
    if (stat(filename, &st) == 0) {
        return st.st_size;
    } else {
        CHECK(FATAL, 1, "Error getting file stats for %s", filename);
        perror("Error getting file status");
        return -1;
    }
}

static size_t clean_str(char* dest, const char* src)
{
    CHECK(ERROR, !dest || !src, \
          "No dest or src provided. Dest: %s. Src: %s", dest, src);

    size_t len = 0;
    while(*src)
    {
        char ch = *src;
        if (ispunct(ch) || ch == ' ' || ch > 'z') { src++; continue; }
        if (ch < 'a') ch = ch - 'A' + 'a';
        
        *dest++ = ch;
        len++;

        src++;
    }

    *dest = '\0';
    
    return (len + 1);
}

size_t read_file(FILE *file, char * const buffer, const size_t buffer_size)
{
    CHECK(FATAL, !file || !buffer || buffer_size == 0, \
          "Error reading file, some data is missing");

    size_t read_bytes = fread(buffer, sizeof(char), buffer_size, file);
    if (read_bytes == 0) return 0;

    buffer[read_bytes] = '\0';
    
    return read_bytes;
}

static size_t prepare_buffers(char * const buffer, char * const clean_buffer, const size_t buffer_size)
{
    CHECK(FATAL, !buffer || !clean_buffer || buffer_size == 0, \
          "Error preparing buffers, some data is missing");

    memcpy(clean_buffer, buffer, buffer_size);

    size_t index_size = 0;

    // Replace \n with \0
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
    CHECK(FATAL, !buffer || !index || buffer_size == 0, \
          "Error parsing buffer, some data is missing");

    size_t curr_len = 0;
    size_t start_byte = 0;

    // Iterate through buffer to update index
    for (size_t i = 0; i < buffer_size; i++)
    {
        if (buffer[i] == '\0')
        {
            index[start_byte].str_ptr = &(buffer[i - curr_len + 1]);
            index[start_byte].str_len = curr_len;
            curr_len = 0;
            start_byte++;
        }
        curr_len++;
    }

    return 0;
}

static size_t handle_clean_buffer(char* clean_buffer, line_t * const index, const size_t index_size)
{
    CHECK(FATAL, !clean_buffer || !index || index_size == 0, \
          "Error cleaning buffer, some data is missing");

    // Clean strs line by line
    char* new_start_ptr = (char*)clean_buffer;
    for (size_t i = 0; i < index_size; i++)
    {
        size_t new_len = clean_str(new_start_ptr, index[i].str_ptr);
        index[i].clean_str_ptr = new_start_ptr;
        index[i].clean_str_len = new_len;
        new_start_ptr += new_len;
    }
    return 0;
}

size_t parse_file(char* buffer, char * const clean_buffer, line_t** index, const size_t buffer_size)
{
    CHECK(FATAL, !buffer || !clean_buffer || !index || buffer_size == 0, \
          "Error parsing file, some data is missing");
    
    size_t index_size = prepare_buffers(buffer, clean_buffer, buffer_size);
 
    *index = calloc(index_size, sizeof(line_t));
    CHECK(FATAL, !index, \
          "Can't calloc index");
    
    size_t parse_res = parse_buffer(buffer, *index, buffer_size);
    if (parse_res) return 0;

    size_t clean_res = handle_clean_buffer(clean_buffer, *index, index_size);
    if (clean_res) return 0;

    return index_size;
}

size_t  clean_file(const char * const filename)
{
    FILE* file_out = load_file(filename, "w");
    CHECK(FATAL, !file_out, \
          "Error cleaning file %s", filename);

    fclose(file_out);
    return 0;
}

size_t  print_parsed_to_file (const char * const filename, const char * const label, \
                              const line_t * const index, const size_t index_size)
{
    CHECK(FATAL, strlen(filename) == 0 || strlen(label) == 0 || !index || index_size == 0, \
          "Error printing parsed result to file, some data is missing");

     // Open file to print output
    FILE* file_out = load_file(filename, "a");
    if (!file_out) return 1;

    // Print result
    fprintf(file_out, "\n\n%s\n\n", label);
    
    for(size_t i = 0; i < index_size; i++)
    {
        if (index[i].clean_str_ptr) fprintf(file_out, "%s\n", index[i].str_ptr);
    }

    fclose(file_out);

    return 0;
}

size_t prepare_data(const char * const filename, FILE* file,                   \
                    char** buffer, char** clean_buffer, size_t* buffer_size)
{
    CHECK(FATAL, !filename, \
          "No filename provided for preparing data")

    file = load_file(filename, "r");

    ssize_t read_size = get_file_size_stat(filename);
    *buffer_size      = ((size_t)read_size) + 1;

    *buffer = calloc(*buffer_size, sizeof(char));
    CHECK(FATAL, !buffer, \
          "Can't calloc buffer");

    *clean_buffer = calloc(*buffer_size, sizeof(char));
    CHECK(FATAL, !clean_buffer, \
          "Can't calloc clean_buffer");

    // Read file
    size_t bytes_read = read_file(file, *buffer, *buffer_size);
    fclose(file);

    CHECK(FATAL, bytes_read == 0, \
          "Couldn't read file");

    return 0;
}


size_t  print_original_to_file (const char * const filename,                         \
                                const char * const buffer, const size_t buffer_size)
{
    CHECK(FATAL, !buffer || buffer_size == 0, \
          "Error printing original data to file, some data is missing");

    // Open file to print output
    FILE* file_out = load_file(filename, "a");

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

    fclose(file_out);

    return 0;
}
