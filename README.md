# TeekoGameWithArdunio
# Teeko Description
This project involves creating a replica of the board game ‘Teeko’. Teeko is a turn-based game 
played between two players on a 5x5 board. Each player has a set of 4 coloured pieces (green for 
player 1, red for player 2) they must take turns placing on the board. A player wins the game if 
they manage to place all of their pieces in a line (which may be a diagonal, vertical or horizontal 
line). Note that these lines do not wrap around the edges of the board.
The game consists of two phases, referred to in this document as game phase 1 and game phase 2.

  1. In game phase 1, the two players take turns placing their 4 pieces on the board. These 
  pieces may be placed on any empty space on the board. This phase ends when all 8 pieces 
  (4 green pieces + 4 red pieces) have been placed on the board.
  2. In game phase 2, players take turns picking up one of their pieces and moving it to one of 
  the adjacent empty spaces. An adjacent space is one of the 8 surrounding spaces but does 
  not wrap around the edges of the board. A piece cannot be placed in the same space it was 
  picked up.

## Program Description

### project.c 
  this is the main file that contains the event loop and examples of how timebased events are implemented. 
### game.h/game.c 
  this file contains the implementation of the board used to store the state of the game and the position of the cursor.   
### display.h/display.c 
  this file contains the implementation for displaying the current state of the board. 
### buttons.h/buttons.c 
  this contains the code which deals with the push buttons. It sets up pin change interrupts on those pins and records rising edges (buttons being pushed). 
### serialio.h/serialio.c 
  this file is responsible for handling serial input and output using interrupts. It also maps the C standard IO routines (e.g. printf() and fgetc()) to use the serial 
  interface so you are able to use printf() etc for debugging purposes if you wish.
### terminalio.h/terminalio.c 
  this encapsulates the sending of various escape sequences which enable some control over terminal appearance and text placement – you can call these functions        (declared in terminalio.h) instead of remembering various escape sequences. Additional information about terminal IO will be provided on the course Blackboard site.
### timer0.h/timer0.c 
  sets up a timer that is used to generate an interrupt every millisecond and update a global time value that is used to time various game events
