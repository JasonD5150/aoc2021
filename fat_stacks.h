//
// Created by Jason Davidson on 12/10/21.
//

#ifndef AOC2021_FAT_STACKS_H
#define AOC2021_FAT_STACKS_H

typedef struct CharNode {
    char data;
    struct CharNode *next;
} CharNode;

void push(CharNode ** top, char data);

char pop(CharNode** top_ref);

#endif //AOC2021_FAT_STACKS_H
