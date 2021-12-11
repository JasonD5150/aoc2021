//
// Created by Jason Davidson on 12/4/21.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

#define BOARD_SIZE 5
#define INPUT_FILENAME "day04.txt"

enum BingoDirection {
    ACROSS, DOWN, DIAGONAL_DOWN, DIAGONAL_UP
};
struct Bingo {
    int board[BOARD_SIZE][BOARD_SIZE];
    int playing_board[BOARD_SIZE][BOARD_SIZE];
    int winning_row_sum;
    int bingo[BOARD_SIZE];
    int unmarked[BOARD_SIZE * BOARD_SIZE];
    int unmarked_size;
    int unmarked_sum;
    int has_bingo;
    int last_number_called;
    int last_number_called_point;
    enum BingoDirection bingo_direction;
};

static void print_board(int board[BOARD_SIZE][BOARD_SIZE], char *name) {
    printf("\n*** %s ***\n", name);
    for (int j = 0; j < BOARD_SIZE; j++) {
        for (int k = 0; k < BOARD_SIZE; k++) {
            printf("%d ", board[j][k]);
        }
        printf("\n");
    }
}

static char *bingo_direction(enum BingoDirection d) {
    if (d == ACROSS) {
        return "ACROSS";
    } else if (d==DOWN) {
        return "DOWN";
    } else if (d==DIAGONAL_DOWN) {
        return "DIAGONAL_DOWN";
    } else if (d == DIAGONAL_UP) {
        return "DIAGONAL_UP";
    }
    return "LOSER";
}
static void print_bingo_board(struct Bingo bingo) {
    print_board(bingo.board, "board");
    print_board(bingo.playing_board, "playing board");
    printf("\nunmarked:");
    print_array_size(bingo.unmarked, bingo.unmarked_size);
    printf("\nbingo hit_map:");
    print_array_size(bingo.bingo, BOARD_SIZE);
    printf("\nunmarked sum: %d", bingo.unmarked_sum);
    printf("\nhas bingo %d", bingo.has_bingo);
    printf("\nwin direction: %s", bingo_direction(bingo.bingo_direction));
    printf("\nlast number called point:%d", bingo.last_number_called_point);
    printf("\nlast number called:%d winning row sum:%d", bingo.last_number_called,
           bingo.winning_row_sum);
}

static int load_boards(char *buffer[MAX_BUFFER], int buffer_len, struct Bingo *boards) {
    int size = 0;

    int board_row = 0;
    for (int i = 2; i < buffer_len; i++) {
        if (strlen(buffer[i]) != 0) {
            /* into the board material now - read until first char in buffer is EOL/NULL */
            to_int_array(buffer[i], boards[size].board[board_row], " ");
            empty_array(boards[size].playing_board[board_row], BOARD_SIZE);
            boards[size].last_number_called = -1;
            boards[size].winning_row_sum = -1;
            board_row++;
        } else {
            size++;
            board_row = 0;
        }
    }
    return size + 1;
}


static struct Bingo *load(int called_numbers[], int *boards_size, int *called_size) {
    char *buffer[MAX_BUFFER];
    int buffer_len = read_string_file(INPUT_FILENAME, buffer, MAX_BUFFER);

    *called_size = to_int_array(buffer[0], called_numbers, ",");
    struct Bingo *boards = malloc(sizeof(struct Bingo) * (buffer_len - 2) / 5);
    *boards_size = load_boards(buffer, buffer_len, boards);
    return boards;
}

static int build_unmarked_numbers(struct Bingo *board) {
    int c = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board->playing_board[i][j] == END_OF_BUFFER) {
                board->unmarked[c++] = board->board[i][j];
            }
        }
    }
    return c + 1;
}

static void mark_bingo(struct Bingo *board, int *bingo_buffer, int call, int call_point, enum BingoDirection bingo_direction) {
    copy_int_array(board->bingo, bingo_buffer, BOARD_SIZE);
    int unmarked_number_len = build_unmarked_numbers(board);
    board->unmarked_size = unmarked_number_len;
    board->last_number_called = call;
    board->has_bingo = 1;
    board->unmarked_sum = 0;
    board->winning_row_sum = 0;
    board->bingo_direction = bingo_direction;
    board->last_number_called_point = call_point;
    for (int i = 0; i < unmarked_number_len; i++) {
        board->unmarked_sum = board->unmarked_sum + board->unmarked[i];
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        board->winning_row_sum = board->winning_row_sum + board->bingo[i];
    }
}

static void check_bingo(struct Bingo *board, int call, int call_point) {
    int bingo[BOARD_SIZE + 1];
    if (board->has_bingo == 1) {
        return;
    }
    /* row bingo */
    for (int i = 0; i < BOARD_SIZE; i++) {
        empty_array(bingo, BOARD_SIZE);
        for (int j = 0; j < BOARD_SIZE; j++) {
            bingo[j] = board->playing_board[i][j];
        }
        compact(bingo, BOARD_SIZE);
        if (array_size(bingo) == BOARD_SIZE) {
            mark_bingo(board, bingo, call, call_point, ACROSS);
            return;
        }
    }
    if (board->has_bingo == 1) {
        return;
    }
    /* column bingo */
    for (int i = 0; i < BOARD_SIZE; i++) {
        empty_array(bingo, BOARD_SIZE);
        for (int j = 0; j < BOARD_SIZE; j++) {
            bingo[j] = board->playing_board[j][i];
        }
        compact(bingo, BOARD_SIZE);
        if (array_size(bingo) == BOARD_SIZE) {
            mark_bingo(board, bingo, call, call_point, DOWN);
            return;
        }
    }
}

static void play_board(struct Bingo *board, int call, int call_point) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board->board[i][j] == call) {
                board->playing_board[i][j] = call;
                check_bingo(board, call, call_point);
            }
        }
    }
}

static void play_bingo(struct Bingo *boards, int boards_size, int *called_numbers, int called_size) {
    for (int c = 0; c < called_size; c++) {
        for (int i = 0; i < boards_size; i++) {
            play_board(&boards[i], called_numbers[c], c);
        }
    }
}

static int p1() {
    int called_numbers[MAX_BUFFER], boards_size = 0, called_size = 0;
    struct Bingo *boards = load(called_numbers, &boards_size, &called_size);
    play_bingo(boards, boards_size, called_numbers, called_size);
    int result = 0;
    /* first called */
    int min_call_point = UINT16_MAX;
    for (int i = 0; i < boards_size; i++) {
        if (min_call_point > boards[i].last_number_called_point) {
            min_call_point = boards[i].last_number_called_point;
        }
    }
    for (int i = 0; i < boards_size; i++) {
        if(boards[i].has_bingo && boards[i].last_number_called_point == min_call_point) {
            result = boards[i].unmarked_sum * boards[i].last_number_called;
            print_bingo_board(boards[i]);
            break;
        }
    }
    return result;
}

static int p2() {
    int called_numbers[MAX_BUFFER], boards_size = 0, called_size = 0;
    struct Bingo *boards = load(called_numbers, &boards_size, &called_size);
    play_bingo(boards, boards_size, called_numbers, called_size);
    int result = 0;
    /* last called */
    int max_call_point = 0;
    for (int i = 0; i < boards_size; i++) {
        if (max_call_point < boards[i].last_number_called_point) {
            max_call_point = boards[i].last_number_called_point;
        }
    }
    for (int i = 0; i < boards_size; i++) {
        if(boards[i].has_bingo && boards[i].last_number_called_point == max_call_point) {
            result = boards[i].unmarked_sum * boards[i].last_number_called;
            print_bingo_board(boards[i]);
            break;
        }
    }
    return result;
}

int day04(enum part p) {

    if (p == ONE) {
        return p1();
    } else {
        return p2();
    }

}