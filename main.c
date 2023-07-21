#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOARD_LINES 8
#define BOARD_COLUMNS 8
#define RED "\x1b[31m\0"
#define BLACK "\x1b[30m\0"
#define WHITE_BACKGROUND "\x1b[47m\0"
#define GREY_BACKGROUND "\x1b[48;5;244m\0"
#define RESET_CURSOR_POSITION "\x1b[H"

typedef struct {
    int pos_x, pos_y;
    char colour[10];
} Piece;

Piece *board[BOARD_LINES][BOARD_COLUMNS];

void board_initialize() {
    Piece *game_piece = NULL;
    char colour[11];
    
    for(int i=0; i<BOARD_LINES; i++) {
        for(int j=0; j<BOARD_COLUMNS; j++) {
            game_piece = NULL;
                 
            if(i<3 && i%2 == j%2) {
                strcpy(colour, RED);   
                game_piece = (Piece *) malloc(sizeof(Piece));   
            }
            else if(i>4 && i%2 != j%2) {
                strcpy(colour, BLACK);
                game_piece = (Piece *) malloc(sizeof(Piece));
            }
                
            if(game_piece) {
                game_piece -> pos_x = i;
                game_piece -> pos_y = j;
                strcpy(game_piece->colour, colour);
            }
                
            board[i][j] = game_piece;    
        }
    }
}

void draw_board() {
    for(int i=0; i<8; i++) {
        for(int j=0; j<8; j++) {
            if(i%2 == j%2)
                printf(WHITE_BACKGROUND);
            else 
                printf(GREY_BACKGROUND);
      
            printf("   ");
            
            printf("\x1b[3D");
            
            if(board[i][j]) {
                printf("%s", board[i][j]->colour);
                printf(" \u25EF ");
            }
            else {
                printf("\x1b[3C");
            }
        }
        printf("\n");
    }
}
            

int main() {
    board_initialize();
    draw_board();  
}