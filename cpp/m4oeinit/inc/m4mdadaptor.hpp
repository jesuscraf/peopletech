
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4mdadaptor.hpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                29-02-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene el interfaz abstracto del adaptador del servicio de metadatos
//
//
//==============================================================================


#include "m4oeinit_dll.hpp"
#include "m4types.hpp"


#ifndef __M4MDADAPTOR_HPP__
#define __M4MDADAPTOR_HPP__


#define  MD_SERVICE_ID "METADATA"

//=============================================================================
// Declaraciones adelantadas
//=============================================================================

class	ClCMCRWrapper;
class	ClMappings;
class	ClCSCRWrapper;
class	ClCacheDirectory;
class	ClPresentation;
class	ClPresentationAttributes;
class	ClMDUpdateDeltasList;
class	m4VariantType;
class	ClVMStatisticsWrapper;
class	ClLongString ;


//=================================================================================
//
// ClMDAdaptor
//
// Clase para el interfaz abstracto del adaptador del servicio de metadatos
//
//=================================================================================

class ClMDAdaptor
{
public:

	//=================================================================================
	// metacanal
	//=================================================================================
	virtual m4return_t RemoteGetCMCR(m4uint8_t ai_iCsType, 
									 m4pcchar_t ai_pccChannelId, 
									 m4pcchar_t ai_pccOrgId, 
									 m4uint8_t ai_iLanguage,
									 m4bool_t ai_bApplyMappings,
									 m4VariantType &ai_roStartDate, 
									 m4VariantType &ai_roEndDate, 
									 m4VariantType &ai_roCorDate, 
									 ClCMCRWrapper * &ao_poCMCRWrap,
									 ClVMStatisticsWrapper* ai_poSttWrapper) = 0;

	//=================================================================================
	// mappings
	//=================================================================================
	virtual m4return_t RemoteGetMap( m4pcchar_t ai_pccMapId, 
									 ClMappings* &ao_poMappping,
									 ClVMStatisticsWrapper* ai_poSttWrapper) = 0;

	//=================================================================================
	// seguridad
	//=================================================================================
	virtual m4return_t RemoteGetCSCR(m4uint8_t ai_iCsType, 
									 m4pcchar_t ai_pccChannelId, 
									 m4pcchar_t ai_pccMask,
									 ClCSCRWrapper * &ao_poCSCRWrap,
									 ClVMStatisticsWrapper* ai_poSttWrapper) = 0;

	//=================================================================================
	// presentación
	//=================================================================================
	virtual m4return_t RemoteGetPresentation(m4pcchar_t ai_pcIdPresentation,
											 const ClPresentationAttributes & ai_ClPresentationAttributes,
											 m4bool_t ai_bDefinition,
											 ClPresentation * &ao_poPresentation,
	 										 ClVMStatisticsWrapper* ai_poSttWrapper) = 0;

	//=================================================================================
	// MDUpdateList
	//=================================================================================
	virtual m4return_t RemoteGetMDUpdatedList(ClCacheDirectory * ai_pCacheDirectory,
											  ClMDUpdateDeltasList *& ao_poUpdatedDeltasList,
											  ClVMStatisticsWrapper* ai_poSttWrapper) = 0;

	//=================================================================================
	// CacheRemove
	//=================================================================================
	virtual m4return_t RemoteCacheRemove(m4uint16_t ai_iCacheType, 
										 m4pcchar_t ai_pcId,
	 									 ClVMStatisticsWrapper* ai_poSttWrapper) = 0;

} ;

#endif

