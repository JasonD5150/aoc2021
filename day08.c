//
// Created by Jason Davidson on 12/8/21.
//

#include <string.h>
#include <math.h>
#include <assert.h>
#include "days.h"

#define INPUT_FILENAME "day08.txt"

struct Segment {
    char s[8];
    int length;
};

static struct Segment segment_digit_lookup[10];

static int load_buffer(char **buffer) {
    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);
    return buffer_len;
}

static int split_patterns(char *buffer, char **patterns, enum part p) {
    char *pipe_split[2];
    str_split(buffer, "|", pipe_split);

    int split = 0;
    if (p == TWO) {
        split = 1;
    }
    int p_len = str_split(*(pipe_split + split), " ", patterns);
    for (int i = 0; i < p_len; i++) {
        order_word(*(patterns + i));
    }
    return p_len;
}

static int count_matches(struct Segment s, const char *pattern, int pattern_len) {
    int c = 0;
    for (int i = 0; i < pattern_len; i++) {
        for (int j = 0; j < s.length; j++) {
            if (*(pattern + i) == *(s.s + j)) {
                c++;
            }
        }
    }
    return c;
}

static int contains_digit_overlap(const char *pattern, int pattern_len, int digit) {
    struct Segment s = *(segment_digit_lookup + digit);
    if (count_matches(s, pattern, pattern_len) == s.length) {
        return 1;
    }
    return 0;
}

static int intersects_4(const char *pattern, int pattern_len) {
    struct Segment s = segment_digit_lookup[4];
    if (count_matches(s, pattern, pattern_len) == 3) {
        return 1;
    }
    return 0;
}

static void build_signal_to_lookup(char **patterns, int pattern_len) {

    /* build known lookup */
    for (int i = 0; i < pattern_len; i++) {
        int length = (int) strlen(*(patterns + i));
        char *p = *(patterns + i);
        struct Segment s;
        s.length = length;
        strlcpy(s.s, p, length + 1);
        switch (length) {
            case 2:
                *(segment_digit_lookup + 1) = s;
                break;
            case 3:
                *(segment_digit_lookup + 7) = s;
                break;
            case 4:
                *(segment_digit_lookup + 4) = s;
                break;
            case 7:
                *(segment_digit_lookup + 8) = s;
                break;
            default:
                break;
        }
    }
    for (int i = 0; i < pattern_len; i++) {
        char *p = *(patterns + i);
        int length = (int) strlen(p);
        struct Segment s;
        switch (length) {
            case 5:
                // Can be 2,3,5
                if (contains_digit_overlap(p, length, 7)) {
                    s.length = length;
                    strlcpy(s.s, p, length + 1);
                    *(segment_digit_lookup + 3) = s;
                } else {
                    if (intersects_4(p, length)) {
                        s.length = length;
                        strlcpy(s.s, p, length + 1);
                        *(segment_digit_lookup + 5) = s;
                    } else {
                        s.length = length;
                        strlcpy(s.s, p, length + 1);
                        *(segment_digit_lookup + 2) = s;
                    }
                }
                break;
            case 6:
                // Can be 0,6,9
                if (contains_digit_overlap(p, length, 4)) {
                    s.length = length;
                    strlcpy(s.s, p, length + 1);
                    *(segment_digit_lookup + 9) = s;
                } else {
                    if (contains_digit_overlap(p, length, 7)) {
                        s.length = length;
                        strlcpy(s.s, p, length + 1);
                        *segment_digit_lookup = s;
                    } else {
                        s.length = length;
                        strlcpy(s.s, p, length + 1);
                        *(segment_digit_lookup + 6) = s;
                    }
                }
                break;
            default:
                break;
        }
    }
}

static int decode_output(char *output) {
    for (int i = 0; i < 10; i++) {
        if (strcmp(segment_digit_lookup[i].s, output) == 0) {
            return i;
        }
    }
    return -1;
}

static int sub_display(enum part p) {
    char *buffer[MAX_BUFFER];
    int buffer_len = load_buffer(buffer);

    int result = 0;
    for (int i = 0; i < buffer_len; i++) {
        char *output_p[4];
        char *signal_p[10];

        int signal_p_len = split_patterns(*(buffer + i), signal_p, ONE);
        int output_p_len = split_patterns(*(buffer + i), output_p, TWO);

        build_signal_to_lookup(signal_p, signal_p_len);

        // now decode the output
        int o_val = 0;
        for (int j = 0, k = 3; j < output_p_len; j++, k--) {
            int decoded_digit = decode_output(*(output_p + j));
            assert(decoded_digit >= 0);

            if (p == ONE) {
                if (decoded_digit == 1 || decoded_digit == 4 || decoded_digit == 7 || decoded_digit == 8) {
                    result++;
                }
            } else {
                decoded_digit = decoded_digit * (int) pow(10, k);
                o_val = o_val + decoded_digit;
            }
        }
        result = result + o_val;
    }
    return result;

}

static int p1() {
    return sub_display(ONE);
}

static int p2() {
    return sub_display(TWO);
}

int day08(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}
