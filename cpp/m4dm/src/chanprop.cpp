#include "dmres.hpp"

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             chanprop.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             25/05/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "chanprop.hpp"
#include "node_knl.hpp"
#include "autoload.hpp"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "cllgadap.hpp"
#include "cldmcontext.hpp"
#include "m4mdfac.hpp"



///////////////////////////////////////////////////////////////////////////////
//
// ClOrganizationProperty
//

// Pinchamos para controlar el set de la sociedad

m4return_t ClOrganizationProperty :: Set( const m4VariantType &ai_var )
{

	m4return_t			iResult = M4_SUCCESS ;
	m4bool_t			bLevel2 ;
	m4bool_t			bIsEqual ;
	m4uint8_t			iValid ;
	m4uint16_t			iLength ;
	m4pcchar_t			pccM4Obj ;
	m4pchar_t			pcOrganization ;
	m4pchar_t			pcOldOrganization ;
	m4pchar_t			pcRole ;
	m4char_t			acOrg[ M4CL_MAX_ORG_ID + 1 ] ;
	m4char_t			acM4Obj[ M4CL_MAX_T3_ID + 1 ] ;
	ClChannelManager	*poChannelManager ;
	ClLogonAdaptor		*poLogonAdaptor ;


	pcOrganization = m4pchar_t( ai_var ) ;
	Get( pcOldOrganization ) ;

// Si las dos son nulas no hago nada
// Si la nueva es nula la pongo a NULL sin comprobar
// Si las dos son iguales no hago nada

	if( pcOrganization == NULL )
	{
		if( pcOldOrganization != NULL )
		{
			return( SetNull() ) ;
		}
		else
		{
			return( M4_SUCCESS ) ;
		}
	}

	if( pcOldOrganization != NULL )
	{
		if( strcmpi( pcOrganization, pcOldOrganization ) == 0 )
		{
			return( M4_SUCCESS ) ;
		}
	}


// El canal no puede ser nulo porque se hace siempre al attach
	CHECK_CHLOG_DEBUGF( m_pClChannel == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


// Si no está hecha la build no comprobamos porque se va a hacer en la build
	if( m_pClChannel->IsBuilt() == M4_FALSE )
	{
		return( Set( pcOrganization ) ) ;
	}


// El role no puede ser nulo porque ya está hecha la build
// El channel manager tampoco debería ser nulo pero lo comprobamos
// También comprobamos el logonadaptor

	m_pClChannel->Role_ID.Get( pcRole ) ;
	CHECK_CHLOG_ERRORF( pcRole == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poChannelManager = m_pClChannel->GetpChannelManager() ;
	CHECK_CHLOG_ERRORF( poChannelManager == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;

	poLogonAdaptor = poChannelManager->GetLogonAdaptor() ;
	CHECK_CHLOG_ERRORF( poLogonAdaptor == NULL, M4_ERROR, M4_CH_DM_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;


	bLevel2 = m_pClChannel->GetIsLevel2() ;
	iValid = poLogonAdaptor->CheckRoleAndOrg( pcRole, pcOrganization, NULL, 0, bLevel2 ) ;

	if( iValid == 0 )
	{
		// Ya saca javi el mensaje
		return( M4_ERROR ) ;
	}


	iLength = strlen( pcOrganization ) ;

	if( iLength > M4CL_MAX_ORG_ID )
	{
		iLength = M4CL_MAX_ORG_ID ;
	}

	memcpy( acOrg, pcOrganization, iLength ) ;
	acOrg[ iLength ] = '\0' ;


	pccM4Obj = m_pClChannel->GetpChannelDef()->Id() ;
	iLength = strlen( pccM4Obj ) ;

	if( iLength > M4CL_MAX_T3_ID )
	{
		iLength = M4CL_MAX_T3_ID ;
	}

	memcpy( acM4Obj, pccM4Obj, iLength ) ;
	acM4Obj[ iLength ] = '\0' ;

	// checks the organization tree
	if( poChannelManager->GetDMContext()->ExecutingInClient() == M4_FALSE )
	{
		// error reported internally
		iValid = poLogonAdaptor->CheckProjectTreeValidation( acM4Obj, acOrg ) ;

		if ( iValid != 1 )
		{
			return( M4_ERROR ) ;
		}
	}

	// Se chequea que sea una sociedad equivalente para ese M4Obj, sólo si la anterior no era nula
	if( pcOldOrganization != NULL )
	{
		iResult = poChannelManager->GetpFactory()->MDFac_IsEqualOrg( "M4OBJECT_MAPPINGS", acOrg, pcOldOrganization, acM4Obj, M4_FALSE, bIsEqual ) ;
		CHECK_CHLOG_ERRORF( iResult == M4_ERROR || bIsEqual == M4_FALSE, M4_ERROR, M4_CH_DM_BAD_CHANGE_ORG, pcOldOrganization << acOrg << acM4Obj ) ;
	}


    return( Set( acOrg ) ) ;
}



m4return_t ClOrganizationProperty :: Set( m4pchar_t ai_pcValue )
{
	return( ClGenericProperty :: Set( ai_pcValue )  ) ;
}





///////////////////////////////////////////////////////////////////////////////
//
// ClAutoLoadProperty
//

m4return_t ClAutoLoadProperty::Get(m4VariantType &ao_var) const
{
    m4uint8_t iam;
    Get (iam);

    ao_var.Type = M4CL_CPP_TYPE_NUMBER;    
    ao_var.Data.DoubleData = iam;

    return M4_SUCCESS;
}

m4return_t ClAutoLoadProperty::Set(const m4VariantType &ai_var)
{
    return Set ((m4uint8_t)ai_var.Data.DoubleData);
}

m4return_t ClAutoLoadProperty::Get(m4uint8_t &ao_var) const
{
    ao_var = m_paupi->GetAutoLoadMode ();

    return M4_SUCCESS;
}

m4return_t ClAutoLoadProperty::Set(m4uint8_t ai_var)
{
    return m_paupi->SetAutoLoadMode (ai_var);
}

m4return_t ClAutoLoadProperty::GetSize(ClSize &ao_size) const
{
	ao_size += sizeof(ClAutoLoadProperty);
		ao_size -= sizeof(ClProperty);
	if (ClProperty::GetSize(ao_size) != M4_SUCCESS)
		return 	M4_ERROR;

    return 	M4_SUCCESS;
}

m4return_t ClAutoLoadProperty::_Serialize(ClGenericIODriver& IOD)
{
    return IOD.Write(m_paupi->GetAutoLoadMode());
}

m4return_t ClAutoLoadProperty::_DeSerialize(ClGenericIODriver& IOD)
{
    m4uint8_t iAutoLoadMode;
    m4return_t iRet = IOD.Read(iAutoLoadMode);

    if (iRet != M4_SUCCESS)
        iRet = m_paupi->SetAutoLoadMode(iAutoLoadMode);

    return iRet;
}


///////////////////////////////////////////////////////////////////////////////
//
// ClRecordDateProperty
//

m4return_t ClRecordVersionProperty::Get(m4VariantType& ao_var) const
{
    // Devolvemos una cadena con el formato "RecordId:NewDate"

    m4char_t pAuxStr[128];
    sprintf(pAuxStr, "%d:%f", m_iRecordId, m_dNewVersion);
    ao_var.SetString(pAuxStr);

    return M4_SUCCESS;
}

m4return_t ClRecordVersionProperty::Set(const m4VariantType& ai_var)
{
    // Esperamos una cadena con el formato "RecordId:NewDate"
    
    m4char_t iVarType = ai_var.GetType();
    if (iVarType != M4CL_CPP_TYPE_STRING_VAR || iVarType == M4CL_CPP_TYPE_STRING)
        return M4_ERROR;
    
    m4char_t pAuxStr[128];
    strcpy(pAuxStr, ai_var.Data.PointerChar);
    
    // Establish string and get the first token
    m4char_t* token = strtok(pAuxStr, ":");
    if (token == NULL)
        return M4_ERROR;
    
    m_iRecordId = atol(token);
    
    // Get next token
    token = strtok(NULL, ":");
    if (token == NULL)
        return M4_ERROR;
    
    m_dNewVersion = atof(token);
    
    return M4_SUCCESS;
}

void ClRecordVersionProperty::Get(m4uint32_t& ao_iRecordId, m4date_t& ao_dNewVersion) const
{
    ao_iRecordId = m_iRecordId;
    ao_dNewVersion = m_dNewVersion;
}

void ClRecordVersionProperty::Set(const m4uint32_t ai_iRecordId, const m4date_t ai_dNewVersion)
{
    m_iRecordId = ai_iRecordId;
    m_dNewVersion = ai_dNewVersion;
}

m4return_t ClRecordVersionProperty::_Serialize(ClGenericIODriver& IOD)
{
    m4return_t iRet = IOD.Write(m_iRecordId);

    if (iRet == M4_SUCCESS)
        iRet = IOD.Write(m_dNewVersion);

    return iRet;
}

m4return_t ClRecordVersionProperty::_DeSerialize(ClGenericIODriver& IOD)
{
    m4return_t iRet = IOD.Read(m_iRecordId);

    if (iRet == M4_SUCCESS)
        iRet = IOD.Read(m_dNewVersion);

    return iRet;
}
