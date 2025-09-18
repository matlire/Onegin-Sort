#include "io.h"

void parse_arguments(const int argc, char* const argv[], \
                     const char** in_file, const char** out_file)
{
    CHECK(FATAL, argc != 5, "Incorrect arguments for shell");

    for (int i = 1; i < argc; i++)
    {
        if (strstr(argv[i], "--infile"))
        {
            *in_file = strdup(argv[++i]);
        }
        if (strstr(argv[i], "--outfile"))
        {
            *out_file = strdup(argv[++i]);
        }
    }
}

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

size_t read_file(FILE *file, operational_data_t* op_data)
{
    CHECK(FATAL, !file || !op_data, \
          "Error reading file, some data is missing");

    size_t read_bytes = fread(op_data->buffer, sizeof(char), op_data->buffer_size, file);
    if (read_bytes == 0) return 0;

    op_data->buffer[read_bytes] = '\0';
    
    return read_bytes;
}

static size_t prepare_buffers(operational_data_t* op_data)
{
    CHECK(FATAL, !op_data, \
          "Error preparing buffers, some data is missing");

    memcpy(op_data->clean_buffer, op_data->buffer, op_data->buffer_size);

    // Replace \n with \0
    for (size_t i = 0; i < op_data->buffer_size; i++)
    {
        if ((op_data->buffer)[i] == '\n')
        {
            op_data->index_size++;
            (op_data->buffer)[i] = '\0';
        }
    }
    op_data->index_size++;

    return 0;
}

static size_t parse_buffer(operational_data_t* op_data)
{
    CHECK(FATAL, !op_data, \
          "Error parsing buffer, some data is missing");

    size_t curr_len = 0;
    size_t start_byte = 0; 

    // Iterate through buffer to update index
    for (size_t i = 0; i < op_data->buffer_size; i++)
    {
        if ((op_data->buffer)[i] == '\0')
        {
            (op_data->index)[start_byte].str_ptr = &((op_data->buffer)[i - curr_len + 1]);
            (op_data->index)[start_byte].str_len = curr_len;
            curr_len = 0;
            start_byte++;
        }
        curr_len++;
    }

    return 0;
}

static size_t handle_clean_buffer(operational_data_t* op_data)
{
    CHECK(FATAL, !op_data, \
          "Error cleaning buffer, some data is missing");

    // Clean strs line by line
    char* new_start_ptr = (char*)(op_data->clean_buffer);
    for (size_t i = 0; i < op_data->index_size; i++)
    {
        size_t new_len = clean_str(new_start_ptr, (op_data->index)[i].str_ptr);
        (op_data->index)[i].clean_str_ptr = new_start_ptr;
        (op_data->index)[i].clean_str_len = new_len;
        new_start_ptr += new_len;
    }
    return 0;
}

size_t parse_file(operational_data_t* op_data)
{
    CHECK(FATAL, !op_data, \
          "Error parsing file, some data is missing");

    size_t prep_res = prepare_buffers(op_data);
    (op_data->index) = calloc(op_data->index_size, sizeof(line_t));
    CHECK(FATAL, !(op_data->index), \
          "Can't calloc index");

    size_t parse_res = parse_buffer(op_data);
    if (parse_res) return 0;

    size_t clean_res = handle_clean_buffer(op_data);
    if (clean_res) return 0;

    return prep_res;
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
                              operational_data_t* op_data)
{
    CHECK(FATAL, strlen(filename) == 0 || strlen(label) == 0 || !op_data, \
          "Error printing parsed result to file, some data is missing");

     // Open file to print output
    FILE* file_out = load_file(filename, "a");
    if (!file_out) return 1;

    // Print result
    fprintf(file_out, "\n\n%s\n\n", label);
    
    for(size_t i = 0; i < op_data->index_size; i++)
    {
        if (((op_data->index)[i]).clean_str_ptr) fprintf(file_out, "%s\n", (op_data->index)[i].str_ptr);
    }

    fclose(file_out);

    return 0;
}

size_t prepare_data(const char * const filename, operational_data_t* op_data)
{
    CHECK(FATAL, !filename || !op_data, \
          "No filename provided for preparing data")

    FILE* file = load_file(filename, "r");

    ssize_t read_size = get_file_size_stat(filename);
    op_data->buffer_size = ((size_t)read_size) + 1;

    op_data->buffer = calloc(op_data->buffer_size, sizeof(char));
    CHECK(FATAL, !(op_data->buffer), \
          "Can't calloc buffer");

    op_data->clean_buffer = calloc(op_data->buffer_size, sizeof(char));
    CHECK(FATAL, !(op_data->clean_buffer), \
          "Can't calloc clean_buffer");

    // Read file
    size_t bytes_read = read_file(file, op_data);
    fclose(file);

    CHECK(FATAL, bytes_read == 0, \
          "Couldn't read file");

    return 0;
}


size_t  print_original_to_file (const char * const filename,                         \
                                operational_data_t* op_data)
{
    CHECK(FATAL, !filename || !op_data, \
          "Error printing original data to file, some data is missing");

    // Open file to print output
    FILE* file_out = load_file(filename, "a");

    // Restore original
    fprintf(file_out, "\n\nOriginal Onegin\n\n");
    for(size_t i = 0; i < op_data->buffer_size; i++)
    {
        if ((op_data->buffer)[i] == '\0')
        {
            fprintf(file_out, "\n");
            continue;
        }

        fprintf(file_out, "%c", (op_data->buffer)[i]);
    } 

    fclose(file_out);

    return 0;
}
