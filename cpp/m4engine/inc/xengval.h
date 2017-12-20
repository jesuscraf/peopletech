//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             engvalue.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//
//    Objeto Value basico del engine
//
//
//==============================================================================
#ifndef __ENGVALUE__H__
#define __ENGVALUE__H__


#include "clstr.h"
#include "m4clflag.h"
#include "formattp.h"

//Valores numericos y cadena anonima para BOOLEAN

#define STR_VALID_BOOLEAN_TRUE "1"
#define STR_VALID_BOOLEAN_FALSE "0"
#define DOUBLE_VALID_BOOLEAN_TRUE 1
#define DOUBLE_VALID_BOOLEAN_FALSE 0

//Defectos puestos por SetNULL

#define NULL_DEFAULT_NUMERIC_V 0
#define NULL_DEFAULT_NUMERIC_P 20
#define NULL_DEFAULT_NUMERIC_S 0
#define NULL_DEFAULT_STRING ""
#define NULL_DEFAULT_DATE   ClMIT_Dates::MinusInfiniteDate()
#define NULL_DEFAULT_TS     ClMIT_Dates::MinusInfiniteDate()
#define NULL_DEFAULT_BOOLEAN   M4_FALSE
#define NULL_DEFAULT_CURRENCY_V 0
#define NULL_DEFAULT_CURRENCY_P 20
#define NULL_DEFAULT_CURRENCY_S 0
#define NULL_DEFAULT_CURRENCY_T ""
#define NULL_DEFAULT_CURRENCY_ED ClMIT_Dates::MinusInfiniteDate()
#define NULL_DEFAULT_CURRENCY_ET ""
#define NULL_DEFAULT_VARIANT_V 0
#define NULL_DEFAULT_VARIANT_P 18
#define NULL_DEFAULT_VARIANT_S 6


//=========================================================================ClMIT_ChnValue

class ClMIT_ChnValue
{
MIT_PRIVATE:

	StMIT_Inspector *m_poInsp ;	//Simple link
MIT_PUBLIC:

	enum TYPE { 
		NUMERIC, STRING, DATE, TIME_STAMP, BOOLEAN, CURRENCY, VARIANT, NO_INIT,
		TYPE_COUNT	//No usar, es un contador interno de numero de operadores
					//Son indices en el array de texto trace para el tipo
	} ;

	enum OPERATOR { //Son indices en el array de texto trace para operator, y para otros

		//Logicos 

		LESS, LESS_EQUAL, EQUAL, NOT_EQUAL, GREATER, GREATER_EQUAL, 
		VAL_AND, VAL_OR, VAL_NOT, 

		//Aritmeticos

		MINUS_SIGN, DIV, MUL, ADD, SUBSTRACT,

		//Absorcion

		ABSORB_A, ABSORB_B,

		//Cadena

		STR_CAT,	

		OPERATOR_COUNT	//No usar, es un contador interno de numero de operadores

		/* TABLA DE OPERADORES Y TIPOS VALIDOS (B-binario, U-unario)
		
		Todos B		LESS	LESS_EQUAL		EQUAL	NOT_EQUAL	GREATER		GREATER_EQUAL 
		---------------------------------------------------------------------------------
		NUMERIC		Si		Si				Si		Si			Si			Si
		STRING*		Si		Si				Si		Si			Si			Si
		DATE		Si		Si				Si		Si			Si			Si
		TIME_STAMP	Si		Si				Si		Si			Si			Si
		BOOLEAN		No		No				Si		Si			No			No
		  
		*La comparacion de cadenas es es case-sensitive, se obvian los blancos finales de 
		ambas cadenas.
		Si cualquiera de estos operandos manipula operadores NULL, devolvera un NULL

					AND (B)				OR (B)				NOT (U)
		---------------------------------------------------------------------------------
		BOOLEAN		Si					Si					Si
					NULL&NULL=NULL		NULL|NULL=NULL		!NULL=NULL
					FALSE&NULL=FALSE	FALSE|NULL=NULL
					TRUE&NULL=NULL		TRUE|NULL=TRUE
		
					MINUS_SIGN (U)		DIV (B)		MUL (B)		ADD (B)	SUSBSTRACT (B)
		---------------------------------------------------------------------------------
		NUMERIC		Si					Si			Si			Si		Si

		Si cualquiera de estos operandos manipula operadores NULL, devolvera un NULL

		STR_CAT (B)	
		---------------------------------------------------------------------------------
		STRING  	Si					

		STR_CAT opera con datos de cualquier tipo, excepto con CURRENCY y NO_INIT. 
		NULL && NULL=NULL, NULL && A= A
		
		ABSORB_A y ABSOR_B son binarios, y validos para todos los tipos (incluido NO_INIT).
			ABSORB_A devuelve incondicionalmente el operador B  (A ABSORB_A B = B ), y
			ABSORB_B devuleve incondicionalmente el aoperador A. No le afecta que el 
			operando desechado sea o no NULL.
		
		*/
	} ;

MIT_PRIVATE:
	
	
	//--------Miembros para tipo numerico y currency (valor de la moneda)

	m4double_t  m_dDouble ;			//Tambien lo usa BOOLEAN
	m4int16_t m_iPrecision, m_iScale ;	//Precision:digitos enteros, Escala:digitos decimales

	//--------Miembros para tipo fechas, timestamp y moneda (fecha de cambio)

	m4date_t    m_Date ;	

	//--------Miembros para tipo moneda

	ClMIT_Str      m_oExchType;	

	//--------Miembros para tipo cadena y moneda (tipo de moneda)

	ClMIT_Str m_oStr ;

	//-------Miembros comunes a todos los tipos
	
	TYPE m_Type ;

	// Tipo real para los items de tipo variant.
	TYPE m_RealType;
	
	enum VALUE_FLAGS {

		//Si  es true, sucesivos Init no permiten un cambio de tipo

		FIXED_TYPE=ClMIT_Flag::BIT0,

		//Flag de valor NULL. El constructor REAL del objeto y todos los Init lo ponen a
		//	FALSE, asi que el marcar un valor NULL tiene validez hasta el proximo Init o End.

		NULL_VALUE=ClMIT_Flag::BIT1,

		//Flag de updated, se pone a true al actualizar el valor, via Init o via SetNull (un
		//	set NULL actualiza tambien valores).

		UPDATED=ClMIT_Flag::BIT2

	} ;

	ClMIT_Flag m_oFlags ;

	enum OPERATOR_DATA_FLAGS {
		//Permitido este operador para este tipo? Si no ERROR (ademas de marcar como nulo)
		ALLOWED		=ClMIT_Flag::BIT0,	
		//Si todos los operandos son NULL, el resultado siempre es NULL independientemente de
		//	este o de cualquier otro falg.
		//Si true, el tipo opera con NULLs. Si false, no es error, pero resultado es NULL.
		DEAL_NULLS	=ClMIT_Flag::BIT1,	
		//Si true, el operador opera con tipos diferentes. Irrelevante en el caso de algun
		//	operando NULL y DEAL_NULLS activado. Filtro para tipos diferentes no-null.
		//	ERROR si no se cumple.
		MULTI_TYPE	=ClMIT_Flag::BIT2		
	} ;

	static const struct StOperatorData {

		ClMIT_Flag::Value_t m_oFlag ;

		//Util para poner el tipo NULL si hay errores:
		//	-Si NO_INIT, se pone el del 1 operando, o el del 2 si el primero es NO_INIT.
		//	-Si no, se pone el tipo indicado.

		TYPE  m_NULLType ;	
		
	} m_stOpeData[TYPE_COUNT][OPERATOR_COUNT] ;

	static const m4int16_t m_iOpeArgs[OPERATOR_COUNT]  ;

	static const m4char_t *m_pcTypeTraceStr[TYPE_COUNT] ;
	static const m4char_t *m_pcOpeTraceStr[OPERATOR_COUNT] ;

MIT_PUBLIC:

	// ----------------------------- Inicialización---------------------------------------	

	//-----------Inicializaciones para manipular monedas

	// Resetea y crea un acceso al canal moneda
	// Si no se inicializa al principio no puede hacerse conversiones de tipo currency.
	// Hace una llamada directa a la capa apichn
	
	static m4bool_t StartUpCurrency( ClMIT_Chn * ai_poChannel );

	//Datos referentes al cambio por defecto 
	static void SetDefExchCurr( m4char_t *ai_pcExchCurr ) ; 
	static void SetDefExchDate( m4date_t ai_ExchDate ) ;
	static void SetDefExchType( m4char_t *ai_pcExchType ) ;

	static const m4char_t * GetDefExchCurr() ;
	static m4date_t GetDefExchDate() ;
	static const m4char_t *GetDefExchType( ) ;

	//-----------Inicializaciones basicas del objeto 

	// Lo crea con tipo NO_INIT. Pone NULL a false.
	ClMIT_ChnValue() ;
	virtual ~ClMIT_ChnValue() { ClMIT_ChnValue::End(); }
	virtual size_t GetSizeof() { return sizeof(ClMIT_ChnValue) ; }

	//-----------Inicializaciones del objeto 

	// Al inicializar el tipo, este implica el tipo de almacenamiento.
	// La clase se encarga de destruir el dato (si es una cadena.aloca espacio si hace 
	//	falta, aunque esto, si es posible, se evita en posibles reinicializaciones).
	// Actualmente el tipo boolean, a nivel de canal, es un NUMERIC, en el que se interpreta
	//	un 1 como TRUE y un 0 como FALSE.
	// Si es FixedType (osea, el objeto pertenece a un ClChnItem), se producira un error 
	//	local si se intenta cambiar de tipo, y NO se toca el objeto.
	// InitDate pone NULL a true o false en funcion de si la fecha es +- infinito. En el
	//	resto de los casos, siempre pone NULL a false.
	// Todos los Init ponen a true el flag de Updated.

	void InitDouble( m4double_t  ai_dData, m4int16_t ai_iPrecision, m4int16_t ai_iScale  ) ;
	void InitString( const m4char_t  * ai_pcData ) ;
	void InitDate( m4date_t ai_Date ) ;
	void InitTimeStamp( m4date_t ai_Date ) ;
	void InitBoolean( m4bool_t ai_bSw ) ;
	//ai_pcCurr y ai_pcExchType pueden ser NULL, en cuyo caso se pondtan los valores a "".
	void InitCurrency( 
		m4double_t ai_dCurValue,	m4int16_t ai_iPrecision, m4int16_t ai_iScale,
		const m4char_t* ai_pcCurr, m4date_t ai_ExchDate, const m4char_t*  ai_pcExchType ) ;
	//Utiles en reinicializaciones
	void InitDouble( m4double_t  ai_dData) { InitDouble(ai_dData,GetPrecision(),GetScale() ); }	
	void InitCurrency( 
		m4double_t ai_dCurValue, const m4char_t* ai_pcCurr, m4date_t ai_ExchDate, const m4char_t*  ai_pcExchType ) 
		{ InitCurrency(ai_dCurValue,GetPrecision(),GetScale(),ai_pcCurr,ai_ExchDate,ai_pcExchType) ; }

	void InitVariant(const m4char_t * ai_pcData, TYPE ai_eRealType);
	void InitVariant(m4double_t ai_dData, TYPE ai_eRealType);
	void InitVariant(m4double_t ai_dCurValue, const m4char_t* ai_pcCurr, m4date_t ai_ExchDate, const m4char_t*  ai_pcExchType, TYPE ai_eRealType);

	// Constructor copia. Si el destino es FixedType, y el tipo no coincide con el del 
	//	origen, se producira un error local. El flag NULL y UPDATED es copiado.
	// m_poSource puede ser NO_INIT.
	void Init( const ClMIT_ChnValue *m_poSource ) ;

	// Borrara m_oStr si compete. Pone tipo NO_INIT y limpia el falg de NULL y de UPDATED.
	virtual void End() ;

	//-----------Manejo de flag NULL. 

	//  Normalmente, no hay que manejar esto desde fuera de esta clase, excepto en unos 
	//modulos que tratan explicitamente las marcas de nulos (carga de items, analex ...)

	// Pone la marca de NULL, y resetea valores segun tipo: numeric a 0, date a -infinito,
	//	string a "", boolean a false y currency todo a value 0 ó "". Admite tipo NO_INIT.
	//  Pone a true el flag de Updated.

	void SetNULL() { SetNULL(m_Type) ; }
	void SetNULL(TYPE ai_Type) ;

	// Limpia el flag, no altera tipo ni valores.
	void ClearNULL() { m_oFlags.Off(NULL_VALUE) ; } 

	m4bool_t IsUpdated() const { return m_oFlags.True(UPDATED) ; } 
	void ClearUpdated() { m_oFlags.Off(UPDATED) ; } 


	//-----------Manejo de flag FixedType. 

	void SetFixedType( m4bool_t ai_bSw ) { m_oFlags.Switch(FIXED_TYPE,ai_bSw) ; }
	m4bool_t IsFixedType() { return m_oFlags.True(FIXED_TYPE) ; }

	//-----------Sobrecargas para usar el objeto con STL

	ClMIT_ChnValue(const ClMIT_ChnValue& ai_oSource) ;
	ClMIT_ChnValue & operator = (const ClMIT_ChnValue &ai_oOpe);
	m4bool_t operator == (const ClMIT_ChnValue &ai_oOpe) const;
	m4bool_t operator < (const ClMIT_ChnValue &ai_oOpe) const;

	// ------------------------------Gets & Sets------------------------------------------	

	// NOTA: Los gets u operaciones sobre tipos incorrectos dan errores locales y retornan
	//	los mismos valores defecto que se retornarian si el value fuese de tipo NULL.

	//-----------Independientes de tipo

	TYPE GetType() const { return m_Type; }
	TYPE ClMIT_ChnValue::GetRealType() const ;

	m4bool_t IsNULL() const { return m_oFlags.True(NULL_VALUE) ; }	

	// Informacion operador-tipo


	static ClMIT_ChnValue::StOperatorData const * const GetOpeInfo( 
		ClMIT_ChnValue::TYPE ai_Type, ClMIT_ChnValue::OPERATOR ai_Ope) ;
		//{ return &ClMIT_ChnValue::m_stOpeData[ai_Type][ai_Ope] ; } 

	//Numero de operandos (actualmente 1 ó 2, si 1, siempre asociativo a derecha)

	static m4int16_t GetOpeArgs(ClMIT_ChnValue::OPERATOR ai_Ope) 
		{ return m_iOpeArgs[ai_Ope] ; } 

	//Por ahora, todos los operadores son asociativos a derecha

	static m4int16_t IsOpeRight(ClMIT_ChnValue::OPERATOR ai_Ope) { return M4_TRUE ; } ;

	//Util para trace, da el tipo como una cadena

	const m4char_t * GetTypeString() const { return GetTypeString(m_Type); }
	static const m4char_t * GetTypeString( TYPE ai_Type )  { return m_pcTypeTraceStr[ai_Type]; } 
	static const m4char_t * GetOpeString( OPERATOR ai_Ope ) { return m_pcOpeTraceStr[ai_Ope]; } 

	//-----------Solo para NUMERIC y CURRENCY

	m4int16_t GetScale() const ;
	m4int16_t GetPrecision() const ;
	void SetScale(m4int16_t ai_iScale)  ;
	
	//-----------Solo para el tipo CURRENCY

	m4double_t GetCurrencyValue() const ;
	m4char_t*  GetCurrencyType() const ; //Puntero de uso inmediato
	m4date_t   GetCurrencyExchDate() const ; 
	m4char_t*  GetCurrencyExchType() const ; //Puntero de uso inmediato

	// Cambia el valor y tipo del actual currency en base al tipo de moneda pasado.
	// Da true si tiene exito, false si no, en cuyo caso no altera el valor de la moneda actual.
	// Usa la capa apichn para hacer el cambio
	// Pone a true el flag de Updated.

	m4bool_t ExchangeCurrency( const m4char_t* ai_pcDestCur );

	//-----------Solo para NUMERIC

	m4double_t GetDouble() const  ;

	//-----------Solo para STRING

	m4char_t * GetString() const ;	//Puntero de uso inmediato

	//-----------Solo para FECHA y TIME_STAMP

	m4date_t   GetDate() const ;

	//-----------Solo para BOOLEAN
	
	m4bool_t   GetBoolean() const ;

	//-----------Solo para VARIANT
	m4char_t * ClMIT_ChnValue::GetVariantString() const;
	m4double_t ClMIT_ChnValue::GetVariantNumber() const;

	// ------------------------------Valores anonimos --------------------------------	

	// Este metodo obtiene el valor a poner desde una cadena. Por ello, hay que establecer
	//	un convenio de como se interpretan los valores:
	// -NUMERIC. La cadena admite el mismo formato que atof(). La precison es el numero de
	//		caracteres hasta encontrar un punto, y la escala es el numero de caracteres tras
	//		el punto, sin contar blancos finales.
	// -STRING. La cadena se copia tal cual.
	// -DATE. Debe estar en formato TS, osea: aaaa-mm-dd  
	// -TIME_STAMP. Debe estar en formato TS, osea: aaaa-mm-dd ??  
	// -BOOLEAN. La cadena "1" se interpreta como TRUE, y "0" como false. Otras cadenas dan 
	//	error local.
	// -CURRENCY. No sirve, puesto que requiere 4 parametros.

	// En ultima instancia llama al Init correspondiente segun el tipo, por lo cual actua 
	//	como estos.
	// Pone a true el flag de Updated.

	void InitAnonymous(const m4char_t *ai_pcAnonymousValue ) ;

	// Hay ocasiones en las que un nodo de canal es fisicamente de tipo string, pero 
	//	conceptualmente es una cadena anonima cuyo valor es realmente de otro tipo.
	// Estos metodos hacen los Gets en este contexto, siguiendo las mismas reglas que
	//	SetAnonymous para convertir los datos.
	// Ejemplo de un contexto donde es util: en campo VALUE de tablas de PROPERTIES.	
	// Todas dan errores locales si el tipo no es STRING, o no pueden convertir, 
	//	activando ademas el NULL.

	m4double_t			GetAnonymousDouble() const  ;
	const m4char_t *	GetAnonymousString()  const ;	//Punteros de uso inmediato.
	m4date_t			GetAnonymousDate() const ;	
	m4date_t			GetAnonymousTimeStamp() const ;	
	m4bool_t			GetAnonymousBoolean() const  ;

	static m4double_t		GetAnonymousDouble(const m4char_t *ai_pcAnonymousValue)  ;
	static m4double_t		GetAnonymousDouble(const m4char_t *ai_pcAnonymousValue, 
		m4int16_t *ao_iPrecision, m4int16_t *ao_iScale )  ;
	static const m4char_t * GetAnonymousString(const m4char_t *ai_pcAnonymousValue)  ;//Punteros de uso inmediato.
	static m4date_t			GetAnonymousDate(const m4char_t *ai_pcAnonymousValue)  ;
	static m4date_t			GetAnonymousTimeStamp(const m4char_t *ai_pcAnonymousValue)  ;
	static m4bool_t			GetAnonymousBoolean(const m4char_t *ai_pcAnonymousValue) ;

	//Inversas a las anteriores, partiendo de los valores obtienen cadenas anonimas
	
	//ai_iPrecision o ai_iScale < 0, no formatea a un numero fijo de decimales
	static void DoubleToAnonymous(m4double_t ai_dDouble, ClMIT_Str *ao_Str, m4int16_t ai_iScale=-1 )  ;
	//Formatea siempre sin decimales, tanatos caracteres como haga falta.
	static void NumberToAnonymous(m4double_t ai_dDouble, ClMIT_Str *ao_Str ) ;
	static void StringToAnonymous(const m4char_t * ai_pcStr, ClMIT_Str *ao_Str)  ;
	//static void DateToAnonymous( m4date_t  ai_Date, ClMIT_Str *ao_Str)  ;
	//static void TimeStampToAnonymous(m4date_t  ai_Date, ClMIT_Str *ao_Str)  ;
	static void BooleanToAnonymous(m4bool_t   ai_Bool, ClMIT_Str *ao_Str) ;

	//Para traza, deposita en ao_poStr una cadena que representa el valor
	void GetTraceValue( ClMIT_Str *ao_poStr ) const ;

	
	// ------------------------------Formateos (relacionados con str anonimas)------------	

	//  Este grupo de metodos no usa el flag de NULL para nada; es responsabilidad de otros
	//modulos testear esto si va a tener efecto en impresiones.	

	//   Transforma un dato de canal en los valores apropiados para la DLL de formateo.
	// -Si se devuelve FALSE:
	//		-El tipo no es soportado por la DLL. Utilizar la cadena ao_poStr directamente. Por
	//			ejemplo, un boolean es transformado a la cadena "T" o "F", y no hay que llamar
	//			a ningun formateo.
	//		-El tipo NO_INIT es convertido de esta forma, poniendo una cadena para indicar el
	//			error.
	// -Si se devuelve TRUE:
	//		-En ao_poStr esta la cadena tal cual espera recibirla la DLL.
	//		-En ao_iType esta el tipo de dato respecto a la llamada a la DLL. Puede pasarse
	//			un NULL, en cuyo caso no se almacena nada.
	//		-Es responsabilidad de la funcion que llama el obtener los parametros adecuados
	//			de formateo, que dependeran de ao_iType, y llamar a la funcion de formateo.
	//		-Ejemplo. Un item fecha es almacenado en la cadena en formato de entrada a la DLL,
	//			y el tipo es puesto conforme a esto.

	m4bool_t GetInputFormat( ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType=NULL ) const ;

	//Grupo de metodos como el anterior, solo que el dato se da directamente
	
	static 	m4bool_t GetInputFormatDouble( 
		m4double_t  ai_dDouble , m4int16_t ai_iPrecision, m4int16_t ai_iScale,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType=NULL )  ;
	static 	m4bool_t GetInputFormatString( 
		m4char_t  *ai_pcStr,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType=NULL )  ;
	static 	m4bool_t GetInputFormatDate( 
		m4date_t  ai_Date, 
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType=NULL )  ;
	static 	m4bool_t GetInputFormatTimeStamp( 
		m4date_t  ai_Date, 
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType=NULL )  ;
	static 	m4bool_t GetInputFormatBoolean( 
		m4bool_t ai_bSw,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType=NULL )  ;
	static 	m4bool_t GetInputFormatCurrency( 
		m4double_t  ai_dDouble , m4int16_t ai_iPrecision, m4int16_t ai_iScale,
		ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType=NULL )  ;

	static m4bool_t GetInputFormatVariant(m4char_t  *ai_pcValue, ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType);
	static m4bool_t GetInputFormatVariant(TYPE ai_eRealType, m4double_t ai_dValue, m4int16_t ai_iPrecision, m4int16_t ai_iScale, ClMIT_Str *ao_poStr, FORMAT_DATA_TYPE *ao_pType);

	// ------------------------------Comparaciones logicas--------------------------------	

	//Da TRUE si la operacion tiene exito. En caso de FALSE, el valor de vuelta sera NULL. 
	//	El tipo del NULL sera el tipo teorico del resultado (si no se sabe, se pondra a NO_INIT).
	//En operadores unarios, se ignora el segundo parametro (se hace caso al propio objeto). 
	//Los tipos, en operadores binarios,  deben de coincidir, o se produce un error local. 
	//	La unica excepcion es STR_CAT y los operadores de absorcion.
	//Si cualquiera de los dos tipos en operadores binarios, o el unico tipo en unarios, es 
	//	tipo NO_INIT, siempre hay un error local. 
	//Si Result es numerico, se copia la precision y escala que sean mayores del los dos 
	//	operandos (o se copia la del operando A si es unario).
	//Si en una operacion se ve inmmerso un Value marcado como NULL, el resultado tambien
	//	se marca como NULL, salvo las excepciones (ver tablas de operadores).
	//Actiualiza el flag de Updated de ao_poResult.
	
	m4bool_t Evaluate( 
		OPERATOR ai_Op, const ClMIT_ChnValue * ai_poOpe, ClMIT_ChnValue * ao_poResult ) const ;

};

//=========================================================================StMIT_NULLDisplayDef
// Almacenan cadenas de impresion para nulos.

struct StMIT_NULLDisplayDef {
MIT_PUBLIC:
	ClMIT_Str m_oStr[ClMIT_ChnValue::TYPE_COUNT] ;	//Acceso publico

	void End() ;	//Hace End sobre las cadenas (las vacia y pone a "").
	void Init(StMIT_NULLDisplayDef *ai_poSource) ;
} ;

#endif



