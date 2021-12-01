#include <stdio.h>
#include <string.h>

#include "day01.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\nday arg required");
        return 1;
    }

    char *day = argv[1];
    int result = 0;

    printf("\nexecuting %s ", day);

    if (strcmp(day, "day01") == 0) {
        result = day01();
    } else if (strcmp(day, "day01p2") == 0) {
        result = day01p2();
    } else {
        printf("\ninvalid day: %s - expecting 'day01', 'day02', etc...", day);
    }
    printf("\nresult %d", result);
}