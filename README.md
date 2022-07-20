# TeekoGameWithArdunio
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
### spi.h/spi.c (IN Students only) 
  this file encapsulates all SPI communication. Note that by default, all SPI communication uses busy waiting (i.e. polling) – the “send” routine returns only when the     data is sent. If you need the CPU cycles for other activities, you may wish to consider converting this to interrupt based IO, similar to the way that serial IO is       handled.
### terminalio.h/terminalio.c 
  this encapsulates the sending of various escape sequences which enable some control over terminal appearance and text placement – you can call these functions        (declared in terminalio.h) instead of remembering various escape sequences. Additional information about terminal IO will be provided on the course Blackboard site.
### timer0.h/timer0.c 
  sets up a timer that is used to generate an interrupt every millisecond and update a global time value that is used to time various game events
