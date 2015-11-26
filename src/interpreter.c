/*
*  Jessica Hawkins
*  Compiler Design - Interpreter
*
*  Given the AST built by blockparser methods, interpret
*   and check for run-time errors.
*/

#include "interpreter.h"

void runBlock (struct Block* blockp)
{
	if (blockp->hasstmtorbl)
	{
		runStmtorBlock (blockp->stmtorblp);
	}
}

void runStmtorBlock (struct StatementOrBlock* stmtorblockp)
{
	if (stmtorblockp->isstatement)
	{
		runStatement(stmtorblockp->stmtp);
	}
	else
	{
		runBlock(stmtorblockp->blockp);
	}

	//check for more stmtorblocks
	if (stmtorblockp->hasnext)
	{
		runStmtorBlock (stmtorblockp->stmtorblp);
	}
}

void runStatement (struct Statement* stmtp)
{
	if (stmtp->statementtype == EMPTYSTMT)
	{
		//do nothing
	}
	else if (!err_flag && (stmtp->statementtype == ASSIGNSTMT))
	{
		struct Value toassign = evalBoolExpr(stmtp->boolexprp);

		if (!err_flag && existsname(stmtp->identifier))
		{
			if (toassign.mytype == gettype(stmtp->identifier))
			{
				assignval(stmtp->identifier, toassign.myval);
			}
			else
			{
				printf("Error: ");
				if (toassign.mytype == INTTYPE)
				{
					printf("Trying to assign int to bool variable %s\n", stmtp->identifier);
				}
				else
				{
					printf("Trying to assign bool to int variable %s\n", stmtp->identifier);
				}
				err_flag = true;
			}
		}
		else if (!err_flag)
		{
    		//produce an error message.
			printf("Error: ");
			printf("symbol %s was not declared\n", stmtp->identifier);
			err_flag = true;
		}
	}
	else if (!err_flag && (stmtp->statementtype == PRINTSTMT))
	{
		struct Value toprint = evalBoolExpr(stmtp->boolexprp);
		if (!err_flag)
		{
			if (toprint.mytype == INTTYPE)
			{
				printf("%d\n", toprint.myval);
			}
			else if (toprint.myval == 0)
			{
				printf("false\n");
			}
			else
			{
				printf("true\n");
			}
		}
	}
	else if (!err_flag && (stmtp->statementtype == CONDITSTMT))
	{
		struct Value boolexpr = evalBoolExpr(stmtp->boolexprp);

		if (!err_flag && (boolexpr.myval > 0))
		{
			runStmtorBlock(stmtp->stmtorblockp1);
		}
		else if (!err_flag)
		{
			runStmtorBlock(stmtp->stmtorblockp2);
		}
	}
	else if (!err_flag && (stmtp->statementtype == ITERSTMT))
	{
		struct Value boolexpr = evalBoolExpr(stmtp->boolexprp);
		while (!err_flag && (boolexpr.myval > 0))
		{
			runStmtorBlock(stmtp->stmtorblockp1);
			boolexpr = evalBoolExpr(stmtp->boolexprp);
		}
	}
}

//returns the value of the IntegerExpression, or throws a run-time
// error if it could not be evaluated
struct Value evalBoolExpr (struct BooleanExpression* boolexpr)
{
	struct Value toreturn = evalBoolTerm(boolexpr->booltermp);
	struct Value temp;
	struct BooleanTerm* curtermp = boolexpr->booltermp;
	//optype curop;

	if (!err_flag && (curtermp->hasnext) && (toreturn.mytype != BOOLTYPE))
	{
		printf("Error: ");
		printf(" Invalid type for boolean operator \"or\"\n");
		err_flag = true;
	}

	while (!err_flag && curtermp->hasnext)
	{
		curtermp = curtermp->nextbooltermp;
		temp = evalBoolTerm(curtermp);
		if (temp.mytype == BOOLTYPE)
		{
			if (toreturn.myval > 0 || temp.myval > 0)
			{
				toreturn.myval = 1;
			}
			else
			{
				toreturn.myval = 0;
			}
		}
		else
		{
			printf("Error: ");
			printf(" Invalid type for boolean \"or\"\n");
			err_flag = true;
		}
	}

//printf("boolexpr %d, %d\n", toreturn.myval, toreturn.mytype);
	return toreturn;
}

struct Value evalBoolTerm (struct BooleanTerm* termp)
{
	struct Value toreturn = evalBoolFactor(termp->boolfactorp);
	struct Value temp;
	struct BooleanFactor* curfactorp = termp->boolfactorp;

	if (!err_flag && (curfactorp->hasnext) && (toreturn.mytype != BOOLTYPE))
	{
		printf("Error: ");
		printf(" Invalid type for boolean operator \"and\"\n");
		err_flag = true;
	}

	while (!err_flag && curfactorp->hasnext)
	{
		curfactorp = curfactorp->nextboolfactorp;
		temp = evalBoolFactor(curfactorp);
		if (temp.mytype == BOOLTYPE)
		{
			if (toreturn.myval > 0 && temp.myval > 0)
			{
				toreturn.myval = 1;
			}
			else
			{
				toreturn.myval = 0;
			}
		}
		else
		{
			printf("Error: ");
			printf(" Invalid type for boolean \"and\"\n");
			err_flag = true;
		}
	}

//printf("boolterm %d\n", toreturn.myval);
	return toreturn;
}

struct Value evalBoolFactor (struct BooleanFactor* factorp)
{
	struct Value toreturn;

	if (factorp->hasRelExpr)
	{
		toreturn = evalRelExpr(factorp->relexprp);
	}
	else
	{
		if (factorp->value == true)
		{
			toreturn.myval = 1;
			toreturn.mytype = BOOLTYPE;
		}
		else
		{
			toreturn.myval = 0; //false
			toreturn.mytype = BOOLTYPE;
		}
	}

	//check for "not"
	if (!err_flag && factorp->negate)
	{
		if ((toreturn.mytype == BOOLTYPE) && (toreturn.myval == 0))
		{
			toreturn.myval = 1;
		}
		else if (toreturn.mytype == BOOLTYPE)
		{
			toreturn.myval = 0;
		}
		else
		{
			printf("Error: ");
			printf(" Invalid type for boolean \"not\"\n");
			err_flag = true;
		}
	}

//printf("boolfactor %d\n", toreturn.myval);
	return toreturn;
}

struct Value evalRelExpr(struct RelationalExpression* relexprp)
{
	struct Value toreturn;

	if (relexprp->hasrelop)
	{
		struct Value temp1 = evalIntExpr(relexprp->intexprp1);
		int int1 = temp1.myval;
		struct Value temp2 = evalIntExpr(relexprp->intexprp2);
		int int2 = temp2.myval;
		toreturn.myval = 0;
		if (temp1.mytype == temp2.mytype)
		{
			if (   ((relexprp->therelop == LESSTHAN) && (int1 < int2))
				|| ((relexprp->therelop == LESSTHANEQ) && (int1 <= int2))
				|| ((relexprp->therelop == EQUALS) && (int1 == int2))
				|| ((relexprp->therelop == NOTEQUALS) && (int1 != int2))
				|| ((relexprp->therelop == GREATERTHANEQ) && (int1 >= int2))
				|| ((relexprp->therelop == GREATERTHAN) && (int1 > int2)))
			{
				toreturn.myval = 1;
				toreturn.mytype = BOOLTYPE;
			}
			else
			{
				toreturn.mytype = BOOLTYPE;
			}
		}
		else
		{
			printf("Error: ");
			printf(" Comparing incompatible types bool and int\n");
			err_flag = true;
		}
	}
	else
	{
		toreturn = evalIntExpr(relexprp->intexprp1);
	}

//printf("relatexpr %d\n", toreturn.myval);
	return toreturn;
}

//returns the value of the IntegerExpression, or throws a run-time
// error if it could not be evaluated
struct Value evalIntExpr (struct IntegerExpression* intexpr)
{
	struct Value toreturn = evalTerm(intexpr->termp);
	struct Value temp;
	struct Term* curtermp = intexpr->termp;
	optype curop;

	if ((curtermp->hasnext) && (toreturn.mytype != INTTYPE))
	{
		printf("Error: ");
		if (curop == PLUS)
		{
			printf(" Invalid type for integer operator \"+\"\n");
		}
		else
		{
			printf(" Invalid type for integer operator \"-\"\n");
		}
		err_flag = true;
	}

	while (!err_flag && curtermp->hasnext)
	{
		curop = curtermp->myop;
		curtermp = curtermp->nexttermp;
		temp = evalTerm(curtermp);
		if (curop == PLUS)
		{
			if (temp.mytype == INTTYPE)
			{
				toreturn.myval = toreturn.myval + temp.myval;
			}
			else
			{
				printf("Error: ");
				printf(" Invalid type for integer operator \"+\"\n");
				err_flag = true;
			}				
		}
		else
		{
			if (temp.mytype == INTTYPE)
			{
				toreturn.myval = toreturn.myval - temp.myval;
			}
			else
			{
				printf("Error: ");
				printf(" Invalid type for integer operator \"-\"\n");
				err_flag = true;
			}
		}
	}

//printf("intexpr %d\n", toreturn.myval);
	return toreturn;
}

struct Value evalTerm (struct Term* termp)
{
	struct Value toreturn = evalFactor(termp->factorp);
	struct Value temp;
	struct Factor* curfactorp = termp->factorp;
	optype curop;

	if ((curfactorp->hasnext) && (toreturn.mytype != INTTYPE))
	{
		printf("Error: ");
		if (curop == TIMES)
		{
			printf(" Invalid type for integer operator \"*\"\n");
		}
		else if (curop == MODOP)
		{
			printf(" Invalid type for integer operator \"mod\"\n");
		}
		else
		{
			printf(" Invalid type for integer operator \"div\"\n");
		}
		err_flag = true;
	}

	while (curfactorp->hasnext)
	{
		curop = curfactorp->myop;
		curfactorp = curfactorp->nextfactorp;
		temp = evalFactor(curfactorp);
		if (curop == TIMES)
		{
			if (temp.mytype == INTTYPE)
			{
				toreturn.myval = toreturn.myval * temp.myval;
			}
			else
			{
				printf("Error: ");
				printf("Invalid type for integer operator \"*\"\n");
				err_flag = true;
			}
		}
		else if (curop == MODOP)
		{
			if (temp.mytype == INTTYPE)
			{
				int n = temp.myval;
				toreturn.myval = toreturn.myval - (n * (int) (toreturn.myval/n));
			}
			else
			{
				printf("Error: ");
				printf("Invalid type for integer operator \"mod\"\n");
				err_flag = true;
			}
		}
		else if (curop == DIVOP)
		{
			if (temp.myval != 0)
			{
				if (temp.mytype == INTTYPE)
				{
					toreturn.myval = toreturn.myval / temp.myval;
				}
				else
				{
					printf("Error: ");
					printf("Invalid type for integer operator \"/\"\n");
					err_flag = true;
				}
			}
			else
			{		
    			//produce an error message.
				printf("Error: ");
				printf("Divide by zero\n");
				err_flag = true;
			}
		}
	}

//printf("term %d\n", toreturn.myval);
	return toreturn;
}

struct Value evalFactor (struct Factor* factorp)
{
	struct Value toreturn;

	if (factorp->hasuint)
	{
		toreturn.myval = factorp->uintval;
		toreturn.mytype = INTTYPE;
	}
	else if (factorp->hasid)
	{
		if (existsname(factorp->identifier))
		{
			if (hasbeeninited(factorp->identifier))
			{
				toreturn.myval = getvalueid(factorp->identifier);
				toreturn.mytype = gettype(factorp->identifier);
			}
			else
			{	
    			//produce an error message.
				printf("Error: ");
				printf("symbol %s has not been initialized\n", factorp->identifier);
				err_flag = true;
			}
		}
		else
		{		
    		//produce an error message.
			printf("Error: ");
			printf("symbol %s was not declared\n", factorp->identifier);
			err_flag = true;
		}
	}
	else
	{
		toreturn = evalBoolExpr(factorp->boolexprp);
	}

	if (!err_flag && factorp->negate)
	{
		if (toreturn.mytype == INTTYPE)
		{
			toreturn.myval = 0 - toreturn.myval;
		}
		else
		{
			printf("Error: ");
			printf("Invalid type for integer unary operator \"-\"\n");
			err_flag = true;
		}
	}

//printf("factor %d\n", toreturn.myval);
	return toreturn;
}
