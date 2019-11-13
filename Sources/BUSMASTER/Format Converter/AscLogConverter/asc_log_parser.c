
/*  A Bison parser, made from ..\..\sources\busmaster\format converter\asclogconverter\asc_log_parser.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

#define	HEXNUMBER	258
#define	NUMBER	259
#define	TOKHEAT	260
#define	STATE	261
#define	TOKTARGET	262
#define	TOKTEMPERATURE	263
#define	DOUBLEVAL	264
#define	MSGDIR	265
#define	DATAMSG	266
#define	LENGTHTOKEN	267
#define	BITCOUNTTOKEN	268
#define	EQUAL	269
#define	EXTID	270
#define	MONTH_BASE	271
#define	DAY	272
#define	FULLTIME	273
#define	DATETOKEN	274
#define	REMOTE	275
#define	IGNORE	276
#define	TEXT	277
#define	COLON	278
#define	AM_PM	279
#define	BASETOKEN	280
#define	BASE	281
#define	NO_INTERNAL_EVENTS_LOGGED_TOKEN	282
#define	TIMESTAMPSTOKEN	283
#define	TIMEMODE	284
#define	BEGINTRIGGERTOKEN	285
#define	LINEEND	286
#define	ENDTRIGGERTOKEN	287



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define TIME_MODE_ABSOLUTE			 0
#define TIME_MODE_RELATIVE			 1
#define	TIME_MODE_UNDEFINED			-1

#define NUMERIC_MODE_DEC			0
#define NUMERIC_MODE_HEX			1
#define NUMERIC_MODE_NONE			-1

#define DEF_LOG_END_TEXT			"\n***END DATE AND TIME ***"
// PTV[1.6.4]
// Added new line
#define	DEF_LOG_STOP_TEXT			"\n***[STOP LOGGING SESSION]***\r\n"

#define	MAX_LOG_HEADER				400

int nLen = 0;
int nTimeMode = TIME_MODE_ABSOLUTE;
int nNumLines = 0;
int nNumericMode = NUMERIC_MODE_HEX;
char data[256];

int nDay = 1;
int nMonth = 1;
int nYear = 2000;
int nHour = 1;
int nMins = 1;
int nSec = 1;
int nHeaderFound = 0;
int nTimeStampFound = 0;
char gchErrorString[255] = {'\0'};



extern FILE *yyin, *yyout;

void yyerror(const char *str)
{
	//fprintf(stderr,"error: %s\n",str);
}

int yywrap()
{
	// PTV[1.6.4]
	//fprintf(yyout, DEF_LOG_END_TEXT);
	//fprintf(yyout, DEF_LOG_STOP_TEXT);
	// PTV[1.6.4]
	return 1;	//1 Sepcifies conversion is over.
				//0 specifies the parser start the conversion from different
				//Input
}
int nInitialiseDataBytes()
{
	memset(data, 0, 256);
}
int nSecondToTime(unsigned int nSeconds, int *nHours, int *nMin, int *nSec)
{
	*nHours = nSeconds / 3600; 
	nSeconds = nSeconds -(*nHours * 3600);
	*nMin = nSeconds / 60; 
	*nSec = nSeconds - (*nMin) * 60;
}
int nGetLogTimeStamp(char* pchAscTime, char* pchLogTime)
{
	char chSeparators[] = ".";
	char* pchTemp;
	int nLen = 0;
	int i = 0;
	int nMax = 0;
	char chMilliSeconds[5] = {'0'};
	
	int nHours, nMin, nSec;
	unsigned int unSeconds;
	double fMilli = 0;
	chMilliSeconds[4] = '\0';
	memset(chMilliSeconds, 0x30, 4);
	pchTemp = strtok(pchAscTime, chSeparators);
	unSeconds = strtoul(pchTemp, NULL, 10);
	nSecondToTime(unSeconds, &nHours, &nMin, &nSec);
	
	pchTemp = strtok(NULL, chSeparators);
	nLen = strlen(pchTemp);
	nMax = min(nLen, 4);
	for ( i=0; i < nMax ; i++ )
	{
		chMilliSeconds[i] = pchTemp[i];
	}
	
	sprintf(pchLogTime, "%u:%u:%u:%s", nHours, nMin, nSec, chMilliSeconds);
}
int nInitialiseDefaultValues()
{
	nLen = 0;
	nTimeMode = TIME_MODE_ABSOLUTE;
	nNumLines = 0;
	nNumericMode = NUMERIC_MODE_HEX;
	nDay = 1;
	nMonth = 1;
	nYear = 2000;
	nHour = 1;
	nMins = 1;
	nSec = 1;
	nHeaderFound = 0;
	nTimeStampFound = 0;
	strcpy(gchErrorString, "Conversion Completed Successfully");
	
	nInitialiseDataBytes();
}
int nConvertFile(FILE* fpInputFile, FILE* fpOutputFile)
{
int i =0;
	if( (NULL != fpInputFile) && (NULL != fpOutputFile) )
	{ 
		yyin = fpInputFile;
		yyout = fpOutputFile;

		nInitialiseDefaultValues();
		
		//Add Dummy Header with initial values for log file.This will be replaced with the proper values
		//afrter conversion.
		for( i = 0 ; i < MAX_LOG_HEADER; i++ )
		{
			fprintf(yyout, " ");
		}

		yyparse();
		
		//Actual Header
		nAddFunctionHeader();

		if ( nTimeStampFound == 0 )
		{
			//TODO::
			strcpy(gchErrorString, "Proper Header was not found in ASC File.Logging Mode is Assumed as Absolute & Numeric Format Assumed as Hex");
		}


	}
	return 0;
}
int nAddFunctionHeader()
{
	fseek(yyout, 0L, SEEK_SET );

	fprintf(yyout, "***BUSMASTER Ver %s***\n", BUSMASTER_VER);
	fprintf(yyout, "***PROTOCOL CAN***\n");
	fprintf(yyout, "***NOTE: PLEASE DO NOT EDIT THIS DOCUMENT***\n");
	fprintf(yyout, "***[START LOGGING SESSION]***\n");
	fprintf(yyout,"***START DATE AND TIME %d:%d:%d %d:%d:%d:%s%s", nDay, nMonth, nYear, nHour, nMins, nSec, "000", "***\n");
		
	
	
	if ( NUMERIC_MODE_DEC == nNumericMode )
	{
		fprintf(yyout, "***DEC***\n");
	}
	else
	{	
		fprintf(yyout, "***HEX***\n");
	}

	if ( nTimeMode == TIME_MODE_RELATIVE )
	{
		fprintf(yyout, "***RELATIVE MODE***\n");
	}
	else
	{
		fprintf(yyout, "***ABSOLUTE MODE***\n");
	}
	fprintf(yyout,"***START DATABASE FILES***\n");

	fprintf(yyout,"***END DATABASE FILES***\n");

	fprintf(yyout,"***<Time><Tx/Rx><Channel><CAN ID><Type><DLC><DataBytes>***");

    return 0;
}
int GetMonth(char* pchValue)
{
	int nMonth = 1;
	if( strcmp("Jan", pchValue) == 0 )
			nMonth = 1;
	else if( strcmp("Feb", pchValue) == 0 )
		nMonth = 2; 
	else if( strcmp("Mar", pchValue) == 0 )
		nMonth = 3; 
	else if( strcmp("Apr", pchValue) == 0 )
		nMonth = 4; 
	else if( strcmp("May", pchValue) == 0 )
		nMonth = 5; 
	else if( strcmp("Jun", pchValue) == 0 )
		nMonth = 6; 
	else if( strcmp("Jul", pchValue) == 0 )
		nMonth = 7; 
	else if( strcmp("Aug", pchValue) == 0 )
		nMonth = 8; 
	else if( strcmp("Sep", pchValue) == 0 )
		nMonth = 9; 
	else if( strcmp("Oct", pchValue) == 0 )
		nMonth = 10; 
	else if( strcmp("Nov", pchValue) == 0 )
		nMonth = 11; 
	else if( strcmp("Dec", pchValue) == 0 )
		nMonth = 12; 
	return nMonth;
}

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		56
#define	YYFLAG		-32768
#define	YYNTBASE	34

#define YYTRANSLATE(x) ((unsigned)(x) <= 287 ? yytranslate[x] : 50)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,    33,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     5,     7,     9,    11,    13,    15,    17,    19,
    21,    23,    25,    27,    29,    31,    33,    34,    37,    41,
    45,    53,    61,    67,    73,    78,    82,    90,    97,    99,
   101
};

static const short yyrhs[] = {    35,
     0,    34,    35,     0,     1,     0,    39,     0,    49,     0,
    40,     0,    41,     0,    42,     0,    43,     0,    44,     0,
    48,     0,    37,     0,    45,     0,    46,     0,    38,     0,
    47,     0,     0,    36,     4,     0,    12,    14,     4,     0,
    13,    14,     4,     0,     9,     4,     4,    10,     4,     4,
    36,     0,     9,     4,    15,    10,     4,     4,    36,     0,
     9,     4,    15,    10,    20,     0,     9,     4,     4,    10,
    20,     0,    25,    16,    28,    29,     0,    25,    16,    27,
     0,    19,    17,    16,     4,    18,    24,     4,     0,    19,
    17,    16,     4,    18,     4,     0,    31,     0,    32,     0,
     1,    33,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
   219,   220,   223,   225,   227,   229,   231,   233,   235,   237,
   239,   242,   244,   246,   248,   250,   254,   255,   278,   283,
   289,   319,   349,   378,   408,   436,   454,   495,   524,   531,
   538
};

static const char * const yytname[] = {   "$","error","$undefined.","HEXNUMBER",
"NUMBER","TOKHEAT","STATE","TOKTARGET","TOKTEMPERATURE","DOUBLEVAL","MSGDIR",
"DATAMSG","LENGTHTOKEN","BITCOUNTTOKEN","EQUAL","EXTID","MONTH_BASE","DAY","FULLTIME",
"DATETOKEN","REMOTE","IGNORE","TEXT","COLON","AM_PM","BASETOKEN","BASE","NO_INTERNAL_EVENTS_LOGGED_TOKEN",
"TIMESTAMPSTOKEN","TIMEMODE","BEGINTRIGGERTOKEN","LINEEND","ENDTRIGGERTOKEN",
"';'","commands","command","Can_Data_Bytes","Msg_Len","Bit_Count","Standard_Msg",
"Extended_Msg","ExtendedRTR_Msg","RemoteFrame","Base_TimeStamps","Base_No_Internal_Events",
"Log_Creation_Time","Log_Creation","Line_End","End_Statement","stmnt",""
};
#endif

static const short yyr1[] = {     0,
    34,    34,    35,    35,    35,    35,    35,    35,    35,    35,
    35,    35,    35,    35,    35,    35,    36,    36,    37,    38,
    39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
    49
};

static const short yyr2[] = {     0,
     1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     0,     2,     3,     3,
     7,     7,     5,     5,     4,     3,     7,     6,     1,     1,
     2
};

static const short yydefact[] = {     0,
     3,     0,     0,     0,     0,     0,    29,    30,     0,     1,
    12,    15,     4,     6,     7,     8,     9,    10,    13,    14,
    16,    11,     5,    31,     0,     0,     0,     0,     0,     2,
     0,     0,    19,    20,     0,    26,     0,     0,     0,     0,
    25,     0,    24,     0,    23,     0,    17,    17,    28,     0,
    21,    22,    27,    18,     0,     0
};

static const short yydefgoto[] = {     9,
    10,    51,    11,    12,    13,    14,    15,    16,    17,    18,
    19,    20,    21,    22,    23
};

static const short yypact[] = {     2,
   -29,     6,    -8,    12,    11,    13,-32768,-32768,     0,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,     1,    16,    26,    19,   -10,-32768,
    27,    28,-32768,-32768,    32,-32768,    10,     3,     4,    22,
-32768,    37,-32768,    38,-32768,    -2,-32768,-32768,-32768,    39,
    40,    40,-32768,-32768,    45,-32768
};

static const short yypgoto[] = {-32768,
    41,    -1,-32768,-32768,-32768,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768
};


#define	YYLAST		50


static const short yytable[] = {    55,
     1,    49,     1,    24,    31,    26,    42,    44,     2,    25,
     2,     3,     4,     3,     4,    32,    36,    37,     5,    33,
     5,    50,    43,    45,     6,    27,     6,    28,    29,    34,
     7,     8,     7,     8,    35,    40,    38,    39,    41,    46,
    47,    48,    53,    54,    56,     0,    52,     0,     0,    30
};

static const short yycheck[] = {     0,
     1,     4,     1,    33,     4,    14,     4,     4,     9,     4,
     9,    12,    13,    12,    13,    15,    27,    28,    19,     4,
    19,    24,    20,    20,    25,    14,    25,    17,    16,     4,
    31,    32,    31,    32,    16,     4,    10,    10,    29,    18,
     4,     4,     4,     4,     0,    -1,    48,    -1,    -1,     9
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */


/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif



/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 3:
{yyerrok;yyclearin; ;
    break;}
case 18:
{
		int nStrlen = strlen((char*)yyvsp[0]);
		int i=0;
		char o = '0';
		//prefix zeros to get 3-digits per byte.
		if(nNumericMode == NUMERIC_MODE_DEC)
		{
			if(nStrlen < 3)
			{
				for(i=0 ; i<(3-nStrlen); i++)
				{
					strcpy((data+nLen) ,&o);
					nLen += 1;
				}
			}
		}
		strcpy((data+nLen) , (char*)yyvsp[0]);
		nLen += strlen((char*)yyvsp[0]);
		data[nLen] = ' ';
		nLen++;
		data[nLen] = '\0';
	;
    break;}
case 19:
{
		yyval = yyvsp[0];
	;
    break;}
case 20:
{
		yyval = yyvsp[0];
	;
    break;}
case 21:
{
	        
		char chLogTime[256] = {'\0'};
		char chId[12] = {'\0'};

		nGetLogTimeStamp((char*)yyvsp[-6], chLogTime);
		
		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,yyvsp[-4]);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, yyvsp[-4]);
		}
		fprintf(yyout, "\n%s %s %s %s %s %s %s", chLogTime, yyvsp[-3], yyvsp[-5], chId, "s", yyvsp[-1], data);
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	;
    break;}
case 22:
{
		char chLogTime[256] = {'\0'};
		
		char chId[12] = {'\0'};
		nGetLogTimeStamp((char*)yyvsp[-6], chLogTime);
		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,yyvsp[-4]);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, yyvsp[-4]);
		}

		
		fprintf(yyout, "\n%s %s %s %s %s %s %s", chLogTime, yyvsp[-3], yyvsp[-5], chId, "x", yyvsp[-1], data);


		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);*/
	;
    break;}
case 23:
{
		
		char chLogTime[256] = {'\0'};
		char chId[12] = {'\0'};		
		nGetLogTimeStamp((char*)yyvsp[-4], chLogTime);



		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,yyvsp[-2]);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, yyvsp[-2]);
		}
		
		fprintf(yyout,"\n%s %s %s %s %s %s %s", chLogTime, yyvsp[-1], yyvsp[-3], chId, "xr", "0",  "");
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);*/
	;
    break;}
case 24:
{
		
		char chLogTime[256] = {'\0'};
		char chId[12] = {'\0'};		
		nGetLogTimeStamp((char*)yyvsp[-4], chLogTime);



		if (NUMERIC_MODE_DEC == nNumericMode )
		{
			strcpy(chId,yyvsp[-2]);	
		}
		else
		{
			strcpy(chId,"0x");
			strcat(chId, yyvsp[-2]);
		}
		
		fprintf(yyout,"\n%s %s %s %s %s %s %s", chLogTime, yyvsp[-1], yyvsp[-3], chId, "sr", "0",  "");
		nLen = 0;
		nInitialiseDataBytes();
		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);*/
	;
    break;}
case 25:
{
		//Default
		nTimeStampFound = 1;
		if(_stricmp ("dec", (char*)yyvsp[-2]) == 0)
		{
			nNumericMode = NUMERIC_MODE_DEC;
			
		}
		else
		{
			nNumericMode = NUMERIC_MODE_HEX;
		}
		if(strcmp("relative", (char*)yyvsp[0]) == 0)
		{
			nTimeMode = TIME_MODE_RELATIVE;
		}
		else
		{
			nTimeMode = TIME_MODE_ABSOLUTE;
		}
		
		/*free($1);
		free($2);
		free($3);
		free($4);*/
	;
    break;}
case 26:
{
		//Default
		nTimeStampFound = 1;
		if(_stricmp ("dec", (char*)yyvsp[-1]) == 0)
		{
			nNumericMode = NUMERIC_MODE_DEC;
		}
		else
		{
			nNumericMode = NUMERIC_MODE_HEX;
		}
		nTimeMode = TIME_MODE_ABSOLUTE;
		free(yyvsp[-2]);
		free(yyvsp[-1]);
		free(yyvsp[0]);
	;
    break;}
case 27:
{	
		/*date Wed Dec 7 12:23:39 pm 2011*/
		/*8:12:2011 20:15:28:553****/

		
		char chSeparators[]   = " :,\t\n";
		char* chTemp;
		
		
		nMonth = GetMonth((char*)yyvsp[-4]);
		nHeaderFound = 1;
		
		
		chTemp = strtok((char*)yyvsp[-2], chSeparators);
		nHour = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nMins = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nSec = atoi(chTemp);
		
		nHour = nHour % 12;
		if( strcmp("pm", (char*)yyvsp[-1]) == 0 )
		{
			nHour = nHour + 12;
		}
		
		nDay = atoi( (char*)yyvsp[-3] );
		nYear = atoi( (char*)yyvsp[0] );

		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	;
    break;}
case 28:
{	
		/*date Wed Dec 7 12:23:39 pm 2011*/
		
		char chSeparators[]   = " :,\t\n";
		char* chTemp;
		nMonth = GetMonth((char*)yyvsp[-3]);
		/*8:12:2011 20:15:28:553****/
		
		chTemp = strtok((char*)yyvsp[-1], chSeparators);
		nHour = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nMins = atoi(chTemp);
		chTemp = strtok( NULL, chSeparators ); 
		nSec = atoi(chTemp);
		
		nDay = atoi( (char*)yyvsp[-2] );
		nYear = atoi( (char*)yyvsp[0] );

		/*free($1);
		free($2);
		free($3);
		free($4);
		free($5);
		free($6);
		free($7);*/
	;
    break;}
case 29:
{
		nNumLines++;
	;
    break;}
case 30:
{
	fprintf(yyout, DEF_LOG_END_TEXT);
	fprintf(yyout, DEF_LOG_STOP_TEXT);
	;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */


  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
