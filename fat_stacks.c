//
// Created by Jason Davidson on 12/10/21.
//

#include <printf.h>
#include <stdlib.h>
#include "fat_stacks.h"

void push(CharNode **top_ref, char data) {
    // allocate node
    CharNode *new_node = (CharNode *) malloc(sizeof(CharNode));

    if (new_node == NULL) {
        printf("Stack overflow n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = (*top_ref);
    (*top_ref) = new_node;
}

char pop(CharNode **top_ref) {
    char data;
    CharNode *top;

    // If stack is empty then error
    if (*top_ref == NULL) {
        printf("Stack overflow n");
        exit(0);
    } else {
        top = *top_ref;
        data = top->data;
        *top_ref = top->next;
        free(top);
        return data;
    }
}

