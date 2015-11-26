/*
*  Jessica Hawkins
*  Compiler Design - Interpreter
*
* Contains main method to parse and, if no errors, interpret
*/

#include <string.h>
#include <stdio.h>

#include "blockparser.h"
#include "scanner.h"
#include "instreamreader.h"
#include "interpreter.h"

// read in the name of file to parse via command line
int main(int argc, char **argv)
{
	if (argc < 2)
	{
	    printf("Error - incorrect number of arguments.\n");
	    printf("Please give the name of the file to be parsed.\n");
	}
	else
	{
		success_open = false;
	    //assume open_input can deal with potential bad argument
	    open_input (argv[1]);

		if (success_open)
		{
			printf("Opened file, checking for syntactical or symantic errors...\n");
			//read and parse the text file
			struct Block* blockp = parseblock( );
	
			//If no errors, interpret the AST
			if (!err_flag)
			{
				printf("No syntactical or symantic errors found, attempting to run program...\n");
				runBlock (blockp);
			}

			close_input ( );
		}
	}
}
