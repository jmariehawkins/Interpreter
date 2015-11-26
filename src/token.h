/*
*  Jessica Hawkins
*  Compiler Design - Assignment 8
*
*  Header file for token struct.
*/

#ifndef TOKEN_H_
#define TOKEN_H_

#include "instreamreader.h"

typedef int Class;
typedef int Relop;
typedef int Keyword;
typedef int SpecialChar;

enum myclasses { EOL, ID, UINT, OPADD, OPMIN,
   OPMULT, LPAR, RPAR, ASSIGNMENT, QMARK, COMMENT,
   KEYWORD, RELOP, SPECIALCHAR, EOFCL, NUMCLASSES };  // EOL should be 0, ID 1, UINT 2, etc.
							//RELOP is relational operator

enum relops { LESSTHAN, LESSTHANEQ, EQUALS, NOTEQUALS,
	GREATERTHANEQ, GREATERTHAN, NUMRELOPS }; // all the relational operators:
		// '<', '=<', '=', '!=', '>=', or '>'

enum keywords { AND, BEGIN, BOOL, DIV, DO, IF, ELSE, END,
	FALSE, FI, INT, MOD, NOT, OD, OR, PRINT, SKIP, THEN, TRUE,
	VAR, WHILE, NUMKEYWORDS }; //all the keywords

enum specialchars { COLON, SEMIC }; //all the special chars
	// ':' or ';'

#define MAXIDLEN (50)

struct token {

    int adinfo; //additional info, e.g. UNINT value, which relational operator,
		// which keyword, or which special char

    char id [MAXIDLEN+1]; // if myclass is ID, name of identifier

    Class myClass;

    // Position of first char of this symbol // Do I ever actually use this??? or just isr's cpos?
    struct position *mypos; //pointer because sizeof(mypos) might change?

};

#endif // TOKEN_H_
