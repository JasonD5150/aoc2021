//
// Created by Jason Davidson on 12/13/21.
//

#include <string.h>
#include <printf.h>
#include <stdlib.h>
#include <stdbool.h>
#include "util.h"

#define INPUT_FILENAME "day13.txt"
#define MAX_DOT_SIZE 1350

typedef struct Fold {
    char axis;
    int fold_line;
} Fold;

static bool g_dot_map[MAX_DOT_SIZE][MAX_DOT_SIZE];

static Fold g_folds[MAX_DOT_SIZE];
static int g_fold_len = 0;

static int g_map_width = 0;
static int g_map_height = 0;

static void print_map(bool m[MAX_DOT_SIZE][MAX_DOT_SIZE], int width, int height) {
    printf("\n===== MAP %dw x %dh ====\n", width, height);
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            if (m[h][w]) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
}

static void load() {
    char *buffer[MAX_BUFFER];
    int buffer_len;
    buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);

    /* load dots */
    int c = 0;
    for (int i = 0; i < buffer_len; i++) {
        if (strlen(buffer[i]) == 0) {
            continue;
        } else if (*buffer[i] == 'f') {
            char *splits[2];
            str_split(buffer[i], "=", splits);
            int fold_line = (int) strtol(splits[1], (char **) NULL, 10);
            char fold_axis = splits[0][11];
            Fold f;
            f.axis = fold_axis;
            f.fold_line = fold_line;
            g_folds[g_fold_len++] = f;
        } else {
            char *splits[2];
            str_split(buffer[i], ",", splits);
            int g_w = (int) strtol(splits[0], (char **) NULL, 10);
            int g_h = (int) strtol(splits[1], (char **) NULL, 10);
            if (g_map_width < g_w) {
                g_map_width = g_w;
            }
            if (g_map_height < g_h) {
                g_map_height = g_h;
            }
            g_dot_map[g_h][g_w] = true;
            c++;
        }
    }
}

static void fold_map_y(Fold f) {
    for (int h = f.fold_line; h <= g_map_height; h++) {
        for (int w = 0; w <= g_map_width; w++) {
            if (g_dot_map[h][w]) {
                g_dot_map[h][w] = false;
                g_dot_map[f.fold_line - (h - f.fold_line)][w] = true;
            }
        }
    }
    g_map_height = f.fold_line;
}

static void fold_map_x(Fold f) {
    for (int h = 0; h <= g_map_height; h++) {
        for (int w = f.fold_line; w <= g_map_width; w++) {
            if (g_dot_map[h][w]) {
                g_dot_map[h][w] = false;
                g_dot_map[h][f.fold_line - (w - f.fold_line)] = true;
            }
        }
    }
    g_map_width = f.fold_line;
}

int p1() {
    load();
    fold_map_y(g_folds[0]);
    fold_map_x(g_folds[0]);

    int result = 0;
    for (int h = 0; h <= g_map_height; ++h) {
        for (int w = 0; w <= g_map_width; ++w) {
            result = result + (int) g_dot_map[h][w];
        }
    }
    return result;
}

int p2() {
    load();
    for (int i = 0; i < g_fold_len; i++) {
        if (g_folds[i].axis == 'y') {
            fold_map_y(g_folds[i]);
        } else if (g_folds[i].axis == 'x') {
            fold_map_x(g_folds[i]);
        }
    }
    print_map(g_dot_map, g_map_width, g_map_height);
    return 0;
}

int day13(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}