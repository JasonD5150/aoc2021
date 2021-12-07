//
// Created by Jason Davidson on 12/1/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "util.h"

const char *FILE_PREFIX = "/Users/jasondavidson/git/aoc2021/input/";
const int END_OF_BUFFER = -1;
const int MAX_BUFFER = INT16_MAX;

enum part daypart(char *arg) {
    int p = (int) strtol(arg, (char **) NULL, 10);
    if (p == 1) {
        return ONE;
    } else return TWO;
}

void l_print_array(const int *array, int size) {
    printf("\nArray Size: %d", size);
    printf("\n[ ");
    for (int i = 0; i < size; i++) {
        printf("%d", *(array + i));
        if (i == size - 1) {
            printf(" ");
        } else {
            printf(", ");
        }
    }
    printf("]\n");
}

void l_print_string_array(char *array[], int size) {
    printf("\nArray Size: %d", size);
    printf("\n[ ");
    for (int i = 0; i < size; i++) {
        printf("%s", array[i]);
        if (i == size - 1) {
            printf(" ");
        } else {
            printf(", ");
        }
    }
    printf("]\n");
}

void strip_extra_spaces(char *str) {
    int i, x;
    for (i = x = 0; str[i]; ++i)
        if (!isspace(str[i]) || (i > 0 && !isspace(str[i - 1])))
            str[x++] = str[i];
    str[x] = '\0';
}

int is_zero_array(const int *array, int size) {
    for (int i = 0; i < size; i++) {
        if (array[i]) return 0;
    }
    return 1;
}

void zero_array(int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = 0;
    }
}

void empty_array(int *array, int size) {
    for (int i = 0; i < size; i++) {
        array[i] = END_OF_BUFFER;
    }
    array[size] = END_OF_BUFFER;
}

void print_array(int *array) {
    int size = array_size(array);
    l_print_array(array, size);
}

void print_array_size(int *array, int size) {
    l_print_array(array, size);
}

void print_string_array(char *array[]) {
    int size = string_array_size(array);
    l_print_string_array(array, size);
}

int max(int a, int b) {
    if (a > b) return a;
    return b;
}

int int_compare(const void *a, const void *b) {
    return (*(int *) a - *(int *) b);
}

void sort_int_array(int *array) {
    qsort(array, array_size(array), sizeof(int), int_compare);
}

void sort_int_array_size(int *array, int size) {
    qsort(array, size, sizeof(int), int_compare);
}


int array_size(const int *array) {
    int i = 0;
    while (*(array + i++) != END_OF_BUFFER);
    int size = i - 1;
    return size;
}

int string_array_size(char *array[]) {
    int i = 0;
    do {
        if (array[i++] == NULL) break;
    } while (1);
    int size = i - 1;
    return size;
}

int read_int_file(char *filename, int *buffer) {

    char fn[strlen(FILE_PREFIX) + strlen(filename)];
    sprintf(fn, "%s%s", FILE_PREFIX, filename);
    FILE *fp = fopen(fn, "r");
    int input_number = 0;
    int i = 0;

    while (fscanf(fp, "%d", &input_number) == 1) {
        if (i >= MAX_BUFFER) {
            fclose(fp);
            buffer[MAX_BUFFER] = END_OF_BUFFER;
            return MAX_BUFFER;
        }
        buffer[i++] = input_number;
    }
    buffer[i] = END_OF_BUFFER;

    fclose(fp);

    return i;
}

int l_read_lines(char *lines[], FILE *fp, int max, char terminator) {
    char buffer[max];
    int i = 0, j = 0, done = 0;

    while (!done) {
        char c = (char) fgetc(fp);
        if (feof(fp)) {
            done = 1;
        }
        if (c != terminator && i < max && !done) {
            buffer[i++] = c;
        } else {
            int str_len = (int) strlen(buffer) + 1;
            lines[j] = malloc(str_len);
            strlcpy(lines[j], buffer, str_len);
            j++;
            i = 0;
            for (int k = 0; k < max; k++) {
                buffer[k] = 0;
            }
        }
    }
    lines[j] = malloc(1);
    lines[j] = 0;
    return j;
}

int read_string_file(char *filename, char *buffer[], int buffer_size) {
    char file_name[strlen(FILE_PREFIX) + strlen(filename)];
    sprintf(file_name, "%s%s", FILE_PREFIX, filename);
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("\nCould not find file %s", file_name);
        exit(1);
    }

    int line_count = l_read_lines(buffer, fp, buffer_size, '\n');
    fclose(fp);
    return line_count;
}

int str_count(const char *string, const char *token) {
    char c;
    int i = 0, j = 0;
    while ((c = string[i]) != '\0') {
        if (c == *token) {
            j++;
        }
        i++;
    }
    return j;
}

int str_split(const char *string, const char *separator, char *splits[]) {
    strip_extra_spaces(string);
    int str_len = (int) strlen(string);
    char *copy = malloc(str_len + 1);
    strlcpy(copy, string, str_len + 1);
    char *token = strtok(copy, separator);
    int i = 0;
    do {
        int token_length = (int) strlen(token);
        splits[i] = malloc(token_length + 1);
        strlcpy(splits[i], token, token_length + 1);
        i++;
        token = strtok(NULL, separator);
    } while (token != NULL);
    splits[i] = malloc(1);
    splits[i] = 0;
    return i;
}

void int2bin(int a, char *buffer, int buf_size) {
    buffer[buf_size] = '\0';
    buffer += (buf_size - 1);
    for (int i = buf_size - 1; i >= 0; i--) {
        *buffer-- = (char) ((a & 1) + '0');
        a >>= 1;
    }
}

char *substring(char *string, int start) {
    int str_len = (int) strlen(string);
    char *result = malloc(str_len - start + 1);

    printf("%s", (string + start));

    strlcpy(result, (string + start), str_len - start);
    return result;
}

int bit_on(int num, int pos) {
    return (num >> (pos - 1)) & 1;
}

int bit_matches(int num, int pos, int bit) {
    int shift = pos - 1;
    int n = (num >> shift) & 1;

    if (n == bit) {
        return 1;
    }
    return 0;
}

int elements(int *num_list, int buffer_len) {
    int c = 0;
    for (int i = 0; i < buffer_len; i++) {
        if (num_list[i] != -1) {
            c++;
        }
    }
    return c;
}

int compact(int *num_list, int buffer_len) {
    int c = elements(num_list, buffer_len);
    int new_list[c];
    c = 0;
    for (int i = 0; i < buffer_len; i++) {
        if (num_list[i] != -1) {
            new_list[c++] = num_list[i];
        }
    }
    for (int i = 0; i < buffer_len; i++) {
        num_list[i] = -1;
    }
    for (int i = 0; i < c; i++) {
        num_list[i] = new_list[i];
    }
    return c;
}

int one_element_remains(int *num_list, int buffer_len) {
    int c = 0;
    for (int i = 0; i < buffer_len; i++) {
        if (num_list[i] != -1) {
            c++;
        }
    }
    return c;
}

int to_short_array(char *string, short *array, char *separator) {
    char *splits[MAX_BUFFER];
    int split_count = str_split(string, separator, splits);
    int c = 0;
    for (int i = 0; i < split_count; i++) {
        array[i] = (short) strtol(splits[i], (char **) NULL, 10);
        c = i;
    }
    return c + 1;
}


int to_int_array(char *string, int *array, char *separator) {
    char *splits[MAX_BUFFER];
    int split_count = str_split(string, separator, splits);
    int c = 0;
    for (int i = 0; i < split_count; i++) {
        array[i] = (int) strtol(splits[i], (char **) NULL, 10);
        c = i;
    }
    return c + 1;
}

void copy_int_array(int *dest, int *src, int len) {
    for (int i = 0; i < len; i++) {
        dest[i] = src[i];
    }
}

int num_digits(int i) {
    if (i == 0) {
        return 0;
    }
    return (int)floor(log10(abs(i))) + 1;
}

int is_odd(int i) {
    if (i % 2 == 0) return 0;
    return 1;
}

int mid_element_mean(const int *sorted_array, int array_size) {

    if (is_odd(array_size) == 1) {
        double mid = ceil((double)array_size / 2);
        return *(sorted_array + (int) mid);
    }
    int mid_l = (array_size / 2) - 1;
    int mid_r = (array_size / 2);
    int l = *(sorted_array + mid_l);
    int r = *(sorted_array + mid_r);
    double avg = round((double)(l + r) / 2);
    return (int) avg;
}

double average_int_s(const int *array, int array_size) {
    long sum = 0;
    for (int i = 0; i < array_size; i++) {
        sum = sum + *(array + i);
    }
    return (double) sum / array_size;
}

