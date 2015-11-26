/*
* Jessica Hawkins
*
* The symbol (variable) table
*
*/

#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include <string.h>

#include "token.h"
//#include "blockparser.h"

typedef int vartype;

enum vartypes { BOOLTYPE, INTTYPE, NUMTYPES };

struct symboltable *cursymtablep; //points to the current symboltable

struct symbol {

	bool hasvalue;
	double value;
	struct symbol *nextsymbol;
	bool hasnext;
	vartype mytype;

	char name[MAXIDLEN+1];
};

//implement symboltable as linked list (easy way to make
// it of variable length)
struct symboltable {

	int numsymbols;

	struct symbol *firstsymbol;
	//array of pointers to symbols
    //struct symbol **symbols;

	bool hasparentsymtable;
	struct symboltable *parentsymtablep; //does this work?

};

//methods

//initialize and return the symbol
struct symbol* initsymbol (char *idname);

//initialize and return the symbol table
struct symboltable initsymtable ( );

//add a new symbol
// don't need to pass it the symbol table pointer
// b/c there's only one symbol table; global var
void addsymbol (char *idname, vartype ty);

//assign a value to a symbol
void assignval (char *idname, double val);

//returns true if the given symbol exists in the table
bool existssymbol (struct token tok);

//same as existssymbol
bool existsname (char *idname);

//true if has been initialized
bool hasbeeninited (char *idname);

//get the value of a given symbol
double getval (struct token tok);

//get the value of a given symbol, given an id
int getvalueid (char *id);

//get the type of a given symbol (bool or int)
vartype gettype (char *idname);

#endif // SYMBOLTABLE_H_

