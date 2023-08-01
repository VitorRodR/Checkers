#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define BOARD_LINES 8
#define BOARD_COLUMNS 8
#define PIECE_SYMBOL " \u25EF "
#define YELLOW "\x1b[33m\0"
#define RED "\x1b[31m\0"
#define BLACK "\x1b[30m\0"
#define GREEN "\x1b[42m\0"
#define DEFAULT_COLOR "\x1b[39m\0"
#define WHITE_BACKGROUND "\x1b[47m\0"
#define GREY_BACKGROUND "\x1b[48;5;244m\0"
#define DEFAULT_BACKGROUND_COLOR "\x1b[49m\0"
#define ERASE_SCREEN "\x1b[2J\0"
#define RESET_CURSOR "\x1b[H\0"
#define FALSE 0
#define TRUE 1

typedef struct Player   Player;
typedef struct Position Position;
typedef struct Square   Square;
typedef struct Piece    Piece;

struct Player {
    int piece_count;
}; 

struct Position {
    int x, y;
};

struct Square {
    Position pos;
    char colour[15];
};

struct Piece {
    Position pos;
    Player *player;
    int checker;
    char colour[15];
};