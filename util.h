//
// Created by Jason Davidson on 12/1/21.
//

#ifndef AOC2021_UTIL_H
#define AOC2021_UTIL_H
extern const char *FILE_PREFIX;

extern const int END_OF_BUFFER;

extern const int MAX_BUFFER;

int int_compare(const void *a, const void *b);

void sort_int_array(int *array);

int array_size(const int *array);

int string_array_size(char *array[]);

int *read_int_file(char *filename);

int read_string_file(char *filename, char *lines[], int buffer_size);

void print_array(int *array);

void print_string_array(char *array[]);

void zero_array(int *array, int size);

int is_zero_array(const int *array, int size);

int max(int a, int b);

int str_count(const char *string, const char *token);

int str_split(const char *string, const char *separator, char *splits[]);

void int2bin(int a, char *buffer, int buf_size);

char *substring(char *string, int start);

#endif //AOC2021_UTIL_H
