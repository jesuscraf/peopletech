//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                chandata_ser.cpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                16-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//   This module defines the channel data serialization class
//
//
//==============================================================================


#include "chandata_ser.hpp"
#include "node_knl.hpp"
#include "clprsstb.hpp"
#include "chan_knl.hpp"
#include "cldates.hpp"
#include "m4mdrt.hpp"
#include "m4mdfac.hpp"
#include "access.hpp"
#include "m4magicn.hpp"

ClChannelDataSerializer::ClChannelDataSerializer(void)
{	
	m_iCsType = 0;
	m_dStartDate = 0;
	m_dEndDate = 0;
	m_dCorDate = 0; 
	m_dVersion = 0;
	m_iSerializeMCR = 0;
	m_pcChannelId = 0;
	m_pcMask = 0;
}

ClChannelDataSerializer::~ClChannelDataSerializer(void)
{
	Reset();
}


void ClChannelDataSerializer::Reset()
{
	if (m_pcChannelId)
		delete [] m_pcChannelId;

	if (m_pcMask)
		delete [] m_pcMask;

	m_iCsType = 0;
	m_dStartDate = 0;
	m_dEndDate = 0;
	m_dCorDate = 0; 
	m_dVersion = 0;
	m_iSerializeMCR = 0;
	m_pcChannelId = 0;
	m_pcMask = 0;

}

m4return_t ClChannelDataSerializer::Init( m4uint8_t ai_iSerializeMCR, m4pcchar_t ai_pccChannelId, m4uint8_t ai_iCsType, m4uint8_t ai_iLanguage, m4pcchar_t ai_pccMask, m4date_t ai_dVersion, m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4date_t ai_dCorDate)
{
	m_iCsType = ai_iCsType;
	m_iLanguage = ai_iLanguage;
	m_dStartDate = ai_dStartDate;
	m_dEndDate = ai_dEndDate;
	m_dCorDate = ai_dCorDate; 
	m_dVersion = ai_dVersion;
	m_iSerializeMCR = ai_iSerializeMCR;

	if (ai_pccChannelId)
	{
		m_pcChannelId = new m4char_t [strlen(ai_pccChannelId) + 1];
		strcpy(m_pcChannelId, ai_pccChannelId);
	}

	if (ai_pccMask)
	{
		m_pcMask = new m4char_t [strlen(ai_pccMask) + 1];
		strcpy(m_pcMask, ai_pccMask);
	}

	return M4_SUCCESS;
}

m4return_t ClChannelDataSerializer::Init( ClChannel_Data* ai_poChannelData, m4bool_t ai_bSerializeMCR )
{
	m4return_t ret;
	ClCompiledMCR * pCMCR = ai_poChannelData->GetpExtInfo ()->MCR.GetpCMCR();


	/* Bug 0089067
	Se permite marcar desde fuera que se quiere serializar el metacanal.
	Así, para el canal de estadísticas se debería hacer.
	*/
	m4uint8_t iSerializeMCR = 0 ;

	if( ai_bSerializeMCR == 1 )
	{
		iSerializeMCR = 1 ;
	}
	else
	{
		enum ClMCROwner::BuildType_e	iType = ai_poChannelData->GetpExtInfo()->MCR.BuildType() ;

		if( iType == ClMCROwner::ID || iType == ClMCROwner::Data )
		{
			iSerializeMCR = M4_FALSE ;
		}
		else
		{
			iSerializeMCR = M4_TRUE ;
		}
	}

	/* Bug 0118646
	Ya no se chequea que el canal sea proxy
	*/
	
    ret = Init(iSerializeMCR, pCMCR->GetChannelId(), pCMCR->GetCacheCsType(), pCMCR->GetChannelLanguage(),
			   pCMCR->GetSecurityMaskId(), pCMCR->GetChannelVersion(),
			   pCMCR->GetChannelStartDate(), pCMCR->GetChannelEndDate(), pCMCR->GetChannelCorStartDate());

	return ret;
}

m4return_t	ClChannelDataSerializer::SerializeInfoData(ClGenericIODriver& IOD)
{
	m4return_t ret = M4_SUCCESS;

	ret = IOD.Write((m4uint32_t)M4_CH_MAGIC_NUMBER_DATA);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = IOD.Write(m_iSerializeMCR);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!m_iSerializeMCR)
	{
		ret = IOD.Write((void*)m_pcChannelId, strlen(m_pcChannelId)+1);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Write(m_iCsType);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Write(m_iLanguage);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Write((void*)m_pcMask, strlen(m_pcMask)+1);

		if (ret == M4_ERROR)
			return M4_ERROR;


		ret = IOD.Write(m_dVersion);

		if (ret == M4_ERROR)
			return M4_ERROR;

		ret = IOD.Write(m_dStartDate);

		if (ret == M4_ERROR)
			return M4_ERROR;


		ret = IOD.Write(m_dEndDate);

		if (ret == M4_ERROR)
			return M4_ERROR;


		ret = IOD.Write(m_dCorDate);

		if (ret == M4_ERROR)
			return M4_ERROR;

	}

	return ret;
}

m4return_t	ClChannelDataSerializer::Serialize(ClGenericIODriver& IOD, ClChannel_Data* ai_poChannelData, m4bool_t ai_bSerializeMCR, m4uint8_t ai_iRecSerMod, m4uint8_t ai_iIteSerMod)
{
	m4return_t ret = M4_SUCCESS;

	ret = Init(ai_poChannelData, ai_bSerializeMCR);

	if (ret == M4_ERROR)
		return M4_ERROR;

	ret = SerializeInfoData(IOD);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (m_iSerializeMCR)
	{
		//Serialiamos el CMCR
		ai_poChannelData->GetpChannelDef()->GetpCMCR()->GetCMCRWrapper()->Serialize(IOD);

		m4bool_t bHasSecurity = (ai_poChannelData->GetpChannelDef()->GetpCMCR()->GetCSCRWrapper() != 0) ? M4_TRUE : M4_FALSE;

		IOD.Write(bHasSecurity);

		if (M4_TRUE == bHasSecurity)
		{
			ai_poChannelData->GetpChannelDef()->GetpCMCR()->GetCSCRWrapper()->Serialize(IOD);
		}
	}

    ret = ai_poChannelData->Serialize(IOD, ai_iRecSerMod, ai_iIteSerMod, NULL, 0);

	Reset();

	return ret;
}

m4return_t	ClChannelDataSerializer::DeSerializeInfoData(ClGenericIODriver& IOD)
{
	m4return_t ret;
	m4uint32_t iMagicNumber;

	ret = IOD.Read(iMagicNumber);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (iMagicNumber != M4_CH_MAGIC_NUMBER_DATA)
	{
		g_oChLog<<BeginError(M4_CH_DM_BAD_DATAL_MAGIC_NUMBER)<<iMagicNumber<<m4uint32_t(M4_CH_MAGIC_NUMBER_DATA)<<EndLog;
		return M4_ERROR;
	}

	ret = IOD.Read(m_iSerializeMCR);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (!m_iSerializeMCR)
	{
		m4uint32_t iSize;
		m_pcChannelId = m_pcMask = 0;

		ret = IOD.ReadSize(iSize);
    
		if (ret==M4_ERROR)
			return M4_ERROR;

		if (!iSize)
			return M4_ERROR;

		m_pcChannelId = new m4char_t[iSize];

		ret = IOD.ReadBuffer(m_pcChannelId);

		if (ret != M4_ERROR)
		{
			ret = IOD.Read(m_iCsType);

			if (ret != M4_ERROR)
			{
				ret = IOD.Read(m_iLanguage);

				if (ret != M4_ERROR)
				{
					ret = IOD.ReadSize(iSize);
    
					if (ret==M4_ERROR)
						return M4_ERROR;

					if (!iSize)
						return M4_ERROR;

					m_pcMask = new m4char_t[iSize];

					ret = IOD.ReadBuffer(m_pcMask);
					
					if (ret != M4_ERROR)
					{
						ret = IOD.Read(m_dVersion);

						if (ret != M4_ERROR)
						{
							ret = IOD.Read(m_dStartDate);

							if (ret != M4_ERROR)
							{
								ret = IOD.Read(m_dEndDate);

								if (ret != M4_ERROR)
								{
									ret = IOD.Read(m_dCorDate);
								}
							}
						}
					}
				}
			}
		}

		if (ret == M4_ERROR)
		{
			if (m_pcChannelId)
				delete [] m_pcChannelId;

			if (m_pcMask)
				delete [] m_pcMask;

			m_pcChannelId = m_pcMask = 0;
		}
	}

	return ret;
}


m4return_t	ClChannelDataSerializer::DeSerialize(ClGenericIODriver& IOD, ClCMCRFactory *ai_poFactory, ClCompiledMCR *ai_poCMCR, ClLaneArgumnent* ai_poLaneArgument, ClChannel_Data* ao_poChannelData)
{
	m4return_t ret = M4_SUCCESS;

	ret = DeSerializeInfoData(IOD);

	if (ret == M4_ERROR)
		return M4_ERROR;

	if (m_iSerializeMCR)
	{
		//Deserializamos el CMCR
		ClCompiledMCR * pCMCR = new ClCompiledMCR();
		ClCMCRWrapper * pCMCRWrap = new ClCMCRWrapper();
		
		ret = pCMCRWrap->DeSerialize(IOD);

		if (M4_ERROR == ret)
		{
			delete pCMCR;		
			delete pCMCRWrap;
			return M4_ERROR;
		}

		pCMCR->SetCMCRWrapper(pCMCRWrap);


		m4bool_t bHasSecurity;

		IOD.Read(bHasSecurity);

		if (M4_TRUE == bHasSecurity)
		{
			ClCSCRWrapper * pCSCRWrap = new ClCSCRWrapper();

			ret = pCSCRWrap->DeSerialize(IOD);

			if (M4_ERROR == ret)
			{
				delete pCMCR;
				delete pCMCRWrap;
				delete pCSCRWrap;
				return M4_ERROR;
			}

			pCMCR->SetCSCRWrapper(pCSCRWrap);
		}

		if (M4_ERROR != ret)
		{
			ret = ao_poChannelData->GetpExtInfo()->MCR.BuildFromMCR (pCMCR);
		}
		else
		{
			pCMCR->Destroy();		
		}
	}
	else
	{
		if( ai_poCMCR != NULL )
		{
			// Si nos pasan un metacanl utilizamos ese para construir
			ret = ao_poChannelData->GetpExtInfo()->MCR.BuildFromMCR( ai_poCMCR ) ;
		}
		else
		{
			ClBuidParameters	oParameters ;

			oParameters.m_poLane = ai_poLaneArgument ;
			oParameters.m_pccM4ObjId = m_pcChannelId ;
			oParameters.m_pccIdOrg = NULL ;		// De momento nulo !!!!!!!!!!!!!!!
			oParameters.m_pccRSM = m_pcMask ;
			oParameters.m_dStartDate = m_dStartDate ;
			oParameters.m_dEndDate = m_dEndDate ;
			oParameters.m_dCorDate = m_dCorDate ;
			oParameters.m_iCsType = m_iCsType ;
			oParameters.m_iLanguage = m_iLanguage ;
			oParameters.m_bConvertRSM = M4_FALSE ;
			oParameters.m_bBuildSecurity = M4_TRUE ;
			oParameters.m_bApplyMappings = M4_FALSE ;	// No se aplican mapeos

			ret = ao_poChannelData->GetpExtInfo()->MCR.BuildFromId( ai_poFactory, oParameters ) ;
		}

        if (M4_ERROR != ret)
		{
			/* Bug 0271932
			Hay que comparar teniendo en cuenta la precisión
			*/
			if (fabs(ao_poChannelData->GetpExtInfo()->MCR.GetpCMCR()->GetChannelVersion() - m_dVersion) >= M4_LN4_DATE_PRECISION)
			{
				ret = M4_ERROR;
			}
		}
	}

	if (M4_ERROR != ret)
	{
		ret = ao_poChannelData->Init ();

		if (M4_ERROR != ret)
			ret = ao_poChannelData->DeSerialize(IOD);
	}

	/* Bug 0089067
	No hay que resetear la información de si se ha serializado el metacanal.
	Reset();
	*/

	return ret;
}


