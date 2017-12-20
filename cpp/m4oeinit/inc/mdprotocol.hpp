//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              serial 
// File:                mdprotocol.hpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                02-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module...
//
//
//==============================================================================

#ifndef _MDPROTOCOL_HPP
#define _MDPROTOCOL_HPP
#include "m4define.hpp"
#include "m4types.hpp"
#include "m4var.hpp"
#include "cldatstoiod.hpp"

#define M4_SERVICE_METADATA_CMCR						1
#define M4_SERVICE_METADATA_CSCR						2
#define M4_SERVICE_METADATA_PRESENTATION				3
#define M4_SERVICE_METADATA_GET_UPDATE_LIST				4
#define M4_SERVICE_METADATA_CACHE_REMOVE				5
#define M4_SERVICE_METADATA_MAP							6


class ClCMCRFactory;
class ClPresentationFactory;
class ClCSCRWrapper;
class ClCMCRWrapper;
class ClMappings;
class ClPresentation;
class ClPresentationAttributes;
class ExecDumpInfo;
class ClCacheDirectory;
class ClMDUpdateList;
class ClMDUpdateDeltasList;
class ClChannelManager;
class ClMDUpdateDeltasList;
class ClLongString;


// M4_SERVICE_METADATA_CMCR
m4return_t BuildCMCRRequest (ClDataStoreIODriver &IODOut, m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccOrgId, m4uint8_t ai_iLanguage, m4bool_t ai_bApplyMappings, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate);

m4return_t BuildCMCRFromRequest (ClCMCRFactory * ai_poCMCRFact, ClDataStoreIODriver &IODIn, ClCMCRWrapper * &ao_poCMCRWrap, ExecDumpInfo *aio_poDumpInfo );

m4return_t BuildResponseFromCMCR (ClDataStoreIODriver &IODOut, ClCMCRWrapper * ai_poCMCRWrap, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildCMCRFromResponse (ClDataStoreIODriver &IODIn, ClCMCRWrapper * &ao_poCMCRWrap);

// M4_SERVICE_METADATA_MAP
m4return_t BuildMapRequest (ClDataStoreIODriver &IODOut, m4pcchar_t ai_pccMapId);

m4return_t BuildMapFromRequest (ClCMCRFactory * ai_poCMCRFact, ClDataStoreIODriver &IODIn, ClMappings* &ao_poMappings, ExecDumpInfo *aio_poDumpInfo );

m4return_t BuildResponseFromMap (ClDataStoreIODriver &IODOut, ClMappings * ao_poMappings, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildMapFromResponse (ClDataStoreIODriver &IODIn, ClMappings* &ao_poMappings);

// M4_SERVICE_METADATA_CSCR
m4return_t BuildCSCRRequest (ClDataStoreIODriver &IODOut, m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask);

m4return_t BuildCSCRFromRequest (ClCMCRFactory * ai_poCMCRFact, ClDataStoreIODriver &IODIn, ClCSCRWrapper * &ao_poCSCRWrap, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildResponseFromCSCR (ClDataStoreIODriver &IODOut, ClCSCRWrapper * ai_poCSCRWrap, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildCSCRFromResponse (ClDataStoreIODriver &IODIn, ClCSCRWrapper * &ao_poCSCRWrap);

// M4_SERVICE_METADATA_PRESENTATION
m4return_t BuildPresentationRequest (ClDataStoreIODriver &IODOut, m4pcchar_t ai_pcIdPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition);

m4return_t BuildPresentationFromRequest (ClPresentationFactory * ai_poPresFact, ClDataStoreIODriver &IODIn, ClPresentation * &ao_poPresentation, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildResponseFromPresentation (ClDataStoreIODriver &IODOut, ClPresentation * ao_poPresentation, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildPresentationFromResponse (ClDataStoreIODriver &IODIn, ClPresentation * &ao_poPresentation);

// M4_SERVICE_METADATA_UPDATE_LIST
m4return_t BuildUpdatedListRequest (ClDataStoreIODriver &IODOut, ClCacheDirectory * ai_pCacheDirectory);

m4return_t BuildUpdatedListFromRequest (ClChannelManager * ai_pChannelManager, ClMDUpdateList * ai_poUpdateList, ClDataStoreIODriver &IODIn, ClMDUpdateDeltasList *& ao_poMDUpdateDeltasList, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildResponseFromUpdatedList (ClDataStoreIODriver &IODOut, ClMDUpdateDeltasList * ao_poMDUpdateDeltasList, ExecDumpInfo *aio_poDumpInfo);

m4return_t BuildUpdatedListFromResponse (ClDataStoreIODriver &IODIn, ClMDUpdateDeltasList *& poMDUpdateDeltasList);

// M4_SERVICE_METADATA_CACHE_REMOVE
m4return_t BuildCacheRemoveRequest (ClDataStoreIODriver &IODOut, m4uint16_t ai_iCacheType, m4pcchar_t ai_pcId);

m4return_t BuildCacheRemoveResponseFromRequest (ClCacheDirectory * ai_poCacheDirectory, ClDataStoreIODriver &IODIn, ClDataStoreIODriver &IODOut, ExecDumpInfo *aio_poDumpInfo);

m4return_t ReadCacheRemoveResponse (ClDataStoreIODriver &IODIn);

#endif
