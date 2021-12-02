//
// Created by Jason Davidson on 12/1/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

const char *FILE_PREFIX = "/Users/jasondavidson/git/aoc2021/input/";
const int END_OF_BUFFER = -1;
const int MAX_BUFFER = INT16_MAX;

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

void print_array(int *array) {
    int size = array_size(array);
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
            return buffer;
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
    };
    lines[j] = malloc(1);
    lines[j] = 0;
    return j;
}

int read_string_file(char *filename, char *lines[], int buffer_size) {
    char file_name[strlen(FILE_PREFIX) + strlen(filename)];
    sprintf(file_name, "%s%s", FILE_PREFIX, filename);
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL) {
        printf("\nCould not find file %s", file_name);
        exit(1);
    }

    int line_count = l_read_lines(lines, fp, buffer_size, '\n');
    fclose(fp);
    return line_count;
}

int str_count(const char *string, const char *token) {
    char c;
    int i = 0, j = 0;
    while ((c = string[i]) != NULL) {
        if (c == *token) {
            j++;
        }
        i++;
    }
    return j;
}

int str_split(const char *string, const char *separator, char *splits[]) {
    int str_len = strlen(string);
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
    return i - 1;
}

void int2bin(int a, char *buffer, int buf_size) {
    buffer[buf_size] = '\0';
    buffer += (buf_size - 1);
    for (int i = buf_size - 1; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';
        a >>= 1;
    }
}

char *substring(char *string, int start) {
    int str_len = strlen(string);
    char *result = malloc(str_len - start + 1);

    printf("%s", *(string + start));

    strlcpy(result, *(string + start), str_len - start);
    return result;
}