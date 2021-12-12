//
// Created by Jason Davidson on 12/12/21.
//

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "util.h"

#define INPUT_FILENAME "day12.txt"

#define MAX_CAVES 100

typedef struct Cave {
    char *name;
    bool small;
    int connected_cave_count;
    struct Cave **connected_caves;
} Cave;

static Cave *start = NULL;
static Cave *end = NULL;
static Cave caves[MAX_CAVES];

static Cave *cave_with_name(Cave *c, int cave_count, char *name) {
    for (int i = 0; i < cave_count; ++i) {
        if (strcmp(c[i].name, name) == 0) {
            return &c[i];
        }
    }
    return NULL;
}

static bool cave_visited(Cave *c, Cave **visited, int visit_count) {
    for (int i = 0; i < visit_count; ++i) {
        if (visited[i] == c) {
            return true;
        }
    }
    return false;
}

static Cave *store_cave(char *name, int *cave_count) {
    Cave *c = cave_with_name(caves, *cave_count, name);
    if (c == NULL) {
        c = &caves[*cave_count];
        c->name = malloc((sizeof(char *) * strlen(name) + 1));
        strcpy(c->name, name);
        c->small = islower(*name);
        c->connected_cave_count = 0;
        c->connected_caves = malloc(sizeof(Cave *) * MAX_CAVES);
        if (strcmp(name, "start") == 0) {
            start = c;
        }
        if (strcmp(name, "end") == 0) {
            end = c;
        }
        *cave_count = *cave_count + 1;
    }
    return c;
}

static int find_paths(Cave *start_c, Cave *end_c, Cave **visited, int visit_count, bool visit_once) {
    if (start_c == end_c) {
        return 1;
    }
    int result = 0;
    Cave **new_visited = malloc(sizeof(Cave *) * (visit_count + start_c->small));

    int new_visit_count = 0;
    if (visited != NULL && visit_count > 0) {
        for (int i = 0; i < visit_count; ++i) {
            new_visited[new_visit_count++] = visited[i];
        }
    }

    if (start_c->small) {
        new_visited[new_visit_count++] = start_c;
    }

    for (int i = 0; i < start_c->connected_cave_count; ++i) {
        if (strcmp(start_c->connected_caves[i]->name, "start") == 0) {
            continue;
        }
        if (!cave_visited(start_c->connected_caves[i], new_visited, new_visit_count)) {
            result = result + find_paths(start_c->connected_caves[i], end_c, new_visited, new_visit_count, visit_once);
        } else if (!visit_once) {
            result = result + find_paths(start_c->connected_caves[i], end_c, new_visited, new_visit_count, 1);
        }
    }
    return result;
}


static int load() {
    char *buffer[MAX_BUFFER];
    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);
    int cave_count = 0;
    for (int i = 0; i < buffer_len; i++) {
        char *splits[2];
        str_split(buffer[i], "-", splits);

        Cave *c1 = store_cave(splits[0], &cave_count);
        Cave *c2 = store_cave(splits[1], &cave_count);
        c1->connected_caves[c1->connected_cave_count++] = c2;
        c2->connected_caves[c2->connected_cave_count++] = c1;
    }
    return cave_count;
}

static int p1() {
    load();
    return find_paths(start, end, NULL, 0, true);
}

static int p2() {
    load();
    return find_paths(start, end, NULL, 0, false);
}

int day12(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}