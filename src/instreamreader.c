/*
*  Jessica Hawkins
*  Compiler Design - Assignment 3
*
*  This file contains the struct streamreader and
*   associated subroutines.
*/

#include "instreamreader.h"

// Precondition: sr is a valid streamreader which has been initialized
// Postcondition: returns true if file opened successfully, false otherwise
struct instreamreader open_input (char *filename) 
{
	sr.cch = ' ';

	if (strcmp(filename, "") == 0)
	{
		//filename is empty string, so use stdin
		//printf("opened file stream... using stdin\n");

		//initialize position cpos
		sr.cpos = init (filename, 1, -1);
		success_open = true;
	}
	else if ((sr.fp = fopen(filename, "r")) != NULL)
	{
		//printf("opened file stream...\n");

		//initialize position cpos
		sr.cpos = init (filename, 1, -1);
		success_open = true;
	}
	else
	{
		printf("Error: Couldn't open input file.\n");
		success_open = false;
	}

	return sr;
}

// Precondition: sr is a valid streamreader which has been initialized
// Postcondition: returns true if end of file; false otherwise
bool is_end_of_input ( )
{
	return (sr.cch == EOF);
}

// Precondition: sr is a valid streamreader which has been initialized
// Postcondition: cch, cpos, and prevch are updated for sr accordingly.
void inc_char ( )
{
	sr.prevch = sr.cch;
	sr.cch = getc(sr.fp); //get next character

	//update cpos
	if (sr.prevch == '\n')
	{
		inc_line(sr.cpos);
	}
	else
	{
		inc_col(sr.cpos);
	}
}

// closing the input stream
void close_input ( )
{
	fclose(sr.fp);
}
