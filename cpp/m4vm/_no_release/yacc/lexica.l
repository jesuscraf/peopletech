
	#include "flexlexer.h"
	#include "vmres.hpp"
	#include "gram_tab.h"
	#include "chlog.hpp"
	#include "parser.h"
	#include <stdio.h>
	//#include <string.h>
	#include "m4string.hpp"
	#include <ctype.h> 
	//Tamaño de "resize" para un LITERAL en el analizador lexico.
	#define M4CL_VM_LEX_LITERAL_RESIZE						64


 
digito             [0-9]
letra              [a-zA-Z@#$&_]
identificador      ({letra}+({digito}|{letra})*)|("#-"({digito}|{letra})*)
numero             {digito}+(\.{digito}*)?((E|e)[+\-]?{digito}+)?
hexanumber         ("0X"|"0x")[0-9a-fA-F]+
fecha                      \{[ \t]*[0-9][0-9][0-9][0-9]"-"[0-9][0-9]"-"[0-9][0-9]([ \t]+[0-9][0-9](":"[0-9][0-9](":"[0-9][0-9])?)?)?[ \t]*\}
literal            \"
espacio                         [ \t\r]
nl                                      \n
operador                        [+-*/<>=\.!:]
%%
"IF"                {
		    return IF;
		    }

"THEN"              {
		    return THEN;
		    }

"ELSE"              {
		    return ELSE;
		    }
"ELSEIF"                        {
					return ELSEIF;
					}

"ENDIF"             {
		    return ENDIF;
		    }

"WHILE"             {
		    return WHILE;
		    }

"WEND"              {
		    return WEND;
		    }

"FOR"               {
		    return FOR;
		    }

"TO"                {
		    return TO;
		    }

"NEXT"              {
		    return NEXT;
		    }

"DO"                {
		    return DO; 
		    }

"RETURN"            {
		    return RETURN;
		    }

"UNTIL"             {
		    return UNTIL;
		    }

"="                                     {
					return IGUAL;
					}

">="                            {
					return MAYIGUAL;
					}

"<="                            {
					return MENIGUAL;
					}
">"                                     {
					return MAYOR;
					}
"<"                                     {
					return MENOR;
					}

"<>"                            {
					return DISTINTO;
					}

"AND"                           {
					return AND;
					}
"OR"                            {
					return OR;
					}

"NOT"                           {
					return NOT;
					}

"+"                                     {
					return MAS;
					}
"-"                                     {
					return MENOS;
					}
"*"                                     {
					return MULTI;
					}
"/"                                     {
					return DIVIDE;
					}

"!"                                     {
					return ACCCHANNEL;
					}
".."                            {
					return ACCATRIB;
					}
"."                                     {
					return ACCMIEMBRO;
					}
"::"                            {
					return ACCPADRE;
					}
":"                             {
					return SEPINST;
					}

{identificador}     {
					strncpy(yylval.str,(char*)yytext,MAX_TOKEN);
					yylval.str[MAX_TOKEN-1]='\0';
					if (m_poTrad!=NULL)
					{
						m4int32_t p,ilen=strlen(yytext);
						
						p=(((istrstream *)yyin)->rdbuf())->in_avail();
						m_poTrad->PushIdentifier((m4char_t *)yytext,p,ilen);
					}
					return IDENTIFICADOR;
		    }

{numero}            {
	if (strlen((char*)yytext) >MAX_TOKEN-11 ){//el -10: el 'punto', los 8 decimales, el signo, y el /0.
			m_poTrad->GetpParser()->yyerror("number to long",M4_CH_VM_COMP_NUMBER_TOO_BIG);
			return EOF2;//->Forzamos la salida del parser
	}
	
	m4double_t dNumber   = atof((char*)yytext); 
	m4double_t dNumberAbs= fabs(dNumber);

	if (dNumberAbs > 1e244  ){//el -11: el 'punto', los 8 decimales, el signo, el /0 y uno que se come el truncado del logaritmo
		
		m_poTrad->GetpParser()->yyerror("number to long",M4_CH_VM_COMP_NUMBER_TOO_BIG);
		return EOF2;//->Forzamos la salida del parser
	
	}
			
	
	sprintf((char*)&yylval,"%.8lf", dNumber );
	return NUMERO;		    
	}

{hexanumber}        {
					int iTemp;
					
					sscanf((char*)yytext,"%x",&iTemp);
					sprintf((char*)&yylval,"%.8lf",(float)iTemp);
		    return NUMERO;
					}

{fecha}             {
					char *pszTemp;
					char *pszTemp2;
					
					pszTemp=yytext+1;
					while (isspace(*pszTemp)) pszTemp++;
					
					pszTemp2=pszTemp+strlen(pszTemp)-2;
					while (isspace(*pszTemp2)) pszTemp2--;

					strncpy(yylval.str,(char*)pszTemp,(char*)pszTemp2-(char*)pszTemp+1);
                    yylval.str [ (char*)pszTemp2-(char*)pszTemp+1 ]=0;
					yylval.str[MAX_TOKEN-1]='\0';
		    return FECHA;
		    } 


{literal}           {
					//Reservo espacio dinamico: no compruebo memoria
					//que libero en TRAD.CPP - ClTrad::StoreString(...)
					//yylval.pstr = new char[yyleng-1];
					//strncpy(yylval.pstr,(char*)yytext+1,yyleng-2);
					//yylval.pstr[yyleng-2]='\0';
					//return LITERAL;

					yylval.hString = 0;
					int Aux_yyleng=0, Aux_SizeBuffer=M4CL_VM_LEX_LITERAL_RESIZE;
					char cTemp=yyinput();	//First char
					char *Aux_pstr = new char[Aux_SizeBuffer+1];	//Buffer temporal redimensionable
					while (1) {
						if (cTemp=='"') {
							Aux_pstr[Aux_yyleng]='\0';
							break;	//Fin de cadena
						}
						else if (cTemp=='\r') {
							if ( (cTemp=yyinput())=='\n' ) {
								m_poTrad->GetpParser()->yyerror("string without close",M4_CH_VM_COMP_STRING_NOT_CLOSED);
								if (Aux_pstr) delete []Aux_pstr;
								return EOF2;	//Fin de linea sin comillas => forzamos error en el parser
							}else {
								//Falso fin de linea => recuperarnos
								//Redimensionamos buffer, si es necesario
								if (Aux_yyleng >= Aux_SizeBuffer) {
									Aux_SizeBuffer += M4CL_VM_LEX_LITERAL_RESIZE;
									char *pAuxstr = new char[Aux_SizeBuffer+1];
									memcpy(pAuxstr, Aux_pstr, Aux_SizeBuffer-M4CL_VM_LEX_LITERAL_RESIZE);
									if (Aux_pstr) delete [] Aux_pstr;
									Aux_pstr = pAuxstr;
								}
								Aux_pstr[Aux_yyleng++] = '\r';
							}
						}
						else if (cTemp==EOF) {
							if (Aux_pstr) delete [] Aux_pstr;	//Liberamos memoria
							break;	//Fin de parser sin comillas => Parser genera error
						}
						//Redimensionamos buffer, si es necesario
						if (Aux_yyleng >= Aux_SizeBuffer) {
							Aux_SizeBuffer += M4CL_VM_LEX_LITERAL_RESIZE;
							char *pAuxstr = new char[Aux_SizeBuffer+1];
							memcpy(pAuxstr, Aux_pstr, Aux_SizeBuffer-M4CL_VM_LEX_LITERAL_RESIZE);
							if (Aux_pstr) delete [] Aux_pstr;
							Aux_pstr = pAuxstr;
						}
						Aux_pstr[Aux_yyleng++] = cTemp;
						cTemp=yyinput();	//Next char
					}

					if (cTemp=='"') {
						m_oDynamicPool->AddString(Aux_pstr, yylval.hString) ;	//DeleteString en "StoreString()" (parser.cpp)
						delete [] Aux_pstr;
						return LITERAL;
					}
		    }

{nl}                {
					iNumLine++;
		    return NL;
		    }
"/*"                            {
					int iTemp;
					int ret=0;

					loop:
						iTemp = yyinput();

					loop2:
						// nos comemos el comentario, hasta encontrar un caracter especial
						// BUG 0078309: Hay que ignorar los \r
						while (iTemp!='*' && iTemp!='\n' && iTemp!=EOF)
							iTemp = yyinput();

						// segun el caracter
						switch (iTemp) {
						case EOF:
							return NL;

						case '\n':
							iNumLine++;
							ret=1;
							break;

						case '*':
							// leemos 1 mas
							iTemp=yyinput();
							if (iTemp=='/') {
								goto loopExit;
							} else {
								// backtracking
								//unput(iTemp);
								goto loop2;
							}
							break;

						//default:
							// nos comemos el caracter
						}
						goto loop;

					loopExit:
					if (ret) return NL;
			
			/*
					// Solucion antigua
					int iTemp;
					int ret;
					ret=0;
				loop: iTemp=yyinput();
					
						
					while (iTemp!='*' && iTemp!='\r' && iTemp!=EOF) 
						iTemp=yyinput();
					if (iTemp==EOF)
					   return NL;
					//while (yyinput()!='*') ;
					if (iTemp=='\r' && (iTemp=yyinput())=='\n') {
						iNumLine++;
						ret=1;
						goto loop;
					}

					switch (yyinput()){

						case '/': break;
						case '*': unput ('*');
							
						default: goto loop;
					}
			
					if (ret) return NL;
			*/

			}
\x00                {
		    return EOF2;
		    }
<<EOF>>             {
		    return EOF2;
		    }

{espacio}                       {/*los comemos*/
					}
"'"					{
					/* 
						"'"[^\r\n]*{nl} {
						Comentarios "'" de 1 linea: 'comentario
						iNumLine++;
						return NL;
					*/
					int iTemp=yyinput();	//Next char
					
					loop_3:
					// BUG 0078309: Hay que ignorar los \r
					while (iTemp!='\n' && iTemp!=EOF) 
						iTemp=yyinput();
					
					if (iTemp==EOF)
					   return NL;
					
					if (iTemp=='\n') {
						iNumLine++;
						return NL;
					}
					goto loop_3;	//seguimos comiendo chars
					}

"//"				{
					/*
						"//"[^\r\n]*{nl}        {
						Comentarios "//" de 1 linea: //comentario
						iNumLine++;
						return NL;
					*/
					int iTemp=yyinput();	//Next char
					
					loop_4:
					// BUG 0078309: Hay que ignorar los \r
					while (iTemp!='\n' && iTemp!=EOF) 
						iTemp=yyinput();
					
					if (iTemp==EOF)
					   return NL;
					
					if (iTemp=='\n') {
						iNumLine++;
						return NL;
					}
					goto loop_4;	//seguimos comiendo chars
					}

[^ \r\n\t{operador}]      return ((unsigned char)*yytext);
					


