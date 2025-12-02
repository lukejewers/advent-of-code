#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_LEN 1000

bool is_twice_repeated_sequence(long long n) {
    char str[25];
    int len = sprintf(str, "%lld", n);

    if (len % 2 != 0) return false;

    int half = len / 2;
    for (int i = 0; i < half; ++i) {
        if (str[i] != str[i + half]) return false;
    }
    return true;
}

long long part_1(long long *buffer, size_t row_len)
{
    long long acc = 0;
    for (size_t i = 0; i < row_len; i += 2) {
        for (long long j = buffer[i]; j <= buffer[i+1]; ++j) {
            if (is_twice_repeated_sequence(j)) acc += j;
        }
    }
    return acc;
}

bool is_repeated_sequence(long long n) {
    char str[25];
    int len = sprintf(str, "%lld", n);

    for (int k = 1; k <= len/2; ++k) {
        if (len % k != 0) continue;

        bool valid = true;
        for (int i = 0; i < len; ++i) {
            if (str[i] != str[i % k]) {
                valid = false;
                break;
            }
        }
        if (valid) return true;
    }
    return false;
}

long long part_2(long long *buffer, size_t row_len)
{
    long long acc = 0;
    for (size_t i = 0; i < row_len; i += 2) {
        for (long long j = buffer[i]; j <= buffer[i+1]; ++j) {
            if (is_repeated_sequence(j)) acc += j;
        }
    }
    return acc;
}

size_t parse_file(FILE *fptr, long long *buffer)
{
    size_t row = 0;
    long long start, end;
    while (fscanf(fptr, "%lld-%lld,", &start, &end) == 2) {
        buffer[row++] = start;
        buffer[row++] = end;
    }
    return row;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];

    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    long long buffer[MAX_BUFFER_LEN];
    size_t row_len = parse_file(fptr, buffer);
    fclose(fptr);

    if (strcmp(filepath, "02.sample.input") == 0) assert(part_1(buffer, row_len) == 1227775554);
    else if (strcmp(filepath, "02.input") == 0) assert(part_1(buffer, row_len) == 31839939622);

    if (strcmp(filepath, "02.sample.input") == 0) assert(part_2(buffer, row_len) == 4174379265);
    else if (strcmp(filepath, "02.input") == 0) assert(part_2(buffer, row_len) == 41662374059);

    return 0;
}
