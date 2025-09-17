#include "sorting.h"

void radix_LSD_sort(line_t * const data, const size_t data_size)
{
    if (!data || data_size == 0) return;

    const size_t radix = 256 + 1; 

    size_t max_line_len = 0;
    for (size_t i = 0; i < data_size; i++)
    {
        size_t len = data[i].clean_str_len;
        if (len > max_line_len) max_line_len = len + 1;
    }

    line_t* temp  = calloc(data_size, sizeof(line_t));
    if (!temp) return;

    size_t* count = calloc(radix, sizeof(size_t));
    if (!count)
    {
        free(temp);
        return;
    }

    for (ssize_t pos = max_line_len - 1; pos >= 0; pos--)
    {
        memset(count, 0, radix * sizeof(size_t));
        for (size_t i = 0; i < data_size; i++)
        {
            if (data[i].clean_str_len <= 1) continue;
            char c = (pos < (ssize_t)data[i].clean_str_len) ? data[i].clean_str_ptr[pos] : 0;
            count[(size_t)c]++;
        }

        for (size_t i = 1; i < radix; i++) count[i] += count[i - 1];
        for (ssize_t i = data_size - 1; i >= 0; i--)
        {
            if (data[i].clean_str_len <= 1) continue;
            char c = (pos < (ssize_t)data[i].clean_str_len) ? data[i].clean_str_ptr[pos] : 0;
            size_t idx = --count[(size_t)c];
            temp[idx] = data[i];
        }

        for(size_t i = 0; i < data_size; i++) data[i] = temp[i];
    }

    free(temp);
    free(count);
}
