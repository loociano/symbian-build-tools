/* t:\mks\bin\yacc -t -LC -P t:/mks/etc/yyparse.cpp -o rcomp.cpp -D rcomp.hpp ../rcomp.y */
#ifdef YYTRACE
#define YYDEBUG 1
#else
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#endif
#line 1 "../rcomp.y"

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

extern YYSTYPE yylval;
#if YYDEBUG
enum YY_Types { YY_t_NoneDefined, YY_t_Id, YY_t_Value, YY_t_pStructItem, YY_t_pSimpleStructItem, YY_t_pArrayStructItem, YY_t_pStructArrayStructItem, YY_t_pStringArray, YY_t_NumInitialiser
};
#endif
#if YYDEBUG
yyTypedRules yyRules[] = {
	{ "&00: %19 &00",  0},
	{ "%19: %20",  0},
	{ "%20: %20 %21",  0},
	{ "%20:",  0},
	{ "%21: %22",  0},
	{ "%21: %23",  0},
	{ "%21: %24",  0},
	{ "%21: %25",  0},
	{ "%21: %26",  0},
	{ "%21: %27",  0},
	{ "%21: %28",  0},
	{ "%22: %29 %30 &40",  0},
	{ "%29: &02 &26 &41",  0},
	{ "%29: &02 &26 %02 &41",  0},
	{ "%29: &02 &26 &31 &41",  0},
	{ "%30: %30 %03 &42",  0},
	{ "%30:",  0},
	{ "%03: %05",  3},
	{ "%03: %04",  3},
	{ "%03: %07",  3},
	{ "%03: %08",  3},
	{ "%05: %06",  3},
	{ "%05: %06 &43 %13 &44",  3},
	{ "%05: %06 &45 %12",  3},
	{ "%05: %06 &43 %13 &44 &45 %15",  3},
	{ "%06: %01 &26",  4},
	{ "%06: %01 &46 %18 &47 &26",  4},
	{ "%01: &12",  1},
	{ "%01: &11",  1},
	{ "%01: &13",  1},
	{ "%01: &14",  1},
	{ "%01: &15",  1},
	{ "%01: &16",  1},
	{ "%01: &10",  1},
	{ "%01: &21",  1},
	{ "%01: &24",  1},
	{ "%01: &22",  1},
	{ "%01: &25",  1},
	{ "%01: &20",  1},
	{ "%01: &23",  1},
	{ "%01: &17",  1},
	{ "%01: &18",  1},
	{ "%01: &19",  1},
	{ "%04: %10",  3},
	{ "%04: %10 &45 &41 %17 &40",  3},
	{ "%10: %09 &48",  5},
	{ "%10: %09 %13 &48",  5},
	{ "%10: &31 %02 %09 &48",  5},
	{ "%10: &31 %02 %09 %13 &48",  5},
	{ "%09: %01 &26 &49",  5},
	{ "%02: &12",  1},
	{ "%02: &11",  1},
	{ "%07: &02 &26",  3},
	{ "%08: %11",  3},
	{ "%08: &31 %02 %11",  3},
	{ "%11: &02 &26 &49 &48",  6},
	{ "%11: &02 &26 &49 %13 &48",  6},
	{ "%23: %31 &41 %32 &40",  0},
	{ "%31: &07 %33",  0},
	{ "%31: &08 %33",  0},
	{ "%31: %33",  0},
	{ "%33: &03 &26 &26",  0},
	{ "%33: &03 &26",  0},
	{ "%32: %32 %34 &42",  0},
	{ "%32: %32 &01 &42",  0},
	{ "%32:",  0},
	{ "%34: &26 &45 %12",  0},
	{ "%34: %35",  0},
	{ "%34: %36",  0},
	{ "%34: %37",  0},
	{ "%35: &26 &45 &41 &40",  0},
	{ "%35: &26 &45 &41 %17 &40",  0},
	{ "%36: %38 %32 &40",  0},
	{ "%38: &26 &45 &26 &41",  0},
	{ "%37: %39 %40 &40",  0},
	{ "%37: %39 %40 &01",  0},
	{ "%39: &26 &45 &41 &26 &41",  0},
	{ "%40: %41",  0},
	{ "%40: %41 &50 %42",  0},
	{ "%40: %41 &50 &01",  0},
	{ "%41: %32 &40",  0},
	{ "%42: %43",  0},
	{ "%42: %42 &50 %43",  0},
	{ "%43: %44 %32 &40",  0},
	{ "%44: &26 &41",  0},
	{ "%12: &28",  2},
	{ "%12: &32",  2},
	{ "%12: &26",  2},
	{ "%12: %15",  2},
	{ "%12: %13",  2},
	{ "%17: %12",  7},
	{ "%17: %17 &50 %12",  7},
	{ "%13: %18",  2},
	{ "%18: &27",  8},
	{ "%18: %18 &34 %18",  8},
	{ "%18: %18 &35 %18",  8},
	{ "%18: %18 &36 %18",  8},
	{ "%18: %18 &37 %18",  8},
	{ "%18: %18 &38 %18",  8},
	{ "%18: &35 %18",  8},
	{ "%18: &43 %18 &44",  8},
	{ "%15: %16",  2},
	{ "%15: %16 %15",  2},
	{ "%16: &33",  2},
	{ "%16: %14",  2},
	{ "%14: &46 %18 &47",  2},
	{ "%25: &04 &26",  0},
	{ "%25: &04 &33",  0},
	{ "%24: &09 &26",  0},
	{ "%26: &05 %13",  0},
	{ "%27: &06",  0},
	{ "%28: %45 %46 &40",  0},
	{ "%28: %45 %46 &40 &42",  0},
	{ "%45: &30 &26 &41",  0},
	{ "%45: &30 &41",  0},
	{ "%47: &26",  0},
	{ "%47: &26 &45 %12",  0},
	{ "%46: %47",  0},
	{ "%46: %46 &50 %47",  0},
{ "$accept",  0},{ "error",  0}
};
yyNamedType yyTokenTypes[] = {
	{ "$end",  0,  0},
	{ "error",  256,  0},
	{ "L_STRUCT",  257,  1},
	{ "L_RESOURCE",  258,  1},
	{ "L_NAME",  259,  1},
	{ "L_OFFSET",  260,  1},
	{ "L_SYSTEM",  261,  1},
	{ "L_GLOBAL",  262,  1},
	{ "L_LOCAL",  263,  1},
	{ "L_CHARACTER_SET",  264,  1},
	{ "L_BUF",  265,  1},
	{ "L_WORD",  266,  1},
	{ "L_BYTE",  267,  1},
	{ "L_LONG",  268,  1},
	{ "L_DOUBLE",  269,  1},
	{ "L_TEXT",  270,  1},
	{ "L_LTEXT",  271,  1},
	{ "L_LINK",  272,  1},
	{ "L_LLINK",  273,  1},
	{ "L_SRLINK",  274,  1},
	{ "L_BUF8",  275,  1},
	{ "L_TEXT8",  276,  1},
	{ "L_LTEXT8",  277,  1},
	{ "L_BUF16",  278,  1},
	{ "L_TEXT16",  279,  1},
	{ "L_LTEXT16",  280,  1},
	{ "L_LABEL",  281,  2},
	{ "L_NUM_NATURAL",  282,  2},
	{ "L_NUM_FLOAT",  283,  2},
	{ "L_NATURAL_EXPR",  284,  2},
	{ "L_ENUM",  285,  2},
	{ "L_LEN",  286,  2},
	{ "L_CHAR_LITERAL",  287,  2},
	{ "L_STRING_LITERAL",  288,  2},
	{ "'+'",  43,  0},
	{ "'-'",  45,  0},
	{ "'*'",  42,  0},
	{ "'/'",  47,  0},
	{ "'|'",  124,  0},
	{ "UMINUS",  289,  0},
	{ "'}'",  125,  0},
	{ "'{'",  123,  0},
	{ "';'",  59,  0},
	{ "'('",  40,  0},
	{ "')'",  41,  0},
	{ "'='",  61,  0},
	{ "'<'",  60,  0},
	{ "'>'",  62,  0},
	{ "']'",  93,  0},
	{ "'['",  91,  0},
	{ "','",  44,  0}

};
#endif
static short yydef[] = {

	   3,   -1,   15,   10,    9,  117,    7,    4,  118,   14, 
	  13,   12,   11,  104,  104,    8,    6,  120,   34,    5, 
	 104,  119,   34
};
static short yyex[] = {

	   0,    0,   -1,    1
};
static short yyact[] = {

	 -82,  -86,  -87,  -89,  -97,  -84,  -85,  -88,  -91,  285, 
	 264,  263,  262,  261,  260,  259,  258,  257,  -95,  -81, 
	 281,  123,   -3,  281,  -79,  -78, -107,  282,   45,   40, 
	 -99,  281, -101, -100,  288,  281,   -5,  281,  -86,  258, 
	-180,  123,  -77,  281,  -96,  123,  -75,   61,  -74,   -6, 
	 125,   44,  -70,  -68,  -69,  -71,  -72,  124,   47,   45, 
	  43,   42, -125,  281, -162, -132, -133,  -66,  286,  267, 
	 266,  123, -163,  -64, -149, -154, -155, -153, -152, -151, 
	-150, -142, -141, -140, -144, -148, -146, -143, -147, -145, 
	 -63,  286,  280,  279,  278,  277,  276,  275,  274,  273, 
	 272,  271,  270,  269,  268,  267,  266,  265,  257,  125, 
	 -79,  -78,  -59, -110, -107, -187, -111, -190,  288,  287, 
	 283,  282,  281,   60,   45,   40, -192,   59, -104,  -70, 
	 -68,  -69,  -71,  -72,  124,   47,   45,   43,   42,   41, 
	-127,  -57,  -58,  281,  256,  125, -160,  123, -161,  123, 
	 -16,  281, -132, -133,  267,  266,  -79,  -78, -138, -107, 
	 282,   93,   45,   40,  -53,   61,  -52,  -17,  281,   60, 
	 -50,  -51,   61,   40, -159,   59,  -59, -190,  288,   60, 
	 -72,  124,  -70,  -71,  -72,  124,   47,   42,  -45,   61, 
	-123,   59, -124,   59,  -44,   91,  -43, -149, -154, -155, 
	-153, -152, -151, -150, -142, -141, -140, -144, -148, -146, 
	-143, -147, -145,  280,  279,  278,  277,  276,  275,  274, 
	 273,  272,  271,  270,  269,  268,  267,  266,  265,  257, 
	-137,   93,  -40,  123, -134,   91,  -70,  -68,  -69,  -71, 
	-102,  -72,  124,   62,   47,   45,   43,   42,  -37,   44, 
	-117, -116,  256,  125, -114,  -57,  -58,  281,  256,  125, 
	-119,  -57,  -58,  281,  256,  125,  -79,  -78,  -59,  -36, 
	 -19, -107, -187, -111, -190,  288,  287,  283,  282,  281, 
	 123,   60,   45,   40,  -79,  -78, -129, -107,  282,   93, 
	  45,   40,  -34,  281,  -33,  281,  -79,  -78, -136, -107, 
	 282,   93,   45,   40,  -70,  -68,  -69,  -71,  -30,  -72, 
	 124,   62,   47,   45,   43,   42,  -20,   41, -184,  -29, 
	 281,  256, -118,  123,  -79,  -78,  -59, -121,  -23, -107, 
	-187, -111, -190,  288,  287,  283,  282,  281,  125,   60, 
	  45,   40, -128,   93, -135,   93,  -27, -139,  125,   44, 
	-156,  281,  -26,   61, -112,  123,  -24,   44, -115,  123, 
	 -27, -120,  125,   44, -113,  -57,  -58,  281,  256,  125, 
	 -29,  281,   -1
};
static short yypact[] = {

	   9,   18,   47,   57,   63,  127,  165,  172,  178,  181, 
	 181,  185,  185,  188,  188,  195,  235,  249,  323,  353, 
	 356,  357,  359,  371,  367,  178,  118,  362,  355,  351, 
	 348,  345,  235,  195,  343,  333,  320,  317,  310,  118, 
	 300,  295,  293,  288,  275,  263,  257,  252,  242,   27, 
	 118,   27,  233,  231,  213,  193,  191,  189,   27,  175, 
	 168,  160,  154,  151,  149,  147,  143,   27,   27,   27, 
	  27,   27,  134,   23,  118,   91,   68,   27,   27,   50, 
	  45,   43,   41,   39,   39,   37,   34,   31,   27,   23,   20
};
static short yygo[] = {

	 -42,  -61,   54,  -55,  -65,   62,  -60, -175, -174,   -8, 
	-176, -177,  -41,  -62,   54,   -7, -130, -131,   54, -108, 
	-122, -158,  -94, -109,   74,   50,   44,   26,  -32,  -35, 
	 -38,  -54,  -98, -189,   88,   61,   49,   43,   40, -191, 
	-103, -157, -188,   25,    8,   -9,  -28,  -31,   35,  -39, 
	 -49,  -13,  -12,  -11,  -10, -106, -105,  -73,   -4,   78, 
	  77,   71,   70,   69,   68,   67,   58,   51,   -2,   -1, 
	-164, -166, -167, -168, -169, -170, -171, -172, -173,  -76, 
	 -83,  -47,  -46,  -25,  -67,   20,   14,   13, -178, -126, 
	-179,   84,   83,  -56, -181, -182, -183,  -15,  -14,  -48, 
	 -18,  -22, -186, -185,   23,  -21,  -90,  -80,  -92,  -93, 
	  73,   -1
};
static short yypgo[] = {

	   0,    0,    0,   68,    8,    8,    9,    7,   10,   90, 
	  33,   58,   58,   58,   58,  109,  107,  107,  109,  106, 
	 106,   76,   75,   73,   74,   74,   39,   42,   58,   58, 
	  58,   58,   47,   47,   23,   23,  105,  103,  100,   98, 
	  96,   96,   97,   95,   94,   94,   93,   84,   84,   90, 
	  80,   72,   17,   17,   11,   11,    4,    4,   13,   15, 
	  15,   15,   15,    7,    1,    1,    1,    1,    1,    1, 
	   1,    1,    1,    1,    1,    1,    1,    1,    1,    1, 
	   9,    8,    8,   79,   78,   78,   78,   71,   69,   69, 
	  70,   70,   70,   70,   70,   70,   70,   79,    6,    6, 
	   6,    6,   80,   80,   84,   93,   93,   93,   99,  101, 
	 101,   23,   23,   23,   45,   45,   77,   77,   42,   99, 
	  99,    0
};
static short yyrlen[] = {

	   0,    0,    0,    1,    1,    4,    2,    1,    2,    2, 
	   1,    3,    3,    3,    3,    1,    3,    1,    3,    2, 
	   3,    1,    2,    2,    2,    2,    3,    2,    3,    2, 
	   3,    1,    3,    1,    1,    1,    2,    3,    2,    5, 
	   3,    3,    4,    3,    5,    4,    3,    3,    3,    3, 
	   2,    4,    5,    4,    3,    1,    1,    1,    3,    5, 
	   4,    3,    2,    5,    1,    1,    1,    1,    1,    1, 
	   1,    1,    1,    1,    1,    1,    1,    1,    1,    1, 
	   5,    6,    3,    3,    4,    4,    3,    3,    2,    0, 
	   1,    1,    1,    1,    1,    1,    1,    0,    1,    1, 
	   1,    1,    2,    1,    0,    1,    1,    1,    3,    1, 
	   3,    1,    1,    1,    1,    1,    4,    3,    1,    3, 
	   1,    2
};
#define YYS0	164
#define YYDELTA	75
#define YYNPACT	91
#define YYNDEF	23

#define YYr119	0
#define YYr120	1
#define YYr121	2
#define YYr1	3
#define YYr21	4
#define YYr22	5
#define YYr25	6
#define YYr43	7
#define YYr52	8
#define YYr62	9
#define YYr92	10
#define YYr94	11
#define YYr95	12
#define YYr96	13
#define YYr97	14
#define YYr115	15
#define YYr118	16
#define YYr117	17
#define YYr116	18
#define YYr114	19
#define YYr113	20
#define YYr110	21
#define YYr109	22
#define YYr108	23
#define YYr107	24
#define YYr106	25
#define YYr105	26
#define YYr102	27
#define YYr100	28
#define YYr99	29
#define YYr98	30
#define YYr93	31
#define YYr91	32
#define YYr90	33
#define YYr87	34
#define YYr86	35
#define YYr84	36
#define YYr83	37
#define YYr80	38
#define YYr76	39
#define YYr75	40
#define YYr74	41
#define YYr73	42
#define YYr72	43
#define YYr71	44
#define YYr70	45
#define YYr66	46
#define YYr64	47
#define YYr63	48
#define YYr61	49
#define YYr59	50
#define YYr57	51
#define YYr56	52
#define YYr55	53
#define YYr54	54
#define YYr53	55
#define YYr51	56
#define YYr50	57
#define YYr49	58
#define YYr48	59
#define YYr47	60
#define YYr46	61
#define YYr45	62
#define YYr44	63
#define YYr42	64
#define YYr41	65
#define YYr40	66
#define YYr39	67
#define YYr38	68
#define YYr37	69
#define YYr36	70
#define YYr35	71
#define YYr34	72
#define YYr33	73
#define YYr32	74
#define YYr31	75
#define YYr30	76
#define YYr29	77
#define YYr28	78
#define YYr27	79
#define YYr26	80
#define YYr24	81
#define YYr23	82
#define YYr15	83
#define YYr14	84
#define YYr13	85
#define YYr12	86
#define YYr11	87
#define YYrACCEPT	YYr119
#define YYrERROR	YYr120
#define YYrLR2	YYr121
#if YYDEBUG
char * yysvar[] = {
	"$accept",
	"data-type",
	"len-declaration",
	"struct-item",
	"array-struct-item",
	"simple-struct-item",
	"simple-struct-item-start",
	"struct-type-struct-item",
	"struct-array-struct-item",
	"array-struct-item-start",
	"array-struct-item-base",
	"struct-array-struct-item-base",
	"simple-initialiser",
	"natural-expression",
	"character-code-expression",
	"string-expression",
	"string-expression-item",
	"simple-initialiser-list",
	"natural-expression-numeric",
	"source",
	"statement-list",
	"statement",
	"struct-statement",
	"resource-statement",
	"character_set-statement",
	"name-statement",
	"offset-statement",
	"system-statement",
	"enum-statement",
	"struct-statement-start",
	"struct-item-list",
	"resource-statement-start",
	"resource-item-list",
	"resource-statement-start-names",
	"resource-item",
	"resource-simple-array-item",
	"struct-resource-item",
	"struct-array-resource-item",
	"struct-resource-item-start",
	"struct-array-resource-item-start",
	"struct-array-resource-item-list-top",
	"struct-array-resource-item-list-top-start",
	"struct-array-resource-item-list",
	"struct-array-resource-item-list-item",
	"struct-array-resource-item-list-item-start",
	"enum-statement-start",
	"enum-list",
	"enum-list-entry",
	0
};
short yyrmap[] = {

	 119,  120,  121,    1,   21,   22,   25,   43,   52,   62, 
	  92,   94,   95,   96,   97,  115,  118,  117,  116,  114, 
	 113,  110,  109,  108,  107,  106,  105,  102,  100,   99, 
	  98,   93,   91,   90,   87,   86,   84,   83,   80,   76, 
	  75,   74,   73,   72,   71,   70,   66,   64,   63,   61, 
	  59,   57,   56,   55,   54,   53,   51,   50,   49,   48, 
	  47,   46,   45,   44,   42,   41,   40,   39,   38,   37, 
	  36,   35,   34,   33,   32,   31,   30,   29,   28,   27, 
	  26,   24,   23,   15,   14,   13,   12,   11,    2,    3, 
	   4,    5,    6,    7,    8,    9,   10,   16,   17,   18, 
	  19,   20,   58,   60,   65,   67,   68,   69,   79,   81, 
	  82,   85,   88,   89,  103,  104,  112,  111,  101,   78, 
	  77,    0
};
short yysmap[] = {

	   1,    2,   27,   33,   37,   46,   65,   83,   94,  104, 
	 105,  106,  107,  108,  109,  119,  125,  131,  155,  167, 
	 169,  172,  174,  185,  184,  182,  179,  175,  168,  166, 
	 165,  162,  161,  160,  158,  156,  150,  148,  145,  144, 
	 142,  141,  139,  138,  135,  134,  133,  132,  129,  127, 
	 126,  124,  123,  121,  120,  115,  114,  113,   91,   88, 
	  82,   64,   63,   62,   59,   58,   55,   53,   52,   51, 
	  50,   49,   47,   45,   44,   42,   41,   31,   30,   28, 
	  25,   14,   13,   12,   11,    9,    8,    7,    6,    4, 
	   3,  100,   26,   90,   24,   43,    5,   29,   34,   35, 
	  36,  149,  130,  102,   48,  103,   32,  188,  164,   97, 
	  98,  183,  190,  153,  186,  151,  152,  173,  154,  187, 
	 176,  157,  136,  137,   54,   38,  116,  177,  159,  140, 
	  61,   56,   57,  146,  178,  163,  143,  122,  180,   66, 
	  67,   68,   69,   70,   71,   72,   73,   74,   75,   76, 
	  77,   78,   79,   80,   81,  181,  189,  147,  128,  117, 
	 118,   60,   89,   23,    0,   22,   21,   20,   19,   18, 
	  17,   16,   15,   87,   86,   85,   84,   39,   10,   40, 
	 112,  111,  110,  171,  170,  191,   99,   96,   95,   93, 
	  92,  101
};
int yy_parse::yyntoken = 51;
int yy_parse::yynvar = 48;
int yy_parse::yynstate = 192;
int yy_parse::yynrule = 122;
#endif



#line 2 "t:/mks/etc/yyparse.cpp"
// C++ YACC parser code
// Copyright 1991 by Mortice Kern Systems Inc.  All rights reserved.
//
// If YYDEBUG is defined as 1 and yy_parse::yydebug is set to 1,
// yyparse() will print a travelogue of its actions as it reads
// and parses input.
//
// YYSYNC can be defined to cause yyparse() to attempt to always
// hold a lookahead token

const YY_MIN_STATE_NUM = 20;	// not useful to be too small!

#if YYDEBUG
#ifdef YYTRACE
long	* yy_parse::States	= yyStates;
#endif
yyTypedRules * yy_parse::Rules	= yyRules;
yyNamedType * yy_parse::TokenTypes = yyTokenTypes;

#define YY_TRACE(fn) { done = 0; fn(); if (done) YYRETURN(-1); }
#endif

// Constructor for yy_parse: user-provided tables
yy_parse::yy_parse(int sz, short * states, YYSTYPE * stack)
{
	mustfree = 0;
	if ((size = sz) < YY_MIN_STATE_NUM
	 || (stateStack = states) == (short *) 0
	 || (valueStack = stack) == (YYSTYPE *) 0) {
		fprintf(stderr,"Bad state/stack given");
		exit(1);
	}
	reset = 1;		// force reset
#if YYDEBUG
	yydebug = 0;
	typeStack = new short[size+1];
	if (typeStack == (short *) 0) {
		fprintf(stderr,"Cannot allocate typeStack");
		exit(1);
	}
#endif
}
// Constructor for yy_parse: allocate tables with new
yy_parse::yy_parse(int sz)
{
	size = sz;
	reset = 1;		// force reset
	mustfree = 1;		// delete space in deconstructor
#if YYDEBUG
	yydebug = 0;
	typeStack = new short[size+1];
#endif
	stateStack = new short[size+1];
	valueStack = new YYSTYPE[size+1];

	if (stateStack == (short *) 0 || valueStack == (YYSTYPE *) 0
#if YYDEBUG
		|| typeStack == (short *) 0
#endif
	    ) {
		fprintf(stderr,"Not enough space for parser stacks");
		exit(1);
	}
}
// Destructor for class yy_parse
//	Free up space
yy_parse::~yy_parse()
{
	if (mustfree) {
		delete stateStack;
		delete valueStack;
	}
	stateStack = (short *) 0;
#if YYDEBUG
	delete typeStack;
#endif
}

#ifdef YACC_WINDOWS

// The initial portion of the yacc parser.
// In an windows environment, it will load the desired
// resources, obtain pointers to them, and then call
// the protected member win_yyparse() to acutally begin the
// parsing. When complete, win_yyparse() will return a
// value back to our new yyparse() function, which will 
// record that value temporarily, release the resources
// from global memory, and finally return the value
// back to the caller of yyparse().

int
yy_parse::yyparse(yy_scan* ps)
{
	int wReturnValue;
	HANDLE hRes_table;
	short *old_yydef;		// the following are used for saving
	short *old_yyex;		// the current pointers
	short *old_yyact;
	short *old_yypact;
	short *old_yygo;
	short *old_yypgo;
	short *old_yyrlen;

	// the following code will load the required
	// resources for a Windows based parser.

	hRes_table = LoadResource (hInst,
		FindResource (hInst, "UD_RES_yyYACC", "yyYACCTBL"));
	
	// return an error code if any
	// of the resources did not load

	if (hRes_table == (HANDLE)NULL)
		return (1);
	
	// the following code will lock the resources
	// into fixed memory locations for the parser
	// (also, save away the old pointer values)

	old_yydef = yydef;
	old_yyex = yyex;
	old_yyact = yyact;
	old_yypact = yypact;
	old_yygo = yygo;
	old_yypgo = yypgo;
	old_yyrlen = yyrlen;

	yydef = (short *)LockResource (hRes_table);
	yyex = (short *)(yydef + Sizeof_yydef);
	yyact = (short *)(yyex + Sizeof_yyex);
	yypact = (short *)(yyact + Sizeof_yyact);
	yygo = (short *)(yypact + Sizeof_yypact);
	yypgo = (short *)(yygo + Sizeof_yygo);
	yyrlen = (short *)(yypgo + Sizeof_yypgo);

	// call the official yyparse() function

	wReturnValue = win_yyparse (ps);

	// unlock the resources

	UnlockResource (hRes_table);

	// and now free the resource

	FreeResource (hRes_table);

	//
	// restore previous pointer values
	//

	yydef = old_yydef;
	yyex = old_yyex;
	yyact = old_yyact;
	yypact = old_yypact;
	yygo = old_yygo;
	yypgo = old_yypgo;
	yyrlen = old_yyrlen;

	return (wReturnValue);
}	// end yyparse()


// The parser proper.
//	Note that this code is reentrant; you can return a value
//	and then resume parsing by recalling yyparse().
//	Call yyreset() before yyparse() if you want a fresh start

int
yy_parse::win_yyparse(yy_scan* ps)

#else /* YACC_WINDOWS */

// The parser proper.
//	Note that this code is reentrant; you can return a value
//	and then resume parsing by recalling yyparse().
//	Call yyreset() before yyparse() if you want a fresh start
int
yy_parse::yyparse(yy_scan* ps)

#endif /* YACC_WINDOWS */

{
	short	* yyp, * yyq;		// table lookup
	int	yyj;
#if YYDEBUG
	int	yyruletype = 0;
#endif

	if ((scan = ps) == (yy_scan *) 0) {	// scanner
		fprintf(stderr,"No scanner");
		exit(1);
	}

	if (reset) {			// start new parse
		yynerrs = 0;
		yyerrflag = 0;
		yyps = stateStack;
		yypv = valueStack;
#if YYDEBUG
		yytp = typeStack;
#endif
		yystate = YYS0;
		yyclearin();
		reset = 0;
	} else			// continue saved parse
		goto yyNext;			// after action

yyStack:
	if (++yyps > &stateStack[size]) {
		scan->yyerror("Parser stack overflow");
		YYABORT;
	}
	*yyps = yystate;	/* stack current state */
	*++yypv = yyval;	/* ... and value */
#if YYDEBUG
	if (yydebug) {
		*++yytp = yyruletype;	/* ... and type */
		YY_TRACE(yyShowState)
	}
#endif

	/*
	 * Look up next action in action table.
	 */
yyEncore:
#ifdef YYSYNC
	if (yychar < 0) {
		if ((yychar = scan->yylex()) < 0) {
			if (yychar == -2) YYABORT;
			yychar = 0;
		}	/* endif */
		yylval = ::yylval;
#if YYDEBUG
		if (yydebug)
			yyShowRead();	// show new input token
#endif
	}
#endif
#ifdef YACC_WINDOWS
	if (yystate >= Sizeof_yypact) 	/* simple state */
#else /* YACC_WINDOWS */
	if (yystate >= sizeof yypact/sizeof yypact[0]) 	/* simple state */
#endif /* YACC_WINDOWS */
		yyi = yystate - YYDELTA;	/* reduce in any case */
	else {
		if(*(yyp = &yyact[yypact[yystate]]) >= 0) {
			/* Look for a shift on yychar */
#ifndef YYSYNC
			if (yychar < 0) {
				if ((yychar = scan->yylex()) < 0) {
					if (yychar == -2) YYABORT;
					yychar = 0;
				}	/* endif */
				yylval = ::yylval;
#if YYDEBUG
				if (yydebug)
					yyShowRead();	// show new input token
#endif
			}
#endif
			yyq = yyp;
			yyi = yychar;
			while (yyi < *yyp++)
				;
			if (yyi == yyp[-1]) {
				yystate = ~yyq[yyq-yyp];
#if YYDEBUG
				if (yydebug) {
					yyruletype = yyGetType(yychar);
					YY_TRACE(yyShowShift)
				}
#endif
				yyval = yylval;		/* stack value */
				yyclearin();		/* clear token */
				if (yyerrflag)
					yyerrflag--;	/* successful shift */
				goto yyStack;
			}
		}

		/*
	 	 *	Fell through - take default action
	 	 */

#ifdef YACC_WINDOWS
		if (yystate >= Sizeof_yydef) 	/* simple state */
#else /* YACC_WINDOWS */
		if (yystate >= sizeof yydef /sizeof yydef[0])
#endif /* YACC_WINDOWS */
			goto yyError;
		if ((yyi = yydef[yystate]) < 0)	 { /* default == reduce? */

			/* Search exception table */
			yyp = &yyex[~yyi];
#ifndef YYSYNC
			if (yychar < 0) {
				if ((yychar = scan->yylex()) < 0) {
					if (yychar == -2) YYABORT;
					yychar = 0;
				}	/* endif */
				yylval = ::yylval;
#if YYDEBUG
				if (yydebug)
					yyShowRead();	// show new input token
#endif
			}
#endif
			while((yyi = *yyp) >= 0 && yyi != yychar)
				yyp += 2;
			yyi = yyp[1];
		}
	}

	yyj = yyrlen[yyi];

#if YYDEBUG
	if (yydebug) {
		npop = yyj; rule = yyi;
		YY_TRACE(yyShowReduce)
		yytp -= yyj;
	}
#endif
	yyps -= yyj;		/* pop stacks */
	yypvt = yypv;		/* save top */
	yypv -= yyj;
	yyval = yypv[1];	/* default action $ = $1 */
#if YYDEBUG
	if (yydebug)
		yyruletype = yyRules[yyrmap[yyi]].type;
#endif
	switch (yyi) {		/* perform semantic action */
		
case YYr1: {	/* source :  statement-list */
#line 111 "../rcomp.y"
	if(verbose)	{	MOFF; cout << Divider << "\n" << Divider << endl; MON; }
										
} break;

case YYr11: {	/* struct-statement :  struct-statement-start struct-item-list '}' */
#line 136 "../rcomp.y"
	if(verbose) { MOFF; cout << Divider << "\n" << * pSH << Divider << endl; MON;}	
} break;

case YYr12: {	/* struct-statement-start :  L_STRUCT L_LABEL '{' */
#line 139 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-statement-start                     " << yypvt[-1].Value << endl; MON;}
											pSH = new StructHeader(yypvt[-1].Value);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										
} break;

case YYr13: {	/* struct-statement-start :  L_STRUCT L_LABEL len-declaration '{' */
#line 145 "../rcomp.y"
	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct-statement-start                     " << yypvt[-2].Value << " " << Types.GetName(yypvt[-1].Id) << endl; MON;}
											pSH = new StructHeader(yypvt[-2].Value, yypvt[-1].Id);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										
} break;

case YYr14: {	/* struct-statement-start :  L_STRUCT L_LABEL L_LEN '{' */
#line 151 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-statement-start                     " << yypvt[-2].Value << " (WORD)" << endl; MON;}
											pSH = new StructHeader(yypvt[-2].Value, L_WORD);
											REGISTER_LINE;
											pG->SHA.Add(pSH);
										
} break;

case YYr15: {	/* struct-item-list :  struct-item-list struct-item ';' */
#line 158 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-item-list                           Adding struct-item." << endl; MON;}
											REGISTER_LINE;
											pSH->iSIA.Add(yypvt[-1].pStructItem);
										
} break;

case YYr21: {	/* simple-struct-item :  simple-struct-item-start */
#line 171 "../rcomp.y"
	yyval.pStructItem = yypvt[0].pSimpleStructItem;
} break;

case YYr22: {	/* simple-struct-item :  simple-struct-item-start '(' natural-expression ')' */
#line 173 "../rcomp.y"
	if(verbose) { MOFF;cout << "    Limit: " << yypvt[-1].Value << endl; MON;}
											yypvt[-3].pSimpleStructItem->iLengthLimit = yypvt[-1].Value;
											yyval.pStructItem = yypvt[-3].pSimpleStructItem;
										
} break;

case YYr23: {	/* simple-struct-item :  simple-struct-item-start '=' simple-initialiser */
#line 178 "../rcomp.y"
	if(verbose) { MOFF;cout << "    Default: " << yypvt[0].Value << endl; MON;}
											yypvt[-2].pSimpleStructItem->iDefault = yypvt[0].Value;
											yyval.pStructItem = yypvt[-2].pSimpleStructItem;
										
} break;

case YYr24: {	/* simple-struct-item :  simple-struct-item-start '(' natural-expression ')' '=' string-expression */
#line 183 "../rcomp.y"
	if(verbose) { MOFF;cout << "    Limit: " << yypvt[-3].Value << ", Default: " << yypvt[0].Value << endl; MON;}
											NumericValue Limit(yypvt[-3].Value, L_LONG);
											if(String(yypvt[0].Value).ExportLength(TargetCharacterSet,SourceCharacterSet) > Limit.GetULong() )
											{
												REGISTER_LINE;
												ErrorHandler::OutputErrorLine("Text length exceeds specified limit");
												exit(1);
											}
											yypvt[-5].pSimpleStructItem->iLengthLimit = yypvt[-3].Value;
											yypvt[-5].pSimpleStructItem->iDefault = yypvt[0].Value;
											yyval.pStructItem = yypvt[-5].pSimpleStructItem;
										
} break;

case YYr25: {	/* simple-struct-item-start :  data-type L_LABEL */
#line 198 "../rcomp.y"
	if(verbose) 
											{			   RCTypeArray Types;
														   MOFF;cout << "simple-struct-item                         " << Types.GetName(yypvt[-1].Id) << " " << yypvt[0].Value << endl; MON;
											}
											yyval.pSimpleStructItem = new SimpleStructItem(yypvt[-1].Id,yypvt[0].Value); 
											assert(yyval.pSimpleStructItem != NULL);
										
} break;

case YYr26: {	/* simple-struct-item-start :  data-type '<' natural-expression-numeric '>' L_LABEL */
#line 206 "../rcomp.y"
	if(verbose) 
											{			   RCTypeArray Types;
														   MOFF;cout << "simple-struct-item                         " << Types.GetName(yypvt[-4].Id) << " " << yypvt[0].Value << endl; MON;
											}
											String s(NumericValue::ltoa(yypvt[-2].NumInitialiser));
											yyval.pSimpleStructItem = new SimpleStructItem(yypvt[-4].Id,yypvt[0].Value,s);
											assert(yyval.pSimpleStructItem != NULL);
										
} break;

case YYr27: {	/* data-type :  L_BYTE */
#line 220 "../rcomp.y"
	yyval.Id = L_BYTE;
} break;

case YYr28: {	/* data-type :  L_WORD */
#line 221 "../rcomp.y"
	yyval.Id = L_WORD;
} break;

case YYr29: {	/* data-type :  L_LONG */
#line 222 "../rcomp.y"
	yyval.Id = L_LONG;
} break;

case YYr30: {	/* data-type :  L_DOUBLE */
#line 223 "../rcomp.y"
	yyval.Id = L_DOUBLE;
} break;

case YYr31: {	/* data-type :  L_TEXT */
#line 228 "../rcomp.y"
 
	    yyval.Id = ( TargetCharacterSet == String::Unicode ) ? L_TEXT16: L_TEXT8;
	    REGISTER_LINE;
	    ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT - use LTEXT instead");
	    
} break;

case YYr32: {	/* data-type :  L_LTEXT */
#line 234 "../rcomp.y"

	    yyval.Id = ( TargetCharacterSet == String::Unicode ) ? L_LTEXT16: L_LTEXT8;
	    
} break;

case YYr33: {	/* data-type :  L_BUF */
#line 238 "../rcomp.y"
 
	    yyval.Id = ( TargetCharacterSet == String::Unicode ) ? L_BUF16: L_BUF8;
	    
} break;

case YYr34: {	/* data-type :  L_TEXT8 */
#line 244 "../rcomp.y"
	yyval.Id = L_TEXT8;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT8 - use LTEXT8 instead");
										
} break;

case YYr35: {	/* data-type :  L_TEXT16 */
#line 248 "../rcomp.y"
	yyval.Id = L_TEXT16;
											REGISTER_LINE;
											ErrorHandler::OutputErrorLine("Warning: Deprecated use of zero-terminated TEXT16 - use LTEXT16 instead");
										
} break;

case YYr36: {	/* data-type :  L_LTEXT8 */
#line 252 "../rcomp.y"
	yyval.Id = L_LTEXT8;
} break;

case YYr37: {	/* data-type :  L_LTEXT16 */
#line 253 "../rcomp.y"
	yyval.Id = L_LTEXT16;
} break;

case YYr38: {	/* data-type :  L_BUF8 */
#line 254 "../rcomp.y"
	yyval.Id = L_BUF8;
} break;

case YYr39: {	/* data-type :  L_BUF16 */
#line 255 "../rcomp.y"
	yyval.Id = L_BUF16;
} break;

case YYr40: {	/* data-type :  L_LINK */
#line 256 "../rcomp.y"
	yyval.Id = L_LINK;
} break;

case YYr41: {	/* data-type :  L_LLINK */
#line 257 "../rcomp.y"
	yyval.Id = L_LLINK;
} break;

case YYr42: {	/* data-type :  L_SRLINK */
#line 258 "../rcomp.y"
	yyval.Id = L_SRLINK;
} break;

case YYr43: {	/* array-struct-item :  array-struct-item-base */
#line 261 "../rcomp.y"
	yyval.pStructItem = yypvt[0].pArrayStructItem;
} break;

case YYr44: {	/* array-struct-item :  array-struct-item-base '=' '{' simple-initialiser-list '}' */
#line 263 "../rcomp.y"
	if(verbose) {	MOFF;cout << "array-struct-item                          with simple-initialiser-list" << endl;MON;}
											yypvt[-4].pArrayStructItem->iDefaults = * yypvt[-1].pStringArray;
											if(yypvt[-4].pArrayStructItem->iSize.Length() > 0)
											{
												NumericValue v(yypvt[-4].pArrayStructItem->iSize, L_LONG);
												REGISTER_LINE;
												if(yypvt[-1].pStringArray->Size()!=long(v.GetULong()))
												{
													ErrorHandler::OutputErrorLine("Size does not match number of initialisers");
													exit(1);
												}
											}
											yyval.pStructItem = yypvt[-4].pArrayStructItem;
											delete yypvt[-1].pStringArray;
										
} break;

case YYr45: {	/* array-struct-item-base :  array-struct-item-start ']' */
#line 280 "../rcomp.y"
	if(verbose) {	MOFF;cout << "array-struct-item-base                     with no size" << endl;MON;}
											yyval.pArrayStructItem =yypvt[-1].pArrayStructItem;
										
} break;

case YYr46: {	/* array-struct-item-base :  array-struct-item-start natural-expression ']' */
#line 284 "../rcomp.y"
	if(verbose) {	MOFF;cout << "array-struct-item-base                     with size " << yypvt[-1].Value << endl;MON;}
											yypvt[-2].pArrayStructItem->iSize = yypvt[-1].Value;
											yyval.pArrayStructItem = yypvt[-2].pArrayStructItem;
										
} break;

case YYr47: {	/* array-struct-item-base :  L_LEN len-declaration array-struct-item-start ']' */
#line 289 "../rcomp.y"
	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array-struct-item-base                     with LenType " << Types.GetName(yypvt[-2].Id) << endl;MON;
												}
											yypvt[-1].pArrayStructItem->iLenType = yypvt[-2].Id;
											yyval.pArrayStructItem = yypvt[-1].pArrayStructItem;
										
} break;

case YYr48: {	/* array-struct-item-base :  L_LEN len-declaration array-struct-item-start natural-expression ']' */
#line 297 "../rcomp.y"
	if(verbose) 
												{		 	RCTypeArray Types;
														 	MOFF;cout << "array-struct-item-base                     with size " << yypvt[-1].Value << " and LenType " << Types.GetName(yypvt[-3].Id) << endl;MON;
												}
											yypvt[-2].pArrayStructItem->iLenType = yypvt[-3].Id;
											yypvt[-2].pArrayStructItem->iSize = yypvt[-1].Value; 
											yyval.pArrayStructItem = yypvt[-2].pArrayStructItem; 
										
} break;

case YYr49: {	/* array-struct-item-start :  data-type L_LABEL '[' */
#line 307 "../rcomp.y"
	if(verbose) 
												{		 	RCTypeArray Types; 
														 	MOFF;cout << "array-struct-item-start                    " << Types.GetName(yypvt[-2].Id) << " " << yypvt[-1].Value << endl;MON;
												}
											yyval.pArrayStructItem = new ArrayStructItem(yypvt[-2].Id, yypvt[-1].Value);
										
} break;

case YYr50: {	/* len-declaration :  L_BYTE */
#line 315 "../rcomp.y"
	yyval.Id = L_BYTE;
} break;

case YYr51: {	/* len-declaration :  L_WORD */
#line 316 "../rcomp.y"
	yyval.Id = L_WORD;
} break;

case YYr52: {	/* struct-type-struct-item :  L_STRUCT L_LABEL */
#line 319 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-type-struct-item                    " << yypvt[0].Value << endl;MON;}
											yyval.pStructItem = new StructTypeStructItem(yypvt[0].Value);
										
} break;

case YYr53: {	/* struct-array-struct-item :  struct-array-struct-item-base */
#line 324 "../rcomp.y"
	yyval.pStructItem = yypvt[0].pStructArrayStructItem;
} break;

case YYr54: {	/* struct-array-struct-item :  L_LEN len-declaration struct-array-struct-item-base */
#line 326 "../rcomp.y"
	if(verbose) {	RCTypeArray Types; MOFF;cout << "struct-array-struct-item                   - Setting Size to " << Types.GetName(yypvt[-1].Id) << endl;MON;}
											yypvt[0].pStructArrayStructItem->iLenType = yypvt[-1].Id; yyval.pStructItem = yypvt[0].pStructArrayStructItem;
										
} break;

case YYr55: {	/* struct-array-struct-item-base :  L_STRUCT L_LABEL '[' ']' */
#line 331 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-array-struct-item-base              " << yypvt[-2].Value << endl;MON;}
											yyval.pStructArrayStructItem = new StructArrayStructItem(yypvt[-2].Value);
										
} break;

case YYr56: {	/* struct-array-struct-item-base :  L_STRUCT L_LABEL '[' natural-expression ']' */
#line 335 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-array-struct-item-base              " << yypvt[-3].Value << " " << yypvt[-1].Value << endl;MON;}
											yyval.pStructArrayStructItem = new StructArrayStructItem(yypvt[-3].Value, yypvt[-1].Value);
										
} break;

case YYr57: {	/* resource-statement :  resource-statement-start '{' resource-item-list '}' */
#line 344 "../rcomp.y"
	
	    pResourceHeader->AddDefault();
	    CurrentId+=CurrentIdStep;
	    if(verbose) { MOFF;cout << "Resource ID "<< CurrentId << endl << Divider << "\n" << * pResourceHeader << Divider << endl;MON;}
	    pResourceHeader->SetResourceId(*pResourceNameIds,CurrentId,FormatIdAsHex);
	    pG->Index.Add(pResourceHeader);
	    pResourceHeader = NULL;
	    
} break;

case YYr59: {	/* resource-statement-start :  L_LOCAL resource-statement-start-names */
#line 356 "../rcomp.y"
	
	    if(verbose) { MOFF;cout << "resource-statement-start                   LOCAL" << endl;MON;}
		    assert(pResourceHeader != NULL);
		    pResourceHeader->iLocal = 1;
	    
} break;

case YYr61: {	/* resource-statement-start-names :  L_RESOURCE L_LABEL L_LABEL */
#line 364 "../rcomp.y"
	if(verbose) {	MOFF;cout << "resource-statement-start-names             " << yypvt[-1].Value << " " << yypvt[0].Value << endl;MON;}
											assert(pResourceHeader == NULL);
											pResourceHeader = new ResourceHeader(yypvt[0].Value);
											pCurrentRIA = & (pResourceHeader->iRIA);
											REGISTER_LINE;
											if(pResourceNameIds->IsStored(yypvt[0].Value))
											{
												ErrorHandler::OutputErrorLine("Resource with this name encountered already");
												exit(1);
											}
											pCurrentRIA->FillFromStruct(yypvt[-1].Value);
										
} break;

case YYr62: {	/* resource-statement-start-names :  L_RESOURCE L_LABEL */
#line 376 "../rcomp.y"
	if(verbose) {	MOFF;cout << "resource-statement-start-names             " << yypvt[0].Value << " <Resource not named>" << endl;MON;}
											assert(pResourceHeader == NULL);
											pResourceHeader = new ResourceHeader;
											pCurrentRIA = & (pResourceHeader->iRIA);
											REGISTER_LINE;
											pCurrentRIA->FillFromStruct(yypvt[0].Value);
										
} break;

case YYr63: {	/* resource-item-list :  resource-item-list resource-item ';' */
#line 385 "../rcomp.y"
	if(verbose) {	MOFF;cout << "resource-item-list" << endl;MON;}
} break;

case YYr64: {	/* resource-item-list :  resource-item-list error ';' */
#line 386 "../rcomp.y"
	yyerrok(); yyclearin(); 
} break;

case YYr66: {	/* resource-item :  L_LABEL '=' simple-initialiser */
#line 390 "../rcomp.y"
	if(verbose) {	MOFF;cout << "resource-item                              " << yypvt[-2].Value << " " << yypvt[0].Value << endl;MON;}
											REGISTER_LINE;
											pCurrentRIA->Set(yypvt[-2].Value, yypvt[0].Value);
										
} break;

case YYr70: {	/* resource-simple-array-item :  L_LABEL '=' '{' '}' */
#line 400 "../rcomp.y"
	
	    if (verbose) 
		{ MOFF;cout << "resource-simple-array-item                 " << yypvt[-3].Value << endl;MON;} 
	    
} break;

case YYr71: {	/* resource-simple-array-item :  L_LABEL '=' '{' simple-initialiser-list '}' */
#line 405 "../rcomp.y"
	
	    if (verbose) 
		{ MOFF;cout << "resource-simple-array-item                 " << yypvt[-4].Value << " with simple-initialiser-list" << endl;MON;}
	    REGISTER_LINE;
	    pCurrentRIA->Set(yypvt[-4].Value, * yypvt[-1].pStringArray);
	    delete yypvt[-1].pStringArray;
	    
} break;

case YYr72: {	/* struct-resource-item :  struct-resource-item-start resource-item-list '}' */
#line 434 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-resource-item" << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										
} break;

case YYr73: {	/* struct-resource-item-start :  L_LABEL '=' L_LABEL '{' */
#line 439 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-resource-item-start                 " << yypvt[-3].Value << " " << yypvt[-1].Value << endl;MON;}
											REGISTER_LINE;
											pCurrentRIA->Set(yypvt[-3].Value, yypvt[-1].Value);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = pCurrentRIA->Find(yypvt[-3].Value)->GetRIA();
										
} break;

case YYr74: {	/* struct-array-resource-item :  struct-array-resource-item-start struct-array-resource-item-list-top '}' */
#line 448 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-array-resource-item" << endl;MON;}
											pG->SRIStack.Pop();
										
} break;

case YYr75: {	/* struct-array-resource-item :  struct-array-resource-item-start struct-array-resource-item-list-top error */
#line 452 "../rcomp.y"
	pG->SRIStack.Pop();
} break;

case YYr76: {	/* struct-array-resource-item-start :  L_LABEL '=' '{' L_LABEL '{' */
#line 455 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-array-resource-item-start           " << yypvt[-4].Value << " " << yypvt[-1].Value << endl;MON;}
											ResourceItem * p = pCurrentRIA->Find(yypvt[-4].Value);
											pG->SRIStack.Push(p);
											REGISTER_LINE;
											p->Set(yypvt[-1].Value);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										
} break;

case YYr80: {	/* struct-array-resource-item-list-top-start :  resource-item-list '}' */
#line 470 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-array-resource-item-list-top        " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										
} break;

case YYr83: {	/* struct-array-resource-item-list-item :  struct-array-resource-item-list-item-start resource-item-list '}' */
#line 480 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-array-resource-item-list-item       " << endl;MON;}
											pCurrentRIA = pG->RIAStack.Pop();
										
} break;

case YYr84: {	/* struct-array-resource-item-list-item-start :  L_LABEL '{' */
#line 485 "../rcomp.y"
	if(verbose) {	MOFF;cout << "struct-array-resource-item-list-item-start " << yypvt[-1].Value << endl;MON;}
											ResourceItem * p = pG->SRIStack.Peek();
											REGISTER_LINE;
											p->Set(yypvt[-1].Value);
											pG->RIAStack.Push(pCurrentRIA);
											pCurrentRIA = p->GetRIA();
										
} break;

case YYr86: {	/* simple-initialiser :  L_CHAR_LITERAL */
#line 501 "../rcomp.y"
 
	    // convert literal to unsigned long value of 1st character
	    String s(yypvt[0].Value);
	    wchar_t first;
	    int length=1;
	    if (s.Export(&first, length, SourceCharacterSet)==0)
		{
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("Warning: Ignoring trailing characters in character literal");
		}
	    _ltoa(first,yyval.Value,10);
	    
} break;

case YYr87: {	/* simple-initialiser :  L_LABEL */
#line 514 "../rcomp.y"

	    if (pG->EnumValues.IsStored(yypvt[0].Value))
			{
			_ltoa(pG->EnumValues.FindId(yypvt[0].Value),yyval.Value,10);
			}
		else
			{
			
			}
	    
} break;

case YYr90: {	/* simple-initialiser-list :  simple-initialiser */
#line 534 "../rcomp.y"

	    if(verbose) 
		{	
		MOFF;cout << "simple-initialiser-list                    - single string " << yypvt[0].Value << endl;MON;
		}
	    yyval.pStringArray = new StringArray;
	    yyval.pStringArray->Add(new String(yypvt[0].Value) );
	    
} break;

case YYr91: {	/* simple-initialiser-list :  simple-initialiser-list ',' simple-initialiser */
#line 543 "../rcomp.y"
	if(verbose) {	MOFF;cout << "simple-initialiser-list                    - part of list " << yypvt[0].Value << endl;MON;}
											assert(yypvt[-2].pStringArray != NULL);
											yypvt[-2].pStringArray->Add(new String(yypvt[0].Value ) );
											yyval.pStringArray = yypvt[-2].pStringArray;
										
} break;

case YYr92: {	/* natural-expression :  natural-expression-numeric */
#line 551 "../rcomp.y"
	String s(NumericValue::ltoa(yypvt[0].NumInitialiser) ); strcpy(yyval.Value, s.GetBuffer() ); 
} break;

case YYr93: {	/* natural-expression-numeric :  L_NUM_NATURAL */
#line 554 "../rcomp.y"
	if(verbose) {	MOFF;cout << "Converting number " << yypvt[0].Value << endl;MON;}
											REGISTER_LINE;
											NumericValue v(yypvt[0].Value, L_LONG); yyval.NumInitialiser = v.GetLong();
										
} break;

case YYr94: {	/* natural-expression-numeric :  natural-expression-numeric '+' natural-expression-numeric */
#line 558 "../rcomp.y"
	yyval.NumInitialiser = yypvt[-2].NumInitialiser + yypvt[0].NumInitialiser;	
} break;

case YYr95: {	/* natural-expression-numeric :  natural-expression-numeric '-' natural-expression-numeric */
#line 559 "../rcomp.y"
	yyval.NumInitialiser = yypvt[-2].NumInitialiser - yypvt[0].NumInitialiser;	
} break;

case YYr96: {	/* natural-expression-numeric :  natural-expression-numeric '*' natural-expression-numeric */
#line 560 "../rcomp.y"
	yyval.NumInitialiser = yypvt[-2].NumInitialiser * yypvt[0].NumInitialiser;	
} break;

case YYr97: {	/* natural-expression-numeric :  natural-expression-numeric '/' natural-expression-numeric */
#line 561 "../rcomp.y"
	yyval.NumInitialiser = yypvt[-2].NumInitialiser / yypvt[0].NumInitialiser;	
} break;

case YYr98: {	/* natural-expression-numeric :  natural-expression-numeric '|' natural-expression-numeric */
#line 562 "../rcomp.y"
	yyval.NumInitialiser = yypvt[-2].NumInitialiser | yypvt[0].NumInitialiser;	
} break;

case YYr99: {	/* natural-expression-numeric :  '-' natural-expression-numeric */
#line 563 "../rcomp.y"
	yyval.NumInitialiser = - yypvt[0].NumInitialiser;		
} break;

case YYr100: {	/* natural-expression-numeric :  '(' natural-expression-numeric ')' */
#line 564 "../rcomp.y"
	yyval.NumInitialiser = yypvt[-1].NumInitialiser;		
} break;

case YYr102: {	/* string-expression :  string-expression-item string-expression */
#line 568 "../rcomp.y"

	    if (strlen(yyval.Value)+strlen(yypvt[0].Value) > sizeof(yyval.Value)-1)
		{
		REGISTER_LINE;
		ErrorHandler::OutputErrorLine("String expression is too long");
		exit(1);
		}
	    strcat(yyval.Value, yypvt[0].Value);
	    
} break;

case YYr105: {	/* character-code-expression :  '<' natural-expression-numeric '>' */
#line 584 "../rcomp.y"
	
	    REGISTER_LINE;
	    if(yypvt[-1].NumInitialiser < 0 || (yypvt[-1].NumInitialiser > 255 && TargetCharacterSet != String::Unicode))
		{
		    ErrorHandler::OutputErrorLine("Character code must be a number in the range 0 to 255.");
		    exit(1);
		}
	    if(yypvt[-1].NumInitialiser > 0xffff)
		{
		    ErrorHandler::OutputErrorLine("Unicode character code must be a number in the range 0 to 65535.");
		    exit(1);
		}
	    if (TargetCharacterSet != String::Unicode)
		{
		* yyval.Value = char(yypvt[-1].NumInitialiser); * (yyval.Value + 1) = '\0'; 
		} 
	    else
		{
		if (SourceCharacterSet == String::CP1252)
		    {
		    if ( (yypvt[-1].NumInitialiser >= 0x80) && (yypvt[-1].NumInitialiser <= 0x9F ) ) // 80-9F are illegal Unicode values.
			{
			ErrorHandler::OutputErrorLine("Warning: Deprecated non-unicode value in source stream");
			}
		    * yyval.Value = char(UnicodeEscape);
		    asUTF8(yyval.Value + 1, yypvt[-1].NumInitialiser);
		    }
		else
		if (SourceCharacterSet == String::UTF8)
		    {
		    asUTF8(yyval.Value, yypvt[-1].NumInitialiser);
		    }
		else
		    {
		    // Unsatisfactory, but do people use other character sets?
		    if (yypvt[-1].NumInitialiser > 255)
			{
			ErrorHandler::OutputErrorLine("Don't know how to handle character > 255");
			}
		    * yyval.Value = char(yypvt[-1].NumInitialiser); * (yyval.Value + 1) = '\0'; 
		    }
		}
	    
} break;

case YYr106: {	/* name-statement :  L_NAME L_LABEL */
#line 635 "../rcomp.y"

	    REGISTER_LINE;
	    SetIdFromName(yypvt[0].Value);
	    
} break;

case YYr107: {	/* name-statement :  L_NAME L_STRING_LITERAL */
#line 640 "../rcomp.y"

	    REGISTER_LINE;
	    SetIdFromName(yypvt[0].Value);
	    
} break;

case YYr108: {	/* character_set-statement :  L_CHARACTER_SET L_LABEL */
#line 655 "../rcomp.y"
	if(verbose) {	MOFF;cout << "character_set-statement                    " << yypvt[0].Value << endl;MON;}
											REGISTER_LINE;
											SourceCharacterSet = CharacterSetID(yypvt[0].Value);
											if ( SourceCharacterSet == String::UNKNOWN )
											{
												String err = "Warning: Unrecognised character set name '";
												err += yypvt[0].Value;
												err += "'";
												ErrorHandler::OutputErrorLine(err);
											}
											if ( SourceCharacterSet == String::Unicode )
											{
											    SourceCharacterSet = String::UNKNOWN;
												ErrorHandler::OutputErrorLine("Unicode source is unsupported");
											}
										
} break;

case YYr109: {	/* offset-statement :  L_OFFSET natural-expression */
#line 679 "../rcomp.y"
	if(verbose) {	RCTypeArray Types;
															MOFF;cout << "offset-statement                           " << yypvt[0].Value << endl;MON; }
											REGISTER_LINE;
										 	CurrentId=((long) NumericValue(yypvt[0].Value, L_LONG).GetULong() );
										
} break;

case YYr110: {	/* system-statement :  L_SYSTEM */
#line 690 "../rcomp.y"
	if(verbose) {	MOFF;cout << "system-statement" << endl;MON;}
											CurrentIdStep=-1;
										
} break;

case YYr113: {	/* enum-statement-start :  L_ENUM L_LABEL '{' */
#line 704 "../rcomp.y"
	
	    if(verbose) 
		{ MOFF;cout << "enum-statement" << endl;MON;} 
	    CurrentEnumName = yypvt[-1].Value;
	    CurrentEnumValue=0;
	    
} break;

case YYr114: {	/* enum-statement-start :  L_ENUM '{' */
#line 711 "../rcomp.y"
	
	    if(verbose) 
		{ MOFF;cout << "enum-statement" << endl;MON;} 
	    CurrentEnumName = "";
	    CurrentEnumValue=0;
	    
} break;

case YYr115: {	/* enum-list-entry :  L_LABEL */
#line 720 "../rcomp.y"
	pG->EnumValues.Add(yypvt[0].Value, CurrentEnumValue++);
} break;

case YYr116: {	/* enum-list-entry :  L_LABEL '=' simple-initialiser */
#line 722 "../rcomp.y"
	
	    CurrentEnumValue = atol(yypvt[0].Value);
	    pG->EnumValues.Add(yypvt[-2].Value, CurrentEnumValue);
	    CurrentEnumValue++;			// Increment so that next field has value (yypvt[0].Value+1)
	    
} break;

case YYr117: {	/* enum-list :  enum-list-entry */
#line 731 "../rcomp.y"

} break;

case YYr118: {	/* enum-list :  enum-list ',' enum-list-entry */
#line 732 "../rcomp.y"

} break;
#line 343 "t:/mks/etc/yyparse.cpp"
	case YYrACCEPT:
		YYACCEPT;
	case YYrERROR:
		goto yyError;
	}
yyNext:
	/*
	 *	Look up next state in goto table.
	 */

	yyp = &yygo[yypgo[yyi]];
	yyq = yyp++;
	yyi = *yyps;
	while (yyi < *yyp++)		/* busy little loop */
		;
	yystate = ~(yyi == *--yyp? yyq[yyq-yyp]: *yyq);
#if YYDEBUG
	if (yydebug)
		YY_TRACE(yyShowGoto)
#endif
	goto yyStack;

yyerrlabel:	;		/* come here from YYERROR	*/
	yyerrflag = 1;
	if (yyi == YYrERROR) {
		yyps--, yypv--;
#if YYDEBUG
		if (yydebug) yytp--;
#endif
	}
	
yyError:
	switch (yyerrflag) {

	case 0:		/* new error */
		yynerrs++;
		yyi = yychar;
		scan->yyerror("Syntax error");
		if (yyi != yychar) {
			/* user has changed the current token */
			/* try again */
			yyerrflag++;	/* avoid loops */
			goto yyEncore;
		}

	case 1:		/* partially recovered */
	case 2:
		yyerrflag = 3;	/* need 3 valid shifts to recover */
			
		/*
		 *	Pop states, looking for a
		 *	shift on `error'.
		 */

		for ( ; yyps > stateStack; yyps--, yypv--
#if YYDEBUG
					, yytp--
#endif
		) {
#ifdef YACC_WINDOWS
			if (*yyps >= Sizeof_yypact) 	/* simple state */
#else /* YACC_WINDOWS */
			if (*yyps >= sizeof yypact/sizeof yypact[0])
#endif /* YACC_WINDOWS */
				continue;
			yyp = &yyact[yypact[*yyps]];
			yyq = yyp;
			do
				;
			while (YYERRCODE < *yyp++);
			if (YYERRCODE == yyp[-1]) {
				yystate = ~yyq[yyq-yyp];
				goto yyStack;
			}
				
			/* no shift in this state */
#if YYDEBUG
			if (yydebug && yyps > stateStack+1)
				YY_TRACE(yyShowErrRecovery)
#endif
			/* pop stacks; try again */
		}
		/* no shift on error - abort */
		break;

	case 3:
		/*
		 *	Erroneous token after
		 *	an error - discard it.
		 */

		if (yychar == 0)  /* but not EOF */
			break;
#if YYDEBUG
		if (yydebug)
			YY_TRACE(yyShowErrDiscard)
#endif
		yyclearin();
		goto yyEncore;	/* try again in same state */
	}
	YYABORT;

}
#if YYDEBUG
/*
 * Return type of token
 */
int
yy_parse::yyGetType(int tok)
{
	yyNamedType * tp;
	for (tp = &yyTokenTypes[yyntoken-1]; tp > yyTokenTypes; tp--)
		if (tp->token == tok)
			return tp->type;
	return 0;
}

	
// Print a token legibly.
char *
yy_parse::yyptok(int tok)
{
	yyNamedType * tp;
	for (tp = &yyTokenTypes[yyntoken-1]; tp > yyTokenTypes; tp--)
		if (tp->token == tok)
			return tp->name;
	return "";
}
/*
 * Read state 'num' from YYStatesFile
 */
#ifdef YYTRACE

char *
yy_parse::yygetState(int num)
{
	int	size;
	char	*cp;
	static FILE *yyStatesFile = (FILE *) 0;
	static char yyReadBuf[YYMAX_READ+1];

	if (yyStatesFile == (FILE *) 0
	 && (yyStatesFile = fopen(YYStatesFile, "r")) == (FILE *) 0)
		return "yyExpandName: cannot open states file";

	if (num < yynstate - 1)
		size = (int)(States[num+1] - States[num]);
	else {
		/* length of last item is length of file - ptr(last-1) */
		if (fseek(yyStatesFile, 0L, 2) < 0)
			goto cannot_seek;
		size = (int) (ftell(yyStatesFile) - States[num]);
	}
	if (size < 0 || size > YYMAX_READ)
		return "yyExpandName: bad read size";
	if (fseek(yyStatesFile, States[num], 0) < 0) {
	cannot_seek:
		return "yyExpandName: cannot seek in states file";
	}

	(void) fread(yyReadBuf, 1, size, yyStatesFile);
	yyReadBuf[size] = '\0';
	return yyReadBuf;
}
#endif /* YYTRACE */
/*
 * Expand encoded string into printable representation
 * Used to decode yyStates and yyRules strings.
 * If the expansion of 's' fits in 'buf', return 1; otherwise, 0.
 */
int
yy_parse::yyExpandName(int num, int isrule, char * buf, int len)
{
	int	i, n, cnt, type;
	char	* endp, * cp, * s;

	if (isrule)
		s = yyRules[num].name;
	else
#ifdef YYTRACE
		s = yygetState(num);
#else
		s = "*no states*";
#endif

	for (endp = buf + len - 8; *s; s++) {
		if (buf >= endp) {		/* too large: return 0 */
		full:	(void) strcpy(buf, " ...\n");
			return 0;
		} else if (*s == '%') {		/* nonterminal */
			type = 0;
			cnt = yynvar;
			goto getN;
		} else if (*s == '&') {		/* terminal */
			type = 1;
			cnt = yyntoken;
		getN:
			if (cnt < 100)
				i = 2;
			else if (cnt < 1000)
				i = 3;
			else
				i = 4;
			for (n = 0; i-- > 0; )
				n = (n * 10) + *++s - '0';
			if (type == 0) {
				if (n >= yynvar)
					goto too_big;
				cp = yysvar[n];
			} else if (n >= yyntoken) {
			    too_big:
				cp = "<range err>";
			} else
				cp = yyTokenTypes[n].name;

			if ((i = strlen(cp)) + buf > endp)
				goto full;
			(void) strcpy(buf, cp);
			buf += i;
		} else
			*buf++ = *s;
	}
	*buf = '\0';
	return 1;
}
#ifndef YYTRACE
/*
 * Show current state of yyparse
 */
void
yy_parse::yyShowState()
{
	(void) printf("state %d (%d), char %s (%d)\n%d stateStack entries\n",
		yysmap[yystate],yystate,yyptok(yychar),yychar,
		yypv - valueStack);
}
// show results of reduction: yyi is rule number
void
yy_parse::yyShowReduce()
{
	(void) printf("Reduce by rule %d (pop#=%d)\n", yyrmap[rule], npop);
}
// show read token
void
yy_parse::yyShowRead()
{
	(void) printf("read %s (%d)\n", yyptok(yychar), yychar);
}
// show Goto
void
yy_parse::yyShowGoto()
{
	(void) printf("goto %d (%d)\n", yysmap[yystate], yystate);
}
// show Shift
void
yy_parse::yyShowShift()
{
	(void) printf("shift %d (%d)\n", yysmap[yystate], yystate);
}
// show error recovery
void
yy_parse::yyShowErrRecovery()
{
	(void) printf("Error recovery pops state %d (%d), uncovers %d (%d)\n",
		yysmap[*(yyps-1)], *(yyps-1), yysmap[yystate], yystate);
}
// show token discards in error processing
void
yy_parse::yyShowErrDiscard()
{
	(void) printf("Error recovery discards %s (%d), ",
		yyptok(yychar), yychar);
}
#endif	/* ! YYTRACE */
#endif	/* YYDEBUG */
#line 738 "../rcomp.y"
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



