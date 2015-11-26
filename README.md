# Interpreter
An interpreter for a simple calculator language.  It parses an input program, checks for errors, builds a syntax tree based on the input, and runs the program if correct.

            
Contact:
Jessica Hawkins
jessica.hawkins@utdallas.edu


For details on the p-language, see Language_Description.txt.

                   
How to compile and run:
-----------------------

The program can be compiled using gcc:

      gcc src/*.c src/*.h

and run in the command-line with the name of the input file as a command-line argument, for example:

      a.exe input/bad_else_if.p

The expected output of each input file is in the top of each input file as a comment.


.........................................................................
.........................................................................

Program details:
----------------

Reading file:

The token, position, scanner, and instreamreader files are used to read the input file.

** A "token" is the smallest piece of input to be processed, such as a variable name, an operator, "true", etc.  The token file defines the "token" struct as well as all of the keywords, special characters, relational operators, and types of tokens (classes.)

** The position file keeps track of the current line number and column number being read from the input file in a "position" struct.

** The "scanner" struct keeps track of the current position in the input file, the current token, and the next token to be processed.

** The "instreamreader" struct keeps track of the current character and next character to be read from the input file.  The instreamreader file contains methods to open and close an input stream to the input file.

..........................................................................

Parsing and building AST:

The blockparser and bitvector files are used to parse the input and attempt to build an abstract syntax tree based on the input.  The blockparser file defines the allowed syntax blocks in the p-language (see Language_Description.txt.)

The SymbolTable file defines the allowed variable types (bool, int, or num) and a struct which keeps track of variable definitions in the input file.

.........................................................................

Running:

buildASTinterpret.c contains the "main" method, which:

** Attempts to open the input file

** Calls the "parseblock" method in blockparser.c (If there are syntax/semantics errors, they will be found and printed.)

** Calls the "runBlock" method in interpreter.c  (If there are run-time errors, they will be found and printed.  Otherwise, runs the input program.)

