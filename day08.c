//
// Created by Jason Davidson on 12/8/21.
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "days.h"

#define INPUT_FILENAME "day08.txt"

#define D0 "abcefg"
#define D1 "cf"
#define D2 "acdeg"
#define D3 "acdfg"
#define D4 "bcdf"
#define D5 "abdfg"
#define D6 "abdefg"
#define D7 "acf"
#define D8 "abcdefg"
#define D9 "abcdfg"

struct Segment {
    char *s;
    int segments;
    int unique;
    int length;
};

const struct Segment s0 = {.s = D0, .segments = 6, .unique = 0, .length = strlen(D0)};
const struct Segment s1 = {.s = D1, .segments = 2, .unique = 1, .length = strlen(D1)};
const struct Segment s2 = {.s = D2, .segments = 5, .unique = 0, .length = strlen(D2)};
const struct Segment s3 = {.s = D3, .segments = 5, .unique = 0, .length = strlen(D3)};
const struct Segment s4 = {.s = D4, .segments = 4, .unique = 1, .length = strlen(D4)};
const struct Segment s5 = {.s = D5, .segments = 5, .unique = 0, .length = strlen(D5)};
const struct Segment s6 = {.s = D6, .segments = 6, .unique = 0, .length = strlen(D6)};
const struct Segment s7 = {.s = D7, .segments = 3, .unique = 1, .length = strlen(D7)};
const struct Segment s8 = {.s = D8, .segments = 7, .unique = 1, .length = strlen(D8)};
const struct Segment s9 = {.s = D9, .segments = 6, .unique = 0, .length = strlen(D9)};

static int load_buffer(char **buffer) {
    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);
    return buffer_len;
}

static int signal_patterns(char *buffer, char **sig_patterns) {
    char *pipe_split[2];
    str_split(buffer, "|", pipe_split);

    int sig_len = str_split(pipe_split[0], " ", sig_patterns);
    for (int i = 0; i < sig_len; i++) {
        order_word(*(sig_patterns + i));
    }

    return sig_len;
}

static int output_patterns(char *buffer, char **output_patterns) {
    char *pipe_split[2];
    str_split(buffer, "|", pipe_split);

    int out_len = str_split(pipe_split[1], " ", output_patterns);

    for (int i = 0; i < out_len; i++) {
        order_word(*(output_patterns + i));
    }
    return out_len;
}

static int p1() {

    char *buffer[MAX_BUFFER];
    int buffer_len = load_buffer(buffer);

    int unique_count = 0;
    for (int i = 0; i < buffer_len; i++) {
        char *output_p[4];
        char *signal_p[10];

        int signal_p_len = signal_patterns(buffer[i], signal_p);
        int output_p_len = output_patterns(buffer[i], output_p);

        for (int j = 0; j < output_p_len; j++) {
            int l = strlen(output_p[j]);
            if (l == s1.length || l == s4.length || l == s7.length || l == s8.length) {
                unique_count++;
            }
        }
    }
    return unique_count;
}

static int p2() {
    return 1;
}

int day08(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}
