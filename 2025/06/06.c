#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_ROW_LEN 10
#define MAX_COL_LEN 4096

typedef struct {
    size_t row_count;
    size_t col_count;
} ArrayDimensions;

ArrayDimensions parse_file_part1(FILE *fptr, int operands[MAX_ROW_LEN][MAX_COL_LEN], char *operators)
{
    ArrayDimensions dimensions = {0};
    int ch, num, index = 0, strnum_count = 0;
    char strnum[10];
    bool in_operator_row = false;

    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            if (strnum_count > 0) {
                strnum[strnum_count] = '\0';
                num = strtol(strnum, NULL, 10);
                if (!in_operator_row) operands[dimensions.row_count][index++] = num;
                strnum_count = 0;
            }
            if (!in_operator_row) {
                if (dimensions.row_count == 0) dimensions.col_count = index;
                dimensions.row_count++;
                index = 0;
            }
        } else if (ch == '+' || ch == '*') {
            if (dimensions.row_count > 0 && !in_operator_row) {
                in_operator_row = true;
                index = 0;
            }
            if (in_operator_row && index < dimensions.col_count) {
                operators[index++] = ch;
            }
        } else if (isspace(ch)) {
            if (strnum_count > 0) {
                strnum[strnum_count] = '\0';
                num = strtol(strnum, NULL, 10);
                if (!in_operator_row) operands[dimensions.row_count][index++] = num;
                strnum_count = 0;
            }
        } else if (isdigit(ch)) {
            strnum[strnum_count++] = ch;
        }
    }

    return dimensions;
}

ArrayDimensions parse_file_part2(FILE *fptr, char buffer[MAX_ROW_LEN][MAX_COL_LEN])
{
    ArrayDimensions dimensions = {0};
    size_t max_len = 0;
    while (fgets(buffer[dimensions.row_count], MAX_COL_LEN, fptr)) {
        size_t len = strlen(buffer[dimensions.row_count]);
        if (len > 0 && buffer[dimensions.row_count][len - 1] == '\n') {
            buffer[dimensions.row_count][len - 1] = '\0';
        }
        if (len > max_len) max_len = len;
        dimensions.row_count++;
    }
    dimensions.col_count = max_len;
    return dimensions;
}

long long part_1(int operands[][MAX_COL_LEN], char *operators, size_t row_count, size_t col_count)
{
    long long acc = 0;
    for (size_t c = 0; c < col_count; ++c) {
        long long res = 1;
        if (operators[c] == '+') res = 0;
        for (size_t r = 0; r < row_count; ++r) {
            long long curr = operands[r][c];
            if (operators[c] == '*') res *= curr;
            else if (operators[c] == '+') res += curr;
        }
        acc += res;
    }
    return acc;
}

long long part_2(char buffer[][MAX_COL_LEN], ArrayDimensions dimensions)
{
    unsigned long long nums[MAX_ROW_LEN];
    size_t num_idx = 0;
    unsigned long long total = 0;

    for (int col = dimensions.col_count - 1; col >= 0; col--) {
        unsigned long long n = 0;
        bool digit_found = false;

        for (size_t r = 0; r < dimensions.row_count; r++) {
            char ch = buffer[r][col];
            if (ch >= '0' && ch <= '9') {
                n = n * 10 + ch - '0';
                digit_found = true;
            } else if (ch == '*') {
                unsigned long long col_prod = n;
                for (size_t j = 0; j < num_idx; j++) {
                    col_prod *= nums[j];
                }
                total += col_prod;
                num_idx = 0;
                n = 0;
                digit_found = false;
            } else if (ch == '+') {
                unsigned long long col_sum = n;
                for (size_t j = 0; j < num_idx; j++) {
                    col_sum += nums[j];
                }
                total += col_sum;
                num_idx = 0;
                n = 0;
                digit_found = false;
            }
        }

        if (digit_found) nums[num_idx++] = n;
        else num_idx = 0;
    }

    return total;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];

    FILE *fptr1 = fopen(filepath, "r");
    assert(fptr1 != NULL);

    int operands[MAX_ROW_LEN][MAX_COL_LEN] = {{0}};
    char operators[MAX_COL_LEN] = {0};
    ArrayDimensions dimensions1 = parse_file_part1(fptr1, operands, operators);
    fclose(fptr1);

    long long result1 = part_1(operands, operators, dimensions1.row_count, dimensions1.col_count);
    if (strcmp(filepath, "06.sample.input") == 0) assert(result1 == 4277556);
    else if (strcmp(filepath, "06.input") == 0) assert(result1 == 5322004718681);

    FILE *fptr2 = fopen(filepath, "r");
    assert(fptr2 != NULL);

    char buffer[MAX_ROW_LEN][MAX_COL_LEN] = {{0}};
    ArrayDimensions dimensions2 = parse_file_part2(fptr2, buffer);
    fclose(fptr2);

    long long result2 = part_2(buffer, dimensions2);
    if (strcmp(filepath, "06.sample.input") == 0) assert(result2 == 3263827);
    else if (strcmp(filepath, "06.input") == 0) assert(result2 == 9876636978528);

    return 0;
}
