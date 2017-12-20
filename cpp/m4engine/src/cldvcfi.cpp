//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcfi.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/28/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	David Fernández && 07 abr 98
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

#include "clstr.h"
#include "sysinfo.h"
#include "mitdates.h"
#include "xapichn.h"
#include "physfrp.h"


#include "cldvcfi.hmg"
#include "cldvcfi.h"


ClENG_DvcFile::ClENG_DvcFile()
{
	m_poBuffArray=NULL ;
}

void ClENG_DvcFile::End() 
{	
	m_oOutRepository.End() ;

	M4_DELETE_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poBuffArray, M4ClBuffer *, m_SzBuffArray 
	) ;	
	ClENG_OutDevice::End();
}

m4bool_t ClENG_DvcFile::Init( 
		//Campos para ClENG_OutDevice::Init

		m4int16_t ai_iNumPages,
		ClMIT_Flag::Value_t ai_Flags, ClENG_Palette *ai_poFGPalette,
		
		//Campos para ClENG_ReportOuputRepository::Init
		// (cuidado; el break y duplex es a nivel de File, difieren del de nivel device)

		m4bool_t ai_MonoBreak,
		StFileParams *ai_FileParams, m4bool_t ai_bDelete,
		ClENG_BreakList::BREAK_MODE ai_BreakMode,
		ClENG_BreakList::DUPLEX_MODE ai_Duplex, 
		DECLARE_EXTERNAL_FILE ai_DeclareExtFile,
		m4bool_t ai_bCreateDir,
		ClENG_ReportOuputRepository::PRESERVE_MODE ai_Preserve, 
		m4char_t *ai_pcToPrinter,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect )
{
	m4int16_t i;
	m4bool_t Return = M4_TRUE ;
	m4char_t cBreakSeparator = 0x00;

	// UNICODE REPORTS
	// Get if it is a UTF16 device.
	m4bool_t bIsUTF16 = IsUTF16();

	ClENG_DvcFile::End();

	if ( ai_FileParams->m_oFileNameExpression.StrLen() > 0 && ai_FileParams->m_poChnData != NULL )
	{
		// Si hay evaluación de expresiones por ruptura, se utiliza _ como separador
		// para diferenciar expresiones con resultados idénticos
		cBreakSeparator = '_';
	}

	//*******Init m_oOutRepository

	Return = m_oOutRepository.Init( 
		
		//General flow modifiers

		ai_MonoBreak, ai_BreakMode, ai_Duplex,
		
		//Root Path modifiers
		
		ai_FileParams->m_oPath, ai_FileParams->m_bIsFile, ai_bCreateDir,

		//File expression for break output
		ai_FileParams->m_oFileNameExpression,
		// Acceso al canal de datos para evaluación de expresiones
		ai_FileParams->m_poChnData,
		
		//PRESERVE_DIR is a root path modifier, other modes are file name modifiers

		ai_Preserve,
		
		//Name file modifiers

		ai_FileParams->m_iNumOfSecChars, ai_FileParams->m_iNumOfPageChars,
		ai_FileParams->m_cSeparator, cBreakSeparator,
		ai_FileParams->m_oExtension,

		//Break output management

		ai_FileParams->m_iOffSet, ai_FileParams->m_bUseDelBreak, 
		ai_FileParams->m_bDefaultDelBreak, ai_FileParams->m_bConsecutiveBreaks,
		ai_pcToPrinter, 

		//Flush management

		ai_iNumPages,

		//Report output management

		ai_bDelete, ai_DeclareExtFile,

		//Other specifics purposes

		ai_FileParams->m_oWebPath,

		// UNICODE REPORTS
		bIsUTF16,

		ClMIT_Trace::PROPAGATE_ERROR
	) ;
	if (!Return) goto exit ;

	//*******Ajusto al numero de paginas reales

	ai_iNumPages = m_oOutRepository.GetRealPageLayoutCount() ;

	//*******Init device

	Return=ClENG_OutDevice::Init(
		ai_iNumPages, ai_Flags, ai_poFGPalette,
		ClMIT_Trace::PROPAGATE_ERROR
	);
	if (!Return) goto exit ;

	//*******Init array redirecciones (los contenidos se ponen en los InitPage)

	m_SzBuffArray = ai_iNumPages ;
	M4_NEW_ARRAY(m_poInsp->m_poMemProf, ENG_MGI_CLDEVICE, 
		m_poBuffArray, M4ClBuffer *, m_SzBuffArray 
	) ;	

	for(i=0; i<m_SzBuffArray; i++) {
		m_poBuffArray[i]=NULL;
	};

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

m4bool_t ClENG_DvcFile::InitPage( m4int16_t ai_iNPage, DvcUnit_t ai_SizeX, DvcUnit_t  ai_SizeY, 
	DvcUnit_t ai_OffsetX, DvcUnit_t  ai_OffsetY, size_t ai_szBlockBuffer,
	m4bool_t &ao_InitializeLastPhantomPage,
	ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect) 
{
	m4bool_t Return = M4_TRUE ;

	m4int16_t pageCount = m_oOutRepository.GetRealPageLayoutCount() ;
	
	if ( !M4_IN_RANGE(ai_iNPage, 0, pageCount-1) ) {
		*m_poInsp->m_poTrace << ClMIT_Msg( &TRACE_ARRAY[ CLDVCFI_MSG_10 ] ) << SEND_MSG ;
		Return=M4_FALSE ;
		goto exit ;
	} ;

	if ( m_oOutRepository.IsPhantomPage() && ai_iNPage==1 ) {
		ao_InitializeLastPhantomPage = M4_TRUE ;
		M4_VIRTUAL_OBJECT( &(m_poLayoutPages[pageCount-1]), ClENG_Page )->
			Init( ai_SizeX, ai_SizeY, ai_OffsetX, ai_OffsetY, ai_szBlockBuffer ) ;
		
		m_poBuffArray[ pageCount-1] = &m_poLayoutPages[pageCount-1].m_oLayoutBuff ;
	} else {
		ao_InitializeLastPhantomPage = M4_FALSE ;
	} ;

	M4_VIRTUAL_OBJECT( &(m_poLayoutPages[ai_iNPage]), ClENG_Page )->
		Init( ai_SizeX, ai_SizeY, ai_OffsetX, ai_OffsetY, ai_szBlockBuffer ) ;
	m_poBuffArray[ai_iNPage] = &m_poLayoutPages[ai_iNPage].m_oLayoutBuff ;

exit:

	m_poInsp->m_poTrace->TestEffect( Return, ai_ErrorEffect ) ; //Aborta si efecto inmediato
	return Return ;		
}

ClENG_BreakList::DUPLEX_MODE ClENG_DvcFile::GetMappingToFileDuplex(
			StENG_DvcParams::DUPLEX_MODE ai_Duplex) 
{
	switch (ai_Duplex) {
	case StENG_DvcParams::EMULATED_DUPLEX:
		return ClENG_BreakList::EVEN_ODD_DUPLEX ;
		break ;

	case StENG_DvcParams::REAL_DUPLEX:
	case StENG_DvcParams::NO_DUPLEX:
	default:
		return ClENG_BreakList::SHARING_DUPLEX ;
		break ;
	} ;
}

ClENG_PhysicalFile *ClENG_DvcFile::HasFlushFile(m4int16_t ai_iNPage) 
{
	return m_oOutRepository.GetFlushFile(ai_iNPage) ;
}

void ClENG_DvcFile::GetNavigationPathMask(ClMIT_Str *ao_Str) 
{
	m_oOutRepository.GetNavigationPathMask(ao_Str) ;
}


m4bool_t ClENG_DvcFile::PreparePublishGroup(ClENG_Chn *ai_poChn, const m4char_t *ai_pcReportId) 
{
	return m_oOutRepository.PreparePublishGroup(ai_poChn, ai_pcReportId) ;
}

m4bool_t ClENG_DvcFile::IsDelBreak() 
{
	return m_oOutRepository.IsDelBreak() ;
}

void ClENG_DvcFile::SetDelBreak(m4bool_t ai_bDelBreak) 
{
	m_oOutRepository.SetDelBreak(ai_bDelBreak) ;
}

m4uint32_t ClENG_DvcFile::GetPublishedBreakCount() 
{
	return m_oOutRepository.GetPublishedBreakCount() ;
}

m4bool_t ClENG_DvcFile::IsUTF16()
{
	return M4_FALSE;
}
