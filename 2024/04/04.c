#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define GRID_ROW 140
#define GRID_COL 140

int part_1(char grid[GRID_ROW][GRID_COL])
{
    int acc = 0;
    int directions[16] = {1,0,-1,0,0,1,0,-1,1,1,1,-1,-1,1,-1,-1};
    for (int r = 0; r < GRID_ROW; ++r) {
        for (int c = 0; c < GRID_COL; ++c) {
            for (int d = 0; d < 16; d += 2) {
                int dr = directions[d];
                int dc = directions[d+1];

                if (r + 3 * dc < 0 || r + 3 * dc >= GRID_ROW ||
                    c + 3 * dr < 0 || c + 3 * dr >= GRID_COL) {
                    continue;
                }

                char word[5] = {
                    grid[r][c],
                    grid[r+1*dc][c+1*dr],
                    grid[r+2*dc][c+2*dr],
                    grid[r+3*dc][c+3*dr],
                    '\0'
                };

                if (strcmp(word, "XMAS") == 0) acc++;
            }
        }
    }
    return acc;
}

int part_2(char grid[GRID_ROW][GRID_COL])
{
    int acc = 0;
    for (int r = 1; r < GRID_ROW - 1; r++) {
        for (int c = 1; c < GRID_COL - 1; c++) {
            if (grid[r][c] != 'A') continue; // centre is 'A'

            if (r-1 < 0 || r+1 >= GRID_ROW || c-1 < 0 || c+1 >= GRID_COL) continue;

            char tl = grid[r-1][c-1];  // top-left
            char tr = grid[r-1][c+1];  // top-right
            char bl = grid[r+1][c-1];  // bottom-left
            char br = grid[r+1][c+1];  // bottom-right

            bool diag1_valid = false;
            if ((tl == 'M' && br == 'S') || (tl == 'S' && br == 'M')) diag1_valid = true;

            bool diag2_valid = false;
            if ((tr == 'M' && bl == 'S') || (tr == 'S' && bl == 'M')) diag2_valid = true;

            if (diag1_valid && diag2_valid) acc++;
        }
    }
    return acc;
}

void parse_file(FILE *fptr, char grid[GRID_ROW][GRID_COL])
{
    int ch, row = 0, col = 0;
    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            row++;
            col = 0;
            continue;
        }
        grid[row][col++] = ch;
    }
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    char grid[GRID_ROW][GRID_COL] = {0};
    parse_file(fptr, grid);
    fclose(fptr);

    if (strcmp(filepath, "04.sample.input") == 0) assert(part_1(grid) == 18);
    else if (strcmp(filepath, "04.input") == 0) assert(part_1(grid) == 2557);

    if (strcmp(filepath, "04.sample.input") == 0) assert(part_2(grid) == 9);
    else if (strcmp(filepath, "04.input") == 0) assert(part_2(grid) == 1854);


    return 0;
}
