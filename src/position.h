/*
*  Jessica Hawkins
*  Compiler Design - Assignment 3
*
* Header file for the position struct
*  and associated subroutines.
*/

#ifndef POSITION_H_
#define POSITION_H_

#include <stdlib.h>

struct position {
	char *filename;
	int linenum;
	int colnum;
};

struct position* init(char *filen, int linen, int coln); //initializes pos to these values

void inc_col(struct position *pos); //increments the column of pos

void inc_line(struct position *pos); //increments the line of pos

struct position copypos(struct position pos1); //returns a copy of pos1

void printpos(struct position pos); //prints pos

#endif // POSITION_H_
