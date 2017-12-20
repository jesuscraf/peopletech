
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


#include "m4mdadapimp.hpp"
#include "clappclientadap.hpp"
#include "dm.hpp"
#include "sttwrap.hpp"
#include "sttlib.hpp"
#include "sttlib.hpp"
#include "cldatstoiod.hpp"
#include "mdprotocol.hpp"
#include "cllgadap.hpp"
#include "m4cstrace.hpp"
#include "m4objglb.hpp"
#include "csres.hpp"
#include "m4objreg.hpp"
#include "cllstr.hpp"
#include "attributes.hpp"


ClMDAdaptorImp::ClMDAdaptorImp(void)
{
	m_poClientAPI = 0;
	m_poChannelManager = 0;
	m_poCsTrace = 0;
	m_iGenCSTimeOut = 0;
	m_bInit = M4_FALSE;
}

ClMDAdaptorImp::~ClMDAdaptorImp(void)
{
	m_poClientAPI = 0;
	m_poCsTrace = 0;
	m_poChannelManager = 0;
	m_iGenCSTimeOut = 0;
	m_bInit = M4_FALSE;
}

m4return_t ClMDAdaptorImp::Init(ClVMBaseEnv * ai_poRegistry, ClAppClientAdap *ai_poClientAPI, ClChannelManager * ai_poChannelManager, ClCsTrace	* ai_poCsTrace)
{
	if (M4_TRUE == m_bInit)
		return M4_SUCCESS;

	if (!ai_poClientAPI)
		return M4_ERROR;

	m_poClientAPI = ai_poClientAPI;

	if (!ai_poChannelManager)
		return M4_ERROR;

	m_poChannelManager = ai_poChannelManager;

	if (!ai_poCsTrace)
		return M4_ERROR;

	m_poCsTrace = ai_poCsTrace;

	if (!ai_poRegistry)
		return M4_ERROR;

	m_iGenCSTimeOut = ai_poRegistry->GetGenCSTimeOut();

	m_bInit = M4_TRUE;

	return M4_SUCCESS;
}

//=================================================================================
// metacanal
//=================================================================================
m4return_t ClMDAdaptorImp::RemoteGetCMCR(m4uint8_t ai_iCsType, 
									 m4pcchar_t ai_pccChannelId, 
									 m4pcchar_t ai_pccOrgId,
									 m4uint8_t ai_iLanguage,
									 m4bool_t ai_bApplyMappings,
									 m4VariantType &ai_roStartDate, 
									 m4VariantType &ai_roEndDate, 
									 m4VariantType &ai_roCorDate, 
									 ClCMCRWrapper * &ao_poCMCRWrap,
									 ClVMStatisticsWrapper* ai_poSttWrapper)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;

	m4return_t iResult = M4_ERROR;

	if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		//Ponemos el servicio a las Stt
		ai_poSttWrapper->GetStatistics()->sttAddService("METADA_SERVICE");
	}

	//Arquitectura
	m4uint16_t iArchitecture = M4_ARCH_INTEL;

	iArchitecture = m_poChannelManager->GetLogonAdaptor()->GetServerArchitecture();

	//Creamos los DataStorage
	M4DataStorage *poInputDataStorage = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poOutputDataStorage = M4DataStorage::GetNewDataStorage();

	ClDataStoreIODriver IODIn, IODOut;

	iResult = IODOut.Init(GEN_IO_MODE_WRITE, poOutputDataStorage, iArchitecture);

	if (iResult == M4_ERROR)
	{
		ao_poCMCRWrap = NULL ;
		M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
		M4DataStorage::DeleteDataStorage(poInputDataStorage); 
		DUMP_CHLOG_ERRORF( M4_CS_MCR_NO_CREATE_M4OBJ, ai_pccChannelId << M4ClCsType( ai_iCsType ) ) ;
		return M4_ERROR;
	}
	
	iResult = IODOut.Write((m4uint16_t)M4_SERVICE_METADATA_CMCR);

	if (iResult != M4_ERROR)
	{
		iResult = BuildCMCRRequest (IODOut, ai_iCsType, ai_pccChannelId, ai_pccOrgId, ai_iLanguage, ai_bApplyMappings, ai_roStartDate, ai_roEndDate, ai_roCorDate );

		if (iResult != M4_ERROR)
		{
			m4uint32_t iOutputSize=IODOut.GetActualOffset();

			IODOut.End();

			m_poCsTrace->PreMetaDataTrace();

			iResult = SendCommand(poOutputDataStorage, poInputDataStorage);

			if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
			{
				//Ponemos el tamano de la PDU a las Stt
				ai_poSttWrapper->GetStatistics()->sttAddOutputPDUSize(poOutputDataStorage->Size());
				
				ai_poSttWrapper->GetStatistics()->sttAddInputPDUSize(poInputDataStorage->Size());
			}

			if (iResult == M4_SUCCESS)
			{
				IODIn.Init(GEN_IO_MODE_READ, poInputDataStorage, iArchitecture);

				iResult = BuildCMCRFromResponse (IODIn, ao_poCMCRWrap);

				if( m_poCsTrace->IsEnabled() == M4_TRUE )
				{
					m4pchar_t pcRole = m_poChannelManager->GetLogonAdaptor()->GetRole() ;

					m_poCsTrace->PostMetaDataTrace( iOutputSize,
													IODIn.GetActualOffset(),
													ai_pccChannelId,
													ai_iCsType,
													pcRole,
													M4_FALSE);
				}

				IODIn.End();

				if (iResult == M4_ERROR)
				{
					//hasta que haya NO_UPDATE en la cache
					ao_poCMCRWrap = NULL ;
					M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
					M4DataStorage::DeleteDataStorage(poInputDataStorage); 
					// de debug porque ya debe estar en la pila
					DUMP_CHLOG_DEBUGF( M4_CS_MCR_NO_CREATE_M4OBJ, ai_pccChannelId << M4ClCsType( ai_iCsType ) ) ;
					return M4_ERROR;
				}
			}
			else
			{
				ao_poCMCRWrap = NULL ;
				M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
				M4DataStorage::DeleteDataStorage(poInputDataStorage); 
				DUMP_CHLOG_ERRORF( M4_CS_MCR_NO_CREATE_M4OBJ, ai_pccChannelId << M4ClCsType( ai_iCsType ) ) ;
				return M4_ERROR;
			}
		}
		else
		{
			ao_poCMCRWrap = NULL ;
			M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
			M4DataStorage::DeleteDataStorage(poInputDataStorage); 
			DUMP_CHLOG_ERRORF( M4_CS_MCR_NO_CREATE_M4OBJ, ai_pccChannelId << M4ClCsType( ai_iCsType ) ) ;
			return M4_ERROR;
		}
	}

	M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
	M4DataStorage::DeleteDataStorage(poInputDataStorage); 

	return M4_SUCCESS;
}


//=================================================================================
// mappings
//=================================================================================
m4return_t ClMDAdaptorImp::RemoteGetMap( m4pcchar_t ai_pccMapId, 
								 ClMappings* &ao_poMappings,
								 ClVMStatisticsWrapper* ai_poSttWrapper)
{

	m4return_t			iResult = M4_ERROR ;
	m4uint16_t			iArchitecture = M4_ARCH_INTEL ;
//	m4uint32_t			iOutputSize = 0 ;
//	m4pchar_t			pcRole = NULL ;
	ClDataStoreIODriver	IODIn ;
	ClDataStoreIODriver	IODOut ;
	M4DataStorage		*poInputDataStorage = NULL ;
	M4DataStorage		*poOutputDataStorage = NULL ;


	ao_poMappings = NULL ;

	if( m_bInit == M4_FALSE )
	{
		return( M4_ERROR ) ;
	}


	if( ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED )
	{
		ai_poSttWrapper->GetStatistics()->sttAddService( "METADA_SERVICE" ) ;
	}

	//Creamos los DataStorage
	poInputDataStorage = M4DataStorage::GetNewDataStorage() ;
	poOutputDataStorage = M4DataStorage::GetNewDataStorage() ;


	iArchitecture = m_poChannelManager->GetLogonAdaptor()->GetServerArchitecture();
	iResult = IODOut.Init( GEN_IO_MODE_WRITE, poOutputDataStorage, iArchitecture ) ;

	if( iResult == M4_SUCCESS )
	{
		iResult = IODOut.Write( m4uint16_t( M4_SERVICE_METADATA_MAP ) ) ;
	}

	if( iResult == M4_SUCCESS )
	{
		iResult = BuildMapRequest( IODOut, ai_pccMapId ) ;
	}

	if( iResult == M4_SUCCESS )
	{
//		iOutputSize = IODOut.GetActualOffset() ;
		IODOut.End() ;

//		m_poCsTrace->PreMapTrace() ;

		iResult = SendCommand( poOutputDataStorage, poInputDataStorage ) ;

		if( ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED )
		{
			//Ponemos el tamano de la PDU a las Stt
			ai_poSttWrapper->GetStatistics()->sttAddOutputPDUSize( poOutputDataStorage->Size() ) ;
			ai_poSttWrapper->GetStatistics()->sttAddInputPDUSize( poInputDataStorage->Size() ) ;
		}
	}

	if( iResult == M4_SUCCESS )
	{
		iResult = IODIn.Init( GEN_IO_MODE_READ, poInputDataStorage, iArchitecture ) ;
	}

	if( iResult == M4_SUCCESS )
	{
		iResult = BuildMapFromResponse( IODIn, ao_poMappings ) ;

/*
		if( m_poCsTrace->IsEnabled() == M4_TRUE )
		{
			m4pchar_t pcRole = m_poChannelManager->GetLogonAdaptor()->GetRole() ;
			m_poCsTrace->PostMapTrace( iOutputSize, IODIn.GetActualOffset(), ai_pccMapId, pcRole, M4_FALSE ) ;
		}
*/
		IODIn.End() ;
	}


	M4DataStorage::DeleteDataStorage( poOutputDataStorage ) ; 
	M4DataStorage::DeleteDataStorage( poInputDataStorage ) ; 

	if( iResult == M4_ERROR )
	{
		ao_poMappings = NULL ;
		DUMP_CHLOG_ERRORF( M4_CS_MCR_NO_CREATE_MAP, ai_pccMapId ) ;
		return M4_ERROR;
	}
	
	return( M4_SUCCESS ) ;
}


//=================================================================================
// seguridad
//=================================================================================
m4return_t ClMDAdaptorImp::RemoteGetCSCR(m4uint8_t ai_iCsType, 
									 m4pcchar_t ai_pccChannelId, 
									 m4pcchar_t ai_pccRSM,									 
									 ClCSCRWrapper * &ao_poCSCRWrap,
									 ClVMStatisticsWrapper* ai_poSttWrapper)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;
	
	m4return_t iResult = M4_ERROR;

	if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		//Ponemos el servicio a las Stt
		ai_poSttWrapper->GetStatistics()->sttAddService("METADA_SERVICE");
	}

	m4uint16_t iArchitecture = M4_ARCH_INTEL;

	iArchitecture = m_poChannelManager->GetLogonAdaptor()->GetServerArchitecture();

	M4DataStorage *poInputDataStorage = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poOutputDataStorage = M4DataStorage::GetNewDataStorage();

	ClDataStoreIODriver IODIn, IODOut;

	iResult = IODOut.Init(GEN_IO_MODE_WRITE, poOutputDataStorage, iArchitecture);

	if (iResult == M4_ERROR)
	{
		M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
		M4DataStorage::DeleteDataStorage(poInputDataStorage); 
		DUMP_CHLOG_ERRORF( M4_CS_MCR_NO_CREATE_SECURITY, ai_pccChannelId << ai_pccRSM << M4ClCsType(ai_iCsType) ) ;
		return M4_ERROR;
	}

	iResult = IODOut.Write((m4uint16_t)M4_SERVICE_METADATA_CSCR);

	if (iResult != M4_ERROR)
	{
		iResult = BuildCSCRRequest (IODOut, ai_iCsType, ai_pccChannelId, ai_pccRSM);

		if (iResult != M4_ERROR)
		{
			m4uint32_t iOutputSize = IODOut.GetActualOffset();

			IODOut.End();

			m_poCsTrace->PreSecurityTrace();
			
			iResult = SendCommand(poOutputDataStorage, poInputDataStorage);

			if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
			{
				//Ponemos el tamano de la PDU a las Stt
				ai_poSttWrapper->GetStatistics()->sttAddOutputPDUSize(poOutputDataStorage->Size());
				
				ai_poSttWrapper->GetStatistics()->sttAddInputPDUSize(poInputDataStorage->Size());
			}

			if (iResult == M4_SUCCESS)
			{
				IODIn.Init(GEN_IO_MODE_READ, poInputDataStorage, iArchitecture);

				iResult = BuildCSCRFromResponse (IODIn, ao_poCSCRWrap);

				if( m_poCsTrace->IsEnabled() == M4_TRUE )
				{
					m4pchar_t pcRole = m_poChannelManager->GetLogonAdaptor()->GetRole() ;

					m_poCsTrace->PostSecurityTrace( iOutputSize, IODIn.GetActualOffset(), (m4pchar_t)ai_pccChannelId, ai_iCsType, pcRole, M4_FALSE);
				}

				IODIn.End();

				if (iResult == M4_ERROR)
				{
					//hasta que haya NO_UPDATE en la cache
					M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
					M4DataStorage::DeleteDataStorage(poInputDataStorage); 
					// de debug porque ya debe estar en la pila
					DUMP_CHLOG_DEBUGF( M4_CS_MCR_NO_CREATE_SECURITY, ai_pccChannelId << ai_pccRSM << M4ClCsType(ai_iCsType) ) ;
					return M4_ERROR;
				}						
			}
			else
			{
				M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
				M4DataStorage::DeleteDataStorage(poInputDataStorage); 
				DUMP_CHLOG_ERRORF( M4_CS_MCR_NO_CREATE_SECURITY, ai_pccChannelId << ai_pccRSM << M4ClCsType(ai_iCsType) ) ;
				return M4_ERROR;
			}
		}
		else
		{
			M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
			M4DataStorage::DeleteDataStorage(poInputDataStorage); 
			DUMP_CHLOG_ERRORF( M4_CS_MCR_NO_CREATE_SECURITY, ai_pccChannelId << ai_pccRSM << M4ClCsType(ai_iCsType) ) ;
			return M4_ERROR;
		}
	}

	M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
	M4DataStorage::DeleteDataStorage(poInputDataStorage); 

	return M4_SUCCESS;
}

//=================================================================================
// presentación
//=================================================================================
m4return_t ClMDAdaptorImp::RemoteGetPresentation(m4pcchar_t ai_pcIdPresentation,
											 const ClPresentationAttributes & ai_ClPresentationAttributes,
											 m4bool_t ai_bDefinition,
											 ClPresentation * &ao_poPresentation,
	 										 ClVMStatisticsWrapper* ai_poSttWrapper)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;
	
	m4return_t iResult = M4_ERROR;

	if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		//Ponemos el servicio a las Stt
		ai_poSttWrapper->GetStatistics()->sttAddService("METADA_SERVICE");
	}

	m4uint16_t iArchitecture = M4_ARCH_INTEL;

	iArchitecture = m_poChannelManager->GetLogonAdaptor()->GetServerArchitecture();

	M4DataStorage *poInputDataStorage = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poOutputDataStorage = M4DataStorage::GetNewDataStorage();

	ClDataStoreIODriver IODIn, IODOut;

	iResult = IODOut.Init(GEN_IO_MODE_WRITE, poOutputDataStorage, iArchitecture);

	if (iResult == M4_ERROR)
	{
		//hasta que haya NO_UPDATE en la cache
		M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
		M4DataStorage::DeleteDataStorage(poInputDataStorage); 
		g_oChLog<<BeginError(M4_CH_CS_NO_BUILD_PRESENTATION)<<ai_pcIdPresentation<<ai_ClPresentationAttributes.m_pRole<<EndLog;
		return M4_ERROR;
	}

	iResult = IODOut.Write((m4uint16_t)M4_SERVICE_METADATA_PRESENTATION);

	if (iResult != M4_ERROR)
	{
		iResult = BuildPresentationRequest (IODOut, ai_pcIdPresentation, ai_ClPresentationAttributes, ai_bDefinition);

		if (iResult != M4_ERROR)
		{
			m4uint32_t iOutputSize=IODOut.GetActualOffset();

			IODOut.End();

			m_poCsTrace->PrePresentationTrace();

			iResult = SendCommand(poOutputDataStorage, poInputDataStorage);

			if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
			{
				//Ponemos el tamano de la PDU a las Stt
				ai_poSttWrapper->GetStatistics()->sttAddOutputPDUSize(poOutputDataStorage->Size());
				
				ai_poSttWrapper->GetStatistics()->sttAddInputPDUSize(poInputDataStorage->Size());
			}

			if (iResult != M4_ERROR)
			{
				IODIn.Init(GEN_IO_MODE_READ, poInputDataStorage, iArchitecture);

				iResult = BuildPresentationFromResponse (IODIn, ao_poPresentation);

				m_poCsTrace->PostPresentationTrace( iOutputSize, IODIn.GetActualOffset(), ai_pcIdPresentation, ai_ClPresentationAttributes, M4_FALSE);

				IODIn.End();

				if (iResult == M4_ERROR)
				{
					//hasta que haya NO_UPDATE en la cache
					M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
					M4DataStorage::DeleteDataStorage(poInputDataStorage); 
					g_oChLog<<BeginDebug(M4_CH_CS_NO_BUILD_PRESENTATION)<<ai_pcIdPresentation<<ai_ClPresentationAttributes.m_pRole<<EndLog;
					return M4_ERROR;
				}
			}
			else
			{
				M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
				M4DataStorage::DeleteDataStorage(poInputDataStorage); 
				g_oChLog<<BeginError(M4_CH_CS_NO_BUILD_PRESENTATION)<<ai_pcIdPresentation<<ai_ClPresentationAttributes.m_pRole<<EndLog;
				return M4_ERROR;
			}
		}
		else
		{
			M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
			M4DataStorage::DeleteDataStorage(poInputDataStorage); 
			g_oChLog<<BeginError(M4_CH_CS_NO_BUILD_PRESENTATION)<<ai_pcIdPresentation<<ai_ClPresentationAttributes.m_pRole<<EndLog;
			return M4_ERROR;
		}
	}
	M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
	M4DataStorage::DeleteDataStorage(poInputDataStorage); 

	return M4_SUCCESS;
}

//=================================================================================
// MDUpdateList
//=================================================================================
m4return_t ClMDAdaptorImp::RemoteGetMDUpdatedList(ClCacheDirectory * ai_pCacheDirectory,
											  ClMDUpdateDeltasList *& ao_poUpdatedDeltasList,
											  ClVMStatisticsWrapper* ai_poSttWrapper)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;
	
	m4return_t iResult = M4_ERROR;

	if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		//Ponemos el servicio a las Stt
		ai_poSttWrapper->GetStatistics()->sttAddService("METADA_SERVICE");
	}

	m4uint16_t iArchitecture = M4_ARCH_INTEL;

	iArchitecture = m_poChannelManager->GetLogonAdaptor()->GetServerArchitecture();

	M4DataStorage *poInputDataStorage = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poOutputDataStorage = M4DataStorage::GetNewDataStorage();

	ClDataStoreIODriver IODIn, IODOut;

	iResult = IODOut.Init(GEN_IO_MODE_WRITE, poOutputDataStorage, iArchitecture);

	if (iResult == M4_ERROR)
	{
		M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
		M4DataStorage::DeleteDataStorage(poInputDataStorage); 
		g_oChLog<<BeginError(M4_CH_CS_NO_MD_GET_UPDATE_LIST)<<EndLog;
		return M4_ERROR;
	}

	iResult = IODOut.Write((m4uint16_t)M4_SERVICE_METADATA_GET_UPDATE_LIST);

	if (iResult != M4_ERROR)
	{
		iResult = BuildUpdatedListRequest (IODOut, ai_pCacheDirectory);

		if (iResult != M4_ERROR)
		{
			IODOut.End();

			iResult = SendCommand(poOutputDataStorage, poInputDataStorage);

			if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
			{
				//Ponemos el tamano de la PDU a las Stt
				ai_poSttWrapper->GetStatistics()->sttAddOutputPDUSize(poOutputDataStorage->Size());
				
				ai_poSttWrapper->GetStatistics()->sttAddInputPDUSize(poInputDataStorage->Size());
			}

			if (iResult != M4_ERROR)
			{
				IODIn.Init(GEN_IO_MODE_READ, poInputDataStorage, iArchitecture);

				iResult = BuildUpdatedListFromResponse (IODIn, ao_poUpdatedDeltasList);

				IODIn.End();

				if (iResult == M4_ERROR)
				{
					//hasta que haya NO_UPDATE en la cache
					M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
					M4DataStorage::DeleteDataStorage(poInputDataStorage); 
					g_oChLog<<BeginDebug(M4_CH_CS_NO_MD_GET_UPDATE_LIST)<<EndLog;
					return M4_ERROR;
				}
			}
			else
			{
				M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
				M4DataStorage::DeleteDataStorage(poInputDataStorage); 
				g_oChLog<<BeginError(M4_CH_CS_NO_MD_GET_UPDATE_LIST)<<EndLog;
				return M4_ERROR;
			}
		}
		else
		{
			M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
			M4DataStorage::DeleteDataStorage(poInputDataStorage); 
			g_oChLog<<BeginError(M4_CH_CS_NO_MD_GET_UPDATE_LIST)<<EndLog;
			return M4_ERROR;
		}
	}
	M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
	M4DataStorage::DeleteDataStorage(poInputDataStorage); 


	return M4_SUCCESS;
}

//=================================================================================
// CacheRemove
//=================================================================================
m4return_t ClMDAdaptorImp::RemoteCacheRemove(m4uint16_t ai_iCacheType, 
										 m4pcchar_t ai_pcId,
	 									 ClVMStatisticsWrapper* ai_poSttWrapper)
{
	if (M4_FALSE == m_bInit)
		return M4_ERROR;
	
	m4return_t iResult = M4_ERROR;

	if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
	{
		//Ponemos el servicio a las Stt
		ai_poSttWrapper->GetStatistics()->sttAddService("METADA_SERVICE");
	}

	m4uint16_t iArchitecture = M4_ARCH_INTEL;

	iArchitecture = m_poChannelManager->GetLogonAdaptor()->GetServerArchitecture();

	M4DataStorage *poInputDataStorage = M4DataStorage::GetNewDataStorage();
	M4DataStorage *poOutputDataStorage = M4DataStorage::GetNewDataStorage();

	ClDataStoreIODriver IODIn, IODOut;

	iResult = IODOut.Init(GEN_IO_MODE_WRITE, poOutputDataStorage, iArchitecture);

	if (iResult == M4_ERROR)
	{
		//hasta que haya NO_UPDATE en la cache
		M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
		M4DataStorage::DeleteDataStorage(poInputDataStorage); 
		g_oChLog<<BeginError(M4_CH_CS_NO_MD_SERVER_CACHE_REMOVE_BY_ID)<<ai_pcId<<ai_iCacheType<<EndLog;
		return M4_ERROR;
	}

	iResult = IODOut.Write((m4uint16_t)M4_SERVICE_METADATA_CACHE_REMOVE);

	if (iResult != M4_ERROR)
	{
		iResult = BuildCacheRemoveRequest (IODOut, ai_iCacheType, ai_pcId);

		if (iResult != M4_ERROR)
		{
			IODOut.End();

			iResult = SendCommand(poOutputDataStorage, poInputDataStorage);

			if (ai_poSttWrapper->GetStatisticsLevel() != M4_VM_STATISTICS_DISABLED)
			{
				//Ponemos el tamano de la PDU a las Stt
				ai_poSttWrapper->GetStatistics()->sttAddOutputPDUSize(poOutputDataStorage->Size());
				
				ai_poSttWrapper->GetStatistics()->sttAddInputPDUSize(poInputDataStorage->Size());
			}

			if (iResult != M4_ERROR)
			{
				IODIn.Init(GEN_IO_MODE_READ, poInputDataStorage, iArchitecture);

				iResult = ReadCacheRemoveResponse (IODIn);

				IODIn.End();

				if (iResult == M4_ERROR)
				{
					M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
					M4DataStorage::DeleteDataStorage(poInputDataStorage); 
					g_oChLog<<BeginDebug(M4_CH_CS_NO_MD_SERVER_CACHE_REMOVE_BY_ID)<<ai_pcId<<ai_iCacheType<<EndLog;
					return M4_ERROR;
				}
			}
			else
			{
				M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
				M4DataStorage::DeleteDataStorage(poInputDataStorage); 
				g_oChLog<<BeginError(M4_CH_CS_NO_MD_SERVER_CACHE_REMOVE_BY_ID)<<ai_pcId<<ai_iCacheType<<EndLog;
				return M4_ERROR;
			}
		}
		else
		{
			M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
			M4DataStorage::DeleteDataStorage(poInputDataStorage); 
			g_oChLog<<BeginError(M4_CH_CS_NO_MD_SERVER_CACHE_REMOVE_BY_ID)<<ai_pcId<<ai_iCacheType<<EndLog;
			return M4_ERROR;
		}
	}
	M4DataStorage::DeleteDataStorage(poOutputDataStorage); 
	M4DataStorage::DeleteDataStorage(poInputDataStorage); 


	return M4_SUCCESS;
}


m4return_t ClMDAdaptorImp::SendCommand (M4DataStorage *ai_poInputData, M4DataStorage *ai_pOutputData)
{
	if (M4_FALSE == m_bInit)
		return M4_SUCCESS;

	m4return_t	iResult = M4_ERROR;
	m4handle_t	hRequestID;

	iResult = m_poClientAPI->SyncRequest(MD_SERVICE_ID,  0,  0, ai_poInputData, ai_pOutputData, hRequestID, m_iGenCSTimeOut);

	return iResult;
}