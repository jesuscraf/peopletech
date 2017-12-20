
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4mdadapimp.hpp   
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
//    Este módulo contiene la implementación del adaptador del servicio de metadatos
//
//
//==============================================================================


#include "m4oeinit_dll.hpp"
#include "m4mdadaptor.hpp"
#include "m4types.hpp"


#ifndef __M4MDADAPIMP_HPP__
#define __M4MDADAPIMP_HPP__


//=============================================================================
// Declaraciones adelantadas
//=============================================================================


class ClAppClientAdap;
class ClChannelManager;
class ClCsTrace;
class ClVMBaseEnv;
class M4DataStorage;

//=================================================================================
//
// ClMDAdaptorImp
//
// Clase para el adaptador del servicio de metadatos
//
//=================================================================================

class ClMDAdaptorImp : public ClMDAdaptor
{
public:

	ClMDAdaptorImp( void );

	m4return_t Init(ClVMBaseEnv * ai_poRegistry, ClAppClientAdap *ai_poClientAPI, ClChannelManager * ai_poChannelManager, ClCsTrace	* ai_poCsTrace);

	virtual	~ClMDAdaptorImp( void );


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
									 ClVMStatisticsWrapper* ai_poSttWrapper);

	//=================================================================================
	// mappings
	//=================================================================================
	virtual m4return_t RemoteGetMap( m4pcchar_t ai_pccMapId, 
									 ClMappings* &ao_poMappings,
									 ClVMStatisticsWrapper* ai_poSttWrapper);

	//=================================================================================
	// seguridad
	//=================================================================================
	virtual m4return_t RemoteGetCSCR(m4uint8_t ai_iCsType, 
									 m4pcchar_t ai_pccChannelId, 
									 m4pcchar_t ai_pccMask,
									 ClCSCRWrapper * &ao_poCSCRWrap,
									 ClVMStatisticsWrapper* ai_poSttWrapper);

	//=================================================================================
	// presentación
	//=================================================================================
	virtual m4return_t RemoteGetPresentation(m4pcchar_t ai_pcIdPresentation,
											 const ClPresentationAttributes & ai_ClPresentationAttributes,
											 m4bool_t ai_bDefinition,
											 ClPresentation * &ao_poPresentation,
	 										 ClVMStatisticsWrapper* ai_poSttWrapper);

	//=================================================================================
	// MDUpdateList
	//=================================================================================
	virtual m4return_t RemoteGetMDUpdatedList(ClCacheDirectory * ai_pCacheDirectory,
											  ClMDUpdateDeltasList *& ao_poUpdatedDeltasList,
											  ClVMStatisticsWrapper* ai_poSttWrapper);

	//=================================================================================
	// CacheRemove
	//=================================================================================
	virtual m4return_t RemoteCacheRemove(m4uint16_t ai_iCacheType, 
										 m4pcchar_t ai_pcId,
	 									 ClVMStatisticsWrapper* ai_poSttWrapper);

private:
	m4return_t SendCommand (M4DataStorage *ai_poInputData, M4DataStorage *ai_pOutputData);

	ClChannelManager *	m_poChannelManager;
	ClAppClientAdap *	m_poClientAPI;
	ClCsTrace	*		m_poCsTrace;
	m4uint32_t			m_iGenCSTimeOut;
	m4bool_t			m_bInit;
} ;

#endif

