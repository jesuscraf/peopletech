#include "csres.hpp"
#include "chlog.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              exeforchannel
// File:                mdprotocol.cpp
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


#include "mdprotocol.hpp"
#include "m4mdrt.hpp"
#include "m4mdfac.hpp"
#include "clmappings.hpp"
#include "clpresen.hpp"
#include "presfac.hpp"
#include "exechannel.hpp"
#include "cachedirsrv.hpp"
#include "mdupdate.hpp"
#include "dm.hpp"
#include "sttwrap.hpp"
#include "cllstr.hpp"
#include "m4regimpl.hpp"
#include "m4unicode.hpp"
#include "m4logfile.hpp"
#include "execlogs.hpp"


void dumpMetadata( m4pcchar_t ai_pccType, m4pcchar_t ai_pccChannel )
{
	M4ThreadLogger*	pThreadLogger = NULL;

	if( ClVMDebugEnv::getInstance()->getDebugLevel() < 1 )
	{
		return ;
	}

	// m4metadata.log trace.
	pThreadLogger = M4ThreadLogger::GetInstance( MD_LOG );

	if( pThreadLogger != NULL )
	{
		pThreadLogger->InitLogFile( "m4metadata", 5 * 1024 * 1024, "TickCount\tThreadId\tObjectType\tObjectId\tSessionKey\tServerHost\tServerPort\tSessionId\tUserClick\tReqUniqueID", "log", M4_TRUE, M4_FALSE, M4_TRUE );
		pThreadLogger->WriteLine( "\t%s\t%s", ai_pccType, ai_pccChannel );
	}
}



m4return_t BuildCMCRRequest (ClDataStoreIODriver &IODOut, m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccOrgId, m4uint8_t ai_iLanguage, m4bool_t ai_bApplyMappings, m4VariantType &ai_roStartDate, m4VariantType &ai_roEndDate, m4VariantType &ai_roCorDate)
{
	m4return_t ret = M4_SUCCESS;

	if (!ai_pccChannelId)
		return M4_ERROR;

	ret = IODOut.Write(ai_iCsType);

	if (ret == M4_ERROR)
		return M4_ERROR;


	ret = IODOut.Write(ai_pccChannelId);

	if (ret == M4_ERROR)
		return M4_ERROR;

	m4bool_t bIsOrg = (ai_pccOrgId != NULL) ? M4_TRUE : M4_FALSE;
	ret = IODOut.Write(bIsOrg);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (bIsOrg == M4_TRUE)
	{
		ret = IODOut.Write(ai_pccOrgId);
		
		if (ret == M4_ERROR)
			return M4_ERROR;
	}

	ret = IODOut.Write(ai_roStartDate.Data.DoubleData);

	if (ret == M4_ERROR)
		return M4_ERROR;


	ret = IODOut.Write(ai_roEndDate.Data.DoubleData);

	if (ret == M4_ERROR)
		return M4_ERROR;


	ret = IODOut.Write(ai_roCorDate.Data.DoubleData);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IODOut.Write(ai_iLanguage);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IODOut.Write(ai_bApplyMappings);

	return ret;
}


m4return_t BuildCMCRFromRequest (ClCMCRFactory * ai_poCMCRFact, ClDataStoreIODriver &IODIn, ClCMCRWrapper * &ao_poCMCRWrap, ExecDumpInfo *aio_poDumpInfo )
{
	m4return_t ret = M4_SUCCESS;
    
	m4uint32_t iSize;
	m4bool_t bApplyMappings;
	m4uint8_t iCsType;
	m4pchar_t pcChannelId=NULL, pcOrgId=NULL; 
	m4uint8_t iLanguage;
	m4VariantType vStartDate, vEndDate, vCorDate;

	if (!ai_poCMCRFact)
		return M4_ERROR;

	vStartDate.Type = vEndDate.Type = vCorDate.Type = M4CL_CPP_TYPE_DATE;

	ret = IODIn.Read(iCsType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (aio_poDumpInfo != NULL)
		*aio_poDumpInfo << "iCsType" << ": " << (m4int32_t) iCsType << endl;

	ret = IODIn.ReadSize(iSize);
    
	if (ret==M4_ERROR)
		return M4_ERROR;

    if (!iSize)
		return M4_ERROR;

	pcChannelId = new m4char_t[iSize+1];
	ret = IODIn.ReadBuffer(pcChannelId);
	pcChannelId[iSize] = '\0';

	if (ret != M4_ERROR)
	{
		dumpMetadata("MCR", pcChannelId);

		if (aio_poDumpInfo != NULL)
			*aio_poDumpInfo << "pcChannelId" << ": " << pcChannelId << endl;
		
		m4bool_t bIsOrg = M4_TRUE;
		ret = IODIn.Read(bIsOrg);
		if (ret != M4_ERROR)
		{
			if (bIsOrg == M4_TRUE)
			{
				ret = IODIn.ReadSize(iSize);
				if (ret != M4_ERROR)
				{
					pcOrgId = new m4char_t[iSize+1];
					ret = IODIn.ReadBuffer(pcOrgId);
					pcOrgId[iSize] = '\0';
				}
			}
		}
		
		if (ret != M4_ERROR)
		{
			
			if (aio_poDumpInfo != NULL)
			{
				*aio_poDumpInfo << "pcOrgId" << ": ";
				if (pcOrgId != NULL)
					*aio_poDumpInfo << pcOrgId;
				else
					*aio_poDumpInfo << "<NULL>";
					
				*aio_poDumpInfo << endl;
			}

			ret = IODIn.Read(vStartDate.Data.DoubleData);
			
			if (ret != M4_ERROR)
			{
				if (aio_poDumpInfo != NULL)
					*aio_poDumpInfo << "vStartDate.Data.DoubleData" << ": " << vStartDate.Data.DoubleData << endl;
				
				ret = IODIn.Read(vEndDate.Data.DoubleData);
				
				if (ret != M4_ERROR)
				{
					if (aio_poDumpInfo != NULL)
						*aio_poDumpInfo << "vEndDate.Data.DoubleData" << ": " << vEndDate.Data.DoubleData << endl;
					
					ret = IODIn.Read(vCorDate.Data.DoubleData);
					
					if (ret != M4_ERROR)
					{
						if (aio_poDumpInfo != NULL)
							*aio_poDumpInfo << "vCorDate.Data.DoubleData" << ": " << vCorDate.Data.DoubleData << endl;
						
						ret = IODIn.Read(iLanguage);
						
						if (ret != M4_ERROR)
						{
							if (aio_poDumpInfo != NULL)
								*aio_poDumpInfo << "iLanguage" << ": " << (m4int32_t) iLanguage << endl;

							ret = IODIn.Read(bApplyMappings);
						
							if (ret != M4_ERROR)
							{
								if (aio_poDumpInfo != NULL)
								{
									*aio_poDumpInfo << "bApplyMappings" << ": " << (m4int32_t) bApplyMappings << endl;
									aio_poDumpInfo->SetCurrentStage("BuildCMCRFromRequest : MDFac_BuildMDWrapper");
								}
								
								ret = ai_poCMCRFact->MDFac_BuildMDWrapper( pcChannelId, pcOrgId, iCsType, vStartDate, vEndDate, vCorDate, iLanguage, bApplyMappings, ao_poCMCRWrap) ;
							}
						}
					}
				}
			}
		}
	}
	
	if (pcOrgId != NULL)
		delete [] pcOrgId;
	
	delete [] pcChannelId;

	if (ret == M4_ERROR)
		ao_poCMCRWrap = 0;
	
	return ret;	
}


m4return_t BuildResponseFromCMCR (ClDataStoreIODriver &IODOut, ClCMCRWrapper * ai_poCMCRWrap, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS, response;

	if (ai_poCMCRWrap)
		response = M4_SUCCESS;
	else
		response = M4_ERROR;

	ret = IODOut.Write(response);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (response == M4_SUCCESS)
	{
		ret = ai_poCMCRWrap->Serialize(IODOut);
	}

	return ret;
}


m4return_t BuildCMCRFromResponse (ClDataStoreIODriver &IODIn, ClCMCRWrapper * &ao_poCMCRWrap)
{
	m4return_t ret = M4_ERROR, response;
	
	ret = IODIn.Read(response);

	if (ret == M4_ERROR)
	{
		g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_NO_RESPONSE)<<EndLog;
		return M4_ERROR;
	}

	if (response == M4_SUCCESS)
	{
		ao_poCMCRWrap = new ClCMCRWrapper();

		ret = ao_poCMCRWrap->DeSerialize(IODIn);

		if (ret == M4_ERROR)
		{
			g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_ERROR_DESERIALIZING)<<EndLog;
			delete ao_poCMCRWrap;
			ao_poCMCRWrap = 0;
		}
	}
	else
	{
		g_oChLog<<BeginDebug(M4_CH_CS_MD_SERVICE_SERVER_ERROR)<<EndLog;
		ret = M4_ERROR;
	}

	return ret;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


m4return_t BuildMapRequest (ClDataStoreIODriver &IODOut, m4pcchar_t ai_pccMapId)
{
	m4return_t ret = M4_SUCCESS;

	if (!ai_pccMapId)
		return M4_ERROR;

	ret = IODOut.Write(ai_pccMapId);

	return ret;
}


m4return_t BuildMapFromRequest (ClCMCRFactory * ai_poCMCRFact, ClDataStoreIODriver &IODIn, ClMappings* &ao_poMappings, ExecDumpInfo *aio_poDumpInfo )
{
	m4return_t ret = M4_SUCCESS;
    
	m4uint32_t iSize;
	m4pchar_t pcMaplId=NULL;

	if (!ai_poCMCRFact)
		return M4_ERROR;

	ret = IODIn.ReadSize(iSize);
    
	if (ret==M4_ERROR)
		return M4_ERROR;

    if (!iSize)
		return M4_ERROR;

	pcMaplId = new m4char_t[iSize+1];
	ret = IODIn.ReadBuffer(pcMaplId);
	pcMaplId[iSize] = '\0';

	if (ret != M4_ERROR)
	{
		dumpMetadata("Map", pcMaplId);

		if (aio_poDumpInfo != NULL)
		{
			*aio_poDumpInfo << "pcMaplId" << ": " << pcMaplId << endl;
			aio_poDumpInfo->SetCurrentStage("BuildMapFromRequest : MDFac_BuildMappings");
		}
		ret = ai_poCMCRFact->MDFac_BuildMappings( pcMaplId, ao_poMappings ) ;
	}
	
	delete [] pcMaplId;

	if (ret == M4_ERROR)
		ao_poMappings = 0;
	
	return ret;	
}


m4return_t BuildResponseFromMap (ClDataStoreIODriver &IODOut, ClMappings * ao_poMappings, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS, response;

	if (ao_poMappings)
		response = M4_SUCCESS;
	else
		response = M4_ERROR;

	ret = IODOut.Write(response);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (response == M4_SUCCESS)
	{
		ret = ao_poMappings->Serialize(IODOut);
	}

	return ret;
}


m4return_t BuildMapFromResponse (ClDataStoreIODriver &IODIn, ClMappings* &ao_poMappings)
{
	m4return_t ret = M4_ERROR, response;
	
	ret = IODIn.Read(response);

	if (ret == M4_ERROR)
	{
		g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_NO_RESPONSE)<<EndLog;
		return M4_ERROR;
	}

	if (response == M4_SUCCESS)
	{
		ao_poMappings = new ClMappings;

		ret = ao_poMappings->DeSerialize(IODIn);

		if (ret == M4_ERROR)
		{
			g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_ERROR_DESERIALIZING)<<EndLog;
			delete ao_poMappings;
			ao_poMappings = 0;
		}
	}
	else
	{
		g_oChLog<<BeginDebug(M4_CH_CS_MD_SERVICE_SERVER_ERROR)<<EndLog;
		ret = M4_ERROR;
	}

	return ret;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

m4return_t BuildCSCRRequest (ClDataStoreIODriver &IODOut, m4uint8_t ai_iCsType, m4pcchar_t ai_pccChannelId, m4pcchar_t ai_pccMask)
{
	m4return_t ret = M4_SUCCESS;

	if (!ai_pccChannelId)
		return M4_ERROR;

	if (!ai_pccMask)
		return M4_ERROR;

	ret = IODOut.Write(ai_iCsType);

	if (ret == M4_ERROR)
		return M4_ERROR;


	ret = IODOut.Write((void*)ai_pccChannelId, strlen(ai_pccChannelId)+1);

	if (ret == M4_ERROR)
		return M4_ERROR;


	ret = IODOut.Write((void*)ai_pccMask, strlen(ai_pccMask)+1);

	return ret;
}

m4return_t BuildCSCRFromRequest (ClCMCRFactory * ai_poCMCRFact, ClDataStoreIODriver &IODIn, ClCSCRWrapper * &ao_poCSCRWrap, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS;
    
	m4uint32_t iSize;
	m4uint8_t iCsType;
	m4pchar_t pcChannelId, pcMask; 
	m4VariantType vStartDate, vEndDate, vCorDate;

	vStartDate.Type = vEndDate.Type = vCorDate.Type = M4CL_CPP_TYPE_DATE;

	if (!ai_poCMCRFact)
		return M4_ERROR;

	ret = IODIn.Read(iCsType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (aio_poDumpInfo != NULL)
		*aio_poDumpInfo << "iCsType" << ": " << (m4int32_t) iCsType << endl;

	ret = IODIn.ReadSize(iSize);
    
	if (ret==M4_ERROR)
		return M4_ERROR;

    if (!iSize)
		return M4_ERROR;

	pcChannelId = new m4char_t[iSize+1];
	ret = IODIn.ReadBuffer(pcChannelId);
	pcChannelId[iSize] = '\0';

	if (ret != M4_ERROR)
	{
		dumpMetadata("SECURITY", pcChannelId);

		if (aio_poDumpInfo != NULL)
			*aio_poDumpInfo << "pcChannelId" << ": " << pcChannelId << endl;
		
		ret = IODIn.ReadSize(iSize);

		if ( (ret!=M4_ERROR) && (iSize) )
		{
			pcMask = new m4char_t[iSize+1];
			ret = IODIn.ReadBuffer(pcMask);
			pcMask[iSize] = '\0';

			if (ret != M4_ERROR)
			{
				if (aio_poDumpInfo != NULL)
					aio_poDumpInfo->SetCurrentStage("BuildCSCRFromRequest : MDFac_BuildSecWrapper");
				
				ret = ai_poCMCRFact->MDFac_BuildSecWrapper( pcChannelId, iCsType, pcMask, M4CL_LANGUAGE_NONE, ao_poCSCRWrap ) ;
			}

			delete [] pcMask;
		}
	}

	delete [] pcChannelId;

	if (ret == M4_ERROR)
		ao_poCSCRWrap = 0;
	
	return ret;	
}

m4return_t BuildResponseFromCSCR (ClDataStoreIODriver &IODOut, ClCSCRWrapper * ai_poCSCRWrap, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS, response;

	if (ai_poCSCRWrap)
		response = M4_SUCCESS;
	else
		response = M4_ERROR;

	ret = IODOut.Write(response);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (response == M4_SUCCESS)
	{
		ret = ai_poCSCRWrap->Serialize(IODOut);
	}

	return ret;
}


m4return_t BuildCSCRFromResponse (ClDataStoreIODriver &IODIn, ClCSCRWrapper * &ao_poCSCRWrap)
{
	m4return_t ret = M4_ERROR, response;
	
	ret = IODIn.Read(response);

	if (ret == M4_ERROR)
	{
		g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_NO_RESPONSE)<<EndLog;
		return M4_ERROR;
	}

	if (response == M4_SUCCESS)
	{
		ao_poCSCRWrap = new ClCSCRWrapper();

		ret = ao_poCSCRWrap->DeSerialize(IODIn);

		if (ret == M4_ERROR)
		{			
			g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_ERROR_DESERIALIZING)<<EndLog;
			delete ao_poCSCRWrap;
			ao_poCSCRWrap = 0;
		}
	}
	else
	{
		g_oChLog<<BeginDebug(M4_CH_CS_MD_SERVICE_SERVER_ERROR)<<EndLog;
		ret = M4_ERROR;
	}

	return ret;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


m4return_t BuildPresentationRequest (ClDataStoreIODriver &IODOut, m4pcchar_t ai_pcIdPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition)
{
	m4return_t ret = M4_SUCCESS;

	if (!ai_pcIdPresentation)
		return M4_ERROR;

	if (ai_ClPresentationAttributes.m_pOrganization == NULL) {
		return M4_ERROR;
	}

	if (ai_ClPresentationAttributes.m_pRole == NULL)
		return M4_ERROR;

	ret = IODOut.Write(ai_pcIdPresentation);

	if (ret == M4_ERROR)
		return M4_ERROR;

#ifdef METADATA_PROTOCOL_COMPATIBILITY
	//Para compatibilidad con servers 180.3.1 o anterior.
	ret = IODOut.Write("PEPE");

	if (ret == M4_ERROR)
		return M4_ERROR;
#endif

	ret = IODOut.Write(ai_ClPresentationAttributes.m_pOrganization);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IODOut.Write(ai_ClPresentationAttributes.m_pRole);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IODOut.Write(ai_ClPresentationAttributes.m_iLanguage);
	
	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IODOut.Write(ai_bDefinition);

	return ret;
}

m4return_t BuildPresentationFromRequest (ClPresentationFactory * ai_poPresFact, ClDataStoreIODriver &IODIn, ClPresentation * &ao_poPresentation, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS;    
	m4uint32_t iSize;
	m4pchar_t pcIdPres, pcOrganization, pcRole;
	m4uint8_t iLanguage;
	m4bool_t bDefinition = M4_FALSE; 

	if (!ai_poPresFact)
		return M4_ERROR;

	ret = IODIn.ReadSize(iSize);
    
	if (ret==M4_ERROR)
		return M4_ERROR;

    if (!iSize)
		return M4_ERROR;

	pcIdPres = new m4char_t[iSize+1];
	ret = IODIn.ReadBuffer(pcIdPres);
	pcIdPres[iSize] = '\0';

	if (ret != M4_ERROR)
	{
		dumpMetadata("PRESENTATION", pcIdPres);

		if (aio_poDumpInfo != NULL)
			*aio_poDumpInfo << "pcIdPres" << ": " << pcIdPres << endl;
		
#ifdef METADATA_PROTOCOL_COMPATIBILITY
		//Para compatibilidad con servers 180.3.1 o anterior.
		ret = IODIn.ReadSize(iSize);

		if ( (ret!=M4_ERROR) && (iSize) )
		{
			m4pchar_t pcUse = new m4char_t[iSize+1];
			ret = IODIn.ReadBuffer(pcUse);
			pcUse[iSize] = '\0';
		}
#endif

		ret = IODIn.ReadSize(iSize);

		if (ret!=M4_ERROR)
		{
			pcOrganization = new m4char_t[iSize+1];
			ret = IODIn.ReadBuffer(pcOrganization);
			pcOrganization[iSize] = '\0';

			if (ret != M4_ERROR)
			{
				if (aio_poDumpInfo != NULL) {
					*aio_poDumpInfo << "pcOrganization" << ": " << pcOrganization << endl;
				}			

				ret = IODIn.ReadSize(iSize);
				if ((ret != M4_ERROR) && (iSize) ) {

					pcRole = new m4char_t[iSize+1];
					ret = IODIn.ReadBuffer(pcRole);
					pcRole[iSize] = '\0';

					if (ret != M4_ERROR) {
						if (aio_poDumpInfo != NULL) {
							*aio_poDumpInfo << "pcRole" << ": " << pcRole << endl;			
						}

						ret = IODIn.Read(iLanguage);
					
						if (ret != M4_ERROR)
						{
							if (aio_poDumpInfo != NULL)
							{
								*aio_poDumpInfo << "iLanguage" << ": " << (m4uint16_t)iLanguage << endl;
							}
					
							ret = IODIn.Read(bDefinition);
						
							if (ret != M4_ERROR)
							{
								if (aio_poDumpInfo != NULL)
								{
									*aio_poDumpInfo << "bDefinition" << ": " << (m4uint16_t)iLanguage << endl;
									aio_poDumpInfo->SetCurrentStage("BuildPresentationFromRequest : BuildPresentationFromId");
								}
					
							ret = ai_poPresFact->BuildPresentationFromId( pcIdPres, pcOrganization, pcRole, iLanguage, M4_TRUE, bDefinition, ao_poPresentation ) ;
							
							}							
						
						}
					}

					delete [] pcRole;
				}
			}

			delete [] pcOrganization;
		}
	}

	delete [] pcIdPres;

	if (ret == M4_ERROR)
		ao_poPresentation = 0;
	
	return ret;	
}


m4return_t BuildResponseFromPresentation (ClDataStoreIODriver &IODOut, ClPresentation * ao_poPresentation, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS, response;

	if (ao_poPresentation)
		response = M4_SUCCESS;
	else
		response = M4_ERROR;

	ret = IODOut.Write(response);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (response == M4_SUCCESS)
	{
		ret = ao_poPresentation->Serialize(IODOut);
	}

	return ret;
}


m4return_t BuildPresentationFromResponse (ClDataStoreIODriver &IODIn, ClPresentation * &ao_poPresentation)
{
	m4return_t ret = M4_ERROR, response;
	
	ret = IODIn.Read(response);

	if (ret == M4_ERROR)
	{
		g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_NO_RESPONSE)<<EndLog;
		return M4_ERROR;
	}

	if (response == M4_SUCCESS)
	{
		ao_poPresentation = new ClPresentation();

		ret = ao_poPresentation->DeSerialize(IODIn);

		if (ret == M4_ERROR)
		{
			g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_ERROR_DESERIALIZING)<<EndLog;
			delete ao_poPresentation;
			ao_poPresentation = 0;
		}
	}
	else
	{
		g_oChLog<<BeginDebug(M4_CH_CS_MD_SERVICE_SERVER_ERROR)<<EndLog;
		ret = M4_ERROR;
	}

	return ret;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


m4return_t BuildUpdatedListRequest (ClDataStoreIODriver &IODOut, ClCacheDirectory * ai_pCacheDirectory)
{
	m4return_t ret = M4_SUCCESS;

	ret = ai_pCacheDirectory->Serialize(IODOut);

	return ret;
}

m4return_t BuildUpdatedListFromRequest (ClChannelManager * ai_pChannelManager, ClMDUpdateList * ai_poUpdateList, ClDataStoreIODriver &IODIn, ClMDUpdateDeltasList *& ao_poMDUpdateDeltasList, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS;

	ClCacheDirectoryServer oCacheDirectoryServer;
	ClVMStatisticsWrapper oSttWrapper(M4_FALSE);

	oCacheDirectoryServer.Init(0, &oSttWrapper, ai_pChannelManager);

	ao_poMDUpdateDeltasList = 0;

	ret = oCacheDirectoryServer.DeSerialize(IODIn);

	if (ret != M4_ERROR)
	{
		dumpMetadata("GET_UPDATE_LIST", "---");

		if (aio_poDumpInfo != NULL)
		{
			//*aio_poDumpInfo << "dDate" << ": " << dLastModificationDate << endl;
		}

		ret = oCacheDirectoryServer.GetUpdatedList(ao_poMDUpdateDeltasList, ai_poUpdateList);

		if (ret == M4_ERROR)
			ao_poMDUpdateDeltasList = 0;
	}
	
	return ret;	
}

m4return_t BuildResponseFromUpdatedList (ClDataStoreIODriver &IODOut, ClMDUpdateDeltasList * ao_poMDUpdateDeltasList, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS, response;

	if (ao_poMDUpdateDeltasList)
		response = M4_SUCCESS;
	else
		response = M4_ERROR;

	ret = IODOut.Write(response);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (response == M4_SUCCESS)
	{
		ret = ao_poMDUpdateDeltasList->Serialize(IODOut);
	}

	return ret;
}

m4return_t BuildUpdatedListFromResponse (ClDataStoreIODriver &IODIn, ClMDUpdateDeltasList *& poMDUpdateDeltasList)
{
	m4return_t ret = M4_ERROR, response;
	
	ret = IODIn.Read(response);

	if (ret == M4_ERROR)
	{
		g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_NO_RESPONSE)<<EndLog;
		return M4_ERROR;
	}

	if (response == M4_SUCCESS)
	{
		poMDUpdateDeltasList = new ClMDUpdateDeltasList();

		ret = poMDUpdateDeltasList->DeSerialize(IODIn);

		if (ret == M4_ERROR)
		{
			g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_ERROR_DESERIALIZING)<<EndLog;
			delete poMDUpdateDeltasList;
			poMDUpdateDeltasList = 0;
		}
	}
	else
	{
		g_oChLog<<BeginDebug(M4_CH_CS_MD_SERVICE_SERVER_ERROR)<<EndLog;
		ret = M4_ERROR;
	}

	return ret;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

m4return_t BuildCacheRemoveRequest (ClDataStoreIODriver &IODOut, m4uint16_t ai_iCacheType, m4pcchar_t ai_pcId)
{
	m4return_t ret = M4_SUCCESS;
	
	ret = IODOut.Write(ai_iCacheType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IODOut.Write(ai_pcId);

	return ret;
}

m4return_t BuildCacheRemoveResponseFromRequest (ClCacheDirectory * ai_poCacheDirectory, ClDataStoreIODriver &IODIn, ClDataStoreIODriver &IODOut, ExecDumpInfo *aio_poDumpInfo)
{
	m4return_t ret = M4_SUCCESS, response;
	m4uint32_t iSize;
	m4uint16_t iCacheType;
	m4pchar_t pcId; 

	if (!ai_poCacheDirectory)
		return M4_ERROR;

	ret = IODIn.Read(iCacheType);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (aio_poDumpInfo != NULL)
		*aio_poDumpInfo << "iCacheType" << ": " << (m4int32_t) iCacheType << endl;

	ret = IODIn.ReadSize(iSize);
    
	if (ret==M4_ERROR)
		return M4_ERROR;

    if (!iSize)
		return M4_ERROR;

	pcId = new m4char_t[iSize+1];
	ret = IODIn.ReadBuffer(pcId);
	pcId[iSize] = '\0';

	if (ret != M4_ERROR)
	{
		dumpMetadata("CACHE_REMOVE", pcId);

		if (aio_poDumpInfo != NULL)
			*aio_poDumpInfo << "pcId" << ": " << pcId << endl;
			
		ret = ai_poCacheDirectory->RemoveObjectById( iCacheType, pcId, M4_TRUE ) ;
	}

	response = ret;

	ret = IODOut.Write(response);

	delete [] pcId;

	return ret;	
}

m4return_t ReadCacheRemoveResponse (ClDataStoreIODriver &IODIn)
{
	m4return_t ret = M4_ERROR, response;
	
	ret = IODIn.Read(response);

	if (ret == M4_ERROR)
	{
		g_oChLog<<BeginError(M4_CH_CS_MD_SERVICE_NO_RESPONSE)<<EndLog;
		return M4_ERROR;
	}

	if (response != M4_SUCCESS)
	{
		g_oChLog<<BeginDebug(M4_CH_CS_MD_SERVICE_SERVER_ERROR)<<EndLog;
		ret = M4_ERROR;
	}

	return ret;
}
