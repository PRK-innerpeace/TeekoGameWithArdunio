# TeekoGameWithArdunio
## Program Description

• project.c – this is the main file that contains the event loop and examples of how timebased events are implemented. You should read and understand this file.
• game.h/game.c – this file contains the implementation of the board used to store the state 
of the game and the position of the cursor. You should read this file and understand what 
representation is used for the board state and the cursor position. You will need to modify 
this file to add required functionality.
• display.h/display.c – this file contains the implementation for displaying the current state 
of the board. This file contains useful functions for displaying the board to the LED matrix 
(internal students) or the terminal display (external students). This file contains the same 
functions for IN and EX students but with significantly different implementations.
• buttons.h/buttons.c – this contains the code which deals with the push buttons. It sets up 
pin change interrupts on those pins and records rising edges (buttons being pushed). For 
EX students this code also handles button debouncing.
• ledmatrix.h/ledmatrix.c (IN students only) – this contains functions which give easier 
access to the services provided by the LED matrix. It makes use of the SPI routines 
implemented in spi.c
• pixel_colour.h (IN students only) – this file contains definitions of some useful colours. 
Colours are defined in terminalio.h for EX students.
• serialio.h/serialio.c – this file is responsible for handling serial input and output using 
interrupts. It also maps the C standard IO routines (e.g. printf() and fgetc()) to use the serial 
interface so you are able to use printf() etc for debugging purposes if you wish. You should 
not need to look in this file, but you may be interested in how it works and the buffer sizes 
used for input and output (and what happens when the buffers fill up). 
• spi.h/spi.c (IN Students only) – this file encapsulates all SPI communication. Note that by 
default, all SPI communication uses busy waiting (i.e. polling) – the “send” routine returns 
only when the data is sent. If you need the CPU cycles for other activities, you may wish 
to consider converting this to interrupt based IO, similar to the way that serial IO is 
handled.
• terminalio.h/terminalio.c – this encapsulates the sending of various escape sequences
which enable some control over terminal appearance and text placement – you can call 
these functions (declared in terminalio.h) instead of remembering various escape 
sequences. Additional information about terminal IO will be provided on the course 
Blackboard site.
• timer0.h/timer0.c – sets up a timer that is used to generate an interrupt every millisecond 
and update a global time value that is used to time various game events
