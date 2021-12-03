#include "util.h"
#include "days.h"

int window_sum(const int *buffer, int buffer_len, int pos, int size) {
    int w = 0;
    for (int k = pos; k < pos + size && k < buffer_len; k++) {
        w = w + *(buffer + k);
    }
    return w;
}

static int p1() {
    int buffer[MAX_BUFFER];
    int buffer_len = read_int_file("day01.txt", buffer);
    int increase = 0;
    for (int i = 0, j = 1; j < buffer_len; j++, i++) {
        if (*(buffer + j) > *(buffer + i)) {
            increase++;
        }
    }
    return increase;
}

static int p2() {
    int buffer[MAX_BUFFER];
    int buffer_len = read_int_file("day01.txt", buffer);
    int increase = 0;
    for (int i = 0, j = 1; j < buffer_len; j++, i++) {
        int window_1_sum = window_sum(buffer, buffer_len, i, 3);
        int window_2_sum = window_sum(buffer, buffer_len, j, 3);

        if (window_2_sum > window_1_sum) {
            increase++;
        }
    }
    return increase;
}

int day01(enum part p) {
    if (p == ONE) {
        return p1();
    } else {
        return p2();
    }
}