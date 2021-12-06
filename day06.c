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
    int int_buffer_len = to_int_array(buffer[0], int_buffer, ",");
    return int_buffer_len;
}

static int to_char_array(char *string, char *array, char *separator) {
    char *splits[MAX_BUFFER];
    int split_count = str_split(string, separator, splits);
    int c = 0;
    for (int i = 0; i < split_count; i++) {
        array[i] = *splits[i];
        c = i;
    }
    return c + 1;
}

static long load_fish_char(char *char_buffer) {
    char *buffer[MAX_BUFFER];
    read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);
    long l = to_char_array(buffer[0], char_buffer, ",");
    return l;
}

static int day_calc(int fish, int day, int *spawn) {
    int r = fish - day;
    if(r < 0) {
        r = r + 7;
    }
}

static int p1() {

    char *buffer = malloc((char) sizeof(char) * UINT32_MAX);
    long buffer_len = load_fish_char(buffer);

    int day = 1;
    while (day <= 80) {
        int append_count = 0;
        for (int i = 0; i < buffer_len; i++) {
            if (*(buffer + i) == '0') {
                *(buffer + i) = '6';
                append_count++;
            } else {
                *(buffer + i) = (char) (*(buffer + i) - 1);
            }
        }
        for (long i = buffer_len; i < buffer_len + append_count; i++) {
            *(buffer + i) = '8';
        }
        buffer_len = buffer_len + append_count;
        printf("\nday[%3d] size[%3ld]", day, buffer_len);
        //print_array_size(buffer, buffer_len);
        day++;
    }
    //print_array_size(buffer, buffer_len);
    return buffer_len;
}

static int recur(int fish) {
    if(fish == 0) {
        fish = 6;
        recur(8);
    } else {
        fish--;
    }
}
static long p1_big() {

    short *buffer = malloc((short) sizeof(short) * UINT32_MAX);
    long buffer_len = load_fish(buffer);

    int fish = 0;
    int day = 0;
    int spawn = 0;
    while (day < 256) {
        recur(fish);
        if(fish == 0) {
            fish = 6;
            spawn++;
        } else {
            fish--;
        }
        day++;
    }

    printf("\nfish spawned %d over %d days", spawn, day);

    return 1;
}

static long p2() {

    int *buffer = malloc((int) sizeof(int) * UINT32_MAX);
    int buffer_len = load_fish(buffer);
    int counts[30];
    /* have to do a chunk at a time - let's do
       30 chunks of 20 */

    int chunk_size = 1;
    int chunk_start = 0;
    int chunk_end = chunk_size;
    int c = 0;
    printf("\n%d", buffer_len);
    while (chunk_start < buffer_len) {
        int day = 1;
        short *working_buffer = malloc((short) sizeof(short) * UINT32_MAX);
        for (int i = chunk_start, j = 0; i < chunk_end; i++, j++) {
            *(working_buffer + j) = (short) *(buffer + i);
        }
        long working_buffer_len = (chunk_end - chunk_start);
        printf("\nchunk_start[%d] chunk_end[%d] working_buffer_len[%d]\n", chunk_start, chunk_end, working_buffer_len);

        while (day <= 18) {
            long append_count = 0;
            for (long i = 0; i < working_buffer_len; i++) {
                if (*(working_buffer + i) == 0) {
                    *(working_buffer + i) = 6;
                    append_count++;
                } else {
                    *(working_buffer + i) = (short) (*(working_buffer + i) - 1);
                }
            }
            for (long i = working_buffer_len; i < working_buffer_len + append_count; i++) {
                *(working_buffer + i) = 8;
            }
            working_buffer_len = working_buffer_len + append_count;
            print_array(working_buffer);
            day++;
        }

        printf("\nday[%d] working_buffer_len[%d]", day, working_buffer_len);

        counts[c++] = working_buffer_len;
        chunk_start = chunk_start + chunk_size;
        chunk_end = chunk_end + chunk_size;
        free(working_buffer);
    }
    long result = 0;
    for (int i = 0; i < c; i++) {
        result = result + counts[i];
    }
    return result;
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
