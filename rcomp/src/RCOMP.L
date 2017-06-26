%{
// RCOMPL.CPP
// Generated from RCOMP.L
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <limits.h>
#include "structst.h"
#include "rcompl.hpp"
#include "rcomp.hpp"
#include "fileline.h"

#define VALUE_LEN   1024	// must match sizeof(YYSTYPE.Value)
char buf[VALUE_LEN];
char * pCh;
#define CHECK_APPEND(x)	\
    if (pCh-buf==VALUE_LEN-1) { yyerror("string too long - %c ignored", (x)); } else {*pCh++ = (x); }

int isCharLiteral;

YYSTYPE yylval;

extern String		InputBaseName;
extern FileLineManager* pFileLineHandler;
extern int*		pCurrentLineNumber;
char RealLineNumber[200];

%}
    
Digit [0-9]
Lower [a-z]
Upper [A-Z]
Letter {Lower}|{Upper}
Alphanumeric {Letter}|{Digit}
AlphanumUscore {Alphanumeric}|_
HexChar [A-Fa-f0-9]
ExpChar [eE]
FileNameChar [^"]


%s string_rules	/* Rule set for string literals.								*/
				/* n.b. Exclusive rule sets i.e. %x are available in MKS only	*/
				/* so they are not used here; hence all the <0>'s.              */
				
%s file_line_rules	/* Rule set for file_line_directive.*/

%s cpp_comment	/* C++ comment to end of line */
%s c_comment	/* C comment */

%%


/* Translations section */
/* ==================== */

/*******************************************/
/* Main keywords                           */
/*******************************************/
<0>STRUCT				return L_STRUCT;
<0>RESOURCE				return L_RESOURCE;
<0>NAME					return L_NAME;
<0>CHARACTER_SET			return L_CHARACTER_SET;
<0>OFFSET				return L_OFFSET;
<0>SYSTEM				return L_SYSTEM;
<0>GLOBAL				return L_GLOBAL;
<0>LOCAL				return L_LOCAL;
<0>ENUM					return L_ENUM;
<0>enum					return L_ENUM;


/*******************************************/
/* Types                                   */
/*******************************************/
<0>BUF					return L_BUF;
<0>BUF8					return L_BUF8;
<0>BUF16				return L_BUF16;
<0>WORD					return L_WORD;
<0>BYTE					return L_BYTE;
<0>LONG					return L_LONG;
<0>DOUBLE				return L_DOUBLE;
<0>TEXT					return L_TEXT;
<0>LTEXT				return L_LTEXT;
<0>TEXT8				return L_TEXT8;
<0>LTEXT8				return L_LTEXT8;
<0>TEXT16				return L_TEXT16;
<0>LTEXT16				return L_LTEXT16;
<0>LINK					return L_LINK;
<0>LLINK				return L_LLINK;
<0>SRLINK				return L_SRLINK;


/*******************************************/
/* Others                                  */
/*******************************************/
<0>LEN					return L_LEN;


/*******************************************/
/* String & character literals             */
/*******************************************/
<0>\"					{ BEGIN(string_rules); pCh = buf; isCharLiteral=0; }
<0>'					{ BEGIN(string_rules); pCh = buf; isCharLiteral=1; }

/* Escaped single- and double-quotes.*/
<string_rules>\\\"		{ CHECK_APPEND('"'); }
<string_rules>\\'		{ CHECK_APPEND('\''); };

/* Convert escaped character into corresponding actual character e.g. \t to tab. */
<string_rules>\\[rbfntv]	{ CHECK_APPEND( * ( strchr("\rr\bb\ff\nn\tt\vv\aa", yytext[1])-1));}

/* Escaped backslash */
<string_rules>\\\\			{ CHECK_APPEND('\\'); }

<string_rules>\\\n			/* Escaped newline ignored*/ ;

/* End of line before terminating double-quotes.*/
<string_rules>\n		{ yyerror( isCharLiteral?"Unterminated character literal":"Unterminated string"); BEGIN 0; }

/* End of string reached.*/
<string_rules>\"	{ 
    if (!isCharLiteral) 
	    {
	    *pCh = '\0'; BEGIN(0); strcpy( yylval.Value, buf); return L_STRING_LITERAL;
	    }
    CHECK_APPEND(*yytext);
    }

<string_rules>'		{ 
    if (isCharLiteral) 
	    {
	    *pCh = '\0'; BEGIN(0); strcpy( yylval.Value, buf); return L_CHAR_LITERAL;
	    }
    CHECK_APPEND(*yytext);
    }

/* Anything other than \n is stored.*/
<string_rules>.			{ CHECK_APPEND(*yytext); }


/*******************************************/
/* Labels                                  */
/*******************************************/
<0>{Letter}{AlphanumUscore}*    {	
    BEGIN(0);
    strcpy( yylval.Value, yytext);
    return L_LABEL;
    }

/*******************************************/
/* Numbers                                 */
/*******************************************/
<0>{Digit}+					{	strcpy( yylval.Value, yytext); return L_NUM_NATURAL; }
<0>0x{HexChar}+					{	strcpy( yylval.Value, yytext); return L_NUM_NATURAL; }
<0>{Digit}+\.{Digit}+{ExpChar}{Digit}+		{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<0>-{Digit}+\.{Digit}+{ExpChar}{Digit}+		{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<0>{Digit}+\.{Digit}+				{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<0>-{Digit}+\.{Digit}+				{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<0>{Digit}+{ExpChar}{Digit}+			{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}
<0>-{Digit}+{ExpChar}{Digit}+			{	strcpy( yylval.Value, yytext); return L_NUM_FLOAT;}

/*******************************************/
/* file_line_directive                     */
/*******************************************/
<0>^[#][ ]{Digit}+[ ]\"					{	BEGIN( file_line_rules); strcpy( RealLineNumber, yytext+2); }

<file_line_rules>\"$					{	BEGIN(0); // # <line> "" means start of base file.
											pFileLineHandler->SetBase( InputBaseName, * pCurrentLineNumber);
										}
										
<file_line_rules>{FileNameChar}*\"$		{	BEGIN(0); // # <line> <filename> means start of named base file.
											pFileLineHandler->SetInclude( yytext, * pCurrentLineNumber);
										}

<file_line_rules>{FileNameChar}*\"[ ]1([ ]3){0,1}$	{
											BEGIN(0); // # <line> <filename> 1 means start of an included file.
											pFileLineHandler->SetInclude( yytext, * pCurrentLineNumber);
										}

<file_line_rules>{FileNameChar}*\"[ ]2([ ]3){0,1}$	{
											BEGIN(0); // # <line> <filename> 2 means end of an included file and now at <line> in <filename>.
											pFileLineHandler->PostInclude( yytext, RealLineNumber, * pCurrentLineNumber);
										}



/*******************************************/
/* White space                             */
/*******************************************/
<0,file_line_rules>[ \t\n]	    ; // skipped

<0,file_line_rules>"//"		    { BEGIN(cpp_comment); }
<cpp_comment>\n			    { BEGIN(0); }
<cpp_comment>.			    ; // skipped

<0,file_line_rules>"/*"		    { BEGIN(c_comment); }
<c_comment>"*/"			    { BEGIN(0); }
<c_comment>\n			    ; // skipped
<c_comment>.			    ; // skipped

/*******************************************/
/* Special single characters               */
/*******************************************/
<0>[.{};=\[\],()+\-*/|<>]		return * yytext;


/*******************************************/
/* Everything else cannot be recognised    */
/*******************************************/
.                       { yyerror("*** Unknown character '%c' (value 0x%x) ", *yytext, *yytext);}
