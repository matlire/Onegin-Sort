#include "sorting.h"

void radix_sort(line_t * const data, const size_t data_size, const int direction)
{
    CHECK(FATAL, !data || data_size < 2, \
          "Incorrect input data for radix sort");

    const size_t radix = 256 + 1; 

    // Find max line size for radix
    size_t max_line_len = 0;
    for (size_t i = 0; i < data_size; i++)
    {
        size_t len = data[i].clean_str_len;
        if (len > max_line_len) max_line_len = len + 1;
    }

    // Temp array for buckit distribution during sorting
    line_t* temp  = calloc(data_size, sizeof(line_t));
    CHECK(FATAL, !temp, \
          "Can't calloc temp for radix sort");

    // Count array for saving character occurencies
    size_t* count = calloc(radix, sizeof(size_t));
    CHECK(FATAL, !count, \
          "Can't calloc count");

    if (!count)
    {
        free(temp);
        return;
    }

    // Direction = -1/1 support
    ssize_t start_pos = (direction == -1) ? (ssize_t)max_line_len - 1 : 0;
    ssize_t end_pos   = (direction == -1) ? -1 : (ssize_t)max_line_len;
    ssize_t step      = (direction == -1) ? -1 : 1;
    
    // Sorting loop
    for (ssize_t pos = start_pos; pos != end_pos; pos += step)
    {
        memset(count, 0, radix * sizeof(size_t));
    
        // Count occurencies
        for (size_t i = 0; i < data_size; i++)
        {
            size_t  len = data[i].clean_str_len;
            if (len <= 1) continue;

            ssize_t idx = (direction == -1) ? pos : (pos - (ssize_t)(max_line_len - len));
            
            char c = (idx >= 0 && idx < (ssize_t)len) ? data[i].clean_str_ptr[idx] : 0;
            count[(size_t)c]++;
        }

        // Sum up occurencies
        for (size_t i = 1; i < radix; i++) count[i] += count[i - 1];

        // Sort loop for pos
        for (ssize_t i = data_size - 1; i >= 0; i--)
        {
            size_t  len = data[i].clean_str_len;
            if (len <= 1) continue;

            ssize_t idx = (direction == -1) ? pos : (pos - (ssize_t)(max_line_len - len));

            char c = (idx >= 0 && idx < (ssize_t)len) ? data[i].clean_str_ptr[idx] : 0;
            size_t idy = --count[(size_t)c];
            temp[idy]  = data[i];
        }

        // Update data with sorted data at pos
        for(size_t i = 0; i < data_size; i++) data[i] = temp[i];
    }
    
    free(temp);
    free(count);
}
