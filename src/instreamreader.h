/*
*  Jessica Hawkins
*  Compiler Design - Assignment 5
*
*  Header file for struct streamreader and
*   associated subroutines.
*/

#ifndef INSTREAMREADER_H_
#define INSTREAMREADER_H_

#include <stdbool.h> //for return type bool
#include <stdio.h>

#include "position.h"

bool success_open; //true if open_input was successful

struct instreamreader {
	char cch; //current character
	char prevch; //previous character
	struct position *cpos; //current position

	FILE *fp; //pointer to the file to read from
} sr;

//***The following methods interact with global variable sr:

struct instreamreader open_input (char *filename);

bool is_end_of_input ( ); //returns true if EOF

void inc_char ( ); //increments the current char

void close_input ( ); //closes input stream

#endif // INSTREAMREADER_H_
