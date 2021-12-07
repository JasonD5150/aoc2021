//
// Created by Jason Davidson on 12/6/21.
//


#include <stdlib.h>
#include <printf.h>
#include "days.h"

#define INPUT_FILENAME "day06.txt"

static int load_fish(int *int_buffer) {
    char *buffer[MAX_BUFFER];
    read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);
    int int_buffer_len = to_int_array(*buffer, int_buffer, ",");
    return int_buffer_len;
}

static long sum_fish(int *buffer, int buffer_len, int days) {
    int tot_days = 9; /* 0 - 8 inclusive == 9 */
    long *fish = malloc((long) sizeof(long) * tot_days);

    for (int i = 0; i < buffer_len; i++) {
        /* initialize each fish pointer as the sum days until it creates a new fish */
        printf("\nfish[%d] = fish[%d] + 1", *(buffer + i), *(buffer + i));
        *(fish + *(buffer + i)) = *(fish + *(buffer + i)) + 1;
    }

    for (int i = 0; i < days; i++) {
        int spawn_mod_9_cursor = (i + 7) % 9;
        int day_mod_9_cursor = i % 9;
        printf("\nday %d -> fish[%d](%ld) += fish[%d](%ld)",
               i, spawn_mod_9_cursor, *(fish + spawn_mod_9_cursor), day_mod_9_cursor, *(fish + day_mod_9_cursor));
        *(fish + spawn_mod_9_cursor) = *(fish + spawn_mod_9_cursor) + *(fish + day_mod_9_cursor);
    }
    long sum = 0;
    for (int i = 0; i < tot_days; i++) {
        sum = sum + *(fish + i);
    }
    return sum;

}

static int p1() {

    int *buffer = malloc((int) sizeof(int) * UINT32_MAX);
    int buffer_len = load_fish(buffer);

    return (int) sum_fish(buffer, buffer_len, 80);
}


static long p2() {

    int *buffer = malloc((int) sizeof(int) * UINT32_MAX);
    int buffer_len = load_fish(buffer);

    return sum_fish(buffer, buffer_len, 256);
}

int day06(enum part p) {
    if (p == ONE) {
        return p1();
    } else {
        long l = p2();
        printf("\n%ld", l);
        return 1;
    }
}
