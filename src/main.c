#include "logging/logging.h"
#include "sorting/sorting.h"
#include "io/io.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Function to automatically free provided buffers
*/
void terminate_prep(operational_data_t* op_data);

const char * const LOG_FILE = "dist/log.log";

const char *  IN_FILE = NULL;
const char * OUT_FILE = NULL;

int main(const int argc, char * const argv[])
{
    printf("Starting Onegin...\n"); 

    // Init logging
    init_logging(LOG_FILE, INFO);
    log_printf(INFO, "Starting Onegin...");

    // Parse shell arguments
    parse_arguments(argc, argv, &IN_FILE, &OUT_FILE);

    // Prepare data
    operational_data_t op_data = { 0 };

    size_t prep_res = prepare_data(IN_FILE, &op_data); 
    if (prep_res == 1) { terminate_prep(&op_data); return 0; }

    // Parse file
    size_t parse_res = parse_file(&op_data);
    if (parse_res != 0) { terminate_prep(&op_data); return 0; }
    
    // Clear file
    clean_file(OUT_FILE);

    // Calc results & print to file
    radix_sort(op_data.index, op_data.index_size, -1);
    print_parsed_to_file(OUT_FILE, "Onegin sorted A-Z start->end", &op_data);
    radix_sort(op_data.index, op_data.index_size, 1);
    print_parsed_to_file(OUT_FILE, "Onegin sorted A-Z start<-end", &op_data);

    print_original_to_file(OUT_FILE, &op_data);

    log_printf(INFO, "Done!");

    // Clear memory
    terminate_prep(&op_data);

    printf("Done!"); 

    return 0;
}

void terminate_prep(operational_data_t* op_data)
{
    if (op_data->buffer)       free(op_data->buffer);
    if (op_data->clean_buffer) free(op_data->clean_buffer);
    if (op_data->index)        free(op_data->index);
    if (IN_FILE)               free((char*)IN_FILE);
    if (OUT_FILE)              free((char*)OUT_FILE);

    op_data->buffer       = NULL;
    op_data->clean_buffer = NULL;
    op_data->index        = NULL;
    IN_FILE               = NULL;
    OUT_FILE              = NULL;

    close_log_file();
}
