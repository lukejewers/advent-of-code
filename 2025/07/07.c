#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_COL_LEN 200
#define MAX_ROW_LEN 200

typedef struct {
    size_t row_count;
    size_t col_count;
} ArrayDimensions;

ArrayDimensions parse_file(FILE *fptr, int grid[MAX_ROW_LEN][MAX_COL_LEN])
{
    ArrayDimensions dimensions = {0};
    int ch, index;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            if (dimensions.row_count == 0) dimensions.col_count = index;
            index = 0;
            dimensions.row_count++;
        } else if (ch == 'S' || ch == '^') {
            grid[dimensions.row_count][index] = 1;
            index++;
        } else index++;
    }

    return dimensions;
}

long long part_1(int grid[MAX_ROW_LEN][MAX_COL_LEN], ArrayDimensions dimensions)
{
    bool active_beams[MAX_COL_LEN] = {0};
    bool next_beams[MAX_COL_LEN] = {0};
    size_t start_col = 0;
    while (start_col < dimensions.col_count && grid[0][start_col] != 1) {
        start_col++;
    }
    active_beams[start_col - 1] = true;
    active_beams[start_col + 1] = true;
    long long split_count = 1;

    for (size_t r = 1; r < dimensions.row_count; ++r) {
        memset(next_beams, 0, dimensions.col_count * sizeof(bool));
        for (size_t c = 0; c < dimensions.col_count; ++c) {
            if (active_beams[c]) {
                if (grid[r][c] == 1) {
                    split_count++;
                    if (c > 0) next_beams[c - 1] = true;
                    if (c < dimensions.col_count - 1) next_beams[c + 1] = true;
                } else {
                    next_beams[c] = true;
                }
            }
        }

        memcpy(active_beams, next_beams, dimensions.col_count * sizeof(bool));

        bool beams_left = false;
        for (size_t i = 0; i < dimensions.col_count; ++i) {
            if (active_beams[i]) {
                beams_left = true;
                break;
            }
        }
        if (!beams_left) break;
    }

    return split_count;
}

long long part_2(int grid[MAX_ROW_LEN][MAX_COL_LEN], ArrayDimensions dimensions)
{
    long long timelines[MAX_ROW_LEN][MAX_COL_LEN] = {0};
    size_t start_col = 0;
    while (start_col < dimensions.col_count && grid[0][start_col] != 1) {
        start_col++;
    }
    timelines[1][start_col - 1] = 1;
    timelines[1][start_col + 1] = 1;

    for (size_t r = 1; r < dimensions.row_count - 1; ++r) {
        for (size_t c = 0; c < dimensions.col_count; ++c) {
            long long current_timeline_count = timelines[r][c];
            if (current_timeline_count) {
                if (grid[r][c] == 1) {
                    if (c > 0) timelines[r + 1][c - 1] += current_timeline_count;
                    if (c < dimensions.col_count - 1) timelines[r + 1][c + 1] += current_timeline_count;
                } else {
                    timelines[r + 1][c] += current_timeline_count;
                }
            }
        }
    }

    long long total_timelines = 0;
    size_t last_row = dimensions.row_count - 1;
    for (size_t c = 0; c < dimensions.col_count; ++c) {
        total_timelines += timelines[last_row][c];
    }

    return total_timelines;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    int grid[MAX_ROW_LEN][MAX_COL_LEN];
    ArrayDimensions dimensions = parse_file(fptr, grid);
    fclose(fptr);

    if (strcmp(filepath, "07.sample.input") == 0) assert(part_1(grid, dimensions) == 21);
    else if (strcmp(filepath, "07.input") == 0) assert(part_1(grid, dimensions) == 1667);

    if (strcmp(filepath, "07.sample.input") == 0) assert(part_2(grid, dimensions) == 40);
    else if (strcmp(filepath, "07.input") == 0) assert(part_2(grid, dimensions) == 62943905501815);

    return 0;
}
