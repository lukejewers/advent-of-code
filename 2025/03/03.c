#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_ROW_LEN 200
#define MAX_COL_LEN 100

typedef struct {
    size_t rows;
    size_t cols;
} GridSize;

int part_1(int buffer[][MAX_COL_LEN], size_t rows_count, size_t cols_count)
{
    int acc = 0;
    for (size_t r = 0; r < rows_count; ++r) {
        int max = buffer[r][0] * 10 + buffer[r][1];
        for (size_t i = 0; i < cols_count - 1; ++i) {
            for (size_t j = i+1; j < cols_count; ++j) {
                int curr = (buffer[r][i] * 10) + buffer[r][j];
                if (curr > max) max = curr;
            }
        }
        acc += max;
    }
    return acc;
}

long long part_2(int buffer[][MAX_COL_LEN], size_t rows_count, size_t cols_count)
{
    int target_digits_len = 12;
    long long acc = 0;

    for (size_t r = 0; r < rows_count; ++r) {
        int result[target_digits_len];
        int pos = 0;

        for (int i = 0; i < target_digits_len; ++i) {
            int max_val = -1;
            int max_idx = pos;

            for (int j = pos; j <= cols_count - (target_digits_len - i); ++j) {
                if (buffer[r][j] > max_val) {
                    max_val = buffer[r][j];
                    max_idx = j;
                }
            }

            result[i] = max_val;
            pos = max_idx + 1;
        }

        long long num = 0;
        for (int i = 0; i < target_digits_len; ++i) {
            num = num * 10 + result[i];
        }

        acc += num;
    }
    return acc;
}

GridSize parse_file(FILE *fptr, int buffer[MAX_ROW_LEN][MAX_COL_LEN])
{
    GridSize grid_size = {0};
    int ch, curr_col;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            if (grid_size.rows == 0) grid_size.cols = curr_col;
            grid_size.rows++;
            curr_col = 0;
        }
        if (isdigit(ch)) buffer[grid_size.rows][curr_col++] = ch - '0';
    }
    return grid_size;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    int buffer[MAX_ROW_LEN][MAX_COL_LEN] = {{0}};
    GridSize grid_size = parse_file(fptr, buffer);

    if (strcmp(filepath, "03.sample.input") == 0) assert(part_1(buffer, grid_size.rows, grid_size.cols) == 357);
    else if (strcmp(filepath, "03.input") == 0) assert(part_1(buffer, grid_size.rows, grid_size.cols) == 17430);

    if (strcmp(filepath, "03.sample.input") == 0) assert(part_2(buffer, grid_size.rows, grid_size.cols) == 3121910778619);
    else if (strcmp(filepath, "03.input") == 0) assert(part_2(buffer, grid_size.rows, grid_size.cols) == 171975854269367);

    return 0;
}
