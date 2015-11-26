
/*
* Jessica Hawkins
* Compiler Design - Assignment 3
*
* This file contains the position struct
*  and associated subroutines.
*/

#include "position.h"

#include <string.h>
#include <stdio.h>

//Postcondition: initializes pos to these values
struct position* init(char *filen, int linen, int coln) 
{
	struct position *pos;
	pos = (struct position *) malloc(sizeof(struct position)); //is this right?

	pos->filename = filen;
	pos->linenum = linen;
	pos->colnum = coln;

	return pos;
}

//Precondition: pos points to a valid struct which has been initialized.
//Postcondition: increments the column of pos, returns pos
void inc_col(struct position *pos)
{
	//Increment the column of pos
	(pos->colnum)++;
	//printf("Inc_col:%s:%d:%d \n", (*pos).filename, (*pos).linenum, (*pos).colnum);
}

//Precondition: pos points to a valid struct which has been initialized.
//Postcondition: increments the line of pos, returns line#
void inc_line(struct position *pos) 
{
	//Increment the line of a position (and reset column to 0)
	pos->linenum = (pos->linenum) + 1;
	pos->colnum = 0;
	//printf("Inc_line:%s:%d:%d \n", (*pos).filename, (*pos).linenum, (*pos).colnum);
}

//Precondition: pos1 points to a valid struct which has been initialized.
//Postcondition: copies pos1 into pos2
struct position copypos(struct position pos1)
{
	//Create a copy of a position
	struct position pos2;

	pos2.filename = pos1.filename;
	pos2.linenum = pos1.linenum;
	pos2.colnum = pos1.colnum;
	printf("Copy:%s:%d:%d \n", pos1.filename, pos1.linenum, pos1.colnum);
	return pos2;
}

//Precondition: pos points to a valid struct which has been initialized.
void printpos(struct position pos) //correct args here?
{
}

