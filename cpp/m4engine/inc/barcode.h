//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             barcode.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/18/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//
// Definition:
//==============================================================================

#ifndef __BARCODE__H__
#define __BARCODE__H__

#include "field.h"

//===================================================================ClENG_BarCodeField
/*
 -Clase base para todos los codigos de barras.
 -No pueden formar parte de TOC, da error si se intenta.
 -Soporta alineaciones.
 -El valor resultante de la expresion debe ser de tipo string, o da error. Esto es asi
	intencionadamente, porque el uso de codigos de barras suele implicar formatos 
	con un numero de digitos prefijado, y si permitiesemos valores numericos seria 
	dificil estar seguro de la escala y precisión pasados.
 -Los codigos de barras necesitan fuentes especiales. El API de las fuentes tiene metodos
	para preguntar por mapeos de caracteres especificos a cada codificación.
*/


class ClENG_BarCodeField : public ClENG_Field {

MIT_PUBLIC:

	enum FLAGS { 
		CLIP=ClMIT_Flag::BIT0,
		FORMAT_NULL=ClMIT_Flag::BIT1	//En caso de NULL, dice si se desea formatear la cadena
	} ;

MIT_PRIVATE:

	ClMIT_Flag	   m_oFlag ;
	ClENG_FieldStr::ALIGN	m_AlignX, m_AlignY;
	DvcUnit_t m_OffsetX, m_OffsetY ;
	ClFONT_M_Font *m_poFont ;

	m4uint32_t m_uiIdFGColour ;	//IdColour foreground

	ClMIT_Str m_oCodedStr ;
	size_t    m_szIndStr, m_szNChar;

							//Para tratar impresion de NULLs 
	StMIT_NULLDisplayDef    m_oNULLDef ;


	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_BarCodeField();
	virtual ~ClENG_BarCodeField() { ClENG_BarCodeField::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_BarCodeField) ; }

MIT_PUBLIC:

	// Si se le pasa a la altura un valor negativo, el valor se toma como el numero de 
	//	lineas: la altura depende de la fuente elejida.
	// Si la anchura es negativa, se interpreta como numero de caracteres, y la anchura
	//   de calcula en funcion de la fuente. Se garantiza que siempre caben esos 
	//   caracteres, aunque la fuente sea proporcional.
	// Si el top o left son negativos, tambien se calculan en funcion del alto o ancho de 
	//	la fuente.
	// Si poFormat=NULL, no se formateara.
	
	void Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height, const m4char_t *ai_pcExpr, 
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,
		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
		m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, 
		m4bool_t ai_bSwClip,
		StMIT_NULLDisplayDef *ai_poNULLDef,
		m4char_t *ai_pcIdPi );
    

	virtual void End() ;

	//-------------------------Gets-----------------------------------------------------

	m4bool_t IsClip() {return m_oFlag.True(CLIP);}
	m4uint32_t GetIdFGColour() { return m_uiIdFGColour ; } 
	ClFONT_M_Font *GetFont() { return m_poFont ; }

	//-------------------------Fuciones virtuales  a implementar por derivadas----------

MIT_PROTECTED:

	//Llama antes que nada al metodo virtual Encode con el valor actual. La cadena 
	//	codificada que este metodo devueleve es la que se imprime (y sobre la que se
	//	aplica centrados, etc).
	//Actualiza siempre m_szIndStr (desde que char imprimir), m_szNChar (cuantos), y deja
	//	la cadena a imprimir en m_oFormatedStr.
	//Formatea la cadena y aplica relleno. Se usa SourceStr y m_oFormatedStr para aplicar 
	//	el formateo. Tambien actualiza OffsetX e OffsetY.

	virtual DvcUnit_t PostLoadValue() ;

	virtual m4bool_t FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) ;

	virtual void ResolvePi(ClMIT_ChnValue *ai_poValue);

	//Metodo a sobrecargar por cada derivada. Lo unico que debe hacer es codificar el
	//	valor depositado en ai_InValue y dejarlo en ao_CodedValue. 
	//Aunque haya error, actualziar CodedValue (con "" o lo que se quiera).

	virtual void Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue  )=0 ;

} ;

//===================================================================ClENG_BarCode39
/*
 CODIGO C39: 
	Codigo de barras alfanumerico. El codigo puede ser lo largo que sea para codificar 
	el dato. Se admiten 26 caracteres (A-Z), numeros (0-9) y los simbolos (-. *$/+%).
	NOTA: (como ha habido problemas, solo se soporta $ y %)
    El caracter '*' indica comienzo y fin de codigo. No necesita checksum.
*/

class ClENG_BarCode39 : public ClENG_BarCodeField {

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_BarCode39();
	virtual ~ClENG_BarCode39() { ClENG_BarCode39::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_BarCode39) ; }

	//Usar el Init del ClENG_BarCodeField
	
	virtual void End() { ClENG_BarCodeField::End(); }

MIT_PROTECTED:

	//-------------------------Virtuales------------------------------------------------

	virtual void Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) ;
} ;

//===================================================================ClENG_BarCodeI25
/*
  INTERLEAVED 25: 
	-Solo acepta numeros representados por parejas. 
	-Si el codigo contiene un numero de cifras impar, deberemos poner un 0 al principio. 
	-El codigo contiene 102 caracteres, 100 que representan todas las parejas posibles 
		de cifras (00 a 99) y los caracteres de Inicio y Fin.
*/

class ClENG_BarCodeI25 : public ClENG_BarCodeField {

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_BarCodeI25() ;
	virtual ~ClENG_BarCodeI25() { ClENG_BarCodeI25::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_BarCodeI25) ; }

	//Usar el Init del ClENG_BarCodeField
	
	virtual void End() { ClENG_BarCodeField::End(); }

MIT_PROTECTED:

	//-------------------------Virtuales------------------------------------------------

	virtual void Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) ;
} ;


//===================================================================ClENG_BarCode128
/*
BarCode 128 

-Tiene 3 sets de caracteres:
  
  -A. Mayusculas, puntuadores, numeros, etc. No soporta minusculas pero si los
   32 caracteres de control ASCII.
  -B. Soporta todo el ASCII (may, min, numeros, etc), excepto los char de control.
  -C. Como interleaved, para numerico, cada char es un par de digitos.

-Tiene chars de control especiales para cambiar entre A-B-C, una misma cadena puede
  tener representaciones distintas.
	
-Este charset puede representar una cadena de diversas formas. Debido a ello, hay
  dos formas diferentes de convertir la cadena:

-Formato libre (sin formato). Algoritmicamente se decide la mejor forma de 
  representación. Hay unos flags que modifican el comportamiento del algoritmo:

  -DefaultMode. Por defecto, se comienza en modo B, aunque puede permitirse el A.
	No se permite como defecto el modo C, este debe ser resultado de que el algoritmo
	decida cambiar a el.
  -Allow C flag. Si es true, se permite al algoritmo que cambie a modo C cuando lo crea
	oportuno. Si es false, no se pasara nunca al modo C, pues en A/B es representable 
	todo lo que hay en C.
  -Fill Zero flag. Dice si en modo C se rellena un cero a la izquierda cuando el
	numero de digitos sea impar. Si se rellena, todo se codifica en modo C 
	( es decir un 12345 seria un (01)(23)(45) ) y si no se rellena, el digito de mas
	a la derecha se representara en modo A o B ( es decir, (12)(34)(ToB)5 )
  
  El comportamiento del algoritmo es :
	  
  -Solo si el C flag es true:
	-Detectamos donde esta el proximo bloque de codificacion C, que será el formado por 
		n digitos seguidos, al menos 4, y formado por los n digitos mencionados 
		(si Fill Zero flag es TRUE) o por la maxima cantidad multiplo de 2 
		(si Fill Zero flag es FALSE).
	-Si dicho bloque comienza en la posicion actual, lo codificamos (insertando un
		StartCodeC/ToCodeC/nada según competa), actualizamos la posición actual y seguimos.
  -Si el flag C es false, o es TRUE y no hay bloque C, o es TRUE pero el siguiente bloque
	C empieza más allá del caracter actual, debemos codificar un bloque alfanumerico en 
	A o B. La longitud será:
		-Si el flag C es false, el resto de la cadena.
		-Si es TRUE, pero no se detecta bloque C, idem, el resto de la cadena.
		-Si es TRUE y hay bloque C, el trozo comprendido entre la posicion actual y el 
			comienzo del bloque C.
	-Se codifica insertando un StartCodeA/B, ToCodeA/B, o nada según competa, teniendo en 
	  cuenta que siempre se intenta usar primero el DefaultMode.
	-Cuando precisemos pasar desde A a B o viceversa porque algun char no sea representable 
	  más que en el otro juego, usaremos ToCodeA(o B)-char-char-... si hay más de un 
	  caracter seguido discordante, o un Shift-char si solo hay uno. 

  -En este modo, no es posible insertar caracteres de control barcode 128 tales 
      como FNC1, etc.

-Con formato. A través de la cadena de formato suministrada, controlamos la forma de
  representación. Esto es util cuando tenemos formatos de salidas estrictos 
  impuestos por entidades ajenas (pe, USPS). Además, podemos insertar caracteres de 
  control barcode 128. Estos son los caracteres de formateo:

  Chars         Significado
  -----			------------------------------------------------------------------------
  A / B / C		Start Code A / B / C. Debe aparecer una sola vez al principio. Inicializa
					además el default mode (modo C deja el que hay).
  a / b / c		To code A / B / C . Puede aparecer entre medias del formato, se ignora 
					si se pasa desde un formato a él mismo. Cambia el default mode 
					(modo C deja el que hay).
  F / f			To code 'free'. Activa / desactiva el modo de formato libre. En este modo
					algoritmicamente se decide la representación de los caracteres. 
  Z / z			Pone a TRUE / FALSE el flag 'Fill Zero' del algoritmo free.
  W / w			Pone a TRUE / FALSE el flag 'Allow C' del algoritmo free.
  s				Shift. En modo A, se consumira un solo char del flujo de entrada que se 
					codificara en modo B. Si esta en modo B, es lo mismo respecto al A.
					En modo C provocara un error.
  n				n es un numero  entero. Significa que se consuman y codifiquen los 
					siguientes n caracteres del flujo de entrada. Se codificaran segun
					el ultimo estado en que estemos (a/b/c/f). 
					-En modo f , se actuará como se indica en el apartado 'formato libre',
						pero limitando su acción al trozo de cadena comprendido por los 
						n caracteres especificados.
					-En el modo C todos los n caracteres a codificar deben ser digitos
						o se dará error,
					-En los modos A o B, si es necesario cambiar al otro modo A o B para 
						representar algun caracter, esto se hará como en el formato libre
						(con ToCode o Shift).
  *				Como n, pero sin limite, consume el resto de caracteres del flujo.
  o				Inserta un FNC1. Valido en charset A/B/C
  t				Inserta un FNC2. Valido en A/B. En modo C dará error.
  h				Inserta un FNC3. Idem.
  r				Inserta un FNC4. Idem.
  
*Notas:
	-Si tras procesar el formato quedan caracteres sin consumir, se lanzara un error.
	-Si al dar formato, se quieren consumir caracteres, y no hay, se lanzara un error.


Por ejemplo, si nos viene el dato :
	EA123456784US  --y queremos--> StartB-FNC1-EA1-ToC-(23)(45)(67)(84)-ToB-US
La mascara a usar seria  Bo3c8b2
	B inserta un Start code B y pasa a codificacion b
	o inserta el caracter FNC1
	3 consume 3 chars de datos (EA1) y los codifica en modo B (el ultimo).
	c inserta un To code C y pasa a modo de codificacion C.
	8 consume los siguientes 8 chars (2345678) y los codifica en el ultimo modo (c),
		el cual inserta 4 chars, los correspondientes a 23.45.67.84
	b inserta un To Code B t pasa a codificacion B.
	2 consume los siguientes 2 chars (US) y los codifica en modo b.

*/

class ClENG_BarCode128 : public ClENG_BarCodeField {

MIT_PUBLIC:
	
	enum  DEFAULT_MODE { DEFAULT_A, DEFAULT_B }  ;

MIT_PRIVATE:

	ClMIT_Str m_oFormat ;
	DEFAULT_MODE m_DefaultMode ;
	m4bool_t m_bSwAllowC ;
	m4bool_t m_bSwFillZeroC ;

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_BarCode128() ;
	virtual ~ClENG_BarCode128() { ClENG_BarCode128::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_BarCode128) ; }

	void Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height, const m4char_t *ai_pcExpr, 
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,
		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
		m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, 
		m4bool_t ai_bSwClip,
		StMIT_NULLDisplayDef *ai_poNULLDef,
		m4char_t *ai_pcIdPi, 
		
		m4char_t *ai_pc128Format="", 
		DEFAULT_MODE ai_DefaultMode=DEFAULT_B, m4bool_t ai_bSwAllowC=M4_TRUE,  
		m4bool_t ai_bSwFillZeroC=M4_TRUE ) ;
	
	virtual void End() ;

MIT_PUBLIC:

	//-------------------------Get/Set--------------------------------------------------

	void SetFormat(const m4char_t *ai_pcFormat) { m_oFormat.StrCpy(ai_pcFormat) ; }
	void SetDefaultMode(DEFAULT_MODE ai_DefaultMode) {  m_DefaultMode = ai_DefaultMode; }
	void SetAllowC(m4bool_t ai_bSw) { m_bSwAllowC=ai_bSw; } 
	void SetFillZeroC(m4bool_t ai_bSw) { m_bSwFillZeroC=ai_bSw; } 

MIT_PROTECTED:

	//-------------------------Virtuales------------------------------------------------

	virtual void Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) ;


MIT_PRIVATE:
	
	//--------->Datos y metodos auxiliares del algoritmo

	enum CURRENT_CHARSET { CHARSET_A, CHARSET_B, CHARSET_C, CHARSET_NO_INIT }  ;
	
	struct RT_Data {
		ClFONT_M_Font *m_poFont ;
		DEFAULT_MODE m_DefaultMode ;
		CURRENT_CHARSET m_CCh ; //Current charset
		m4bool_t m_bSwAllowC ;
		m4bool_t m_bSwFillZeroC ;
		m4bool_t m_bSwFreMode ;
		const m4char_t *m_InputPos ; 
		ClMIT_Str m_Out ;
	} ;

	//Si ai_NChar<0, el resto de la cadena.
	static void EncodeFreeMode(RT_Data *ai_RT, size_t ai_NChar) ;

	void EncodeFormatMode(RT_Data *ai_RT, const m4char_t *ai_pcFormat) ;

	//Cambia el m_CurrentCharSet al pasado (no se debe pasar CHARSET_NO_INIT).
	//Si el charset no coincide con el que habia, concatena en m_Out un
	//	Start o ToCode dependiendo de  si el estado era CHARSET_NO_INIT u otro.

	static void ChangeToCharSet(RT_Data *ai_RT, CURRENT_CHARSET ai_NewCharSet) ;

	//Inserta en m_Out los siguientes ai_NChar caracteres de m_InputPos, codificados en 
	//	modo C. Actualiza m_InputPos para apuntar al siguiente a leer.
	//Como siempre, si ai_NChar es <0, es hasta el final de la cadena.
	//Si no esta en modo C, cambiara a el.
	//Todos estos caracteres ya deben de ser numericos, cualquier control de este tipo
	//	se debe de haber pasado.
	//Tiene en cuenta el modo FillZero, si ai_NChar es impar mete un 0 de relleno,
	//	(solo puede ser impar si FillZero es TRUE, de otra forma sera par, esto ya debe 
	//	haberse procesado).

	static void EncodeC(RT_Data *ai_RT, size_t ai_NChar) ;

	//Devuelve cuantos caracteres hay seguidos en ai_pcStr (hasta un maximo de ai_NChar,
	//	o hasta el fin de cadena si es <0) codificables directamente en el charset A.
	// Si ai_bExclusive es true, además da los codificables exclusivamente en A y no en B
	static size_t IsAEncodable(const m4char_t *ai_pcStr, m4int32_t ai_NChar, m4bool_t ai_bExclusive ) ;

	//Como el anterior, para el charset B.
	// Si ai_bExclusive es true, además da los codificables exclusivamente en B y no en A
	static size_t IsBEncodable(const m4char_t *ai_pcStr, m4int32_t ai_NChar, m4bool_t ai_bExclusive ) ;

	//Simplemente es un IsAEncodable o IsBEncodable en funcion de ai_CharSet
	static size_t IsABEncodable(
		CURRENT_CHARSET ai_CharSet,
		const m4char_t *ai_pcStr, m4int32_t ai_NChar, m4bool_t ai_bExclusive ) ;


	//Localiza bloques codificables con charset C. Para ello, explora la cadena ai_pcStr
	// (hasta un maximo de ai_NChar,  hasta el fin de cadena si es <0), siendo un bloque 
	//	C aquel compuesto de al menos 4 digitos seguidos.
	//Devuelve la longitud del primer bloque encontrado (0 si no hay), y en ao_BeginIndex
	//	el indice de comienzo en ai_pcStr de dicho bloque.
	static size_t NextCEncodableBlock(const m4char_t *ai_pcStr, m4int32_t ai_NChar,
		m4int32_t *ao_BeginIndex) ;

	//Inserta en m_Out los siguientes ai_NChar caracteres de m_InputPos, codificados en 
	//	modo A ó B. Actualiza m_InputPos para apuntar al siguiente a leer.
	//Como siempre, si ai_NChar es <0, es hasta el final de la cadena.
	//Puede ir cambiando entre ambos modos, pero el que tiene preferencia es el default 
	//	mode.
	//Si algun caracter no es codificable en A o B lanza un error local y copiara el mismo
	//	char sin transformar.
	//Solo si es preciso, se cambia al modo no preferente, bien con un ToCodeA o ToCodeB
	//	segun competa, si hay mas de un caracter SOLO representable en el modo no 
	//	preferente, o bien con un shift si solo hay un caracter.

	static void EncodeAB(RT_Data *ai_RT, size_t ai_NChar) ;

	//Simplemente es un EncodeAAux o EncodeBAux en funcion de ai_CharSet
	static void EncodeABAux(CURRENT_CHARSET ai_CharSet,RT_Data *ai_RT, size_t ai_NChar) ;

	//Inserta en m_Out los siguientes ai_NChar caracteres de m_InputPos, codificados en 
	//	modo A. Actualiza m_InputPos para apuntar al siguiente a leer.
	//No verifica nda ni inserta codigos de cambio, ya debe de estar hecho.
	static void EncodeAAux(RT_Data *ai_RT, size_t ai_NChar) ;

	//Idem para modo B
	static void EncodeBAux(RT_Data *ai_RT, size_t ai_NChar) ;
} ;

//===================================================================ClENG_PostNet
/*
 PostNet 

  Codigo de barras exclusivamente numerico. Normalmente la longitud es fija, aunque 
	este field permitira fija o variable.
  Esta compuesto de  un StartFrameBar-digits-CheckDigit-FinalFrameBar

*/

class ClENG_PostNet : public ClENG_BarCodeField {

MIT_PRIVATE:

	//Longitud fija o sin limite si es <0

	m4int16_t m_FixedLength ;	

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_PostNet();
	virtual ~ClENG_PostNet() { ClENG_PostNet::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_PostNet) ; }

	//Usar el Init del ClENG_BarCodeField
	
	virtual void End() { ClENG_BarCodeField::End(); }

	void Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height, const m4char_t *ai_pcExpr, 
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,
		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
		m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, 
		m4bool_t ai_bSwClip, StMIT_NULLDisplayDef *ai_poNULLDef, m4char_t *ai_pcIdPi, 
	
		m4int16_t ai_FixedLength
		) ;
	


MIT_PROTECTED:

	//-------------------------Virtuales------------------------------------------------

	//Si la longitud es fija y ai_pcInValue tiene una longitud mayor, dara error.

	virtual void Encode( const char *ai_pcInValue, ClMIT_Str *ao_CodedValue) ;
} ;


#endif
