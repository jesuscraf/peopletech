//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             cldvcfi.h
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

#ifndef __CLDVCFI__H__
#define __CLDVCFI__H__

//Para el multimap
#include "m4stl.hpp"
#include "m4ipcs.hpp"

#include "cldevice.h"
#include "clfile.h"
#include "clargs.h"
#include "designin.h"

#include "groupfrp.h"

class ClENG_PhysicalFile ;

//========================================================================ClENG_DvcFile

class ClENG_DvcFile  : public ClENG_OutDevice
{

MIT_PROTECTED:

	// Asociacion con el repositorio de ficheros
	ClENG_ReportOuputRepository m_oOutRepository;

MIT_PROTECTED:

	//Auxiliar de proceso 
	M4ClBuffer **m_poBuffArray ;
	size_t m_SzBuffArray ;

MIT_PUBLIC:

	//---------------------------------------Inicializacion--------------------------------
	
	ClENG_DvcFile();
	virtual ~ClENG_DvcFile() { ClENG_DvcFile::End() ; }
	virtual size_t GetSizeof() { return sizeof(ClENG_DvcFile) ; }
	virtual void End() ;


	m4bool_t Init( 
	
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
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR
	 );
	
	//Si en ao_InitializeLastPhantomPage se devuelve TRUE, el programa llamador debería
	//	inicializar su ultima pagina (si es que tiene estructuras de control propias 
	//	asociadas por cada página). Debe incializar la ultima página con el contenido 
	//	de la actual pagina. Esto se debe a que la ultima pagina es una phantom page.

	m4bool_t InitPage( m4int16_t ai_iNPage, DvcUnit_t ai_SizeX, DvcUnit_t  ai_SizeY, 
		DvcUnit_t ai_OffsetX, DvcUnit_t  ai_OffsetY, size_t ai_szBlockBuffer,
		m4bool_t &ao_InitializeLastPhantomPage,
		ClMIT_Trace::ERROR_EFFECT ai_ErrorEffect = ClMIT_Trace::IMMEDIATE_ERROR) ;

	// ------------------------- Metodos forzados a virtuales puros--------------------	

MIT_PUBLIC:

	virtual m4bool_t PreReset()=0 ;
	virtual void PostReset()=0 ; 
	virtual m4bool_t RefreshLayout()=0 ;
	virtual m4bool_t FlushLayout()=0;

	// ------------------------- Metodos Flow virtuales sobrecargados-------------------	

	virtual ClENG_PhysicalFile *HasFlushFile(m4int16_t ai_iNPage) ;

	virtual void GetNavigationPathMask(ClMIT_Str *ao_Str) ;

	virtual m4bool_t PreparePublishGroup(ClENG_Chn *ai_poChn, const m4char_t *ai_pcReportId) ;
	
	virtual m4bool_t IsDelBreak() ;
	virtual void SetDelBreak(m4bool_t ai_bDelBreak) ;
	virtual m4uint32_t GetPublishedBreakCount() ;

	virtual m4bool_t IsUTF16();

	// ------------------------------Metodos de utileria general-------------------------

MIT_PUBLIC:


	static ClENG_BreakList::DUPLEX_MODE GetMappingToFileDuplex(
			StENG_DvcParams::DUPLEX_MODE ai_Duplex) ;

} ;
			

#endif
