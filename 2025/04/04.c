#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_ROW_LEN 200
#define MAX_COL_LEN 200

typedef struct {
    size_t row_count;
    size_t col_count;
} GridSize;

GridSize parse_file(FILE *fptr, char grid[][MAX_COL_LEN])
{
    GridSize grid_size = {0};
    int ch, idx;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            if (grid_size.row_count == 0) grid_size.col_count = idx;
            grid_size.row_count++;
            idx = 0;
        } else grid[grid_size.row_count][idx++] = ch;
    }
    return grid_size;
}

bool roll_accessible(char grid[][MAX_COL_LEN], size_t r, size_t c, size_t row_count, size_t col_count)
{
    int directions[16] = {1,0, -1,0, 0,1, 0,-1, 1,1, 1,-1, -1,1, -1,-1};
    int adj = 0;
    for (int d = 0; d < 16; d += 2) {
        int dr = directions[d];
        int dc = directions[d+1];
        int nr = r + dr;
        int nc = c + dc;
        if (nr >= 0 && nr < row_count && nc >= 0 && nc < col_count && grid[nr][nc] == '@') adj++;
    }
    return adj < 4;
}

int part_1(char grid[][MAX_COL_LEN], size_t row_count, size_t col_count)
{
    int acc = 0;
    for (size_t r = 0; r < row_count; ++r) {
        for (size_t c = 0; c < col_count; ++c) {
            if (grid[r][c] != '@') continue;
            if (roll_accessible(grid, r, c, row_count, col_count)) acc++;
        }
    }
    return acc;
}

int part_2(char grid[][MAX_COL_LEN], size_t row_count, size_t col_count)
{
    int acc = 0;
    int accessed[MAX_ROW_LEN*MAX_COL_LEN] = {0};
    while (true) {
        int count = 0;
        for (size_t r = 0; r < row_count; ++r) {
            for (size_t c = 0; c < col_count; ++c) {
                if (grid[r][c] != '@') continue;
                if (roll_accessible(grid, r, c, row_count, col_count)) {
                    acc++;
                    accessed[count++] = r;
                    accessed[count++] = c;
                }
            }
        }
        if (count == 0) break;
        for (size_t i = 0; i < count; i += 2) {
            size_t accessed_r = accessed[i];
            size_t accessed_c = accessed[i+1];
            grid[accessed_r][accessed_c] = '.';
        }
    }
    return acc;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    char grid[MAX_ROW_LEN][MAX_COL_LEN] = {{0}};
    GridSize grid_size = parse_file(fptr, grid);
    fclose(fptr);

    if (strcmp(filepath, "04.sample.input") == 0) assert(part_1(grid, grid_size.row_count, grid_size.col_count) == 13);
    else if (strcmp(filepath, "04.input") == 0) assert(part_1(grid, grid_size.row_count, grid_size.col_count) == 1411);

    if (strcmp(filepath, "04.sample.input") == 0) assert(part_2(grid, grid_size.row_count, grid_size.col_count) == 43);
    else if (strcmp(filepath, "04.input") == 0) assert(part_2(grid, grid_size.row_count, grid_size.col_count) == 8557);

    return 0;
}
