/*
*  Jessica Hawkins
*  Compiler Design - Assignment 3
*
*/

#include "scanner.h"
#include "token.h"

//initialize scanner sc
struct scanner initscanner ( )
{
	return sc;
}

//return the class of the current token in the scanner
Class cur_class ( )
{
	return (sc.ctok).myClass;
}

//return the current position in the scanner
struct position* cur_pos ( )
{
	return sr.cpos;
}

bool isalphanum (char c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| (c >= '1' && c <= '9')
		|| (c == '0'));
}

void set_class (Class cl)
{
	(sc.ctok).myClass = cl;
}

void set_info (int c)
{
	(sc.ctok).adinfo = c;
}

char* classname (Class c)
{
	switch (c)
	{
		case EOL:
			return "EOL";
		case ID:
			return "identifier";
		case UINT:
			return "unsigned int";
		case OPADD:
			return "+";
		case OPMIN:
			return "-";
		case OPMULT:
			return "*";
		case LPAR:
			return "(";
		case RPAR: 
			return ")";
		case ASSIGNMENT:
			return "assignment";
		case QMARK:
			return "query";
		case COMMENT:
			return "comment";
		case KEYWORD:
			return "keyword";
		case RELOP:
			return "relational operator";
		case SPECIALCHAR:
			return "special char";
		case EOFCL:
			return "end of file";
	}
	return "unknown";
}

// Precondition: assumes sr is a valid streamreader which has been opened.
//  Assume this token starts with cch in the streamreader sr (that is,
//  inc_char has already been called.)
// Postcondition: reads a token; sc.ctok has been updated to reflect the next token
void nextsy ( )
{
	set_class(-1);
	sc.ctok.adinfo = -1;

	while ((sr.cch == ' ' || sr.cch == '\t') && !is_end_of_input( ))
	{
		inc_char( );
	}

	char c = sr.cch;
	char d;

	// Decide (tentatively) what class this token is
	switch (c)
	{
		case '(':
			set_class (LPAR);
			inc_char( );
			break;
		case ')':
			set_class (RPAR);
			inc_char( );
			break;
		case '+':
			set_class (OPADD);
			set_info(c);
			inc_char( );
			break;
		case '-':
			set_class (OPMIN);
			set_info(c);
			inc_char( );
			break;
		case '*':
			set_class (OPMULT);
			set_info(c);
			inc_char( );
			break;
		case '?':
			set_class (QMARK);
			inc_char( );
			break;
		case ':':
			set_class (ASSIGNMENT);
			inc_char( );
			d = sr.cch; //must be '='
			  //otherwise, this becomes SPECIALCHAR
			break;
		case ';':
			set_class (SPECIALCHAR);
			set_info(SEMIC);
			inc_char( );
			break;
		case '<': case '>': case '=': case '!':
			set_class (RELOP);
			inc_char( );
			d = sr.cch; //later check for correct relop
			break;
		case '/':
			set_class (COMMENT);
			inc_char( );
			d = sr.cch; //must be '/'
			break;
		case '\n':      //EOL for Unix is just \n
			set_class (EOL);
			inc_char( );
			break;
		case '\r':      //EOL for Windows is \r\n
			set_class (EOL);
			inc_char( );
			d = sr.cch; //must be \n
			break;
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
		case '8': case '9':
			set_class (UINT);
			set_info((c - '0')); //convert char to int
			break;
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
		case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
		case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
		case 's': case 't': case 'u': case 'v': case 'w': case 'x':
		case 'y': case 'z': case 'A': case 'B': case 'C': case 'D':
		case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K': case 'L': case 'M': case 'N': case 'O': case 'P':
		case 'Q': case 'R': case 'S': case 'T': case 'U': case 'V':
		case 'W': case 'X': case 'Y': case 'Z':
			set_class (ID); //check later - may also be KEYWORD
			break;
		case EOF:
			set_class (EOFCL);
			break;
		default:
			// starts with an illegal char
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf("  unexpected char %c\n", c);

			errorfound = true;
			set_class (EOL); //or error?
			inc_char( );
			break;
	}

	// Check for assignment ':='
	if (cur_class( ) == ASSIGNMENT)
	{
		if (d == '=')
		{
			inc_char( );
		}
		else
		{
			set_class(SPECIALCHAR);
			set_info(COLON);
		}
	}

	// Check for comment '//'
	if ((cur_class( ) == COMMENT) && (d != '/'))
	{
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf("  expected char \\\n");
		errorfound = true;
	}
	else if ((cur_class( ) == COMMENT) && (d == '/'))
	{
		inc_char( );
		//go to end of line
		char c = sr.cch;
		while (c != '\n' && c != EOF)
		{
		    inc_char ( );
		    c = sr.cch;
		}
		set_class(EOL);
		inc_char( );
	}

	if (cur_class( ) == RELOP)
	{
		if (c == '<')
		{
			set_info(LESSTHAN);
		}
		else if (c == '=' && d == '<')
		{
			set_info(LESSTHANEQ);
			inc_char( );
		}
		else if (c == '=' && d != '<')
		{
			set_info(EQUALS);
		}
		else if (c == '!' && d == '=')
		{
			set_info(NOTEQUALS);
			inc_char( );
		}
		else if (c == '>' && d == '=')
		{
			set_info(GREATERTHANEQ);
			inc_char( );
		}
		else if (c == '>' && d != '=')
		{
			set_info(GREATERTHAN);
		}
		else
		{
			//error - incorrect relational operator
		}
	}

	//check for EOL \r\n
	if ((cur_class( ) == EOL) && (c == '\r') && (d != '\n'))
	{
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" incorrect newline sequence\n");
		errorfound = true;
	}
	else if ((cur_class( ) == EOL) && (c == '\r') && (d == '\n'))
	{
			inc_char( );
	}

	// If UINT, keep reading until the end of the int
	if (cur_class( ) == UINT)
	{
		int val = (sc.ctok).adinfo;
		inc_char( );

		while (sr.cch >= '0' && sr.cch <= '9')
		{
			val = val * 10;
			val = val + (sr.cch - '0');
			inc_char( );
		}

		set_info(val);
	}

	// If ID, keep reading until the end of the identifier
	if (cur_class( ) == ID)
	{
		int idlength = 0;

		while ((cur_class( ) == ID) && isalphanum(sr.cch) && idlength < MAXIDLEN)
		{
			sc.ctok.id [idlength] = sr.cch;
			sc.ctok.id [idlength+1] = '\0';
			idlength++;

			inc_char( );

			//Test for keywords
			if (idlength == 2)
			{
				if (strcmp(sc.ctok.id, "do") == 0)
				{
					set_class (KEYWORD);
					set_info (DO);
				}
				else if (strcmp(sc.ctok.id, "if") == 0)
				{
					set_class (KEYWORD);
					set_info (IF);
				}
				else if (strcmp(sc.ctok.id, "fi") == 0)
				{
					set_class (KEYWORD);
					set_info (FI);
				}
				else if (strcmp(sc.ctok.id, "od") == 0)
				{
					set_class (KEYWORD);
					set_info (OD);
				}
				else if (strcmp(sc.ctok.id, "or") == 0)
				{
					set_class (KEYWORD);
					set_info (OR);
				}
			}
			else if (idlength == 3)
			{
				if (strcmp(sc.ctok.id, "and") == 0)
				{
					set_class (KEYWORD);
					set_info (AND);
				}
				else if (strcmp(sc.ctok.id, "div") == 0)
				{
					set_class (KEYWORD);
					set_info (DIV);
				}
				else if (strcmp(sc.ctok.id, "end") == 0)
				{
					set_class (KEYWORD);
					set_info (END);
				}
				else if (strcmp(sc.ctok.id, "int") == 0)
				{
					set_class (KEYWORD);
					set_info (INT);
				}
				else if (strcmp(sc.ctok.id, "mod") == 0)
				{
					set_class (KEYWORD);
					set_info (MOD);
				}
				else if (strcmp(sc.ctok.id, "not") == 0)
				{
					set_class (KEYWORD);
					set_info (NOT);
				}
				else if (strcmp(sc.ctok.id, "var") == 0)
				{
					set_class (KEYWORD);
					set_info (VAR);
				}
			}
			else if (idlength == 4)
			{
				if (strcmp(sc.ctok.id, "bool") == 0)
				{
					set_class (KEYWORD);
					set_info (BOOL);
				}
				else if (strcmp(sc.ctok.id, "else") == 0)
				{
					set_class (KEYWORD);
					set_info (ELSE);
				}
				else if (strcmp(sc.ctok.id, "skip") == 0)
				{
					set_class (KEYWORD);
					set_info (SKIP);
				}
				else if (strcmp(sc.ctok.id, "then") == 0)
				{
					set_class (KEYWORD);
					set_info (THEN);
				}
				else if (strcmp(sc.ctok.id, "true") == 0)
				{
					set_class (KEYWORD);
					set_info (TRUE);
				}
			}
			else if (idlength == 5)
			{
				if (strcmp(sc.ctok.id, "begin") == 0)
				{
					set_class (KEYWORD);
					set_info (BEGIN);
				}
				else if (strcmp(sc.ctok.id, "false") == 0)
				{
					set_class (KEYWORD);
					set_info (FALSE);
				}
				else if (strcmp(sc.ctok.id, "print") == 0)
				{
					set_class (KEYWORD);
					set_info (PRINT);
				}
				else if (strcmp(sc.ctok.id, "while") == 0)
				{
					set_class (KEYWORD);
					set_info (WHILE);
				}
			}
		}
	}

	//New addition to scanner -- ignore EOL like whitespace
	if (cur_class( ) == EOL)
	{
		nextsy ( );
	}
}



















