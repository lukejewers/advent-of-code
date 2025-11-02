#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 2000

int compare(const void *a, const void *b)
{
    return (*(int*)a - *(int*)b);
}

int part_1(int *buffer, int row_count) {
    int evens[row_count/2];
    int odds[row_count/2];

    int even_idx = 0, odd_idx = 0;
    for (int i = 0; i < row_count; i++) {
        if (i % 2 == 0) evens[even_idx++] = buffer[i];
        else odds[odd_idx++] = buffer[i];
    }

    qsort(evens, even_idx, sizeof(int), compare);
    qsort(odds, odd_idx, sizeof(int), compare);

    int acc = 0;
    for (int i = 0; i < even_idx; i++) {
        acc += abs(evens[i] - odds[i]);
    }
    return acc;
}

int part_2(int *buffer, int row_count)
{
    int evens[row_count/2];
    int odds[row_count/2];

    int even_idx = 0, odd_idx = 0;
    for (int i = 0; i < row_count; i++) {
        if (i % 2 == 0) evens[even_idx++] = buffer[i];
        else odds[odd_idx++] = buffer[i];
    }

    int similarity = 0;
    for (size_t i = 0; i < row_count/2; ++i) {
        int count = 0;
        for (size_t j = 0; j < row_count/2; ++j) {
            if (evens[i] == odds[j]) count++;
        }
        similarity += evens[i] * count;
    }
    return similarity;
}

size_t parse_file(FILE *fptr, int buffer[MAX_BUFFER_SIZE])
{
    int ch, row = 0, str_count = 0;
    long int converted_num;
    char str[10], *endptr;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == ' ' || ch == '\n') {
            if (str_count > 0) {
                str[str_count] = '\0';
                converted_num = strtol(str, &endptr, 10);
                buffer[row++] = converted_num;
                str_count = 0;
            }
        } else {
            str[str_count++] = ch;
        }
    }
    return row;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    int buffer[MAX_BUFFER_SIZE];
    size_t row_count = parse_file(fptr, buffer);
    fclose(fptr);

    if (strcmp(filepath, "01.sample.input") == 0) assert(part_1(buffer, row_count) == 11);
    else if (strcmp(filepath, "01.input") == 0) assert(part_1(buffer, row_count) == 2815556);

    if (strcmp(filepath, "01.sample.input") == 0) assert(part_2(buffer, row_count) == 31);
    else if (strcmp(filepath, "01.input") == 0) assert(part_2(buffer, row_count) == 23927637);

    return 0;
}
