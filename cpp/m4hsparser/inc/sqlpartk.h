


#ifndef __SQLPARTK_H__
#define __SQLPARTK_H__

#include "selectinfo.hpp"


typedef union { 
		char pcValue[80];
		ColumnInfo * poColumn;
		TableInfo * poTable;
		ItemInfo * poItem;
		TextInfo * poText;
		ElementInfo * poElement;
		SelectInfo * poSelect;
		FunctionInfo * poFunction;
		JoinInfo * poJoin;
		} YYSTYPE;
#define	LEXER_ERROR	258
#define	LITERAL_INTEGER	259
#define	LITERAL_FLOAT	260
#define	LITERAL_STR	261
#define	LITERAL_DATE	262
#define	LITERAL_TIME_STAMP	263
#define	IDENTIFIER	264
#define	SELECT	265
#define	FROM	266
#define	GROUP_BY	267
#define	WHERE	268
#define	UNION	269
#define	UNION_ALL	270
#define	INTERSECT	271
#define	IN_KW	272
#define	ALL	273
#define	DISTINCT	274
#define	EXISTS	275
#define	HAVING	276
#define	ORDER_BY	277
#define	ASC	278
#define	DESC	279
#define	OPE_OR	280
#define	OPE_AND	281
#define	OPE_NOT	282
#define	OPE_EQU	283
#define	OPE_NOT_EQU	284
#define	OPE_GT	285
#define	OPE_LT	286
#define	OPE_GE	287
#define	OPE_LE	288
#define	OPE_LIKE	289
#define	OPE_NOT_LIKE	290
#define	OPE_BETWEEN	291
#define	OPE_NOT_BETWEEN	292
#define	OPE_IS_NULL	293
#define	OPE_IS_NOT_NULL	294
#define	OPE_ADD	295
#define	OPE_SUBS	296
#define	OPE_CONCAT_STR	297
#define	OPE_MULT	298
#define	OPE_DIV	299
#define	OPE_MINUS_SIGN	300
#define	OPE_PLUS_SIGN	301
#define	OPEN_PAREN	302
#define	CLOSE_PAREN	303
#define	OPE_OUTER_JOIN	304
#define	OPE_COMMA	305
#define	OPE_POINT	306
#define	OPE_AT	307
#define	OPE_AMP	308
#define	OPE_ALM	309
#define	OPE_DOL	310


//extern YYSTYPE yylval;

#endif //__SQLPARTK_H__
