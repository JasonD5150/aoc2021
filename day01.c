//
// Created by Jason Davidson on 12/1/21.
//

#include "util.h"
#include "day01.h"

int day01p2() {

    /* 3 buffer sliding window */

    int *buffer = read_int_file("day01.txt");
    int buffer_len = array_size(buffer);
    int increase = 0;
    for (int i = 0, j = 1; j < buffer_len; j++, i++) {
        int window_1_sum = 0;
        for (int k = i; k < i + 3 && k < buffer_len; k++) {
            window_1_sum = window_1_sum + *(buffer + k);
        }
        int window_2_sum = 0;
        for (int k = j; k < j + 3 && k < buffer_len; k++) {
            window_2_sum = window_2_sum + *(buffer + k);
        }

        if (window_2_sum > window_1_sum) {
            increase++;
        }
    }
    return increase;
}

int day01() {
    int *buffer = read_int_file("day01.txt");
    int buffer_len = array_size(buffer);
    int increase = 0;
    for (int i = 0, j = 1; j < buffer_len; j++, i++) {
        if (*(buffer + j) > *(buffer + i)) {
            increase++;
        }
    }
    return increase;
}