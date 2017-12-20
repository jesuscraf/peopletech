//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             RFNfileW.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

#ifndef __RFNFILEW__H__
#define __RFNFILEW__H__

#include "clstr.h"
#include "clmetric.h"
#include "fontm.h"
#include "rfnstrt.h"
#include "m4stl.hpp"
#include "softtrans.h"


//==========================================================================ClRFN_RFN_W
/*
	Esta clase gestiona la creacion de un fichero RFN.
*/

class ClRFN_RFN_W
{
MIT_PUBLIC:

	StMIT_Inspector  *m_poInsp ;	//Simple link

	ClMIT_File m_oFile ;	//Fichero a escribir.

	m4uint32_t m_IndexZoneOff ;		//Offset comienzo bloques Index
	m4uint32_t m_MetricZoneOff ;	//Offset bloques Metric, donde escribir
	m4uint32_t m_CountIndexWritten ;	//Index block escritos hasta el momento
	m4uint32_t m_CountMetricWritten ;	//Metric block escritos hasta el momento

	ClMIT_Str m_oPCL_Driver, m_oESCP_Driver, m_oRVW_Driver, m_oPSC_Driver ;

	typedef list <m4int32_t> OffList_t ;	//Lista auxiliar de Offsets de fichero a actualizar
	struct StSoftFontData {
		ClMIT_Str m_oFileName ;	//Nombre completo, path incluido
		OffList_t m_oOffsets ;	//Offsets a actualizar
		ClMIT_Str m_oTrans ;	//Cadena de translacion
		m4int16_t m_iPSC_AFM_Widths[ENG_PSC_AFM_WIDTHS_ARRAY_SIZE] ; 
	} ;
	typedef map <ClMIT_Str, StSoftFontData, less<ClMIT_Str> > SoftFontList_t ;	//Key,nombre fichero
	SoftFontList_t m_oSoftFonts ;

MIT_PUBLIC:

	//PUBLIC SOLO PARA CONSULTA !

	struct StHeaderData {
		m4int16_t m_HeightVarCount ;
		m4bool_t m_bSwFixed ; 
		m4bool_t m_bSwOn[ClFONT_M_RFNStruct::DVC_COUNT] ;
	} m_oHeaderData ;
	
MIT_PUBLIC:

	struct StHeightVarData {
		m4int32_t m_DesignHeight;		//Centesimas de Point
		m4int32_t m_DesignWidth ;		//Centesimas de CPI. 0 lo calcula segun la maxima metrica.
	} m_oHeightVarData ;

	struct StSelectionItem {

		m4bool_t	m_bLoaded[ClFONT_M_RFNStruct::DVC_COUNT] ;	//Dice si se especifico el Item Selection

		m4int16_t	m_PCL_Height ;		//Centesimas de Point
		m4int16_t	m_PCL_MaxWidth ;	//DvcUnit
		m4int16_t	m_PCL_SelectionHeight ;	//Centesimas de Point
		m4int16_t	m_PCL_DvcFontId ;
		ClMIT_Str   m_PCL_DvcCharSet ;
		ClMIT_Str   m_PCL_DvcSoftFontF ;	//Al grabar tiene tratamiento especial. Solo NdFile
		ClMIT_Str   m_PCL_DvcSoftFontP ;	//	idem, pero Path completo
		ClMIT_Str   m_PCL_DvcTrans ;

		m4int16_t	m_PSC_Height ;		//Centesimas de Point
		m4int16_t	m_PSC_MaxWidth ;	//DvcUnit
		m4int16_t	m_PSC_SelectionHeight ;	//Centesimas de Point
		ClMIT_Str   m_PSC_DvcFontId ;
		ClMIT_Str   m_PSC_DvcSoftFontF ;	//Al grabar tiene tratamiento especial. Solo NdFile
		ClMIT_Str   m_PSC_DvcSoftFontP ;	//	idem, pero Path completo
		ClMIT_Str   m_PSC_DvcTrans ;

		m4int16_t	m_ESCP_Height ;		//Centesimas de Point
		m4int16_t	m_ESCP_MaxWidth ;	//DvcUnit
		m4int16_t	m_ESCP_SelectionHeight ;	//Centesimas de Point
		m4int16_t	m_ESCP_DvcFontId ;
		m4bool_t    m_ESCP_Sw2Y, m_ESCP_Sw2x, m_ESCP_SwCompr ;
		
		m4int16_t	m_RVW_Height ;		//Centesimas de Point
		m4int16_t	m_RVW_MaxWidth ;	//DvcUnit
		m4int16_t	m_RVW_SelectionHeight ;	//Centesimas de Point
		ClMIT_Str	m_RVW_DvcFontId ;
		ClMIT_Str	m_RVW_DvcTrans ;

		//Solo para paso Variable
		m4int32_t   m_Metric[ClFONT_M_RFNStruct::DVC_COUNT]
							[ClFONT_M_RFNStruct::METRIC_ARRAY_SIZE] ; 

		void Reset() ;
		m4bool_t NothingLoaded() ;

	} m_oSelectionItem[ ClFONT_M_RFNStruct::SELECTION_COUNT ] ;

MIT_PUBLIC:

	ClRFN_RFN_W() ;  

	virtual ~ClRFN_RFN_W() { ClRFN_RFN_W::End() ;}
	virtual size_t GetSizeof() { return sizeof(ClRFN_RFN_W) ; }

	virtual void End() ;	//Cierra el fichero
	void Init() ;

	//============================================================>Crear RFN
	// Para crear el RFN, el ciclo a seguir es:
	//
	//	-PrepareFile()
	//	-Bucle N bloques HEIGHT a procesar
	//		-BeginHeightBlock()
	//			-Bucle M selections activas a procesar
	//				-Llamadas especificas a SelectionDataDvcXXX segun los Dvc activos.
	//			-Fin Bucle
	//		-EndHeightBlock()
	//	-Fin Bucle
	//	-EndFile()
	//
	// Se puede abortar el ciclo cuando se quiera. No hay porque llamar a EndFile() si se aborta,
	//	pero si todo va correcto si..


	//En el Init se crea el Header Block del fichero, y se escriben tantos Index Block basura
	//	como ai_HeightVarCount haya, con el fin de poder ir escribiendo Metric Blocks segun
	//	se necesiten.
	
	m4bool_t PrepareFile( 
		m4char_t * ai_pcPCL_Driver, m4char_t * ai_pcESCP_Driver, m4char_t *ai_pcRVW_Driver, 
		m4char_t * ai_pcPSC_Driver,
		m4char_t *ai_pcFileName, m4char_t *ai_pcFontName, m4char_t *ai_pcFilePARName,
		m4int16_t ai_HeightVarCount, m4bool_t ai_bSwFixed, 
		m4bool_t ai_bSwPCL, m4bool_t ai_bSwPSC, m4bool_t ai_bSwRVW, m4bool_t ai_bSwESCP ) ;

	//Si faltan de escribir bloques Height, error.
	//Graba las SoftFonts, si hay.
	//Cierra el ciclo y el fichero

	void EndFile() ;	

	//Guarda datos en bloque cabecera m_oHeaderData para escribir luego.

	void  BeginHeightBlock(m4int32_t ai_DesignHeight, m4int32_t ai_DesignWidth) ;

	//Reescribe el IndexBlock. Si es preciso, calcula m_DesignWidth.
	//Compara las metrics pasadas de todas las variaciones de negrita,etc, redireccionando a 
	//	un unico MetricBlock aquellas que coincidan.
	//Si se escriben mas bloques de los indicados en Header, error.
	//Combinaciones no pasadas, son desactivadas.

	m4bool_t EndHeightBlock() ;

	//Obtienen los datos, mapeando y haciendo lo pertinente.
	//Llamadas para devices desactivados son ignoradas.
	//	-ai_MapHeight se usa para mapear la font en windows.
	//	-ai_Pitch. Pone el valor PCL_MaxWidth. Si 0, el mayor ancho mapeado.
	//	-ai_DvcHeight.Pone PCL_Height. Si 0, la altura que de el mapeador windows.
	//	-ai_SelectionHeight. Pone PCL_SelectionHeight. Si 0, se iguala a ai_DvcHeight.
	//Si ya se cargo la seleccion pasada, error.

	m4bool_t SelectionDataPCL( 

		m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
		m4char_t *ai_pcMapName, 
		Point_t ai_MapHeight, CPI_t ai_Pitch, 
		Point_t ai_DvcHeight, Point_t ai_SelectionHeight,

		m4int16_t ai_PCL_DvcFontId,	m4char_t * ai_PCL_DvcCharSet,
		m4char_t * ai_PCL_SoftFont,	//Pasar con path completo si hace falta
		m4char_t * ai_PCL_DvcTrans
	) ;


	m4bool_t SelectionDataPSC( 

		m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
		m4char_t *ai_pcMapName, Point_t ai_MapHeight,  CPI_t ai_Pitch,
		Point_t ai_DvcHeight, Point_t ai_SelectionHeight,

		m4char_t * ai_PSC_DvcFontId,	
		m4char_t * ai_PSC_SoftFont,	//Pasar con path completo si hace falta
		m4char_t * ai_PSC_DvcTrans
	) ;
	
	
	m4bool_t SelectionDataRVW( 

		m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
		m4char_t *ai_pcMapName, Point_t ai_MapHeight, CPI_t ai_Pitch, 
		Point_t ai_DvcHeight, Point_t ai_SelectionHeight,
		m4char_t * ai_RVW_DvcTrans
	) ;

	m4bool_t SelectionDataESCP( 

		m4bool_t ai_bSwBold, m4bool_t ai_bSwItalic, m4bool_t ai_bSwUnderline,
		m4char_t *ai_pcMapName, Point_t ai_MapHeight, CPI_t ai_Pitch,
		Point_t ai_DvcHeight, Point_t ai_SelectionHeight,

		m4int16_t ai_ESCP_DvcFontId, m4bool_t ai_ESCP_Sw2Y, m4bool_t ai_ESCP_Sw2x, 
		m4bool_t ai_ESCP_SwCompr 
	) ;

	//============================================================>Usos especiales

	//	Parte de un fichero PRN generado a través de Word o algun otro programa, usando un driver
	//HP LaserJet Series II, ya que este driver genera softfonts.
	//	Extrae las SoftFonts y las almacena en un fichero aparte (USP) para usarlas en PCL. Por
	//cada SoftFont que encuentre, va creando un fichero distinto, numerando secuencialmente
	//los ficheros y aplicandoles la extension USP.
	//	Si se pasa ai_iCharSetOffset, aplica un Offset a los caracteres generados. Esto es 
	//porque con algunas fonts, el driver aplica un Offset en el juego de caracteres, y como esto
	//no nos interesa, tenemos la posibilidad de contrarestar este efecto.
	//	No insertar graficos en el PRN, ya que usamos funciones de cadena sensibles al valor 0 como
	//fin de cadena. La unica manera de meter flujos binarios en PCL es pasar raster-maps, y esto
	//pasa con graficos (que no tratamos) y con mapas soft-fonts (que si tratamos).
	//  El fichero PRN se carga por completo en RAM, por lo que hay un limite para evitar 
	//problemas. Este limites es PRN_MAX_SIZE.
	//	Da FALSE si tiene problemas.

	enum { PRN_MAX_SIZE=500000 } ;

	static m4bool_t ExtractFromPRN_To_USP(m4char_t *ai_pcPRNFile, m4char_t *ai_pcUSPFile, 
		m4int16_t ai_iCharSetOffset, m4int16_t *ao_poFontsCreated ) ;

	//	Igual que ExtractFromPRN_To_USP, solo que para fonts PFA (Postscricpt).

	static m4bool_t ExtractFromPRN_To_PFA(m4char_t *ai_pcPRNFile, m4char_t *ai_pcPFAFile, 
		m4int16_t *ao_poFontsCreated ) ;

	// Algoritmo de conversion para una fuente TTF a PFA
	static m4bool_t ConvertFromTTF_To_PFA(m4char_t *ai_pcTTFFile, m4char_t *ai_pcPFAFile);

MIT_PRIVATE:

	//Auxiliar ExtractFromPRN_To_USP
	static m4bool_t FindSoftFontHeader(m4char_t *ai_pcFrom, m4char_t **ao_pcBegHeader, 
		m4char_t **ao_pcBegHeaderData, size_t *ao_SizeData ) ;	
	//Auxiliar ExtractFromPRN_To_USP
	static m4bool_t FindSoftFontCharDef(
		m4char_t *ai_pcFrom, m4int32_t *ao_CharCode, m4int32_t *ao_SizeDef,
		m4char_t **ao_pcBegDefSeq, m4char_t **ao_pcBegDef ) ;

MIT_PRIVATE:

	//============================================================>Auxiliares

	inline void WInt(m4int16_t ai_Int) ;
	inline void WLong(m4int32_t ai_Long) ;
	inline void WChar(m4uint8_t ai_Char) ;
	inline void WStr(m4char_t *ai_pcStr, m4int32_t ai_Size) ; //ai_Size teorico sin NULL
	
	//No se implementa como operador == porque hace falta datos del Header

	m4bool_t SelectItemEq( 
		ClRFN_RFN_W::StSelectionItem *ai_OpeA, ClRFN_RFN_W::StSelectionItem *ai_OpeB) ;

	m4bool_t MapFont( 
		m4char_t *ai_pcDevice, m4char_t *ai_pcFaceName, Point_t ai_MapHeight,
		m4bool_t ai_bBold, m4bool_t ai_bItalic, m4bool_t ai_bUnderline,
		m4bool_t ai_bSwFixed, 
		DvcUnit_t *ao_iHeight, DvcUnit_t *ao_iMaxWidth, DvcUnit_t *ao_piWidth,
		m4char_t *ai_pcDriver) ;

	//Actualiza la lista de FontName, creando una nueva entrada con un Offset nuevo si el 
	//	fichero no existia en la lista, o si existe creando solo un nuevo Offset.

	m4bool_t AddSoftFont( m4char_t *ai_pcFullFontName, m4int32_t ai_FileOff, 
		m4char_t *ai_pcTranslation, m4bool_t ai_bPSC_AFM) ;

	//Crea los SoftFont Blocks y actualiza sus referencias. Error si no puede abrir algun
	//	fichero.
	//Graba los bloques a partir del final del fichero. A la salida no garantiza ningun 
	//	posicionamiento particular.

	m4bool_t SaveSoftFontsToFile() ;


	//Comvierte y valida los datos de translate tipo cadena a un array. (Cadena vacia="-")
	//Previamente, resetea todos valores a 0.
	//Da false si hay errores.

	m4bool_t TranslationStrToArray( m4char_t *ai_pcTrans, 
		m4int16_t ao_Array[ENG_TRANS_ARRAY_SIZE]  ) ;

	//Auxiliar de TranslationStrToArray, crea un vector con todas las entradas en forma
	//	secuencial (parsea formato base*repeticiones).
	//La primera entrada del array siempre es un caracter, y el resto son numeros. En 
	//	ao_FirstEntry se devuelve este caracter, y en el vector las restantes entradas 
	//	(es decir, un I,12,4 devolveria en ao_FirstEntry I, y un vector de tamaño 2,
	//	conteniendo 12 y 4).
	typedef vector <m4int16_t> IntVector_t ;	
	m4bool_t TranslationVector( m4char_t *ai_pcTrans, IntVector_t &ao_Vector, m4char_t *ao_FirstEntry ) ;

} ;

#endif

