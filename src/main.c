#include "sorting/sorting.h"
#include "io/io.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* file = load_file("dist/onegin_cleaned.txt", "r");
    ssize_t read_size = get_file_size_stat("dist/onegin_cleaned.txt");
    size_t arr_size = ((size_t)read_size) + 1;

    char* res = calloc(arr_size, sizeof(char));
    char* clean_res = calloc(arr_size, sizeof(char));
    size_t s = parse_file(file, res, clean_res, arr_size);
    fclose(file);

    free(res);
    free(clean_res);

    return 0;
}
