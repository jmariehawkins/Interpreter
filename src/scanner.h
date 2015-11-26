/*
*  Jessica Hawkins
*  Compiler Design - Assignment 3
*
*/

#ifndef SCANNER_H_
#define SCANNER_H_

#include <string.h>

#include "instreamreader.h"
#include "token.h"

bool errorfound;

struct scanner {

	struct token prevtok; //next token (for look-ahead)
    struct token ctok; //current token

} sc;

//***the following methods interact with global variable sc:

//initialize scanner sc
struct scanner initscanner ( );

Class cur_class ( );

struct position* cur_pos ( );

bool isalphanum (char c);

void set_class ( Class cl);

void set_info ( int c);

char* classname (Class c);

// Precondition: assumes sr is a valid streamreader which has been opened.
// Postcondition: sc.ctok has been updated to reflect the next token
void nextsy ( );

#endif // SCANNER_H_
