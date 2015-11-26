/* Jessica Hawkins
*
* Parsing structure for the language P,
*  in order to build the AST.
*/

#ifndef BLOCKPARSER_H_
#define BLOCKPARSER_H_

#include "bitvector.h"
#include "token.h"
#include "scanner.h"
#include "SymbolTable.h"

typedef int stmttype;
typedef int optype;
typedef int syntaxbranch;


enum stmttypes { EMPTYSTMT, ASSIGNSTMT, PRINTSTMT, CONDITSTMT,
	ITERSTMT, NUMSTMTTYPES };

enum operators { PLUS, MINUS, TIMES, MODOP, DIVOP, NUMOPS };

enum syntaxbranches { BLOCK, DECLARATION, STMTORBLOCK, STMT,
	BOOLEXPR, BOOLTERM, BOOLFACTOR, RELATEXPR, INTEXPR, TERM,
	FACTOR, NUMBRANCHES };  //used for building first( )

bool err_flag; //if this becomes true, processing stops

struct Block {

	//begin token

	struct Declaration *decp;
	struct StatementOrBlock *stmtorblp;

	bool hasstmtorbl; //false if the block was only declaration(s), added 11/26/15

	struct symboltable *symtablep;

	//end token
};

struct Declaration {

	//var token

	char identifier [MAXIDLEN+1]; // name of identifier
	//char *identifier;

	//: token

	vartype type; //specified in enum

	//; token

	bool hasnext;
	struct Declaration *declp;
};

//Type removed for simplification
//StatementOrBlocks removed for simplification

struct StatementOrBlock {

	bool isstatement; //true if statement; false if block

	struct Statement *stmtp;
	struct Block *blockp;

	//; token if (hasnext)	

	bool hasnext;
	struct StatementOrBlock *stmtorblp;
};

//all statement types condensed into one for simplification
struct Statement {

	stmttype statementtype;

	//emptystmt has "skip" token
	//other types may have print, if, else, etc. tokens

	char identifier [MAXIDLEN+1]; // name of identifier
	//char *identifier;

	struct BooleanExpression *boolexprp;
	struct StatementOrBlock *stmtorblockp1;
	struct StatementOrBlock *stmtorblockp2;
};


/*struct EmptyStatement {
};

struct AssignmentStatement {
};

struct PrintStatement {
};

struct ConditionalStatement {
};

struct IterativeStatement {
};*/


struct BooleanExpression {

	struct BooleanTerm *booltermp;

	//vartype mytype; //used during evaluation
};

struct BooleanTerm {

	struct BooleanFactor *boolfactorp;

	//vartype mytype; //used during evaluation

	bool hasnext; //true if this term followed by "or"
	struct BooleanTerm *nextbooltermp;

};

struct BooleanFactor {

	bool negate; //true if there is a "not" token

	bool hasRelExpr; //true if need to evaluate relational expression
	bool value; //may be true, false, or returned from the relational
		//expression

	//vartype mytype; //used during evaluation

	struct RelationalExpression *relexprp;

	bool hasnext; //true if this factor followed by "and"
	struct BooleanFactor *nextboolfactorp;
};

struct RelationalExpression {

	struct IntegerExpression *intexprp1;

	bool hasrelop; //true if using relational operator
	Relop therelop; //relational operator. enum in token.h

	struct IntegerExpression *intexprp2;
};

//relationaloperator is a single token
//removed for simplification

struct IntegerExpression {

	//vartype mytype; //used during evaluation
	struct Term *termp;
};

struct Term {

	struct Factor *factorp;

	//vartype mytype; //used during evaluation

	bool hasnext; //true if has next term
	optype myop; //+ or -; as in, this myop *nexttermp
	struct Term *nexttermp;
};

struct Factor {

	bool negate; //true if has minus token

	//vartype mytype; //used during evaluation

	bool hasid; //true if identifier
	char identifier [MAXIDLEN+1]; // name of identifier
	//char *identifier;

	bool hasuint; //true if unsigned int
	int uintval; //value of the unsigned int

//**********should be IntegerExpression?**********************************************
	struct BooleanExpression *boolexprp;

	bool hasnext; //true if has next factor
	optype myop; //*, mod, or div; as in, this myop *nextfactorp
	struct Factor *nextfactorp;
};

//Returns a bitvector containing the legal first tokens of each
// line in the syntax.
struct bitvector first (syntaxbranch br);

//helper to printing error diagnostics (prints what was found)
void printfound ( );

//Attempts to parse a block beginning with the current token, and
// returns the AST, whose "head" node is a struct Block.
struct Block* parseblock ( );

//The following methods are parts of the parsing process which build
// the appropriate struct for the AST and return it, while checking
// for parsing errors relating to that part of the AST.

struct Block* pblock ( );

struct Declaration* pdeclaration ( );

struct StatementOrBlock* pstmtorblock ( );

struct Statement* pstmt ( );

struct BooleanExpression* pboolexpr ( );

struct BooleanTerm* pboolterm ( );

struct BooleanFactor* pboolfactor ( );

struct RelationalExpression* prelatexpr ( );

struct IntegerExpression* pintexpr ( );

struct Term* pterm ( );

struct Factor* pfactor ( );

#endif // BLOCKPARSER_H


