/*
    SIMPLE CHECKERS GAME
    AUTHOR: VITOR RODRIGUES
    STATUS: INCOMPLETE
    08/01/2023, 13h08
*/


#include "utilities.h"

Square *board_itself[BOARD_LINES][BOARD_COLUMNS];
Piece *board[BOARD_LINES][BOARD_COLUMNS];
Player *player_turn = NULL;
Player *p1, *p2;

void board_itself_initialize() {
    Square *board_square = NULL;
    
    for(int i=0; i<BOARD_LINES; i++) {
        for(int j=0; j<BOARD_COLUMNS; j++) {
            board_square = (Square *) malloc(sizeof(Square));
            board_square->pos.x = i;
            board_square->pos.y = j;
            
            if(i%2 == j%2)
                strcpy(board_square->colour, WHITE_BACKGROUND);
            else
                strcpy(board_square->colour, GREY_BACKGROUND);
                
            board_itself[i][j] = board_square;
        }
    }
}

void board_initialize() {
    Player *player    = NULL;
    Piece *game_piece = NULL;
    char colour[11];
    
    for(int i=0; i<BOARD_LINES; i++) {
        for(int j=0; j<BOARD_COLUMNS; j++) {
            game_piece = NULL;
                 
             // Positioning the pieces
            if((i%2 == j%2) && (i<3 || i>4)) {
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
            printf("%s", board_itself[i][j]->colour);
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
void change_player_turn() {
    player_turn = (player_turn == p1) ? p2 : p1;
}

void move_piece(Piece *piece_to_move, Position new_pos) {
    Position piece_pos = piece_to_move->pos;
    
    piece_to_move->pos.x = new_pos.x;
    piece_to_move->pos.y = new_pos.y;
        
    board[piece_pos.x][piece_pos.y] = NULL;
    board[new_pos.x][new_pos.y] = piece_to_move;
    
    if(new_pos.x == 0 || new_pos.x == 7) {
        piece_to_move->checker = TRUE;
        strcpy(piece_to_move->colour, YELLOW);
    }
}               

int play_finish(Piece *piece) {
    Position piece_pos = piece->pos;
    Position atk_opt1, atk_opt2, atk_opt3, atk_opt4;
    Piece *victm1, *victm2, *victm3, *victm4;
    Piece *oponnent = (piece->player == p1) ? p2 : p1;
    
    int aux = (piece->player == p1) ? 1 : -1;
    int piece_foward     = piece_pos.x + aux;
    int piece_back_off   = piece_pos.x - aux;
    
    atk_opt1.x = piece_foward + aux;
    atk_opt1.y = piece_pos.y + 2;
    
    atk_opt2.x = piece_foward + aux;
    atk_opt2.y = piece_pos.y - 2;
    
    atk_opt3.x = piece_back_off - aux;
    atk_opt3.y = piece_pos.y + 2;
    
    atk_opt4.x = piece_back_off - aux;
    atk_opt4.y = piece_pos.y - 2;
    
    victm1 = board[piece_foward][piece_pos.y + 1];
    victm2 = board[piece_foward][piece_pos.y - 1];
    victm3 = board[piece_back_off][piece_pos.y + 1];
    victm4 = board[piece_back_off][piece_pos.y - 1];
    
    
    int atk_way1 = victm1 && victm1->player == oponnent &&
                  !board[atk_opt1.x][atk_opt1.y];
    int atk_way2 = victm2 && victm2->player == oponnent &&
                  !board[atk_opt2.x][atk_opt2.y];
    int atk_way3 = victm3 && victm3->player == oponnent &&
                  !board[atk_opt3.x][atk_opt3.y];
    int atk_way4 = victm4 && victm4->player == oponnent &&
                  !board[atk_opt4.x][atk_opt4.y];
                  
    //printf("atk_way1: %d\natk_way2: %d\n", atk_way1, atk_way2);
    //sleep(2);
    
    if(atk_way1 && atk_way2) {
        char opt;
        printf("WHERE ATTACK? (l/r): ");
        scanf(" %c", &opt);
        
        if(opt == 'r') {
            if(attack_play(piece, atk_opt1))
                move_piece(piece, atk_opt1);
        }
        else {
            if(attack_play(piece, atk_opt2))
                move_piece(piece, atk_opt2);   
        }  
    }    
    else if(atk_way1) {
        if(attack_play(piece, atk_opt1))
            move_piece(piece, atk_opt1);
    } 
    else if(atk_way2) {
        if(attack_play(piece, atk_opt2))
            move_piece(piece, atk_opt2);
    }
    else if(atk_way3) {
        if(attack_play(piece, atk_opt3))
            move_piece(piece, atk_opt3);
    }
    else if(atk_way4) {
        if(attack_play(piece, atk_opt4))
            move_piece(piece, atk_opt4);
    }
    else
        return TRUE;
        
    return FALSE;
} 
    
    

// Normal move
int normal_move(Piece *piece, Position new_pos) {
    Position piece_pos = piece->pos;
    
    int piece_foward = (piece->player == p1) ? 1 : -1;
    piece_foward = piece_pos.x + piece_foward;
    
    if(new_pos.x != piece_foward || abs(piece_pos.y-new_pos.y) != 1)
        return FALSE;
        
    return TRUE;
}

// Normal attack
int attack_play(Piece *piece_attacking, Position new_pos) {
    Position piece_pos = piece_attacking->pos;
    
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
int checker_play(Piece *checker_piece, Position new_pos) {
    Position piece_pos = checker_piece->pos;
    
    if(checker_piece->checker != TRUE)
        return FALSE;
    
    int pos_x = piece_pos.x;
    int pos_y = piece_pos.y; 
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
    
    // A checker can't attack two or more pieces at the same time
    if(on_way_opponent_pieces_count == 1) {
        board[on_way_opponent_pos.x][on_way_opponent_pos.y]->player->piece_count--;
        board[on_way_opponent_pos.x][on_way_opponent_pos.y] = NULL;
    }
    else if(on_way_opponent_pieces_count > 1)
        return FALSE;
    
    
    return TRUE;
}    

int check_move(Piece *piece_to_move, int new_pos_x, int new_pos_y, Player *p1, Player *p2) {
    Position piece_pos = piece_to_move->pos;
    Position new_pos   = { new_pos_x, new_pos_y };
    
    if(!piece_to_move            || board[new_pos.x][new_pos.y] ||
        piece_pos.x == new_pos.x || piece_pos.y == new_pos.y    ||
        piece_to_move->player != player_turn)
        return FALSE;
        
    int n_move = normal_move (piece_to_move, new_pos);
    int attack = attack_play (piece_to_move, new_pos);
    int c_play = checker_play(piece_to_move, new_pos);
    
    
    if(!(n_move || attack || c_play))
        return FALSE;
    
    move_piece(piece_to_move, new_pos);
    
    while(attack && !play_finish(piece_to_move)) {
        draw_board();
        sleep(2);
    }
    
    return TRUE;
}
 
    
int get_move_command() {
    int pos_x, pos_y;
    int new_pos_x, new_pos_y;
    Piece *selected_piece;
   
    do { 
        printf("PIECE COORDINATES (X, Y): ");
        scanf("%d%d", &pos_x, &pos_y);
    } while(pos_x     >= BOARD_LINES   || pos_x     < 0 ||
            pos_y     >= BOARD_COLUMNS || pos_y     < 0);
    
    selected_piece = board[pos_x][pos_y];
    
    if(!selected_piece) {
        printf(RED);
        printf("NO PIECE SELECTED!\n");
        sleep(2);
        return FALSE;
    }
    
    do {
         printf("NEW PIECE COORDINATES (X, Y): ");
         scanf("%d%d", &new_pos_x, &new_pos_y);
    } while(new_pos_x >= BOARD_LINES   || new_pos_x < 0 ||
            new_pos_y >= BOARD_COLUMNS || new_pos_y < 0);
    
    if(!check_move(selected_piece, new_pos_x, new_pos_y, p1, p2)) {
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
    
    board_itself_initialize();
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