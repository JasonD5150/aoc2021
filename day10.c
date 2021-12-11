//
// Created by Jason Davidson on 12/10/21.
//

#include <string.h>
#include <printf.h>
#include "util.h"
#include "fat_stacks.h"

#define INPUT_FILENAME "day10.txt"

enum BalanceStatus {
    BALANCED, INCOMPLETE, CORRUPT
};

typedef struct BalanceResult {
    char found;
    char expected;
    enum BalanceStatus balance_status;
    CharNode *stack;
} BalanceResult;

static char inverse_of_char(char c) {
    if (c == '[') return ']';
    if (c == '{') return '}';
    if (c == '<') return '>';
    if (c == '(') return ')';
    return '\0';
}

static int char_score(char c) {
    if (c == ')') return 3;
    if (c == ']') return 57;
    if (c == '}') return 1197;
    if (c == '>') return 25137;
    return 0;
}

static int completion_score(char c) {
    if (c == ')') return 1;
    if (c == ']') return 2;
    if (c == '}') return 3;
    if (c == '>') return 4;
    return 0;
}

static int is_starting_char(char c) {
    if (c == '[' || c == '{' || c == '<' || c == '(') return 1;
    return 0;
}

static int is_ending_char(char c) {
    if (c == ']' || c == '}' || c == '>' || c == ')') return 1;
    return 0;
}

static int stack_to_string(CharNode *c, char *stack_string) {
    if (c==NULL) {
        stack_string[0] = '\0';
        return 0;
    }
    int d = 0;
    stack_string[d] = c->data;
    d++;

    CharNode *nn = c->next;
    while (nn != NULL) {
        stack_string[d] = nn->data;
        nn = nn->next;
        d++;
    }
    stack_string[d] = '\0';
    return d;
}

static int inverse_stack_to_string(CharNode *c, char *stack_string) {
    if (c==NULL) {
        stack_string[0] = '\0';
        return 0;
    }
    int d = 0;
    stack_string[d] = inverse_of_char(c->data);
    d++;

    CharNode *nn = c->next;
    while (nn != NULL) {
        stack_string[d] = inverse_of_char(nn->data);
        nn = nn->next;
        d++;
    }
    stack_string[d] = '\0';
    return d;
}

BalanceResult check_balance(char *expression) {
    int i = 0;

    // Declare an empty character stack
    CharNode *stack = NULL;

    char c;
    BalanceResult br;

    while ((c = *(expression + i)) != '\0') {
        if (is_starting_char(c) == 1) {
            push(&stack, c);
        }
        if (is_ending_char(c) == 1) {

            char p = pop(&stack);
            if (stack != NULL && c != inverse_of_char(p)) {
                br.found = c;
                br.expected = inverse_of_char(p);
                br.stack = stack;
                br.balance_status = CORRUPT;
                return br;
            }
        }
        i++;
    }

    if (stack == NULL) {
        br.balance_status = BALANCED;
    } else {
        br.stack = stack;
        br.balance_status = INCOMPLETE;
    }
    return br;
}

static int p1() {
    char *buffer[MAX_BUFFER];
    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);
    char illegal_count[127];

    illegal_count[']'] = 0;
    illegal_count['}'] = 0;
    illegal_count['>'] = 0;
    illegal_count[')'] = 0;

    for (int i = 0; i < buffer_len; i++) {
        char *b = *(buffer + i);
        BalanceResult br = check_balance(b);
        if (br.balance_status == CORRUPT) {
            illegal_count[br.found]++;
            printf("\n%s CORRUPT expected:%c found:%c", b, br.expected, br.found );
        }
    }
    int result = illegal_count[']'] * char_score(']') +
                 illegal_count['}'] * char_score('}') +
                 illegal_count['>'] * char_score('>') +
                 illegal_count[')'] * char_score(')');
    return result;
}

static int p2() {
    char *buffer[MAX_BUFFER];
    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);

    int c = 0;
    long scores[MAX_BUFFER];
    for (int i = 0; i < buffer_len; i++) {
        char *b = *(buffer + i);
        BalanceResult br = check_balance(b);
        if (br.balance_status == INCOMPLETE) {
            long total_score = 0;
            char stack_s[strlen(b)];
            int is_l = inverse_stack_to_string(br.stack, stack_s);

            for (int j = 0; j < is_l; j++) {
                total_score = total_score * 5 + (long)completion_score(*(stack_s + j));
            }
            scores[c++] = total_score;
        }
    }
    sort_long_array_size(scores, c);
    printf("\n%ld", scores[(c/2)]);
    return 1;
}

int day10(enum part p) {
    if (p == ONE) {
        return p1();
    }
    return p2();
}