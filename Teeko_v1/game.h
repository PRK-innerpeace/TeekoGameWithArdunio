/*
** game.h
**
** Written by Luke Kamols
**
** Function prototypes for those functions available externally
*/

#ifndef GAME_H_
#define GAME_H_
#define GAME_PHASE_1 1
#define GAME_PHASE_2 2
#include <stdint.h>

// initialise the display of the board, this creates the internal board
// and also updates the display of the board
void initialise_game(void);

// returns which piece is located at position (x,y)
// the value returned will be SQUARE_EMPTY, PLAYER_1 or PLAYER_2
// anything outside the bounds of the boards will be SQUARE_EMPTY
uint8_t get_piece_at(int8_t x, int8_t y);

// update the cursor display, by changing whether it is visible or not
// call this function at regular intervals to have the cursor flash
void flash_cursor(void);
void flash_cursor_hang_on(uint8_t player);
// moves the position of the cursor by (dx, dy) such that if the cursor
// started at (cursor_x, cursor_y) then after this function is called, 
// it should end at ( (cursor_x + dx) % WIDTH, (cursor_y + dy) % HEIGHT)
// the cursor should be displayed after it is moved as well
void move_display_cursor(int8_t dx, int8_t dy);
//place piece at current cursor position and switch player
void place_piece();

void Turn_Indicator();
void pick_up();
void insert_eight_possible_direction();
uint8_t check_legal();
void init_legal_direction();
void check_legal_led();
void insert_legal_position();
void clear_legal_position();
uint8_t check_arithmetic_progression(int8_t p[]);
// returns 1 if the game is over, 0 otherwise
uint8_t is_game_over(void);


#endif /* GAME_H_ */