//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             font.h
// Project:			 fmetrics.dll
// Author:           Meta Software M.S. , S.A
// Date:			 17/07/1997
// Language:         C++
// Operating System: WINDOWS. 
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Contiene todo lo necesario para manejo de fuentes
//    Modulo temporal, debe hacerse multiplataforma.
//
//==============================================================================

#ifndef __FONT__H__
#define __FONT__H__

#include "m4fmetrics_dll.hpp"
#include "mit.h"
#include "m4stl.hpp"
#include "m4clflag.h"
#include "clmetric.h"
#include "clstr.h"
#include "softtrans.h"


//==============================================================================ClFONT_M_Font

/*

  Cuando se diseña un impreso, se desea que este diseño sea independiente del dispositivo,
y que valga para impresora, pantalla, etc.
  Sin embargo, hay fuentes que pueden o no representarse en segun que dispositivos, y que
además necesitan informacion dependiente de este.
  Se ha adoptado esta solución:

  -Es muy deseable controlar que fuentes pueden usarse, con el fin de permitir solo
    aquellas que se representen de manera más fiel en la mayoria de los dispositivos.
  -Un dispositivo utilizará la fuente más parecida a la que le hayamos pedido, siendo 
    responsabilidad suya el lograr esto.
  -Para ello, se creara fuera una base de datos con las fuentes permitidas. Esta base de 
    datos recoge toda la información común de las fuentes, asi como alguna otra 
	dependiente del dispositivo, que es la que se le pasa a esta clase.

*/

class M4_DECL_M4FMETRICS ClFONT_M_Font
{

	friend class ClRFN_RFN_R ;

MIT_PRIVATE:
	ClMIT_Str m_oDesc ;
	ClMIT_IdNumDesc m_oLocation ;
	
MIT_PROTECTED:
	StMIT_Inspector* m_poInsp;  

MIT_PUBLIC:

	enum {
		SIZE_WIDTH_ARRAY=256,
		PCL_SOFT_FONT_BEGIN=1000
	} ;

	friend class ClFONT_M_FileRFN;

MIT_PRIVATE:

	//-----------------------Informacion independiente del device-------------------

	//NOTA: 
	//	Hay un Init particular por device, en el que se deben de pasar los datos 
	//necesarios para generar la fuente teorica. Si los datos representan 
	//fisicamente una fuente con tamaños distintos a los teoricos, las alineaciones 
	//seran incorrectas.
	//	Cuando hacemos un Init de una Font, las alturas, etc, que pasamos son lo que 
	//desamos obtener, pero el mapeador, en cada caso, se APROXIMA lo que puede a 
	//estos requisitos, pero puede que no logre ajustarse 100% a lo indicado. 
	//	DesignHeight y DesignMaxWidth son el alto/ancho que se pide en el Init, 
	//mientras que Height y MaxWidth son los que el mapeador logra: debemos garantizar
	//que estos valores son menores o iguales a los pedidos. 
	//	Cuando algun objeto grafico calcule su altura o anchura en funcion de altos o 
	//anchos de fuente, seria muy aconsejable que utilizara los valores Design, 
	//porque asi se asegura de que el tamaño sera siempre el mismo, y de que el texto
	//cabra siempre, porque los valores no de Design siempre son menores o iguales a 
	//los reales.

	m4uint32_t m_uiIdFont ;	//Id de fuente para DBFont
	ClMIT_Str m_oFaceName ;		//Nobre simplemente indicativo

	ClMIT_Str m_oFRN_File ;	//Fichero desde el que se generó la font
	Point_t m_MapHeight ;	//Altura de mapeo cuando se generó la font

	enum {								
		FIXED_SPACE = ClMIT_Flag::BIT0, 
		UNDERLINE = ClMIT_Flag::BIT1,
		BOLD = ClMIT_Flag::BIT2,
		ITALIC = ClMIT_Flag::BIT3,
		SOFT_FONT = ClMIT_Flag::BIT4
	};
	ClMIT_Flag m_oFlag;

	DvcUnit_t m_Height ;			//Alto de un caracter
	DvcUnit_t m_MaxWidth;			//Ancho del caracter mas grande (en fuentes fijas, 
									//	todos tienen el mismo ancho).
	DvcUnit_t *Width;				//Solo para fuentes variables, array con los anchos 
									//	por cada caracter.
	
	DvcUnit_t m_DesignHeight, m_DesignMaxWidth ;

	m4int32_t m_SoftFontIndex ;	//-1 ó indice al SoftFontData en la DBFont

MIT_PUBLIC:

	enum LOAD_INFO { PCL_INFO, PSC_INFO, ESCP_INFO, RVW_INFO, LOAD_INFO_COUNT } ; 

MIT_PRIVATE:

	//Flags que se ponen nada más crear la font. Dicen si están o no implementadas las 
	//	metricas para el device indicado en el indice.

	m4bool_t m_bImplemented[LOAD_INFO_COUNT] ;

MIT_PUBLIC:
	
	m4bool_t IsImplementedFor(LOAD_INFO ai_LoadInfo) { return m_bImplemented[ai_LoadInfo] ; }

	//-----------------------Informacion PCL--------------------------------------

	struct StPCL {
		m4int16_t iTypeFace ;		// El device PCL le debe sumar el codigo base
		ClMIT_Str m_pcSymbolSet ;
		CPI_t Pitch;
		Point_t Height ;

	} m_stPCL ;

MIT_PUBLIC:
	void SetPCLSoftFontTypeFace(m4int16_t ai_Index) { 
		m_stPCL.iTypeFace = PCL_SOFT_FONT_BEGIN+ai_Index  ;
	} 
MIT_PRIVATE:

	//-----------------------Informacion PSC--------------------------------------
	
	struct StPSC {

		ClMIT_Str pcTypeFace ;	
		CPI_t Pitch;
		Point_t Height ;

	} m_stPSC ;

	//-----------------------Informacion RVW--------------------------------------

	struct StRVW {
		ClMIT_Str m_oFaceName ;
		Point_t m_Height ;
		CPI_t m_Pitch;
	} m_stRVW ;

	//-----------------------Informacion ESC/P-----------------------------------

MIT_PUBLIC:

	enum { ESC_FONT_DRAFT=-1 } ;
	enum ESC_PITCH { ESC_PITCH_10=10, ESC_PITCH_12=12 } ; //En CPI, valido si no proporcional
	enum ESC_HEIGHT { ESC_HEIGHT_10=10 } ; //En Point

MIT_PRIVATE:

	struct StESC {

		enum { 
			DOUBLEY=ClMIT_Flag::BIT0, DOUBLEX=ClMIT_Flag::BIT1,
			COMPR=ClMIT_Flag::BIT2	//No valido con prop, se reduce ancho a un 60%
		} ;
		ClMIT_Flag oFlag;

		m4int16_t iFont ;	//Numero usado en ESC-k, o FONT_DRAFT  
		ESC_PITCH Pitch;
		Point_t Height ;

	} m_stESC ;	

	//-----------------------Informacion Translation-------------------------------
	//	Normalmente, solo debe ser usada para codificaciones de codigo de barras.

MIT_PRIVATE:

	m4int16_t * m_piTrans ;	//Puede ser NULL. Solo se carga si tipo no es vacio.
	m4int16_t * m_piPSC_AFM_Widths ; //Idem 

MIT_PUBLIC:

	// ------------------------- Inicialización ----------------------------------	

	ClFONT_M_Font() ;
	virtual	~ClFONT_M_Font() { ClFONT_M_Font::End();}
	virtual size_t GetSizeof() { return sizeof(ClFONT_M_Font) ; }
	virtual void End() ;

	// Crea el objeto, y carga la información correspondiente. 
	// Propaga un error si tiene algún problema (fichero no encontrado, 
	//	datos no encontrados, etc.). 
	// El valor ai_bFixedSpace se pasa como mecanismo de seguridad adicional. Si no 
	//coincide con el valor del fichero, se debe provocar un error.

	m4bool_t Init( 
		LOAD_INFO ai_LoadInfo,
		ClMIT_IdNumDesc *ai_poIdDesc, const m4char_t *ai_pcRFNFileName,
		Point_t ai_MapHeight,
		m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, 
		m4bool_t ai_bBold, m4bool_t ai_bItalic,
		m4int32_t * ao_SoftFontOffset, m4int32_t *ao_SoftFontSize,
		ClMIT_Str *ao_poSofFontFile,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR	
	);

	//Lo pone el DBFont::SaveFont
	void SetSoftFontIndex(m4int32_t ai_Index) { m_SoftFontIndex=ai_Index; }

	// ---------------- Consulta de Atributos generales de la Fuente -----------------	

	m4uint32_t GetId() { return m_uiIdFont ; }

	m4bool_t IsFixedSpace() {return m_oFlag.True(FIXED_SPACE);}
	m4bool_t IsUnderline() {return m_oFlag.True(UNDERLINE);}
	m4bool_t IsBold() {return m_oFlag.True(BOLD);}
	m4bool_t IsItalic() {return m_oFlag.True(ITALIC);}
	m4bool_t IsSoftFont() {return m_oFlag.True(SOFT_FONT);}
	const m4char_t * GetFaceName()  { return m_oFaceName ; }

	const m4char_t * GetRFN_FileName()  { return m_oFRN_File ; }
	Point_t GetMapHeight() { return m_MapHeight ; }
	ClMIT_IdNumDesc * GetIdNumDesc() { return &m_oLocation ; }
	
	m4int32_t  GeSoftFontIndex() { return m_SoftFontIndex; }

	// ----------------------- Atributos RVW -----------------------------------------	
	// SIEMPRE SE CARGAN.
	// Si por lo que sea hubo problemas, contendra valores dummy 

	const m4char_t * RVW_GetFaceName()  { return (m4char_t *)m_stRVW.m_oFaceName ; }
	Point_t RVW_GetHeight() { return m_stRVW.m_Height;} 
	CPI_t RVW_GetPitch() { return m_stRVW.m_Pitch;} 

	
	// ----------------------- Atributos PCL -----------------------------------------	

	// Puntero de uso inmediato

	const m4char_t * PCL_GetSymbolSet() { return m_stPCL.m_pcSymbolSet.InChar(0) ;}
	CPI_t PCL_GetPitch() { return m_stPCL.Pitch;} 
	Point_t PCL_GetHeight() { return m_stPCL.Height;} 
	m4int16_t PCL_GetTypeFace() { return m_stPCL.iTypeFace ; }

	// ----------------------- Atributos PSC -----------------------------------------

	const m4char_t * PSC_GetTypeFace()  { return (m4char_t *)m_stPSC.pcTypeFace ; }
	CPI_t PSC_GetPitch() { return m_stPSC.Pitch; } 
	Point_t PSC_GetHeight() { return m_stPSC.Height; }  

	// ----------------------- Atributos ESC -----------------------------------------	

	CPI_t ESC_GetPitch() { return m_stESC.Pitch;} 
	Point_t ESC_GetHeight() { return m_stESC.Height;} 
	m4int16_t ESC_GetFontCode() { return m_stESC.iFont;} 

	m4bool_t ESC_IsDoubleY() { return m_stESC.oFlag.True(StESC::DOUBLEY) ; } 
	m4bool_t ESC_IsDoubleX() { return m_stESC.oFlag.True(StESC::DOUBLEX) ; } 
	m4bool_t ESC_IsCompr() { return m_stESC.oFlag.True(StESC::COMPR) ; } 

	// ------------------------------ Conversiones a Dvc -----------------------------	

	// Devuelve la longitud de la cadena en Dvc. Si la fuente es no proporcional será
	// GetDvcWidthChar * número de chars. Si la fuente es proporcional da la mayor 
	// anchura posible en la Font * el número de chars. 

	DvcUnit_t GetDvcLenStr(size_t ai_szNumChar);

	// Devuelve la longitud de una cadena ponderada en Dvc. La longitud, en fuentes 
	//   proporcionales, es dependiente de la cadena.

	DvcUnit_t GetDvcLenStr(m4pchar_t ai_pcStr, size_t ai_szNumChar);
	DvcUnit_t GetDvcLenStr(m4pchar_t ai_pcStr) ;
	
	//Altura en Dvc. Todos caracteres, independientemente del tipo de fuente, tienen
	//	siempre la misma altura.
	
	DvcUnit_t GetDvcHeight() {return m_Height;}
	DvcUnit_t GetDvcDesignHeight() {return m_DesignHeight;}

	// Devuelve la anchura en Dvc de un carácter, que es fija en fuentes no proporcionales
	//   y variable en dependientes.

	DvcUnit_t GetDvcWidthChar( m4char_t ai_cChar );

	//Anchura del caracter mas ancho. 

	DvcUnit_t GetDvcMaxWidthChar( ) { return m_MaxWidth; }
	DvcUnit_t GetDvcDesignMaxWidthChar( ) { return m_DesignMaxWidth; }

	// ---------------------------- Operaciones sobre cadenas -------------------------	

	// Los siguientes dos métodos son necesarios por el tema de las fuentes proporcio-
	//   nales.
	// Estos métodos reciben una cadena y un número de Units. La primera calcula los ca-
	// racteres que corresponden a esas Units. Es decir, empezando por la derecha cuantos
	// caracteres corresponden a esa Len. Ambas redondean al defecto.
	// La siguiente es identica, pero comenzando por la izquierda.
	// Ambas garantizan que nunca se devuelven mas caracteres de los que tenga la cadena.

	size_t TrimRightStr(m4pchar_t ai_pcStr, DvcUnit_t ai_UnitLen);
	size_t TrimLeftStr(m4pchar_t ai_pcStr, DvcUnit_t ai_UnitLen);

	//-----------------------Informacion Translation-------------------------------

	ENG_SOFT_TRANS_TYPE GetTranslationType() ;

	//Obtiene el caracter asociado al pasado, segun el array de translation. Pasar como
	//  parametro valores de ENG_SOFT_TRANS_INDEX, con sumas si quiere accederse a 
	//	offsets.
	//Si no hay array de translation o el valor pasado se sale de rango, devuelve el
	//	mismo valor que se introduce.
	//Pe, GetTranslation(ENG_TRANS_I_2_5_START) da el char de start para interleaved.
	//Pe, GetTranslation(ENG_TRANS_I_2_5_ZERO+31) da el char de interleaved asociado al
	//			valor 31
	
	m4char_t GetTranslation(m4int16_t ai_Char) ;

	//-----------------------Otros-------------------------------

	//Si no hay metricas de este tipo o indice fuera de rango, se devuelve 0
	m4int16_t GetAFM_Width(m4int16_t ai_Pos) ;

} ;

//==============================================================================ClFONT_M_DBFont

class M4_DECL_M4FMETRICS ClFONT_M_DBFont
{
	// Cuidado cuando se descarga este objeto: es normal que otros componentes tengan punteros
	//    apuntando a Fonts de este objeto, por lo que debe de destruirse despues.

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;

MIT_PUBLIC:

	// --------------Lista de Soft-Fonts, subconjunto de Lista de Fonts-------------	
	// Esta estructura ha de hacerse public, para tener acceso desde los Devices del engine
	
	struct StSoftFontData {
		ClMIT_Str m_oPathName ;	//Nombre completo fichero RFN de procedencia
		ClMIT_Str m_oSofFontFile ; //Nombre SOLO del fichero soffont que se indico en el PAR
		m4int32_t m_Offset ;	//Offset de la información
		m4int32_t m_Size ;		//Tamaño a leer
		ClFONT_M_Font*m_poFont ;	//Link a la primer Font que creó esta SoftFont
		m4int32_t m_Index ;		//Indice auxiliar para formar Id numericos unicos
	} ;
	
MIT_PRIVATE:

	typedef map < ClMIT_Str, StSoftFontData, less<ClMIT_Str> > SoftFontList_t ;	//Name-Data
	SoftFontList_t m_oSoftFonts ;
	
	// -------------------Lista de Fonts e indices asociados--------------------------	

	ClFONT_M_Font* m_poMonoFont;	

	typedef map < m4uint32_t, ClFONT_M_Font*, less<m4uint32_t> > FontList_t ;	//Id-Font
	typedef map < m4int16_t, ClFONT_M_Font*, less<m4int16_t> > FontInsertOrder_t ;	//Index-Font

	FontList_t m_oFontList ;
	FontInsertOrder_t m_oIndexOrder ;	//Apunta a las mismas FONTS que m_oFontList, solo que aqui la
								//	clave es el orden de insercion en la DBFont.
	
MIT_PUBLIC:
	// ------------------- Contructor y Destructor -------------------------------	

	ClFONT_M_DBFont() ;
	virtual	~ClFONT_M_DBFont() { ClFONT_M_DBFont::End(); }
	virtual size_t GetSizeof() { return sizeof(ClFONT_M_DBFont) ; }

	// ------------------------ Inicialización -----------------------------------	

	void Init();

	//Clona una DBFont, pero cargando el tipo de informacion especificado

	m4bool_t Init( ClFONT_M_DBFont *ai_poFrom, ClFONT_M_Font::LOAD_INFO ai_LoadInfo,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	//   Borra array de fuentes
	virtual void End() ;

	// --------------------- Informacion SoftFonts --------------------------------	

	m4int16_t GetNumSoftFonts() { return m_oSoftFonts.size() ; }

	// Si ai_bSwOblig es TRUE, error fatal si no encuentra.
	// Da NULL si no lo encuentra.

	StSoftFontData * GetSoftFontDataByIndex(m4int16_t ai_iIndexFont,m4bool_t ai_bSwOblig=M4_TRUE);
	
	// --------------------- Operaciones con fonts --------------------------------	

	// Se inserta una nueva Font, error si no puede.
	// Va actualizando la lista de SoftFonts, y generando en este caso en PCL iTypeFace
	//	validos, en base a PCL_SOFT_FONT_BEGIN+Index

	m4bool_t SaveFont( 
		ClFONT_M_Font::LOAD_INFO ai_LoadInfo,
		ClMIT_IdNumDesc *ai_poIdDesc, const m4char_t *ai_pcRFNFileName,
		Point_t ai_MapHeight,
		m4bool_t ai_bFixedSpace, m4bool_t ai_bUnderline, 
		m4bool_t ai_bBold, m4bool_t ai_bItalic,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR	
	);

	m4int16_t GetNumFonts() { return m_oFontList.size() ; }

	// Devuelve la dirección de una font correspondiente al orden de llegada o Id, NULL si no 
	//	encuentra.
	// Si ai_bSwOblig es TRUE, error fatal si no encuentra.

	ClFONT_M_Font * GetAddressFontByIndex(m4int16_t ai_iIndexFont,m4bool_t ai_bSwOblig=M4_TRUE);
	ClFONT_M_Font * GetAddressFontById(m4uint32_t ai_uiIdFont,m4bool_t ai_bSwOblig=M4_TRUE);
	ClFONT_M_Font * GetAddressSmallestFont() ;	//Font con la menor altura (de diseño)
	ClFONT_M_Font * GetAddressThinestFont() ;	//Font con la menor anchura (de diseño)

	// Devuelve -1 o el indice correspondiente a la fuente cuyo Id se indica.
	// Si SwOblig, da error immediato si no se encuentra

	m4int16_t FindIndexFont( m4uint32_t ai_uiIdFont, m4bool_t ai_bSwOblig=M4_TRUE ) ;


	// El modo normal de funcionamiento es multifuente.
	// En modo monofuente, cualquier llamada a GetAddressFontXXXX devuelve una unica fuente, que
	//	es la indicada en SetMonoFontMode (ErrorFatal si se pasa NULL).

	void SetMultiFontMode() { m_poMonoFont=NULL ; }
	void SetMonoFontMode(ClFONT_M_Font*  ai_poFont) ;

} ;

#endif
