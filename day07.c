//
// Created by Jason Davidson on 12/7/21.
//

#include <stdlib.h>
#include <printf.h>
#include <math.h>
#include "days.h"

#define INPUT_FILENAME "day07.txt"

static int load_crabs(int *int_buffer) {
    char *buffer[MAX_BUFFER];
    read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);
    int int_buffer_len = to_int_array(*buffer, int_buffer, ",");
    return int_buffer_len;
}

static int p1() {
    int *buffer = malloc((int) sizeof(int) * MAX_BUFFER);
    int buffer_len = load_crabs(buffer);

    sort_int_array_size(buffer, buffer_len);

    int median = mid_element_mean(buffer, buffer_len);

    int fuel_sum = 0;
    for (int i = 0; i < buffer_len; i++) {
        fuel_sum = fuel_sum + abs(*(buffer + i) - median);
    }
    return fuel_sum;
}

int distance(int n) {
    return n * (n + 1) / 2;
}
static long p2() {
    int *buffer = malloc((int) sizeof(int) * MAX_BUFFER);
    int buffer_len = load_crabs(buffer);

    double mean = average_int_s(buffer, buffer_len);
    int upper_mean = ceil(mean);
    int lower_mean = floor(mean);

    long upper_fuel = 0;
    long lower_fuel = 0;

    for (int i = 0; i < buffer_len; i++) {
        upper_fuel += distance(abs(upper_mean - *(buffer + i)));
        lower_fuel += distance(abs(lower_mean - *(buffer + i)));
    }
    return upper_fuel < lower_mean ? upper_fuel : lower_fuel;

}

int day07(enum part p) {
    if (p == ONE) {
        return p1();
    }
    long l = p2();
    printf("\n%ld", l);
    return l;
}