//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcrvw.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:24/11/97
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



#include "m4string.hpp"

#include "eng.h"
#include "engdf.inl"
#include "cldvcrvw.hmg"
#include "cldvcrvw.h"

#include "m4unicode.hpp"

//----------------------------Macros RVW

// Para cualquier Block (Offsets relativos al comienzo de grupo)

#define SIZE_HEADER_BLOCK ( sizeof(m4char_t) + 2 * sizeof(m4int32_t) )
#define BLOCK_OFF_PREVIOUS ( sizeof(m4char_t) )
#define BLOCK_OFF_NEXT     ( sizeof(m4char_t) + sizeof(m4int32_t) )

// Bloque BOF

#define ID_RVW_STRING "Report Viewer Format Meta4"

// UNICODE REPORTS
// Incrementamos la versión del visor para el desarrollo de unicode, rompe
// compatibilidad.
// Bug 0125064
#define RVW_VERSION 6

#define SIZE_BOF_BLOCK    ( SIZE_HEADER_BLOCK + 1*sizeof(m4char_t) +2 * sizeof(m4int16_t) + 2*sizeof(m4int32_t) + sizeof(m4int32_t) + strlen(ID_RVW_STRING) )
#define FILE_OFF_BOF_PAGES_PRINTED (SIZE_BOF_BLOCK-2*sizeof(m4int32_t))
#define FILE_OFF_BOF_GROUPS_PRINTED (SIZE_BOF_BLOCK-sizeof(m4int32_t))

// Bloque BOB

#define SIZE_BOB_BLOCK ( SIZE_HEADER_BLOCK  + 5*sizeof(m4int32_t) )
#define BLOCK_OFF_BOB_NEXT ( SIZE_HEADER_BLOCK + 2*sizeof(m4int32_t) ) 
#define BLOCK_OFF_BOB_PAGES_PRINTED ( SIZE_HEADER_BLOCK + 3*sizeof(m4int32_t) ) 

// Bloque EOB

#define SIZE_EOB_BLOCK ( SIZE_HEADER_BLOCK )

// Bloque Page
//	Los tamaños no computan el bloque de tags, pero si el tag 'E'

#define SIZE_NORMAL_PAGE_BLOCK       ( SIZE_HEADER_BLOCK+2*sizeof(m4char_t)+sizeof(m4int32_t) ) 
#define SIZE_EXTENDED_PAGE_BLOCK     ( SIZE_HEADER_BLOCK+4*sizeof(m4char_t)+3*sizeof(m4int32_t) ) 


#define BLOCK_OFF_PAGE_NUMBER ( SIZE_HEADER_BLOCK + sizeof(m4char_t))

//Otros valores internos

#define GRAPHIC_TAG_ERROR -1L

//----------------------------Metodos 'normales'

ClENG_DvcRVW::ClENG_DvcRVW(){
	m_poPagePrinter=NULL;
}

void ClENG_DvcRVW::End() {

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poPagePrinter, ClENG_PagePrinter, GetNumLayoutPages() 
	) ;

	ClENG_DvcFile::End() ;
}

m4bool_t ClENG_DvcRVW::Init( 
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		StFileParams *ai_FileParams, m4bool_t ai_bDelete,
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter, 

		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect ) 
{
	m4bool_t Return=M4_TRUE ;
	
	ClENG_DvcRVW::End();
	m_BAdapter.Init() ;

	//En este device NO se permite el UseDelBreak

	ai_FileParams->m_bUseDelBreak=M4_FALSE ;

	Return = ClENG_DvcFile::Init( 
		//Campos para ClENG_OutDevice::Init

		ai_iNumPages,
		
		ClENG_OutDevice::PRELOAD_ON_DEVICE | 
		ClENG_OutDevice::COLOUR_SUPPORTED | 
		ClENG_OutDevice::COLOUR_ACTIVATED | 
		ClENG_OutDevice::RASTER_SUPPORTED, 

		ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		M4_TRUE,	//SIEMPRE ES MONOBREAK A NIVEL DE FICHERO
		ai_FileParams, ai_bDelete,
		ClENG_BreakList::BREAK_TO_FILE,
		ClENG_DvcFile::GetMappingToFileDuplex(StENG_DvcParams::NO_DUPLEX), 

		ai_DeclareExtFile,
		ai_bCreateDir,
		ai_Preserve, 
		ai_pcToPrinter, 

		ClMIT_Trace::PROPAGATE_ERROR
	);

	
	if (!Return) goto exit ;

	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poPagePrinter, ClENG_PagePrinter, GetNumLayoutPages() 
	) ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		

}

m4bool_t ClENG_DvcRVW::InitPage(	m4int16_t ai_iIdPage,
				DvcUnit_t ai_OffsetX, DvcUnit_t ai_OffsetY, 
				size_t ai_szBlockBuffer, 
				ClENG_PagePrinter::ORIENTATION   ai_Orientation,
				ClENG_PagePrinter::PAPER_SIZE    ai_PageSize,
				DvcUnit_t ai_SizeX,   DvcUnit_t ai_SizeY,
				ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect)
{
	m4bool_t Return=M4_TRUE ;
	m4bool_t initializePhantom ;

	if ( !M4_IN_RANGE(ai_iIdPage, 0, GetNumLayoutPages()-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCRVW_MSG_20 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;

	m_poPagePrinter[ai_iIdPage].Init( ai_Orientation, ai_PageSize, ClENG_PagePrinter::LOWER_BIN, 
		ai_SizeX, ai_SizeY);

	Return=ClENG_DvcFile::InitPage( 
		ai_iIdPage, 
		m_poPagePrinter[ai_iIdPage].GetSizeX(),
		m_poPagePrinter[ai_iIdPage].GetSizeY(),
		ai_OffsetX, ai_OffsetY, 
		ai_szBlockBuffer, initializePhantom,
		ClMIT_Trace::PROPAGATE_ERROR) ;
	if (!Return) goto exit ;

	if ( initializePhantom )  {
		m_poPagePrinter[ GetNumLayoutPages()-1 ].Init( 
			ai_Orientation, ai_PageSize, ClENG_PagePrinter::LOWER_BIN, 
			ai_SizeX, ai_SizeY);
	} ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

m4bool_t ClENG_DvcRVW::PreReset()
{
	m4bool_t  SwOk=M4_TRUE ;
	m4int16_t Page ;
	m4uint32_t PrevBlock, NextBlock, Pos ;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	//NOTA: Se esta escribiendo a m_oLayoutBuff

	//-------------->Primer break, tareas EXCLUSIVAS de inicializacion del fichero

	if ( GetBreakCount()==0 ) {

		//------->Abre y prepara ficheros siempre (no hay ATOMIC mode)

		SwOk = M4_BOOL( SwOk && m_oOutRepository.NewBreak() );	
		if (!SwOk) goto exit ;
	
		//El mismo fichero para todas las paginas layout (siempre modo SHARING)
		F = m_oOutRepository.GetFlushFile(0, &swRedirect ) ;

		//------>Datos que solo van una vez al principio del fichero (o ficheros)

		//--->Bloque cabecera, se escribe a fichero directamente
		//	Pendiente de actualizar PagesPrinted y  GroupsPrinted en End()

		WFBlockHeader(	F,'B',-1, SIZE_BOF_BLOCK ) ;
		WFChar(			F, (m4char_t)m_BAdapter.GetFileStorage() ) ;
		WFString(		F, ID_RVW_STRING ) ;
		WFInt(			F, RVW_VERSION ) ;				
		WFInt(			F, GetNumLayoutPages() ) ;
		WFLong(			F, 0) ;	//PagesPrinted
		WFLong(			F, 0) ;	//GroupsPrinted

		for (Page=0, PrevBlock=0; Page<GetNumLayoutPages(); ++Page, PrevBlock=m_iLastBlock ) {

			// --->Bloque paginas Optimized Preload, se escriben una detras de otra

			m_iLastBlock = F->GetPos() ;
			NextBlock= m_iLastBlock + SIZE_EXTENDED_PAGE_BLOCK+ 
				m_poLayoutPages[Page].m_oOpPrBuff.GetTotal() +
				m_poLayoutPages[Page].m_oPrBuff.GetTotal() ;

			WFBlockHeader(F,'P',PrevBlock, NextBlock ) ;
			WFChar(F, 1 ) ; //Con Extended info
			WFLong(F, Page+1) ;	//PageNumber
			WFLong(F, ClENG_OutDevice::GetSizeX(Page)) ;
			WFLong(F, ClENG_OutDevice::GetSizeY(Page)) ;
			WFChar(F, (m4char_t)m_poPagePrinter[Page].GetOrientation() ) ;//Orientacion
			WFChar(F, (m4char_t)m_poPagePrinter[Page].GetPageSize()) ;	//Tamaño

			//Vuelco a fichero bloque  de tags Optimized Preload

			F->DumpBuffer( &m_poLayoutPages[Page].m_oOpPrBuff ) ;

			//Vuelco a fichero bloque  de tags Preload. No tiene sentido almacenarlos en
			//	buffer preserve, al no tener limitaciones para dejarlos aqui.

			F->DumpBuffer( &m_poLayoutPages[Page].m_oPrBuff ) ;

			WFChar(F,'E') ;	//Marca fin de tags
		} ;

		m_uiOffActBOB = -1 ;
		// m_iLastBlock ya se actualizo en el bucle anterior


		for (Page=0 ; Page<GetNumLayoutPages(); ++Page ) {

			// --->Ahora meto los datos Preserve.
			//	Se queda metida la cabecera de bloque de pagina y los tags Preload. 
			//	Queda pendiente:
			//		-Actualizar los PrevBlock, NextBlock y PageNumber.
			//		-Meter el resto de tags-
			//		-Meter marca de fin de bloque

			WBBlockHeader(Page,'P',0,0) ; 
			WBChar(Page, 0) ;	//Sin Extended Info
			WBLong(Page, 0) ;	//PageNumber		
		} ;
		

	} else {

		//El mismo fichero para todas las paginas layout
		F = m_oOutRepository.GetFlushFile(0, &swRedirect ) ;

	} ;

	//-------------->Tareas comunes a TODOS los breaks

	PrevBlock=m_uiOffActBOB ;	//Copia auxiliar
	m_uiOffActBOB = F->GetPos()  ;

	//--->Escribo bloque BOB y actualizo m_uiOffActBOB y m_iLastBlock 
	//	Queda por actualizar en el fichero:
	//	-NextBreak. Actualizado en PostReset, al escribir el EOB, y en el End(), donde se 
	//		pone a -1 el ultimo Break.
	//	-PagPrinted. Actualizado en PostReset, tratamiento EOB.

	WFBlockHeader(	F, 'b', m_iLastBlock, m_uiOffActBOB + SIZE_BOB_BLOCK ) ;
	WFLong(			F, GetBreakCount()+1 ) ;	//GroupNumber (+1 porque aun no se ejecuto PreReset base)
	WFLong(			F, PrevBlock) ;				//PreviousBreak
	WFLong(			F, 0) ;						//NextBreak
	WFLong(			F, 0) ;						//PagPrinted
	WFLong(			F, GetPagesDvcPrinted()) ;	//PagOffset

	m_iLastBlock=m_uiOffActBOB; 	

	//---------->Propaga

	ClENG_OutDevice::PreReset() ;

exit:
	return SwOk ;
}

void ClENG_DvcRVW::PostReset()
{
	m4uint32_t ThisBlock, Pos ;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	F = m_oOutRepository.GetFlushFile(0, &swRedirect ) ;
	
	ThisBlock=F->GetPos() ;

	//Escribo EOB

	WFBlockHeader( F, 'e', m_iLastBlock, ThisBlock+SIZE_EOB_BLOCK ) ;

	//Actualizo datos BOB pendientes (el Next sera machacado a -1 en el ultimo bloque en End()

	Pos=F->GetPos() ;
	WFLong( F, m_uiOffActBOB + BLOCK_OFF_BOB_NEXT, Pos ) ;
	WFLong( F, m_uiOffActBOB + BLOCK_OFF_BOB_PAGES_PRINTED, GetBreakPagesDvcPrinted() ) ;
	F->SetPos(Pos) ;

	m_iLastBlock = ThisBlock ;

	//---------->NO cierra ficheros, solo un break, se hara en el NotifyLastReset
	
	//---------->Propaga

	ClENG_OutDevice::PostReset(M4_FALSE);
}

void ClENG_DvcRVW::NotifyLastReset() 
{
	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	F = m_oOutRepository.GetFlushFile(0, &swRedirect ) ;

	//Bloque EOF

	WFBlockHeader( F,'E', m_iLastBlock, -1 ) ;	//Vuelca EOF Block a buffer

	//Datos pendientes de actualizar al finalizar el fichero

	WFLong( F, FILE_OFF_BOF_PAGES_PRINTED, GetPagesDvcPrinted() ) ;
	WFLong( F, FILE_OFF_BOF_GROUPS_PRINTED, GetBreakCount() ) ;
	WFLong( F, m_uiOffActBOB+BLOCK_OFF_BOB_NEXT, -1 ) ;


	//---------->Cierra ficheros 

	m_oOutRepository.CloseBreak( IsValidOutput() ) ;
}

m4bool_t  ClENG_DvcRVW::RefreshLayout() 
{
	m4bool_t SwOk=M4_TRUE ;

	//---------->Notifica a FileRep (no abre files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.NewFlush() ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::RefreshLayout() ;
}



m4bool_t ClENG_DvcRVW::FlushLayout()
{
	m4int16_t Page ;
	m4uint32_t CountThis, CountWrited ;
	m4bool_t SwOk=M4_TRUE ;

	ClENG_PhysicalFile *F ;
	m4bool_t swRedirect ;

	//---------->Corre EOPs

	RunAllEOP() ;

	//---------->Actualiza file 

	F = m_oOutRepository.GetFlushFile(0, &swRedirect ) ;

	//Ojo, entre pasadas del bucle no se graba a fichero, por eso usamos CountWrited
	CountWrited = F->GetPos() ;	

	for (Page=0; Page<GetNumLayoutPages(); ++Page ) {

		//En cada buffer ya estan volcados el bloque BOP (parte Preserve) y el bloque de 
		//	tags (parte normal). Añadimos lo que falta.

		WBChar(Page,'E') ;	//Marca de fin de tags

		//Ahora actualizo los datos que faltaban en la cabecera

		CountThis = m_poLayoutPages[Page].m_poActualBuff->GetTotal() ;

		OWBLong(Page,BLOCK_OFF_PREVIOUS,    m_iLastBlock) ;
		OWBLong(Page,BLOCK_OFF_NEXT,        CountWrited+CountThis  ) ;
		OWBLong(Page,BLOCK_OFF_PAGE_NUMBER, GetNumPageDvc(Page)  ) ;
		
		m_iLastBlock = CountWrited ;
		CountWrited += CountThis;
	} ;

	// m_iLastBlock aqui ya esta bien actualizado

	//---------->Vuelca buffers a files  (no cierra files, no ATOMIC_MODE)

	SwOk=m_oOutRepository.CloseFlush( ClENG_DvcFile::m_poBuffArray ) ;

	//---------->Propaga

	return SwOk && ClENG_OutDevice::FlushLayout();
}

//---------------Tags

void ClENG_DvcRVW::SelectFontDevice(m4int16_t ai_iPage, ClFONT_M_Font * ai_poFont)
{
	m4char_t Flags=0 ;

	WBChar(ai_iPage, 'F') ;

	if ( ai_poFont->IsFixedSpace() ) Flags = Flags | 0x01 ;	
	if ( ai_poFont->IsUnderline() )  Flags = Flags | 0x02 ;	
	if ( ai_poFont->IsBold() )       Flags = Flags | 0x04 ;	
	if ( ai_poFont->IsItalic() )     Flags = Flags | 0x08 ;	

	WBChar(ai_iPage, Flags ) ;
	WBString( ai_iPage, ai_poFont->RVW_GetFaceName() ) ;
	WBInt( ai_iPage, (m4int16_t)ai_poFont->RVW_GetHeight()*100 ) ;
}

void ClENG_DvcRVW::SelectColourDevice( m4int16_t ai_iPage, m4uint32_t ai_uiIdColour ) 
{
	ClENG_Colour *C = m_oFGPalette.GetColourById(ai_uiIdColour) ;
	
	WBChar(ai_iPage, 'C') ;
	WBChar(ai_iPage, C->GetR()) ;
	WBChar(ai_iPage, C->GetG()) ;
	WBChar(ai_iPage, C->GetB()) ;
}



void ClENG_DvcRVW::PrintNChar( m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
		m4pchar_t ai_pcStr, size_t ai_szNChar, m4bool_t ai_bRawText, 
		m4char_t *ai_pcHyperlink, m4uint16_t ai_iTargetID, ClENG_OutDevice::StPIParams *ai_pstPI) 
{
	m4uint32_t lValueOffset  = 0;
	M4ClBuffer *PageBuffer=m_poLayoutPages[ai_iPage].m_poActualBuff ;

	ai_iX += GetOffsetX(ai_iPage) ;
	ai_iY += GetOffsetY(ai_iPage) ;
	//ai_iY += GetLastFont(ai_iPage)->GetDvcHeight() ;	//Ajuste de linea-base

	WBChar(ai_iPage, 'S') ;

	WBLong(ai_iPage,ai_iX) ;
	WBLong(ai_iPage,ai_iY) ;


	// Dado que en WString escribe la longitud de la cadena y después la cadena, a la posición 
	// actual en el buffer le tengo que sumar lo que mide un long que es lo que usa para 
	// escribir dicha longitud.  Luego asi solo hay que escribir la cadena.
	lValueOffset = PageBuffer->GetTotal() + sizeof(m4int32_t); 


	// UNICODE REPORTS.
	// Bug 0125206. En los campos multilínea te llega el contenido del campo completo
	// pero en el número de caracteres te llega los que realmente quiere imprimir.
	m4pchar_t pcInputString = new m4char_t[ai_szNChar + 1];
	memcpy(pcInputString, ai_pcStr, ai_szNChar);
	pcInputString[ai_szNChar] = '\0';

	WBWideCharString(ai_iPage, pcInputString);

	delete [] pcInputString;

	if ( ai_pstPI ) {
		ai_pstPI->m_uiOff=lValueOffset ;
		ai_pstPI->m_szNChars=ai_szNChar ;
	} ;
}


void ClENG_DvcRVW::PrintRasterFile(
		m4int16_t ai_iPage,
		DvcUnit_t ai_X,				DvcUnit_t ai_Y,
		DvcUnit_t ai_SizeX,			DvcUnit_t ai_SizeY,
		const m4char_t * ai_pcPath, m4char_t *ai_pcHyperlink,
		m4uint16_t ai_iTargetID,	m4bool_t m_bIsTempWebPath ) 
{
	ClMIT_File File ;
	m4bool_t SwOk=M4_TRUE ;

	ai_X += GetOffsetX(ai_iPage) ;
	ai_Y += GetOffsetY(ai_iPage) ;

	WBChar(ai_iPage, 'G') ;

	WBLong(ai_iPage,ai_X) ;
	WBLong(ai_iPage,ai_Y) ;
	WBLong(ai_iPage,ai_SizeX) ;
	WBLong(ai_iPage,ai_SizeY) ;
	WBString(ai_iPage, ai_pcPath) ;

	SwOk = File.Open( ai_pcPath, ClMIT_File::EXIST, ClMIT_File::READ, ClMIT_File::TRY_OPEN, M4_FALSE ) ;
	
	if (!SwOk) {
		WBLong(ai_iPage, GRAPHIC_TAG_ERROR) ;
	} else {
		WBLong( ai_iPage, File.GetSize() ) ;
		m_poLayoutPages[ai_iPage].m_poActualBuff->AppendFile(&File) ;	
	} ;

	File.Close() ;
}

//----------------------------Auxiliares escritura RVW

void ClENG_DvcRVW::WBBlockHeader( m4int16_t ai_iIdPage, m4char_t ai_cId, m4int32_t ai_iPrevious, m4int32_t ai_iNext ) 
{
	WBChar(ai_iIdPage,ai_cId) ;
	WBLong(ai_iIdPage,ai_iPrevious) ;
	WBLong(ai_iIdPage,ai_iNext) ;
}

void ClENG_DvcRVW::WFBlockHeader( ClENG_PhysicalFile *ai_poFile, m4char_t ai_cId, m4int32_t ai_iPrevious, m4int32_t ai_iNext ) 
{
	WFChar(ai_poFile,ai_cId) ;
	WFLong(ai_poFile,ai_iPrevious) ;
	WFLong(ai_poFile,ai_iNext) ;
}

void ClENG_DvcRVW::WBInt(     m4int16_t ai_iIdPage, m4int16_t ai_iInt) 
{
	ai_iInt=m_BAdapter.EncodeInt(ai_iInt) ;
	m_poLayoutPages[ai_iIdPage].m_poActualBuff->Append(&ai_iInt, sizeof(ai_iInt) ) ;	
}

void ClENG_DvcRVW::WBLong(    m4int16_t ai_iIdPage, m4int32_t ai_iLong) 
{
	ai_iLong=m_BAdapter.EncodeLong(ai_iLong) ;
	m_poLayoutPages[ai_iIdPage].m_poActualBuff->Append(&ai_iLong, sizeof(ai_iLong) ) ;	
}

void ClENG_DvcRVW::WBChar(    m4int16_t ai_iIdPage, m4char_t  ai_cChar) 
{
	m_poLayoutPages[ai_iIdPage].m_poActualBuff->Append(&ai_cChar, sizeof(ai_cChar) ) ;	
}

void ClENG_DvcRVW::WBString(  m4int16_t ai_iIdPage, const m4char_t * ai_pcStr, size_t ai_szLen ) 
{
	if (ai_szLen==-1L) ai_szLen=strlen(ai_pcStr) ;
	WBLong(ai_iIdPage,ai_szLen) ;
	m_poLayoutPages[ai_iIdPage].m_poActualBuff->Append(ai_pcStr, ai_szLen ) ;	
}

void ClENG_DvcRVW::WBWideCharString(m4int16_t ai_iIdPage, const m4char_t * ai_pcStr)
{
	M4XMLCh* pwcString = NULL;
	m4int_t iwcLength = 0;
	m4int32_t iLen = 0;
	
	// Convertir de Cpp a UTF16. Esta función te devuelve el tamaño de la cadena
	// wide char.
	iwcLength = strlen(ai_pcStr);
	pwcString = M4CppToReports(ai_pcStr, iwcLength);

	iLen = iwcLength * sizeof(unsigned short);

	// Escribir caracteres UTF16 que son el doble.
	WBLong(ai_iIdPage, iLen);

	m_poLayoutPages[ai_iIdPage].m_poActualBuff->Append(pwcString, iLen);	

	delete [] pwcString;
}

void ClENG_DvcRVW::WFInt(     ClENG_PhysicalFile *ai_poFile, m4int16_t ai_iInt) 
{
	ai_iInt=m_BAdapter.EncodeInt(ai_iInt) ;
	ai_poFile->W(&ai_iInt, sizeof(ai_iInt) ) ;	
}

void ClENG_DvcRVW::WFLong(    ClENG_PhysicalFile *ai_poFile, m4int32_t ai_iLong) 
{
	ai_iLong=m_BAdapter.EncodeLong(ai_iLong) ;
	ai_poFile->W(&ai_iLong, sizeof(ai_iLong) ) ;
}

void ClENG_DvcRVW::WFChar(    ClENG_PhysicalFile *ai_poFile, m4char_t  ai_cChar) 
{
	ai_poFile->W(&ai_cChar, sizeof(ai_cChar) ) ;	
}

void ClENG_DvcRVW::WFString(  ClENG_PhysicalFile *ai_poFile, const m4char_t * ai_pcStr, size_t ai_szLen ) 
{
	if (ai_szLen==-1L) ai_szLen=strlen(ai_pcStr) ;
	WFLong(ai_poFile,ai_szLen) ;
	ai_poFile->W(ai_pcStr, sizeof(m4char_t)*ai_szLen ) ;	
}


void ClENG_DvcRVW::OWBLong( m4int16_t ai_iIdPage, m4uint32_t ai_uiFrom, m4int32_t ai_iLong ) 
{
	M4ClBuffer::StCopy Copy ;

	ai_iLong=m_BAdapter.EncodeLong(ai_iLong) ;
	Copy.Data= &ai_iLong ;
	Copy.Count=sizeof(ai_iLong) ;
	m_poLayoutPages[ai_iIdPage].m_poActualBuff->Flush(
		M4ClBuffer::MemToBuffManip, &Copy, ai_uiFrom, sizeof(ai_iLong) 
	) ;	
}


void ClENG_DvcRVW::WFLong( ClENG_PhysicalFile *ai_poFile, m4uint32_t ai_Pos, m4int32_t ai_iLong) 
{
	ai_iLong=m_BAdapter.EncodeLong(ai_iLong) ;
	ai_poFile->SetPos(ai_Pos) ;
	ai_poFile->W(&ai_iLong, sizeof(ai_iLong) ) ;	
}


void  ClENG_DvcRVW::PrintNCharPi(
	ClMIT_Str  *ai_poIdPi,m4int16_t ai_iPage, DvcUnit_t ai_iX, DvcUnit_t ai_iY, 
	size_t ai_szNChar, m4bool_t ai_bRawText, m4char_t *ai_pcHyperlink, 
	m4uint16_t ai_iTargetID)
{
	ClENG_OutDevice::StPIParams PIParams ;
	ClMIT_Str StrAux ;

	// Creo la cadena con tantas X como ai_sNChar indica
	StrAux.StrDuplicateChar(0,ai_szNChar,'X') ;

	// Imprimo la cadena y tomo el offset
	PIParams.m_oIdPi.StrCpy(ai_poIdPi->InChar(0));
	PrintNChar(ai_iPage,ai_iX,ai_iY,StrAux,ai_szNChar, ai_bRawText, ai_pcHyperlink,ai_iTargetID,&PIParams);

	// Añado el Pi a la lista
	if (PIParams.m_szNChars) {

		m_oOutRepository.AddNewPi( ai_poIdPi->InChar(0), ai_iPage, 
			PIParams.m_uiOff, PIParams.m_szNChars ) ;
	} ;
}

void ClENG_DvcRVW::ResolvePi(ClMIT_Str  *ai_poIdPi,m4pchar_t ai_pcStr) 
{
	StPi_t piData ;

	m4uint32_t Len;
	m4uint32_t PosActual;
	ClMIT_Str StrAux ;

	M4XMLCh* pwcString = NULL;
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
			*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCRVW_MSG_10 ] ) 
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
		// los bytes necesarios (iByteLength - iByteChar) para asegurar que la cadena UTF16 resultado
		// tenga piData.m_NBytes caracteres.
		// (Sin este reajuste en un entorno Unicode, y con la casuística indicada, se obtiene una
		//  cadena con un tamaño inferior provocando que aparezcan caracteres no representables
		//  en el visor)
		iByteTotal = piData.m_NBytes + iByteLength - iCharLength;
		StrAux.Accommodate( iByteTotal +1 ) ;
 		Len = sprintf(StrAux.InChar(0),"%-*.*s", iByteTotal , iByteTotal , ai_pcStr);
		StrAux.OverStringValid() ;	//No haria falta, se pone por si acaso
		
		// UNICODE REPORTS
		// Convertir de Cpp a UTF16. Esta función te devuelve el tamaño de la cadena
		// wide char.
		// Bug 0125211. Volcar las PIs en UTF16.
		iLen = strlen(StrAux.InChar(0));
		pwcString = M4CppToReports(StrAux.InChar(0), iLen);
		
		iWcPIBytes = iLen * sizeof(unsigned short);

	    // Imprimo la cadena asociada en RAM o en fichero, según dónde de encuentre el Pi
		if ( piData.m_Place == StPi_t::PI_IN_RAM)
		{  
			// Imprimir en el buffer	
			M4ClBuffer::StCopy Param = { pwcString, iWcPIBytes };	           
			m_poLayoutPages[ piData.m_NPage ].m_poActualBuff->Flush(M4ClBuffer::MemToBuffManip,
				&Param, piData.m_Offset, iWcPIBytes );
		}
		else
		{ 	
			// Imprimir directamente en el fichero
			m4bool_t bWasClosed=M4_FALSE;
			
			//Tomo la posicion actual del fichero, y me posiciono en el offset
			if(  ! piData.m_poFile->IsOpen() )
			{
				bWasClosed=M4_TRUE;
				piData.m_poFile->Open( ClMIT_File::READ_WRITE, ClMIT_Trace::PROPAGATE_ERROR) ;
			};

			PosActual = piData.m_poFile->GetPos();
			piData.m_poFile->SetPos( piData.m_Offset );

			// Imprimo y dejo en fichero en la posicion inicial
			piData.m_poFile->W( pwcString, iWcPIBytes );
			piData.m_poFile->SetPos(PosActual);
			if(bWasClosed)
			{
				//Solo lo cierro si ya lo estaba
				piData.m_poFile->Close();
			};
		};
	};

	delete [] pwcString;
	
	return;
}

void ClENG_DvcRVW::PrintLine( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X1, DvcUnit_t ai_Y1, DvcUnit_t ai_X2, DvcUnit_t ai_Y2,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	ai_X1 += GetOffsetX(ai_iPage) ;
	ai_Y1 += GetOffsetY(ai_iPage) ;
	ai_X2 += GetOffsetX(ai_iPage) ;
	ai_Y2 += GetOffsetY(ai_iPage) ;

	WBChar(ai_iPage, 'L' ) ;

	WBLong(ai_iPage,ai_X1) ;
	WBLong(ai_iPage,ai_Y1) ;
	WBLong(ai_iPage,ai_X2) ;
	WBLong(ai_iPage,ai_Y2) ;
	WBInt(ai_iPage, ClMetric::DvcToDPoint(ai_LineWidth)*10) ;
	WBInt(ai_iPage,(m4int16_t)ai_Style) ;
}

void ClENG_DvcRVW::FillZone( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height ) 
{
	ai_X += GetOffsetX(ai_iPage) ;
	ai_Y += GetOffsetY(ai_iPage) ;

	WBChar(ai_iPage, 'Z' ) ;

	WBLong(ai_iPage,ai_X) ;
	WBLong(ai_iPage,ai_Y) ;
	WBLong(ai_iPage,ai_Width) ;
	WBLong(ai_iPage,ai_Height) ;
}

void ClENG_DvcRVW::PrintFrame( 
	m4int16_t ai_iPage, 
	DvcUnit_t ai_X, DvcUnit_t ai_Y, DvcUnit_t ai_Width, DvcUnit_t ai_Height,
	DvcUnit_t ai_LineWidth, STYLE ai_Style ) 
{
	ai_X += GetOffsetX(ai_iPage) ;
	ai_Y += GetOffsetY(ai_iPage) ;

	WBChar(ai_iPage, 'R' ) ;

	WBLong(ai_iPage,ai_X) ;
	WBLong(ai_iPage,ai_Y) ;
	WBLong(ai_iPage,ai_Width) ;
	WBLong(ai_iPage,ai_Height) ;
	WBInt(ai_iPage, ClMetric::DvcToDPoint(ai_LineWidth)*10) ;
	WBInt(ai_iPage,(m4int16_t)ai_Style) ;
}

