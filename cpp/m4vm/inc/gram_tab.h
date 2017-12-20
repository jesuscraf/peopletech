
#ifndef GRAM_TAB_H
#define GRAM_TAB_H
#define MAX_TOKEN 256
/*
typedef union{
	char str[MAX_TOKEN];
} YYSTYPE;
*/

//Deberia colar
//typedef struct _YYSTYPE {
typedef union {
	char str[MAX_TOKEN];
	unsigned long hString;	//Handle de la cadena en el Pool Dinamico
	//char *pstr;	//Puntero a memoria
	//Constructor
	/*
	_YYSTYPE() {
		pstr = NULL;
	}
	*/
	//Destructor - no vale porque se llama dos veces
	//habria que redefinir el operador =
	/*
	~_YYSTYPE() {
		if (pstr != NULL) {
			delete []pstr;
			pstr = NULL;
		}
	}
	*/
} YYSTYPE;


#define	IDENTIFICADOR	258
#define	NUMERO	259
#define	FECHA	260
#define	LITERAL	261
#define	NL	262
#define	EOF2	263
#define	IF	264
#define	THEN	265
#define	ELSE	266
#define	ENDIF	267
#define	WHILE	268
#define	WEND	269
#define	FOR	270
#define	TO	271
#define	NEXT	272
#define	DO	273
#define	RETURN	274
#define	UNTIL	275
#define	ELSEIF	276
#define	IGUAL	277
#define	AND	278
#define	OR	279
#define	MAYIGUAL	280
#define	MENIGUAL	281
#define	MAYOR	282
#define	MENOR	283
#define	DISTINTO	284
#define	NOT	285
#define	MAS	286
#define	MENOS	287
#define	MULTI	288
#define	DIVIDE	289
#define	ACCMIEMBRO	290
#define	ACCPADRE	291
#define	ACCATRIB	292
#define	ACCCHANNEL	293
#define	SEPINST	294
#define	MENOSUNARIO	295



#endif
