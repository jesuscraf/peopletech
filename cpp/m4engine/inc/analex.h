//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             analex.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:13/02/98
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//* Modifications:	Manuel Lazcano (27/05/98) - Class ClTkSysInfo
// 												Class ClTkCTTot
//												Class ClTkThreadInfo
//
// Definition:
//
//    Analizador de expresiones.
//
//==============================================================================
#ifndef __ANALEX__H__
#define __ANALEX__H__

#include "m4stl.hpp"
#include "m4clflag.h"
#include "xapichn.h"
#include "totalize.h"
#include "clstr.h"
#include "reportenv.h"

class ClENG_Tot ;
class ClENG_KeyTot ;
class ClENG_Report ;
class ClMIT_Args ;
class ClENG_Section ;
class ClENG_FieldExternProp ;
class ClENG_StorageDep ;
class ClENG_StringExtraction ;
class ClENG_ReportRequest ;
class ClENG_FieldsManager ;

/*
	Este modulo se encarga de gestionar las expresiones, y se compone de un pequeño
anilzador lexico y sintactico.
	Los pasos que sigue una cadena hasta su evaluacion son:

	-Analizador lexico.
		-Entrada:Una cadena, que es la expresion a evaluar.
		-Salida:Lista secuencial de ClENG_Token.
	-Analizador sintactico.
		-Entrada:La salida del analizador lexico (lista de tokens).
		-Salida:Pila pre-analizada de ClENG_Token, lista para evaluacion. Esta lista 
			puede evaluarse tantas veces como se quiera. El analizador sintactico ya 
			ha dejado todo preparado para que la ejecucion no tenga que evaluar nada
			de sintaxis, sino simplemente calcular.

	El diseño de las clase ClENG_Token se orienta a los datos necesarios para una rapida 
evaluacion.
*/


//==============================================================================ClENG_Token

class ClENG_Token {

MIT_PROTECTED:
	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:

	//-------------------------Miembros-------------------------------------------------

	/*
	Esta clase es de uso particular de ClENG_Analex y ClENG_Expression.
	Esta clase sirve tanto para los tokens del analizador lexico, como del sintactico. 
	El analizador sintactico se encarga de formar el arbol de tokens, cuyos nodos pueden tener
		un número de hijos variable. Algunos tokens forman parte del arbol (y serán 
		eliminados por esta clase), y otros son consumidos por el analizador sintactico, en
		cuyo caso los elimina este.
	*/

	enum TYPE { 
		ATOMIC_VALUE,	//El token es atomico: obtiene un valor por si solo.
		OPERATOR,		//El token es un operador, dice que hacer con los siguientes tokens.
		PRECEDENCE,		//El token es un operador especial para alterar precedencias, ()
		ARG_SEPARATOR	//Item separador de argumentos de funciones
	} ;

	enum TOKEN_FLAGS {

		//Lo normal es que este desactivado, para que ClENG_Token::Evaluate()  evalue los
		//	hijos automaticamente. No se hace esto si esta activado este flag.
		//Si esta a true, implica DEAL_WITH_NULLS (se activa incondicionalmente).

		NOT_EVALUATE_SON=ClMIT_Flag::BIT0, 

		//Lo normal es que este desactivado, para que ClENG_Token::Evaluate()  testee que
		//	ningun hijo sea NULL. Si lo es, se retorna NULL y nisiquiera se evalua el token.
		//Si esta activado, no se hace nada al respecto:es responsabilidad del metodo 
		//	derivado tratar con NULLs.

		DEAL_WITH_NULLS=ClMIT_Flag::BIT1
	} ;
	
MIT_PRIVATE:

	TYPE  m_Type ;

	ClMIT_ChnValue::TYPE m_NULLReturnType ;	//Tipo de NULL para tokens !DEAL_WITH_NULLS

	struct StSon {
		ClENG_Token *m_poTk ;
		ClMIT_ChnValue m_oValue ;	//Un valor por hijo 
	} *m_poSons ;

	m4int16_t m_iMaxSons, m_iCountSons ;

	ClMIT_Flag m_oFlags ;


MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_Token() ;
	virtual ~ClENG_Token() { ClENG_Token::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Token) ; }

	// Se carga toda la rama de hijos.
	virtual void End() ;

	void Init( TYPE ai_Type, m4int16_t ai_iMaxSons, 
		ClMIT_Flag::Value_t ai_Flags=ClMIT_Flag::NONE_BITS, 
		ClMIT_ChnValue::TYPE ai_NULLReturnType=ClMIT_ChnValue::NO_INIT ) ;

	// Si se sobrepasa, error inmediato.

	void AddSon( ClENG_Token * ai_poSon ) ;

	//-------------------------Gets&Sets------------------------------------------------

	TYPE GetType() { return m_Type ; }
	m4int16_t GetMaxSons() { return m_iMaxSons; } 
	m4int16_t GetCountSons() { return m_iCountSons ; }

	//-------------------------Evaluacion------------------------------------------------

	// Si NOT_EVALUATE_SON esta a FALSE (comportamiento normal), lo que hace es evaluar 
	//	PRIMERO todos los hijos, dejando el valor en el value correspondiente al 
	//	hijo, y LUEGO llamar a EvaluateThis, pasandole ao_poValue para que almacene el valor.
	// Si NOT_EVALUATE_SON esta a TRUE, se llama a EvaluateThis directamente, siendo 
	//	responsabilidad del metodo en cuestion evaluar o no hijos. Util para IIF y NVL.
	//	Implica  DEAL_WITH_NULLS.
	// Si NOT DEAL_WITH_NULLS, y algun argumento es NULL, el resultado tambien lo sera,
	//	no evaluandose el token actual. El tipo de NULL es el indicado por m_NULLReturnType.

	void Evaluate(ClMIT_ChnValue *ao_poValue) ;

MIT_PROTECTED:

	//Usado por Evaluate, simplemente evalua el hijo poniendo trazas y las variables de 
	//	almacenamiento se ponen a NO_INIT y !NULL, antes de evaluar el hijo.
	//Da true o false, segun el valor de vuelta sea o no NULL.

	m4bool_t EvaluateSon(m4int16_t B) ;

	//VIRTUAL SUSTITUIBLE. 
	//Debe inicializar ao_Value con un valor. El metodo por defecto pone un tipo NO_INIT.
	//Cuando se llama a este metodo, se garantiza que todos los posibles hijos han sido 
	//	llamados, y que el resultado de la evaluación está y a almacenado. Para acceder a
	//	este valor, las derivadas tienen el metodo GetPointerToSonValue().
	//Generara errores locales si hay problemas.

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 

	ClMIT_ChnValue * GetPointerToSonValue(m4int16_t ai_Son) 
		{return & m_poSons[ai_Son].m_oValue ; }

MIT_PUBLIC:

	//Resetea toda la rama.

	virtual void ResetExternProp( ClMIT_ItemExternProp * ai_poExternProp ) ;
	
MIT_PROTECTED:

	//VIRTUAL SUSTITUIBLE. 
	//Sobrecargar donde interese. Lo usa Expression
	virtual void ResetExternPropThis( ClMIT_ItemExternProp * ai_poExternProp ) ;

	m4bool_t IsEvaluateSons() { return m_oFlags.False(NOT_EVALUATE_SON); }
	m4bool_t IsDealWithNULLs() { return m_oFlags.True(DEAL_WITH_NULLS); }

	//-------------------------Otros-----------------------------------------------------

MIT_PUBLIC:
	// Solo usadas en trace. 

	void Trace() ;

	//VIRTUAL SUSTITUIBLE
	//Sobrecargar GetTraceString(), que debe meter en una cadena una linea con 
	//	informacion del token en cuestion.
	// No es pura porque hay tokens que no estan en ejecucion y no necesitan sobrecargarse. 
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

	//-----------Sobrecargas para usar el objeto con STL
	m4bool_t operator == (const ClENG_Token & ai_oOpe) const
		{ return M4_BOOL(m_Type==ai_oOpe.m_Type); }
	m4bool_t operator < (const ClENG_Token & ai_oOpe) const
		{ return M4_BOOL(m_Type<ai_oOpe.m_Type); }
} ;

//==============================================================================ClENG_TkLiteral

class ClENG_TkLiteral : public ClENG_Token {

	//ClENG_Token flags:NINGUNO
	//NULLReturnType=NO_INIT (defecto, nunca es NULL ni tiene parametros)

MIT_PRIVATE:
	
	ClMIT_ChnValue m_oValue ;

MIT_PUBLIC:

	ClENG_TkLiteral() ;
	virtual ~ClENG_TkLiteral() { ClENG_TkLiteral::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkLiteral) ; }
	virtual void End() ;

	//Se le pasa como ultimo parametro un flag que le indica si lo que queremos inicializar es un valor NULL
	// Por defecto esta a FALSE
	void Init( ClMIT_ChnValue::TYPE ai_Type, const m4char_t * ai_pcAnonymousStr, m4bool_t ai_bNullValue=M4_FALSE ) ;

	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 
} ;

//==========================================================================ClENG_TkItem

class ClENG_TkItem : public ClENG_Token {

	//ClENG_Token flags:NINGUNO
	//NULLReturnType=NO_INIT (defecto, nunca es NULL ni tiene parametros)

MIT_PUBLIC:
	enum LOAD_MODE { ITEM_MODE, TOTALIZE_COUNT, TOTALIZE_SLICE_COUNT } ;
MIT_PRIVATE:

	LOAD_MODE m_LoadMode ;
	ClMIT_ChnItem m_oItem ;

MIT_PUBLIC:

	ClENG_TkItem() ;
	virtual ~ClENG_TkItem() { ClENG_TkItem::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkItem) ; }
	virtual void End() ;

	//Da true si todo va bien, false si no (se produce error local).
	//Si ai_poChn es NULL, error local.
	//Si NodeName es NULL, se usa el NodeDefault. Si este es NULL, error local.
	//Si el LoadMode es GUIDE_LOAD, NodeName debe ser NULL.
	
	m4bool_t Init( 
		ClMIT_Chn *ai_poChn, 
		ClMIT_ChnNode * ai_poNodeDefault, 
		const m4char_t *ai_pcNodeName, const m4char_t *ai_pcItemName, 
		LOAD_MODE ai_TkLoadMode, ClMIT_ChnItem::LOAD_MODE ai_LoadMode,
		ClENG_StorageDep *ai_poDep,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	) ;

	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	virtual void ResetExternPropThis( ClMIT_ItemExternProp * ai_poExternProp ) ;

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 

} ;

//==========================================================================ClENG_TkPrecedence

class ClENG_TkPrecedence : public ClENG_Token {

	//ClENG_Token flags:NINGUNO
	//NULLReturnType=NO_INIT (defecto, nunca es NULL ni tiene parametros)

MIT_PRIVATE:

	m4bool_t m_bSwOpen ;	//True, parentesis apertura, false, cierre.

MIT_PUBLIC:

	ClENG_TkPrecedence() ;
	virtual ~ClENG_TkPrecedence() { ClENG_TkPrecedence::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkPrecedence) ; }
	virtual void End() ;

	void Init( m4bool_t ai_bSwOpen ) ;

	m4bool_t IsOpen() { return m_bSwOpen ; } 
	
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;
} ;

//==========================================================================ClENG_TkExpSep

class ClENG_TkExpSep : public ClENG_Token {

	//ClENG_Token flags:NINGUNO
	//NULLReturnType=NO_INIT (defecto, nunca es NULL ni tiene parametros)

MIT_PUBLIC:

	ClENG_TkExpSep() ;
	virtual ~ClENG_TkExpSep() { ClENG_TkExpSep::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkExpSep) ; }
	virtual void End() ;

	void Init() ;

	virtual void GetTraceString(ClMIT_Str *ao_Str) ;
} ;

//==========================================================================ClENG_TkOperator

class ClENG_TkOperator : public ClENG_Token {

	//ClENG_Token flags     :NINGUNO automatico, se pasan
	//ClENG_TkOperator flags:TO_RIGHT incondicional si IS_FUNCTION
	//NULLReturnType= el que se le pase

MIT_PUBLIC:

	enum GROUP_LEVEL { 
		FUNCTION=0,
		ARITHMETICAL=100,
		LOGICAL=200,
		STRING=300,
		ABSORPTION=400
	} ;

	enum TOKEN_OPERATOR_FLAGS { 
		//True operator tipo funcion, false normal.

		IS_FUNCTION=ClMIT_Flag::BIT0, 

		//Util para operadores unarios, asociativos dcha-izda. Si es funcion, implica este
		//	flag:se activa incondicionalmente

		TO_RIGHT=ClMIT_Flag::BIT1	
	} ;

MIT_PRIVATE:

	//Nivel de precedencia: 0, se ejecuta el primero de todos. Es el
	//	resultado de la suma de la precedencia de grupo, y de la de operador.
	//Para la misma precedencia, se opera de izquierda a derecha

	m4int16_t  m_iOpLevel;
	
	//Numero de argumentos (u operandos) del operador. Al operador se le llama con 
	//	m_iArgs valores, que son resultados de operandos ya analizados.
	//El número de argumentos es fijo por operador.

	m4int16_t m_iArgs ;	

	//La diferencia de un operador 'normal' a uno función solo consiste en la forma en que 
	//	el analizador sintactico obtiene los parámetros:
	//	-Un operador normal. Solo se soportan unarios o binarios. 
	//		-Unarios asociativos a derecha. Una vez localizado el operador, se busca el 
	//			operando a la derecha de este.
	//		-Unarios asociativos a izquierda. Una vez localizado el operador, se busca el 
	//			operando a la izquierda de este.
	//		-Binarios. Se buscan los operandos a la izquierda y derecha del operador.
	//	-Un operador función. Tras el token del operador función debe haber:
	//		-Un PRECEDENCE de apertura, como comienzo de lectura de argumentos, y otro de
	//			cierre, al mismo nivel.
	//		-Los argumentos estan separados, dentro de este margen, por tokens SEPARATOR, 
	//			al mismo nivel que los PRECEDENCE.
	//		-Siempre deben pasarse todos los parametros que requiera la función (incluido
	//			0 parámetros).
	
	ClMIT_Flag m_oFlags ;

MIT_PUBLIC:

	ClENG_TkOperator() ;
	virtual ~ClENG_TkOperator() { ClENG_TkOperator::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkOperator) ; }
	virtual void End() ;

	//Si ai_bIsFunction, ai_bToRight se pone a TRUE incondicionalmente.

	void Init( GROUP_LEVEL ai_Group, m4int16_t ai_iOpLevel, m4int16_t ai_iArgs,
		ClMIT_Flag::Value_t ai_TokenFlags, ClMIT_ChnValue::TYPE ai_NULLReturnType,
		ClMIT_Flag::Value_t ai_OperatorFlags ) ;

	m4bool_t IsFunction() { return m_oFlags.True(IS_FUNCTION) ; }
	m4bool_t IsToRight() { return m_oFlags.True(TO_RIGHT) ; }

	m4int16_t  GetOpLevel() { return m_iOpLevel ; }
	m4int16_t  GetArgCount() { return m_iArgs ; }	

} ;

//======================================================================ClENG_TkNormalOperator

// Podriamos crear una clase por operador: aumentaría la velocidad al no haber cases
//	internos a la hora de calcular.

class ClENG_TkNormalOperator : public ClENG_TkOperator {

	//ClENG_Token flags     :DEAL_WITH_NULLS
	//ClENG_TkOperator flags:TO_RIGHT segun operador
	//NULLReturnType=NO_INIT (sin sentido con DEAL_WITH_NULLS)

MIT_PUBLIC:

	enum NORMAL_OPERATOR { 

		//Redireccionados al operador correspondiente de ClMIT_ChnValue

		//ABSORPTION

		ABSORB_A=ClMIT_ChnValue::ABSORB_A,	//Precedencia 0, binario A]B
		ABSORB_B=ClMIT_ChnValue::ABSORB_B,	//Precedencia 0, binario A[B

		//ARITHMETICAL
		MINUS_SIGN=ClMIT_ChnValue::MINUS_SIGN,	//Precedencia 0, unarios --A
		DIV=ClMIT_ChnValue::DIV, MUL=ClMIT_ChnValue::MUL,//Precedencia 1, binarios A*B
														//Precedencia 2, binarios A+B
		ADD=ClMIT_ChnValue::ADD, SUBSTRACT=ClMIT_ChnValue::SUBSTRACT,


		//LOGICAL,
		// Prec 0, binarios A>B
		LESS=ClMIT_ChnValue::LESS, LESS_EQUAL=ClMIT_ChnValue::LESS_EQUAL, 
		EQUAL=ClMIT_ChnValue::EQUAL, NOT_EQUAL=ClMIT_ChnValue::NOT_EQUAL, 
		GREATER=ClMIT_ChnValue::GREATER, GREATER_EQUAL=ClMIT_ChnValue::GREATER_EQUAL,

		AND=ClMIT_ChnValue::VAL_AND, OR=ClMIT_ChnValue::VAL_OR,		//Precedencia 1, binario A & B
		NOT=ClMIT_ChnValue::VAL_NOT,					//Precedencia 1, unario  !A

		//STRING

		STR_CAT=ClMIT_ChnValue::STR_CAT

	} ;	

MIT_PRIVATE:
	
	NORMAL_OPERATOR m_Operator ;


MIT_PUBLIC:

	ClENG_TkNormalOperator() ;
	virtual ~ClENG_TkNormalOperator() { ClENG_TkNormalOperator::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkNormalOperator) ; }
	virtual void End() ;

	void Init( NORMAL_OPERATOR ai_Operator  ) ;

	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

	//Util para trace
	static m4char_t *GetOpeString( NORMAL_OPERATOR ai_Ope ) ;
	m4char_t *GetOpeString( ) { return GetOpeString(m_Operator); }


MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 
} ;

//======================================================================ClENG_TkReportTot

class ClENG_TkReportTot : public ClENG_TkOperator {

	//ClENG_Token flags     :DEAL_WITH_NULLS solo para ENTRY
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= Ver tabla

MIT_PUBLIC:

	enum ACTION { COUNT, TRUE_COUNT, FALSE_COUNT, NULL_COUNT, ALL_COUNT, 
		SUM, AVG, MAX, MIN, FIRST, LAST,  RESET, ENTRY } ;

MIT_PRIVATE:

	ClENG_Tot *m_poTot ;	//Link

	//Parametros ID de acu.

	static const struct StProp {
		m4char_t * m_pcTraceName ;
		m4int16_t m_iNArgs ;
		ClMIT_ChnValue::TYPE m_NULLReturnType ;
	}  m_Prop[] ;

	ACTION m_Action ;
	ClMIT_Str m_oName ;	//Para traza

MIT_PUBLIC:

	ClENG_TkReportTot() ;
	virtual ~ClENG_TkReportTot() { ClENG_TkReportTot::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkReportTot) ; }
	virtual void End() ;

	//Si ai_poReport es NULL, error local.
	//Da true si todo va bien

	m4bool_t Init(
		ClENG_Report *ai_poReport, const m4char_t * ai_pcIdAcu, ACTION ai_Action,
		ClENG_StorageDep *ai_poDep,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;
	
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 
} ;

//======================================================================ClENG_TkSectionInfo

class ClENG_TkSectionInfo : public ClENG_TkOperator {

	//ClENG_Token flags     :NINGUNO
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= ver tabla


MIT_PUBLIC:

	enum ACTION { RV_COUNT, FIRST_DATE, LAST_DATE, START_DATE, END_DATE, SLICE_INDEX,
				  SLICE_COUNT, GET_PAGE, IS_EOF } ;

MIT_PRIVATE:

	ClENG_Section *m_poSect ;	//Link

	//Parametros ID de acu.
	static const struct StProp {
		m4char_t * m_pcTraceName ;
		m4int16_t m_iNArgs ;
		ClMIT_ChnValue::TYPE m_NULLReturnType ;
	}  m_Prop[] ;

	//Acción a ejecutar (ojo, son indice de m_piNArgs, y de un array de trace ).
	ACTION m_Action ;

MIT_PUBLIC:

	ClENG_TkSectionInfo() ;
	virtual ~ClENG_TkSectionInfo() { ClENG_TkSectionInfo::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkSectionInfo) ; }
	virtual void End() ;

	//Si IdSec no es NULL_SEC, se busca en el report. Para ello, ai_poReport no debe ser 
	//	nulo, y debe de encontrarse la seccion.
	//Si IdSec es NULL_SEC, se usara ai_poSect, que no debe ser nula.
	//Da true si todo va bien

	enum {NULL_SEC=-1} ;

	m4bool_t Init(
		ClENG_Report *ai_poReport, ClENG_Section *ai_poSect, m4uint32_t ai_iIdSec, ACTION ai_Action,
		ClENG_StorageDep *ai_poDep,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;
	
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 
} ;


//=============================================================================ClENG_TkCTTot


class ClENG_TkCTTot : public ClENG_TkOperator {

	//ClENG_Token flags     :DEAL_WITH_NULLS solo para ENTRY
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= Ver tabla


MIT_PUBLIC:

	enum ACTION { COUNT, 
		TRUE_COUNT, FALSE_COUNT, NULL_COUNT, ALL_COUNT,
		SUM, AVG, MAX, MIN, FIRST, LAST, ENTRY, RESET, HEADER, KEY } ;

MIT_PRIVATE:

	ClENG_KeyTot * m_poKeyTot;			
	ClENG_KeyTot::StInfo m_oInfo ;		   

	static const struct StProp {
		m4char_t * m_pcTraceName ;
		m4int16_t m_iNArgs ;
		ClMIT_ChnValue::TYPE m_NULLReturnType ;
	}  m_Prop[] ;


	enum LOAD_MODE { STATIC, DYNAMIC_NORMAL, DYNAMIC_PARTIAL } ;
	LOAD_MODE m_LoadMode ; m4int16_t m_LoadLevel ; 

	m4int16_t m_iIndexTree;	//Indices
	m4int16_t m_iIndexTot;
	m4int16_t *m_piLevelIndex, m_iArrCount ;	//Alocados

	ACTION m_Action ;

MIT_PUBLIC:

	ClENG_TkCTTot() ;
	virtual ~ClENG_TkCTTot() { ClENG_TkCTTot::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkCTTot) ; }
	virtual void End();
	
	//Segun el tipo de Entry, pilla los parametros pertinentes
	m4bool_t Init( 
		ACTION ai_Action, ClENG_Report* ai_poReport, ClMIT_Args *ai_poArg,
		ClENG_StorageDep *ai_poDep,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect=ClMIT_Trace::IMMEDIATE_ERROR 
	);

	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

	//Se usa para obtener info de diplay para VB.
	//Parsea lexicamente la Expresion pasada, dando informacion del primer %CT.... 
	//	que encuentre. 
	//La informacion se rellena siempre que se pueda (no se garantiza que la expresion CT sea
	//	correcta). Si no se rellena porque no procede o por error, la cadena estara vacia.
	//	Func es solo el nombre de la funcion, sin () ni argumentos.
	//Si devuelve 0, no hay %CT a analizar. Si no, devuelve el indice al siguiente caracter 
	//	despues de %CT, de tal forma que pueden procesarse todos los %CT de una expresion 
	//	de esta forma:
	//		m4uint32_t Index, Incr ;
	//		Index=Incr=0 ;
	//		do {
	//			Index += Incr ;
	//			Incr=GetNextCTInfo(&Expr[Index],....) ;
	//			if (Incr==0) break ;
	//			....Procesar
	//		} while (M4_TRUE) ;

	static m4uint32_t GetNextCTInfo(m4char_t *ai_pcExpr, 
		ClMIT_Str *ao_IdCT, ClMIT_Str *ao_IdTree, ClMIT_Str *ao_IdTot, ClMIT_Str *ao_Func ) ;

MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue* ao_Value);

};


//======================================================================ClENG_TkNODEInfo

class ClENG_TkNODEInfo : public ClENG_TkOperator {

	//ClENG_Token flags     :NINGUNO
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= ver tabla


MIT_PUBLIC:

	enum ACTION { IS_EOF, NVL } ;

MIT_PRIVATE:

	ClMIT_Chn * m_poChn ;	
	ClMIT_ChnNode *m_poNode ;	//Link

	static const struct StProp {
		m4char_t * m_pcTraceName ;
		m4int16_t m_iNArgs ;
		ClMIT_ChnValue::TYPE m_NULLReturnType ;
	}  m_Prop[] ;

	//Acción a ejecutar (ojo, son indice de m_piNArgs, y de un array de trace ).
	ACTION m_Action ;

MIT_PUBLIC:

	ClENG_TkNODEInfo() ;
	virtual ~ClENG_TkNODEInfo() { ClENG_TkNODEInfo::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkNODEInfo) ; }
	virtual void End() ;

	m4bool_t Init( 
		ACTION ai_Action, ClENG_Report* ai_poReport, m4char_t *ai_pcIdNODE, 
		ClENG_StorageDep *ai_poDep, ClMIT_Chn *ai_poChn=NULL, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect=ClMIT_Trace::IMMEDIATE_ERROR 
	);
	
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 
} ;

//======================================================================ClENG_TkPI

class ClENG_Field;

class ClENG_TkPI : public ClENG_TkOperator {

	//ClENG_Token flags     :NOT_EVALUATE_SON
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= NO_INIT, irrelevante 

MIT_PRIVATE:

	ClENG_Field* m_poClField;
	ClMIT_Str m_oIdPI;
	ClENG_FieldsManager *m_poFieldManager ;	//Simple Link

MIT_PUBLIC:

	ClENG_TkPI() ; 
	virtual ~ClENG_TkPI() { ClENG_TkPI::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkPI) ; }
	virtual void End() {m_poClField = NULL;};

	//Si ai_poFieldManager es NULL, error local.
	//Da true si todo va bien

	
	m4bool_t Init(m4char_t *ai_pcIdPI,ClENG_FieldsManager *ai_poFieldManager,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;
	
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	// Accedo al gestor de fields le pido el PI y con eso ya tengo ClENG_Field
	// y la copia del value asociado al field en concreto.
	virtual void EvaluateThis(ClMIT_ChnValue* ao_Value);
} ;


//======================================================================ClENG_TkExtractString


class ClENG_TkExtractString : public ClENG_TkOperator {

	//ClENG_Token flags     :NOT_EVALUATE_SON
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= NO_INIT, irrelevante 

MIT_PRIVATE:

	ClMIT_Str m_oIdString;
	StENG_ReportEnv::TrLitMap_t *m_poTrLit ;

MIT_PUBLIC:

	ClENG_TkExtractString() { };
	virtual ~ClENG_TkExtractString() { ClENG_TkExtractString::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkExtractString) ; }
	virtual void End() ;

	//Si ai_poTrLit es NULL, error local.
	//Da true si todo va bien

	m4bool_t Init(m4char_t *ai_pcIdString, StENG_ReportEnv::TrLitMap_t *ai_poTrLit, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;
	
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	// Accedo al gestor de fields le pido el PI y con eso ya tengo ClENG_Field
	// y la copia del value asociado al field en concreto.
	virtual void EvaluateThis(ClMIT_ChnValue* ao_Value);
} ;

//=============================================================================ClENG_TkThreadInfo

class ClENG_TkThreadInfo : public ClENG_TkOperator {

	//ClENG_Token flags     :NINGUNO
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= Ver tabla


MIT_PUBLIC:
	
	enum ACTION { Report_Id, Report_Name, Page, Group_Page, IsReprint,
				  GetFilePath, GetFileName, GetFullPath, SET_DEL_BREAK, IS_DEL_BREAK } ;

MIT_PRIVATE:

	ClENG_Report* m_poReport ;	//Simple link
	ClENG_OutDevice *m_poDvc ;	//Simple link
	ClENG_ReportRequest *m_poExeParams ;	//Simple Link

	static const struct StProp {
		m4char_t * m_pcTraceName ;
		m4int16_t m_iNArgs ;
		ClMIT_ChnValue::TYPE m_NULLReturnType ;
	}  m_Prop[] ;

	//Accion a ejecutar
	ACTION m_Action ;

MIT_PUBLIC:

	ClENG_TkThreadInfo() ;
	virtual ~ClENG_TkThreadInfo()  { ClENG_TkThreadInfo::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkThreadInfo) ; }
	virtual void End() ;
		
	m4bool_t Init( 
		ClENG_Report *ai_poReport, ClENG_OutDevice *ai_poDvc,
		ClENG_ReportRequest *ai_poExeParams,
		ACTION ai_Action, ClENG_StorageDep *ai_poDep,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ;
};


//=============================================================================ClENG_TkSysInfo

class ClENG_TkSysInfo : public ClENG_TkOperator {

	//ClENG_Token flags     :DEAL_WITH_NULLS para IS_NULL e IIF y NVL  y NOT_EVALUATE_SON para IIF y NVL
	//ClENG_TkOperator flags:IS_FUNCTION
	//NULLReturnType= Ver tabla

MIT_PUBLIC:

	enum SYSINFO { GET_CURRENT_DATE, GET_GMT_DATE, TODAY, GET_CURRENT_TIME, 
				   GET_CURRENT_TS, GET_GMT_TIME, GET_GMT_TS, TO_DATE,
				   TO_TIME, TO_TS, IIF, NVL, STRING_F, SPACE_F, STR_IN, STR_NIN,
				   MID, TO_LONG, TO_DOUBLE, TO_PRECISION_DOUBLE, CHAR_F, EXCH_ITEM_CURR,
				   EXCH_CURRENCY, EXCH_DEF_CURRENCY, DEF_EXCHANGE, GET_DEFAULT_CURRENCY,
				   IS_NULL, STR_LEN, CSTR, ALIGN, FORMAT_EXPR, FORMAT_STR, FORMAT_DEF_CUR,
				   PI, SAVE_ITEM, GET_DEFAULT_DATE, GET_DEFAULT_TYPE,
				   DEFAULT_EXCHANGE2, EXCHANGE_ITEM_CUR2, GET_CURRENCY_VALUE,
				   ORGANIZATION_TYPE, ORGANIZATION
	} ;

MIT_PRIVATE:

	//Parametros ID de acu.

	static const struct StProp {
		m4char_t * m_pcTraceName ;
		m4int16_t m_iNArgs ;
		ClMIT_ChnValue::TYPE m_NULLReturnType ;
	}  m_Prop[] ;

	SYSINFO m_Action ;

	ClMIT_Chn			* m_poChn ;	//Simple Link
	ClENG_ReportRequest *m_poExeParams ;	//Simple Link
	ClENG_DBFormat		*m_poDBFormat ;	//Simple Link
	ClENG_FieldsManager *m_poFieldManager ;	//Simple Link

	// Indica si están activas la pruebas unitarias. En tal caso se actual el valor de fecha
	// que se debe utilizar
	m4bool_t DateForTesting(m4date_t *ai_Date) ;

MIT_PUBLIC:

	ClENG_TkSysInfo() ;
	virtual ~ClENG_TkSysInfo() { ClENG_TkSysInfo::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_TkSysInfo) ; }
	virtual void End() ;

	//ai_poChn solo lo usa SAVE_ITEM, ORGANIZATION_TYPE y ORGANIZATION
	//ai_poExeParams lo usa EXCH_DEF_CURRENCY, DEF_EXCHANGE, GET_DEFAULT_CURRENCY,
	//	GET_DEFAULT_DATE, GET_DEFAULT_TYPE, FORMAT_DEF_CUR, DEFAULT_EXCHANGE2
	//ai_poDBFormat lo usa FORMAT_EXPR, FORMAT_STR, FORMAT_DEF_CUR
	//ai_poFieldManager lo usa PI.

	m4bool_t Init(
		SYSINFO ai_Action, ClENG_StorageDep *ai_poDep, 
		ClMIT_Chn *ai_poChn, ClENG_ReportRequest *ai_poExeParams,
		ClENG_DBFormat *ai_poDBFormat, ClENG_FieldsManager *ai_poFieldManager,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;
	
	virtual void GetTraceString(ClMIT_Str *ao_Str) ;

MIT_PROTECTED:

	virtual void EvaluateThis(ClMIT_ChnValue *ao_Value) ; 
} ;


//=============================================================================ClENG_Expression

class ClENG_Expression {

MIT_PROTECTED:
	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PRIVATE:

	ClMIT_Str   m_oExpr ;	//Se copia en el Init, y ya no vuelve a tocarse: 
						//	utilizable como Location en Trace. Si no hay expression, ""
	ClENG_Token *m_poRoot ;	//Alocado, responsable de destruirlo. NULL si no hay expresion
	ClMIT_Str    m_oTraceId ; //Cadena auxiliar de trace

MIT_PUBLIC:
	
	ClENG_Expression() ;
	virtual ~ClENG_Expression() { ClENG_Expression::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Expression) ; }
	virtual void End() ;

	// Puede ser que una expresion se cargue en un momento en el que aun no puede 
	//	pre-analizarse, debido a que el entorno de analizacion aun no esta completo,
	//	por ello se guarda la cadena y aun no se preanaliza.
	
	void Init(const m4char_t *ai_pcExpr, const m4char_t *ai_TraceId) ;

	//Comom un Init, solo que conserva el TraceId actual.

	void ChangeExpression(const m4char_t *ai_pcExpr) ;

	// Resultado de preanalizacion. Lo pone ClENG_Analex.
	// Puede lanzarse varias veces, si ya hay una preanalizacion, se borra.
	// NULL si la expresion esta vacia.
	
	void PreAnalex(ClENG_Token *ai_poRoot) ;

	// Retorna TRUE si no hubo ningun error de evaluacion, false en otro caso.
	// Si no hay expresion preanalizada, da true.

	m4bool_t Evaluate(ClMIT_ChnValue *ao_poValue, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	const m4char_t *GetExpressionStr() { return (m4char_t *)m_oExpr ; }

	const m4char_t *GetTraceId() { return (m4char_t *)m_oTraceId ; }
	void SetTraceId(const m4char_t *ai_TraceId) { m_oTraceId.StrCpy(ai_TraceId) ; }
	
	// Solo usado en debug. Muestra el arbol de operaciones

	void Trace() ;

	// Resetea las extern Prop del arbol de tokens 

	void ResetExternProp( ClMIT_ItemExternProp * ai_poExternProp ) ;

	m4bool_t IsEmpty()  { return M4_BOOL( m_poRoot==NULL ); } 

	m4bool_t IsExpressionStrEmpty()  ;
} ;

//=============================================================================ClENG_Analex

class ClENG_Analex {

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;	//Simple link

	// ----------------------------- Miembros --------------------------------------------	

MIT_PUBLIC:

	// Enviroment de la expresion. Mientras esta viva, deben de existir los objetos aqui
	//	referenciados, pues algunos tokens pueden apuntar a ellos. Los token nunca 
	//	destruyen ni crean estos objetos.
	
	struct StExpressionEnv {
		ClMIT_Chn    *m_poChn ;			//Puede ser NULL si no hay
		ClMIT_ChnNode *m_poDefaultNode ;	//Puede ser NULL si no hay
		ClMIT_ChnItem::LOAD_MODE m_LoadMode ;
		ClMIT_ItemExternProp  * m_poItemExternProp ; //Puede ser NULL si no hay
		ClENG_FieldExternProp * m_poFieldExternProp ; //Puede ser NULL si no hay
		ClENG_FieldsManager *m_poFieldManager ;	//Puede ser NULL si no hay
		ClENG_Report *m_poReport ;			//Puede ser NULL si no hay
		ClENG_Section *m_poSect ;			//Puede ser NULL si no hay
		ClENG_StorageDep *m_poDependencies ; //Puede ser NULL si no hay
		ClENG_OutDevice  *m_poDvc ;			//Puede ser NULL si no hay
		ClENG_ReportRequest *m_poExeParams ;	//Puede ser NULL si no hay
		ClENG_DBFormat		*m_poDBFormat ;	//Puede ser NULL si no hay
		StENG_ReportEnv::TrLitMap_t *m_poTrLit ; //Puede ser NULL si no hay

		m4int16_t m_iLayoutPage ;	//Para expresiones de fields, pagina en la que estan

		StExpressionEnv() ;	//Resetea valores
	} ;						

MIT_PRIVATE:

	typedef list<ClENG_Token *> TokenList_t ;
	TokenList_t m_oTokenList ;

	ClENG_Expression *m_poExpr ;	//Auxiliar, simple link

MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	ClENG_Analex() ;
	virtual ~ClENG_Analex() { ClENG_Analex::End(); }
	virtual size_t GetSizeof() { return sizeof(ClENG_Analex) ; }
	virtual void End() ;

	// Da true si tiene exito. Pasa tanto el analizador lexico, como sintactico, y deja 
	//	en ai_poExpr la expresión lista para evaluarse tantas veces como se quiera.
	// Si no tiene exito, ai_poExpr no se prenaliza.

	m4bool_t Init(
		StExpressionEnv * ai_pstEnv, ClENG_Expression *ai_poExpr, 
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	) ;


	//Preanaliza la expresion
	// 	-ai_pcIndexInfo contiene o una cadena codificando la iteracion en el arbol CT, en la 
	//		forma  "IndexLevel0_IndexLevel1_..._LastIndex" resuelta. Esta cadena se usa para
	//		obtener los indices tanto de los PI como de los CT.
	//	-Convierte secuencias $CT.IdCt.IdLevel en numericos. 
	//	-Las ocurrencias %Resolve_pi.IdPi son transformadas a %Resolve_pi.IdPi_ai_IdPi. Por
	//		ejemplo, un %Resolve_pi.A se trasforma en  %Resolve_pi.A_1
	//Deja la cadena ya convertida en ai_poBuffer. Si tiene problemas, lanza errores locales.
	//La cadena de salida siempre tiene la expresion resultante (si hay error, una cadena con un 
	//	mensaje de error).
	
	static void PrepareForAnalexCT( 
		m4char_t *ai_pcExpr, m4char_t *ai_pcIndexInfo , 
		ClMIT_Str* ai_poBuffer, m4bool_t ai_bSwDependencies=M4_FALSE ); 

	// Convierte secuencias $ct_page a la pagina a la que pertenece el field.
	//El objeto de entrada y salida es el mismo.
	static void PrepareForAnalexField( ClMIT_Str *ai_poBuffer, StExpressionEnv* ai_pstEnv); 

	//Corre los dos anteriores en el orden adecuado (primero field y luego CT)
	
	static void PrepareForAnalexCTAndField( 
		m4char_t *ai_pcExpr, m4char_t *ai_pcIndexInfo , 
		StExpressionEnv* ai_pstEnv,
		ClMIT_Str* ai_poBuffer, m4bool_t ai_bSwDependencies=M4_FALSE) ;

	enum {MAX_LE_INCLUSIONS=20} ;

	//El objeto de entrada y salida es el mismo. Admite recursion de LongExpressions 
	//	(una dentro de otra). Hay un tope maximo de recursiones para detectar bucles infinitos.
	typedef map<ClMIT_Str,ClMIT_Str, less<ClMIT_Str> > LEMap_t;
	static m4bool_t PrepareForLongExpr( ClMIT_Str *ai_poBuffer, LEMap_t * ai_poLE );

	// Traduce literales. Da false si hubo problemas.

	static m4bool_t GetTranslatedString(const m4char_t* ai_pcString, ClMIT_Str* ao_pTranslated, 
		StENG_ReportEnv::TrLitMap_t * ai_poTrLit) ;


MIT_PRIVATE:

	// ----------------------------- Analizador lexico------------------------------------	

	// Da true si todo OK. Si no, false, siempre provoca errores locales.
	// Pasa el analizador lexico.

	m4bool_t AnaLex(const m4char_t *ai_pcStr, StExpressionEnv * ai_pstEnv ) ;

	
	// Da la direccion del primer caracter de la cadena ai_pcStr, que no esta compuesta por
	//	caracteres separadores de tokens (blancos, tabuladores, ...).

	const m4char_t * NextString(const m4char_t *ai_pcStr) ;

	enum CHARACTER_TYPE { END_STRING, TOKEN_SEPARATOR, INTRA_TOKEN_SEPARATOR, TOKEN_CHAR } ;

	CHARACTER_TYPE GetCharType( m4char_t ai_cChar ) ; 
	
	
	// ----------------------------- Analizador sintáctico--------------------------------	

	// Da true si todo OK. Si no, false, siempre provoca errores locales.

	m4bool_t AnaSintax() ;


	//Función RECURSIVA.
	//-Beg y End marcan el rango de tokens (incluyendo Beg y End), de los que es 
	//	responsable la función de procesar. End NO puede apuntar a al fin de la lista, sino
	//	al ultimo token valido. 
	//-El metodo es responsable de analizar la sub-lista de tokens, e ir generando el
	//	árbol de tokens. 
	//-SOLO SI UN TOKEN pasa a ser hijo de algún otro, el hijo se extrae 
	//	de la lista, poniendo a NULL el puntero en la lista, y haciendolo hijo del token, 
	//	que será  en última instancia responsable de borrarlo. El token del iterador Root 
	//	NO se extrae (aunque se le pueden borrar los hijos), es responsabilidad del llamador.

	//-Da false si hay algun problema. En ese caso, todos posibles sub-arboles de Tokens 
	//	que hayan podido crearse y extraerse de la lista, se borran.
	//-Da true si todo va bien. En ese caso, el token de ai_poRoot tiene enganchado el
	//	sub-arbol valido.
	
	m4bool_t GenerateTree(
		TokenList_t::iterator & ai_poBeg, TokenList_t::iterator & ai_poEnd,
		TokenList_t::iterator & ai_poRoot
	) ; 	

	// Hace una busqueda del token de tipo OPERATOR, más cercano a ai_Beg, dentro de los 
	//	limites impuestos por Beg y End ( Beg esta incluido en el rango, End NO lo esta.
	//	End puede apuntar al end() de la lista, y Beg solo si lo hace End ).
	// Selecciona el operator de menor precedencia (es el ultimo a ejecutar, el de mayor 
	//	valor OpLevel, el que esta en los () más externos).
	// ai_Found, a la salida, apuntará a : 
	//		-Al OPERATOR encontrado. Si es binario, no hay ningún testeo adicional, 
	//			mientras que si es unario, se comprueba que a derecha o izquierda, según
	//			la asociatividad, no haya nada más que combinaciones de ( ).
	//		-Si no hay ningún OPERATOR, apunta a un token tipo ATOMIC_VALUE. Se testea que 
	//			solo pueda existir este token, y en todo caso secuencias (). Si 
	//			no es así, dará error local, y se devolverá false. De la misma forma, 
	//			tambien da error si no hay ningún ATOMIC_VALUE.
	//		-A end() SOLO si hay algun error (si no, al OPERATOR o al ATOMIC_VALUE).
	// Da true si todo va bien, false si hay problemas.

	m4bool_t SearchLessOperator(
		TokenList_t::iterator & ai_poBeg, TokenList_t::iterator & ai_poEnd,
		TokenList_t::iterator & ai_Found) ;

	//Se ejecuta una vez por expresion y argumento de funcion. 
	//Testea la integridad de los parentesis.

	m4bool_t TestParenthesis(TokenList_t::iterator &ai_Begin, TokenList_t::iterator &ai_End ) ;

	// hace hijo de Tk al token de It, poniendo en It el puntero a NULL.

	void TransferFromListToToken(TokenList_t::iterator & ai_poIt, ClENG_Token *ai_poTk ) ;


	//Beg y End tienen el mismo sentido que en SearchLessOperator
	//Buscan, desde Beg, un subargumento. El subargumento abarca desde Beg (incluido), hasta
	//	que se encuentra un ARG_SEPARATOR o un PRECEDENCE de cierre, ambos a nivel 0.
	//-ao_Found, a la salida, puede tener:
	//	-Token de tipo ARG_SEPARATOR, si no es el ultimo argumento. El subargumento abarca
	//		desde Beg hasta Found. Se hace un TestParenthesis() en el argumento.
	//	-Token de tipo PRECEDENCE, si es el ultimo argumento. El subargumento abarca
	//		desde Beg hasta Found. Se testea que más allá del paréntesis de cierre solo
	//		pueda haber combinaciones de ( ). Se hace un TestParenthesis() en el argumento.
	//	-A end() SOLO si hay error.
	//-SwEmpty es true si no hay argumento, osea, solo si Func().
	// Da true si todo va bien, false si hay problemas.

	m4bool_t SearchFunctionArg(
		TokenList_t::iterator & ai_poBeg, TokenList_t::iterator & ai_poEnd,
		TokenList_t::iterator & ao_Found, m4bool_t *ao_bSwEmpty
	) ;
} ;



#endif

