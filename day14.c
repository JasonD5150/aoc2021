//
// Created by Jason Davidson on 12/14/21.
//

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <printf.h>
#include <limits.h>
#include "util.h"

#define INPUT_FILENAME "day14.txt"
#define MAX_POLYMER_PAIRS 676

typedef struct PolymerPair {
    char a;
    char b;
    long occurrences;
} PolymerPair;

static PolymerPair **g_polymer_pairs;
static int g_polymer_pairs_len = 0;
static char *g_polymer_template;
static char **g_rules;
static int g_rules_len = 0;

// alphabetic char is the index where A = 0
static long long g_occurrences[26] = {0};

static int cto_i(char c) {
    return c - 65;
}

static char ito_c(int i) {
    return (char) (i + 65);
}

static void print(PolymerPair **buffer, int buffer_len) {
    for (int i = 0; i < buffer_len; i++) {
        PolymerPair *pp = *(buffer + i);
        printf("%c%c[%ld] ", pp->a, pp->b, pp->occurrences);
    }
    printf("\n");

}

static void print_occurrences() {
    printf("\nOccurrences:");
    for (int i = 0; i < 26; i++) {
        if (g_occurrences[i] != 0) {
            printf("%c=%lld ", ito_c(i), g_occurrences[i]);
        }
    }
    printf("\n");
}

static bool pair_eq(PolymerPair *p1, PolymerPair *p2) {
    if (p1->a == p2->a && p1->b == p2->b) {
        return true;
    }
    return false;
}

static int index_of_pair(PolymerPair **buffer, int buffer_len, PolymerPair *p) {
    for (int i = 0; i < buffer_len; i++) {
        PolymerPair *pp = *(buffer + i);
        if (pp->occurrences > 0 && pair_eq(pp, p)) {
            return i;
        }
    }
    return -1;
}

static int add_pair(PolymerPair **buffer, int buffer_len, PolymerPair *p) {

    int i = index_of_pair(buffer, buffer_len, p);

    if (i < 0) { //new
        buffer_len++;
        p->occurrences = 1;
        *(buffer + (buffer_len - 1)) = p;
    } else {
        PolymerPair *ep = *(buffer + i);
        ep->occurrences++;
    }
    return buffer_len;
}

static void process_rule(const char *rule,
                         PolymerPair **buffer, int buffer_len,
                         PolymerPair **new_buffer, int *new_buffer_len) {
    char a = rule[0];
    char b = rule[1];
    char insert = rule[6];

    PolymerPair *p = malloc(sizeof(PolymerPair *));
    p->a = a;
    p->b = b;
    int matched_index = index_of_pair(buffer, buffer_len, p);
    if (matched_index >= 0) {
        p = *(buffer + matched_index);

        g_occurrences[cto_i(insert)] += p->occurrences;

        PolymerPair *lp = malloc(sizeof(PolymerPair *));
        lp->a = a;
        lp->b = insert;
        lp->occurrences = 0;
        PolymerPair *rp = malloc(sizeof(PolymerPair *));
        rp->a = insert;
        rp->b = b;
        rp->occurrences = 0;

        int np_len = index_of_pair(new_buffer, *new_buffer_len, lp);
        if (np_len < 0) {
            *(new_buffer + *new_buffer_len) = lp;
            np_len = *new_buffer_len;
            *new_buffer_len = *new_buffer_len + 1;
        }
        lp = *(new_buffer + np_len);
        lp->occurrences += p->occurrences;

        np_len = index_of_pair(new_buffer, *new_buffer_len, rp);
        if (np_len < 0) {
            *(new_buffer + *new_buffer_len) = rp;
            np_len = *new_buffer_len;
            *new_buffer_len = *new_buffer_len + 1;
        }
        rp = *(new_buffer + np_len);
        rp->occurrences += p->occurrences;
    }
}

static void load() {

    for (int i = 0; i < 26; i++) {
        g_occurrences[i] = 0;
    }

    g_polymer_pairs = malloc(sizeof(PolymerPair *) * MAX_POLYMER_PAIRS);

    char *buffer[MAX_BUFFER];

    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);

    g_polymer_template = buffer[0];

    for (int i = 0; i < strlen(g_polymer_template) - 1; i++) {
        PolymerPair *pp = malloc(sizeof(PolymerPair *));
        pp->a = *(g_polymer_template + i);
        pp->b = *(g_polymer_template + i + 1);
        g_polymer_pairs_len = add_pair(g_polymer_pairs, g_polymer_pairs_len, pp);
    }

    for (int i = 0; i < strlen(g_polymer_template); i++) {
        g_occurrences[cto_i(g_polymer_template[i])]++;
    }

    g_rules = malloc(sizeof(char *) * (buffer_len - 2));
    //load the rules
    for (int i = 2, j = 0; i < buffer_len; i++, j++) {
        g_rules[j] = malloc(sizeof(char *));
        strcpy(g_rules[j], buffer[i]);
        g_rules_len++;
    }
}

static void flail(int steps) {
    for (int step = 1; step <= steps; step++) {
        PolymerPair **new_polymer_pairs = malloc(sizeof(PolymerPair *) * MAX_POLYMER_PAIRS);
        int new_polymer_pairs_len = 0;

        for (int i = 0; i < g_rules_len; i++) {
            process_rule(g_rules[i], g_polymer_pairs, g_polymer_pairs_len,
                         new_polymer_pairs, &new_polymer_pairs_len);
        }
        g_polymer_pairs_len = new_polymer_pairs_len;
        free(g_polymer_pairs);
        g_polymer_pairs = new_polymer_pairs;

        printf("\nAfter step %d: ", step);
        print_occurrences();
        printf("\n");
    }

    long long max = 0, min = LONG_MAX;
    for (int i = 0; i < 26; i++) {
        if (g_occurrences[i] > max) {
            max = g_occurrences[i];
        }
        if (g_occurrences[i] < min && g_occurrences[i] > 0) {
            min = g_occurrences[i];
        }
    }

    printf("\nResult: %lld\n", max - min);

}

static int p1() {
    load();
    print(g_polymer_pairs, g_polymer_pairs_len);
    print_occurrences();
    flail(10);
    return 1;
}

static int p2() {
    load();
    print(g_polymer_pairs, g_polymer_pairs_len);
    print_occurrences();
    flail(40);
    return 1;
}

int day14(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}
