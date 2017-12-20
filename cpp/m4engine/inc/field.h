//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             field.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/18/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Manuel Lazcano (Gestion de PI - Octubre/98)
//
// Definition:
//
//  Este modulo define todas las clases necesarias para manejar campos en el layout.
//  El diseño está completamente abierto, lo cual significa que en cualquier momento 
//podremos crear nuevos tipos de campos, sin que tenga esto gran repercusión en los 
//otros modulos.
//   Es muy importante optimizar estas clases, tanto en tamaño como en tiempo de ejecucion,
//porque va a haber muchas instancias de ellas que se van a jecutar muchas veces.
//
//==============================================================================

#ifndef __FIELD__H__
#define __FIELD__H__

#include "clstr.h"

#include "m4clflag.h"
#include "cldevice.h"
#include "font.h"
#include "xengval.h"
#include "clformat.h"
#include "analex.h"
#include "region.h"

struct StENG_TaskEnv ;

class ClENG_FieldsManager; //Gestor de PI
class ClENG_DBFormat ;

//=============================================================================StENG_FieldEnv

//Auxiliar, environment de ejecución de un Field
//TODO son simples links.
//Los objetos apuntados deben de vivir al manos lo que vivan los Fields.

struct StENG_FieldEnv {
	ClENG_FieldsManager *m_poFieldsManager ;
	ClENG_OutDevice		*m_poDvc ;		
	ClENG_DBFormat		*m_poDBFormat ;

	StENG_FieldEnv() { StENG_FieldEnv::End() ; }  
	virtual void End() ; //Preventivamente, pone todo a NULL
} ;


//=================================================================================ClENG_Field

class ClENG_Field  
{
	//-------------------------Miembros-------------------------------------------------

MIT_PROTECTED:

	//>>>>>>>>>>>>>>Generales

	StMIT_Inspector* m_poInsp;	//Simple link
	StENG_FieldEnv  *m_poFieldEnv ; //Simple link
	ClMIT_Str m_oDesc ;
	ClMIT_IdNumDesc m_oLocation ;	//Solo sirve para trace location. Lo pone el init, y no cambia
	ClENG_FieldExternProp *m_poExternProp ;	//Relacion de uso !PUEDE SER NULL si NO HAY!

	//<<<<<<<<<<<<<<

MIT_PUBLIC:

	//>>>>>>>>>>>>>>Estados posibles del objeto
	enum STATE {
		NOT_INITIALIZED,
		INITIALIZED,
		START_PRINTING, 
		PARTIAL_PRINTED,
		PRINTED
	} ;

MIT_PRIVATE:

	STATE m_State ;

	//<<<<<<<<<<<<<<

	//>>>>>>>>>>>>>>Flags propiedades de campo

	enum FLAGS { 
		STATIC=ClMIT_Flag::BIT0,	//Tipo de campo, estatico/variable
		OPTIMIZE=ClMIT_Flag::BIT1,	//Optimize; intentar meterlo en buffer de device

		HAS_PI = ClMIT_Flag::BIT2,	//Tiene PI asociada

		VARIABLE_HEIGHT = ClMIT_Flag::BIT3,	//El field es de altura variable

		PARTIAL_PRINT = ClMIT_Flag::BIT4,   //El field admite impresiones parciales

		// Si esta activo, n impresiones SUCESIVAS de un mismo valor, solo deben provocar
		//	una única impresión. 

		DUPLICATED=ClMIT_Flag::BIT12,

		//Auxiliar para duplicados, para controlar la primera impresion. Se pone a TRUE en 
		//	el constructor, y a FALSE en el Print.

		DUPLICATE_RESET=ClMIT_Flag::BIT13

	} ;
	ClMIT_Flag m_oFlag ;	

	//<<<<<<<<<<<<<<

	//>>>>>>>>>>>>>>Area del field

	DvcUnit_t m_Top, m_Left, m_Width, m_Height ;	

	//m_Height PUEDE recalcularse en PostLoadValue, m_HeightBackup es el valor original de
	//	diseño a ser reseteado en cada impresión.

	DvcUnit_t m_HeightBackup ;

	//Auxiliar de proceso para impresion de la Region.
	DvcUnit_t m_HeightPrinted ;

	//<<<<<<<<<<<<<<

	//>>>>>>>>>>>>>>Región del field

	// El Init lo resetea y el Print lo muestra.

	ClENG_Region m_oRegion ;

	//<<<<<<<<<<<<<<

MIT_PROTECTED:

	//>>>>>>>>>>>>>>Expresiones de carga & valores de carga

	ClENG_Expression m_oExpr ;	//Expresion usada para cargar m_oValue ;
	ClMIT_ChnValue m_oValue, m_oPrevValue ;	//Valor actual y anterior (para DUPLICATE)

	//<<<<<<<<<<<<<<

	//>>>>>>>>>>>>>>Indices

MIT_PUBLIC:

	//Propiedades para la indexacion
	enum { NO_INDEX_ENTRY=-1, ROOT_ENTRY=0 } ; //Valor que inhabilita la indexacion para el Field

MIT_PROTECTED:

	m4int32_t m_iIndexLevel;      //Nivel de profundidad del indice
	ClENG_Expression m_oIndexExpr;  //Expresion para la generacion de indices
	ClMIT_Str m_oIndexStr;		//Valor formateado resultante de la IndexExpr

	//<<<<<<<<<<<<<<
	
	//>>>>>>>>>>>>>>Hyperlinks

	ClMIT_Str m_oHyperlinkStr;
	ClENG_Expression m_oHyperlink;

	//<<<<<<<<<<<<<<

	//>>>>>>>>>>>>>>Varios

	ClMIT_Str m_oIdPI ;	
	ClMIT_ChnItem m_oSave ;	//Item para SetSave

	//<<<<<<<<<<<<<<

MIT_PUBLIC:

	//-------------------------Inicializaciones-----------------------------------------

	ClENG_Field() ; 
	virtual ~ClENG_Field() { ClENG_Field::End(); } 
	virtual size_t GetSizeof() { return sizeof(ClENG_Field) ; }
	virtual void End() ;

	// -Optimize es un flag solo valido si SwStatic. Indica que este campo se va a mostrar mas
	//	de una vez, y que se desea optimizar el acceso. Esta optimizacion es una 
	//	caracteristica opcional, que implementaran clases superiores que hagan uso de fields.
	// -Pone SetSave a nulo por defecto.
	// -Se nos dice que el field puede manejar Pi (recordar que por ahora las Pi solo se 
	//   gestionan a nivel de ClENG_FieldStr, y el Identificador de Pi si procede. 
	//   Si ai_pcIdPI es NULL o una cadena vacia, no hay PI.
	
	void Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation, 
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height,  const m4char_t *ai_pcExpr, 
		const m4char_t *ai_pcIndexExpr, m4int32_t ai_iIndexLevel,
		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		m4bool_t ai_bSwVariableHeight, m4bool_t ai_bPartialPrint,
		m4char_t *ai_pcIdPI);

	// Necesario antes de usar el objeto. Pre-analiza la expresion. 
	// Si el campo es estatico, llama a LoadValue.
	// Puede llamarse varias veces.
	// Resetea las ExternProp de la expresion, y del field
	// Da true si todo va bien. Si hay problemas, da false y genera errores locales.

	m4bool_t PreAnalex( ClENG_Analex::StExpressionEnv *ai_pstEnv ) ;

	// Determina si el dibujado del campo está condicionado al tamaño final del contendor
	virtual m4bool_t IsAdjustedToContainer() { return M4_FALSE; }

MIT_PUBLIC:

	//-------------------------Gets---------------------------------------------------------

	DvcUnit_t GetTop() { return m_Top ; }
	DvcUnit_t GetLeft() { return m_Left ; }
	DvcUnit_t GetWidth() { return m_Width ; }
	DvcUnit_t GetHeight() { return m_Height ; }
	m4bool_t IsStatic() { return m_oFlag.True(STATIC) ; }
	m4bool_t IsOptimize() { return m_oFlag.True(OPTIMIZE) ; }
	m4bool_t IsPi() { 	return m_oFlag.True(HAS_PI) ; } 
	m4bool_t IsDuplicated() { return m_oFlag.True(DUPLICATED) ; }
	m4bool_t IsVariableHeight() {return m_oFlag.True(VARIABLE_HEIGHT) ; }
	m4bool_t IsPartialPrintAllowed() {return m_oFlag.True(PARTIAL_PRINT) ; }
	STATE GetState() { return m_State ; }


	ClMIT_IdNumDesc & GetIdDesc() { return m_oLocation ; }	//Solo para uso inmmediato

	//-------------------------Sets---------------------------------------------------------

	// Si se pasa el puntero NULL, o el item pasado no esta linkado, se desactiva la 
	//	caracteristica de Save (ver Flush).
	// El item es copiado.

	void SetSave( ClMIT_ChnItem *ai_poSave=NULL ) ;

	// Resetea el link a una ExternProp.

	void ResetExternProp( ClENG_FieldExternProp * ai_poExternProp ) ;

MIT_PROTECTED:
	void SetTop(DvcUnit_t ai_Top) { m_Top = ai_Top ; }
	void SetHeight(DvcUnit_t ai_Height) { m_Height = ai_Height ; }

MIT_PUBLIC:

	//-------------------------Manejo de campos, una vez inicializados------------------

	//A llamar antes de cada impresión. Limpia flags auxiliares, etc.
	//	-Si !IsStatic() llama a LoadValue(), si no ya debe estar el valor cargado.
	//	-Llama a NewPrintCycle para dar oportunidad a las derivadas para limpiar sus flags.

	void NewPrint() ;

	//	-Solo opera si se esta en estado START_PRINITNG o PARTIAL_PRINTED.
	//	-XBase, YBase e ai_PrintInterval son la ventana fisica de device de impresion que 
	//		impone el container. 
	//		El X,Y e Interval correspondiente a computos de Field con calculados antes de 
	//		enviarselos al  FlushValue, esta ya opera dentro la ventana fisica de device, pero
	//		dentro del ventana fisica del Field, NO del container.
	//  -ai_HeightCntPrinted es lo ya impreso en el container (siempre 0 para containers con
	//		Together, y para la primera impresion parcial). Necesario para calculo.
	//	-Se llama a FlushValue() siempre que se pase el filtro de duplicados. Antes imprime 
	//		tambien la Region.
	//		El area real de impresión para el FlushValue (ventana de Field) se haya así:
	//		    - XField=XBase+Left
	//			- 1 impresion parcial o impresion total
	//				- YField=YBase+Top-ai_HeightCntPrinted  
	//				- FieldInterval = InterVal - (Top-ai_HeightCntPrinted)
	//				Si FieldInterval<=0, no HACER nada ni cambiar de estado, el field no se
	//					puede imprimir en la ventana de container indicada.
	//			- Resto impresiones parciales
	//				- YField = YBase
	//				- FieldInterval = Interval 
	//					No hay test porque si ya hubo una primera impresion parcial, la 
	//					siguiente toca seguro.
	//	    ai_PrintInterval es el alto absoluto máximo para imprimir. Este dato es util para
	//			campos con impresión parcial. Es responsabilidad de estos campos hacer caso de
	//			este valor. Los que no lo admiten pueden obviarlo e imprimir todo.
	//	-Se llama a Save() SOLO si no hay PI. Si la hay, el Save se hara al resolver la PI.
	//	-Según se haya impreso todo o no, se pasa a estado PARTIAL_PRINTED o PRINTED.

	void Print( m4int16_t ai_iNumPage, DvcUnit_t ai_XBase, DvcUnit_t ai_YBase, DvcUnit_t ai_Interval, 
		DvcUnit_t ai_HeightCntPrinted) ;

	// Sólo actua en los campos (p.ej. Líneas) que tienen definida la propiedad de ajustar su tamaño al
	// contenedor
	virtual void EvaluateContainerHeight( DvcUnit_t ai_DesignHeight, DvcUnit_t ai_RealHeight) {} ;

	// Se encarga de formatear la expression que tiene asociada y darselo al device
	// device para que lo resuelva (escriba en la posicion adecuada)
	// Si hay Save, debe de hacerlo.

	virtual void ResolvePi(ClMIT_ChnValue *ai_poValue){};

	//--------------------Auxiliares de proceso---------------------------------------------

MIT_PUBLIC:

	//-----------Sobrecargas para usar el objeto con STL
	m4bool_t operator == (const ClENG_Field &ai_oOpe) const
		{ return M4_BOOL(m_Width==ai_oOpe.m_Width); }
	m4bool_t operator < (const ClENG_Field &ai_oOpe) const
		{ return M4_BOOL(m_Width<ai_oOpe.m_Width); }


MIT_PRIVATE:

	// Solo da true si es preciso hacer un control de duplicados. Esto sucede si
	//	el DUPLICATE de esta clase es TRUE, y el DUPLICATE_RESET false, y además, 
	//	en caso de haber m_poExternProp, si el switch de RESET_DUPLICATE es false.
	// El DUPLICATE_RESET de esta clase garantiza que si este metodo da TRUE, ya se ha
	//	llamado a Print al menos una vez (y que, eventualmente, hay un valor anterior con el
	//	que comparar).

	m4bool_t IsDuplicatedControl() ;

MIT_PROTECTED:

	//Se llama siempre que es necesario cargar m_oValue (una vez para campos estaticos, una
	//	por impresion para los que no lo son).
	//Si ai_poValue es NULL, ejecuta la expresion del Field y almacena el resultado en 
	//	m_oValue. Si no es NULL, simplemente copia el valor pasado a m_oValue. En cualquier 
	//	caso, siempre el valor a procesar queda en m_oValue.
	//Tras esto, llama a PostLoadValue (metodo virtual, ideal para formatear datos, hacer
	//	precalculos necesarios para la impresion, etc. Util hacerlo en este momento por que
	//	asi, un campo estatico solo calcula estas cosas una vez, aunque haya n FlushValue). 
	//Para campos de altura variable, recalcula alturas.
	//PostLoadValue puede usar sin problemas m_oValue.

	void LoadValue(ClMIT_ChnValue *ai_poValue=NULL) ;


MIT_PROTECTED:

	//-------------------------Fuciones virtuales  a implementar por derivadas----------

	//VIRTUAL SUSTITUIBLE
	// Se llama siempre dentro del NewPrint, con el dato cargado. 
	// Se pasa a estado START_PRINTIG, con lo que el Print funcionará de nuevo.

	virtual void NewPrintCycle() { return ; }
	
	// VIRTUAL PURA
	// Llamada siempre en LoadValue, tras cargar m_oValue. 
	// Util para preparar y depositar el dato a imprimir en un miembro auxiliar.
	// Si el campo es IsVariableHeight() debe de devolverse obligatoriamente la altura
	//	computada para esta ocurrencia de m_oValue (que puede perfectamente superar la 
	//	altura de un container y de una hoja). Devuelva lo que se devuelva, la altura nominal 
	//	de diseño será la minima. En caso de que no sea IsVariableHeight(), dá igual lo que 
	//	devuelva porque se ignora.
	// El ajuste e impresión de la Region se lleva a cabo en función de este valor.

	virtual DvcUnit_t PostLoadValue()=0 ;

	// VIRTUAL PURA
	//  -ai_X, ai_Y y ai_PrintInterval son coordenadas absolutas de device (ver FlushValue),
	//		marcan la ventana fisica de impresion.
	//	-Campos que no admitan impresiones parciales pueden imprimir directamente en 
	//		ai_X e ai_Y e ignorar ai_PrintInterval.
	//	-Los campos que admitan impresiones parciales deben de imprimir directamente en 
	//		ai_X e ai_Y, pero a partir de los datos que tengan aun pendiente de imprimir.
	//		También deben de imprimir solo lo que quepa en ai_PrintInterval.
	//  -Debe devolverse true si se imprimio todo, es decir, no queda nada pendiente 
	//		(campos que no admitan impresiones parciales siempre devuelven true), o false si
	//		aún quedan cosas por imprimir.

	virtual m4bool_t FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
		DvcUnit_t ai_PrintInterval )=0 ;


MIT_PUBLIC:

	//VIRTUAL SUSTITUIBLE
	// Util para fields con la capacidad de partirse.
	// Permiten REDUCIR el intervalo a imprimir, para justarlo, por ejemplo a un multiplo 
	//	exacto de lo que ocupa una linea.
	// Puede devolver 0 para impedir imprimir.
	
	virtual DvcUnit_t AdjustCntInterval( DvcUnit_t ai_PrintInterval) { return ai_PrintInterval ; }

};


//==============================================================================ClENG_FieldStr

class ClENG_FieldStr : public ClENG_Field {

MIT_PUBLIC:
	enum ALIGN { CENTER, LEFT, RIGHT, POINT } ;
	enum FLAGS { 
		CLIP=ClMIT_Flag::BIT0,
		FORMAT_NULL=ClMIT_Flag::BIT1	//En caso de NULL, dice si se desea formatear la cadena
	} ;

	enum FILL_MODE { 
		NO_FILL,	//Nunca usa caracter de relleno
		ALWAYS_FILL,	//Siempre rellena
		NO_EMPTY_FILL	//Solo rellena con cadenas no vacias
	} ;

MIT_PRIVATE:

	ALIGN m_AlignX, m_AlignY;
	DvcUnit_t m_OffsetX, m_OffsetY ;
	ClMIT_Flag     m_oFlag ;
	ClENG_Format  *m_poFormat ;	//NULO si no hay
	ClFONT_M_Font *m_poFont ;

	FILL_MODE m_FillMode ;
	m4char_t m_cFillChar ;	//Caracter de relleno, si no hay

							//Ver virtuales
	ClMIT_Str m_oSourceStr, m_oFormatedStr ;	
	size_t m_szIndStr, m_szNChar;

	m4uint32_t m_uiIdFGColour ;	//IdColour foreground

							//Para tratar impresion de NULLs 
	StMIT_NULLDisplayDef    m_oNULLDef, m_oNULLFormatDef ;


	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_FieldStr();
	virtual ~ClENG_FieldStr() { ClENG_FieldStr::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_FieldStr) ; }

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
		ALIGN ai_AlignX, ALIGN ai_AlignY, 
		m4uint32_t ai_uiIdFGColour, 
		ClFONT_M_Font * ai_poFont, ClENG_Format *ai_poFormat, m4bool_t ai_bSwClip,
		FILL_MODE ai_FillMode, m4char_t ai_cFillChar,

		StMIT_NULLDisplayDef *ai_poNULLDef=NULL, StMIT_NULLDisplayDef * ai_poNULLFormatDef=NULL, 
		m4bool_t ai_bSwFormatNULL=M4_FALSE,

		m4char_t *ai_pcIdPi=NULL );
    

	virtual void End() ;

	//-------------------------Gets-----------------------------------------------------

	m4bool_t IsClip() {return m_oFlag.True(CLIP);}
	m4char_t GetFillChar() { return m_cFillChar ; }
	FILL_MODE GetFillMode() { return m_FillMode ; }
	m4bool_t IsFormatNULL() { return m_oFlag.True(FORMAT_NULL);}
	m4uint32_t GetIdFGColour() { return m_uiIdFGColour ; } 

	//-------------------------Fuciones virtuales  a implementar por derivadas----------

MIT_PROTECTED:

	//Actualiza siempre m_szIndStr (desde que char imprimir), m_szNChar (cuantos), y deja
	//	la cadena a imprimir en m_oFormatedStr.
	//Formatea la cadena y aplica relleno. Se usa SourceStr y m_oFormatedStr para aplicar 
	//	el formateo. Tambien actualiza OffsetX e OffsetY.

	virtual DvcUnit_t PostLoadValue() ;

	virtual m4bool_t FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) ;

	virtual void ResolvePi(ClMIT_ChnValue *ai_poValue);
	
} ;

//==============================================================================ClENG_FieldStrBox

//Clase casi igual a la anterior, solo que saca cadenas dentro de casillas de ancho fijo.

class ClENG_FieldStrBox : public ClENG_Field {

MIT_PRIVATE:
	ClENG_FieldStr::ALIGN m_AlignX;
	ClENG_Format  *m_poFormat ;	//NULO si no hay
	ClFONT_M_Font *m_poFont ;
	DvcUnit_t	   m_SizeBox ;
	size_t		   m_szNBoxes ;

	ClENG_FieldStr::FILL_MODE m_FillMode ;
	m4char_t m_cFillChar ;	//Carater de relleno, si hay

	m4uint32_t m_uiIdFGColour ;	//IdColour foreground

	// Almacenan la cadena, cuantas Box a saltar, siempre se imprime desde el caracter 0, y 
	//	se indica cuantos caracteres se imprimen desde ahi (siempre recorta si se sobrepasa).

	ClMIT_Str m_oSourceStr, m_oFormatedStr ;	
	size_t m_szSkipBox, m_szNChar;

	m4bool_t m_bSwFormatNULL ; //En caso de NULL, dice si se desea formatear la cadena
							//Para tratar impresion de NULLs 
	StMIT_NULLDisplayDef    m_oNULLDef, m_oNULLFormatDef ;

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_FieldStrBox();
	virtual ~ClENG_FieldStrBox() { ClENG_FieldStrBox::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_FieldStrBox) ; }

MIT_PUBLIC:

	// Si la anchura es negativa, se interpreta como numero de caracteres, y la anchura
	//   de calcula en funcion de la fuente. Se garantiza que siempre caben esos 
	//   caracteres, aunque la fuente sea proporcional.
	// Si el top o left son negativos, tambien se calculan en funcion del alto o ancho de 
	//	la fuente.
	// Si ai_poFormat=NULL, no se formateara.

	void Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, const m4char_t *ai_pcExpr,
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel, 
		m4char_t* ai_pcHyperlink, size_t ai_szNBoxes,
		m4bool_t ai_bSwStatic, m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,

		ClENG_FieldStr::ALIGN ai_AlignX, 
		m4uint32_t ai_uiIdFGColour, ClFONT_M_Font * ai_poFont, ClENG_Format *ai_poFormat,
		ClENG_FieldStr::FILL_MODE ai_FillMode, m4char_t ai_cFillChar,

		StMIT_NULLDisplayDef *ai_poNULLDef=NULL, StMIT_NULLDisplayDef * ai_poNULLFormatDef=NULL, 
		m4bool_t ai_bSwFormatNULL=M4_FALSE
	);

	virtual void End() ;

	//-------------------------Gets-----------------------------------------------------

	m4char_t GetFillChar() { return m_cFillChar ; }
	ClENG_FieldStr::FILL_MODE GetFillMode() { return m_FillMode ; }
	m4bool_t IsFormatNULL() { return m_bSwFormatNULL; }
	m4uint32_t GetIdFGColour() { return m_uiIdFGColour ; } 

	//-------------------------Funciones virtuales  a implementar por derivadas----------

MIT_PROTECTED:

	virtual DvcUnit_t PostLoadValue() ;

	virtual m4bool_t FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) ;

} ;

//============================================================================ClENG_FieldMultiL

class ClENG_FieldMultiL : public ClENG_Field {

MIT_PUBLIC:

	enum FLAGS { 

		//Solo util para multilineas de altura fija, no variable. Dice, en caso de sobrepasar el
		//	alto asignado al field, si imprimir y machacar o no imprimir.
		CLIP=ClMIT_Flag::BIT0,
		
		//Si true, al imprimir una linea se distribuye entre los balncos el espacio sobrante.

		JUSTIFIED=ClMIT_Flag::BIT1
	} ;

MIT_PRIVATE:

	ClMIT_Flag m_oFlag ;

	//IdColour foreground
	m4uint32_t m_uiIdFGColour ;	

	//Font del field,su altura+interline son el alto por linea.
	ClFONT_M_Font *m_poFont ;


	//Espacio extra a dejar al final de una linea
	DvcUnit_t m_InterLine ;

	//Siguiente posicion (indice) de la cadena a imprimir.
	m4uint32_t m_CurrentPos ;

	//Altura del field en lineas, util para test de Clip en multilineas de alto fijo.
	//Contiene el maximo numero de lineas que caben; si caben 3'5, caben 3.
	m4uint32_t m_HeightInLines ;

	//Auxiliar para proceso clip, lineas impresas
	m4uint32_t m_LinesPrinted ;

	//Altura de una sola linea
	DvcUnit_t m_LineHeight ;

	//Altura de todas las líneas a imprimir.
	DvcUnit_t m_iTotalLineHeight;

	// Alineamiento.
	ClENG_FieldStr::ALIGN	m_AlignX;
	ClENG_FieldStr::ALIGN	m_AlignY;
	DvcUnit_t				m_OffsetX;
	DvcUnit_t				m_OffsetY;
	

MIT_PROTECTED:

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_FieldMultiL();
	virtual ~ClENG_FieldMultiL() { ClENG_FieldMultiL::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_FieldMultiL) ; }

	virtual void End() ;

	// Si se le pasa la altura negativa, se interpreta como máximo numero
	//   de lineas y ajusta la altura, según la fuente e interlineado, para que quepan.
	// Si la anchura es negativa, se interpreta como numero de caracteres, y la anchura
	//   de calcula en funcion de la fuente. Se garantiza que siempre caben esos 
	//   caracteres, aunque la fuente sea proporcional.
	// Si el top o left son negativos, tambien se calculan en funcion del alto o ancho de 
	//	la fuente (en este caso NO se usa el interlineado).

	void Init(	
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, 
		DvcUnit_t ai_Height, DvcUnit_t ai_InterLine, m4bool_t ai_bIsVarHeight, 
		m4bool_t ai_bIsJustified, const m4char_t *ai_pcStr, 
		const m4char_t *ai_pcIndexExpr, m4uint32_t ai_uiIndexLevel,	

		m4char_t* ai_pcHyperlink, m4bool_t ai_bSwStatic, 
		m4bool_t ai_bSwDuplicated, m4bool_t ai_bSwOptimize, 
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		ClENG_FieldStr::ALIGN ai_AlignX, ClENG_FieldStr::ALIGN ai_AlignY, 
		m4uint32_t ai_uiIdFGColour, 
		ClFONT_M_Font * ai_poFont, m4bool_t ai_bSwClip
	);


	//-------------------------Gets-----------------------------------------------------

	m4bool_t IsClip() {return m_oFlag.True(CLIP);}
	m4bool_t IsJustified() { return m_oFlag.True(JUSTIFIED); }
	m4uint32_t GetIdFGColour() { return m_uiIdFGColour ; } 
	
	//-------------------------Fuciones virtuales  a implementar por derivadas----------

MIT_PROTECTED:

	//Se testea que sea de tipo string. Si no lo es, error local.
	virtual DvcUnit_t PostLoadValue() ;

	//Se usa m_oValue directamente.
	//Se testea que sea de tipo string. Si no lo es, error local.

	virtual m4bool_t FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) ;

	virtual void NewPrintCycle() ;

MIT_PUBLIC:

	virtual DvcUnit_t AdjustCntInterval( DvcUnit_t ai_PrintInterval) ;
	
	//---------------------------- Fuciones auxiliares de proceso ----------------------------
MIT_PRIVATE:
	
	//Devuelve en cuantas lineas se imprimira el contenido actual de m_oValue. No tiene en 
	//  cuenta ninguna propiedad de Justify ni nada parecido, simplemente según el alto y ancho 
	//	actual computa el umero de lineas. 

	m4uint32_t PreCalculateLinesToPrint();
	
	// -ai_Pos. Posición de cadena desde la que escanear. 
	// -Devuelve la posición de la cadena para el próximo ExtractLine (es decir, 
	//		ai_Pos+caracteres consumidos). Apunta a fin de cadena si no hay más lineas.
	// -BegLine, posición comienzo desde la que imprimir (dentro de ai_Pos).
	// -ao_CharsToPrint, caracteres a imprimir (excluye posible Tag CR-LF final). 
	//		Este valor puede ser  0 por fin de cadena o tags CRLF que generen lineas vacias.
	//		Así pues, el rango de la  cadena expresado por BegLine-ao_CharsToPrin esta compuesto 
	//		o bien por nada, o por bloques de palabras y  bloques de separadores de palabras 
	//		que hay que imprimir en una linea.
	// -ao_SwCRLF. Flag que indica si la ruptura de linea se produce entre palabras (posible
	//		justificación), o bien por fin de cadena o tag CRLF (NO justificar).

	char * ExtractLine( char * ai_Pos, char **ao_BegLine, size_t *ao_CharsToPrint, m4bool_t *ao_SwCRLF ) ;

	void PrintJustified( 
		m4int16_t ai_iNumPage, DvcUnit_t ai_Y, DvcUnit_t ai_X, 
		char *ai_BegLine, size_t ai_CharsToPrint ) ;

	m4void_t SetAligment(char * ai_pLine, size_t ai_iCharNum);
} ;

//==============================================================================ClENG_FieldLine

//Tira una linea

class ClENG_FieldLine : public ClENG_Field {

MIT_PROTECTED:

	ClENG_Line m_oLineBackup ;
	DvcUnit_t m_TopBackup ;
	DvcUnit_t m_HeightBackup ;
	DvcUnit_t m_HeightPrinted ;

MIT_PUBLIC:

	ClENG_Line m_oLine ;	//Public para gets, NO para Init ó End

	//-------------------------Inicializaciones-----------------------------------------

MIT_PUBLIC:

	ClENG_FieldLine();
	virtual ~ClENG_FieldLine() { ClENG_FieldLine::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClENG_FieldLine) ; }

MIT_PUBLIC:

	//Practicamente, los mismos parametros que el Init de una linea.
	//El objeto es una region con una linea en el interior. Los LineTop, etc son relativos a la
	//	region delimitada por el field.
	//El flag de visible hace referencia al objeto Line (aunque daria igual si fuese el de Field)

	void Init( 
		StENG_FieldEnv * ai_poFieldEnv,
		ClMIT_IdNumDesc *ai_poLocation, 
		m4bool_t ai_bSwStatic, m4bool_t ai_bSwOptimize,
		m4char_t *ai_pcFrame, m4uint32_t ai_FrameFillColour,
		DvcUnit_t ai_Top, DvcUnit_t ai_Left, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
		DvcUnit_t ai_LineTop, DvcUnit_t  ai_LineLeft, DvcUnit_t ai_LineTop2, DvcUnit_t ai_LineLeft2,
		m4bool_t ai_bAdjustLineToContainer,
		DvcUnit_t ai_LineWidth, ClENG_OutDevice::STYLE ai_Style, m4uint32_t ai_uiIdColour, 
		m4bool_t ai_bSwVisible
	);

	virtual void End() ;

	virtual void NewPrintCycle() ;

	m4bool_t SetLine( m4char_t *ai_pcLineProps ) ;

	// Determina si el dibujado del campo está condicionado al tamaño final del contendor
	virtual m4bool_t IsAdjustedToContainer();

	// Sólo actua en las líneas que tienen definida la propiedad de ajustar su tamaño al contenedor.
	// Se invoca en el momento en el que ya está determinado el tamaño del contenedor
	virtual void EvaluateContainerHeight( DvcUnit_t ai_DesignHeight, DvcUnit_t ai_RealHeight) ;



	//-------------------------Funciones virtuales  a implementar por derivadas----------

MIT_PROTECTED:

	virtual DvcUnit_t PostLoadValue() ;

	virtual m4bool_t FlushValue( m4int16_t ai_iNumPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_PrintInterval ) ;
} ;


//========================================================================ClENG_FieldExternProp

/*
	Muchos field pueden compartir un único objeto de este tipo, por eso en las siguientes
clases solo hay punteros a este tipo de objeto, y no objetos.
	La relación siempre es de uso: los ClENG_Field nunca borran ni crean objetos de este tipo,
y el objeto ClENG_FieldExternProp debe vivir mientras lo hagan los ClENG_Field asociados.
	La gran ventaja es que aqui agrupamos comportamientos comunes a muchos objetos: si por
ejemplo hay 50 fields en los que queremos, de golpe, resetear el flag RESET_DUPLICATE, en
vez de tener que recorrerlos todos (y tener que mantener por ello una lista), basta con
tocar el flag una vez en el objeto ClENG_FieldExternProp.
	El link no se pone en el Init, si no posteriormente. Puede ser reinicializado a NULL, y
también reinicializado tantas veces como se quiera. El proceso Init no debe depender para
nada de ningun miembro de esta clase. Más bien está pensada para poner propiedades que
puedan modificar a comportamientos ya implementados en mienbros de ClENG_Field.

*/

class ClENG_FieldExternProp 
{
MIT_PRIVATE:
	enum FLAGS { 

		//Mientras está activo, en el objeto field nunca se hace un control de duplicate,
		//	lo que quiere decir que el field se saca, tenga o no esta capacidad activada.

		RESET_DUPLICATE=ClMIT_Flag::BIT0	
	} ;

	ClMIT_Flag m_oFlag ;

MIT_PUBLIC:
	
	ClENG_FieldExternProp() { ClENG_FieldExternProp::Init() ; }
	virtual ~ClENG_FieldExternProp() { ClENG_FieldExternProp::End(); } 
	virtual void End() {}
	void Init();	//Resetea RESET_DUPLICATE a false
	
	void SetResetDuplicate(m4bool_t ai_bSw) { m_oFlag.Switch(RESET_DUPLICATE, ai_bSw) ; }
	m4bool_t IsResetDuplicate() { return m_oFlag.True(RESET_DUPLICATE) ; }
} ;

//================================================================================ClENG_FieldsManager
// GESTOR DE PREIMPRESIONES (ClENG_FieldsManager)
//
//Clase gestora de fields, se tendrá un objeto de estos a nivel de report. 
//Se mantendra, por cada thread, una multilista a nivel de Field (esta), que almacenara las
// propiedades del Field designado como PI, y una copia del valor asociado al field, que se
// utilizara posteriormente para resolver la PI. Se mantendra otra lista a nivel de dispositivo
// de salida (ClENG_DvcFi), que guardará las propiedades relacionadas con la salida de datos (posi-
// cion, field en RAM o no, etc...). Al hacer un Show a nivel de field, si el field es de tipo
// PI se añade a la lista, luego, al hacer el FlushValue, se lanza a la salida un dummy-field,
// cuyas propiedades se añaden a la lista que hay a nivel de DvcFile, para ser posteriormente 
// resuelto y rellenado a la hora del ResolvePi. Esta secuencia lógica debe seguirse siempre,
// por lo que despues de declarar una o mas PI, debe haceerse una llamada a la expresion Print_Pi
// con el identificador de PI adecuado. 


class ClENG_FieldsManager
{

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PUBLIC:

	//Valor activo en cada llamada de ResolvePi. Se rellenara en cada iteracion del bucle 
	// de ResolvePi, para que la capa de Analex tenga acceso directo al valor instantaneo
	ClMIT_ChnValue m_oActiveValue ; 
	m4bool_t m_bValueActivated ; 

	void SwitchValueActivation( ) { m_bValueActivated=M4_BOOL(!m_bValueActivated); }
	m4bool_t IsValueActivated() { return m_bValueActivated ; }

MIT_PRIVATE:

	// La lista de Pi a nivel de field se guarda en un multimap
	// Como clave está el identificador de PI
	// Como valor se guarda una estructrura, cuyos campos son el puntero objeto field al 
	// que corresponde el PI y una copia del ItemValue del field para su posterior uso al
	// resolver.

	struct stFieldPi
	{
		ClENG_Field *pField;
		ClMIT_ChnValue ItValue;
	};


	//Definicion del multimap que almacenara las PIs.
	typedef multimap<ClMIT_Str,stFieldPi, less<ClMIT_Str> > PIList_t;
	PIList_t m_oPIList; 

	
MIT_PUBLIC:
	ClENG_FieldsManager();
	virtual ~ClENG_FieldsManager(){ClENG_FieldsManager::End();};
	virtual size_t GetSizeof() { return sizeof(ClENG_FieldsManager) ; }

	// Destruye la lista de Pi, dando error por cada Pi que no haya sido 
	// resuelta.
	virtual void End() ;

	
	void Init();

	//Añade el Pi a la lista con clave ai_iIdPI
	//  - Si ya existe otro no da error puesto que podemos tener varias entradas para una misma clave
	//  - Inserta el par Clave/Valor en la lista devolviendo M4_TRUE
	m4bool_t AddNewPi  (ClMIT_Str *ai_poIdPi,ClENG_Field * ai_opClField, const ClMIT_ChnValue* ai_poValue);

	//Sacar Pi de la lista y devolver el objeto Field asociado. 
	//Si el Pi no esta devuelve M4_FALSE, sino M4_TRUE 
	//La llamada se realiza mediante un bucle en el EvaluateThis del ClENG_TkPi, y se repite mientras
	// no devuelve error. Si devuelve error, se comprueba que no sea la primera llamada, en cuyo 
	// la Pi referida no existe.
	m4bool_t ExtractPi( ClMIT_Str *ai_poIdPi, ClENG_Field * &ao_opClField, ClMIT_ChnValue &ao_poValue);

};


#endif
