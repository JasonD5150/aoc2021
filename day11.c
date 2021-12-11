//
// Created by Jason Davidson on 12/11/21.
//

#include <string.h>
#include <printf.h>
#include "util.h"

#define INPUT_FILENAME "day11.txt"
#define NEIGHBOR_COUNT 8
enum MapDirection {
    UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT
};

enum EnergyState {
    NONE, WILL_FLASH, FLASHING, DID_FLASH
};
typedef struct EnergyMap {
    int energy_level;
    enum EnergyState energy_state;
    // clockwise where UP is 0, DOWN is 4
    struct EnergyMap *neighbors[NEIGHBOR_COUNT];
} EnergyMap;

static EnergyMap g_energy_map[10][10];
static int g_map_width = 100;
static int g_map_height = 100;

static void print_map() {
    printf("\n===== MAP =======\n");
    for (int i = 0; i < g_map_width; i++) {
        for (int j = 0; j < g_map_height; j++) {
            if (g_energy_map[i][j].energy_level == 10) {
                printf("*");
            } else {
                printf("%d", g_energy_map[i][j].energy_level);
            }
        }
        printf("\n");
    }
}

static EnergyMap *hm_at(int i, int j) {
    if (i < 0) return NULL;
    if (i >= g_map_height) return NULL;
    if (j < 0) return NULL;
    if (j >= g_map_width) return NULL;
    return &g_energy_map[i][j];
}

static void set_neighbors(int i, int j, EnergyMap *hm) {
    //starting at 12 o'clock
    int i1 = i - 1, j1 = j;
    for (int c = 0; c < NEIGHBOR_COUNT; c++) {
        hm->neighbors[c] = hm_at(i1, j1);
        i1 = i;
        j1 = j;
        if (c == 0) {
            i1 = i - 1; // set up for c+1
            j1 = j + 1;
        } else if (c == 1) {
            j1 = j + 1;
        } else if (c == 2) {
            j1 = j + 1;
            i1 = i + 1;
        } else if (c == 3) {
            i1 = i + 1;
        } else if (c == 4) {
            j1 = j - 1;
            i1 = i + 1;
        } else if (c == 5) {
            j1 = j - 1;
        } else if (c == 6) {
            j1 = j - 1;
            i1 = i - 1;
        }
    }
}

static EnergyMap *neighbor(EnergyMap *m, enum MapDirection d) {
    return m->neighbors[d];
}

static int step_up_neighbors(EnergyMap *m, EnergyMap **n) {
    int c = 0;
    for (int i = 0; i < NEIGHBOR_COUNT; i++) {
        EnergyMap *nm = neighbor(m, i);
        if (nm != NULL) {
            if (nm->energy_level < 10) {
                nm->energy_level = nm->energy_level + 1;
            }
            if (nm->energy_level == 10) {
                *(n + c++) = nm;
            }
        }
    }
    return c;
}

static void affect_neighbors(EnergyMap *m) {
    EnergyMap *bn[NEIGHBOR_COUNT];
    if (m->energy_level == 10 && m->energy_state != DID_FLASH) {
        m->energy_state = DID_FLASH;
        int b_n_c = step_up_neighbors(m, bn);
        if (b_n_c == 0) return;

        for (int i = 0; i < b_n_c; i++) {
            affect_neighbors(*(bn + i));
        }
    } else {
        return;
    }
}

static int increase_step(int step) {
    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            if (step > 0) {
                EnergyMap *m = &g_energy_map[i][j];
                m->energy_level = m->energy_level + 1;
            }
        }
    }

    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            if (step > 0) {
                EnergyMap *m = &g_energy_map[i][j];
                affect_neighbors(m);
            }
        }
    }
    int flashed = 0;
    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            EnergyMap *m = &g_energy_map[i][j];
            if (m->energy_level == 10) {
                m->energy_level = 0;
                m->energy_state = NONE;
                flashed++;
            }
        }
    }
    return flashed;
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
            EnergyMap hm;
            hm.energy_level = h;
            *(g_energy_map[i] + j) = hm;
        }
    }
    /* set neighbor pointers */
    for (int i = 0; i < g_map_height; i++) {
        for (int j = 0; j < g_map_width; j++) {
            set_neighbors(i, j, &g_energy_map[i][j]);
        }
    }
}

static int p1() {
    load();

    int result = 0;
    for (int i = 0; i <= 100; i++) {
        int flashed = increase_step(i);
        result = result + flashed;
        print_map();
    }
    return result;
}

static int all_flashed() {
    int c = 0;
    for (int i = 0; i < g_map_width; i++) {
        for (int j = 0; j < g_map_height; j++) {
            if (g_energy_map[i][j].energy_level == 0) {
                c++;
            }
        }
    }
    if (c == g_map_height * g_map_width) {
        return 1;
    }
    return 0;
}

static int p2() {
    load();
    int step = 0;
    while (1) {
        increase_step(step);
        print_map();
        if (all_flashed() == 1) {
            break;
        }
        step++;
    }
    return step;
}

int day11(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}