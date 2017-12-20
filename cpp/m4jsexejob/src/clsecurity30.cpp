//## begin module%35AB212D03C2.cm preserve=no
//## end module%35AB212D03C2.cm

//## begin module%35AB212D03C2.cp preserve=no
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
//## end module%35AB212D03C2.cp

//## Module: ClSecurity30%35AB212D03C2; Package body
//## Subsystem: M4JSExeJob::src%37F8BA6602AA
//## Source file: D:\Work\v600\m4jsexejob\src\clsecurity30.cpp

//## begin module%35AB212D03C2.additionalIncludes preserve=no
//## end module%35AB212D03C2.additionalIncludes

//## begin module%35AB212D03C2.includes preserve=yes
#include <m4jsres.hpp>
#include <m4log.hpp>
#include <m4jsdefinechannel.hpp>
#include "clm4objservice.hpp"
#include "conn_provider.hpp"
//LogonInterface
#include <logoninterface.hpp>
//Channel
#include <channel.hpp>
//UserInterface
#include <usinterface.hpp>
//executor
#include <executor.hpp>
//StatsJSExecute
#include <clstatsjsexecute.hpp>
// LDB Interface
#include <m4ldb.hpp>
// Factory interface
#include <clfactm4objservice.hpp>
#include <clarraylibres.hpp>
//## end module%35AB212D03C2.includes

// access
#include <access.hpp>
// ClSecurity30
#include <clsecurity30.hpp>
// ClSecurity
#include <clsecurity.hpp>


//## begin module%35AB212D03C2.declarations preserve=no
//## end module%35AB212D03C2.declarations

//## begin module%35AB212D03C2.additionalDeclarations preserve=yes
#define SIZE_STR 50
#define NODE_CHANNEL_USER  "ROOT_SESSION"
#define METHOD_SET_PARAMS_CHANNEL_USER "SET_PARAMS"
//## end module%35AB212D03C2.additionalDeclarations


// Class ClSecurity30 






ClSecurity30::ClSecurity30 (ClM4ObjService* ai_poObjService, ClLogonInterface *ai_poLogon, ClStatsJSEXECUTE *ai_poStats)
  //## begin ClSecurity30::ClSecurity30%900415030.hasinit preserve=no
  //## end ClSecurity30::ClSecurity30%900415030.hasinit
  //## begin ClSecurity30::ClSecurity30%900415030.initialization preserve=yes
  //## end ClSecurity30::ClSecurity30%900415030.initialization
{
  //## begin ClSecurity30::ClSecurity30%900415030.body preserve=yes

	m_poObjService = ai_poObjService;

	if (m_poObjService)
		m_poChannelManager = m_poObjService->GetpChannelManager();
	else
		m_poChannelManager = 0;
	
	m_poLogon=ai_poLogon;

	m_poStats=ai_poStats;

	m_poUSSesion=NULL;

	m_poChannelUser=NULL;

	m_poAccessUSer=NULL;

	m_poChannelCredential=NULL;

  //## end ClSecurity30::ClSecurity30%900415030.body
}


ClSecurity30::~ClSecurity30 ()
{
  //## begin ClSecurity30::~ClSecurity30%900415029.body preserve=yes
	if (m_poLogon)
		delete m_poLogon;
  //## end ClSecurity30::~ClSecurity30%900415029.body
}



//## Other Operations (implementation)
m4return_t  ClSecurity30::GetUserRole (m4pchar_t  ai_pBuffer, m4pchar_t  ai_pUser, m4pchar_t  ai_pRole, m4pchar_t ai_pOrganization, m4pchar_t ai_pParamsChannelUser)
{
  //## begin ClSecurity30::GetUserRole%900415031.body preserve=yes

	m4uint32_t	i = 0;
	m4uint32_t	j = 0;
	m4uint32_t	k = 0;
	m4bool_t	is_good_string = M4_TRUE;
	m4uint32_t	uiLCredential  = strlen(ai_pBuffer);
	m4pcchar_t	pAux = NULL;
	m4pcchar_t	pOrganization = NULL;
	m4int32_t	iOrganizationLen = 0;
	m4int32_t	iPos = 0;

	// Inicializar.
	*ai_pUser = '\0';
	*ai_pRole = '\0';
	*ai_pOrganization = '\0';

	// Obtenemos el usuario
	while ((ai_pBuffer[i]!= '|') && (is_good_string == M4_TRUE) && (k <= uiLCredential))
	{
		if (ai_pBuffer[i] == '\0')
		{   
			// Si llegamos al final de la cadena y no a un | significa que está mal construida la cadena de la
			// credencial.
			is_good_string = M4_FALSE;
		}
		else
		{
			ai_pUser[j] = ai_pBuffer[i];
			i++;
			j++;
		}

		k++;
	}

	if (is_good_string == M4_FALSE)
	{
		SETCODEF(M4_ERROR_SECURITY_STRING_30,ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	// Si hemos superado el buffer alocado se considera un error
	if (k > uiLCredential)
	{
		SETCODEF(M4_ERROR_SECURITY_STRING_30,ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	ai_pUser[j] = '\0';

	i++;
	j = 0;
	k = 0;

	//Obtenemos el rol con el que el usuario va a ejecutar la tarea.
	while (((ai_pBuffer[i] != '\0') && (ai_pBuffer[i] != '|')) && (k <= uiLCredential))
	{
		ai_pRole[j] = ai_pBuffer[i];
		i++;
		j++;
		k++;
	}

	// Aqui si llegamos al final de la cadena y no nos encotramos con el 
	// contexto se considera correcto.
	if (uiLCredential < k)
	{
		SETCODEF(M4_ERROR_SECURITY_STRING_30,ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	ai_pRole[j] = '\0';

	// Si ya hemos llegado al final de la cadena consideramos que la cadena del contexto es vacía.
	if (ai_pBuffer[i] == '\0')
	{
		*ai_pParamsChannelUser = '\0';
		return M4_SUCCESS;
	}

	if (ai_pBuffer[i] == '|')
	{
		if (ai_pBuffer[i + 1] == '\0')
		{
			*ai_pParamsChannelUser = '\0';
		}
		else
		{
			i++;
			k = 0;
			while (ai_pBuffer[i] != '\0')
			{
				ai_pParamsChannelUser[k] = ai_pBuffer[i];
				i++;
				k++;
			}
			ai_pParamsChannelUser[k] = '\0';
		}
	}
	else
	{
		SETCODEF(M4_ERROR_SECURITY_STRING_30,ERRORLOG,"#*s1*#");
		return M4_ERROR;
	}

	// Obtener la sociedad también de la credencial (|ROOT_SESSION#ID_ORGANIZATION#"0001").
	pAux = strstr(ai_pBuffer, "#ID_ORGANIZATION#");
	if (pAux != NULL)
	{
		iOrganizationLen = strlen("#ID_ORGANIZATION#");
		pAux += iOrganizationLen;

		// Saltar la primera comilla.
		pAux++;

		// Buscar siguiente comilla para extraer el valor de la sociedad.
		pOrganization = strchr(pAux, '"');
		if (pOrganization != NULL)
		{
			iPos = pOrganization - pAux;
			strncpy(ai_pOrganization, pAux, iPos);
			ai_pOrganization[iPos] = '\0';
		}
	}

	return M4_SUCCESS;

  //## end ClSecurity30::GetUserRole%900415031.body
}

m4return_t  ClSecurity30::ChangeChannelUser (m4pchar_t  ai_pUser, m4pchar_t  ai_pRole, m4pchar_t ai_pOrganization, m4pchar_t ai_pParamsChannelUser)
{
  //## begin ClSecurity30::ChangeChannelUser%900415032.body preserve=yes
	
	ClChannel *	poChannelUser = NULL;
	
	m_poUSSesion = NULL;
	
	// Llamamos al método que abre la sesión este método es estático
	// Este es el método que testea si se cumple las condiciones de seguridad con respecto al usuario y al rol que 
	// le pasamos.
	m4return_t ret = m_poLogon->OpenSessionEx(ai_pUser, ai_pRole, m_poUSSesion);
	if (m_poUSSesion == NULL)
	{
		SETCODEF(M4_POINTER_NULL, ERRORLOG, "#*s1*#m_poUSSesion#");
		SETCODEF(M4_ERROR_OPEN_NEW_SESSION, ERRORLOG, "#*s1*#%s#%s#", ai_pUser, ai_pRole);
		ReleaseConn();
		return  M4_VIOLATION_SECURITY_JS;
	}
	
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_OPEN_NEW_SESSION, ERRORLOG, "#*s1*#%s#%s#", ai_pUser, ai_pRole);	
		ReleaseConn();
		return ret;
	}
	
	// Obtenemos el canal sesión del usuario
	ClUSSessionTicketInterface * poTicketInterface;
	
	// Primero obtenemos el ticket de la sesión
	ret = m_poUSSesion->GetSessionTicket(poTicketInterface);
	if (NULL == poTicketInterface)
	{			
		SETCODEF(M4_POINTER_NULL,ERRORLOG, "#*s1*#poTicketInterface#");
		SETCODEF(M4_ERROR_GET_TICKET_SESSION, ERRORLOG, "#*s1*#%s#%s#", ai_pUser, ai_pRole);
		ReleaseConn();
		return M4_ERROR;
	}
	
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_ERROR_GET_TICKET_SESSION, ERRORLOG, "#*s1*#%s#%s#", ai_pUser, ai_pRole);
		ReleaseConn();
		return M4_ERROR;
	}
	
	// A partir del ticket se obtiene el canal sesión
	m_poChannelCredential = (ClChannel *)poTicketInterface->GetTicketCode();
	if (m_poChannelCredential == NULL)
	{
		SETCODEF(M4_POINTER_NULL,ERRORLOG, "#*s1*#poChannelUser#");
		SETCODEF(M4_CHANGE_CHANNEL_USER_ERROR, ERRORLOG, "#*s1*#");
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Obtenemos el canal sesión que tiene por defecto la máquina virtual con permisos de superusuario
	// y lo guardamos para restituirlo posteriormente.
	m_poChannelUser = m_poChannelManager->GetSessionChannel();
	if (m_poChannelUser == NULL)
	{ 
		SETCODEF(M4_POINTER_NULL, ERRORLOG, "#*s1*#m_poChannelUser#");
		SETCODEF(M4_ERROR_GET_DEFAULTCHANNELUSER, ERRORLOG, "#*s1*#");
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Cambiamos la seguridad de la máquina virtual
	m_poChannelManager->SetSessionChannel(m_poChannelCredential);
	if (m_poChannelUser->Detach() != M4_SUCCESS)
	{
		SETCODEF(M4_SRV_SESSION_CHANNEL, ERRORLOG, "#*s1*#");
		return M4_ERROR;
	}
	
	// Establecemos una relación entre la máquina virtual y el nuevo canal sesión
	if (m_poChannelCredential->Attach(m_poChannelManager) != M4_SUCCESS)
	{
		SETCODEF(M4_SRV_SESSION_CHANNEL, ERRORLOG, "#*s1#");
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Configuramos el canal sesión de acuerdo a los parámetros indicados por el usuario de la tarea
	m4VariantType arg;
	
	// Creamos el acceso al canal de usuarios
	m_poAccessUSer = m_poChannelCredential->CreateAccess();
	if (NULL == m_poAccessUSer)
	{
		SETCODEF(M4_ERROR_ACCESS_META4OBJECTUSER, ERRORLOG, "#*s1*#%s#%s#", ai_pUser, ai_pRole);
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Accedemos al nodo donde se encuentran los campos que tenemos que rellenar para establecer el contexto de la seguridad
	ClNode * poNodeChannelUser = &(m_poAccessUSer->Node[ NODE_CHANNEL_USER]);
	if (NULL == poNodeChannelUser)
	{
		SETCODEF(M4_ERROR_CREATE_NODE, ERRORLOG, "#*s1*#ROOT_SESSION#SCH_SESSION#");
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Bug 0273907. Establecer el campo sociedad antes de llamar al método SET_PARAMS.
	arg.Type = M4CL_CPP_TYPE_STRING_VAR;
	if (ai_pOrganization != NULL && strlen(ai_pOrganization) != 0)
	{
		arg.Data.PointerChar = ai_pOrganization;
	}
	else
	{
		arg.Data.PointerChar = NULL;
	}
	
	ret = m_poAccessUSer->GetpExecutor()->Stack.Push(arg);
	ret = poNodeChannelUser->RecordSet.Current.Item["ID_ORGANIZATION"].Value.Set(arg);
	if (M4_SUCCESS != ret)
	{		
		SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#ID_ORGANIZATION#ROOT_SESSION#SCH_SESSION#");
		ReleaseConn();
		return M4_ERROR;
	}

	// Bug 0273907. Establecer el item SYNCHRONIZE_SERVER antes de llamar al método SET_PARAMS.
	arg.Type = M4CL_CPP_TYPE_NUMBER;
	arg.Data.DoubleData = 0;
	ret = m_poAccessUSer->GetpExecutor()->Stack.Push(arg);
	ret = poNodeChannelUser->RecordSet.Current.Item["SYNCHRONIZE_SERVER"].Value.Set(arg);
	if (M4_SUCCESS != ret)
	{		
		SETCODEF(M4_SET_VALUE_ERROR, ERRORLOG, "#*s1*#SYNCHRONIZE_SERVER#ROOT_SESSION#SCH_SESSION#");
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Almacenamos la cadena que contiene el contexto en la pila de los meta4object
	arg.Type = M4CL_CPP_TYPE_STRING_VAR;
	arg.Data.PointerChar = ai_pParamsChannelUser;
	ret = m_poAccessUSer->GetpExecutor()->Stack.Push(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_PUSH_STACK_ERROR, ERRORLOG, "#*s1*#SET_PARAMS#SCH_SESSION");
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Llamamos al método que parsea la cadena para poder obtener los valores con los que tiene que rellenar 
	// los campos que configuran el contexto del canal sesión: moneda,idioma,role ...
	ret = poNodeChannelUser->RecordSet.Current.Item[METHOD_SET_PARAMS_CHANNEL_USER].Call();
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"#*s1#SET_PARAMS#CH_SESSION#");
		ReleaseConn();
		return M4_ERROR;
	}
	
	// Recuperamos el valor que retorna el método
	ret = m_poAccessUSer->GetpExecutor()->Stack.Pop(arg);
	if (M4_SUCCESS != ret)
	{
		SETCODEF(M4_POP_STACK_ERROR,ERRORLOG,"#*s1*#SET_PARAMS#SET_PARAMS#SCH_SESSION");
		ReleaseConn();
		return M4_ERROR;
	}
	
	m4bool_t IsThereAnyError = M4_FALSE;
	
	// Determinamos si nos han devuelto algún valor que indique error
	if (arg.Data.DoubleData == -1)
	{
		SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"#*s1*#SET_PARAMS#ROOT_SESSION#SCH_SESSION#");
		IsThereAnyError=M4_TRUE;
	}
	
	if (arg.Data.DoubleData == -2)
	{
		SETCODEF(M4_ERROR_PARAMETERS_META4OBJECTUSER,ERRORLOG,"#*s1*#SET_PARAMS#ROOT_SESSION#SCH_SESSION#");
		IsThereAnyError = M4_TRUE;
	}
	else
	{
		if (arg.Data.DoubleData < 0)
		{
			SETCODEF(M4_EXECUTE_METHOD_ERROR,ERRORLOG,"#*s1*#SET_PARAMS#ROOT_SESSION#SCH_SESSION#");
			IsThereAnyError = M4_TRUE;	
		}
	}
	
	if (IsThereAnyError == M4_TRUE)
	{
		ReleaseConn();
		return M4_ERROR;
	}
	
	NotifyLogon(m_poUSSesion->GetId());
	
	return M4_SUCCESS;
  //## end ClSecurity30::ChangeChannelUser%900415032.body
}

m4return_t  ClSecurity30::ChangeSecurity (m4pchar_t ai_poSecurityString)
{
  //## begin ClSecurity30::ChangeSecurity%900415034.body preserve=yes


		m4uint32_t uiLenCredential=strlen(ai_poSecurityString);

		m4pchar_t poUser=new m4char_t[uiLenCredential+2];
		m4pchar_t poRole=new m4char_t [uiLenCredential+2];
		m4pchar_t poOrganization=new m4char_t [uiLenCredential+2];
		m4pchar_t poParamsChannelUser=new m4char_t[uiLenCredential+2];

		
	m4return_t ret = GetUserRole(ai_poSecurityString, poUser, poRole, poOrganization, poParamsChannelUser);
	if (M4_ERROR == ret)
	{
		delete [] poUser;
		delete [] poRole;
		delete [] poParamsChannelUser;
		return M4_ERROR;
	}

	ret = ChangeChannelUser(poUser, poRole, poOrganization, poParamsChannelUser);
	
	m_poStats->SetUser(poUser);


	delete [] poUser;
	delete [] poRole;
	delete [] poParamsChannelUser;
	


	if (M4_SUCCESS != ret)
	{

		return M4_ERROR;
	}


	return M4_SUCCESS;



  //## end ClSecurity30::ChangeSecurity%900415034.body
}

m4return_t ClSecurity30::ReleaseConn ()
{
  //## begin ClSecurity30::ReleaseConn%900489435.body preserve=yes

		m4bool_t IsAnyProblem=M4_FALSE;

		//Liberamos el canal de usuarios que nos 
		//habían asignado
		if (m_poUSSesion!=NULL)
		{
			if (m_poChannelUser != NULL)
			{
				if (m_poAccessUSer != NULL)
				{

					m_poAccessUSer->Destroy();
	
					m_poAccessUSer=NULL;
				}
				
				ClChannel *poChannelUser=m_poChannelManager->GetSessionChannel();

				//Si el canal sesión que tiene asignado la máquina virtual es diferente
				//al que tenía inicialmente por defecto significa que tenemos que restaurar 
				//el estado que tenía por defecto la máquina virtual
				if (m_poChannelCredential == poChannelUser)
				{
					m_poChannelManager->SetSessionChannel(m_poChannelUser);

					//Liberamos la asociación que existía entre el canal sesión
					//con la seguridad establecida por el usuario
					if (m_poChannelCredential->Detach() != M4_SUCCESS)
					{
						SETCODEF ( M4_SRV_SESSION_CHANNEL , ERRORLOG , "#*s1*#") ;

						IsAnyProblem=M4_FALSE;
					}
			
					//Establecemos la asociación entre el canal sesión
					//con permisos de usuarios
					if (m_poChannelUser->Attach(m_poChannelManager) != M4_SUCCESS)
					{
						SETCODEF ( M4_SRV_SESSION_CHANNEL , ERRORLOG , "#*s1*#") ;

						IsAnyProblem=M4_FALSE;
					}
			
				}

				m_poChannelUser=NULL;

				m_poChannelCredential=NULL;
			}
			
			NotifyLogoff(M4_FALSE);

			//Cerramos la sesión que establecimos
			m_poLogon->CloseSession(m_poUSSesion,M4_FALSE);

			m_poUSSesion=NULL;
		}

		if (M4_TRUE == IsAnyProblem)
		{
			return M4_ERROR;
		}

		return M4_SUCCESS;

  //## end ClSecurity30::ReleaseConn%900489435.body
}

m4return_t ClSecurity30::ReleaseResources ()
{
  //## begin ClSecurity30::ReleaseResources%942231906.body preserve=yes

		m4return_t ret=M4_SUCCESS;

	 if (NULL == m_poUSSesion)
	 {
		return M4_ERROR;
	 }
	
			ClChannel		*poSessionChannel;
			ClUSSessionTicketInterface * poTicket;

	//Dado la sesión obtenemos el ticket 
	//asociado a él
	m_poUSSesion->GetSessionTicket(poTicket);

	//A partir del ticket obtenemos el canal sesión
	poSessionChannel = (ClChannel *)poTicket->GetTicketCode();

	if (NULL == poSessionChannel)
	{
		return M4_ERROR;
	}

	//A partir del canal sesión podemos obtener
	//la máquina virtual asocido a él
	ClChannelManager *poChannelManager = poSessionChannel->GetpChannelManager();
	
	if (poChannelManager == NULL)
	{
		return M4_ERROR;
	}
	
	//Y por último el entorno de ejecución
	//de la máquina virtual
	ClVMBaseEnv *poEnv = poChannelManager->GetEnvironment();

	if (NULL == poEnv )
	{
			return M4_ERROR;
	}
			
	//Liberamos la conexión lógica
	ret = m_poObjService->GetpConnProvider()->SetConnection(NULL,M4_TRUE);

	if (M4_SUCCESS !=  ret)
	{
		return M4_ERROR;
	}
	
	return poSessionChannel->DetachAndLeaveChannelManager();

  //## end ClSecurity30::ReleaseResources%942231906.body
}

m4void_t ClSecurity30::NotifyLogon (m4objid_t ai_liSessionId)
{
  //## begin ClSecurity30::NotifyLogon%1079025394.body preserve=yes

	// we set the thread variable to indicate the current sessiond id
	// later LDB will use it to free dynamic connections
	m4objid_t *pidSessionId = new m4objid_t;
	*pidSessionId = ai_liSessionId;

	M4Thread::AddProperty(M4_THREADPROP_SESSION_ID,pidSessionId);

  //## end ClSecurity30::NotifyLogon%1079025394.body
}

m4void_t ClSecurity30::NotifyLogoff (m4bool_t ai_bDoFree)
{
  //## begin ClSecurity30::NotifyLogoff%1079025395.body preserve=yes

	// we notify LDB to destroy all physical connections related to this session

	m4objid_t *pidSessionId = (m4objid_t *) M4Thread::GetProperty(M4_THREADPROP_SESSION_ID);
	M4Thread::AddProperty(M4_THREADPROP_SESSION_ID,NULL);
	if (pidSessionId != NULL) {
		
		// gets a valid LDB instance
		ILdb *pLDB = ClFactM4ObjService::GetpLDB();
		if (pLDB != NULL) {
			
			// so we have a valid session id. Notify LDB
			pLDB->UserSessionClosedByIdSession(*pidSessionId);
		}
	}

	if (ai_bDoFree == M4_TRUE) {
		ClIdGenerator::SetNewId ((m4uint32_t) *pidSessionId);
	}

	// frees memory
	delete pidSessionId;
  //## end ClSecurity30::NotifyLogoff%1079025395.body
}

// Additional Declarations
  //## begin ClSecurity30%35AB149F015D.declarations preserve=yes
  //## end ClSecurity30%35AB149F015D.declarations

//## begin module%35AB212D03C2.epilog preserve=yes
//## end module%35AB212D03C2.epilog
