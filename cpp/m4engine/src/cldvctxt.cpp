//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvctxt.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:24/09/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández 22/04/98
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


#include "m4string.hpp"

#include "eng.h"
#include "engdf.inl"
#include "cldvctxt.hmg"
#include "cldvctxt.h"

#include "m4unicode.hpp"

//==============================================================================ClENG_PageTxt

const m4char_t ClENG_PageTxt::m_cFillChar=0xFF ;

ClENG_PageTxt::ClENG_PageTxt() 
{
	m_poInsp=GET_INSP();
	m_puiRowEatenAux=NULL ;
}

void ClENG_PageTxt::End() 
{
	M4_DELETE_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_CLDEVICE, m_puiRowEatenAux, m4uint32_t, m_iCols * m_iSizeOfCharacter) ;
}

void ClENG_PageTxt::Init(
				ClENG_PagePrinter::ORIENTATION ai_Orientation, 
				ClENG_PagePrinter::PAPER_SIZE  ai_PageSize, 
				ClENG_PagePrinter::PAPER_SOURCE  ai_PaperSource,
				DvcUnit_t ai_SizeX,   
				DvcUnit_t ai_SizeY,
				ClFONT_M_Font *ai_poFont,
				int ai_iSizeOfCharacter) {

	
	ClENG_PageTxt::End() ;
	m_oPagePrinter.Init(ai_Orientation, 
				ai_PageSize,
				ai_PaperSource,
				ai_SizeX,   
				ai_SizeY);
	
	// UNICODE REPORTS
	m_iSizeOfCharacter = ai_iSizeOfCharacter;

	//Fijarse que los ajustes no usan decimales, se redondean el numero de filas y columnas
	m_iCols = (int) (m_oPagePrinter.GetSizeX()/ai_poFont->GetDvcDesignMaxWidthChar());
	m_iRows = (int) (m_oPagePrinter.GetSizeY()/ai_poFont->GetDvcDesignHeight());
	m_DvcPerCol=ai_poFont->GetDvcDesignMaxWidthChar()  ;
	m_DvcPerRow=ai_poFont->GetDvcDesignHeight()  ;

	m_DvcDifCol = m_DvcPerCol / 2 ;
	m_DvcDifRow = m_DvcPerRow / 2 ;

	m_oBuff.Accommodate(m_iCols * m_iRows * ai_iSizeOfCharacter) ; 
	m_oRowAux.Accommodate( m_iCols * ai_iSizeOfCharacter) ;
	M4_NEW_ARRAY(m_poInsp->m_poMemProf,ENG_MGI_CLDEVICE, m_puiRowEatenAux, m4uint32_t, m_iCols * ai_iSizeOfCharacter) ;
	ClearBuff() ;
}

void ClENG_PageTxt::ClearBuff()
{
	memset( m_oBuff.InChar(0), m_cFillChar, m_iCols * m_iRows * m_iSizeOfCharacter);
	m_iMaxRowPrinted=0 ;
}

//==============================================================================ClENG_DvcTxt

ClENG_DvcTxt::ClENG_DvcTxt(){
	m_poPage = NULL;
	m_poFont = NULL;
}

void ClENG_DvcTxt::End() {

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPage, ClENG_PageTxt, GetNumLayoutPages() ) ;
	ClENG_DvcFile::End() ;
}

m4bool_t ClENG_DvcTxt::Init(	
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
		m4bool_t ai_bSmallestFont,
		m4char_t *ai_pcEOLSeq, 
		m4char_t *ai_pcEOPSeq,
		m4char_t ai_cFillChar, 
		m4bool_t ai_bClipColumn, 
		m4bool_t ai_bFillPageRows,

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )

{
	m4bool_t  Return=M4_TRUE ;
	m4int16_t iIndex;
	ClFONT_M_Font *F ;

	ClENG_DvcTxt::End();
	

	Return=ClENG_DvcFile::Init(
		//Campos para ClENG_OutDevice::Init

		ai_iNumPages,
		ClMIT_Flag::NONE_BITS, NULL,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		ai_bForceMonoBreak, 
		ai_FileParams, ai_bDelete,
		ClENG_BreakList::BREAK_TO_FILE,
		ClENG_DvcFile::GetMappingToFileDuplex(ai_Duplex), 
		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 
		
		ClMIT_Trace::PROPAGATE_ERROR
	);
	if (!Return) goto exit ;

	if (ai_bSmallestFont) {
		F = ai_poDBFont->GetAddressSmallestFont() ;
		if (!F) {
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCTXT_MSG_40 ] ) 
				<< ai_uiIdFont << SEND_MSG ;
			Return=M4_FALSE ;
			goto exit ;
		} ;
		ai_uiIdFont=F->GetId() ;
	} ;
	
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, m_poPage, ClENG_PageTxt, GetNumLayoutPages()) ;
	
	iIndex = ai_poDBFont->FindIndexFont(ai_uiIdFont,M4_FALSE);
	if (iIndex==-1) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCTXT_MSG_40 ] ) 
			<< ai_uiIdFont << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;
	m_poFont = ai_poDBFont->GetAddressFontByIndex(iIndex);
	ai_poDBFont->SetMonoFontMode(m_poFont);

	m_cFillChar=ai_cFillChar ;
	m_oEOLSeq.StrCpy(ai_pcEOLSeq) ;
	m_oEOPSeq.StrCpy(ai_pcEOPSeq) ;
	m_bClipColumn=ai_bClipColumn ;
	m_bFillPageRows=ai_bFillPageRows ;
	m_bIgnoreWarning=ai_bIgnoreTxtWarning ;
	
exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
	
}

m4bool_t ClENG_DvcTxt::InitPage(	m4int16_t ai_iIdPage,
							DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
							ClENG_PagePrinter::ORIENTATION   ai_Orientation,
							ClENG_PagePrinter::PAPER_SIZE    ai_PageSize,
							DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
							ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect
							) 
{
	m4bool_t  Return=M4_TRUE ;
	m4bool_t  initializePhantom ;
	
	if ( !M4_IN_RANGE(ai_iIdPage, 0, GetNumLayoutPages()-1)  ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCTXT_MSG_10 ] ) << SEND_MSG ;
		m_poInsp->m_poTrace->ImmediateError() ;
	} ;

	// UNICODE REPORTS
	m_poPage[ai_iIdPage].Init(	ai_Orientation, ai_PageSize, ClENG_PagePrinter::PRINTER_DEFAULT_BIN, 
		ai_SizeX, ai_SizeY,	m_poFont, _GetSizeOfCharacter() );

	Return=ClENG_DvcFile::InitPage(
		ai_iIdPage, 
		m_poPage[ai_iIdPage].m_oPagePrinter.GetSizeX(),
		m_poPage[ai_iIdPage].m_oPagePrinter.GetSizeY(),
		ai_OffsetX,	ai_OffsetY, m_poPage[ai_iIdPage].GetSizeBuff(), initializePhantom,
		ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!Return) goto exit ;

	// UNICODE REPORTS
	if ( initializePhantom )  {
		m_poPage[ GetNumLayoutPages()-1 ].Init( 
			ai_Orientation, ai_PageSize, ClENG_PagePrinter::PRINTER_DEFAULT_BIN, 
			ai_SizeX, ai_SizeY, m_poFont, _GetSizeOfCharacter() );
	} ;
	
exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}


m4bool_t ClENG_DvcTxt::PreReset() 
{
	m4bool_t SwOk=M4_TRUE ;
	
	//------->Abre y prepara ficheros siempre (no hay ATOMIC mode)

	SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak() );	
	if (!SwOk) goto exit ;

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;
	
exit:

	return SwOk ;
}

void     ClENG_DvcTxt::PostReset() 
{
	//---------->Cierra ficheros (no hay ATOMIC mode)

	m_oOutRepository.CloseBreak( IsValidOutput() ) ;

	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE) ;
}



m4bool_t  ClENG_DvcTxt::RefreshLayout() 
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Notifica a FileRep (no abre files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.NewFlush() ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::RefreshLayout() ;
}


m4bool_t ClENG_DvcTxt::FlushLayout() 
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Corre EOPs

	RunAllEOP() ;	//En EOP, proceso de actualización de buffers

	//---------->Vuelca buffers a files  (no cierra files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.CloseFlush( ClENG_DvcFile::m_poBuffArray ) ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::FlushLayout();
}


void ClENG_DvcTxt::_GetStringToPrint(char * ai_pcInString, void *& ao_pvOutString, int & ao_iStringLen, int & ao_iStringWcLen)
{
	ao_iStringLen = strlen(ai_pcInString);
	ao_pvOutString = M4CppToANSI(ai_pcInString, ao_iStringLen);
	ao_iStringWcLen = ao_iStringLen;
}


void ClENG_DvcTxt::_GetEOLString(void *& ao_pvEOLString, int & ao_iEOLLen)
{
	char * pcEOLString = NULL;
	ao_iEOLLen = strlen(m_oEOLSeq);

	pcEOLString = new char[ao_iEOLLen + 1];
	if (pcEOLString == NULL)
	{
		ao_pvEOLString = NULL;
		ao_iEOLLen = 0;
		return;
	}

	strcpy(pcEOLString, m_oEOLSeq);
	ao_pvEOLString = pcEOLString;
}

void ClENG_DvcTxt::_GetEOPString(void *& ao_pvEOPString, int & ao_iEOPLen)
{
	char * pcEOPString = NULL;
	ao_iEOPLen = strlen(m_oEOPSeq);

	pcEOPString = new char[ao_iEOPLen + 1];
	if (pcEOPString == NULL)
	{
		ao_pvEOPString = NULL;
		ao_iEOPLen = 0;
		return;
	}

	strcpy(pcEOPString, m_oEOPSeq);
	ao_pvEOPString = pcEOPString;
}

void ClENG_DvcTxt::_GetFillChar(void *& ao_pvFillChar, int & ao_iFillCharLen)
{
	char * pcFillchar = NULL;
	ao_iFillCharLen = 1;

	pcFillchar = new char[ao_iFillCharLen + 1];
	if (pcFillchar == NULL)
	{
		ao_pvFillChar = NULL;
		ao_iFillCharLen = 0;
		return;
	}

	sprintf(pcFillchar, "%c", m_cFillChar);
	ao_pvFillChar = pcFillchar;
}

int ClENG_DvcTxt::_GetSizeOfCharacter()
{
	return sizeof(char);
}

char* ClENG_DvcTxt::_GetPrintToString(void * ai_pvInString, int & ao_iStringLen)
{
	return M4ANSIToCpp((char *)ai_pvInString, ao_iStringLen);
}


void ClENG_DvcTxt::PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID,
		ClENG_OutDevice::StPIParams *ai_pstPI) 
{
	//Dvcunit to fila o columna
	m4uint16_t iRow;
	m4uint16_t iCol;
	m4char_t   *Beg, *BAux ;
	size_t lValueOffset = 0, TestOver;
	size_t ClipNChar=0 ;
	ClENG_PageTxt *Page= &m_poPage[ai_iPage] ;

	// UNICODE REPORTS
	// Bug 0125206. En los campos multilínea te llega el contenido del campo completo
	// pero en el número de caracteres te llega los que realmente quiere imprimir.
	void * pvString = NULL;
	m4pchar_t pcString = NULL;
	int iLen = 0;
	int iWcLen = 0;
	m4pchar_t pcFileString = NULL;
	int iAuxLen;
	size_t iCharLength;
	int iCharPos;

	void * pvFillChar = NULL;
	int iFillCharLength = 0;

	int iSizeOfCharacter = _GetSizeOfCharacter();
	
	_GetFillChar(pvFillChar, iFillCharLength);

	iCharPos = ai_szNChar ;
	pcString = new m4char_t[iCharPos + 1];
	memcpy(pcString, ai_pcStr, iCharPos);
	pcString[iCharPos] = '\0';

	ai_iX += GetOffsetX(ai_iPage) ;
	ai_iY += GetOffsetY(ai_iPage) ;
		
	iCol = ai_iX/Page->m_DvcPerCol;
	if (iCol < Page->m_iCols-1  && ai_iX % Page->m_DvcPerCol > Page->m_DvcDifCol)
	{
		++iCol;
	}

	iRow = ai_iY/m_poPage[ai_iPage].m_DvcPerRow;
	if (iRow < Page->m_iRows-1  && ai_iY % Page->m_DvcPerRow > Page->m_DvcDifRow)
	{
		++iRow;
	}

	// UNICODE REPORTS.
	// Convertir una cadena (multi byte) a multibyte ansi. Si estamos en ansi, te devuelve
	// ansi, es decir, esa misma cadena. Si estamos en unicode, pasa de utf8 a ansi.
	// Bug 0126376. Para comprobar las longitudes de los campos en diseño, se necesita
	// la longitud de la cadena a imprimir en multibyte.
	_GetStringToPrint(pcString, pvString, iLen, iWcLen);
	
	if ((iCol >= Page->m_iCols) || (iRow >= Page->m_iRows)) 
	{
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCTXT_MSG_20 ] ) 
			<< ai_iPage << ai_iX << ai_iY << ClMIT_StrNLink(pcString,iCharPos) << SEND_MSG ;
		//Efecto local

	}
	else
	{

		Page->m_iMaxRowPrinted = M4_MAX(Page->m_iMaxRowPrinted,iRow+1) ;

		if ( iCol+iLen > Page->m_iCols )
		{
			// AntonioADN. Bug 164100
			// Se estaba realizando incorrectamente el recorte de las cadenas
			// - Se estaban tomando n bytes como n caracteres. Esto es correcto
			//     en entorno ANSI pero no así en Unicode. En este caso podría
			//     generarse un UTF8 mal formado o recortar caracteres de más.
			// - No se estaba actualizando la logitud de salida de la cadena
			//   Esto provocaba que se imprimiera la cadena completa
			size_t Ant=iCharPos ;
			iLen = Page->m_iCols-iCol ;
			iCharPos = M4CharPosition( pcString, -1, iLen, iCharLength ) ;
			iWcLen = iLen * _GetSizeOfCharacter() ;

			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCTXT_MSG_30 ] ) 
				<< ai_iPage << ai_iX << ai_iY 
				<< ClMIT_StrNLink(pcString,Ant) << ClMIT_StrNLink(pcString,iCharPos) << SEND_MSG ;
			//Efecto local
		} ;

		lValueOffset = iRow*Page->m_iCols+iCol;

		// UNICODE REPORTS
		lValueOffset = lValueOffset * iSizeOfCharacter;
		
		Beg =  Page->m_oBuff.InChar(lValueOffset) ;

		// Test efecto local de sobreescritura
		for ( BAux=Beg, TestOver=iWcLen ; TestOver ; TestOver -= iSizeOfCharacter, BAux += iSizeOfCharacter )
		{
			if (*BAux!=ClENG_PageTxt::m_cFillChar)
			{
				break ;
			}
		} ;

		if ( TestOver != 0 && !m_bIgnoreWarning)
		{
			//Para que mensaje salga 'bonito'
			iAuxLen = iWcLen;

			for ( BAux=Beg; iAuxLen ; iAuxLen -= iSizeOfCharacter, BAux += iSizeOfCharacter )
			{
				if (*BAux==ClENG_PageTxt::m_cFillChar)
				{
					memcpy( BAux, pvFillChar, iFillCharLength ) ;
				}
			} ;

			// Se pasa del formato de archivo (Txt ANSI o Unicode) a formato CPP (ANSI o UTF-8)
			iAuxLen = iLen;
			pcFileString = _GetPrintToString(Beg, iAuxLen);

			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCTXT_MSG_50 ] ) 
				<< ai_iPage << ai_iX << ai_iY 
				<< ClMIT_StrNLink(pcString,iCharPos) << pcFileString << SEND_MSG;

			delete [] pcFileString;
		} ;

		// UNICODE REPORTS
		memcpy(Beg, pvString, iWcLen);

		ClipNChar=iLen;
	} ;

	delete [] pvString;
	delete [] pcString;

	delete [] pvFillChar;

	if ( ai_pstPI ) {
		ai_pstPI->m_uiOff=lValueOffset ;
		ai_pstPI->m_szNChars=ClipNChar ;
	} ;
}



DvcUnit_t ClENG_DvcTxt::AdjustHeight( m4int16_t ai_iPage, DvcUnit_t ai_Height ) 
{
	DvcUnit_t Resto=ai_Height%m_poPage[ai_iPage].m_DvcPerRow  ;
	
	return ai_Height + (Resto ? m_poPage[ai_iPage].m_DvcPerRow-Resto : 0 )  ;
}

void ClENG_DvcTxt::PrintNCharPi(ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
								size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID)
{
	ClENG_OutDevice::StPIParams PIParams ;
	ClMIT_Str StrAux ;

	// Creo la cadena con tantas X como ai_sNChar indica
	StrAux.StrDuplicateChar(0,ai_szNChar,'X') ;

	// Imprimo la cadena y tomo el offset
	PIParams.m_oIdPi.StrCpy(ai_poIdPi->InChar(0));
	PrintNChar(ai_iPage,ai_iX,ai_iY,StrAux,ai_szNChar,ai_bRawText,ai_pcHyperlink,ai_iTargetID,&PIParams);

	// Añado el Pi a la lista
	if (PIParams.m_szNChars) {
		m_oOutRepository.AddNewPi( ai_poIdPi->InChar(0), ai_iPage, 
			PIParams.m_uiOff, PIParams.m_szNChars ) ;
	} ;
}


void ClENG_DvcTxt::ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr) 
{
	StPi_t piData ;

	m4uint32_t Len;
	m4uint32_t PosActual;
	m4char_t * Beg;
	ClMIT_Str StrAux ;

	void * pwcString = NULL;
	m4int_t iLen = 0;
	m4int_t iWcPIBytes = 0;
	m4int_t iByteLength = 0;
	m4int_t iCharLength = 0;
	m4int_t iByteTotal = 0;

	// Tomo el Pi a imprimir ( le digo que imprima el error si no lo encuentra)

	if ( m_oOutRepository.ExtractPi( ai_poIdPi->InChar(0),  &piData ) )
	{
		iByteLength = strlen(ai_pcStr);
		iCharLength = M4StrLen(ai_pcStr, iByteLength);

	    if (iCharLength > piData.m_NBytes)
		{
			// Sacar mensaje de que se va a trucar
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCTXT_MSG_60 ] ) 
				<< (m4int16_t)piData.m_NPage << (m4uint32_t)piData.m_NBytes 
				<< ai_pcStr << SEND_MSG ;
		   //Efecto Local
		}
			
		// Creo la cadena a imprimir
		// -- si sobra espacion relleno con blancos hasta PiNBytes 
		// -- si falta se trunca
		// UNICODE REPORTS. Bug 163475
		//  Se está trabajando con un array de bytes. En el caso de un entorno ANSI
		// iByteLength e iByteChar siempre seran iguales. En el caso de un entorno Unicode
		// la cadena será UTF8 y si hay algún caracter con ASCII equivalente mayor a 127 utilizará
		// más de un byte por caracter siendo iByteLength mayor que iByteChar. Por lo tanto, se añade
		// los bytes necesarios (iByteLength - iByteChar) para asegurar que la cadena ANSI o UTF16 resultado
		// tenga piData.m_NBytes caracteres.
		// (Sin este reajuste en un entorno Unicode, y con la casuística indicada, se obtiene una
		//  cadena con un tamaño inferior provocando que aparezcan caracteres no representables
		//  en el visor)
		iByteTotal = piData.m_NBytes + iByteLength - iCharLength;
		StrAux.Accommodate( iByteTotal +1 ) ;
 		Len = sprintf(StrAux.InChar(0),"%-*.*s", iByteTotal , iByteTotal , ai_pcStr);
		StrAux.OverStringValid() ;	//No haria falta, se pone por si acaso
		
		// UNICODE REPORTS
		// Convertir de Cpp a UTF16 o ANSI

		iLen = strlen(StrAux.InChar(0));
		_GetStringToPrint(StrAux.InChar(0), pwcString, (int&)iLen, (int&)iWcPIBytes); 

		// Imprimo la cadena asociada en RAM o en fichero, según dónde de encuentre el Pi
		if ( piData.m_Place == StPi_t::PI_IN_RAM) {  
			
			// Imprimir en el buffer
			Beg =  m_poPage[ piData.m_NPage ].m_oBuff.InChar( piData.m_Offset ) ;
			memcpy( Beg, pwcString, iWcPIBytes );

		} else { 
			
			// Imprimir directamente en el fichero
			m4bool_t bWasClosed=M4_FALSE;

			//Tomo la posicion actual del fichero, y me posiciono en el offset
			if(  ! piData.m_poFile->IsOpen() ) {
				bWasClosed=M4_TRUE;
				piData.m_poFile->Open( ClMIT_File::READ_WRITE, ClMIT_Trace::PROPAGATE_ERROR) ;
			};

			PosActual = piData.m_poFile->GetPos();
			piData.m_poFile->SetPos( piData.m_Offset );

			// Imprimo y dejo en fichero en la posicion inicial
			piData.m_poFile->W( pwcString, iWcPIBytes);
			piData.m_poFile->SetPos(PosActual);
			if(bWasClosed){
				//Solo lo cierro si ya lo estaba
				piData.m_poFile->Close();
			}	
		}

		delete [] pwcString;
  };

  return;
}


void ClENG_DvcTxt::CreatePagePi(m4int16_t ai_iPage)
{
	ClENG_PI_Manager::PI_List_t::iterator it, itEnd ;
	StPi_t *pi ;
	

	m_oOutRepository.GetPiIterators( it, itEnd )  ;

	
	for ( it; it!=itEnd; it++) {
		pi = (*it).second ;

		if ( pi->m_Place == StPi_t::PI_IN_RAM && pi->m_NPage == ai_iPage  ) {
			m_TxtMultiMapPi.insert( TxtMultiMapPi::value_type( pi->m_Offset, pi) );
		}
	} ;
	return;
}


void ClENG_DvcTxt::EOP(m4int16_t ai_iPage)
{
	m4char_t *BegRow, *Pos, *PosRowAux ;
	m4uint32_t NextOff, ThisOff, ColRowAux, *PosEatenRow, Eaten ; 

	//NOTA: estos valores se hacen signed para posibilitar -1 en salidas de bucles
	//Ultima columna y longitud de la columna con chars validos
	m4int32_t LastColumn, LenVColumn ;	
	m4int32_t LastRow ;	//Ultima Row a procesar
	m4int32_t Row, Col, XAux ;
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;

	TxtMultiMapPi::iterator itListAuxPi;
	TxtMultiMapPi::iterator itListAuxPi2;
	StPi_t *pi ;
	m4uint32_t offTxt ;


	// UNICODE REPORTS
	void * pvEOLSeq = NULL;
	void * pvEOPSeq = NULL;
	void * pvFillChar = NULL;
	int iEOLSeqLength = 0;
	int iEOPSeqLength = 0;
	int iFillCharLength = 0;
	
	_GetEOLString(pvEOLSeq, iEOLSeqLength);
	_GetEOPString(pvEOPSeq, iEOPSeqLength);
	_GetFillChar(pvFillChar, iFillCharLength);
	
	int iSizeOfCharacter = _GetSizeOfCharacter();

	// Creo una lista con los Pi de esa pagina 
	CreatePagePi(ai_iPage);
    
	LastRow =  m_bFillPageRows ? m_poPage[ai_iPage].m_iRows-1 : m_poPage[ai_iPage].m_iMaxRowPrinted-1 ;
	LastColumn = m_poPage[ai_iPage].m_iCols-1  ;
	
	BegRow = m_poPage[ai_iPage].m_oBuff.InChar(0);

	for (Row=0;Row<=LastRow;++Row)
	{
		//Primero creamos los arrays auxiliares, pues e usan en el proceso a PI
		for (
			Col=0, LenVColumn=0, Pos=BegRow, 
			PosRowAux=m_poPage[ai_iPage].m_oRowAux.InChar(0), ColRowAux=0,	//Se actualizan a la par
			PosEatenRow=m_poPage[ai_iPage].m_puiRowEatenAux, Eaten=0; 
			Col<=LastColumn; ++Col, ++PosEatenRow) 
		{
			// UNICODE REPORTS
			if (*Pos==ClENG_PageTxt::m_cFillChar)
			{
				if ( m_cFillChar != NO_FILL_CHAR )
				{
					memcpy(PosRowAux, pvFillChar, iFillCharLength);
					PosRowAux += iFillCharLength;
					ColRowAux++;
				} 
				else 
				{
					Eaten++;
				}
			}
			else
			{
				// LenVColumn no se actualiza hasta no encontrar un char valido 
				// (asi no cuenta secuencias finales de caracteres de relleno).
				// Se refiere a la longitud del m_oRowAux a imprimir.

				// UNICODE REPORTS
				// En el caso del dispositivo unicode, el tamaño del caracter será 2
				// y en el dispositivo ansi, el tamaño del caracter es 1.
				memcpy(PosRowAux, Pos, iSizeOfCharacter);
				LenVColumn = ColRowAux + 1;	
				PosRowAux = PosRowAux + iSizeOfCharacter;
				ColRowAux++;
			} 

			*PosEatenRow = Eaten ;

			// En el caso del dispositivo unicode, se incrementa de dos en dos
			// los caracteres cuando se leen. En el dispositivo ansi, se incrementan
			// de uno en uno.
			Pos = Pos + iSizeOfCharacter;
		} //Al salir, ColRowAux es igual a la longitud  escrita en m_oRowAux

		PosEatenRow = m_poPage[ai_iPage].m_puiRowEatenAux; //Para usarlo luego
		PosRowAux = m_poPage[ai_iPage].m_oRowAux.InChar(0);
		
		if ( !m_TxtMultiMapPi.empty()) {
			ThisOff= (Row  )*m_poPage[ai_iPage].m_iCols * iSizeOfCharacter;
			NextOff= (Row+1)*m_poPage[ai_iPage].m_iCols * iSizeOfCharacter;
			//Actualizacion de PIs pendientes
			for( itListAuxPi = m_TxtMultiMapPi.begin(); 
				itListAuxPi!= m_TxtMultiMapPi.end(); ) 	
			{
				offTxt = (*itListAuxPi).first ;
				pi = (*itListAuxPi).second ;

				//Son PIs de la linea que estamos procesando ?
				if ( offTxt < NextOff ) {
					//El offset es lo que haya escrito en el buffer hasta el momento, 
					//	mas la coordenada X del caracter, menos los bytes comidos por 
					//	NO_FILL_CHAR, para esa posicion X.
					XAux = pi->m_Offset-ThisOff ; 
					pi->m_Offset = PageBuffer->GetTotal() + XAux - PosEatenRow[XAux/iSizeOfCharacter] * iSizeOfCharacter ;
					
					//Borramos y nos movemos al siguiente 
					itListAuxPi2 = itListAuxPi;
					itListAuxPi2++;
					m_TxtMultiMapPi.erase(itListAuxPi);
					itListAuxPi = itListAuxPi2;

				} else break;	//Nos salimos, el resto de la lista tiene lineas mayores
			};
		} ;

		// UNICODE REPORTS
		PageBuffer->Append( PosRowAux, m_bClipColumn ? LenVColumn * iSizeOfCharacter: ColRowAux * iSizeOfCharacter);
		PageBuffer->Append(pvEOLSeq, iEOLSeqLength);

		BegRow=Pos ;
	} ;

	// UNICODE REPORTS
	PageBuffer->Append(pvEOPSeq, iEOPSeqLength);

	delete [] pvEOLSeq;
	delete [] pvEOPSeq;
	delete [] pvFillChar;

	m_poPage[ai_iPage].ClearBuff();
	ClENG_DvcFile::EOP(ai_iPage);
}

// =============================ClENG_DvcTxtUnicode========================================

ClENG_DvcTxtUnicode::ClENG_DvcTxtUnicode()
{
}

ClENG_DvcTxtUnicode::~ClENG_DvcTxtUnicode()
{
}

m4bool_t ClENG_DvcTxtUnicode::Init(	

		// Campos para ClENG_OutDevice::Init
		m4int16_t ai_iNumPages,
		
		// Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)
		m4bool_t ai_bForceMonoBreak,
		StFileParams *ai_FileParams, 
		m4bool_t ai_bDelete,
		StENG_DvcParams::DUPLEX_MODE ai_Duplex, 
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		// campos para el device ClENG_DvcTxt
		ClFONT_M_DBFont* ai_poDBFont, 
		StENG_DvcParams::TUMBLE_MODE  ai_Tumble,
		m4uint32_t ai_uiIdFont, 
		m4bool_t ai_bIgnoreTxtWarning,
		m4bool_t ai_bSmallestFont,
		m4char_t *ai_pcEOLSeq, 
		m4char_t *ai_pcEOPSeq,
		m4char_t ai_cFillChar, 
		m4bool_t ai_bClipColumn, 
		m4bool_t ai_bFillPageRows,
		m4bool_t ai_bPreprocessForExcel,
		m4char_t ai_cFieldDelimiter,


		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t bReturn = M4_FALSE;
	ClFONT_M_Font * F = NULL;

	m_oEOLSeq.StrCpy(ai_pcEOLSeq);
	m_oEOPSeq.StrCpy(ai_pcEOPSeq);
	m_cFillChar = ai_cFillChar;
	m_bPreprocessForExcel = ai_bPreprocessForExcel;
	m_cFieldDelimiter = ai_cFieldDelimiter;

	bReturn = ClENG_DvcTxt::Init(ai_iNumPages, ai_bForceMonoBreak, ai_FileParams, ai_bDelete, 
		ai_Duplex, ai_DeclareExtFile, ai_bCreateDir, ai_Preserve, ai_pcToPrinter, ai_poDBFont, 
		ai_Tumble, ai_uiIdFont, ai_bIgnoreTxtWarning, ai_bSmallestFont, ai_pcEOLSeq, ai_pcEOPSeq, 
		ai_cFillChar, ai_bClipColumn, ai_bFillPageRows, ai_ErrorEffect);

	return 	bReturn;
}


void ClENG_DvcTxtUnicode::_GetStringToPrint(char * ai_pcInString, void *& ao_pcOutString, int & ao_iStringLen, int & ao_iStringWcLen)
{
	m4bool_t	bEscapeChars = M4_FALSE;
	m4uint32_t	i = 0;
	m4uint32_t	iLength = 0;
	m4char_t	c = '\0';
	m4char_t	cFieldDelimeter = '\0';
	string		sScapedInString;
	
	if (m_bPreprocessForExcel == M4_TRUE)
	{
		// Entrecomillar la cadena en el caso de que contenga retornos de carro.
		// En este caso, si contiene comillas dobles, escapar.
		iLength = strlen(ai_pcInString);
		for (i = 0 ; i < iLength; i++)
		{
			c = ai_pcInString[i];
			if (c == '\n' || c == '\r')
			{
				bEscapeChars = M4_TRUE;
				break;
			}
			else if ((m_cFieldDelimiter != NO_FIELD_DELIMITER_CHAR) && (c == m_cFieldDelimiter && i != iLength - 1))
			{
				bEscapeChars = M4_TRUE;
				break;
			}
			// Bug 0262424: escapar comillar solo en el caso de que no se especifique delimitador.
			// Caso de la salida a excel normal.
			else if (c == '"' && (m_cFieldDelimiter != NO_FIELD_DELIMITER_CHAR))
			{
				bEscapeChars = M4_TRUE;
				break;
			}
		}

		if (bEscapeChars == M4_TRUE)
		{
			sScapedInString.assign("\"");
			for (i = 0 ; i < iLength; i++)
			{
				c = ai_pcInString[i];
				if (c == '"')
				{
					// Escapar dobles comillas.
					sScapedInString.append("\"\"");
				}
				else if (c == '\r' && ai_pcInString[i + 1] == '\n')
				{
					// Reemplazar CRLF por LF.
					sScapedInString.append("\n");
					i++;
				}
				else if (c == '\r')
				{
					// Reemplazar CR por LF.
					sScapedInString.append("\n");
				}
				else
				{
					sScapedInString.append(&c, 1);
				}
			}

			if (m_cFieldDelimiter != NO_FIELD_DELIMITER_CHAR)
			{
				cFieldDelimeter = m_cFieldDelimiter;
			}
			else
			{
				cFieldDelimeter = '\t';
			}

			// La cadena viene tabulada, incluso al final. Sólo hay que entrecomillar el
			// contenido real del campo y dejar fuera el tabulador.
			iLength = sScapedInString.size();
			if (sScapedInString[iLength - 1] == cFieldDelimeter)
			{
				sScapedInString.assign(sScapedInString.substr(0, iLength - 1) + '"' + cFieldDelimeter);
			}
			else
			{
				sScapedInString.append("\"");
			}
		}
		else
		{
			sScapedInString = ai_pcInString;
		}
	}
	else
	{
		sScapedInString = ai_pcInString;
	}
	
	ao_iStringLen = sScapedInString.size();
	ao_pcOutString = M4CppToReports(sScapedInString.c_str(), ao_iStringLen);

	ao_iStringWcLen = ao_iStringLen * sizeof (unsigned short);
}

void ClENG_DvcTxtUnicode::_GetEOLString(void *& ao_pvEOLString, int & ao_iEOLLen)
{
	ao_iEOLLen = strlen(m_oEOLSeq);
	ao_pvEOLString = M4CppToReports(m_oEOLSeq, ao_iEOLLen);
	ao_iEOLLen = ao_iEOLLen * sizeof (unsigned short);
}

void ClENG_DvcTxtUnicode::_GetEOPString(void *& ao_pvEOPString, int & ao_iEOPLen)
{
	ao_iEOPLen = strlen(m_oEOPSeq);
	ao_pvEOPString = M4CppToReports(m_oEOPSeq, ao_iEOPLen);
	ao_iEOPLen = ao_iEOPLen * sizeof (unsigned short);
}

void ClENG_DvcTxtUnicode::_GetFillChar(void *& ao_pvFillChar, int & ao_iFillCharLen)
{
	m4char_t szFillChar[2];
	sprintf(szFillChar, "%c", m_cFillChar);

	ao_iFillCharLen = strlen(szFillChar);

	ao_pvFillChar = M4CppToReports(szFillChar, ao_iFillCharLen);
	ao_iFillCharLen = ao_iFillCharLen * sizeof (unsigned short);
}

int ClENG_DvcTxtUnicode::_GetSizeOfCharacter()
{
	return sizeof(unsigned short);
}

char* ClENG_DvcTxtUnicode::_GetPrintToString(void * ai_pvInString, int & ao_iStringLen)
{
	return M4ReportsToCpp((M4XMLCh*)ai_pvInString, ao_iStringLen);
}

m4bool_t ClENG_DvcTxtUnicode::IsUTF16()
{
	return M4_TRUE;
}