//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvctxt.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:22/09/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
#ifndef __CLDVCTXT__H__
#define __CLDVCTXT__H__

#include "clstr.h"

#include "cldvcfi.h"
#include "pageprn.h"
#include "font.h"

//==============================================================================ClENG_PageTxt
class ClENG_PageTxt {

friend class ClENG_DvcTxt;

MIT_PROTECTED:

	StMIT_Inspector* m_poInsp;	//Simple link

MIT_PRIVATE:

	//Caracter de relleno usado internamente  buffer. Actualmente, el 255. 
	static const m4char_t m_cFillChar ;

	//Caracteres por fila y por columna.
	//MaxRowPrinted tiene el numero maximo de filas impresas hasta el momento en la actual 
	//pagina (0, no se imrpimio nada).
	//m_DvcDifRow, m_DvcDifCol tienen el diferencial usado para ajustes de fila y columna.
	//	Ahora mismo, el 50% de sus homologos (parte inferior que provoca el salto).
	m4uint16_t m_iRows, m_iMaxRowPrinted;		
	m4uint16_t m_iCols;
	DvcUnit_t m_DvcPerRow, m_DvcPerCol, m_DvcDifRow, m_DvcDifCol ;

	// Array dinámico de tamaño: [m_iCols*m_iRows]
	// A este buffer volcarán los datos los métodos Action: PrintNChar, PrintStr, ...
	//	posteriormente se volcará todo al buffer de pag de ClENG_OutDevice, en el 
	//	método EOP (volcará y limpiará buffer de pag).
	ClMIT_Str m_oBuff ;

	// Linea de tamaño [m_iCols], auxiliar de proceso EOP
	ClMIT_Str m_oRowAux ;
	// Array de tamaño [m_iCols], auxiliar de proceso EOP 
	m4uint32_t *m_puiRowEatenAux ;

	// sizeof(unsigned short) en unicode y sizeof(char) en ansi.
	int m_iSizeOfCharacter;

MIT_PUBLIC:

	// Cada ClENG_PageTxt tiene asociada un objeto ClENG_PagePrinter. 
	ClENG_PagePrinter m_oPagePrinter;
	

MIT_PUBLIC:
		
	// ---------------------- Inicialización -------------------------------------	

	/*	Init aloca espacio para el buffer de pag
		Le deberá entrar el tamaño del papel y lo que necesita el PagePrinter para
			inicializarse, y la fuente a utilizar.
		Según el tamaño del papel y la height y cpi de la fuente se determinará
			el número de filas y columnas. Se ajustan de tal forma que una fila 
			ocupe exactamente el alto y ancho de diseño de la font.
		De la misma forma, una coordenada se ajusta por proximidad al 50%. Es decir, si
			la primera fila va desde la coordenada 0 a la 15, una cadena con coordenadas 
			0-7 va a la primera fila, y una que vaya del 8 al 15 va a la primera. En la
			ultima fila o columna no se hace este ajuste.
	*/	
	void Init(	ClENG_PagePrinter::ORIENTATION ai_Orientation, 
				ClENG_PagePrinter::PAPER_SIZE  ai_PageSize, 
				ClENG_PagePrinter::PAPER_SOURCE  ai_PaperSource,
				DvcUnit_t ai_SizeX,   
				DvcUnit_t ai_SizeY,
				ClFONT_M_Font *ai_poFont,
				int ai_iSizeOfCharacter);
	
	ClENG_PageTxt() ;
	virtual ~ClENG_PageTxt() { ClENG_PageTxt::End() ; }
	virtual void End();
	size_t GetSizeBuff() {return m_oBuff.GetMaxSize();}

	// Rellena el buffer YA CREADO con m_cFillChar. Resetea MaxRowPrinted a 0.

	void ClearBuff(); 

	//----------------------- Gets---------------------------------------//

	m4uint16_t GetNCols () { return m_iCols; }

};



//==============================================================================ClENG_DvcTxt
class ClENG_DvcTxt : public ClENG_DvcFile
{
MIT_PUBLIC:

	enum { MAX_SIZE = 300, NO_FILL_CHAR=-1, NO_FIELD_DELIMITER_CHAR=-1 };

MIT_PRIVATE:

	ClENG_PageTxt *m_poPage; // array dinámico de páginas 
	
	ClFONT_M_Font *m_poFont ;	//Simple link. Font usada para los ajustes de Dvc a Filas/Columnas

	// Secuencia de caracteres a imprimir como salto de linea y como salto de pagina.
	ClMIT_Str m_oEOLSeq, m_oEOPSeq ;

	// Caracter de relleno que sustituira a las ocurrencias de ClENG_PageTxt::m_cFillChar.
	// Normalmente es un caracter en blanco. Un NO_FILL_CHAR no mete ningun caracter, con
	//	lo cual el efecto sera una especie de concatenacion de columnas no escritas.

	m4char_t m_cFillChar ;

	//Si m_bClipColumn esta activo, las secuencias ClENG_PageTxt::m_cFillChar al final de una
	//	columna seran recortadas.
	//Si m_bFillPageRows esta activo, al saltar de pagina se generaran tantos saltos de linea 
	//	vacios como hagan falta para que todas paginas tengan el mismo numero de linea.
	m4bool_t m_bClipColumn, m_bFillPageRows ;

	// Lista Auxiliar de Pi, ordenada por Offset para el caso de optimizacion
	//	a txt fichero , poder gestionar correctamente el offset de fichero.
	//	Contiene links (no objetos) al StPI_t en cuestion.
	typedef multimap<m4uint32_t, StPi_t *, less<m4uint32_t> > TxtMultiMapPi;
	TxtMultiMapPi m_TxtMultiMapPi;  
	
	//MANU - IgnoreWarning es un booleano que indica que la Warning de String Overlapped
	// no debe mostrarse. Este flag se lee desde las properties de Report (211).
	m4bool_t m_bIgnoreWarning;

MIT_PUBLIC:	
	
	// ---------------------- Inicialización -------------------------------------		
	ClENG_DvcTxt();
	virtual ~ClENG_DvcTxt() { ClENG_DvcTxt::End(); }
	virtual void End();
	virtual size_t GetSizeof() { return sizeof(ClENG_DvcTxt) ; }

	// El device es:
	//		PRELOAD_ON_DEVICE	= FALSE
	//		COLOUR_SUPPORTED	= FALSE
	//		COLOUR_ACTIVATED	= FALSE
	//		RASTER_PRELOADED	= FALSE
	//		HARD_DITHERING		= FALSE
	//		HARD_SCALE			= FALSE
	// No soporta impresion de graficos	
	// Si ai_bSmallestFont=TRUE, se coje el Id de la font con la altura menor de la DbFont, si no 
	//	la indicada.

	m4bool_t Init(	

		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, 
		m4bool_t ai_bDelete,
		StENG_DvcParams::DUPLEX_MODE ai_Duplex, 
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Especificos del device

		ClFONT_M_DBFont* ai_poDBFont, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		m4uint32_t ai_uiIdFont, 
		m4bool_t ai_bIgnoreTxtWarning,
		m4bool_t ai_bSmallestFont=M4_FALSE,
		m4char_t *ai_pcEOLSeq="\r\n", 
		m4char_t *ai_pcEOPSeq="\f",
		m4char_t ai_cFillChar=' ', 
		m4bool_t ai_bClipColumn=M4_TRUE, 
		m4bool_t ai_bFillPageRows=M4_TRUE,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;


	// Se ejecuta una vez por página txt
	// El resto de parámetros que necesite el Init de PagePrinter se meten a capón dentro 
	//	de InitPage, porque no son necesarios
	m4bool_t InitPage(	m4int16_t ai_iIdPage,
					DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
					ClENG_PagePrinter::ORIENTATION   ai_Orientation,
					ClENG_PagePrinter::PAPER_SIZE     ai_PageSize,
					DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
					ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR);


	//---------------------------Sobrecargas Flow---------------------------------

	m4bool_t PreReset() ;
	void     PostReset() ;

	m4bool_t RefreshLayout();
	m4bool_t FlushLayout() ;
	
	// ---------------------------------Sobrecarga metodos Action-----------------			

	void EOP(m4int16_t ai_iPage) ;
	
	// La PrintStr de la base usa PrintNChar, por lo que no hace falta que la 
	// sobrecargue
	// Debe ver si la cadena a escribir entra en los márgenes o hay que recortarla
	// y soltarlo en el buffer de pag de txt. Si la ignora o la recorta, genera warnings.
	// X,Y se interpretan como top-left
	virtual void PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_X, DvcUnit_t ai_Y, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET, 
		ClENG_OutDevice::StPIParams *ai_pstPI=NULL) ; 	

    
	// ********** Impresiones Pendientes (Pi)

	void  PrintNCharPi(ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
					size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID=ClENG_OutDevice::DVC_NO_INDEX_TARGET);


	void ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr);

	
	DvcUnit_t AdjustHeight( m4int16_t ai_iPage, DvcUnit_t ai_Height ) ;

MIT_PRIVATE:

	// Funcion para crear una lista con los Pi de una pagina concreta
	// que están en memoria. Esta lista tendrá ordenado los pi por Offset.
	// Nos sirve para actualizar los Offset de los Pi en el momento de paso
	// del buffer auxiliar de Txt al buffer que después vuelca directamente
	// a fichero.
	void CreatePagePi(m4int16_t ai_iPage);
	

	void SelectFontDevice( m4int16_t ai_iPage, ClFONT_M_Font *ai_poFont ) {}


	// UNICODE REPORTS
	// Funciones que se sobrecargan en el dispositivo texto ascii para poder imprimir
	// una cadena ascii.
	virtual void _GetStringToPrint(char * ai_pcInString, void *& ao_pvOutString, int & ao_iStringLen, int & ao_iStringWcLen);
	virtual void _GetEOLString(void *& ao_pvEOLString, int & ao_iEOLLen);
	virtual void _GetEOPString(void *& ao_pvEOPString, int & ao_iEOPLen);
	virtual void _GetFillChar(void *& ao_pvFillChar, int & ao_iFillCharLen);
	virtual int _GetSizeOfCharacter();
	virtual char* _GetPrintToString(void * ai_pvInString, int & ao_iStringLen);
};


// =============================ClENG_DvcTxtUnicode========================================

class ClENG_DvcTxtUnicode : public ClENG_DvcTxt
{

MIT_PRIVATE:

	// Secuencia de caracteres a imprimir como salto de linea y como salto de pagina.
	ClMIT_Str m_oEOLSeq;
	ClMIT_Str m_oEOPSeq;

	// Caracter de relleno que sustituira a las ocurrencias de ClENG_PageTxt::m_cFillChar.
	// Normalmente es un caracter en blanco. Un NO_FILL_CHAR no mete ningun caracter, con
	// lo cual el efecto sera una especie de concatenacion de columnas no escritas.
	m4char_t m_cFillChar;

	// Flag que indica si hay que preprocesar la cadena a imprimir para Excel.
	m4bool_t m_bPreprocessForExcel;
	m4char_t m_cFieldDelimiter;

	// Funciones que se sobrecargan en el dispositivo texto unicode para poder imprimir
	// en utf16, una cadena wide char.
	virtual void _GetStringToPrint(char * ai_pcInString, void *& ao_pvOutString, int & ao_iStringLen, int & ao_iStringWcLen);
	virtual void _GetEOLString(void *& ao_pvEOLString, int & ao_iEOLLen);
	virtual void _GetEOPString(void *& ao_pvEOPString, int & ao_iEOPLen);
	virtual void _GetFillChar(void *& ao_pvFillChar, int & ao_iFillCharLen);
	virtual int _GetSizeOfCharacter();
	virtual char* _GetPrintToString(void * ai_pvInString, int & ao_iStringLen);

MIT_PUBLIC:	
	
	// ---------------------- Inicialización -------------------------------------		
	ClENG_DvcTxtUnicode();
	virtual ~ClENG_DvcTxtUnicode();
	virtual size_t GetSizeof() { return sizeof(ClENG_DvcTxtUnicode) ; }

	m4bool_t Init(	

		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, 
		m4bool_t ai_bDelete,
		StENG_DvcParams::DUPLEX_MODE ai_Duplex, 
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		//Especificos del device

		ClFONT_M_DBFont* ai_poDBFont, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		m4uint32_t ai_uiIdFont, 
		m4bool_t ai_bIgnoreTxtWarning,
		m4bool_t ai_bSmallestFont=M4_FALSE,
		m4char_t *ai_pcEOLSeq="\r\n", 
		m4char_t *ai_pcEOPSeq="\f",
		m4char_t ai_cFillChar=' ', 
		m4bool_t ai_bClipColumn=M4_TRUE, 
		m4bool_t ai_bFillPageRows=M4_TRUE,
		m4bool_t ai_bPreprocessForExcel=M4_FALSE,
		m4char_t ai_cFieldDelimiter='\t',

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR ) ;

		virtual m4bool_t IsUTF16();

};

#endif

