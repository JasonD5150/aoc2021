//
// Created by Jason Davidson on 12/5/21.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "geometrics.h"

int point_hash(struct Point p) {
    char s[10];
    sprintf(s, "%d%04d",p.x, p.y);
    int hash = (int) strtol(s, (char **)NULL, 10);
    assert(hash <= MAX_POINT_HASH);
    return hash;
}

struct Point point_from_hash(int hash) {
    int s = (int)sqrt(MAX_POINT_HASH);
    for (int x = 0; x < s; x++) {
        for (int y = 0; y < s; y++) {
            struct Point p = make_point(x, y);
            int h = point_hash(p);
            if (h == hash) {
                return p;
            }
        }
    }
}

struct Point make_point(int x, int y) {
    struct Point p;
    p.x = x;
    p.y = y;
    return p;
}

int eq_points(struct Point p1, struct Point p2) {
    if (p1.x == p2.x && p1.y == p2.y) return 1;
    return 0;
}

enum LineType line_type(struct Line line) {
    if (line.p1.y == line.p2.y) return HORIZONTAL;
    if (line.p1.x == line.p2.x) return VERTICAL;
    return DIAGONAL;
}

char *line_type_string(enum LineType line_type) {
    if (line_type == HORIZONTAL) {
        return "HORIZONTAL";
    } else if (line_type == VERTICAL) {
        return "VERTICAL";
    }
    return "DIAGONAL";
}

double line_length(struct Line line) {
    double d; //giggity
    d = sqrt(powl((line.p2.x - line.p1.x), 2) + powl((line.p2.y - line.p1.y), 2));

    return d;
}

struct Point max_point(struct Point p1, struct Point p2, enum LineType line_type) {
    if (line_type == HORIZONTAL || line_type == DIAGONAL) {
        if (p1.x > p2.x) {
            return p1;
        }
    } else if (line_type == VERTICAL) {
        if (p1.y > p2.y) {
            return p1;
        }
    }
    return p2;
}

struct Point min_point(struct Point p1, struct Point p2, enum LineType line_type) {
    if (line_type == HORIZONTAL || line_type == DIAGONAL) {
        if (p1.x < p2.x) {
            return p1;
        }
    } else if (line_type == VERTICAL) {
        if (p1.y < p2.y) {
            return p1;
        }
    }
    return p2;
}

struct Line make_line(struct Point p1, struct Point p2) {
    struct Line line;
    line.p1 = p1;
    line.p2 = p2;
    line.type = line_type(line);
    line.length = line_length(line);
    line.run = line.p2.x - line.p1.x;
    line.rise = line.p2.y - line.p1.y;
    line.max_p = max_point(line.p1, line.p2, line.type);
    line.min_p = min_point(line.p1, line.p2, line.type);
    return line;
}