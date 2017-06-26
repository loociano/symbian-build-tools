%{
// RCOMP.CPP
// Generated from RCOMP.Y
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

#include <assert.h>
#include <ctype.h>
#include <fstream.h>
#include "resource.h"
#include "rcstack.h"
#include "rcompl.hpp"
#include "rcomp.hpp"
#include "datatype.h"
#include "mem.h"
#include "rcbinstr.h"
#include "indextab.h"
#include "linkarra.h"
#include "numval.h"
#include "rcscan.h"
#include "errorhan.h"
#include "fileacc.h"
#include "version.h"
#include "ctable.h"
#include "main.h"

#if defined(__VC32__) && !defined(_DEBUG)
#pragma warning( disable : 4702 )	// unreachable code
#pragma warning( disable : 4102 )	// 'yyerrlabel' : unreferenced label
#pragma warning( disable : 4244 )	// '=' : conversion from 'int' to 'short', possible loss of data
#endif



String::CharacterSet CharacterSetID( const String & character_set_name );
void asUTF8(char* aUtf8, int aUnicode);
void SetIdFromName( const String & NameStatementValue);


unsigned short & d = MemCheckControl::iLogMemory;

StructHeader *		pSH;

StructHeaderArray * 	pSHA;	// Used in resource struct handling functions.
ResourceHeader *	pResourceHeader;
ResourceItemArray *	pCurrentRIA;
int			verbose;
String::CharacterSet	SourceCharacterSet = String::CP1252;
String::CharacterSet	TargetCharacterSet = String::CP1252;
unsigned short		logmemorysetting;
int *			pCurrentLineNumber;
FileLineManager *	pFileLineHandler;
NameIdMap *		pResourceNameIds;
LinkArray *		pLinks;
long			CurrentEnumValue;
String			CurrentEnumName;
char			TempStr[300];


int CurrentIdStep=1;
long CurrentId=0;
int FormatIdAsHex=0;	// defaults to decimal, changes in SetIdFromName


const String	Divider("*******************************************");

#define REGISTER_LINE	ErrorHandler::Register(pFileLineHandler->GetCurrentFile(), pFileLineHandler->GetErrorLine(* pCurrentLineNumber))

%}

%token <Id> L_STRUCT L_RESOURCE L_NAME L_OFFSET L_SYSTEM L_GLOBAL L_LOCAL L_CHARACTER_SET
%token <Id> L_BUF L_WORD L_BYTE L_LONG L_DOUBLE L_TEXT L_LTEXT L_LINK L_LLINK L_SRLINK
%token <Id> L_BUF8, L_TEXT8, L_LTEXT8, L_BUF16, L_TEXT16, L_LTEXT16
%token <Value> L_LABEL L_NUM_NATURAL L_NUM_FLOAT L_NATURAL_EXPR L_ENUM
%token <Value> L_LEN
%token <Value> L_CHAR_LITERAL L_STRING_LITERAL

%type <Id>			data-type len-declaration
%type <pStructItem>		struct-item array-struct-item simple-struct-item
%type <pSimpleStructItem>	simple-struct-item-start
%type <pStructItem>		struct-type-struct-item  struct-array-struct-item
%type <pArrayStructItem>	array-struct-item-start array-struct-item-base
%type <pStructArrayStructItem>	struct-array-struct-item-base
%type <Value>			simple-initialiser natural-expression
%type <Value>			character-code-expression string-expression string-expression-item
%type <pStringArray>		simple-initialiser-list
%type <NumInitialiser>		natural-expression-numeric

%left	'+' '-'
%left	'*' '/'
%left	'|'
%left	UMINUS

%union {
	char				Value[1024];
	unsigned long 			Id;
	StructItem *			pStructItem;
	SimpleStructItem *		pSimpleStructItem;
	ArrayStructItem *   	pArrayStructItem;
	StructArrayStructItem *	pStructArrayStructItem;
	StringArray *			pStringArray;
	long					NumInitialiser;
}

%%

/*****************************************************************/
/* TOP-MOST RULE                                                 */
/*****************************************************************/
source :	statement-list				{	if(verbose)	{	MOFF; cout << Divider << "\n" << Divider << endl; MON; }
										}
;
/*****************************************************************/
/* statement-list and statement                                  */
/*****************************************************************/
statement-list:
	statement-list statement
|	/* Nothing */
;
statement:
	struct-statement
|	resource-statement
|   character_set-statement
|	name-statement
|	offset-statement
|	system-statement
|   enum-statement
;

/***********************************************************************************/
/* STRUCT statement                                                                */
/***********************************************************************************/
struct-statement:
	struct-statement-start struct-item-list '}'
										{	if(verbose) { MOFF; cout << Divider << "\n" << * pSH << Divider << endl; MON;}	}
;
struct-statement-start:
	L_STRUCT L_LABEL '{'				{	if(verbose) {	MOFF;cout << "struct-statement-start                     " << $2 << endl; MON;}
											pSH = new StructHeader($2);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										}
|	L_STRUCT L_LABEL len-declaration '{'
										{	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct-statement-start                     " << $2 << " " << Types.GetName($3) << endl; MON;}
											pSH = new StructHeader($2, $3);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										}
|	L_STRUCT L_LABEL L_LEN '{'
										{	if(verbose) {	MOFF;cout << "struct-statement-start                     " << $2 << " (WORD)" << endl; MON;}
											pSH = new StructHeader($2, L_WORD);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										}
;
struct-item-list:
	struct-item-list struct-item ';'	{	if(verbose) {	MOFF;cout << "struct-item-list                           Adding struct-item." << endl; MON;}
											REGISTER_LINE;
											pSH->iSIA.Add($2);
										}
|	/* Nothing */
;
struct-item:
	simple-struct-item
|	array-struct-item
|	struct-type-struct-item
|	struct-array-struct-item
;
simple-struct-item:
	simple-struct-item-start			{	$$ = $1;}
|	simple-struct-item-start '(' natural-expression ')'
										{	if(verbose) { MOFF;cout << "    Limit: " << $3 << endl; MON;}
											$1->iLengthLimit = $3;
											$$ = $1;
										}
|	simple-struct-item-start '=' simple-initialiser	/****************************************************************/
										{	if(verbose) { MOFF;cout << "    Default: " << $3 << endl; MON;}
											$1->iDefault = $3;
											$$ = $1;
										}
|	simple-struct-item-start '(' natural-expression ')' '=' string-expression /****************************************************************/
										{	if(verbose) { MOFF;cout << "    Limit: " << $3 << ", Default: " << $6 << endl; MON;}
											NumericValue Limit($3, L_LONG);
											if(String($6).ExportLength(TargetCharacterSet,SourceCharacterSet) > Limit.GetULong() )
											{
												REGISTER_LINE;
												ErrorHandler::OutputErrorLine("Text length exceeds specified limit");
												exit(1);
											}
											$1->iLengthLimit = $3;
											$1->iDefault = $6;
											$$ = $1;
										}

;
simple-struct-item-start:
	data-type L_LABEL					{	if(verbose) 
											{			   RCTypeArray Types;
														   MOFF;cout << "simple-struct-item                         " << Types.GetName($1) << " " << $2 << endl; MON;
											}
											$$ = new SimpleStructItem($1,$2); 
											assert($$ != NULL);
										}
|	data-type '<' natural-expression-numeric '>' L_LABEL
										{	if(verbose) 
											{			   RCTypeArray Types;
														   MOFF;cout << "simple-struct-item                         " << Types.GetName($1) << " " << $5 << endl; MON;
											}
											String s(NumericValue::ltoa($3));
											$$ = new SimpleStructItem($1,$5,s);
											assert($$ != NULL);
										}
;

/* Note that generic text identifiers are converted to their explicit
   8 or 16-bit forms at this point, depending on the target character set.
*/
data-type:
	L_BYTE								{	$$ = L_BYTE;}
|	L_WORD								{	$$ = L_WORD;}
|	L_LONG								{	$$ = L_LONG;}
|	L_DOUBLE							{	$$ = L_DOUBLE;}



|	L_TEXT	
	    { 
	    $$ = ( TargetCharacterSet == String::Unicode ) ? L_TEXT16: L_TEXT8;
	    REGISTER_LINE;
	    ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT - use LTEXT instead");
	    }
|	L_LTEXT	
	    {
	    $$ = ( TargetCharacterSet == String::Unicode ) ? L_LTEXT16: L_LTEXT8;
	    }
|	L_BUF	
	    { 
	    $$ = ( TargetCharacterSet == String::Unicode ) ? L_BUF16: L_BUF8;
	    }



|	L_TEXT8								{	$$ = L_TEXT8;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT8 - use LTEXT8 instead");
										}
|	L_TEXT16							{	$$ = L_TEXT16;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT16 - use LTEXT16 instead");
										}
|	L_LTEXT8							{	$$ = L_LTEXT8;}
|	L_LTEXT16							{	$$ = L_LTEXT16;}
|	L_BUF8								{	$$ = L_BUF8;}
|	L_BUF16								{	$$ = L_BUF16;}
|	L_LINK								{	$$ = L_LINK;}
|	L_LLINK								{	$$ = L_LLINK;}
|	L_SRLINK							{	$$ = L_SRLINK;}
;
array-struct-item:
	array-struct-item-base				{	$$ = $1;}
|	array-struct-item-base '=' '{' simple-initialiser-list '}'
										{	if(verbose) {	MOFF;cout << "array-struct-item                          with simple-initialiser-list" << endl;MON;}
											$1->iDefaults = * $4;
											if($1->iSize.Length() > 0)
											{
												NumericValue v($1->iSize, L_LONG);
												REGISTER_LINE;
												if($4->Size()!=long(v.GetULong()))
												{
													ErrorHandler::OutputErrorLine("Size does not match number of initialisers");
													exit(1);
												}
											}
											$$ = $1;
											delete $4;
										}
;
array-struct-item-base:
	array-struct-item-start ']'			{	if(verbose) {	MOFF;cout << "array-struct-item-base                     with no size" << endl;MON;}
											$$ =$1;
										}
|	array-struct-item-start natural-expression ']'
										{	if(verbose) {	MOFF;cout << "array-struct-item-base                     with size " << $2 << endl;MON;}
											$1->iSize = $2;
											$$ = $1;
										}
|	L_LEN len-declaration array-struct-item-start ']'
										{	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array-struct-item-base                     with LenType " << Types.GetName($2) << endl;MON;
												}
											$3->iLenType = $2;
											$$ = $3;
										}
|	L_LEN len-declaration array-struct-item-start natural-expression ']'
										{	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array-struct-item-base                     with size " << $4 << " and LenType " << Types.GetName($2) << endl;MON;
												}
											$3->iLenType = $2;
											$3->iSize = $4; 
											$$ = $3; 
										}
;
array-struct-item-start:
	data-type L_LABEL '['				{	if(verbose) 
												{		 	RCTypeArray Types; 
														 	MOFF;cout << "array-struct-item-start                    " << Types.GetName($1) << " " << $2 << endl;MON;
												}
											$$ = new ArrayStructItem($1, $2);
										}
;
len-declaration:
	L_BYTE								{	$$ = L_BYTE;}
|	L_WORD								{	$$ = L_WORD;}
;
struct-type-struct-item:
	L_STRUCT L_LABEL					{	if(verbose) {	MOFF;cout << "struct-type-struct-item                    " << $2 << endl;MON;}
											$$ = new StructTypeStructItem($2);
										}
;
struct-array-struct-item:
	struct-array-struct-item-base		{	$$ = $1;}
|	L_LEN len-declaration struct-array-struct-item-base
										{	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct-array-struct-item                   - Setting Size to " << Types.GetName($2) << endl;MON;}
											$3->iLenType = $2; $$ = $3;
										}
;
struct-array-struct-item-base:
	L_STRUCT L_LABEL '[' ']'			{	if(verbose) {	MOFF;cout << "struct-array-struct-item-base              " << $2 << endl;MON;}
											$$ = new StructArrayStructItem($2);
										}
|	L_STRUCT L_LABEL '[' natural-expression ']'
										{	if(verbose) {	MOFF;cout << "struct-array-struct-item-base              " << $2 << " " << $4 << endl;MON;}
											$$ = new StructArrayStructItem($2, $4);
										}
;
/***********************************************************************************/
/* RESOURCE statement                                                              */
/***********************************************************************************/
resource-statement:
	resource-statement-start '{' resource-item-list '}'
	    {	
	    pResourceHeader->AddDefault();
	    CurrentId+=CurrentIdStep;
	    if(verbose) { MOFF;cout << "Resource ID "<< CurrentId << endl << Divider << "\n" << * pResourceHeader << Divider << endl;MON;}
	    pResourceHeader->SetResourceId(*pResourceNameIds,CurrentId,FormatIdAsHex);
	    pG->Index.Add(pResourceHeader);
	    pResourceHeader = NULL;
	    }
;
resource-statement-start:
	L_GLOBAL resource-statement-start-names		/* Ignore GLOBAL (obsolete feature).*/
|	L_LOCAL  resource-statement-start-names
	    {	
	    if(verbose) { MOFF;cout << "resource-statement-start                   LOCAL" << endl;MON;}
		    assert(pResourceHeader != NULL);
		    pResourceHeader->iLocal = 1;
	    }
|	resource-statement-start-names
;
resource-statement-start-names:
	L_RESOURCE L_LABEL L_LABEL			{	if(verbose) {	MOFF;cout << "resource-statement-start-names             " << $2 << " " << $3 << endl;MON;}
											assert(pResourceHeader == NULL);
											pResourceHeader = new ResourceHeader($3);
											pCurrentRIA = & (pResourceHeader->iRIA);
											REGISTER_LINE;
											if(pResourceNameIds->IsStored($3))
											{
												ErrorHandler::OutputErrorLine("Resource with this name encountered already");
												exit(1);
											}
											pCurrentRIA->FillFromStruct($2);
										}
|	L_RESOURCE L_LABEL					{	if(verbose) {	MOFF;cout << "resource-statement-start-names             " << $2 << " <Resource not named>" << endl;MON;}
											assert(pResourceHeader == NULL);
											pResourceHeader = new ResourceHeader;
											pCurrentRIA = & (pResourceHeader->iRIA);
											REGISTER_LINE;
											pCurrentRIA->FillFromStruct($2);
										}
;
resource-item-list:
	resource-item-list resource-item ';'{	if(verbose) {	MOFF;cout << "resource-item-list" << endl;MON;}}
|	resource-item-list error ';'		{	yyerrok(); yyclearin(); }
|	/* Nothing */
;
resource-item:
	L_LABEL '=' simple-initialiser		{	if(verbose) {	MOFF;cout << "resource-item                              " << $1 << " " << $3 << endl;MON;}
											REGISTER_LINE;/****************************************************************/
											pCurrentRIA->Set($1, $3);
										}
|	resource-simple-array-item
|	struct-resource-item
|	struct-array-resource-item
;
resource-simple-array-item:
	L_LABEL '=' '{' '}'                 
	    {	
	    if (verbose) 
		{ MOFF;cout << "resource-simple-array-item                 " << $1 << endl;MON;} 
	    }
|	L_LABEL '=' '{' simple-initialiser-list '}'
	    {	
	    if (verbose) 
		{ MOFF;cout << "resource-simple-array-item                 " << $1 << " with simple-initialiser-list" << endl;MON;}
	    REGISTER_LINE;
	    pCurrentRIA->Set($1, * $4);
	    delete $4;
	    }
;

/*---------------------------------------------------------------------------------*/
/* A note about RIAStack, SRIStack and pCurrentRIA.                                */
/*                                                                                 */
/*     RIA stands for Resource Item Array.                                         */
/*     SRI stands for Struct Array Resource Item.                                  */
/*                                                                                 */
/*     A push to RIAStack is made when dropping inside a STRUCT or STRUCT[] in     */
/*     order to set values for the components. When this happens pCurrentRIA is    */
/*     set to the RIA for the STRUCT or last item of the STRUCT[].                 */
/*                                                                                 */
/*     pCurrentRIA is set to the result of popping from RIAStack when a closing    */
/*     brace is reached.                                                           */
/*                                                                                 */
/*     A push is made to SRIStack when going into an item for a STRUCT[]. On       */
/*     reaching a closing brace the STRUCT[] is popped off the SRIStack. An new    */
/*     item may then be added to this array.                                       */
/*---------------------------------------------------------------------------------*/

struct-resource-item:
	struct-resource-item-start resource-item-list '}'
										{	if(verbose) {	MOFF;cout << "struct-resource-item" << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										}
;
struct-resource-item-start:
	L_LABEL '=' L_LABEL '{'	{	if(verbose) {	MOFF;cout << "struct-resource-item-start                 " << $1 << " " << $3 << endl;MON;}
											REGISTER_LINE;
											pCurrentRIA->Set($1, $3);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = pCurrentRIA->Find($1)->GetRIA();
										}
;
struct-array-resource-item:
	struct-array-resource-item-start struct-array-resource-item-list-top '}'
										{	if(verbose) {	MOFF;cout << "struct-array-resource-item" << endl;MON;}
											pG->SRIStack.Pop();
										}
|	struct-array-resource-item-start struct-array-resource-item-list-top error
										{	pG->SRIStack.Pop();}
;
struct-array-resource-item-start:
	L_LABEL '=' '{'	L_LABEL '{'			{	if(verbose) {	MOFF;cout << "struct-array-resource-item-start           " << $1 << " " << $4 << endl;MON;}
											ResourceItem * p = pCurrentRIA->Find($1);
											pG->SRIStack.Push(p);
											REGISTER_LINE;
											p->Set($4);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										}
;
struct-array-resource-item-list-top:
	struct-array-resource-item-list-top-start
|	struct-array-resource-item-list-top-start ',' struct-array-resource-item-list
|	struct-array-resource-item-list-top-start ',' error
;
struct-array-resource-item-list-top-start:
	resource-item-list '}'				{	if(verbose) {	MOFF;cout << "struct-array-resource-item-list-top        " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										}
;
struct-array-resource-item-list:
	struct-array-resource-item-list-item
|	struct-array-resource-item-list ',' struct-array-resource-item-list-item
;
struct-array-resource-item-list-item:
	struct-array-resource-item-list-item-start resource-item-list '}'
										{	if(verbose) {	MOFF;cout << "struct-array-resource-item-list-item       " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										}
;
struct-array-resource-item-list-item-start:
	L_LABEL '{'							{	if(verbose) {	MOFF;cout << "struct-array-resource-item-list-item-start " << $1 << endl;MON;}
											ResourceItem * p = pG->SRIStack.Peek();
											REGISTER_LINE;
											p->Set($1);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										}
;


/*****************************************************************/
/* simple-initialiser and simple-initialiser-list                */
/*****************************************************************/
simple-initialiser:
	L_NUM_FLOAT
|	L_CHAR_LITERAL  
	    { 
	    // convert literal to unsigned long value of 1st character
	    String s($1);
	    wchar_t first;
	    int length=1;
	    if (s.Export(&first, length, SourceCharacterSet)==0)
		{
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("Warning: Ignoring trailing characters in character literal");
		}
	    _ltoa(first,$$,10);
	    }
|	L_LABEL 
	    {
	    if (pG->EnumValues.IsStored($1))
			{
			_ltoa(pG->EnumValues.FindId($1),$$,10);
			}
		else
			{
			/*
			Could be a reference to another resource, perhaps even a forward reference:
			the OverwriteLink functions do FindId again when writing out the data.
			Sadly this also permits things which are really syntax errors, inadvertently
			converting labels into string literals..
			*/
			}
	    }
|	string-expression
|	natural-expression
;
simple-initialiser-list:
	simple-initialiser  
	    {
	    if(verbose) 
		{	
		MOFF;cout << "simple-initialiser-list                    - single string " << $1 << endl;MON;
		}
	    $$ = new StringArray;
	    $$->Add(new String($1) );
	    }
|	simple-initialiser-list ',' simple-initialiser	
										{	if(verbose) {	MOFF;cout << "simple-initialiser-list                    - part of list " << $3 << endl;MON;}
											assert($1 != NULL);
											$1->Add(new String($3 ) );
											$$ = $1;
										}
;

natural-expression:
	natural-expression-numeric			{	String s(NumericValue::ltoa($1) ); strcpy($$, s.GetBuffer() ); }
;	
natural-expression-numeric:
	L_NUM_NATURAL						{	if(verbose) {	MOFF;cout << "Converting number " << $1 << endl;MON;}
											REGISTER_LINE;
											NumericValue v($1, L_LONG); $$ = v.GetLong();
										}
|	natural-expression-numeric '+' natural-expression-numeric	{	$$ = $1 + $3;	}
|	natural-expression-numeric '-' natural-expression-numeric	{	$$ = $1 - $3;	}
|	natural-expression-numeric '*' natural-expression-numeric	{	$$ = $1 * $3;	}
|	natural-expression-numeric '/' natural-expression-numeric	{	$$ = $1 / $3;	}
|	natural-expression-numeric '|' natural-expression-numeric	{	$$ = $1 | $3;	}
|	'-' natural-expression-numeric %prec UMINUS					{	$$ = - $2;		}
|	'(' natural-expression-numeric ')'							{	$$ = $2;		}
;
string-expression:
	string-expression-item  
|	string-expression-item string-expression {
	    if (strlen($$)+strlen($2) > sizeof($$)-1)
		{
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("String expression is too long");
		exit(1);
		}
	    strcat($$, $2);
	    }
;
string-expression-item:
	L_STRING_LITERAL
|	character-code-expression
;
character-code-expression:
	'<' natural-expression-numeric '>'  
	    {	
	    REGISTER_LINE;
	    if($2 < 0 || ($2 > 255 && TargetCharacterSet != String::Unicode))
		{
		    ErrorHandler::OutputErrorLine("Character code must be a number in the range 0 to 255.");
		    exit(1);
		}
	    if($2 > 0xffff)
		{
		    ErrorHandler::OutputErrorLine("Unicode character code must be a number in the range 0 to 65535.");
		    exit(1);
		}
	    if (TargetCharacterSet != String::Unicode)
		{
		* $$ = char($2); * ($$ + 1) = '\0'; 
		} 
	    else
		{
		if (SourceCharacterSet == String::CP1252)
		    {
		    if ( ($2 >= 0x80) && ($2 <= 0x9F ) ) // 80-9F are illegal Unicode values.
			{
			ErrorHandler::OutputErrorLine("Warning: Deprecated non-unicode value in source stream");
			}
		    * $$ = char(UnicodeEscape);
		    asUTF8($$ + 1, $2);
		    }
		else
		if (SourceCharacterSet == String::UTF8)
		    {
		    asUTF8($$, $2);
		    }
		else
		    {
		    // Unsatisfactory, but do people use other character sets?
		    if ($2 > 255)
			{
			ErrorHandler::OutputErrorLine("Don't know how to handle character > 255");
			}
		    * $$ = char($2); * ($$ + 1) = '\0'; 
		    }
		}
	    }
;


/*****************************************************************/
/* name-statement                                                */
/*****************************************************************/
name-statement:
	L_NAME L_LABEL
	    {
	    REGISTER_LINE;
	    SetIdFromName($2);
	    }
|	L_NAME L_STRING_LITERAL
	    {
	    REGISTER_LINE;
	    SetIdFromName($2);
	    }
;


/*****************************************************************/
/* character_set-statement                                       */
/* Defines the SOURCE character set. Note that Unicode is a      */
/* character set id, but we can't read Unicode source            */
/* (because LEX and YACC can't handle it)                        */
/*****************************************************************/

character_set-statement:
	L_CHARACTER_SET L_LABEL		{	if(verbose) {	MOFF;cout << "character_set-statement                    " << $2 << endl;MON;}
											REGISTER_LINE;
											SourceCharacterSet = CharacterSetID($2);
											if ( SourceCharacterSet == String::UNKNOWN )
											{
												String err = "Warning: Unrecognised character set name '";
												err += $2;
												err += "'";
												ErrorHandler::OutputErrorLine(err);
											}
											if ( SourceCharacterSet == String::Unicode )
											{
											    SourceCharacterSet = String::UNKNOWN;
												ErrorHandler::OutputErrorLine("Unicode source is unsupported");
											}
										}
;



/*****************************************************************/
/* offset-statement                                              */
/*****************************************************************/
offset-statement:
	L_OFFSET natural-expression			{	if(verbose) {	RCTypeArray Types;
															MOFF;cout << "offset-statement                           " << $2 << endl;MON; }
											REGISTER_LINE;
										 	CurrentId=((long) NumericValue($2, L_LONG).GetULong() );
										}
;

/*****************************************************************/
/* system-statement                                              */
/*****************************************************************/
system-statement:
	L_SYSTEM							{	if(verbose) {	MOFF;cout << "system-statement" << endl;MON;}
											CurrentIdStep=-1;
										}
;

/*****************************************************************/
/* enum-statement                                                */
/*****************************************************************/
enum-statement:
    enum-statement-start enum-list '}'
|	enum-statement-start enum-list '}' ';'
;
enum-statement-start:
	L_ENUM L_LABEL '{'					 
	    {	
	    if(verbose) 
		{ MOFF;cout << "enum-statement" << endl;MON;} 
	    CurrentEnumName = $2;
	    CurrentEnumValue=0;
	    }
|	L_ENUM '{'						
	    {	
	    if(verbose) 
		{ MOFF;cout << "enum-statement" << endl;MON;} 
	    CurrentEnumName = "";
	    CurrentEnumValue=0;
	    }
;

enum-list-entry:
	L_LABEL                             {	pG->EnumValues.Add($1, CurrentEnumValue++);}
|	L_LABEL '=' simple-initialiser      
	    {	
	    CurrentEnumValue = atol($3);
	    pG->EnumValues.Add($1, CurrentEnumValue);
	    CurrentEnumValue++;			// Increment so that next field has value ($3+1)
	    }
;


enum-list:
		enum-list-entry{}
|		enum-list ',' enum-list-entry{}

;

%%

// Function section
// ================

void asUTF8(char* aUtf8, int aUnicode)
	{
	if ((aUnicode & 0xff80) == 0x0000)
		{
		*aUtf8 = (char)aUnicode;
		}
	else if ((aUnicode & 0xf800) == 0x0000)
		{
		*aUtf8++ =(char)(0xc0|(aUnicode>>6));
		*aUtf8   =(char)(0x80|(aUnicode&0x3f));
		}
	else
		{
		*aUtf8++ =(char)(0xe0|(aUnicode>>12));
		*aUtf8++ =(char)(0x80|((aUnicode>>6)&0x3f));
		*aUtf8   =(char)(0x80|(aUnicode&0x3f));
		}
	*++aUtf8 = '\0';
	}


String::CharacterSet CharacterSetID( const String & character_set_name )
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Return a character set ID from a character set name.  The value UNKNOWN
// is returned if the name is not recognised.
// ----------------------------------------------------------------------------
{
	String::CharacterSet ids[] = { String::ISOLatin1, String::ASCII, String::CP1252
	                              , String::CP850, String::ShiftJIS, String::Unicode
								  , String::UTF8
								  , String::UNKNOWN
								 };
	String names[] = { "ISOLATIN1", "ASCII", "CP1252", "CP850", "SHIFTJIS", "UNICODE", "UTF8" };

	for ( int i=0; ids[i]!=String::UNKNOWN; i++ )
	{
		if ( names[i] == character_set_name ) return ids[i];
	}

	return String::UNKNOWN;

} // end of CharacterSetID code

void SetIdFromName( const String & NameStatementValue)
	{
	// space 	0
	// A		1
	// B		2
	// ...
	// Z		26
	//
	// ABCD corresponds to the number 4321 which becomes ( (4*27 + 3) * 27 + 2) * 27 + 1.
	
	if(verbose) 
		{ MOFF;cout << "name-statement                             " << NameStatementValue << endl;MON;}
	if ( NameStatementValue.Length() > 4)
		{
		ErrorHandler::OutputErrorLine( "Name must be no longer than four characters");
		exit( 1);
		}
	
	long NewId = 0;
	
	for( unsigned long i = 0; i < NameStatementValue.Length(); i++)
		{
		NewId *= 27;
		if ( isalpha( NameStatementValue[i]) )
			NewId += toupper( NameStatementValue[i]) - 'A' + 1;
		}

	CurrentId = NewId << 12;
	FormatIdAsHex = 1;
	if(verbose) 
		{ MOFF;cout << "Current id                                " << CurrentId << endl;MON;}
	}


int ParseSourceFile(FILE* aFile, unsigned short aYYDebug)
	{
	// Set up various global pointers which refer to the pG structure
	pSHA = & (pG->SHA);
	pFileLineHandler = & (pG->FileLineHandler);
	pResourceNameIds = & (pG->ResourceNameIds);
	pLinks = & (pG->Links);

	rcscan * pScan = new rcscan(pG->FileLineHandler, aFile);
	yy_parse * pParse = new yy_parse(1000);

	if (pScan==NULL || pParse==NULL)
		return -4;	// KErrNoMemory;

	pParse->yydebug = aYYDebug;
	pCurrentLineNumber = & pScan->yylineno;
	
	int ReturnValue = pParse->yyparse(pScan);

	int bScanErrorFound = pScan->ErrorWasFound();

	delete pScan;
	delete pParse;

	if(ReturnValue != 0)
		return ReturnValue;
	
	if(bScanErrorFound)
		return 1;
	
	return 0;	// successful parse - parse tree now in the pG data structure
	}


