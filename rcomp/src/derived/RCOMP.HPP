const L_STRUCT = 257;
const L_RESOURCE = 258;
const L_NAME = 259;
const L_OFFSET = 260;
const L_SYSTEM = 261;
const L_GLOBAL = 262;
const L_LOCAL = 263;
const L_CHARACTER_SET = 264;
const L_BUF = 265;
const L_WORD = 266;
const L_BYTE = 267;
const L_LONG = 268;
const L_DOUBLE = 269;
const L_TEXT = 270;
const L_LTEXT = 271;
const L_LINK = 272;
const L_LLINK = 273;
const L_SRLINK = 274;
const L_BUF8 = 275;
const L_TEXT8 = 276;
const L_LTEXT8 = 277;
const L_BUF16 = 278;
const L_TEXT16 = 279;
const L_LTEXT16 = 280;
const L_LABEL = 281;
const L_NUM_NATURAL = 282;
const L_NUM_FLOAT = 283;
const L_NATURAL_EXPR = 284;
const L_ENUM = 285;
const L_LEN = 286;
const L_CHAR_LITERAL = 287;
const L_STRING_LITERAL = 288;
const UMINUS = 289;
typedef union {
	char				Value[1024];
	unsigned long 			Id;
	StructItem *			pStructItem;
	SimpleStructItem *		pSimpleStructItem;
	ArrayStructItem *   	pArrayStructItem;
	StructArrayStructItem *	pStructArrayStructItem;
	StringArray *			pStringArray;
	long					NumInitialiser;
} YYSTYPE;
extern YYSTYPE yylval;

// C++ YACC parser header
// Copyright 1991 by Mortice Kern Systems Inc.  All rights reserved.
//
// yy_parse => class defining a parsing object
//	yy_parse needs a class yy_scan, which defines the scanner.
// %prefix or option -p xx determines name of this class; if not used,
// defaults to 'yy_scan'
//
// constructor fills in the tables for this grammar; give it a size
//    to determine size of state and value stacks. Default is 150 entries.
// destructor discards those state and value stacks
//
// int yy_parse::yyparse(yy_scan *) invokes parse; if this returns,
//	it can be recalled to continue parsing at last point.
// void yy_parse::yyreset() can be called to reset the parse;
//	call yyreset() before yy_parse::yyparse(yy_scan *)
#include <stdio.h>		// uses printf(), et cetera
#include <stdlib.h>		// uses exit()

const YYERRCODE = 256;		// YACC 'error' value

// You can use these macros in your action code
#define YYERROR		goto yyerrlabel
#define YYACCEPT	YYRETURN(0)
#define YYABORT		YYRETURN(1)
#define YYRETURN(val)	return(val)

#if YYDEBUG
typedef struct yyNamedType_tag {	/* Tokens */
	char	* name;		/* printable name */
	short	token;		/* token # */
	short	type;		/* token type */
} yyNamedType;
typedef struct yyTypedRules_tag {	/* Typed rule table */
	char	* name;		/* compressed rule string */
	short	type;		/* rule result type */
} yyTypedRules;
#endif

#ifdef YACC_WINDOWS

// include all windows prototypes, macros, constants, etc.

#include <windows.h>

// the following is the handle to the current
// instance of a windows program. The user
// program calling yyparse must supply this!

extern HANDLE hInst;	

#endif	/* YACC_WINDOWS */


class yy_parse {
protected:

#ifdef YACC_WINDOWS

	// protected member function for actual scanning 

	int	win_yyparse(yy_scan * ps);	// parse with given scanner

#endif	/* YACC_WINDOWS */

	int	mustfree;	// set if tables should be deleted
	int	size;		// size of state and value stacks
	int	reset;		// if set, reset state
	short	yyi;		// table index
	short	yystate;	// current state

	short	* stateStack;	// states stack
	YYSTYPE	* valueStack;	// values stack
	short	* yyps;		// top of state stack
	YYSTYPE * yypv;		// top of value stack

	YYSTYPE yylval;		// saved yylval
	YYSTYPE	yyval;		// $
	YYSTYPE * yypvt;	// $n
	int	yychar;		// current token
	int	yyerrflag;	// error flag
	int	yynerrs;	// error count
#if YYDEBUG
	int	done;		// set from trace to stop parse
	int	rule, npop;	// reduction rule # and length
	short	* typeStack;	// type stack to mirror valueStack[]
	short	* yytp;		// top of type stack
	char	* yygetState(int);	// read 'states.out'
#endif
public:
#if YYDEBUG
	// C++ has trouble with initialized arrays inside classes
	static long * States;		// pointer to yyStates[]
	static yyTypedRules * Rules;	// pointer to yyRules[]
	static yyNamedType * TokenTypes; // pointer to yyTokenTypes[]
	static int	yyntoken;	// number of tokens
	static int	yynvar;		// number of variables (nonterminals)
	static int	yynstate;	// number of YACC-generated states
	static int	yynrule;	// number of rules in grammar

	char*	yyptok(int);		// printable token string
	int	yyExpandName(int, int, char *, int);
						// expand encoded string
	virtual int	yyGetType(int);		// return type of token
	virtual void	yyShowRead();		// see newly read token
	virtual void	yyShowState();		// see state, value stacks
	virtual void	yyShowReduce();		// see reduction
	virtual void	yyShowGoto();		// see goto
	virtual void	yyShowShift();		// see shift
	virtual void	yyShowErrRecovery();	// see error recovery
	virtual void	yyShowErrDiscard();	// see token discard in error
#endif
	yy_scan* scan;			// pointer to scanner
	int	yydebug;	// if set, tracing if compiled with YYDEBUG=1

	yy_parse(int = 150);	// constructor for this grammar
	yy_parse(int, short *, YYSTYPE *);	// another constructor

	~yy_parse();		// destructor

	int	yyparse(yy_scan * ps);	// parse with given scanner

	void	yyreset() { reset = 1; } // restore state for next yyparse()

	void	setdebug(int y) { yydebug = y; }

// The following are useful in user actions:

	void	yyerrok() { yyerrflag = 0; }	// clear error
	void	yyclearin() { yychar = -1; }	// clear input
	int	YYRECOVERING() { return yyerrflag != 0; }
};
// end of .hpp header
