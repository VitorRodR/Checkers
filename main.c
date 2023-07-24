#include "utilities.h"

Piece *board[BOARD_LINES][BOARD_COLUMNS];
Player *player_turn = NULL;
Player *p1, *p2;

void board_initialize() {
    Player *player    = NULL;
    Piece *game_piece = NULL;
    char colour[11];
    
    for(int i=0; i<BOARD_LINES; i++) {
        for(int j=0; j<BOARD_COLUMNS; j++) {
            game_piece = NULL;
                 
             // Positioning the pieces
            if((i%2 == j%2) && (i<3 || i>6)) {
                if(i<3) {
                    player = p1;
                    strcpy(colour, RED);   
                }  
                else if(i>4) {
                    player = p2;
                    strcpy(colour, BLACK);
                }
                
                player->piece_count++;
                    
                game_piece = (Piece *) malloc(sizeof(Piece));
                game_piece -> pos.x   = i;
                game_piece -> pos.y   = j;
                game_piece -> player  = player;
                game_piece -> checker = FALSE;
                strcpy(game_piece->colour, colour);
            }
                
            board[i][j] = game_piece;    
        }
    }
}

void draw_board() {
    printf(ERASE_SCREEN);
    printf(RESET_CURSOR);
    
    int player_number;
    
    printf(WHITE_BACKGROUND);
    if(player_turn == p1){ printf(RED); player_number = 1; }
    else { printf(BLACK); player_number = 2; }
    
    printf("       PLAYER %d TURN       \n\n", player_number);
    
    for(int i=0; i<8; i++) {
        printf(DEFAULT_COLOR);
        printf(DEFAULT_BACKGROUND_COLOR);
        printf("%d ", i);
        for(int j=0; j<8; j++) {
            // Drawing the board itself
            if(i%2 == j%2)
                printf(WHITE_BACKGROUND);
            else 
                printf(GREY_BACKGROUND);
      
            printf("   ");
            
            printf("\x1b[3D");
            
            // Drawing pieces
            if(board[i][j]) {
                printf("%s", board[i][j]->colour);
                printf(PIECE_SYMBOL);
            }
            else {
                printf("\x1b[3C");
            }
        }
        printf("\n");
    }
    
    printf(DEFAULT_COLOR);
    printf(DEFAULT_BACKGROUND_COLOR);
    printf("  ");
    for(int i=0; i<8; i++)
        printf(" %d ", i);
        
    printf("\n");
}

// You can't play straight times!
void change_player_turn(Player *p1, Player *p2) {
    player_turn = (player_turn == p1) ? p2 : p1;
}

// Normal attack
int attack_play(Piece *piece_attacking, Position new_pos, Position piece_pos) {
    if(abs(piece_pos.y - new_pos.y) != 2)
        return FALSE;
        
    Position middle_attack_pos;
    
    // Calculating the attack victim position at the board
    middle_attack_pos.x = (piece_pos.x + new_pos.x) / 2;
    middle_attack_pos.y = (piece_pos.y + new_pos.y) / 2;
    Piece *piece_in_danger  = board[middle_attack_pos.x][middle_attack_pos.y];
    
    // You can't attack the void! / No fire friend here!
    if(!piece_in_danger || (piece_in_danger->player == piece_attacking->player))
        return FALSE;
     
    // Someone losses a piece   
    piece_in_danger->player->piece_count--;
    board[middle_attack_pos.x]
         [middle_attack_pos.y] = NULL;
        
    return TRUE;
}

// The checker has superpowers
int checker_play(Piece *checker_piece, Position new_pos, Position piece_pos) {
    int pos_x = piece_pos.x;
    int pos_y = piece_pos.y; 
    // how many pieces at the checker road
    int on_way_pieces_count = 0;
    // how many pponent pieces at the checker road
    int on_way_opponent_pieces_count = 0; 
    // where is this opponent piece exactly?
    Position on_way_opponent_pos;
    // Calculating the move directions
    int dir_ver_move = (new_pos.x > pos_x) ? 1 : -1;
    int dir_hor_move = (new_pos.y > pos_y) ? 1 : -1;
    
    // Countig how many pieces are at the checker road
    for(int i=pos_x+dir_ver_move; i!=new_pos.x; i+=dir_ver_move) {
        for(int j=pos_y+dir_hor_move; j!=new_pos.y; j+=dir_hor_move) {
            if(board[i][j]) {
                on_way_pieces_count++;
                // A checker can't "jump" over a frindly piece
                if(board[i][j]->player == checker_piece->player)
                    return FALSE;
                else {
                    on_way_opponent_pieces_count++;
                    on_way_opponent_pos.x = i;
                    on_way_opponent_pos.y = j;
                }
            }
        }
    }
    
    printf(YELLOW);
    printf("On way pieces: %d\n", on_way_pieces_count);
    sleep(2);
    
    // A checker can't attack two or more pieces at the same time
    if(on_way_pieces_count > 1)
        return FALSE;
    else if(on_way_opponent_pieces_count == 1) {
        board[on_way_opponent_pos.x][on_way_opponent_pos.y]->player->piece_count--;
        board[on_way_opponent_pos.x][on_way_opponent_pos.y] = NULL;
    }
    
    
    return TRUE;
}    
        

int move_piece(Piece *piece_to_move, int new_pos_x, int new_pos_y, Player *p1, Player *p2) {
    Position piece_pos = piece_to_move->pos;
    Position new_pos   = { new_pos_x, new_pos_y };
    
    if(!piece_to_move            || board[new_pos.x][new_pos.y] ||
        piece_pos.x == new_pos.x || piece_pos.y == new_pos.y    ||
        piece_to_move->player != player_turn)
        return FALSE;
      
    int piece_foward = (piece_to_move->player == p1) ? 1 : -1;
    piece_foward = piece_pos.x + piece_foward;
    
    if(!(attack_play(piece_to_move, new_pos, piece_pos)) &&
       !(new_pos.x == piece_foward && abs(piece_pos.y - new_pos.y) == 1) &&
       !(piece_to_move->checker == TRUE && checker_play(piece_to_move, new_pos, piece_pos)))
        return FALSE;
    
    piece_to_move->pos.x = new_pos.x;
    piece_to_move->pos.y = new_pos.y;
        
    board[piece_pos.x][piece_pos.y] = NULL;
    board[new_pos.x][new_pos.y] = piece_to_move;
    
    if(new_pos_x == 0 || new_pos_x == 7) {
        piece_to_move->checker = TRUE;
        strcpy(piece_to_move->colour, YELLOW);
    }
    
    return TRUE;
}
 
    
int get_move_command() {
    int pos_x, pos_y;
    int new_pos_x, new_pos_y;
   
    do { 
        printf("PIECE COORDINATES (X, Y): ");
        scanf("%d%d", &pos_x, &pos_y);
        printf("NEW PIECE COORDINATES (X, Y): ");
        scanf("%d%d", &new_pos_x, &new_pos_y);
    } while(pos_x     > BOARD_LINES   || pos_x     < 0 ||
            pos_y     > BOARD_COLUMNS || pos_y     < 0 ||
            new_pos_x > BOARD_LINES   || new_pos_x < 0 ||
            new_pos_y > BOARD_COLUMNS || new_pos_y < 0);
    
    if(!move_piece(board[pos_x][pos_y], new_pos_x, new_pos_y, p1, p2)) {
        printf(RED);
        printf("INVALID MOVE!\n");
        sleep(2);
        return FALSE;
    }
    
    return TRUE;
}
            

int main() {
    p1 = (Player *) malloc(sizeof(Player));
    p1->piece_count = 0;
    p2 = (Player *) malloc(sizeof(Player));
    p2->piece_count = 0;
    int winner_player;
    
    board_initialize();
    player_turn = p1;
   
    while(p1->piece_count > 0 && p2->piece_count > 0) {
        draw_board(); 
        if(get_move_command())
            change_player_turn(p1, p2);
    }
    
    printf(WHITE_BACKGROUND);
    
    if(player_turn == p2) { winner_player = 1; printf(RED); }
    else { winner_player = 2; printf(BLACK); }
    
    printf("       PLAYER %d WINS       \n\n", winner_player);
    printf("        END OF GAME         \n\n", winner_player);
}