/*
*  Jessica Hawkins
*  Compiler Design - Interpreter
*
*/

#ifndef INTERPR_H_
#define INTERPR_H_

#include "blockparser.h"
#include "SymbolTable.h"

//Used during evaluations to keep track of type
struct Value {
	vartype mytype;
	int myval; //for bool, 0=false, 1=true
};

void runBlock (struct Block* blockp);

void runStmtorBlock (struct StatementOrBlock* stmtorblockp);

void runStatement (struct Statement* stmtp);

//returns the value of the IntegerExpression, or throws a run-time
// error if it could not be evaluated
struct Value evalBoolExpr (struct BooleanExpression* boolexpr);

struct Value evalBoolTerm (struct BooleanTerm* termp);

struct Value evalBoolFactor (struct BooleanFactor* factorp);

struct Value evalRelExpr(struct RelationalExpression* relexprp);

//returns the value of the IntegerExpression, or throws a run-time
// error if it could not be evaluated
struct Value evalIntExpr (struct IntegerExpression* intexpr);

struct Value evalTerm (struct Term* termp);

struct Value evalFactor (struct Factor* factorp);


#endif // INTERPR_H
