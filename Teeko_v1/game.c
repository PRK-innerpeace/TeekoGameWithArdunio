/*
 * game.c
 *
 * Contains functions relating to the play of the game Teeko
 *
 * Authors: Luke Kamols, Jarrod Bennett
 */

#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "display.h"
#include "terminalio.h"
#include "timer0.h"
#include <avr/pgmspace.h>
// Start pieces in the middle of the board
#define CURSOR_X_START ((int)(WIDTH/2))
#define CURSOR_Y_START ((int)(HEIGHT/2))



uint8_t board[WIDTH][HEIGHT];
int8_t direction_dx[8]={-1,0,1,-1,1,-1,0,1};
int8_t direction_dy[8]={-1,-1,-1,0,0,1,1,1};
int8_t legal_x[8],legal_y[8];
int8_t player_posX[4],player_posY[4];
// cursor coordinates should be /* SIGNED */ to allow left and down movement.
// All other positions should be unsigned as there are no negative coordinates.
int8_t cursor_x;
int8_t cursor_y;
uint8_t cursor_visible;
uint8_t current_player,last_player;
uint8_t game_phase,piece_num=0;
uint8_t hang_on=0,legal_pos=0,can_pick_up=0;

void initialise_game(void) {
	
	// initialise the display we are using
	initialise_display();
	
	// initialise the board to be all empty
	for (uint8_t x = 0; x < WIDTH; x++) {
		for (uint8_t y = 0; y < HEIGHT; y++) {
			board[x][y] = EMPTY_SQUARE;
		}
	}
		
	// set the starting player
	current_player = PLAYER_1;
	game_phase =GAME_PHASE_1;
	
	// also set where the cursor starts
	cursor_x = CURSOR_X_START;
	cursor_y = CURSOR_Y_START;
	cursor_visible = 0;
	piece_num=0;
}

uint8_t get_piece_at(int8_t x, int8_t y) {
	// check the bounds, anything outside the bounds
	// will be considered empty
	if (x < 0 || x >= WIDTH || y < 0 || y >= WIDTH) {
		return EMPTY_SQUARE;
	} else {
		//if in the bounds, just index into the array
		return board[x][y];
	}
}

void flash_cursor(void) {
	
	if (cursor_visible) {
		// we need to flash the cursor off, it should be replaced by
		// the colour of the piece which is at that location
		uint8_t piece_at_cursor = get_piece_at(cursor_x, cursor_y);
		update_square_colour(cursor_x, cursor_y, piece_at_cursor);
		
	} else {
		// we need to flash the cursor on
		update_square_colour(cursor_x, cursor_y, CURSOR);
	}
	cursor_visible = 1 - cursor_visible; //alternate between 0 and 1
}

//check the header file game.h for a description of what this function should do
// (it may contain some hints as to how to move the pieces)
void move_display_cursor(int8_t dx, int8_t dy) {
	//YOUR CODE HERE
	/*suggestions for implementation:

		


	 * 3: display the cursor at the new location
	 * 4: reset the cursor flashing cycle. See project.c for how the cursor
	 *		is flashed.
	 */
	//1: remove the display of the cursor at the current location
	//		(and replace it with whatever piece is at that location)
	//update_square_colour(cursor_x,cursor_y,TERMINAL_COLOUR_EMPTY);
	uint8_t piece_at_cursor = get_piece_at(cursor_x, cursor_y);
	update_square_colour(cursor_x, cursor_y, piece_at_cursor);
	
	// 2: update the positional knowledge of the cursor, this will include
	//		variables cursor_x, cursor_y and cursor_visible. Make sure you
	//		consider what should happen if the cursor moves off the board.
	cursor_x=cursor_x+dx;
	cursor_y=cursor_y+dy;
	if(cursor_x>=WIDTH) cursor_x=0;
	if(cursor_x<0) cursor_x=4;
	if(cursor_y>=HEIGHT) cursor_y=0;
	if(cursor_y<0) cursor_y=4;
	//cursor_visible=0;
	if (hang_on)
	{
		update_square_colour(cursor_x,cursor_y,current_player);
	}
	else 	{update_square_colour(cursor_x,cursor_y,CURSOR);}
	


	
}
void check_legal_led(){
	uint8_t piece_at_cursor = get_piece_at(cursor_x, cursor_y);
if (game_phase==GAME_PHASE_1)
{
		if (piece_at_cursor)
		{
			PORTB=0<<3;
			legal_pos=0;
		}
		else  {
			PORTB=1<<3;
			legal_pos=1;
			
		}
}
else if (game_phase==GAME_PHASE_2)
{
 if (hang_on)
 {
	 legal_pos=check_legal();
	 if (legal_pos)
	 {
		 PORTB=1<<3;
	 }
	 else PORTB=0<<3;
 }
 else{
	 	if (piece_at_cursor==current_player)//first you need to check current player's piece
	 	{//second you need to insert possible direction whether you can pick it up or not
		 	insert_eight_possible_direction();
		 	if (legal_x[0]!=-1)
		 	{
			
			 	can_pick_up=1;

				 
		 	}
		 	else	{can_pick_up=0;
		 	PORTB=0<<3;}
		 	if (can_pick_up)
		 	{
			 	PORTB=1<<3;//LED on when the cursor is on the current player's piece
		 	}
		 	
		 	
	 	}
	 	
	 	else {PORTB=0<<3;
		 	can_pick_up=0;
	 	}
 }

	
}

}
void insert_legal_position(){
		uint8_t i=0;
		while (legal_x[i]!=-1)
		{
			
			board[legal_x[i]][legal_y[i]]=LEGAL_POSITION;
			i++;
		}
}
void place_piece(){
	
	update_square_colour(cursor_x,cursor_y,current_player);
	board[cursor_x][cursor_y]=current_player;
	if(current_player==PLAYER_1) current_player=PLAYER_2;
	else if(current_player==PLAYER_2) current_player=PLAYER_1;
	Turn_Indicator();
}



void Turn_Indicator(){

		move_terminal_cursor(56,4);
		if(current_player==PLAYER_1) printf_P(PSTR("1"));
		if(current_player==PLAYER_2) printf_P(PSTR("2"));
		update_square_colour(4,5,current_player);
		 
}
void pick_up(){
	//update_square_colour(cursor_x,cursor_y,TERMINAL_COLOUR_EMPTY);
	board[cursor_x][cursor_y]=EMPTY_SQUARE;
	hang_on=1;
	uint8_t i=0;
	while (legal_x[i]!=-1)
	{
		
		show_square(legal_x[i],legal_y[i],BG_WHITE);
    	i++;
	}
	

	
	
}
uint8_t check_legal(){
	
	for(uint8_t i=0;i<8;i++)
	  {
		  if ((cursor_x==legal_x[i])&&(cursor_y==legal_y[i]))
		  			return 1;
		
		  
	  }
	return 0;
	  
}
void init_legal_direction(){
	for(uint8_t i=0;i<8;i++){
		 legal_x[i]=-1;
		 legal_y[i]=-1;
	}
	
}
void insert_eight_possible_direction(){
	init_legal_direction();
	int8_t j=0,x,y;
	for(uint8_t i=0;i<8;i++){
		x=cursor_x+direction_dx[i];
		y=cursor_y+direction_dy[i];
		if (x>=WIDTH||x<0||y<0||y>=HEIGHT)
			continue;
		
		if(get_piece_at(x,y)==EMPTY_SQUARE)
		  {
			 
			  legal_x[j]=x;
			  legal_y[j]=y;
			  
			  //
			  
			  j++;
		  }
		  }	
	
	
}

void clear_legal_position(){
	uint8_t i=0;
	while (legal_x[i]!=-1)
	{
		
		
		board[legal_x[i]][legal_y[i]]=EMPTY_SQUARE;
		show_square(legal_x[i],legal_y[i],BG_BLACK);
		i++;
	}
}

void flash_cursor_hang_on(uint8_t player) {
	
	if (cursor_visible) {
		// we need to flash the cursor off, it should be replaced by
		// the colour of the piece which is at that location
		uint8_t piece_at_cursor = get_piece_at(cursor_x, cursor_y);
		update_square_colour(cursor_x, cursor_y, piece_at_cursor);
		
		} else {
		// we need to flash the cursor on
		update_square_colour(cursor_x, cursor_y, player);
	}
	cursor_visible = 1 - cursor_visible; //alternate between 0 and 1
}
void locate_four_positon(){
	
	uint8_t flag=0;
	if(current_player==PLAYER_1) flag=PLAYER_2;
	else if(current_player==PLAYER_2) flag=PLAYER_1;
    uint8_t k=0;
	for (uint8_t i=0;i<WIDTH;i++)
	{
		for (uint8_t j=0 ;j<HEIGHT;j++)
		{
		
			if (board[i][j]==flag)
			{
			//	move_terminal_cursor(1, 1+2*k);
				
				 player_posX[k]=i;				
				 player_posY[k]=j;
				 k++;
			}
		}
	}
}
uint8_t check_arithmetic_progression(int8_t p[]){
	
	int8_t a,b,c;
	a=p[0]-p[1];
	b=p[1]-p[2];
	c=p[2]-p[3];
	if (a==b&&b==c)
	{
		return 1;
	}
	else return 0;
	
	
	
}
uint8_t is_game_over(void) {
	// YOUR CODE HERE
	//first  check out four player position
	if (piece_num>=7)
	{
			locate_four_positon();
			//printf("%d %d %d %d %d %d %d %d",player_posX[0],player_posY[0],player_posX[1],player_posY[1],player_posX[2],player_posY[2],player_posX[3],player_posY[3]);
			if (player_posX[0]==player_posX[1]&&player_posX[0]==player_posX[2]&&player_posX[0]==player_posX[3])
			{
				return 1;
			}
			else if (player_posY[0]==player_posY[1]&&player_posY[0]==player_posY[2]&&player_posY[0]==player_posY[3])
			{
				return 1;
			}
			else if (check_arithmetic_progression(player_posX)&&check_arithmetic_progression(player_posY))
			{
				return 1;
			}
			return 0;
	}
	

	
	// Detect if the game is over i.e. if a player has won.
	return 0;
}