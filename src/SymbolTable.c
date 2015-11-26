/*
* Jessica Hawkins
*
* The symbol (variable) table
*
*/

#include "SymbolTable.h"

//initialize and return the symbol
struct symbol* initsymbol (char *idname)
{
    //assign its name
    //has not yet been initialized.
	struct symbol *toreturnp;
	toreturnp = (struct symbol *) malloc(sizeof(struct symbol)); //is this right?

	strcpy(toreturnp->name, idname);
	toreturnp->hasvalue = false;
	toreturnp->value = 0;
	toreturnp->hasnext = false;

	return toreturnp;
}

//initialize and return the symbol table
//..probably don't need this method
struct symboltable initsymtable ( )
{
	cursymtablep->numsymbols = 0;

	return *cursymtablep;
}

//creates a new symbol and adds it to the table
// (if symbol already exists in the table, display
//  an error)
void addsymbol (char *idname, vartype ty)
{
	struct symbol *symp = initsymbol (idname);
	symp->mytype = ty;
	symp->hasvalue = false;

	//add in reverse order, like a stack
	if (cursymtablep->numsymbols == 0)
	{
		cursymtablep->firstsymbol = symp;
	}
	//else if (!existsname(idname))
	else
	{
		symp->nextsymbol = cursymtablep->firstsymbol;
		symp->hasnext = true;
		cursymtablep->firstsymbol = symp;
	}

	(cursymtablep->numsymbols)++;
}

//assign a value to a symbol (assumes type is correct)
void assignval (char *idname, double val)
{
	int i = 0;
	int n = cursymtablep->numsymbols;
	bool found = false;
	struct symbol *cursymbol = cursymtablep->firstsymbol;
	while (i < n && !found)
	{
		if (strcmp( (cursymbol)->name, idname ) == 0)
		{
			found = true;
		}
		else
		{
			i++;
			cursymbol = cursymbol->nextsymbol;
		}
	}

	if (found)
	{
		cursymbol->hasvalue = true;
		cursymbol->value = val;
	}
	else
	{
		//this error should never happen
	}
}

//returns true if the given symbol exists in the table
bool existssymbol (struct token tok)
{
	return existsname (tok.id);
}

bool existsname (char *idname)
{
	int i = 0;
	int n = cursymtablep->numsymbols;
	bool found = false;
	struct symbol *cursymbol = cursymtablep->firstsymbol;
	while (i < n && !found)
	{
		if (strcmp( cursymbol->name, idname ) == 0)
		{
			found = true;
		}
		else
		{
			i++;
			cursymbol = cursymbol->nextsymbol;
		}
	}

	return found;
}

//true if has been initialized
bool hasbeeninited (char *idname)
{
	int i = 0;
	int n = cursymtablep->numsymbols;
	bool found = false;
	bool toreturn = false;
	struct symbol *cursymbol = cursymtablep->firstsymbol;
	while (i < n && !found)
	{
		if (strcmp( cursymbol->name, idname ) == 0)
		{
			found = true;
			toreturn = cursymbol->hasvalue;
		}
		else
		{
			i++;
			cursymbol = cursymbol->nextsymbol;
		}
	}

	return toreturn;
}

//get the value of a given symbol
double getval (struct token tok)
{
	int i = 0;
	int n = cursymtablep->numsymbols;
	double toreturn = -1;
	bool found = false;
	struct symbol *cursymbol = cursymtablep->firstsymbol;
	while (i < n && !found)
	{
		if (strcmp( (cursymbol)->name, tok.id ) == 0)
		{
			found = true;
			if ((cursymbol)->hasvalue)
				toreturn = (cursymbol)->value;
		}
		else
		{
			i++;
			cursymbol = cursymbol->nextsymbol;
		}
	}

	return toreturn;
}

//get the value of a given symbol, given an id
int getvalueid (char* id)
{
	int i = 0;
	int n = cursymtablep->numsymbols;
	int toreturn = -1;
	bool found = false;
	struct symbol *cursymbol = cursymtablep->firstsymbol;
	while (i < n && !found)
	{
		if (strcmp( (cursymbol)->name, id) == 0)
		{
			found = true;
			if ((cursymbol)->hasvalue)
				toreturn = (cursymbol)->value;
		}
		else
		{
			i++;
			cursymbol = cursymbol->nextsymbol;
		}
	}

	return toreturn;
}

//get the type of a given symbol (bool or int)
vartype gettype (char *idname)
{
	int i = 0;
	int n = cursymtablep->numsymbols;
	vartype toreturn;
	bool found = false;
	struct symbol *cursymbol = cursymtablep->firstsymbol;
	while (i < n && !found)
	{
		if (strcmp(cursymbol->name, idname) == 0)
		{
			found = true;
			toreturn = cursymbol->mytype;
		}
		else
		{
			i++;
			cursymbol = cursymbol->nextsymbol;
		}
	}

	return toreturn;
}

