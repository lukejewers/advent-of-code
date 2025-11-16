#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_LEN 100000

int add_mul(char *buffer, size_t index)
{
    if (buffer[index] == 'm' &&
        buffer[index+1] == 'u' &&
        buffer[index+2] == 'l' &&
        buffer[index+3] == '(') {

        index += 4;

        int count = 0;
        char first_num_str[10] = {0};
        while (isdigit(buffer[index])) {
            first_num_str[count++] = buffer[index++];
        }
        first_num_str[count] = '\0';

        if (buffer[index] != ',') return 0;
        index++;

        count = 0;
        char second_num_str[10] = {0};
        while (isdigit(buffer[index])) {
            second_num_str[count++] = buffer[index++];
        }
        second_num_str[count] = '\0';

        if (buffer[index] != ')') return 0;

        if (first_num_str[0] != '\0' && second_num_str[0] != '\0') {
            long converted_first_num = strtol(first_num_str, NULL, 10);
            long converted_second_num = strtol(second_num_str, NULL, 10);
            return converted_first_num * converted_second_num;
        }
    }
    return 0;
}

typedef enum {
    NO_OP,
    IS_DO,
    IS_DONT
} Instruction;

Instruction get_instruction(char *buffer, int index)
{
    if (buffer[index] == 'd' &&
        buffer[index+1] == 'o' &&
        buffer[index+2] == '(' &&
        buffer[index+3] == ')') {
        return IS_DO;
    } else if (buffer[index] == 'd' &&
               buffer[index+1] == 'o' &&
               buffer[index+2] == 'n' &&
               buffer[index+3] == '\'' &&
               buffer[index+4] == 't' &&
               buffer[index+5] == '(' &&
               buffer[index+6] == ')') {
        return IS_DONT;
    }
    return NO_OP;
}

int part_1(char *buffer)
{
    size_t index = 0, acc = 0;
    while (buffer[index] != '\0') {
        acc += add_mul(buffer, index);
        index++;
    }
    return acc;
}

int part_2(char *buffer)
{
    size_t index = 0;
    int acc = 0;
    bool is_do = true;
    while (buffer[index] != '\0') {
        Instruction instruction = get_instruction(buffer, index);
        if      (instruction == IS_DO)   is_do = true;
        else if (instruction == IS_DONT) is_do = false;

        if (is_do) acc += add_mul(buffer, index);
        index++;
    }
    return acc;
}

void parse_file(FILE *fptr, char *buffer)
{
    int ch;
    size_t index = 0;
    while ((ch = fgetc(fptr)) != EOF) {
        buffer[index++] = ch;
    }
    buffer[index] = '\0';
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    char buffer[MAX_BUFFER_LEN];
    parse_file(fptr, buffer);
    fclose(fptr);

    if (strcmp(filepath, "03.sample.input") == 0) assert(part_1(buffer) == 161);
    else if (strcmp(filepath, "03.input") == 0) assert(part_1(buffer) == 180233229);

    if (strcmp(filepath, "03.sample2.input") == 0) assert(part_2(buffer) == 48);
    else if (strcmp(filepath, "03.input") == 0) assert(part_2(buffer) == 95411583);

    return 0;
}
