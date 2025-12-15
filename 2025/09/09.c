#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TILES_COUNT 2000
#define MAX_POINTS_COUNT (MAX_TILES_COUNT / 2)

typedef struct {
    long long x;
    long long y;
} Point;

size_t parse_file(FILE *fptr, Point *tiles)
{
    size_t point_count = 0;
    long long x, y;
    while (fscanf(fptr, "%lld,%lld", &x, &y) == 2) {
        if (point_count >= MAX_POINTS_COUNT) break;
        tiles[point_count].x = x;
        tiles[point_count].y = y;
        point_count++;
    }
    return point_count;
}

long long part_1(Point *tiles, size_t point_count)
{
    long long largest_area = 0;
    for (size_t i = 0; i < point_count; i++) {
        for (size_t j = i + 1; j < point_count; j++) {
            long long xi = tiles[i].x;
            long long yi = tiles[i].y;
            long long xj = tiles[j].x;
            long long yj = tiles[j].y;
            long long area = (llabs(xi - xj) + 1) * (llabs(yi - yj) + 1);
            if (area > largest_area) largest_area = area;
        }
    }
    return largest_area;
}

bool is_on_boundary(long long x, long long y, Point *tiles, size_t point_count)
{
    for (size_t k = 0; k < point_count; k++) {
        long long xk = tiles[k].x;
        long long yk = tiles[k].y;

        size_t next_k = (k + 1) % point_count;
        long long xk1 = tiles[next_k].x;
        long long yk1 = tiles[next_k].y;

        if (xk == xk1) {
            if (x == xk) {
                long long y_min = (yk < yk1) ? yk : yk1;
                long long y_max = (yk > yk1) ? yk : yk1;
                if (y >= y_min && y <= y_max) return true;
            }
        } else if (yk == yk1) {
            if (y == yk) {
                long long x_min = (xk < xk1) ? xk : xk1;
                long long x_max = (xk > xk1) ? xk : xk1;
                if (x >= x_min && x <= x_max) return true;
            }
        }
    }
    return false;
}

bool is_inside_polygon(long long x, long long y, Point *tiles, size_t point_count)
{
    if (is_on_boundary(x, y, tiles, point_count)) return true;

    int intersection_count = 0;

    for (size_t k = 0; k < point_count; k++) {
        long long xk = tiles[k].x;
        long long yk = tiles[k].y;
        size_t next_k = (k + 1) % point_count;
        long long xk1 = tiles[next_k].x;
        long long yk1 = tiles[next_k].y;

        if (xk == xk1) {
            if (xk > x) {
                if (yk < yk1) {
                    if (y > yk && y <= yk1) intersection_count++;
                } else {
                    if (y > yk1 && y <= yk) intersection_count++;
                }
            }
        }
    }

    return intersection_count % 2 != 0;
}

bool check_horizontal_segment(long long x_min, long long x_max, long long y, Point *tiles, size_t point_count)
{
    for (size_t k = 0; k < point_count; k++) {
        long long xk = tiles[k].x;
        long long yk = tiles[k].y;
        size_t next_k = (k + 1) % point_count;
        long long xk1 = tiles[next_k].x;
        long long yk1 = tiles[next_k].y;
        if (xk == xk1) {
            long long seg_y_min = (yk < yk1) ? yk : yk1;
            long long seg_y_max = (yk > yk1) ? yk : yk1;
            if (xk > x_min && xk < x_max) {
                if (y > seg_y_min && y <= seg_y_max) {
                    if (is_inside_polygon(xk - 1, y, tiles, point_count) != is_inside_polygon(xk + 1, y, tiles, point_count)) {
                        return false;
                    }
                }
            }
        }
    }
    if (x_max - x_min > 1 && !is_inside_polygon(x_min + 1, y, tiles, point_count)) return false;
    return true;
}

bool check_vertical_segment(long long y_min, long long y_max, long long x, Point *tiles, size_t point_count)
{
    for (size_t k = 0; k < point_count; k++) {
        long long xk = tiles[k].x;
        long long yk = tiles[k].y;
        size_t next_k = (k + 1) % point_count;
        long long xk1 = tiles[next_k].x;
        long long yk1 = tiles[next_k].y;

        if (yk == yk1) {
            long long seg_x_min = (xk < xk1) ? xk : xk1;
            long long seg_x_max = (xk > xk1) ? xk : xk1;

            if (yk > y_min && yk < y_max) {
                if (x > seg_x_min && x <= seg_x_max) {
                    if (is_inside_polygon(x, yk - 1, tiles, point_count) != is_inside_polygon(x, yk + 1, tiles, point_count)) {
                        return false;
                    }
                }
            }
        }
    }

    if (y_max - y_min > 1 && !is_inside_polygon(x, y_min + 1, tiles, point_count)) return false;

    return true;
}

bool is_rectangle_inside_polygon(long long x1, long long y1, long long x2, long long y2, Point *tiles, size_t point_count)
{
    long long xA = x1; long long yA = y2;
    long long xB = x2; long long yB = y1;

    if (!is_inside_polygon(x1, y1, tiles, point_count)) return false;
    if (!is_inside_polygon(x2, y2, tiles, point_count)) return false;
    if (!is_inside_polygon(xA, yA, tiles, point_count)) return false;
    if (!is_inside_polygon(xB, yB, tiles, point_count)) return false;

    long long x_min = (x1 < x2) ? x1 : x2;
    long long x_max = (x1 > x2) ? x1 : x2;
    long long y_min = (y1 < y2) ? y1 : y2;
    long long y_max = (y1 > y2) ? y1 : y2;

    if (!check_horizontal_segment(x_min, x_max, y_min, tiles, point_count)) return false;
    if (!check_horizontal_segment(x_min, x_max, y_max, tiles, point_count)) return false;

    if (!check_vertical_segment(y_min, y_max, x_min, tiles, point_count)) return false;
    if (!check_vertical_segment(y_min, y_max, x_max, tiles, point_count)) return false;

    return true;
}

long long part_2(Point *tiles, size_t point_count)
{
    long long largest_area = 0;
    for (size_t i = 0; i < point_count; i++) {
        for (size_t j = i + 1; j < point_count; j++) {
            long long xi = tiles[i].x, yi = tiles[i].y;
            long long xj = tiles[j].x, yj = tiles[j].y;

            if (is_rectangle_inside_polygon(xi, yi, xj, yj, tiles, point_count)) {
                long long area = (llabs(xi - xj) + 1) * (llabs(yi - yj) + 1);
                if (area > largest_area) largest_area = area;
            }
        }
    }
    return largest_area;
}

int main(int argc, char **argv)
{
    assert(argc == 2);

    const char *filepath = argv[1];
    FILE *fptr = fopen(filepath, "r");

    Point tiles[MAX_POINTS_COUNT];
    size_t point_count = parse_file(fptr, tiles);
    fclose(fptr);

    if (strcmp(filepath, "09.sample.input") == 0) assert(part_1(tiles, point_count) == 50);
    else if (strcmp(filepath, "09.input") == 0) assert(part_1(tiles, point_count) == 4773451098);

    if (strcmp(filepath, "09.sample.input") == 0) assert(part_2(tiles, point_count) == 24);
    else if (strcmp(filepath, "09.input") == 0) assert(part_2(tiles, point_count) == 1429075575);

    return 0;
}
