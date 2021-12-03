//
// Created by Jason Davidson on 12/2/21.
//

#include "util.h"
#include "days.h"
#include <string.h>
#include <stdlib.h>

enum direction_enum {
    UP, DOWN, FORWARD, DAFUQ
};

struct navigation {
    enum direction_enum direction;
    int degree;
};

enum direction_enum calc_direction(char *direction) {
    if (strcmp(direction, "forward") == 0) {
        return FORWARD;
    } else if (strcmp(direction, "down") == 0) {
        return DOWN;
    } else if (strcmp(direction, "up") == 0) {
        return UP;
    }
    return DAFUQ;
}

struct navigation calc_navigation(char *buffer) {
    char *splits[MAX_BUFFER];

    str_split(buffer, " ", splits);

    int degree = (int) strtol(splits[1], (char **) NULL, 10);

    struct navigation n;
    n.direction = calc_direction(splits[0]);
    n.degree = degree;
    return n;
}

static void p1(char *buffer[], int buffer_len, int *depth, int *horiz_position) {
    for (int i = 0; i < buffer_len; i++) {
        struct navigation n = calc_navigation(buffer[i]);
        if (n.direction == FORWARD) {
            *horiz_position = *horiz_position + n.degree;
        } else if (n.direction == DOWN) {
            *depth = *depth + n.degree;
        } else if (n.direction == UP) {
            *depth = *depth - n.degree;
        }
    }
}

static void p2(char *buffer[], int buffer_len, int *depth, int *horiz_position) {
    int aim = 0;
    for (int i = 0; i < buffer_len; i++) {
        struct navigation n = calc_navigation(buffer[i]);
        if (n.direction == FORWARD) {
            *horiz_position = *horiz_position + n.degree;
            *depth = *depth + (aim * n.degree);
        } else if (n.direction == DOWN) {
            aim = aim + n.degree;
        } else if (n.direction == UP) {
            aim = aim - n.degree;
        }
    }
}

int day02(enum part p) {
    char *buffer[MAX_BUFFER];
    int buffer_len = read_string_file("day02.txt", buffer, MAX_BUFFER);
    int depth = 0;
    int horiz_position = 0;

    if (p == ONE) {
        p1(buffer, buffer_len, &depth, &horiz_position);
    } else {
        p2(buffer, buffer_len, &depth, &horiz_position);
    }
    return depth * horiz_position;
}