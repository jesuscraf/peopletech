//## begin module%37D8F6D602B6.cm preserve=no
//## end module%37D8F6D602B6.cm

//## begin module%37D8F6D602B6.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%37D8F6D602B6.cp

//## Module: ChannelUtil%37D8F6D602B6; Subprogram body
//## Subsystem: M4ExeForChannel::UUSS::src%392278340161
//## Source file: F:\integration\m4exeforchannel\src\channelutil.cpp

//## begin module%37D8F6D602B6.additionalIncludes preserve=no
//## end module%37D8F6D602B6.additionalIncludes

//## begin module%37D8F6D602B6.includes preserve=yes
//	Rose 98 Body Include Dependences
#include <m4log.hpp>
#include <access.hpp>
#include <channel.hpp>
#include <chan_knl.hpp>
#include <dm.hpp>
#include <cllgadap.hpp>

//## end module%37D8F6D602B6.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// ChannelUtil
#include <channelutil.hpp>
// ConnProvider
#include <conn_provider.hpp>


//## begin module%37D8F6D602B6.declarations preserve=no
//## end module%37D8F6D602B6.declarations

//## begin module%37D8F6D602B6.additionalDeclarations preserve=yes
//## end module%37D8F6D602B6.additionalDeclarations


// Class Utility TChannelUtil 


//## Other Operations (implementation)
m4return_t TChannelUtil::CleanUpLinks (ClChannel *ai_pChannel)
{
  //## begin TChannelUtil::CleanUpLinks%936955917.body preserve=yes
//	START CONTRACT CHECK
	if (NULL == ai_pChannel)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "CleanUpLinks: Null Parameter");
		return M4_ERROR;
	}

	if (M4_ERROR == TChannelUtil::CheckChannelID (ai_pChannel, "SCH_SESSION"))
	{
	
		SETCODEF (M4_ERR_SCH_DESERIAL, ERRORLOG, "CleanUpLinks: Channel type other than Session Channel" );
		return M4_ERROR;
	}
//	END CONTRACT CHECK

//	Frees all L2 instances
	if (ai_pChannel->Level2.EraseAllL2Instances(M4_TRUE, M4_FALSE, NULL) != M4_SUCCESS)
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end TChannelUtil::CleanUpLinks%936955917.body
}

m4return_t TChannelUtil::CheckChannelID (ClChannel *ai_pChannel, m4pcchar_t ai_pszChannelID)
{
  //## begin TChannelUtil::CheckChannelID%936955918.body preserve=yes
//	START CONTRACT CHECK
	if (NULL == ai_pChannel)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "CleanUpLinks: Null Parameter");

		return M4_ERROR;
	}
//	END CONTRACT CHECK

	m4pcchar_t	channelId;
	
	channelId = ai_pChannel->GetpChannel_Data()->GetpChannelDef()->Id();
	if (strcmp(channelId, ai_pszChannelID))
	{
		return M4_ERROR;
	}

	return M4_SUCCESS;
  //## end TChannelUtil::CheckChannelID%936955918.body
}

m4return_t TChannelUtil::SetAdmMode (ClChannelManager &aio_oChannelMngr)
{
  //## begin TChannelUtil::SetAdmMode%942317488.body preserve=yes
	ClChannel		*poSessionChannel = aio_oChannelMngr.GetSessionChannel();
	if (NULL != poSessionChannel)
		return M4_ERROR;

//	ESTO ASEGURA QUE SE CREA UN CANAL SESSION INTERNO A LA MAQUINA VIRTUAL CON RSM = M4ADM
	return aio_oChannelMngr.GetLogonAdaptor()->InsertUserData("M4ADM", /* english */ 2, 0 /* logon mechanism: no value */, NULL, NULL /* alias user Id: no value */, NULL /* impersonation: no value */, NULL /* system info */, NULL /* client type */, NULL /* client pub IP */ );
  //## end TChannelUtil::SetAdmMode%942317488.body
}

m4return_t TChannelUtil::InitializeSession (ClChannelManager &ai_oChannelManager, ClChannel &aio_oSessionChannel, const m4string_t &ai_strUsername, m4language_t ai_iLanguage, m4pchar_t ai_pcOrganization, const m4string_t &ai_strSessionID, m4int8_t ai_iLogonMechanism, m4pchar_t ai_pcClientMachine, m4pcchar_t ai_pccHost, m4pcchar_t ai_pccSessionID, m4pchar_t ai_pcUserIdAlias, m4pchar_t ai_pcImpersonatorUser , m4int8_t ai_iLogonOrigin, m4pchar_t ai_pcSystemInfo, m4pchar_t ai_pcClientType, m4pcchar_t ai_pccPort, m4pchar_t ai_pcClientPubIP)
{
  //## begin TChannelUtil::InitializeSession%942686871.body preserve=yes
	ClAccess	*poAccess=aio_oSessionChannel.CreateAccess();
	if (NULL == poAccess)	//Creamos un acceso por cada canal
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Error creating Access");
		return M4_ERROR;
	}

	ClNode *poNode=&(poAccess->Node["ROOT_SESSION"]);
	if (NULL == poNode )
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Error creating Node");
		return M4_ERROR;
	}

	m4return_t		iRet = poNode->RecordSet.Current.Add();
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Add");
		poAccess->Destroy();
		return iRet;
	}

	m4VariantType	arg, start, end, current;
	
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	//	!!! Conversion to non-const to compile
	arg.Data.PointerChar = (m4pchar_t)ai_strUsername.c_str();

	iRet = poNode->RecordSet.Current.Item["ID_APP_USER"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: ID_APP_USER");
		poAccess->Destroy();
		return iRet;
	}

	// Alias Id de usuario
	if( ai_pcUserIdAlias != NULL && *ai_pcUserIdAlias != '\0' )
	{
		arg.Data.PointerChar = ai_pcUserIdAlias ;
		iRet = poNode->RecordSet.Current.Item["APP_USER_ALIAS"].Value.Set(arg);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: APP_USER_ALIAS");
			poAccess->Destroy();
			return iRet;
		}
	}

	// Suplantación de usuario
	if( ai_pcImpersonatorUser != NULL && *ai_pcImpersonatorUser != '\0' )
	{
		arg.Data.PointerChar = ai_pcImpersonatorUser ;
		iRet = poNode->RecordSet.Current.Item["ID_DEBUG_USER"].Value.Set(arg);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: ID_DEBUG_USER");
			poAccess->Destroy();
			return iRet;
		}
	}


	//Bug 0095543
	//Cuando se planifica una tarea el canal de planificacion le pregunta al canal sesion por la select que se parametriza
	//por idioma (SAV_PARAMS). Si el idioma no esta definido la select viene vacia y por lo tanto no se inserta en la cadena
	//de serialización. Como resultado, al deserializar y construir el canal session en la ejecución la subcadena que viene
	//la select estaría vacía.
	if (ai_iLanguage != 0) {
		arg.Type = M4CL_CPP_TYPE_NUMBER;
		arg.Data.DoubleData = ai_iLanguage;

		iRet = poNode->RecordSet.Current.Item["LANGUAGE"].Value.Set(arg);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: LANGUAGE");
			poAccess->Destroy();
			return iRet;
		}
	}

	if (ai_pcOrganization != NULL) {
		arg.Type = M4CL_CPP_TYPE_STRING_VAR;
		arg.Data.PointerChar = ai_pcOrganization;

		iRet = poNode->RecordSet.Current.Item["ID_ORGANIZATION"].Value.Set(arg);
		if (M4_SUCCESS != iRet)
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: ID_ORGANIZATION");
			poAccess->Destroy();
			return iRet;
		}
	}

/*
	En ningún caso se está asignando un valor a este campo, por lo eliminamos

	arg.Type = M4CL_CPP_TYPE_STRING_VAR;
	//	!!! Conversion to non-const to compile
	arg.Data.PointerChar = (m4pchar_t)ai_strSessionID.c_str();
	iRet = poNode->RecordSet.Current.Item["SESSION_ID"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Creating SessionCh: accessing SESSION_ID");
		return iRet;
	}
*/
	arg.Type=M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar = "M4ADM";

	iRet = poNode->RecordSet.Current.Item["ID_APP_ROLE"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: ID_APP_ROLE");
		poAccess->Destroy();
		return iRet;
	}

	iRet = poNode->RecordSet.Current.Item["ID_RSM"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: ID_RSM");
		poAccess->Destroy();
		return iRet;
	}

	//bg 102349.
	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = ai_iLogonMechanism;

	iRet = poNode->RecordSet.Current.Item["LOGON_MECHANISM"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: LOGON_MECHANISM");
		poAccess->Destroy();
		return iRet;
	}

	// luciag: for the switch role
	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = ai_iLogonOrigin;

	iRet = poNode->RecordSet.Current.Item["ENTRY_MODE"].Value.Set(arg);
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: ENTRY_MODE");
		poAccess->Destroy();
		return iRet;
	}

	start.Type=M4CL_CPP_TYPE_DATE;
	start.Data.DoubleData= (m4double_t)::ClMinusInfiniteDate();

	end.Type=M4CL_CPP_TYPE_DATE;
	end.Data.DoubleData= (m4double_t)::ClPlusInfiniteDate();
	
	current.Type=M4CL_CPP_TYPE_DATE;
	current.Data.DoubleData= (m4double_t)::ClActualDate();

	iRet |= poNode->RecordSet.Current.Item["META_DATA_START_DATE"].Value.Set(start);
	iRet |= poNode->RecordSet.Current.Item["META_DATA_END_DATE"].Value.Set(end);

	iRet |= poNode->RecordSet.Current.Item["DATA_START_DATE"].Value.Set(start);
	iRet |= poNode->RecordSet.Current.Item["DATA_END_DATE"].Value.Set(end);

	iRet |= poNode->RecordSet.Current.Item["EXECUTION_START_DATE"].Value.Set(start);
	iRet |= poNode->RecordSet.Current.Item["EXECUTION_END_DATE"].Value.Set(end);

	iRet |= poNode->RecordSet.Current.Item["EXECUTION_DATE"].Value.Set(current);
	iRet |= poNode->RecordSet.Current.Item["DATA_CORRECTION_DATE"].Value.Set(current);
	iRet |= poNode->RecordSet.Current.Item["META_DATA_CORRECTION_DATE"].Value.Set(current);

	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_SESSION_DATE"].Value.Set(current);

	// Session key para auditoría
	m4char_t	acAuditSessionKey[ 100 ] ;
	int			iYear, iMonth, iDay ;
	m4int32_t	lHour, lMinute, lSecond ;
	
	memset( acAuditSessionKey, 0, 100 ) ;
	ClDateToNumbers( current.Data.DoubleData, iYear, iMonth, iDay, lHour, lMinute, lSecond ) ;

	// bg 166736
	if( ai_pccSessionID != NULL && *ai_pccSessionID != '\0' )
	{
		sprintf( acAuditSessionKey, "%s%d%02d%02d%02d%02d%02d%s", ai_strUsername.c_str(), iYear, iMonth, iDay, lHour, lMinute, lSecond, ai_pccSessionID ) ;
	}
	else
	{
		sprintf( acAuditSessionKey, "%s%d%02d%02d%02d%02d%02d", ai_strUsername.c_str(), iYear, iMonth, iDay, lHour, lMinute, lSecond ) ;
	}
	
	current.Type = M4CL_CPP_TYPE_STRING_VAR ;
	current.Data.PointerChar = acAuditSessionKey ;

	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_SESSION_KEY"].Value.Set(current);

	// Máquina cliente para auditoría
	if( ai_pcClientMachine != NULL && *ai_pcClientMachine != '\0' )
	{
		current.Type = M4CL_CPP_TYPE_STRING_VAR ;
		current.Data.PointerChar = ai_pcClientMachine ;
	}
	else
	{
		current.Type = M4CL_CPP_TYPE_NULL ;
	}
	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_CLIENT_MACHINE"].Value.Set(current);

	// Server para auditoría
	if( ai_pccHost != NULL && *ai_pccHost != '\0' )
	{
		current.Type = M4CL_CPP_TYPE_STRING_VAR ;
		current.Data.PointerChar = (m4pchar_t)ai_pccHost ;
	}
	else
	{
		current.Type = M4CL_CPP_TYPE_NULL ;
	}
	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_SRV_NAME"].Value.Set(current);

	// Server port para auditoría de tiempos
	if( ai_pccPort != NULL && *ai_pccPort != '\0' )
	{
		current.Type = M4CL_CPP_TYPE_STRING_VAR ;
		current.Data.PointerChar = (m4pchar_t)ai_pccPort ;
	}
	else
	{
		current.Type = M4CL_CPP_TYPE_NULL ;
	}
	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_SRV_PORT"].Value.Set(current);

	// Server session para auditoría
	if( ai_pccSessionID != NULL && *ai_pccSessionID != '\0' )
	{
		current.Type = M4CL_CPP_TYPE_STRING_VAR ;
		current.Data.PointerChar = (m4pchar_t)ai_pccSessionID ;
	}
	else
	{
		current.Type = M4CL_CPP_TYPE_NULL ;
	}
	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_SRV_ID_SESSION"].Value.Set(current);

	// Información de sistema de la máquina cliente.
	if( ai_pcSystemInfo != NULL && *ai_pcSystemInfo != '\0' )
	{
		current.Type = M4CL_CPP_TYPE_STRING_VAR ;
		current.Data.PointerChar = ai_pcSystemInfo ;
	}
	else
	{
		current.Type = M4CL_CPP_TYPE_NULL ;
	}
	iRet |= poNode->RecordSet.Current.Item["SYSTEM_INFO"].Value.Set(current);

	// Información tipo de cliente.
	if( ai_pcClientType != NULL && *ai_pcClientType != '\0' )
	{
		current.Type = M4CL_CPP_TYPE_NUMBER ;
		current.Data.DoubleData = atoi(ai_pcClientType) ;
	}
	else
	{
		current.Type = M4CL_CPP_TYPE_NULL ;
	}
	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_CLIENT_TYPE"].Value.Set(current);

	// Client public IP.
	if( ai_pcClientPubIP != NULL && *ai_pcClientPubIP != '\0' )
	{
		current.Type = M4CL_CPP_TYPE_STRING_VAR;
		current.Data.PointerChar = ai_pcClientPubIP;
	}
	else
	{
		current.Type = M4CL_CPP_TYPE_NULL;
	}
	iRet |= poNode->RecordSet.Current.Item["USR_AUDIT_CLIENT_PUB_IP"].Value.Set(current);

	if (M4_SUCCESS!=iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Initializing SessionChannel: Dates");
		poAccess->Destroy();
		return M4_ERROR;
	}

	iRet=poAccess->Destroy();
	if (M4_SUCCESS != iRet)
	{
		SETCODEF(M4_ERR_UUSS_UNKNOWN, WARNINGLOG, "Initializing SessionChannel: Destroying access.");
	}

	return M4_SUCCESS;
  //## end TChannelUtil::InitializeSession%942686871.body
}

// Class TDestroyer 





TDestroyer::TDestroyer (ClChannel *ai_pChannel, ClLConn_Provider *ai_pLConnProvider)
  //## begin TDestroyer::TDestroyer%936955907.hasinit preserve=no
      : m_pChannel(NULL), m_pAccess(NULL), m_pLConnProvider(NULL), m_bDestroy(M4_TRUE)
  //## end TDestroyer::TDestroyer%936955907.hasinit
  //## begin TDestroyer::TDestroyer%936955907.initialization preserve=yes
  //## end TDestroyer::TDestroyer%936955907.initialization
{
  //## begin TDestroyer::TDestroyer%936955907.body preserve=yes
	m_pChannel = ai_pChannel;
	m_pLConnProvider = ai_pLConnProvider;
  //## end TDestroyer::TDestroyer%936955907.body
}

TDestroyer::TDestroyer ()
  //## begin TDestroyer::TDestroyer%936955908.hasinit preserve=no
      : m_pChannel(NULL), m_pAccess(NULL), m_pLConnProvider(NULL), m_bDestroy(M4_TRUE)
  //## end TDestroyer::TDestroyer%936955908.hasinit
  //## begin TDestroyer::TDestroyer%936955908.initialization preserve=yes
  //## end TDestroyer::TDestroyer%936955908.initialization
{
  //## begin TDestroyer::TDestroyer%936955908.body preserve=yes
  //## end TDestroyer::TDestroyer%936955908.body
}


TDestroyer::~TDestroyer ()
{
  //## begin TDestroyer::~TDestroyer%936955909.body preserve=yes

	// bg 286953
	// Para el canal sesión siempre es detach y no destroy.
	if( m_bDestroy == M4_FALSE && m_pChannel != NULL )
	{
		if( m_pChannel->Level2.EraseAllL2Instances( M4_TRUE, M4_FALSE, NULL ) != M4_SUCCESS )
		{
			SETCODEF(M4_ERR_UUSS_UNKNOWN, ERRORLOG, "Cleaning Up links!!!" );
		}
	}

	if (NULL != m_pAccess)
		m_pAccess->Destroy();
	ResetAccess ();

	if (NULL != m_pChannel)
	{
		if (m_bDestroy)
			m_pChannel->Destroy();
		else
			m_pChannel->Detach();
	}

	ResetChannel ();

	if (NULL != m_pLConnProvider)
	{
		m_pLConnProvider->SetConnection(NULL);
	}

	ResetEnvironment ();
  //## end TDestroyer::~TDestroyer%936955909.body
}


//## begin module%37D8F6D602B6.epilog preserve=yes
//## end module%37D8F6D602B6.epilog
