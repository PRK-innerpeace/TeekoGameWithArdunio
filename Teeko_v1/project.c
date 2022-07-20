/*
 * project.c
 *
 * Main file
 *
 * Authors: Peter Sutton, Luke Kamols
 * Modified by <YOUR NAME HERE>
 */ 

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define F_CPU 16000000L
#include <util/delay.h>

#include "game.h"
#include "display.h"
#include "buttons.h"
#include "serialio.h"
#include "terminalio.h"
#include "timer0.h"

// Function prototypes - these are defined below (after main()) in the order
// given here
void initialise_hardware(void);
void start_screen(void);
void new_game(void);
void play_game(void);
void handle_game_over(void);

extern uint8_t game_phase,piece_num,current_player,hang_on,legal_pos,can_pick_up;

/////////////////////////////// main //////////////////////////////////
int main(void) {
	// Setup hardware and call backs. This will turn on 
	// interrupts.
	initialise_hardware();
	
	// Show the splash screen message. Returns when display
	// is complete
	start_screen();
	
	// Loop forever,
	while(1) {
		new_game();
		play_game();
		handle_game_over();
	}
}

void initialise_hardware(void) {
	init_button_interrupts();
	// Setup serial port for 38400 baud communication with no echo
	// of incoming characters
	init_serial_stdio(38400,0);
	
	init_timer0();
	DDRB=1<<3|1<<2;
	PORTB=1<<3;
	
	// Turn on global interrupts
	sei();
}

void start_screen(void) {
	// Clear terminal screen and output a message
	clear_terminal();
	move_terminal_cursor(10,10);
	printf_P(PSTR("TEEKO"));
	move_terminal_cursor(10,12);
	printf_P(PSTR("Your name is  JIANUO PAN and student number is 46144443"));


	
	// Output the static start screen and wait for a push button 
	// to be pushed or a serial input of 's'
	start_display();
	
	// Wait until a button is pressed, or 's' is pressed on the terminal
	while(1) {
		// First check for if a 's' is pressed
		// There are two steps to this
		// 1) collect any serial input (if available)
		// 2) check if the input is equal to the character 's'
		char serial_input = -1;
		if (serial_input_available()) {
			serial_input = fgetc(stdin);
		}
		// If the serial input is 's', then exit the start screen
		if (serial_input == 's' || serial_input == 'S') {		
			
			break;
		}
		// Next check for any button presses
		int8_t btn = button_pushed();
		if (btn != NO_BUTTON_PUSHED) {
			break;
		}
	}
}

void new_game(void) {
	// Clear the serial terminal
	clear_terminal();
	
	// Initialise the game and display
	initialise_game();
	move_terminal_cursor(41,4);
	printf_P(PSTR("Current player: "));
	Turn_Indicator();
	init_legal_direction();
	// Clear a button push or serial input if any are waiting
	// (The cast to void means the return value is ignored.)
	(void)button_pushed();
	clear_serial_input_buffer();
}

void play_game(void) {
	
	uint32_t last_flash_time, current_time;
	uint8_t btn; //the button pushed
	
	last_flash_time = get_current_time();
	
	// We play the game until it's over
	while(!is_game_over()) {
		 check_legal_led();
		
		char serial_input = -1;
				if (serial_input_available()) {
					serial_input = fgetc(stdin);
				}
				//  ¡®w¡¯/¡®W¡¯ moves the cursor upwards
				if (serial_input == 'w' || serial_input == 'W') {
					
					move_display_cursor(0, 1);
				}
				//¡®a¡¯/¡®A¡¯ moves the cursor to the left
				if (serial_input == 'a' || serial_input == 'A') {
					
					move_display_cursor(-1, 0);
				}
				//¡®s¡¯/¡®S¡¯ moves the cursor downwards
				if (serial_input == 's' || serial_input == 'S') {
					
					move_display_cursor(0, -1);
				}
				// ¡®d¡¯/¡®D¡¯ moves the cursor to the right
				if (serial_input == 'd' || serial_input == 'D') {
					
					move_display_cursor(1,0);
				}
				if (serial_input == ' ') {
				
				 if(game_phase==GAME_PHASE_1){
					 					// if current square is blank then put player's piece right here
					 					if(legal_pos) {
						 																					 								place_piece();
						 								piece_num++;
						 								if(piece_num>7) 														 {game_phase=GAME_PHASE_2;														 legal_pos=0;														 }
						 				
					 					}
					 					else	{
											 	//, this move should be rejected and the current player should not change.											 	//you can turn on sound for Illegal move
										
								}
				 }
				 else if(game_phase==GAME_PHASE_2)
				   {
					if (can_pick_up)
					{
						pick_up();
						insert_legal_position();
						can_pick_up=0;
					}
					if(legal_pos&&hang_on) {
					clear_legal_position();										place_piece();
					
					hang_on=0;
					legal_pos=0;
					

				   }
					

				}
				}
		       
		
		
		
		
		// We need to check if any button has been pushed, this will be
		// NO_BUTTON_PUSHED if no button has been pushed
		btn = button_pushed();
		
		if (btn == BUTTON3_PUSHED) {
			// If button 3 is pushed, move left,
			// i.e decrease x by 1 and leave y the same
			move_display_cursor(-1, 0);
		}
		if (btn == BUTTON2_PUSHED) {
			// If button 3 is pushed, move left,
			// i.e decrease x by 1 and leave y the same
			move_display_cursor(1, 0);
		}
	    if (btn == BUTTON1_PUSHED) {
				// If button 3 is pushed, move up,	
				
				// i.e decrease x by 1 and leave y the same
				move_display_cursor(0, 1);
			}
		if (btn == BUTTON0_PUSHED) {
					    // If button 3 is pushed, move down,
					    // i.e decrease x by 1 and leave y the same
					    move_display_cursor(0, -1);
				    }
		current_time = get_current_time();
		if(current_time >= last_flash_time + 500) {
			// 500ms (0.5 second) has passed since the last time we
			// flashed the cursor, so flash the cursor
			if(hang_on) {
				flash_cursor_hang_on(current_player);
			}
			else flash_cursor();
			
			// Update the most recent time the cursor was flashed
			
			last_flash_time = current_time;
		}
	}
	// We get here if the game is over.
}

void handle_game_over() {
	move_terminal_cursor(10,13);
	printf_P(PSTR("Win player is:"));
	if(current_player==PLAYER_1) printf_P(PSTR("2(red player)"));
	if(current_player==PLAYER_2) printf_P(PSTR("1(green player)"));
	move_terminal_cursor(10,14);
	printf_P(PSTR("GAME OVER"));
	move_terminal_cursor(10,15);
	printf_P(PSTR("Press a button to start again"));
	
	while(button_pushed() == NO_BUTTON_PUSHED) {
		; // wait
	}
	
}
