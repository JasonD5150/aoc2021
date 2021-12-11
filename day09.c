//
// Created by Jason Davidson on 12/9/21.
//

#include <string.h>
#include <printf.h>
#include <stdlib.h>
#include "util.h"

#define INPUT_FILENAME "day09.txt"

enum MapDirection {
    UP, RIGHT, DOWN, LEFT
};

struct HeightMap {
    int height;
    int risk_level;
    int low_point;
    // clockwise where UP is 0
    struct HeightMap *neighbors[4];
};

static struct HeightMap g_energy_map[100][100];
static int g_map_width = 100;
static int g_map_height = 100;

static struct HeightMap *g_flash_stack[100 * 100];
static int g_stack_height = 0;

static struct HeightMap *hm_at(int i, int j) {
    if (i < 0) return NULL;
    if (i >= g_map_height) return NULL;
    if (j < 0) return NULL;
    if (j >= g_map_width) return NULL;
    return &g_energy_map[i][j];
}

static void set_neighbors(int i, int j, struct HeightMap *hm) {
    //starting at 12 o'clock
    int i1 = i - 1, j1 = j;
    for (int c = 0; c < 4; c++) {
        hm->neighbors[c] = hm_at(i1, j1);
        i1 = i;
        j1 = j;
        if (c == 0) {
            j1 = j + 1;
        } else if (c == 1) {
            i1 = i + 1;
        } else if (c == 2) {
            j1 = j - 1;
        }
    }
}

static struct HeightMap *neighbor(struct HeightMap *m, enum MapDirection d) {
    return m->neighbors[d];
}

static int on_stack(struct HeightMap *m) {
    for (int i = 0; i < g_stack_height; i++) {
        if (g_flash_stack[i] == m) {
            return 1;
        }
    }
    return 0;
}

static int push_stack(struct HeightMap *m) {
    if (!on_stack(m)) {
        g_flash_stack[g_stack_height++] = m;
        return 1;
    }
    return 0;
}

static int basin_neighbors(struct HeightMap *m, struct HeightMap **n) {
    int c = 0;
    for (int i = 0; i < 4; i++) {
        struct HeightMap *nm = neighbor(m, i);
        //printf("\nbasin m:%p %d neighbor = %p", m, i, nm);
        if (nm != NULL && nm->height != 9 && !on_stack(nm)) {
            push_stack(nm);
            *(n + c++) = nm;
        }
    }
    return c;
}

static void traverse_basin(struct HeightMap *m, int *count) {
    struct HeightMap *bn[4];
    int b_n_c = basin_neighbors(m, bn);
    if (b_n_c == 0) return;

    *count = *count + b_n_c;
    for (int i = 0; i < b_n_c; i++) {
        traverse_basin(*(bn + i), count);
    }
}

static int compare_neighbor_height(struct HeightMap m, enum MapDirection d) {
    struct HeightMap *n;
    n = m.neighbors[d];
    if (n != NULL) {
        return m.height - n->height;
    }
    return -1;
}

static void load() {
    char *buffer[MAX_BUFFER];
    int buffer_len;
    buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);

    g_map_height = buffer_len;
    g_map_width = (int) strlen(*buffer);

    /* load first */
    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            char c = *(buffer[i] + j);
            int h = c - 48;
            struct HeightMap hm;
            hm.height = h;
            hm.risk_level = h + 1;
            hm.low_point = 0;
            *(g_energy_map[i] + j) = hm;
        }
    }
    /* set neighbor pointers */
    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            set_neighbors(i, j, &g_energy_map[i][j]);
        }
    }
    /* set the low points */
    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            struct HeightMap *m = &g_energy_map[i][j];
            if (compare_neighbor_height(*m, UP) < 0 &&
                compare_neighbor_height(*m, RIGHT) < 0 &&
                compare_neighbor_height(*m, DOWN) < 0 &&
                compare_neighbor_height(*m, LEFT) < 0) {
                m->low_point = 1;
            }
        }
    }
}

static int low_points(struct HeightMap *lows) {
    int c = 0;
    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            struct HeightMap *m = &g_energy_map[i][j];
            if (m->low_point == 1) {
                *(lows + c++) = *m;
            }
        }
    }
    return c;
}

static int p1() {
    load();
    int risk_level = 0;
    struct HeightMap *lows = malloc((sizeof(struct HeightMap *) * g_map_height * g_map_width));
    int low_count = low_points(lows);
    for (int i = 0; i < low_count; i++) {
        risk_level = risk_level + lows[i].risk_level;
    }
    return risk_level;
}

static int p2() {
    load();

    struct HeightMap *lows = malloc((sizeof(struct HeightMap *) * g_map_height * g_map_width));
    int low_count = low_points(lows);

    int result_array[g_map_width * g_map_height];
    int ra_i = 0;
    for (int i = 0; i < low_count; i++) {
        int count = 0;
        struct HeightMap *l = &lows[i];
        traverse_basin(l, &count);
        *(result_array + ra_i++) = count;
    }
    sort_int_array_desc_s(result_array, ra_i);
    return *result_array * *(result_array + 1) * *(result_array + 2);
}

int day09(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}