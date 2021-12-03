//
// Created by Jason Davidson on 12/3/21.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "days.h"
#include "util.h"

struct bit_info {
    int position;
    int on_count;
    int off_count;
    int most_common;
};

static int int_list(int *num_list) {
    char *buffer[MAX_BUFFER];
    int buffer_len = read_string_file("day03.txt", buffer, MAX_BUFFER);
    for (int i = 0; i < buffer_len; i++) {
        num_list[i] = (int) strtol(buffer[i], NULL, 2);
    }
    return buffer_len;
}

static struct bit_info bit_calc(int *num_list, int buffer_len, int pos) {
    struct bit_info bi;
    bi.position = pos;
    bi.on_count = 0;
    bi.off_count = 0;
    bi.most_common = 0;
    for (int i = 0; i < buffer_len; i++) {
        if (bit_on(num_list[i], pos)) {
            bi.on_count++;
        } else {
            bi.off_count++;
        }
    }
    if (bi.on_count > buffer_len / 2) {
        bi.most_common = 1;
    }
    return bi;
}

static int p1(int bit_width) {
    int num_list[MAX_BUFFER];
    int buffer_len = int_list(num_list);

    int gamma = 0;
    int epsilon = 0;
    for (int pos = bit_width; pos > 0; pos--) {
        struct bit_info bi = bit_calc(num_list, buffer_len, pos);
        gamma = ((gamma << 1) | bi.most_common);
        epsilon = ((epsilon << 1) | !bi.most_common);
    }
    return gamma * epsilon;
}

static int match_and_reduce(int *matched_bit_criteria, int *buffer_len, struct bit_info bi) {
    int m = 0;
    if (*buffer_len == 1) {
        m = matched_bit_criteria[0];
    } else {
        for (int i = 0; i < *buffer_len; i++) {
            if (!bit_matches(matched_bit_criteria[i], bi.position, bi.most_common)) {
                matched_bit_criteria[i] = -1;
            }
        }
        *buffer_len = compact(matched_bit_criteria, *buffer_len);
        if (*buffer_len == 1) {
            m = matched_bit_criteria[0];
        }
    }
    return m;
}

static int p2(int bit_width) {
    int num_list[MAX_BUFFER];
    int buffer_len = int_list(num_list);
    int original_buffer = buffer_len;

    int o2 = 0;
    int co2 = 0;

    int matched_bit_criteria[buffer_len];
    for (int i = 0; i < buffer_len; i++) {
        matched_bit_criteria[i] = num_list[i];
    }

    /* o2 */
    for (int pos = bit_width; pos > 0; pos--) {
        struct bit_info bi = bit_calc(matched_bit_criteria, buffer_len, pos);
        if (bi.off_count == bi.on_count) {
            bi.most_common = 1;
        }
        o2 = match_and_reduce(matched_bit_criteria, &buffer_len, bi);
    }
    //co2
    for (int i = 0; i < original_buffer; i++) {
        matched_bit_criteria[i] = num_list[i];
    }
    buffer_len = original_buffer;
    for (int pos = bit_width; pos > 0; pos--) {
        struct bit_info bi = bit_calc(matched_bit_criteria, buffer_len, pos);
        bi.most_common = !bi.most_common;
        if (bi.off_count == bi.on_count) {
            bi.most_common = 0;
        }
        co2 = match_and_reduce(matched_bit_criteria, &buffer_len, bi);
    }

    printf("\n%d %d", o2, co2);
    return o2 * co2;
}

int day03(enum part p) {
    if (p == ONE) {
        return p1(12);
    } else {
        return p2(12);
    }
}
