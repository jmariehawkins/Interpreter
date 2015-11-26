/* Jessica Hawkins
*
* Parsing structure for the language P,
*  in order to build the AST.
*/

#include "blockparser.h"

//Returns a bitvector containing the legal first tokens of each
// line in the syntax.
struct bitvector first (syntaxbranch br)
{
	struct bitvector toreturn = bitinit (NUMCLASSES + NUMKEYWORDS);

	switch (br)
	{
	case BLOCK:
		bitadd (NUMCLASSES + BEGIN, &toreturn);
		return toreturn;
	case DECLARATION:
		bitadd (NUMCLASSES + VAR, &toreturn);
		return toreturn;
	case STMTORBLOCK:
		return (unionbit (first(STMT), first(BLOCK)));
	case STMT: 
		bitadd (NUMCLASSES + SKIP, &toreturn);
		bitadd (ID, &toreturn);
		bitadd (NUMCLASSES + PRINT, &toreturn);
		bitadd (NUMCLASSES + IF, &toreturn);
		bitadd (NUMCLASSES + WHILE, &toreturn);
		return toreturn;
	case BOOLEXPR:
		return first(BOOLTERM);
	case BOOLTERM:
		return first (BOOLFACTOR);
	case BOOLFACTOR:
		bitadd (NUMCLASSES + NOT, &toreturn);
		bitadd (NUMCLASSES + TRUE, &toreturn);
		bitadd (NUMCLASSES + FALSE, &toreturn);
		return (unionbit (toreturn, first(RELATEXPR)));
	case RELATEXPR:
		return first(INTEXPR);
	case INTEXPR:
		return first(TERM);
	case TERM:
		return first(FACTOR);
	case FACTOR:
		bitadd (OPMIN, &toreturn);
		bitadd (ID, &toreturn);
		bitadd (UINT, &toreturn);
		bitadd (LPAR, &toreturn);
		return toreturn;
	}
	return toreturn; //error

}

//helper to printing error diagnostics (prints what was found)
void printfound ( )
{
	err_flag = true; //********Will halt processing

	char *found = classname (sc.ctok.myClass);
	printf(" found %s", found);
	if (sc.ctok.myClass == SPECIALCHAR)
	{
		if (sc.ctok.adinfo == COLON)
		{
			printf(" :\n");
		}
		else
		{
			printf(" ;\n");
		}
	}
	else if (sc.ctok.myClass == KEYWORD)
	{
		printf(" %s\n", sc.ctok.id);
	}
	else
	{
		printf("\n");
	}
}

struct Block* parseblock ( )
{
	err_flag = false;
	nextsy( ); //load the first symbol

	struct Block *toreturn;

	struct bitvector blockf = first (BLOCK);
	
	//topmost (empty) symbol table for the whole AST
	struct symboltable *symt;
	symt = (struct symboltable*) malloc(sizeof(struct symboltable)); //stored on the heap
	symt->numsymbols = 0;
	//symt->hasparentsymtable = false;
	cursymtablep = symt;

	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == BEGIN))
	{
		toreturn = pblock ( );
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"begin\",");
		printfound( );
	}
	
	//make sure there isn't extra junk after the legal stmtorblocks


	return toreturn; //if error, may be uninitialized
}

//The following methods are parts of the parsing process which build
// the appropriate struct for the AST and return it, while checking
// for parsing errors relating to that part of the AST.

struct Block* pblock ( )
{
	struct Block *toreturn;
	toreturn = (struct Block*) malloc(sizeof(struct Block)); //stored on the heap
	toreturn->hasstmtorbl = false;

	struct bitvector stmtorblockf = first (STMTORBLOCK);

	nextsy( ); //accept "begin"

	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == VAR))
	{
		toreturn->decp = pdeclaration( );
	}
	
	if (!err_flag && ((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, stmtorblockf)))
		|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), stmtorblockf))))
	{
		toreturn->hasstmtorbl = true;
		toreturn->stmtorblp = pstmtorblock( );
	}

	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == END))
	{
		nextsy( ); //accept "end"
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"end\",");
		printfound( );
	}

	return toreturn;
}

struct Declaration* pdeclaration ( )
{
	struct Declaration *toreturn;
	toreturn = (struct Declaration*) malloc(sizeof(struct Declaration)); //stored on the heap

	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == VAR))
	{
		nextsy( ); //accept "var"
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"var\",");
		printfound( );
	}

	if (!err_flag && cur_class( ) == ID)
	{		
		strcpy(toreturn->identifier, sc.ctok.id);
		nextsy( ); //accept ID
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected ID,");
		printfound( );
	}

	if (!err_flag && (cur_class( ) == SPECIALCHAR) && (sc.ctok.adinfo == COLON))
	{
		nextsy( ); //accept ":"
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected special char \":\",");
		printfound( );
	}

	if (!err_flag && (cur_class( ) == KEYWORD) && ((sc.ctok.adinfo == BOOL)
		|| (sc.ctok.adinfo == INT)))
	{
		if (sc.ctok.adinfo == BOOL)
		{
			toreturn->type = BOOLTYPE;
		}
		else
		{
			toreturn->type = INTTYPE;
		}
		nextsy( ); //accept "bool" or "int"
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"bool\" or \"int\",");
		printfound( );
	}

	if (!err_flag && (cur_class( ) == SPECIALCHAR) && (sc.ctok.adinfo == SEMIC))
	{
		nextsy( ); //accept ";"
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected special char \";\",");
		printfound( );
	}

	if (!err_flag)
	{
		//add to symbol table, cursymtablep
		addsymbol(toreturn->identifier, toreturn->type);
	}

	//Check for more declarations
	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == VAR))
	{
		toreturn->hasnext = true;
		toreturn->declp = pdeclaration( );
	}
	else
	{
		toreturn->hasnext = false;
	}

	return toreturn;
}

struct StatementOrBlock* pstmtorblock ( )
{
	struct StatementOrBlock *toreturn;
	toreturn = (struct StatementOrBlock*) malloc(sizeof(struct StatementOrBlock)); //stored on the heap

	struct bitvector stmtf = first (STMT);

	if (!err_flag && (cur_class( ) == KEYWORD) && ((sc.ctok.adinfo == BEGIN)))
	{
		toreturn->isstatement = false;
		toreturn->blockp = pblock( );
	}
	else if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, stmtf)))
		|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), stmtf)))))
	{
		toreturn->isstatement = true;
		toreturn->stmtp = pstmt( );
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected block or stmt,");
		printfound( );
	}

	//check for more stmts or blocks
	if (!err_flag && (cur_class( ) == SPECIALCHAR) && (sc.ctok.adinfo == SEMIC))
	{
		nextsy ( ); //accept ";"

		toreturn->hasnext = true;
		toreturn->stmtorblp = pstmtorblock( );
	}
	else
	{
		toreturn->hasnext = false;
	}

	return toreturn;
}

struct Statement* pstmt ( )
{
	struct Statement *toreturn;
	toreturn = (struct Statement*) malloc(sizeof(struct Statement)); //stored on the heap

	struct bitvector boolexprf = first (BOOLEXPR);
	struct bitvector stmtorblockf = first (STMTORBLOCK);

	if (!err_flag && (cur_class( ) == KEYWORD) && ((sc.ctok.adinfo == SKIP)))
	{
		//****EmptyStatement****
		toreturn->statementtype = EMPTYSTMT;

		nextsy( ); //accept "skip"
	}
	else if (!err_flag && cur_class( ) == ID)
	{
		//****AssignmentStatement****
		toreturn->statementtype = ASSIGNSTMT;

		strcpy(toreturn->identifier, sc.ctok.id);  //************MAY HAVE TO CHANGE THIS FOR MEMORY REASONS,
		//toreturn->identifier = sc.ctok.id;			//*********AND ALSO TO INTERACT CORRECTLY WITH SYMBOLTABLE

		nextsy( ); //accept ID

		if (!err_flag && cur_class( ) == ASSIGNMENT)
		{
			nextsy( ); //accept ":="
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected \":=\",");
			printfound( );
		}

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, boolexprf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), boolexprf)))))
		{
			toreturn->boolexprp = pboolexpr( );
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
			printfound( );
		}

	}
	else if (!err_flag && (cur_class( ) == KEYWORD) && ((sc.ctok.adinfo == PRINT)))
	{
		//****PrintStatement****
		toreturn->statementtype = PRINTSTMT;

		nextsy( ); //accept "print"

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, boolexprf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), boolexprf)))))
		{
			toreturn->boolexprp = pboolexpr( );
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
			printfound( );
		}
	}
	else if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == IF))
	{
		//****ConditionalStatement****
		toreturn->statementtype = CONDITSTMT;

		nextsy( ); //accept "if"

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, boolexprf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), boolexprf)))))
		{
			toreturn->boolexprp = pboolexpr( );
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
			printfound( );
		}

		if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == THEN))
		{
			nextsy( ); //accept "then"
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"then\",");
			printfound( );
		}

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, stmtorblockf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), stmtorblockf)))))
		{
			toreturn->stmtorblockp1 = pstmtorblock( );
		}
		else if (!err_flag)
		{
			//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"var\", stmt or block,");
			printfound( );
		}

		if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == ELSE))
		{
			nextsy( ); //accept "else"
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"else\",");
			printfound( );
		}

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, stmtorblockf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), stmtorblockf)))))
		{
			toreturn->stmtorblockp2 = pstmtorblock( );
		}
		else if (!err_flag)
		{
			//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"var\", stmt or block,");
			printfound( );
		}

		if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == FI))
		{
			nextsy( ); //accept "fi"
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"fi\",");
			printfound( );
		}
	}
	else if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == WHILE))
	{
		//****IterativeStatement****
		toreturn->statementtype = ITERSTMT;

		nextsy( ); //accept "while"

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, boolexprf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), boolexprf)))))
		{
			toreturn->boolexprp = pboolexpr( );
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
			printfound( );
		}

		if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == DO))
		{
			nextsy( ); //accept "do"
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"do\",");
			printfound( );
		}

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, stmtorblockf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), stmtorblockf)))))
		{
			toreturn->stmtorblockp1 = pstmtorblock( );
		}
		else if (!err_flag)
		{
			//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"var\", stmt or block,");
			printfound( );
		}

		if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == OD))
		{
			nextsy( ); //accept "od"
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"od\",");
			printfound( );
		}

	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"skip\", \"print\", \"if\", \"while\", or ID\n");
		printfound( );
		//Logically, this error should never be reached.
	}

	return toreturn;
}

struct BooleanExpression* pboolexpr ( )
{
	struct BooleanExpression *toreturn;
	toreturn = (struct BooleanExpression*) malloc(sizeof(struct BooleanExpression)); //stored on the heap

	struct bitvector booltermf = first (BOOLTERM);

	if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, booltermf)))
		|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), booltermf)))))
	{
		toreturn->booltermp = pboolterm( );
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
		printfound( );
	}

	return toreturn;
}

struct BooleanTerm* pboolterm ( )
{
	struct BooleanTerm *toreturn;
	toreturn = (struct BooleanTerm*) malloc(sizeof(struct BooleanTerm)); //stored on the heap

	struct bitvector boolfactorf = first (BOOLFACTOR);

	if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, boolfactorf)))
		|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), boolfactorf)))))
	{
		toreturn->boolfactorp = pboolfactor( );
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
		printfound( );
	}

	//check for more terms
	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == OR))
	{
		nextsy ( ); //accept "or"

		toreturn->hasnext = true;
		toreturn->nextbooltermp = pboolterm( );
	}
	else if (!err_flag)
	{
		toreturn->hasnext = false;
	}

	return toreturn;
}

struct BooleanFactor* pboolfactor ( )
{
	struct BooleanFactor *toreturn;
	toreturn = (struct BooleanFactor*) malloc(sizeof(struct BooleanFactor)); //stored on the heap

	struct bitvector relatexprf = first (RELATEXPR);

	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == NOT))
	{
		toreturn->negate = true;
		nextsy( ); //accept "not"
	}
	else
	{
		toreturn->negate = false;
	}

	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == FALSE))
	{
		toreturn->value = false;
		toreturn->hasRelExpr = false;
		nextsy( ); //accept "false"
	}
	else if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == TRUE))
	{
		toreturn->value = true;
		toreturn->hasRelExpr = false;
		nextsy( ); //accept "true"
	}
	else if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, relatexprf)))
		|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), relatexprf)))))
	{
		toreturn->hasRelExpr = true;
		toreturn->relexprp = prelatexpr( );
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
		printfound( );
	}

	//check for more factors
	if (!err_flag && (cur_class( ) == KEYWORD) && (sc.ctok.adinfo == AND))
	{
		nextsy( ); //accept "and"

		toreturn->hasnext = true;
		toreturn->nextboolfactorp = pboolfactor( );
	}
	else if (!err_flag)
	{
		toreturn->hasnext = false; //this branch added 11/26/15
	}

	return toreturn;
}

struct RelationalExpression* prelatexpr ( )
{
	struct RelationalExpression *toreturn;
	toreturn = (struct RelationalExpression*) malloc(sizeof(struct RelationalExpression)); //stored on the heap

	struct bitvector intexprf = first (INTEXPR);

	if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, intexprf)))
		|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), intexprf)))))
	{
		toreturn->intexprp1 = pintexpr( );
	}
	else if (!err_flag)
	{
    	//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
		printfound( );
	}

	if (!err_flag && cur_class( ) == RELOP)
	{
		toreturn->hasrelop = true;
		toreturn->therelop = sc.ctok.adinfo;
		nextsy( ); //accept relational operator

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, intexprf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), intexprf)))))
		{
			toreturn->intexprp2 = pintexpr( );
		}
		else if (!err_flag)
		{
			//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
			printfound( );
		}
	}
	else
	{
		toreturn->hasrelop = false;
	}

	return toreturn;
}

struct IntegerExpression* pintexpr ( )
{
	struct IntegerExpression *toreturn;
	toreturn = (struct IntegerExpression*) malloc(sizeof(struct IntegerExpression)); //stored on the heap

	struct bitvector termf = first (TERM);

	if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, termf)))
		|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), termf)))))
	{
		toreturn->termp = pterm( );
	}
	else if (!err_flag)
	{
		//produce an error message.
		printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
		printf(" expected \"-\", \"(\", ID, or UINT,");
		printfound( );
	}

	return toreturn;
}

struct Term* pterm ( )
{
	struct Term *toreturn;
	toreturn = (struct Term*) malloc(sizeof(struct Term)); //stored on the heap
	toreturn->hasnext = false;

	if (!err_flag)
	{
		toreturn->factorp = pfactor( );
	}

	//check for more terms
	if (!err_flag && ((cur_class( ) == OPMIN) || (cur_class( ) == OPADD)))
	{
		toreturn->hasnext = true;
		if (cur_class( ) == OPMIN)
		{
			toreturn->myop = MINUS;
		}
		else
		{
			toreturn->myop = PLUS;
		}
		nextsy ( );

		toreturn->nexttermp = pterm( );
	}

	return toreturn;
}

struct Factor* pfactor ( )
{
	struct Factor *toreturn;
	toreturn = (struct Factor*) malloc(sizeof(struct Factor)); //stored on the heap
	toreturn->hasnext = false;

	if (!err_flag && (cur_class( ) == OPMIN))
	{
		toreturn->negate = true;
		nextsy( ); //accept "-"
	}
	else
	{
		toreturn->negate = false;
	}

	if (!err_flag && (cur_class( ) == ID))
	{
		toreturn->hasid = true;
		toreturn->hasuint = false;
		strcpy(toreturn->identifier, sc.ctok.id); 

		nextsy( ); //accept ID
	}
	else if (!err_flag && (cur_class( ) == UINT))
	{
		toreturn->hasid = false;
		toreturn->hasuint = true;
		toreturn->uintval = sc.ctok.adinfo;
		nextsy( ); //accept UINT
	}
	else if (!err_flag && (cur_class( ) == LPAR))
	{
		nextsy( ); //accept "("

		struct bitvector boolexprf = first (BOOLEXPR);

		if (!err_flag && (((cur_class( ) == KEYWORD) && (ismember(NUMCLASSES + sc.ctok.adinfo, boolexprf)))
			|| ((cur_class( ) != KEYWORD) && (ismember(cur_class( ), boolexprf)))))
		{
			toreturn->hasid = false;
			toreturn->hasuint = false;
			toreturn->boolexprp = pboolexpr( );
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected keyword \"not\", \"false\", \"true\", \"-\", \"(\", ID, or UINT\n");
			printfound( );
		}

		if (!err_flag && (cur_class( ) == RPAR))
		{
			nextsy( ); //accept ")"
		}
		else if (!err_flag)
		{
			//produce an error message.
			printf("Error at:%s:%d:%d \n", (sr.cpos)->filename, (sr.cpos)->linenum, (sr.cpos)->colnum);
			printf(" expected \")\",");
			printfound( );
		}
	}

	//check for more factors
	if (!err_flag && ((cur_class( ) == OPMULT) || (cur_class( ) == KEYWORD &&
		((sc.ctok.adinfo == DIV) || (sc.ctok.adinfo == MOD)))))
	{
		toreturn->hasnext = true;
		if (cur_class( ) == OPMULT)
		{
			toreturn->myop = TIMES;
		}
		else if (sc.ctok.adinfo == DIV)
		{
			toreturn->myop = DIVOP;
		}
		else
		{
			toreturn->myop = MODOP;
		}
		nextsy ( );

		toreturn->nextfactorp = pfactor( );
	}

	return toreturn;
}

