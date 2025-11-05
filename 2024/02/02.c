#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 1000
#define MAX_COLS 10

bool is_sequence_safe(int sequence[MAX_COLS])
{
    int determined_sign = 0;

    for (int j = 0; j < MAX_COLS - 1; ++j) {
        if (sequence[j] == -1 || sequence[j+1] == -1) break;

        int diff = sequence[j+1] - sequence[j];
        int abs_diff = abs(diff);

        if (abs_diff < 1 || abs_diff > 3) return false;
        if (determined_sign == 0) {
            determined_sign = (diff > 0) ? 1 : -1;
        } else if ((diff > 0 && determined_sign != 1) ||
                   (diff < 0 && determined_sign != -1)) {
            return false;
        }
    }
    return true;
}

int part_1(int reports[MAX_ROWS][MAX_COLS])
{
    int safe_count = 0;
    for (int i = 0; i < MAX_ROWS; ++i) {
        if (is_sequence_safe(reports[i])) {
            safe_count++;
        }
    }
    return safe_count;
}

int part_2(int reports[MAX_ROWS][MAX_COLS])
{
    int safe_count = 0;

    for (int i = 0; i < MAX_ROWS; i++) {
        if (is_sequence_safe(reports[i])) {
            safe_count++;
            continue;
        }

        bool made_safe = false;
        for (int skip = 0; skip < MAX_COLS && reports[i][skip] != -1; skip++) {
            int temp[MAX_COLS];
            int idx = 0;
            for (int k = 0; k < MAX_COLS && reports[i][k] != -1; k++) {
                if (k != skip) temp[idx++] = reports[i][k];
            }
            while (idx < MAX_COLS) temp[idx++] = -1;

            if (is_sequence_safe(temp)) {
                made_safe = true;
                break;
            }
        }

        if (made_safe) safe_count++;
    }
    return safe_count;
}

void parse_file(FILE *fptr, int reports[MAX_ROWS][MAX_COLS])
{
    int ch, y_count = 0, x_count = 0, str_count = 0;
    long int converted_num;
    char str[10], *endptr;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == ' ' || ch == '\n') {
            if (str_count > 0) {
                str[str_count] = '\0';
                converted_num = strtol(str, &endptr, 10);
                reports[x_count][y_count++] = converted_num;
                str_count = 0;
            }
            if (ch == '\n') {
                while (y_count < MAX_COLS) {
                    reports[x_count][y_count++] = -1;
                }
                y_count = 0;
                x_count++;
            }
        }
        str[str_count++] = ch;
    }
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    int reports[MAX_ROWS][MAX_COLS] = {{0}};
    parse_file(fptr, reports);
    fclose(fptr);

    if (strcmp(filepath, "02.sample.input") == 0) assert(part_1(reports) == 2);
    else if (strcmp(filepath, "02.input") == 0) assert(part_1(reports) == 442);

    if (strcmp(filepath, "02.sample.input") == 0) assert(part_2(reports) == 4);
    else if (strcmp(filepath, "02.input") == 0) assert(part_2(reports) == 493);

    return 0;
}
