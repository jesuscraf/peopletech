//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         gram.y
// File:                Gram.y
// Project:        Ln4
// Author:         Meta Software M.S. , S.A
// Date:            05 May 97
// Language:        Yacc
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    This module defines la gramática de LN4
////
//==============================================================================

%{


#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "clparsermap.h"

#define MAX_TOKEN 256
int iNumLine;
%}


%token IDENTIFICADOR NUMERO FECHA LITERAL NL EOF2 
%token IF THEN ELSE ENDIF WHILE WEND FOR TO NEXT DO RETURN UNTIL ELSEIF
%right IGUAL
%left AND OR 
%nonassoc MAYIGUAL MENIGUAL MAYOR MENOR DISTINTO NOT
%left MAS MENOS
%left MULTI DIVIDE
%left ACCMIEMBRO ACCPADRE ACCATRIB ACCCHANNEL
%left '(' ')' '[' ']'
%token SEPINST
%left MENOSUNARIO

%union{
	/* Se debe cambiar por una estructura, en vez de una union */
	char str[MAX_TOKEN];
	char *pstr;	/* Cadenas de longitud >MAX_TOKEN. */
}

%%

programa:	listasentencias /*EOF2*/
			;

listasentencias:	/*vacia*/
					|instnlin
					
					;

instnlin:	error newline		{yyerrok;}
			|inst1linTE
			|instnlin inst1linTE
			|instnlin if cond then newline instnlin endifm newline
			|if cond then newline instnlin endifm newline
			|instnlin if cond then newline instnlin elsebody endifm newline
			|if cond then newline instnlin elsebody endifm newline
			|while cond newline instnlin wend newline
			|for asigfor to finfor newline instnlin next newline
			|instnlin while cond newline instnlin wend newline
			|instnlin for asigfor to finfor newline instnlin next newline
			|do newline instnlin until cond newline
			|instnlin do newline instnlin until cond newline
			;

elsebody:	else newline instnlin						
			|elseif cond then newline instnlin elsebody	
			|elseif cond then newline instnlin			
			;

if:			IF										{If();}
			;

then:		THEN									{Then();}
			;

else:		ELSE									{Else();}
			;

elseif:		ELSEIF									{Elseif();}
			;

endifs:		newline										{Endif();}
			;

endifm:		ENDIF									{Endif();}
			;

do:			DO										{Do();}
			;

for:		FOR										{For();}
			;

to:			TO										{To();}
			;

next:		NEXT									{Next();}
			;

while:		WHILE									{While();}
			;

wend:		WEND									{Wend();}
			;

until:		UNTIL									{Until();}
			;

inst1linTE:	newline
			| inst1lin newline
			| if cond then inst1lin endifs
			| if cond then inst1lin else inst1lin endifs
			;

inst1lin:	instsimple
			|instsimple SEPINST inst1lin
			;

instsimple:	target IGUAL exp		{Assign();}
			| RETURN exp			{Return();}
			/*| funcion lista_arg */
			| itemmetodo			{Purge();}
			| atributometodo		{Purge();}
			| channelfunction		{Purge();}
			;

asigfor:	IDENTIFICADOR IGUAL exp	{StoreFor($<str>1); PopIdentifier();}
			;

newline:	NL						{WriteLine();}

finfor:		term					{FinFor();}
			| exp MAS term			{AritmOpAdd();FinFor();}
			| exp MENOS term			{AritmOpSub();FinFor();}
			;

target:		/*IDENTIFICADOR
			| */itemvalor			{strcpy($<str>$,$<str>1);Target($<str>1);}
			;

cond:		expcond					{Condition();}

expcond:	termC					
			|expcond OR termC			{LogOpOr();}
			;

termC:		factorC
			|termC AND factorC		{LogOpAnd();}
			;

factorC:	primaryC
			|NOT '('expcond ')'		{LogOpNot();}
			;

primaryC:	exp IGUAL exp				{LogOpEqual();}
			| exp MAYIGUAL exp			{LogOpGreaterEqual();}
			| exp MENIGUAL exp			{LogOpLessEqual();}
			| exp MAYOR exp				{LogOpGreater();}
			| exp MENOR exp				{LogOpLess();}
			| exp DISTINTO exp			{LogOpNotEqual();}
			| '(' expcond ')'
			;



exp:		term 
			|exp MAS term			{AritmOpAdd();}
			|exp MENOS term			{AritmOpSub();}
			;

term:		factor
			| term MULTI factor		{AritmOpMul();}
			| term DIVIDE factor	{AritmOpDiv();}
			;

factor:		primary
			| MENOS primary %prec MENOSUNARIO		{AritmOpSigno();}
			;

primary:	'(' exp ')'				
			| NUMERO				{StoreNumber($<str>1);}
			| LITERAL				{	//Antes: {StoreString($<pstr>1);}
										if ($<hString>1 != 0) {
											StoreString( (m_oScanner.GetPool())->GetString($<hString>1) );
											//Libero memoria reservada en lexyy.cpp: 
											// en yyFlexLexer::yylex() -> case 36!!!
											(m_oScanner.GetPool())->DeleteString($<hString>1);	//Borramos la cadena dinamica del LITERAL
										}
									}
			/*| IDENTIFICADOR*/			
			| FECHA					{StoreDate($<str>1);}
			/*| funcion lista_arg*/
			| itemvalor				{ItemValorRead($<str>1);}
			| itemrvalue			
			;

itemvalor:	celda					{strcpy($<str>$,$<str>1);}
			;

itemrvalue: itemmetodo				
			|atributo				
			|atributometodo	
			|channelfunction		
			;

channelfunction:	IDENTIFICADOR ACCCHANNEL IDENTIFICADOR lista_arg		{ChannelFunction( $<str>1 , $<str>3 , $<str>4 );PopIdentifier();PopIdentifier();}
			;

itemmetodo:	celda lista_arg									{Metodo($<str>1,$<str>2);}	
			|celda lista_arg ACCATRIB IDENTIFICADOR			{MetodoRegla($<str>1,$<str>4,$<str>2);PopIdentifier();}/*el ultimo id es la regla particular*/
			|ACCPADRE celda lista_arg							{MetodoPadre($<str>2,$<str>3);}
			|ACCPADRE celda lista_arg ACCATRIB IDENTIFICADOR	{MetodoReglaPadre($<str>2,$<str>5,$<str>3);PopIdentifier();}/*el ultimo id es la regla particular*/
			;

atributo:	celda ACCATRIB IDENTIFICADOR				{Atributo($<str>1,$<str>3);PopIdentifier();}	
			;

atributometodo: celda ACCATRIB IDENTIFICADOR lista_arg		{AtributoMetodo($<str>1,$<str>3,$<str>4);PopIdentifier();}
				;


celda:		itemname							{ strcpy($<str>$,$<str>1);}
			| nodename ACCMIEMBRO itemname		{ sprintf ($<str>$, "%s.%s", $<str>1 , $<str>3); }
			;

itemname:	IDENTIFICADOR						{ strncpy($<str>$,$<str>1,MAX_TOKEN-1); /*MAX_TOKEN es la máx long de id->variable*/
                                                  $<str>$[MAX_TOKEN-1]=0; /*Nulterminamos por si acaso*/
												  NotifyPosibleItem();
                                                }
			| IDENTIFICADOR '[' tramo_row ']'	{ sprintf ($<str>$, "%.30s[]", $<str>1);
												  NotifyPosibleItem();
												} /*el [] indica que hay tramo //30 es la máx long de item_id*/
			;

nodename:	IDENTIFICADOR						{ strncpy($<str>$,$<str>1,30); /*long maxima=30*/
                                                    $<str>$[30]=0;/*nulterminamos*/
													NotifyPosibleTI();
                                                }
			| IDENTIFICADOR '[' tramo_row ']'	{ sprintf ($<str>$, "%.30s<>", $<str>1);
												  NotifyPosibleTI();
												} /*el <> indica que se especifica la fila*/ /*30=long maxima*/
			| IDENTIFICADOR ACCCHANNEL IDENTIFICADOR	{sprintf ($<str>$, "%.30s!%.30s", $<str>1 , $<str>3);
														 PopIdentifier();
														 PopIdentifier();// En este caso es nombre de nodo, no cambiamos la TI
														}/*30=long maxima*/
			| IDENTIFICADOR ACCCHANNEL IDENTIFICADOR '[' tramo_row ']'	{ sprintf ($<str>$, "%.30s!%.30s<>", $<str>1 , $<str>3);
																		  PopIdentifier();
																		  PopIdentifier();// En este caso es nombre de nodo, no cambiamos la TI
																		}/*el <> indica que se especifica la fila*/ /*30=long maxima*/
			;

tramo_row:		exp				/*OJO!!!!el tramo o expresion de fila nos lo metera antes que el item en la polaca. */ 
			;




/*regla:		IDENTIFICADOR		{printf("-regla especifica->%s- @%d\n",$<str>1,iNumLine);}
			;*/
/*funcion:	IDENTIFICADOR   	{printf("-Llamada a función->%s- arg.:%s@%d\n",$<str>1,$<str>3,iNumLine);}
			;*/

arg:		exp					{Arg();}
			;

lista_arg:      init_lista_arg arglist_rec end_lista_arg    {strcpy ($<str>$, $<str>3); }
                ;
init_lista_arg:     '('         {InitArgList();}
                    ;

end_lista_arg:      ')'         {strcpy ($<str>$, itoa(EndArgList(), $<str>$ , 10) ) ;}
                    ;

arglist_rec:	/*vacio*/	 																	
			    | largs																			
    			;

largs:          arg
                | largs ',' arg
                ;

%%

void yyerror(char *s){
	
	//printf("Error de sintaxis en linea %d\n",iNumLine);
	if ( !m_poTraductor->ErrorActive() ) {
		DUMP_CHLOG_ERRORF( M4_CH_VM_SYNTAX_ERROR,  m_poTraductor->GetNumLine()+1  );	// (5): Syntax error in LN4 rule
		m_poTraductor->SetError();
	}
	yychar=0; //lo ponemos a capon a eof para que si hay un error de sintaxis no intente seguir compilado y pare.	
}
