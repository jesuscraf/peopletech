// parser.y

/* 
*************************************************************************************************
*										DECLARATION SECTION										*
************************************************************************************************* 
*/


%{


// Donde se define la clase SentencePArser a la que pertenecerá yyparse
#include "sentenceparser.hpp"

%}


%union { 
		char pcValue[80];
		ColumnInfo * poColumn;
		TableInfo * poTable;
		ItemInfo * poItem;
		TextInfo * poText;
		ElementInfo * poElement;
		SelectInfo * poSelect;
		FunctionInfo * poFunction;
		JoinInfo * poJoin;
		}


//---------------->Token 'error en lexer'

%token LEXER_ERROR

//---------------->Tokens basicos

%token LITERAL_INTEGER LITERAL_FLOAT LITERAL_STR LITERAL_DATE LITERAL_TIME_STAMP IDENTIFIER

//---------------->Keywords

%token SELECT FROM GROUP_BY WHERE UNION UNION_ALL INTERSECT IN_KW ALL 
%token DISTINCT EXISTS HAVING ORDER_BY ASC DESC

//---------------->Operadores

%token OPE_OR OPE_AND
%token OPE_NOT OPE_EQU OPE_NOT_EQU OPE_GT OPE_LT OPE_GE OPE_LE OPE_LIKE OPE_NOT_LIKE 
%token OPE_BETWEEN OPE_NOT_BETWEEN OPE_IS_NULL OPE_IS_NOT_NULL
%token OPE_ADD OPE_SUBS OPE_CONCAT_STR 		
%token OPE_MULT OPE_DIV 
%token OPE_MINUS_SIGN OPE_PLUS_SIGN

//---------------->Operadores especiales

%token OPEN_PAREN CLOSE_PAREN OPE_OUTER_JOIN OPE_COMMA OPE_POINT OPE_AT OPE_AMP OPE_ALM OPE_DOL

//=====================================================> Operadores, tratan asociatividad y prec.
/*
NOTAS: 
	-epxr IS NULL y expr IS NOT NULL no admiten expresiones logicas como operandos.
	-NOT no es asociativo (esto es curioso, en casi todos los lenguajes se admiten nots seguidos).
*/

    
// FROM lowest precedence ....                                        

%left		OPE_OR
%left		OPE_AND
%nonassoc	OPE_NOT OPE_EQU OPE_NOT_EQU OPE_GT OPE_LT OPE_GE OPE_LE OPE_LIKE OPE_NOT_LIKE 
%nonassoc	OPE_BETWEEN OPE_NOT_BETWEEN OPE_IS_NULL OPE_IS_NOT_NULL IN_KW
%left 		OPE_ADD OPE_SUBS OPE_CONCAT_STR 		
%left 		OPE_MULT OPE_DIV 
%right 		OPE_MINUS_SIGN OPE_PLUS_SIGN //El lexer devuelve OPE_ADD OPE_SUBS en lugar de estos

// .... TO highest precedence


// Regla de inicio de la gramátca

%start command

%%

/* 
*************************************************************************************************
*										RULES SECTION											*
************************************************************************************************* 
NOTA: en mayusculas estan los simbolos terminales y los no términales
que sólo se utilizan para agrupar terminales
*/

//=====================================================> Tokens y pseudo terminales

CONSTANT : LITERAL_INTEGER  { 
								$$ = $1;
							}
		 | LITERAL_FLOAT    { 
								$$ = $1;
							}
		 | LITERAL_STR      {  
								$$ = $1;
							}                    
		 | LITERAL_DATE     { 
								$$ = $1;
							}                    
		 ;
//=====================================================> NT basicos de la gramatica

column_name	: IDENTIFIER
				{
					$$.poColumn = m_oFactory.CreateColumnInfo ("", $1.pcValue);
				}
			| IDENTIFIER OPE_POINT IDENTIFIER
				{
					$$.poColumn = m_oFactory.CreateColumnInfo ($1.pcValue, $3.pcValue);
				}
			;


table_name 	: OPE_AMP IDENTIFIER IDENTIFIER
				{
					$$.poTable = m_oFactory.CreateTableInfo ( $2.pcValue, $3.pcValue);
					$$.poTable->SetIsBase (true);
				}
			| IDENTIFIER IDENTIFIER
				{
					$$.poTable = m_oFactory.CreateTableInfo ( $1.pcValue, $2.pcValue);
				}
			| OPE_AMP IDENTIFIER
				{
					$$.poTable = m_oFactory.CreateTableInfo ( $2.pcValue, "");
					$$.poTable->SetIsBase (true);
				}
			| IDENTIFIER
				{
					$$.poTable = m_oFactory.CreateTableInfo ( $1.pcValue, "");
				}
			;


item_name:	OPE_AT IDENTIFIER
				{
					$$.poItem = m_oFactory.CreateItemInfo ($2.pcValue);
				}
		 |	OPE_DOL IDENTIFIER
				{
					$$.poItem = m_oFactory.CreateItemInfo ($2.pcValue);
					$$.poItem->SetSessionChannel (M4_TRUE);
				}

			;


//=====================================================> Funciones


// NO se admiten listas vacias. 
expr_nelist	: expr 							
				{ 
					$$ = $1;
				} 
			| expr_nelist OPE_COMMA expr    
				{   
					$$.poText = m_oFactory.CreateTextInfo (",");
					$$.poText->AddLeft ($1.poElement);
					$$.poText->AddRight ($3.poElement);
				}
			;


// SI admite listas vacias.
expr_list	: /*empty*/ 		{
									$$.poElement = 0; 
								}
			| expr_nelist		{ 
									$$ = $1;
								}
			;


function 	: OPE_ALM IDENTIFIER OPEN_PAREN expr_list CLOSE_PAREN 
				{
					$$.poFunction = m_oFactory.CreateFunctionInfo ( $2.pcValue );
					$$.poFunction->SetArgument ($4.poElement);
				}
			; 
			
//=====================================================> Expresiones generales


//Expresion NO contiene ni permite operadores logicos. 
//Las expresiones logicas son un superconjunto de estas expresiones, y la distincion es intencionada,
//para distinguir entre ambas, ya que hay situaciones en las que una expresion normal es valida,
//pero una logica no lo es.

//Con las expresiones se consturye un arbol, el resdultado de esta regla es el símbolo raiz del árbol de expresión.

expr	: expr OPE_ADD expr				
				{
					$$.poElement = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
		| expr OPE_SUBS  expr				
				{
					$$.poElement = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
		| expr OPE_MULT expr				
				{
					$$.poElement = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
		| expr OPE_DIV expr			
				{
					$$.poElement = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
		| OPE_SUBS expr %prec OPE_MINUS_SIGN
				{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo ($1.pcValue);
					$2.poElement->AddLeft (poTextInfoLeft);
					$$ = $2;
				}
		| OPE_ADD expr %prec OPE_PLUS_SIGN
				{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo ($1.pcValue);
					$2.poElement->AddLeft (poTextInfoLeft);
					$$ = $2;
				}
		| expr OPE_CONCAT_STR expr
				{
					$$.poElement = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
		| OPEN_PAREN expr CLOSE_PAREN				
				{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo ($1.pcValue);
					TextInfo * poTextInfoRight = m_oFactory.CreateTextInfo ($3.pcValue);
					$2.poElement->PushLeft (poTextInfoLeft);
					$2.poElement->PushRight (poTextInfoRight);
					$$ = $2;
				}
		| item_name
				{
					$$ = $1;
				}
		| column_name
				{
					$$ = $1;
				}
		| column_name OPE_OUTER_JOIN
				{
					$$.poColumn->SetHasOuterJoin (M4_TRUE);
					$$ = $1;
				}
		| function
				{
					$$ = $1;
				}
		| CONSTANT
				{
					$$.poText = m_oFactory.CreateTextInfo ($1.pcValue)
				}
		;


//Ahora definimos las expresiones logicas.

COMPARISON_OPE	: OPE_EQU     	{  
									$$ = $1;
								}
				| OPE_NOT_EQU	{   
									$$ = $1;
								}                
				| OPE_GT		{   
									$$ = $1;
								} 
				| OPE_LT		{   
									$$ = $1;
								}
				| OPE_GE		{   
									$$ = $1;
								} 
				| OPE_LE		{   
									$$ = $1;
								} 
				;


quantified_expr : expr COMPARISON_OPE command
					{
						$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);
						$$.poText->AddLeft ($1.poElement);
						$$.poText->AddRight ($3.poElement);
					}
				| EXISTS command
					{
						$$.poText = m_oFactory.CreateTextInfo ($1.pcValue);
						$$.poText->AddRight ($2.poElement);
					}
				; 


in_expr : expr IN_KW command  //Subquery ya lleva ()
			{
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poText->AddLeft ($1.poElement);
					$$.poText->AddRight ($3.poElement);
			}
     	| expr IN_KW OPEN_PAREN expr_nelist CLOSE_PAREN %prec IN_KW
			{
					TextInfo * poTextLeft = m_oFactory.CreateTextInfo ($3.pcValue);
					TextInfo * poTextRight = m_oFactory.CreateTextInfo ($5.pcValue);
					$4.poElement->PushLeft (poTextLeft);
					$4.poElement->PushRight (poTextRight);
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poText->AddLeft ($1.poElement);
					$$.poText->AddRight ($4.poElement);
			}
		;


//El resultado de esta regla es el símbolo raiz del árbol de expresión.

condition 	: expr COMPARISON_OPE expr %prec OPE_EQU  
				{
					// Comprobamos si es un join 
					if ( ($1.poElement->GetType() == COLUMNINFO) && ($3.poElement->GetType() == COLUMNINFO) && (!strcmp ($2.pcValue, "=")) )
					{
						$$.poJoin = m_oFactory.CreateJoinInfo (viTreeExpresionLevel.front());
						vpoSelect.front()->AddJoin ($$.poJoin);
					}
					else
						$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);  

					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
			| in_expr        
				{
					$$ = $1;
				}
			| expr OPE_IS_NULL
				{
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);  
					$$.poElement->AddLeft ($1.poElement);
				}
			| expr OPE_IS_NOT_NULL
				{
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);  
					$$.poElement->AddLeft ($1.poElement);
				}
			| expr OPE_LIKE expr
				{
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);  
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
			| expr OPE_NOT_LIKE expr
				{
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);  
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
				}
			| expr OPE_BETWEEN expr OPE_AND expr %prec OPE_BETWEEN	//Between: operador TERNARIO, ojo con el AND
				{
					TextInfo * poTextInfo2 = m_oFactory.CreateTextInfo ($4.pcValue);
					poTextInfo2->AddLeft ($3.poElement);
					poTextInfo2->AddRight ($5.poElement);
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poText->AddLeft ($1.poElement);
					$$.poText->AddRight (poTextInfo2);
				}
			| expr OPE_NOT_BETWEEN expr OPE_AND  expr  %prec OPE_NOT_BETWEEN   //Between: operador TERNARIO
				{
					TextInfo * poTextInfo2 = m_oFactory.CreateTextInfo ($4.pcValue);
					poTextInfo2->AddLeft ($3.poElement);
					poTextInfo2->AddRight ($5.poElement);
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);
					$$.poText->AddLeft ($1.poElement);
					$$.poText->AddRight (poTextInfo2);
				}
			| quantified_expr   
				{ 
					$$ = $1;
				} 
			| condition OPE_AND condition
				{
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);  
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
					// Si cambiamos de operador aumentamos un nivel lógico en la expresión
					if (!vbLastIsAnd.front())
					{
						viTreeExpresionLevel.front() ++;
						vbLastIsAnd.front() = M4_TRUE;

						// Si ha cambiado comprobamos si alguna de las condiciones
						// es unjoin y si le asignamos este nivel en el árbol a postriori
						if ($1.poElement->GetType() == JOININFO)
							$1.poJoin->SetTreeLevel (viTreeExpresionLevel.front());
						if ($3.poElement->GetType() == JOININFO)
							$3.poJoin->SetTreeLevel (viTreeExpresionLevel.front());
					}
				}
			| condition OPE_OR condition
				{
					$$.poText = m_oFactory.CreateTextInfo ($2.pcValue);  
					$$.poElement->AddLeft ($1.poElement);
					$$.poElement->AddRight ($3.poElement);
					// Si cambiamos de operador aumentamos un nivel lógico en la expresión
					if (vbLastIsAnd.front())
					{
						viTreeExpresionLevel.front() ++;
						vbLastIsAnd.front() = M4_FALSE;
						
						// Si ha cambiado comprobamos si alguna de las condiciones
						// es unjoin y si le asignamos este nivel en el árbol a postriori
						if ($1.poElement->GetType() == JOININFO)
							$1.poJoin->SetTreeLevel (viTreeExpresionLevel.front());
						if ($3.poElement->GetType() == JOININFO)
							$3.poJoin->SetTreeLevel (viTreeExpresionLevel.front());
					}
				}
			| OPEN_PAREN condition CLOSE_PAREN 
				{
					TextInfo * poTextInfoLeft = m_oFactory.CreateTextInfo($1.pcValue);
					TextInfo * poTextInfoRight = m_oFactory.CreateTextInfo($3.pcValue);
					$2.poElement->PushLeft (poTextInfoLeft);
					$2.poElement->PushRight (poTextInfoRight);
					$$ = $2;
				}
			| OPE_NOT condition
				{
					$$.poText = m_oFactory.CreateTextInfo($1.pcValue);
					$$.poText->AddRight ($2.poElement);
				}
			;

//=====================================================> Comando SELECT


//--------->select_modif_row option

select_modif_row 	: /*empty*/ {  
									vpoSelect.front()->SetHasDistinct (false);
								}
					| DISTINCT	{ 
									vpoSelect.front()->SetHasDistinct (true);
								}
					; 


//--------->displayed_clause

displayed_column : item_name OPE_EQU expr                            
				 	{ 
						TextInfo * poTextInfo = m_oFactory.CreateTextInfo ($2.pcValue);
						poTextInfo->AddLeft ($1.poItem);
						poTextInfo->AddRight ($3.poElement);
						$$.poText = poTextInfo;
					}
				 | column_name
					{
						$$ = $1;
					}
				 | function
					{
						$$ = $1;
					}
				 | CONSTANT
					{
						$$.poText = m_oFactory.CreateTextInfo ($1.pcValue);
					}
				 ;


displayed_list 	: displayed_column
					{ 
						vpoSelect.front()->AddColumn ($1.poElement);
					}
				| displayed_list OPE_COMMA displayed_column  				 
					{   
						vpoSelect.front()->AddColumn ($3.poElement);
					}
				;


//--------->selected_table_list

table_list	: table_name
				{
					vpoSelect.front()->AddTable ($1.poTable);
				}
			| table_list OPE_COMMA table_name
				{   
					vpoSelect.front()->AddTable ($3.poTable);
				}
			;


//--------->where_clause

where_clause 	: /*empty*/ 		{ 
									}
				| WHERE condition   {
										vpoSelect.front()->SetWhere ($2.poElement);
									}
				;


//--------->group_clause (exlusion group_clause-connect_clause, gestionada en run-time)

group_clause 	: /*empty*/				
					{ 
					}
				| GROUP_BY expr_nelist
					{
						vpoSelect.front()->SetGroupBy ($2.poElement);						
					}
				| GROUP_BY expr_nelist HAVING condition
					{
						vpoSelect.front()->SetGroupBy ($2.poElement);
						vpoSelect.front()->SetHaving ($4.poElement);
					}
				;                 
				                     

//--------->order_clause  (exclusion order_clause-update_clause, gestionada en run-time)

sorted_def  : expr 
				{
					$$ = $1;
				}
			| expr ASC
				{
					$$.poText = m_oFactory.CreateTextInfo ("ASC");
					$$.poText->AddLeft ($1.poElement);
				}
			| expr DESC
				{
					$$.poText = m_oFactory.CreateTextInfo ("DESC");
					$$.poText->AddLeft ($1.poElement);
				}
			;                     

			
sorted_list : sorted_def 
				{
					vpoSelect.front()->AddOrderBy ($1.poElement);
				}
			| sorted_list OPE_COMMA sorted_def
				{   
					vpoSelect.front()->AddOrderBy ($3.poElement);
				}
			; 

        
order_clause 	: /*empty*/ 			{ 
										}
				| ORDER_BY sorted_list  { 
										}
				;


//--------->select_command

// Admitimos sentencias sin parte SELECT

select_command : SELECT { 
							// Creamos el objeto select y lo metemos en la pila
							SelectInfo * poSelectInfo = m_oFactory.CreateSelectInfo ();
							vpoSelect.push_front (poSelectInfo);
							// Inicializamos variables temporales para la select					
							vbLastIsAnd.push_front(M4_TRUE);
							viTreeExpresionLevel.push_front(0);
						} 
						select_modif_row displayed_list 
						FROM table_list 
						where_clause
						group_clause order_clause
						{
							// Ya hemos procesado la select
							// Sacamos el objeto select de la pila
							$$.poSelect = vpoSelect.front();
							vpoSelect.pop_front();
							vbLastIsAnd.pop_front();
							viTreeExpresionLevel.pop_front();									
						}
				
				|		{ 
							// Creamos el objeto select y lo metemos en la pila
							SelectInfo * poSelectInfo = m_oFactory.CreateSelectInfo ();
							vpoSelect.push_front (poSelectInfo);
							// Inicializamos variables temporales para la select					
							vbLastIsAnd.push_front(M4_TRUE);
							viTreeExpresionLevel.push_front(0);
						}
						FROM table_list 
						where_clause
						group_clause order_clause
						{
							// Ya hemos procesado la select
							// Sacamos el objet select de la pila
							$$.poSelect = vpoSelect.front();
							vpoSelect.pop_front();									
							vbLastIsAnd.pop_front();
							viTreeExpresionLevel.pop_front();									
						}


//=====================================================> INPUT de la gramatica

command : 
		OPEN_PAREN select_command CLOSE_PAREN 
				{
					$2.poSelect->SetIsSubselect (true);
					$$ = $2;
				}
		| select_command 
				{ 
					poFinalSelect = $1.poSelect;
					$$ = $1;					
				}
		| select_command UNION command 
				{
					$1.poSelect->AddUnion ($3.poSelect);
					poFinalSelect = $1.poSelect;
					$$ = $1;					
				}
		| select_command INTERSECT command
				{
					$1.poSelect->AddIntersect ($3.poSelect);
					poFinalSelect = $1.poSelect;
					$$ = $1;					
				}
		| LEXER_ERROR
				{ 
					yyerror ( NULL );
				}
		;

%%

/* 
*************************************************************************************************
*										PROGRAM SECTION											*
************************************************************************************************* 
*/




