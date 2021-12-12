#include <stdio.h>
#include <string.h>

#include "days.h"
#include "util.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("\nday and part required: day01 1");
        return 1;
    }

    char *day = argv[1];
    enum part p = daypart(argv[2]);
    int result = 0;

    printf("\nexecuting %s part %d ", day, p + 1);

    if (strcmp(day, "day01") == 0) {
        result = day01(p);
    } else if (strcmp(day, "day02") == 0) {
        result = day02(p);
    } else if (strcmp(day, "day03") == 0) {
        result = day03(p);
    } else if (strcmp(day, "day04") == 0) {
        result = day04(p);
    } else if (strcmp(day, "day05") == 0) {
        result = day05(p);
    } else if (strcmp(day, "day06") == 0) {
        result = day06(p);
    } else if (strcmp(day, "day07") == 0) {
        result = day07(p);
    } else if (strcmp(day, "day08") == 0) {
        result = day08(p);
    } else if (strcmp(day, "day09") == 0) {
        result = day09(p);
    } else if (strcmp(day, "day10") == 0) {
        result = day10(p);
    } else if (strcmp(day, "day11") == 0) {
        result = day11(p);
    } else if (strcmp(day, "day12") == 0) {
        result = day12(p);
    } else {
        printf("\ninvalid day: %s - expecting 'day01', 'day02', etc...", day);
    }
    printf("\nresult %d", result);
}