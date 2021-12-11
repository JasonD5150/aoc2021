//
// Created by Jason Davidson on 12/5/21.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"
#include "geometrics.h"

#define BOARD_SIZE 5
#define INPUT_FILENAME "day05.txt"

struct Space {
    struct Line *lines;
    int line_count;
    int *hits;
    int hitmap_size;
    int max_x;
    int max_y;
};

static void print_space(struct Space space) {
    printf("\n*** Space ***");
    printf("\n x:%d y:%d lines:%d hitmap_size:%d",
           space.max_x, space.max_y,
           space.line_count, space.hitmap_size);

    for (int i = 0; i < space.line_count; i++) {
        printf("\n[%d]\t(%d, %d)\t->\t(%d, %d)\trise: %d \trun %d \ttype: %s",
               i, space.lines[i].p1.x, space.lines[i].p1.y,
               space.lines[i].p2.x, space.lines[i].p2.y,
               space.lines[i].rise, space.lines[i].run,
               line_type_string(line_type(space.lines[i])));
    }

    /* print grid */
    printf("\n");
    for (int y = 0; y <= space.max_y; y++) {
        for (int x = 0; x <= space.max_x; x++) {
            int h = point_hash(make_point(x, y));
            int c = space.hits[h];
            if (c == 0) {
                printf(".");
            } else {
                printf("%d", c);
            }
        }
        printf("\n");
    }

}

static struct Point parse_pair(char *pair) {
    char *xy_split[2];
    str_split(pair, ",", xy_split);
    return make_point((int) strtol(xy_split[0], (char **) NULL, 10),
                      (int) strtol(xy_split[1], (char **) NULL, 10));
}

static void mark_hit(struct Space *space, struct Point p) {
    int h = point_hash(p);
    space->hits[h] = space->hits[h] + 1;
}

static void mark_hits(struct Space *space, struct Line line, int include_diagonal) {
    if (line.type == HORIZONTAL) {
        for (int x = line.min_p.x; x <= line.max_p.x; x++) {
            mark_hit(space, make_point(x, line.p1.y));
        }
    } else if (line.type == VERTICAL) {
        for (int y = line.min_p.y; y <= line.max_p.y; y++) {
            mark_hit(space, make_point(line.p1.x, y));
        }
    }

    if (include_diagonal == 1 && line.type == DIAGONAL) {
        int m = 1;
        if (line.min_p.y > line.max_p.y) {
            m = -1;
        }
        for (int x = line.min_p.x, y = line.min_p.y; x <= line.max_p.x; x++, y += m) {
            mark_hit(space, make_point(x, y));
        }
    }
}

static struct Space load_space() {
    char *buffer[MAX_BUFFER];
    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);

    struct Space space;

    space.lines = malloc(sizeof(struct Line) * buffer_len);
    space.line_count = 0;
    space.hitmap_size = 0;

    space.max_x = 0;
    space.max_y = 0;

    for (int i = 0; i < buffer_len; i++) {
        char *point_splits[MAX_BUFFER];
        str_split(buffer[i], " ", point_splits);

        struct Point p1 = parse_pair(point_splits[0]);
        struct Point p2 = parse_pair(point_splits[2]);
        struct Line line = make_line(p1, p2);
        space.lines[i] = line;
        space.line_count = space.line_count + 1;
        if (space.max_x < p1.x) {
            space.max_x = p1.x;
        } else if (space.max_x < p2.x) {
            space.max_x = p2.x;
        }
        if (space.max_y < p1.y) {
            space.max_y = p1.y;
        } else if (space.max_y < p2.y) {
            space.max_y = p2.y;
        }
    }
    space.hitmap_size = MAX_POINT_HASH;

    printf("\nspace initialized");

    space.hits = malloc(sizeof(int) * space.hitmap_size);

    printf("\nspace loaded");
    return space;
}

static int count_hits(struct Space space) {
    int c = 0;
    for (int i = 0; i < space.hitmap_size; i++) {
        if (space.hits[i] >= 2) {
            c++;
        }
    }
    return c;
}

static int p1() {

    struct Space space = load_space();

    for (int i = 0; i < space.line_count; i++) {
        mark_hits(&space, space.lines[i], 0);
    }
    print_space(space);

    return count_hits(space);
}

static int p2() {
    struct Space space = load_space();

    for (int i = 0; i < space.line_count; i++) {
        mark_hits(&space, space.lines[i], 1);
    }
    print_space(space);

    return count_hits(space);
}

int day05(enum part p) {

    if (p == ONE) {
        return p1();
    } else {
        return p2();
    }

}