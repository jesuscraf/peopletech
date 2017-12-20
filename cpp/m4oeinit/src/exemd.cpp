#include "csres.hpp"
#include "chlog.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              exeforchannel
// File:                exemd.cpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                05-12-1998
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module implements the executive main routiner for the meta data service
//
//
//==============================================================================

// include files
#include "m4define.hpp"
#include "m4types.hpp"
#include "mdprotocol.hpp"
#include "clpresen.hpp"
#include "m4mdrt.hpp"
#include "exechannel.hpp"
#include "cachedir.hpp"
#include "mdupdate.hpp"
#include "cllstr.hpp"
#include "clmappings.hpp"



m4return_t ExecMetaData(ClCMCRFactory			*ai_poCMCRFact, 
						ClPresentationFactory	*ai_poPresFac, 
						ClCacheDirectory		*ai_poCacheDirectory,
						ClChannelManager		*ai_poChannelManager,
						M4DataStorage			*ai_poInputDataStorage,
						M4DataStorage			*ao_poOutputDataStorage,
						void					*poExecParam,
						ExecDumpInfo		    *aio_poDumpInfo)
{

   // Initial step
   if (NULL != aio_poDumpInfo)        aio_poDumpInfo->SetCurrentStage("Preparing to Execute", 0);

#ifdef _DEBUG
	cout << "Executor for Meta Data starting..." << endl;
	cout.flush();
#endif

	m4return_t ret;
	m4uint16_t iMetadataType;

	ClDataStoreIODriver IODIn, IODOut;

	IODIn.Init(GEN_IO_MODE_READ,ai_poInputDataStorage);

	IODOut.Init(GEN_IO_MODE_WRITE,ao_poOutputDataStorage);

	//
	// Protocolo
	//
	//Podria ir en un bucle.
	IODIn.Read(iMetadataType);

	switch (iMetadataType)
	{
		case M4_SERVICE_METADATA_CMCR:
			{
#ifdef _DEBUG
				cout << "M4_SERVICE_METADATA_CMCR" << endl;
				cout.flush();
#endif

				ClCMCRWrapper *poCMCRWrap = 0;

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_CMCR BuildCMCRFromRequest");
				
				ret = BuildCMCRFromRequest (ai_poCMCRFact, IODIn, poCMCRWrap, aio_poDumpInfo);

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_CMCR BuildResponseFromCMCR");
				
				ret = BuildResponseFromCMCR (IODOut, poCMCRWrap, aio_poDumpInfo);

				if (poCMCRWrap)
					poCMCRWrap->Destroy();

			}

			break;

		case M4_SERVICE_METADATA_MAP:
			{
#ifdef _DEBUG
				cout << "M4_SERVICE_METADATA_MAP" << endl;
				cout.flush();
#endif

				ClMappings *poMappings = 0;

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_MAP BuildMapFromRequest");
				
				ret = BuildMapFromRequest (ai_poCMCRFact, IODIn, poMappings, aio_poDumpInfo);

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_MAP BuildResponseFromMap");
				
				ret = BuildResponseFromMap (IODOut, poMappings, aio_poDumpInfo);

				if (poMappings)
					poMappings->Destroy();

			}

			break;

		case M4_SERVICE_METADATA_CSCR:
			{
#ifdef _DEBUG
				cout << "M4_SERVICE_METADATA_CSCR" << endl;
				cout.flush();
#endif

				ClCSCRWrapper *poCSCRWrap = 0;

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_CSCR BuildCSCRFromRequest");
				
				ret = BuildCSCRFromRequest (ai_poCMCRFact, IODIn, poCSCRWrap, aio_poDumpInfo);

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_CSCR BuildResponseFromCSCR");
				
				ret = BuildResponseFromCSCR (IODOut, poCSCRWrap, aio_poDumpInfo);

				if (poCSCRWrap)
					poCSCRWrap->Destroy();

			}
			break;

		case M4_SERVICE_METADATA_PRESENTATION:
			{
#ifdef _DEBUG
				cout << "M4_SERVICE_METADATA_PRESENTATION" << endl;
				cout.flush();
#endif

				ClPresentation *poPresentation = 0;

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_PRESENTATION BuildPresentationFromRequest");
				
				ret = BuildPresentationFromRequest (ai_poPresFac, IODIn, poPresentation, aio_poDumpInfo);

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_PRESENTATION BuildResponseFromPresentation");

				ret = BuildResponseFromPresentation (IODOut, poPresentation, aio_poDumpInfo);

				if (poPresentation)
					poPresentation->Destroy();
			}

			break;

		case M4_SERVICE_METADATA_GET_UPDATE_LIST:
			{
#ifdef _DEBUG
				cout << "M4_SERVICE_METADATA_GET_UPDATE_LIST" << endl;
				cout.flush();
#endif

				ClMDUpdateDeltasList * poMDUpdateDeltasList = 0;

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_GET_UPDATE_LIST BuildUpdatedListFromRequest");
				
				ret = BuildUpdatedListFromRequest (ai_poChannelManager, (ClMDUpdateList *)poExecParam, IODIn, poMDUpdateDeltasList, aio_poDumpInfo);

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_GET_UPDATE_LIST BuildUpdatedListResponseFromList");

				ret = BuildResponseFromUpdatedList (IODOut, poMDUpdateDeltasList, aio_poDumpInfo);

				if (poMDUpdateDeltasList)
					delete poMDUpdateDeltasList;
			}

			break;

		case M4_SERVICE_METADATA_CACHE_REMOVE:
			{
#ifdef _DEBUG
				cout << "M4_SERVICE_METADATA_CACHE_REMOVE" << endl;
				cout.flush();
#endif

				if (NULL != aio_poDumpInfo)
					aio_poDumpInfo->SetCurrentStage("Executing M4_SERVICE_METADATA_CACHE_REMOVE BuildCacheRemoveResponseFromRequest");
				
				ret = BuildCacheRemoveResponseFromRequest (ai_poCacheDirectory, IODIn, IODOut, aio_poDumpInfo);
			}

			break;

		default:
			ret = IODOut.Write((m4return_t)M4_ERROR);

			g_oChLog<<BeginError(M4_CH_CS_BAD_MD_TYPE)<<iMetadataType<<EndLog;			

			break;
	}

	IODIn.End();

	IODOut.End();

	if (NULL != aio_poDumpInfo)
		aio_poDumpInfo->SetCurrentStage("Finishing execution");

#ifdef _DEBUG
	cout << "Executor for Meta Data finishing..." << endl;
	cout.flush();
#endif

	return M4_SUCCESS;
}






