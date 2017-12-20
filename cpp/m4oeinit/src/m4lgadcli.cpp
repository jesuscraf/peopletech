
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4oeinit.dll
// File:                m4lgadcli.cpp   
// Project:             mind3.x    
// Author:              Meta Software M.S. , S.A
// Date:                15-03-2000
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo contiene la implementación logon adaptor parte cliente
//
//
//==============================================================================


#include "m4lgadapi.hpp"

#include "csres.hpp"
#include "channel.hpp"
#include "access.hpp"
#include "m4objglb.hpp"
#include "m4lang.hpp"
#include "clappclientadap.hpp"
#include "cachedir.hpp"
#include "m4objreg.hpp"
#include "licensemanager.hpp"
#include "licres.hpp"
#include "tsap.hpp"
#include "m4win2ux.hpp"
#include "m4systeminfo.hpp"


#define	M4CH_DUMMY_B2	"\x2\x3\x8\x1\x9\x6\x5\xF\xD\x8\xF\x8\xB\x4\x7\x7\xF\x6\x4\xE\x4\xE\xE\x5\xB\xE\x6\x7\xF\x3\x7\x2\xB\x6\xC\x4\xD\xB\x6\x7\x8\x3\xE\x8\xA\xB\x7\x9\x6\x6\x7\x9\xB\x6\x4\x8\xE\xF\x3\x8\x8\xC\xA\x0\x4\x2\x6\x8\x4\xC\x4\xD\x9\x2\x8\x4\x8\x8\x6\x8\xF\x1\x1\xF\xC\x1\x7\xE"


m4pchar_t	ClLogonState :: GetString( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_ItemDef )
{

	ClAccess	*poAccess = NULL ;

	
	if( ai_ItemDef == -1 )
	{
		return( NULL ) ;
	}

	poAccess = ai_LogonAdaptor->_GetSessionAccess() ;

	if( poAccess != NULL )
	{
		ClRegister *pRegRoot = & (poAccess->Node [ ClHandle( ai_LogonAdaptor->m_iRootNode ) ].RecordSet.Register[SUPONGO_QUE_SOLO_HAY_UN_REGISTRO]) ;
		return (m4pchar_t) pRegRoot->Item [(ClItemIndex)ai_ItemDef].Value.Get() ;
	}

	return( NULL ) ;
}

m4return_t	ClLogonState :: _LogOff( ClLogonAdaptor* ai_LogonAdaptor )
{

	m4return_t	iResult ;


	if( ai_LogonAdaptor->_GetSmOltp() == 0 )
	{
	    iResult = ai_LogonAdaptor->_GetAPIClientAdaptor()->CloseSession() ;

		if( iResult != M4_SUCCESS )
		{
			return( iResult ) ;
		}
	}

	// fix bug 0060415
	if (ai_LogonAdaptor->_GetChannelManager() != NULL)
		ai_LogonAdaptor->_GetChannelManager()->ReleaseProxy();

	//
	// We need to destroy the session channel and its access
	//

	if( ai_LogonAdaptor->m_poSessionAccess != NULL )
	{
		ai_LogonAdaptor->m_poSessionAccess->Destroy() ;
		ai_LogonAdaptor->m_poSessionAccess = NULL ;
	}

	if( ai_LogonAdaptor->m_poSessionChannel != NULL )
	{
		ai_LogonAdaptor->m_poSessionChannel->Destroy() ;
		ai_LogonAdaptor->m_poSessionChannel = NULL ;

	}

	DUMP_CHLOG_DEBUG( M4_CH_CS_LOGOFF_NOTIFY ) ;

	ai_LogonAdaptor->_ChangeState( ClLogLoggedOut :: Instance(), M4CL_LOG_LOGGED_OUT ) ;

	return( M4_SUCCESS ) ;
}


m4return_t ClLogonState :: _SetRole( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_iPos,	m4bool_t ai_bSetAsDefaultRole )
{

	m4return_t	iResult = M4_SUCCESS;
	m4return_t	iResult2 = M4_SUCCESS;
	m4pchar_t	pcIdRole ;


// si la posición es negativa es role por defecto

	if( ai_iPos >= 0 )
	{
		pcIdRole = ai_LogonAdaptor->GetIdRoleByPos( ai_iPos ) ;
	}
	else
	{
		pcIdRole = "" ;
		ai_bSetAsDefaultRole = M4_FALSE ;
	}

    if( ai_LogonAdaptor->_GetSmOltp() == 1 )
    {
		iResult = ai_LogonAdaptor->_EnableRole( pcIdRole, ai_bSetAsDefaultRole ) ;

		if( iResult < 0 )
		{
			return( iResult ) ;
		}
    }
    else
	{
        iResult = ai_LogonAdaptor->_GetAPIClientAdaptor()->EnableRole( pcIdRole, ai_bSetAsDefaultRole ) ;

		if( iResult < 0 )
		{
			return( iResult ) ;
		}

		// Apaño para el problema de que en role por defecto y password caducada no se rellena el role en el canal sesión
		// Entonces hacemos un refresh otra vez con role por defecto
		// Si hay un warning no se hace nada

		/* Bug 0111215
		Aunque haya warning hay que hacer el refresh para que se traiga la lista de roles
		*/
		if( ai_iPos < 0 )
		{
			iResult2 = ai_LogonAdaptor->_RefreshSessionChannel( M4_TRUE ) ;

			if( iResult2 != M4_SUCCESS )
			{
				return( iResult2 ) ;
			}
		}
    }

	if( iResult == M4_SUCCESS )
	{
		if( ai_iPos >= 0 )
		{
			iResult2 = ai_LogonAdaptor->_SetRoleAsActive( ai_iPos ) ;
		}
		else
		{
			iResult2 = ai_LogonAdaptor->_SetDefaultRoleAsActive() ;
		}

		if( iResult2 != M4_SUCCESS )
		{
			return( iResult2 ) ;
		}
	}

	return( iResult ) ;
}	


m4return_t	ClLogonState :: _ChangePassword( ClLogonAdaptor* ai_LogonAdaptor, m4pchar_t ai_pszOldPassword, m4pchar_t ai_pszNewPassword )
{

	m4return_t	iResult = M4_SUCCESS ;
	m4pchar_t	pcRole ;


	if( ai_pszOldPassword == NULL )
	{
		ai_pszOldPassword = "" ;
	}

	if( ai_pszNewPassword == NULL )
	{
		ai_pszNewPassword = "" ;
	}

	// jcr apaño cutre para cuando la password está caducada y no estamos logados levantarlo con M4ADM
	if( ai_LogonAdaptor->GetCurrentState() == M4CL_LOG_WITHOUT_ROLE )
	{
		pcRole = M4CL_SEC_SUPER_USER ;
	}
	else
	{
		pcRole = NULL ;
	}

    iResult = ai_LogonAdaptor->_ValidatePassword( ai_pszNewPassword, pcRole ) ;

	if( iResult != M4_SUCCESS )
	{
		return( iResult ) ;
	}

    if( ai_LogonAdaptor->_GetSmOltp() == 1 )
    {
		m4int_t			iLength = -1 ;
		m4pchar_t		pcNewPassword ;
		m4pchar_t		pcOldPassword ;
		m4pchar_t		pcNewPasswordTwoWay;
		m4pchar_t		pcUserId ;
		m4pchar_t		pcDebugUserId ;
		M4ClString		sUserId ;
		m4char_t		acNewPassword[ 2 * M4_MAX_LEN_ONE_WAY + 1 ] ;
		m4char_t		acOldPassword[ 2 * M4_MAX_LEN_ONE_WAY + 1 ] ;
		
		// Obtener el usuario.
		pcUserId = ai_LogonAdaptor->GetUsername();

		// Obtener el usuario suplantador.
		pcDebugUserId = (m4pchar_t) ai_LogonAdaptor->GetSessionRecordSet()->Current.Item[M4_ITEM_DEBUG_USER].Value.Get();

		// Si hay usuario suplantador, nos quedamos con este.
		if (pcDebugUserId != NULL)
		{
			sUserId = pcDebugUserId;
		}
		else
		{
			sUserId = pcUserId;
		}

		// Encriptar usuario y contraseña.
		iResult = M4ConvertPassword( sUserId.c_str(), ai_pszNewPassword, acNewPassword, 2 * M4_MAX_LEN_ONE_WAY ) ;

		if( iResult != M4_SUCCESS )
		{
			pcNewPassword = ai_pszNewPassword ;
		}
		else
		{
			pcNewPassword = acNewPassword ;
		}

		iResult = M4ConvertPassword( sUserId.c_str(), ai_pszOldPassword, acOldPassword, 2 * M4_MAX_LEN_ONE_WAY ) ;

		if( iResult != M4_SUCCESS )
		{
			pcOldPassword = ai_pszOldPassword ;
		}
		else
		{
			pcOldPassword = acOldPassword ;
		}

		// Encriptar two-way la contraseña nueva.
		iResult = EncryptTwoWay( ai_pszNewPassword, strlen( ai_pszNewPassword ) * sizeof( m4char_t ), M4CH_DUMMY_B2 + 23, strlen( M4CH_DUMMY_B2 + 23 ) * sizeof(m4char_t), pcNewPasswordTwoWay, iLength ) ;

		if (iResult != M4_SUCCESS)
		{
			return M4_ERROR ;
		}
		
		iResult = ai_LogonAdaptor->_ValidateServerPassword( pcOldPassword, pcNewPassword, pcNewPasswordTwoWay, pcRole ) ;

		delete pcNewPasswordTwoWay;
    }
    else
	{
		M4ClString	tmpOldPassword( ai_pszOldPassword ) ;
		M4ClString	tmpNewPassword( ai_pszNewPassword ) ;

        iResult = ai_LogonAdaptor->_GetAPIClientAdaptor()->ChangePassword( tmpOldPassword, tmpNewPassword ) ;
    }

	return( iResult ) ;
}


m4return_t	ClLogLoggedOut :: Logon
(
	ClLogonAdaptor*		ai_LogonAdaptor,
	m4pchar_t			ai_pszUserName, 
	m4pchar_t			ai_pszPassword, 
	m4bool_t			ai_bUseCredential,
	m4language_t		ai_Language, 
	m4pchar_t			ai_pzApplicationServerName,
	m4uint32_t			ai_uiApplicationServerPort,
	m4bool_t	        ai_bUseDefaultRole,
	m4pchar_t			ai_pszSSLFile,
	m4bool_t			ai_bUseSSL
)
{

	m4return_t	iResult = M4_SUCCESS ;
	m4return_t	iResultCac = M4_SUCCESS ;
	m4return_t	iWarning = M4_SUCCESS ;
	m4uint8_t	iSmOltp ;
	m4uint8_t	iRefresh = 1 ;
	m4handle_t	hSessionId = 1 ;
	m4char_t	acTempBuffer[ M4CH_MAX_FILE + 1 ] ;
	ClTagList	Args;
	m4int8_t	iLogonMechanism = -1; //bg 102349
	m4char_t	acHostName[256] = "";		
	m4char_t	acRealUserId[ M4_MAX_USER_ID ] ;		// ID Usuario real en caso de logon mediante alias.
	m4pchar_t	pcRealUserId = acRealUserId ;
	m4char_t	acUserAlias[ M4_MAX_USER_ALIAS ] ;		// Alias de usuario en caso de logon mediante alias [solo DIC].
	m4pchar_t	pcUserAlias = acUserAlias ;
	m4char_t	acUserImpersonated[ M4_MAX_USER_ID ] ;	// Usuario a suplantar.
	m4pchar_t	pcUserImpersonated = acUserImpersonated ;
	m4char_t	acClientPubIP[ M4_MAX_CLIENT_IP ];		// Dirección IP pública del puesto cliente.
	m4pchar_t	pcClientPubIP = acClientPubIP;

	// Valores finales para unificar el paso de valores al canal sesión.
	m4pchar_t	pcSessUserID			= NULL ;
	m4pchar_t	pcSessUserAlias			= NULL ;
	m4pchar_t	pcSessUserImpersonator	= NULL ;
	
	ClSystemInfo  * poSystemInfo = NULL;
	string			sSystemInfo;
	string			sClientType;

	hostent* pHostInfo;
	char*	 pcIPAddress;

	*pcRealUserId		= '\0' ;
	*pcUserAlias		= '\0' ;
	*pcUserImpersonated = '\0' ;
	*pcClientPubIP		= '\0';


	if( ai_pszUserName == NULL )
	{
		ai_pszUserName = "" ;
	}

	if( ai_pszPassword == NULL )
	{
		ai_pszPassword = "" ;
	}

	if( ai_pzApplicationServerName == NULL )
	{
		ai_pzApplicationServerName = "" ;
	}

	if( ai_pszSSLFile == NULL || *ai_pszSSLFile == '\0' )
	{
		sprintf( acTempBuffer, "%s%s%s", ai_LogonAdaptor->_GetEnvironment()->GetWorkingDirectory(), M4_BACK_SLASH, DEFAULT_SSL_FILE ) ;
		ai_pszSSLFile = acTempBuffer ;
	}


	DUMP_CHLOG_DEBUGF( M4_CH_CS_LOGON_NOTIFY, ai_pszUserName << ai_pzApplicationServerName << ai_uiApplicationServerPort << ai_Language ) ;

	// Sólo por si acaso
	if( ai_bUseCredential == M4_TRUE )
	{
		ai_bUseDefaultRole = M4_TRUE ;
	}

		// We need a valid language
	M4ClLangInfo::Instance()->CheckAndSetValidLangId( ai_Language ) ;

	ai_LogonAdaptor->m_Language = ai_Language ;

	//Caches
	if( ai_LogonAdaptor->m_poCacheDirectory != NULL )
	{
		ai_LogonAdaptor->m_poCacheDirectory->ActivateCaches() ;
	}

	// Get logon type: kerberos or m4 logon.
	m4int32_t iLogonType = 0;
	m4char_t pszLogonType[256];
	if (ai_LogonAdaptor->_GetConnectionParam(LOGON_TYPE_TAG, pszLogonType) != M4_SUCCESS)
	{
		// If we do not find logon type, we will take on m4 logon.
		iLogonType = M4_LOGON_TYPE;
	}
	else
	{
		// Check logon type value.
		iLogonType = atoi(pszLogonType);
		if ((iLogonType == M4_LOGON_TYPE) || (iLogonType == KERBEROS_LOGON_TYPE))
		{
			Args.SetArg(LOGON_TYPE_TAG, pszLogonType);
		}
		else
		{
			// By default, m4 logon type.
			DUMP_CHLOG_WARNING(M4_CH_CS_WRONG_LOGON_TYPE);
			Args.SetArg(LOGON_TYPE_TAG, _itoa(M4_LOGON_TYPE, pszLogonType, 10));
		}
	}

	iSmOltp = ai_LogonAdaptor->_GetSmOltp() ;

	// Save user and password.
	m4pchar_t pszPassword = ai_pszPassword;
	m4pchar_t pszUserName = ai_pszUserName;

	// Si no estamos en SingleMachine, hacemos OpenSession con todos los arg. pertinentes y cogemos la fecha de MetaDatos.
	// Si estamos en Single Machine esta fecha viene inicializada a 0 (tambien hacemos el OpenSession)
	eStateValidateExecutionNp_t eStateValidateExecutionNpSerialized;

   if( iSmOltp == 0 )
   {
		// bg 114359
		iResult = ai_LogonAdaptor->_GetAPIClientAdaptor()->SetCommsLanguage( ai_Language ) ;

		// bg 118778
		/*
		if( iResult != M4_SUCCESS )
		{
			DUMP_CHLOG_ERRORF( M4_CS_CANNOT_SET_COMM_LANG, ai_Language ) ;
		}
		*/

		//El cliente le va a pedir al server por el estado. 
	    //De esta forma, el cliente puede saber el estado Np que ha calculado el server.
		Args.SetTag( M4_PROP_SERVER_LICENSE_STATE_VALIDATE_EXECUTION_NP ) ;

		Args.SetTag( M4_PROP_SERVER_ARCHITECTURE ) ;

		if( ai_bUseCredential == M4_FALSE )
		{
			Args.SetTag( M4_PROP_SESSION_CREDENTIAL ) ;
		}

		// Se pide el timeout de la sessión
		Args.SetTag( M4_PROP_SESSION_TIMEOUT ) ;

      m4char_t    pzAuxStr[256];

      // Compression
      if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_COMPRES_TYPE_TAG,
                                              pzAuxStr)==M4_SUCCESS)
      {
        // the compress type is a number.
        m4int32_t ctype = atoi(pzAuxStr);
        Args.SetArg(M4_COMPRESSION_TYPE, ctype);

       /*
       if (strcmp(pzAuxStr,TRANSP_PROT_COMPRES_TYPE_VALUE_NONE)==0)
            Args.SetArg(M4_COMPRESSION_TYPE,TRANSP_PROT_COMPRES_TYPE_VALUE_NONE_APIC);
         else
            if (strcmp(pzAuxStr,TRANSP_PROT_COMPRES_TYPE_VALUE_CLASSIC)==0)
               Args.SetArg(M4_COMPRESSION_TYPE,TRANSP_PROT_COMPRES_TYPE_VALUE_CLASSIC_APIC);
            else
               if (strcmp(pzAuxStr,TRANSP_PROT_COMPRES_TYPE_VALUE_ZDELTA)==0)
                  Args.SetArg(M4_COMPRESSION_TYPE,TRANSP_PROT_COMPRES_TYPE_VALUE_ZDELTA_APIC);
               else
                  Args.SetArg(M4_COMPRESSION_TYPE,DEFAULT_TRANSP_PROT_COMPRES_TYPE_VALUE);
        */
      }
      else
      {
        Args.SetArg(M4_COMPRESSION_TYPE,DEFAULT_TRANSP_PROT_COMPRES_TYPE_VALUE);
      }

      /* the compress level is deprecated. Set to high compression level.
      if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_COMPRES_LEVEL_TAG,
                                              pzAuxStr)==M4_SUCCESS)
         if (strcmp(pzAuxStr,TRANSP_PROT_COMPRES_LEVEL_VALUE_LOW)==0)
            Args.SetArg(M4_COMPRESSION_LEVEL,TRANSP_PROT_COMPRES_LEVEL_VALUE_LOW_APIC);
         else
            if (strcmp(pzAuxStr,TRANSP_PROT_COMPRES_LEVEL_VALUE_MED)==0)
               Args.SetArg(M4_COMPRESSION_LEVEL,TRANSP_PROT_COMPRES_LEVEL_VALUE_MED_APIC);
            else
               if (strcmp(pzAuxStr,TRANSP_PROT_COMPRES_LEVEL_VALUE_HIGH)==0)
                  Args.SetArg(M4_COMPRESSION_LEVEL,TRANSP_PROT_COMPRES_LEVEL_VALUE_HIGH_APIC);
               else
                  Args.SetArg(M4_COMPRESSION_LEVEL,DEFAULT_TRANSP_PROT_COMPRES_LEVEL_VALUE);
       */
      Args.SetArg(M4_COMPRESSION_LEVEL, DEFAULT_TRANSP_PROT_COMPRES_LEVEL_VALUE);

      // Protocol dependent part
      m4uint16_t iTransportProtocol=DEFAULT_TRANSP_PROT_TYPE_VALUE;
 
      if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_TYPE_TAG,
                                              pzAuxStr)==M4_SUCCESS)
         if (strcmp(pzAuxStr,TRANSP_PROT_TYPE_VALUE_M4)==0)
            iTransportProtocol=TRANSP_PROT_TYPE_VALUE_M4_NUM; // Redundante, ya lo se
         else
            if (strcmp(pzAuxStr,TRANSP_PROT_TYPE_VALUE_HTTP)==0)
               iTransportProtocol=TRANSP_PROT_TYPE_VALUE_HTTP_NUM; 

      m4char_t   pzNameOfServerToConnect[256]; // The real address to connect form the client API
      m4uint32_t iPortOfServerToConnect; // The real port to connect form the client API

      switch (iTransportProtocol)
      {
         case TRANSP_PROT_TYPE_VALUE_HTTP_NUM :
            if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_SERVLET_NAME_TAG,
                                                    pzAuxStr)!=M4_SUCCESS)
               Args.SetArg(M4_HTTP_PROP_SERVLET,DEFAULT_TRANSP_PROT_SERVLET_NAME); // File name of the servlet to use
            else
               Args.SetArg(M4_HTTP_PROP_SERVLET,pzAuxStr); // File name of the servlet to use
         
            if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_WEB_SERVER_NAME_TAG,
                                                    pzAuxStr)!=M4_SUCCESS)
            {
            	DUMP_CHLOG_DEBUG(M4_CH_CS_NO_WEB_SERVER_NAME);

               return M4_ERROR; // Needs that info
            }
            else
            {
               // We really connect to the web server, but set the application server name
               Args.SetArg(M4_HTTP_PROP_SERVER,ai_pzApplicationServerName);

               strcpy(pzNameOfServerToConnect,pzAuxStr);
            }

            if (!ai_bUseSSL)
            {
               if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_WEB_SERVER_PORT_TAG,
                                                       pzAuxStr)!=M4_SUCCESS)
                  strcpy(pzAuxStr,DEFAULT_TRANSP_PROT_WEB_SERVER_PORT);
            }
            else
            {
               if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_WEB_SERVER_SECURE_PORT_TAG,
                                                       pzAuxStr)!=M4_SUCCESS)
                  strcpy(pzAuxStr,DEFAULT_TRANSP_PROT_WEB_SERVER_SECURE_PORT);
            }

            iPortOfServerToConnect=atoi(pzAuxStr);

            if (iPortOfServerToConnect==0)
               return M4_ERROR;

            // bugid: 0107831
            // Ahora se soporta SSL entre el web y el appserver, por lo que el parámetro
            // ai_uiApplicationServerPort ya viene con el puerto correcto (el 3002 o el 3003)
            // según corresponda.
            /*if (ai_bUseSSL)
            {
               if (ai_LogonAdaptor->_GetConnectionParam(TRANSP_PROT_APP_SERVER_NON_SECURE_PORT_TAG,
                                                       pzAuxStr)!=M4_SUCCESS)
               {
               	DUMP_CHLOG_DEBUG(M4_CH_CS_NO_NON_SECURE_APP_SERVER_PORT);

                  return M4_ERROR; // Needs that info
               }
               else
               {
                  // Change the port of the app server to connect
                  Args.SetArg(M4_HTTP_PROP_SERVER_PORT,pzAuxStr);
               }
            }
            else
            */
            {
               // In this case is not mandatory...
               m4char_t pzAuxBuffer[64];
               Args.SetArg(M4_HTTP_PROP_SERVER_PORT,_itoa(ai_uiApplicationServerPort,pzAuxBuffer,10));
            }

            // Unconditional ones...
		    /*
            deprecated.
            Args.SetArg(M4_HTTP_PROP_COOKIE_ID,M4_COOKIE_ID);
		    Args.SetArg(M4_HTTP_PROP_EXTRA_TIMEOUT,HTTP_EXTRA_TIMEOUT);
            */
            break;

         case TRANSP_PROT_TYPE_VALUE_M4_NUM :
            strcpy(pzNameOfServerToConnect,ai_pzApplicationServerName);
            iPortOfServerToConnect=ai_uiApplicationServerPort;
            break;

         default :
            M4_ASSERT(0); // It should be impossible...
      }

	  // sends the language id
	  {
		  m4char_t pzAuxBuffer[64];
		  Args.SetArg(M4_SESS_USER_LANGUAGE,_itoa(ai_Language,pzAuxBuffer,10));
	  }

	   // Pedimos al server que nos devuelva el id usuario ...
	   Args.SetArg( M4_SESS_REAL_USER_ID, "SetTheValuePlease" ) ;
	   // el alias si procede ...
	   Args.SetArg( M4_SESS_USER_ALIAS, "SetTheValuePlease" ) ;
	   // y el usuario suplantador si procede.
	   Args.SetArg( M4_SESS_IMPERSONATOR_USER, "SetTheValuePlease" ) ;


	   M4ClString tmpPassword( ai_pszPassword ) ;
	   M4ClString tmpSSLFile( ai_pszSSLFile ) ;
	   M4ClString tmpServerName(pzNameOfServerToConnect); 

	   if( ai_bUseCredential == M4_FALSE )
	   {
			M4ClString tmpUserName( ai_pszUserName ) ;

			iResult = ai_LogonAdaptor->_GetAPIClientAdaptor()->OpenSession(
															tmpServerName, 
															iPortOfServerToConnect,
															tmpUserName,
															tmpPassword,
															ai_bUseDefaultRole,
															&Args,
															ai_bUseSSL,
															tmpSSLFile
															);
	   }
	   else
	   {
			iResult = ai_LogonAdaptor->_GetAPIClientAdaptor()->OpenSession(
															tmpServerName, 
															iPortOfServerToConnect,
															tmpPassword,
															&Args,
															ai_bUseSSL,
															tmpSSLFile
															);
	   }

		//---------------------------------------------------------------------
	   //Obtenemos el estado que viene serializado del server.
		m4pchar_t	pcTemp = acTempBuffer;
		*pcTemp = '\0' ;
		if( Args.GetArg( M4_PROP_SERVER_LICENSE_STATE_VALIDATE_EXECUTION_NP, pcTemp, 2 * sizeof( m4char_t ) ) != M4_SUCCESS )		{
			return( M4_ERROR ) ;
		}
		switch(*pcTemp)
		{			
		case '0' :
			eStateValidateExecutionNpSerialized = STATE_VALIDATE_EXECUTION_NP_GREEN;
			break;
			
		case '1' :
			eStateValidateExecutionNpSerialized = STATE_VALIDATE_EXECUTION_NP_YELLOW;
			break;
			
		case '2' :
			eStateValidateExecutionNpSerialized = STATE_VALIDATE_EXECUTION_NP_RED;
			break;
			
		default:
			eStateValidateExecutionNpSerialized = STATE_VALIDATE_EXECUTION_NP_GREEN;
			break;
		}
		//---------------------------------------------------------------------

	   if (iTransportProtocol==TRANSP_PROT_TYPE_VALUE_HTTP_NUM)
      {
	      if (iResult==M4_SUCCESS)
         {
            m4pchar_t pzAuxCookie;

            pzAuxCookie = new m4char_t[512];

		      if (Args.GetArg(M4_HTTP_PROP_COOKIE,pzAuxCookie,sizeof(pzAuxCookie))==M4_SUCCESS)
               ai_LogonAdaptor->_SetPublicCookie(pzAuxCookie);
            else
               ai_LogonAdaptor->_SetPublicCookie("");

            delete pzAuxCookie;
         }
      }

	  // Valores finales para el canal sesión.
	  if( Args.GetArg( M4_SESS_REAL_USER_ID, pcRealUserId, M4_MAX_USER_ID * sizeof( m4char_t ) ) != M4_SUCCESS )
	  {
		return( M4_ERROR ) ;
	  }

	  pcSessUserID = pcRealUserId ;

	  if( Args.GetArg( M4_SESS_USER_ALIAS, pcUserAlias, M4_MAX_USER_ALIAS * sizeof( m4char_t ) ) == M4_SUCCESS )
	  {
		pcSessUserAlias = pcUserAlias ;
	  }

	  if( Args.GetArg( M4_SESS_IMPERSONATOR_USER, pcUserImpersonated, M4_MAX_USER_ID * sizeof( m4char_t ) ) == M4_SUCCESS )
	  {
		pcSessUserImpersonator = pcUserImpersonated ;
	  }

	  if( Args.GetArg( M4_CLIENT_PUB_IP, pcClientPubIP, M4_MAX_CLIENT_IP * sizeof( m4char_t ) ) != M4_SUCCESS )
	  {
		*pcClientPubIP = '\0';
	  }
	}
	else
	{
		//En SM también es necesario hacer un openSession, aunque los argumentos se ignoren.

		// Get local host name.
		// Init sockets, to be able to obtain host name.
		SocketInit();
		
		//En el tamaño del buffer incluimos el nulo final.
		if (M4_gethostname(acHostName, 256) != 0)
		{
			return M4_ERROR;
		}

		M4ClString oEmptyString("");
		iResult = ai_LogonAdaptor->_GetAPIClientAdaptor()->OpenSession( oEmptyString, 
																		0,
																		oEmptyString,
																		(ClTagList*)0,
																		M4_FALSE,
																		oEmptyString
																		);
		
		//Logon LDAP.
		//Vamos a convertir two-way
		m4int_t	iLength = -1 ;
		m4pchar_t pcPassword = NULL;

		iResult = EncryptTwoWay( pszPassword, strlen( pszPassword ) * sizeof( m4char_t ), M4CH_DUMMY_B2 + 23, strlen( M4CH_DUMMY_B2 + 23 ) * sizeof(m4char_t), pcPassword, iLength ) ;

		if (iResult != M4_SUCCESS)
		{
			pcPassword = new m4char_t[strlen(ai_pszPassword) + 1];
			strcpy(pcPassword, ai_pszPassword);
		}
		
		//En SM quitamos el refreh
		//iResultCac = ai_LogonAdaptor->m_poCacheDirectory->Refresh();

		iResult = ai_LogonAdaptor->_Authenticate(pszUserName, pcPassword, ai_bUseDefaultRole, iLogonMechanism, pcRealUserId, pcUserImpersonated);
		delete pcPassword;


		// Valores finales para el canal sesión.
		pcSessUserID = pszUserName ;

		// Si hay logon con alias.
		if( *pcRealUserId != '\0' )
		{
			pcSessUserID = pcRealUserId ;
			pcSessUserAlias = pszUserName ;
		}
		
		// Si hay logon con suplantación.
		if( *pcUserImpersonated != '\0' )
		{
			pcSessUserID = pcUserImpersonated ;

			// y además autenticación mediante alias, dejamos como suplantador el ID real.
			if( *pcRealUserId != '\0' )
			{
				pcSessUserImpersonator = pcRealUserId ;
			}
			else
			{
				pcSessUserImpersonator = pszUserName ;
			}
		}
	}

	
// Si es error o negativo retornamos
// Si success o positivo seguimos y lo devolvemos al final

	if( iResult < 0 )
	{
		return( iResult ) ;
	}

	if( iResult >= 0 )
	{
// lo cambiamos aquí ya. Está logado sin role si no es un error
		ai_LogonAdaptor->_ChangeState( ClLogWithoutRole :: Instance(), M4CL_LOG_WITHOUT_ROLE ) ;
	}
	
	iWarning = iResult ;


    if( iSmOltp == 0 )
    {
		double		dTemp = 0 ;
		m4pchar_t	pcTemp = acTempBuffer ;

		*pcTemp = '\0' ;

		if( Args.GetArg( M4_PROP_SERVER_ARCHITECTURE, pcTemp, M4_MAX_ARCH_SIZE * sizeof( m4char_t ) ) != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}

		switch( toupper( *pcTemp ) )
		{
			
		case 'S' :
			ai_LogonAdaptor->m_uiServerArchitect = M4_ARCH_SPARC ;
			break;
			
		case 'P' :
			ai_LogonAdaptor->m_uiServerArchitect = M4_ARCH_PA_RISC ;
			break;
			
		case 'D' :
			ai_LogonAdaptor->m_uiServerArchitect = M4_ARCH_DIGITAL ;
			break;
			
		case 'A' :
			ai_LogonAdaptor->m_uiServerArchitect = M4_ARCH_AIX ;
			break;
			
		case 'I': 
		default:
			ai_LogonAdaptor->m_uiServerArchitect = M4_ARCH_INTEL ;
			break;
		}

		
		if( ai_bUseCredential == M4_FALSE )
		{
			pcTemp = ai_LogonAdaptor->GetCredential() ;

			if( Args.GetArg( M4_PROP_SESSION_CREDENTIAL, pcTemp, M4CL_MAX_CREDENTIAL_SIZE * sizeof( m4char_t ) ) != M4_SUCCESS )
			{
				*pcTemp = '\0' ;
			}
		}

		// Se asigna el timeout de la sessión
		if( Args.GetArg( M4_PROP_SESSION_TIMEOUT, dTemp ) != M4_SUCCESS )
		{
			dTemp = 0 ;
		}

		ai_LogonAdaptor->m_dSessionTimeout = dTemp ;
	}

    if( iSmOltp == 0 || ai_LogonAdaptor->_GetEnvironment()->GetSMCacheRefresh() == 1 )
    {
		iResultCac = ai_LogonAdaptor->m_poCacheDirectory->Refresh();
	}


// Nos construimos el canal sesión de manera explícita, para que quede claro
    iResult = ai_LogonAdaptor->BuildSessionChannel() ;

	if( iResult != M4_SUCCESS )
	{
		return( iResult ) ;
	}

	// Obtener información de la máquina cliente para meterla en el canal sesión.
	// Sólo para SM, en DIC viene por tag M4_SYSTEM_INFO.
	if (iSmOltp == 1)
	{
		// Información del sistema.
		poSystemInfo = new ClSystemInfo();
		poSystemInfo->Serialize(sSystemInfo);
		delete poSystemInfo;
		
		// Tipo de cliente SM.
		sClientType = M4_CLIENT_TYPE_SM;
	}
	else
	{
		// Tipo de cliente Rich.
		sClientType = M4_CLIENT_TYPE_RC;
	}

	// eh 272310
	if( *acHostName != 0 )
	{
		pHostInfo = gethostbyname( acHostName ) ;
		if( pHostInfo != NULL )
		{
			pcIPAddress = inet_ntoa( *( in_addr* )pHostInfo->h_addr );
			if( pcIPAddress != NULL )
			{
				strncpy( acHostName, pcIPAddress, 255 );
				acHostName[ 255 ] = 0;
			}
		}
	}

    iResult = ai_LogonAdaptor->InsertUserData(pcSessUserID, ai_Language, iLogonMechanism, acHostName, pcSessUserAlias, pcSessUserImpersonator, (m4pchar_t)sSystemInfo.c_str(), (m4pchar_t)sClientType.c_str(), pcClientPubIP);


	if( iResult != M4_SUCCESS )
	{
		return( iResult ) ;
	}

	// En SM con Role por defecto no se hace refresh
	if( iSmOltp == 1 && ai_bUseDefaultRole == M4_TRUE )
	{
		iRefresh = 0 ;
	}

	if( iRefresh == 1 )
	{
		iResult = ai_LogonAdaptor->_RefreshSessionChannel( ai_bUseDefaultRole ) ;

		if( iResult != M4_SUCCESS )
		{
			return( iResult ) ;
		}
	}

	//El _UpdateRSM sera el que inicializa la licencia. Cuando es DIC el estado lo ha dado el server.
	//En SM el estado se obtiene despues de la inicializacion.
	//Ojito, en el _UpdateRSM es donde realmente se hara la inicilizacion de la licencia.
	if (iSmOltp == 0) {
		ai_LogonAdaptor->_SetStateValidateExecutionNp( eStateValidateExecutionNpSerialized );
	}

	if( ai_bUseDefaultRole == M4_TRUE && iWarning == M4_SUCCESS )
	{
		if( iSmOltp == 1 )
		{
			iResult = ai_LogonAdaptor->_EnableRole( NULL, M4_FALSE ) ;

			if( iResult < 0 )
			{
				return( iResult ) ;
			}
	
			iWarning = iResult ;
		}

		if( iResult == M4_SUCCESS )
		{
			iResult = ai_LogonAdaptor->_SetDefaultRoleAsActive() ;

			if( iResult != M4_SUCCESS )
			{
				return( iResult ) ;
			}
		}
	}

	return( iWarning ) ;
}



m4return_t ClLogWithoutRole :: SetRole( ClLogonAdaptor* ai_LogonAdaptor, m4int32_t ai_iPos,	m4bool_t ai_bSetAsDefaultRole )
{
	return _SetRole( ai_LogonAdaptor, ai_iPos, ai_bSetAsDefaultRole ) ;
}	
