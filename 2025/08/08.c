#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_COL_LEN 3
#define MAX_ROW_LEN 1000
#define MAX_EDGES (MAX_ROW_LEN * (MAX_ROW_LEN - 1) / 2)

typedef struct {
    long long x, y, z;
    int circuit;
} Box;

typedef struct {
    int box1, box2;
    long long dist_sq;
} Edge;

int parent[MAX_ROW_LEN];
int set_size[MAX_ROW_LEN];

size_t parse_file(FILE *fptr, Box boxes[MAX_ROW_LEN])
{
    size_t box_count = 0;
    long long x,y,z;
    while (fscanf(fptr, "%lld,%lld,%lld", &x, &y, &z) == 3) {
        if (box_count >= MAX_ROW_LEN) break;
        boxes[box_count].x = x;
        boxes[box_count].y = y;
        boxes[box_count].z = z;
        box_count++;
    }
    return box_count;
}

int compare_edges_ascending(const void *a, const void *b) {
    const Edge *edge_a = (const Edge *)a;
    const Edge *edge_b = (const Edge *)b;

    if (edge_a->dist_sq < edge_b->dist_sq) return -1;
    if (edge_a->dist_sq > edge_b->dist_sq) return 1;
    return 0;
}

int compare_val_descending(const long long *a, const long long *b) {
    if (*a < *b) return 1;
    if (*a > *b) return -1;
    return 0;
}

long long part_1(Box boxes[MAX_ROW_LEN], Edge distances[MAX_EDGES], size_t box_count, int target_connections)
{
    int edge_count = 0;
    for (size_t i = 0; i < box_count; ++i) {
        boxes[i].circuit = i;
        for (size_t j = i + 1; j < box_count; ++j) {
            long long dx = boxes[i].x - boxes[j].x;
            long long dy = boxes[i].y - boxes[j].y;
            long long dz = boxes[i].z - boxes[j].z;
            long long dist_sq = dx * dx + dy * dy + dz * dz;

            distances[edge_count].box1 = i;
            distances[edge_count].box2 = j;
            distances[edge_count].dist_sq = dist_sq;
            edge_count++;
        }
    }

    qsort(distances, edge_count, sizeof(Edge), compare_edges_ascending);

    for (int i = 0; i < target_connections; ++i) {
        int circuit1 = boxes[distances[i].box1].circuit;
        int circuit2 = boxes[distances[i].box2].circuit;

        if (circuit2 != circuit1) {
            int h = circuit1;
            circuit1 = circuit2;
            circuit2 = h;
        }

        for (size_t j = 0; j < box_count; ++j) {
            if (boxes[j].circuit == circuit2) boxes[j].circuit = circuit1;
        }
    }

    long long circuit_sizes[MAX_ROW_LEN] = {0};
    for (size_t i = 0; i < box_count; ++i) {
        circuit_sizes[boxes[i].circuit]++;
    }

    long long final_sizes[MAX_ROW_LEN];
    int final_count = 0;
    for (size_t i = 0; i < box_count; ++i) {
        if (circuit_sizes[i] > 0) {
            final_sizes[final_count++] = circuit_sizes[i];
        }
    }

    qsort(final_sizes, final_count, sizeof(long long), compare_val_descending);

    long long answer = 1;
    for (int i = 0; i < 3 && i < final_count; ++i) {
        answer *= final_sizes[i];
    }

    return answer;
}

long long part_2(Box boxes[MAX_ROW_LEN], Edge distances[MAX_EDGES], size_t box_count)
{
    int edge_count = 0;
    for (size_t i = 0; i < box_count; ++i) {
        boxes[i].circuit = i;

        for (size_t j = i + 1; j < box_count; ++j) {
            long long dx = boxes[i].x - boxes[j].x;
            long long dy = boxes[i].y - boxes[j].y;
            long long dz = boxes[i].z - boxes[j].z;
            long long dist_sq = dx * dx + dy * dy + dz * dz;

            distances[edge_count].box1 = i;
            distances[edge_count].box2 = j;
            distances[edge_count].dist_sq = dist_sq;
            edge_count++;
        }
    }

    qsort(distances, edge_count, sizeof(Edge), compare_edges_ascending);

    int num_circuits = (int)box_count;
    Edge last_connecting_edge = {0};
    int edge_index = 0;

    while (num_circuits > 1 && edge_index < edge_count) {

        Edge current_edge = distances[edge_index];
        edge_index++;

        int box_a_index = current_edge.box1;
        int box_b_index = current_edge.box2;

        int circuit1 = boxes[box_a_index].circuit;
        int circuit2 = boxes[box_b_index].circuit;

        if (circuit1 != circuit2) {
            int target_circuit = circuit1 < circuit2 ? circuit1 : circuit2;
            int old_circuit = circuit1 > circuit2 ? circuit1 : circuit2;

            for (size_t j = 0; j < box_count; ++j) {
                if (boxes[j].circuit == old_circuit) {
                    boxes[j].circuit = target_circuit;
                }
            }

            num_circuits--;
            last_connecting_edge = current_edge;
        }
    }

    return boxes[last_connecting_edge.box1].x * boxes[last_connecting_edge.box2].x;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");
    assert(fptr != NULL);

    Box boxes[MAX_ROW_LEN];
    Edge distances[MAX_EDGES];
    size_t box_count = parse_file(fptr, boxes);
    fclose(fptr);

    if (strcmp(filepath, "08.sample.input") == 0) assert(part_1(boxes, distances, box_count, 10) == 40);
    else if (strcmp(filepath, "08.input") == 0) assert(part_1(boxes, distances, box_count, 1000) == 84968);

    if (strcmp(filepath, "08.sample.input") == 0) assert(part_2(boxes, distances, box_count) == 25272);
    else if (strcmp(filepath, "08.input") == 0) assert(part_2(boxes, distances, box_count) == 8663467782);

    return 0;
}
