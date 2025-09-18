#include "logging/logging.h"
#include "sorting/sorting.h"
#include "io/io.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Function to automatically free provided buffers
*/
void term_prep(char* buffer, char* clean_buffer, line_t* index);

const char * const LOG_FILE = "dist/log.log";
const char * const IN_FILE  = "dist/onegin_cleaned.txt";
const char * const OUT_FILE = "dist/onegin_out.txt";

int main()
{
    printf("Starting...\n");

    // Init logging
    FILE* log_file = load_file(LOG_FILE, "w");
    init_logging(log_file, INFO);

    // Prepare data
    FILE* file = NULL;
    
    char* buffer       = NULL;
    char* clean_buffer = NULL;
    size_t buffer_size = 0;

    line_t* index      = NULL;
    size_t  index_size = 0;

    size_t prep_res = prepare_data(IN_FILE, file, &buffer, &clean_buffer, &buffer_size); 
    if (prep_res == 1) { term_prep(buffer, clean_buffer, index); return 0; }

    // Parse file
    index_size = parse_file(buffer, clean_buffer, &index, buffer_size);
    if (index_size == 0) { term_prep(buffer, clean_buffer, index); return 0; }
    
    // Clear file
    clean_file(OUT_FILE);

    // Calc results & print to file
    radix_sort(index, index_size, -1);
    print_parsed_to_file(OUT_FILE, "Onegin sorted A-Z start->end", index, index_size);
    radix_sort(index, index_size, 1);
    print_parsed_to_file(OUT_FILE, "Onegin sorted A-Z start<-end", index, index_size);

    print_original_to_file(OUT_FILE, buffer, buffer_size);

    // Clear memory
    term_prep(buffer, clean_buffer, index);

    printf("Done!");

    return 0;
}

void term_prep(char* buffer, char* clean_buffer, line_t* index)
{
    if (buffer)       free(buffer);
    if (clean_buffer) free(clean_buffer);
    if (index)        free(index);

    buffer       = NULL;
    clean_buffer = NULL;
    index        = NULL;

    close_log_file();
}
