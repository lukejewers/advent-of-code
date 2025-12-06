#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARR_LEN 1000

typedef struct {
    size_t ranges_len;
    size_t ids_len;
} ArrayLengths;

ArrayLengths parse_file(FILE *fptr, long long *ranges, long long *available_ids)
{
    ArrayLengths lens = {0};
    char line[256];
    while (fgets(line, sizeof(line), fptr) != NULL && line[0] != '\n') {
        long long start, end;
        if (sscanf(line, "%lld-%lld", &start, &end) == 2) {
            ranges[lens.ranges_len++] = start;
            ranges[lens.ranges_len++] = end;
        }
    }
    while (fgets(line, sizeof(line), fptr) != NULL) {
        long long id;
        if (sscanf(line, "%lld", &id) == 1) {
            available_ids[lens.ids_len++] = id;
        }
    }
    return lens;
}

int part_1(long long *ranges, long long *available_ids, size_t ranges_len, size_t ids_len)
{
    int acc = 0;
    for (size_t i = 0; i < ids_len; ++i) {
        long long id = available_ids[i];
        bool is_fresh = false;
        for (size_t j = 0; j < ranges_len; j += 2) {
            if (ranges[j] <= id && id <= ranges[j + 1]) {
                is_fresh = true;
                break;
            }
        }
        if (is_fresh) acc++;
    }
    return acc;
}

long long part_2(long long *ranges, size_t ranges_len) {
    for (size_t i = 0; i < ranges_len; i += 2) {
        for (size_t j = i + 2; j < ranges_len; j += 2) {
            if (ranges[j] < ranges[i]) {
                long long temp_start = ranges[i];
                long long temp_end = ranges[i + 1];
                ranges[i] = ranges[j];
                ranges[i + 1] = ranges[j + 1];
                ranges[j] = temp_start;
                ranges[j + 1] = temp_end;
            }
        }
    }

    long long total = 0;
    long long curr_start = ranges[0], curr_end = ranges[1];
    for (size_t i = 2; i < ranges_len; i += 2) {
        if (ranges[i] <= curr_end + 1) {
            if (ranges[i + 1] > curr_end) {
                curr_end = ranges[i + 1];
            }
        } else {
            total += (curr_end - curr_start + 1);
            curr_start = ranges[i];
            curr_end = ranges[i + 1];
        }
    }

    total += (curr_end - curr_start + 1);
    return total;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    long long ranges[MAX_ARR_LEN];
    long long available_ids[MAX_ARR_LEN];
    ArrayLengths lens = parse_file(fptr, ranges, available_ids);
    fclose(fptr);

    if (strcmp(filepath, "05.sample.input") == 0) assert(part_1(ranges, available_ids, lens.ranges_len, lens.ids_len) == 3);
    else if (strcmp(filepath, "05.input") == 0) assert(part_1(ranges, available_ids, lens.ranges_len, lens.ids_len) == 640);

    if (strcmp(filepath, "05.sample.input") == 0) assert(part_2(ranges, lens.ranges_len) == 14);
    else if (strcmp(filepath, "05.input") == 0) assert(part_2(ranges, lens.ranges_len) == 365804144481581);

    return 0;
}
