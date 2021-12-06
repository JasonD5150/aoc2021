//
// Created by Jason Davidson on 12/5/21.
//

#ifndef AOC2021_GEOMETRICS_H
#define AOC2021_GEOMETRICS_H

#define MAX_POINT_HASH 10000000

enum LineType {
    HORIZONTAL, VERTICAL, DIAGONAL
};

struct Point {
    int x;
    int y;
};

struct Line {
    enum LineType type;
    struct Point p1;
    struct Point p2;
    double length;
    int rise;
    int run;
    struct Point max_p;
    struct Point min_p;
};

int point_hash(struct Point p);
struct Point point_from_hash(int hash);
struct Point make_point(int x, int y);
int eq_points(struct Point p1, struct Point p2);
enum LineType line_type(struct Line line);
double line_length(struct Line line);
struct Line make_line(struct Point p1, struct Point p2);
char *line_type_string(enum LineType line_type);
struct Point max_point(struct Point p1, struct Point p2, enum LineType line_type);
struct Point min_point(struct Point p1, struct Point p2, enum LineType line_type);

#endif //AOC2021_GEOMETRICS_H
