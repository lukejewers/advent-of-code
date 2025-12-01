#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BUFFER_LEN 5000

int part_1(int *instructions, size_t instructions_len)
{
    int dial = 50, count = 0;
    for (size_t i = 0; i < instructions_len; ++i) {
        dial += instructions[i];

        dial = dial % 100;
        if (dial < 0) dial += 100;

        if (dial == 0) count++;
    }
    return count;
}

int part_2(int *instructions, size_t instructions_len)
{
    int dial = 50, count = 0;

    for (size_t i = 0; i < instructions_len; ++i) {
        int steps = instructions[i];
        int step_dir = (steps > 0) ? 1 : -1;

        for (size_t step = 0; step < abs(steps); ++step) {
            dial += step_dir;

            if (dial == 100) dial = 0;
            if (dial == -1) dial = 99;

            if (dial == 0) count++;
        }
    }

    return count;
}

size_t parse_file(FILE *fptr, int buffer[MAX_BUFFER_LEN])
{
    char line[10];
    size_t row = 0;
    while (fgets(line, sizeof(line), fptr)) {
        int sign = 0;
        if (line[0] == 'R')      sign = 1;
        else if (line[0] == 'L') sign = -1;
        long int num = strtol(line + 1, NULL, 10);
        buffer[row++] = num * sign;
    }
    return row;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    int buffer[MAX_BUFFER_LEN];
    size_t buffer_len = parse_file(fptr, buffer);
    fclose(fptr);

    if  (strcmp(filepath, "01.sample.input") == 0) assert(part_1(buffer, buffer_len) == 3);
    else if (strcmp(filepath, "01.input") == 0) assert(part_1(buffer, buffer_len) == 1011);

    if (strcmp(filepath, "01.sample.input") == 0) assert(part_2(buffer, buffer_len) == 6);
    else if (strcmp(filepath, "01.input") == 0) assert(part_2(buffer, buffer_len) == 5937);

    return 0;
}
