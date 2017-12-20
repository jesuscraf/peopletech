//## begin module%355071C3029F.cm preserve=no
//## end module%355071C3029F.cm

//## begin module%355071C3029F.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            UUSS
//	 File:              USService.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:				980506
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
//## end module%355071C3029F.cp

//## Module: USService%355071C3029F; Package body
//## Subsystem: M4UserInterface::src%3795DCBF0109
//## Source file: D:\Work\v600\m4userinterface\src\usservice.cpp

//## begin module%355071C3029F.additionalIncludes preserve=no
//## end module%355071C3029F.additionalIncludes

//## begin module%355071C3029F.includes preserve=yes
#include <boomrequest.hpp>
#include <commandrequest.hpp>
#include <clarraylibres.hpp>
#include <connects.hpp>
#include <usinterface.hpp>
//## end module%355071C3029F.includes

// USService
#include <usservice.hpp>
// FIFOQ
#include <fifoq.hpp>
// ServiceQ
#include <serviceq.hpp>

//## begin module%355071C3029F.declarations preserve=no
//## end module%355071C3029F.declarations

//## begin module%355071C3029F.additionalDeclarations preserve=yes
#define	MAX_ARG_LEN 8192
//## end module%355071C3029F.additionalDeclarations


// Class ClUSServiceAPI 



//## begin ClUSServiceAPI::m_pInstance%3550845D0148.role preserve=no  protected: static ClUSServiceAPI {1 -> 1RHAN}
ClUSServiceAPI *ClUSServiceAPI::m_pInstance = NULL;
//## end ClUSServiceAPI::m_pInstance%3550845D0148.role


ClUSServiceAPI::ClUSServiceAPI ()
  //## begin ClUSServiceAPI::ClUSServiceAPI%894446844.hasinit preserve=no
      : m_pServerSession(NULL), m_pUSQ(NULL)
  //## end ClUSServiceAPI::ClUSServiceAPI%894446844.hasinit
  //## begin ClUSServiceAPI::ClUSServiceAPI%894446844.initialization preserve=yes
  //## end ClUSServiceAPI::ClUSServiceAPI%894446844.initialization
{
  //## begin ClUSServiceAPI::ClUSServiceAPI%894446844.body preserve=yes
  //## end ClUSServiceAPI::ClUSServiceAPI%894446844.body
}


ClUSServiceAPI::~ClUSServiceAPI ()
{
  //## begin ClUSServiceAPI::~ClUSServiceAPI%894446845.body preserve=yes
  //## end ClUSServiceAPI::~ClUSServiceAPI%894446845.body
}



//## Other Operations (implementation)
m4return_t ClUSServiceAPI::CreateUserSession (const m4pchar_t ai_pszUsername, const m4pchar_t ai_pszPassword, ClConnInstance *ai_pConnection, m4handle_t &ao_hRequest, const m4bool_t ai_bDefRole, m4pcchar_t ai_pcLanguage, m4pcchar_t ai_pcOrganization, const m4bool_t ai_bUse2Way, m4pcchar_t ai_pccClientMachine, m4pcchar_t ai_pcLogonOrigin, m4pcchar_t ai_pcSystemInfo, m4pcchar_t ai_pcClientType, m4pcchar_t ai_pcClientPubIP, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ)
{
  //## begin ClUSServiceAPI::CreateUserSession%894446846.body preserve=yes
//	START CONTRACT CHECK
	if (NULL == ai_pszPassword)
		return M4_ERROR;
//	END CONTRACT CHECK

	//bug 100682.
	if (ai_poLog == NULL){
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest	*newReq = new ClCommandRequest(ao_hRequest, ai_poLog);
	m4return_t			iRet;
	m4char_t			pszArg[MAX_ARG_LEN];
	m4pchar_t			pszPwdArg;
	m4uint32_t			uiArgSize;

	iRet = newReq -> SetCommand(M4_US_AUTHENTICATE);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	1st Argument	USERNAME
	if (NULL == ai_pszUsername)
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}
	else
	{
		uiArgSize = strlen(ai_pszUsername);
		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN -1;

		memcpy ( pszArg, ai_pszUsername, uiArgSize );
		pszArg[uiArgSize++] = '\0';

		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}

	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	2nd Argument	USERPWD
	uiArgSize = strlen(ai_pszPassword);

	pszPwdArg = new m4char_t[uiArgSize+1];

	memcpy ( pszPwdArg, ai_pszPassword, uiArgSize );
	pszPwdArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszPwdArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;		
		return iRet;
	}

	//	3rd Argument	SESSIONID
	m4handle_t		sessionId = ao_hRequest;
	sprintf(pszArg, M4_PRIdPTR, sessionId);

	uiArgSize = strlen(pszArg);
	if (MAX_ARG_LEN <= uiArgSize++)
		uiArgSize = MAX_ARG_LEN -1;

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;		
		return iRet;
	}

	//	4th Argument	CONNECTION
	m4handle_t	adrOfConnectionObj = (m4handle_t)((ClConnection*)ai_pConnection);
	sprintf(pszArg, M4_PRIdPTR, adrOfConnectionObj);

	uiArgSize = strlen(pszArg);
	if (MAX_ARG_LEN <= uiArgSize++)
		uiArgSize = MAX_ARG_LEN -1;

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}

//	ADD EMN 990119 Default Role
	//	5th Argument	DEFROLE
	uiArgSize = 1;
	pszArg[0] = 0;
	if (M4_TRUE == ai_bDefRole)
	{
		pszArg[0] = 1;
	}
	pszArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}

// ADD EFV 010612 Language
	//	6th Argument	Language
	if (ai_pcLanguage != NULL)
	{
		uiArgSize = strlen(ai_pcLanguage);

		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN - 1;

		memcpy(pszArg,ai_pcLanguage,uiArgSize);
		pszArg[uiArgSize++] = '\0';
		
		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}
	else
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}

	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}
	
// ADD OSCARG 010612 Organization
	//	7th Argument	Organization
	if (ai_pcOrganization != NULL)
	{
		uiArgSize = strlen(ai_pcOrganization);

		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN - 1;

		memcpy(pszArg,ai_pcOrganization,uiArgSize);
		pszArg[uiArgSize++] = '\0';
		
		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}
	else
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}

	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}
	
// ADD EFV 0085746
	//	8th Argument	Use2Way
	uiArgSize = 1;
	pszArg[0] = 0;
	if (M4_TRUE == ai_bUse2Way)
	{
		pszArg[0] = 1;
	}
	pszArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}

	//	9th Argument	ClientMachine
	if (ai_pccClientMachine != NULL)
	{
		uiArgSize = strlen(ai_pccClientMachine);

		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN - 1;

		memcpy(pszArg,ai_pccClientMachine,uiArgSize);
		pszArg[uiArgSize++] = '\0';
		
		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}
	else
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}

		if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}


	//	10th Argument	LogonOrigin luciag
	if (ai_pcLogonOrigin != NULL)
	{
		uiArgSize = strlen(ai_pcLogonOrigin);

		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN - 1;

		memcpy(pszArg,ai_pcLogonOrigin,uiArgSize);
		pszArg[uiArgSize++] = '\0';
		
		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}
	else
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}


//	11th Argument	SystemInfo
	if (ai_pcSystemInfo != NULL)
	{
		uiArgSize = strlen(ai_pcSystemInfo);

		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN - 1;

		memcpy(pszArg,ai_pcSystemInfo,uiArgSize);
		pszArg[uiArgSize++] = '\0';
		
		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}
	else
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}

	//	12th Argument	ClientType
	if (ai_pcClientType != NULL)
	{
		uiArgSize = strlen(ai_pcClientType);

		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN - 1;

		memcpy(pszArg,ai_pcClientType,uiArgSize);
		pszArg[uiArgSize++] = '\0';
		
		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}
	else
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}

	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}

	//	13th Argument	ClientPubIP
	if (ai_pcClientPubIP != NULL)
	{
		uiArgSize = strlen(ai_pcClientPubIP);

		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN - 1;

		memcpy(pszArg,ai_pcClientPubIP,uiArgSize);
		pszArg[uiArgSize++] = '\0';
		
		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}
	else
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}

	if (M4_SUCCESS != iRet)
	{
		delete[] pszPwdArg;
		delete newReq;
		return iRet;
	}

	/* Bug 0088304
	Para que se generen los mensajes de error de login en nuestra pila
	*/
	//bug 100682. Ya se ha asignado al crear newReq si la log que viene es nula.
	//newReq->SetLog( GETPTHREADLOGSYSTEM() ) ; 	

	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);
//	ADD	EMN	980324
	boomReq->SetRequestId(ao_hRequest);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ){
		delete[] pszPwdArg;
	//	Return control to the caller
		return M4_RET_WAIT;
	}

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

	delete[] pszPwdArg;

//	DEL	EMN	980810	delete newReq;
	delete boomReq;

	if (M4_SUCCESS == opRet)
	{
		ClUSSessionInterface *usSession = (ClUSSessionInterface *)sessionHandle;
		if ( NULL == usSession )
			return M4_ERROR;
		ao_hRequest = sessionHandle;
	}


	return opRet;	
  //## end ClUSServiceAPI::CreateUserSession%894446846.body
}

m4return_t ClUSServiceAPI::CreateUserSession (const m4pchar_t ai_pszUsername, ClConnInstance *ai_pConnection, m4handle_t &ao_hRequest, ClControlQ *ai_pSourceQ)
{
  //## begin ClUSServiceAPI::CreateUserSession%899809128.body preserve=yes
//	START CONTRACT CHECK
	if (NULL == ai_pszUsername)
		return M4_ERROR;
//	END CONTRACT CHECK

	//	Build Request
	ClCommandRequest	*newReq = new ClCommandRequest(0,GETPTHREADLOGSYSTEM()); //bug 100682	

	m4return_t			iRet;

	iRet = newReq -> SetCommand(M4_US_AUTHENTICATE);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	m4char_t		pszArg[MAX_ARG_LEN];
	//	1st Argument	USERNAME
	m4uint32_t		uiArgSize = strlen(ai_pszUsername);
	if (MAX_ARG_LEN <= uiArgSize)
		uiArgSize = MAX_ARG_LEN -1;

	memcpy ( pszArg, ai_pszUsername, uiArgSize );
	pszArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	2nd Argument	USERPWD
	m4uint32_t		sessionId = ClIdGenerator::GetNewId();
	sprintf(pszArg, "%d", sessionId);

	uiArgSize = strlen(pszArg);
	if (MAX_ARG_LEN <= uiArgSize++)
		uiArgSize = MAX_ARG_LEN -1;

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	3rd Argument	SESSIONID
	m4handle_t		adrOfConnectionObj = (m4handle_t)ai_pConnection;
	sprintf(pszArg, M4_PRIdPTR, adrOfConnectionObj);

	uiArgSize = strlen(pszArg);
	if (MAX_ARG_LEN <= uiArgSize++)
		uiArgSize = MAX_ARG_LEN -1;

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	/* Bug 0088304
	Para que se generen los mensajes de error de login en nuestra pila
	*/
	//bug 100682. Ya se ha asignado al crear newReq.
	//newReq->SetLog( GETPTHREADLOGSYSTEM() ) ;

	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);
//	ADD	EMN	980324
	boomReq->SetRequestId(ao_hRequest);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

//	DEL	EMN	980810	delete newReq;
	delete boomReq;
	
	if (M4_SUCCESS == opRet)
	{
		ClUSSessionInterface *usSession = (ClUSSessionInterface *)sessionHandle;
		if ( NULL == usSession )
			return M4_ERROR;
		ao_hRequest = sessionHandle;
	}

	return opRet;	
  //## end ClUSServiceAPI::CreateUserSession%899809128.body
}

m4return_t ClUSServiceAPI::CheckRole (ClUSSessionInterface *ai_pSession, const m4pchar_t ai_pszRoleId, m4handle_t &ao_hRequest, m4bool_t ai_bDefault, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ)
{
  //## begin ClUSServiceAPI::CheckRole%895825632.body preserve=yes
//	START CONTRACT CHECK
	if (NULL == ai_pSession)
		return M4_ERROR;
//	END CONTRACT CHECK

	//bug 100682
	if (ai_poLog == NULL){
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest		*newReq = new ClCommandRequest(ao_hRequest, ai_poLog);
	m4return_t		iRet;

	iRet = newReq -> SetCommand(M4_US_CHECK_ROLE);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	m4char_t		pszArg[MAX_ARG_LEN];
	//	1st Argument
	if (NULL != ai_pszRoleId)
	{
		m4uint32_t		uiArgSize = strlen(ai_pszRoleId);
		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN -1;

		memcpy ( pszArg, ai_pszRoleId, uiArgSize );
		pszArg[uiArgSize++] = '\0';

		iRet = newReq -> AddArg( pszArg, uiArgSize );
		if (M4_SUCCESS != iRet)
		{
			delete newReq;
			return iRet;
		}

		uiArgSize = 1;
		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN -1;

		pszArg[0] = 0;
		if (M4_TRUE == ai_bDefault)
		{
			pszArg[0] = 1;
		}
		pszArg[uiArgSize++] = '\0';

		iRet = newReq -> AddArg( pszArg, uiArgSize );
		if (M4_SUCCESS != iRet)
		{
			delete newReq;
			return iRet;
		}
	}

//	ADD	EMN	980528
	m4handle_t	hObject = (m4handle_t)ai_pSession;

	newReq -> SetHandle(hObject);

	/* Bug 0088304
	Para que se generen los mensajes de error de login en nuestra pila
	*/
	//bug 100682. Ya se ha asignado al crear newReq si la log que viene es nula.
	//newReq->SetLog( GETPTHREADLOGSYSTEM() ) ;	

	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	ao_hRequest = (m4handle_t)boomReq;
	boomReq->SetRequest(newReq);
//	ADD	EMN	980324
	boomReq->SetRequestId((m4handle_t)ai_pSession);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

//	DEL	EMN	980810	delete newReq;
	delete boomReq;

	if (M4_SUCCESS == opRet)
	{
		ClUSSessionInterface *usSession = (ClUSSessionInterface *)sessionHandle;
		if ( NULL == usSession )
			return M4_ERROR;
		ao_hRequest = sessionHandle;
	}

	return opRet;	
  //## end ClUSServiceAPI::CheckRole%895825632.body
}

m4return_t ClUSServiceAPI::ChangePassword (ClUSSessionInterface *ai_pSession, const m4pchar_t &ai_pszOldPassword, const m4pchar_t &ai_pszNewPassword, m4handle_t &ao_hRequest, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ)
{
  //## begin ClUSServiceAPI::ChangePassword%894446848.body preserve=yes
//	START CONTRACT CHECK
	if ((NULL == ai_pSession) || (NULL == ai_pszOldPassword) || (NULL == ai_pszNewPassword))
		return M4_ERROR;
//	END CONTRACT CHECK

	if (NULL == ai_pSession)
		return M4_ERROR;

	//bg 100682
	if (ai_poLog == NULL){
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest		*newReq = new ClCommandRequest(ao_hRequest, ai_poLog);
	m4return_t		iRet;

	iRet = newReq -> SetCommand(M4_US_CHANGE_PWD);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}


	m4char_t		pszArg[MAX_ARG_LEN];
	//	1st Argument
	m4uint32_t		uiArgSize = strlen(ai_pszOldPassword);
	if (MAX_ARG_LEN <= uiArgSize)
		uiArgSize = MAX_ARG_LEN -1;

	memcpy ( pszArg, ai_pszOldPassword, uiArgSize );
	pszArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}
	//	2nd Argument
	uiArgSize = strlen(ai_pszNewPassword);
	if (MAX_ARG_LEN <= uiArgSize)
		uiArgSize = MAX_ARG_LEN -1;

	memcpy ( pszArg, ai_pszNewPassword, uiArgSize );
	pszArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

//	ADD	EMN	980528
	m4handle_t	hObject = (m4handle_t)ai_pSession;
	newReq -> SetHandle(hObject);

	/* Bug 0088304
	Para que se generen los mensajes de error de login en nuestra pila
	*/
	//bug 100682. Ya se ha asignado al crear newReq si la log que viene es nula.
	//newReq->SetLog( GETPTHREADLOGSYSTEM() ) ;	

	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);
//	ADD	EMN	980324
	boomReq->SetRequestId((m4handle_t)ai_pSession);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

//	DEL	EMN	980810	delete newReq;
	delete boomReq;

	if (M4_SUCCESS == opRet)
	{
		ClUSSessionInterface *usSession = (ClUSSessionInterface *)sessionHandle;
		if ( NULL == usSession )
			return M4_ERROR;
		ao_hRequest = sessionHandle;
	}


	return opRet;	
  //## end ClUSServiceAPI::ChangePassword%894446848.body
}

ClUSServiceAPI * ClUSServiceAPI::Instance ()
{
  //## begin ClUSServiceAPI::Instance%894446849.body preserve=yes
	if (NULL == m_pInstance)
	{
		m_pInstance = new ClUSServiceAPI();
	}

	return m_pInstance;
  //## end ClUSServiceAPI::Instance%894446849.body
}

m4return_t ClUSServiceAPI::Initialize (ClServiceQ *ai_pUUSSQ)
{
  //## begin ClUSServiceAPI::Initialize%894446850.body preserve=yes
	if (NULL == m_pUSQ)
		m_pUSQ = ai_pUUSSQ;
	else
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClUSServiceAPI::Initialize%894446850.body
}

// Additional Declarations
  //## begin ClUSServiceAPI%35507A7001B5.declarations preserve=yes
m4return_t ClUSServiceAPI::GenerateCertificate (ClUSSessionInterface *ai_pSession , m4pchar_t ai_szUserName , m4handle_t &ao_hRequest , ClLogSystem *ai_poLog , ClControlQ *ai_pSourceQ ) 
{
	if (NULL == ai_szUserName)
		return M4_ERROR;
//	END CONTRACT CHECK

	//bg 100682
	if (ai_poLog == NULL){
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest	*newReq = new ClCommandRequest(ao_hRequest, ai_poLog);
	m4return_t			iRet;
	m4char_t			pszArg[MAX_ARG_LEN];
	m4uint32_t			uiArgSize;

	iRet = newReq -> SetCommand(M4_US_GET_CERT);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	m4handle_t	hObject = (m4handle_t)ai_pSession;

	newReq -> SetHandle(hObject);

	//	1nd Argument	USERNAME
	uiArgSize = strlen ( ai_szUserName ) ; 
	if (MAX_ARG_LEN <= uiArgSize)
		uiArgSize = MAX_ARG_LEN -1;

	memcpy ( pszArg, ai_szUserName , uiArgSize );
	pszArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	/* Bug 0088304
	Para que se generen los mensajes de error de login en nuestra pila
	*/
	//bug 100682. Ya se ha asignado al crear newReq si la log que viene es nula.
	//newReq->SetLog( GETPTHREADLOGSYSTEM() ) ;
	
	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);
//	ADD	EMN	980324
	boomReq->SetRequestId(ao_hRequest);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

//	DEL	EMN	980810	delete newReq;
	delete boomReq;

	if (M4_SUCCESS == opRet)
	{
		ClUSSessionInterface *usSession = (ClUSSessionInterface *)sessionHandle;
		if ( NULL == usSession )
			return M4_ERROR;
		ao_hRequest = sessionHandle;
	}


	return opRet;	
}


m4return_t ClUSServiceAPI::CreateUserSession (m4string_t  ai_iCertificateType , const m4pchar_t ai_pszCertificate, m4uint32_t ai_iCertificateLen, m4pchar_t ai_pszSessContext, m4pcchar_t ai_pccClientMachine, m4pcchar_t ai_pccSystemInfo, m4pcchar_t ai_pccClientType, ClConnInstance *ai_pConnection, m4handle_t &ao_hRequest, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ)
{
//	START CONTRACT CHECK
	if (NULL == ai_pszCertificate)
		return M4_ERROR;
//	END CONTRACT CHECK

	//bg 100682
	if (ai_poLog == NULL){
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest	*newReq = new ClCommandRequest(ao_hRequest, ai_poLog);
	m4return_t			iRet;
	m4char_t			pszArg[MAX_ARG_LEN];
	m4uint32_t			uiArgSize;

	iRet = newReq -> SetCommand(M4_US_CERT_LOGIN);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	1st Argument	CERT_TYPE
	memcpy ( pszArg, ai_iCertificateType.c_str(), ai_iCertificateType.size() + 1);

	uiArgSize = strlen(pszArg);
	if (MAX_ARG_LEN <= uiArgSize++)
		uiArgSize = MAX_ARG_LEN -1;

	iRet = newReq -> AddArg( pszArg , uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}


	//	2nd Argument	CERTIFICATE
	uiArgSize = ai_iCertificateLen ; 
	if (MAX_ARG_LEN <= uiArgSize)
		uiArgSize = MAX_ARG_LEN -1;

	memcpy ( pszArg, ai_pszCertificate, uiArgSize );
	pszArg[uiArgSize++] = '\0';

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	3nd Argument	SESSCONTEXT
	if (NULL == ai_pszSessContext)
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}
	else
	{
		uiArgSize = strlen(ai_pszSessContext);
		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN -1;

		memcpy ( pszArg, ai_pszSessContext, uiArgSize );
		pszArg[uiArgSize++] = '\0';

		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}

	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	4rd Argument	CERTIFICATE_LENGTH
	sprintf(pszArg, "%d", ai_iCertificateLen );

	uiArgSize = strlen(pszArg);
	if (MAX_ARG_LEN <= uiArgSize++)
		uiArgSize = MAX_ARG_LEN -1;

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	5th Argument	CONNECTION
	m4uint32_t		adrOfConnectionObj = (m4uint32_t)( (ClConnection*)ai_pConnection);
	sprintf(pszArg, "%d", adrOfConnectionObj);

	uiArgSize = strlen(pszArg);
	if (MAX_ARG_LEN <= uiArgSize++)
		uiArgSize = MAX_ARG_LEN -1;

	iRet = newReq -> AddArg( pszArg, uiArgSize );
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	6th Argument	ClientMachine
	if (NULL == ai_pccClientMachine)
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}
	else
	{
		uiArgSize = strlen(ai_pccClientMachine);
		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN -1;

		memcpy ( pszArg, ai_pccClientMachine, uiArgSize );
		pszArg[uiArgSize++] = '\0';

		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}

	//	7th Argument	SystemInfo
	if (NULL == ai_pccSystemInfo)
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}
	else
	{
		uiArgSize = strlen(ai_pccSystemInfo);
		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN -1;

		memcpy ( pszArg, ai_pccSystemInfo, uiArgSize );
		pszArg[uiArgSize++] = '\0';

		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}

	//	8th Argument	ClientType
	if (NULL == ai_pccClientType)
	{
		iRet = newReq -> AddArg( NULL, 0 );
	}
	else
	{
		uiArgSize = strlen(ai_pccClientType);
		if (MAX_ARG_LEN <= uiArgSize)
			uiArgSize = MAX_ARG_LEN -1;

		memcpy ( pszArg, ai_pccClientType, uiArgSize );
		pszArg[uiArgSize++] = '\0';

		iRet = newReq -> AddArg( pszArg, uiArgSize );
	}

	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}


	/* Bug 0088304
	Para que se generen los mensajes de error de login en nuestra pila
	*/
	//bug 100682. Ya se ha asignado al crear newReq si la log que viene es nula.
	//newReq->SetLog( GETPTHREADLOGSYSTEM() ) ;
	
	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);
//	ADD	EMN	980324
	boomReq->SetRequestId(ao_hRequest);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

//	DEL	EMN	980810	delete newReq;
	delete boomReq;

	if (M4_SUCCESS == opRet)
	{
		ClUSSessionInterface *usSession = (ClUSSessionInterface *)sessionHandle;
		if ( NULL == usSession )
			return M4_ERROR;
		ao_hRequest = sessionHandle;
	}


	return opRet;	
}


m4return_t ClUSServiceAPI::CloseAuditory( ClUSSessionInterface *ai_pSession, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ )
{
//	START CONTRACT CHECK
	if (NULL == ai_pSession)
		return M4_ERROR;
//	END CONTRACT CHECK

	if (ai_poLog == NULL)
	{
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest	*newReq = new ClCommandRequest(0, ai_poLog);
	m4return_t			iRet;

	iRet = newReq -> SetCommand(M4_US_CLOSE_AUDITORY);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	m4handle_t	hObject = (m4handle_t)ai_pSession;

	newReq -> SetHandle(hObject);

	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);

	boomReq->SetRequestId(0);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

	delete boomReq;

	return opRet;	
}

m4return_t ClUSServiceAPI::CleanAuditory( ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ )
{
	if (ai_poLog == NULL)
	{
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest	*newReq = new ClCommandRequest(0, ai_poLog);
	m4return_t			iRet;

	iRet = newReq -> SetCommand(M4_US_CLEAN_AUDITORY);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);

	boomReq->SetRequestId(0);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4return_t		opRet = newReq->GetResult();

	delete boomReq;

	return opRet;	
}


// User actions auditory.
m4return_t ClUSServiceAPI::UserAuditoryLogoffAction( ClUSSessionInterface *ai_pSession, ClLogSystem *ai_poLog, ClControlQ *ai_pSourceQ )
{
//	START CONTRACT CHECK
	if (NULL == ai_pSession)
		return M4_ERROR;
//	END CONTRACT CHECK

	if (ai_poLog == NULL)
	{
		ai_poLog = GETPTHREADLOGSYSTEM(); 
	}

	//	Build Request
	ClCommandRequest	*newReq = new ClCommandRequest(0, ai_poLog);
	m4return_t			iRet;

	iRet = newReq -> SetCommand(M4_US_USR_AUDIT_LOGOFF);
	if (M4_SUCCESS != iRet)
	{
		delete newReq;
		return iRet;
	}

	m4handle_t	hObject = (m4handle_t)ai_pSession;

	newReq -> SetHandle(hObject);

	//	Send the request
	ClBoomerangRequest	*boomReq = new ClBoomerangRequest();
	boomReq->SetRequest(newReq);

	boomReq->SetRequestId(0);
	boomReq->Throw(m_pUSQ, ai_pSourceQ);

	// bg 179832
	SETLOGSYSTEM(ai_poLog);

	if ( NULL != ai_pSourceQ)
	//	Return control to the caller
		return M4_RET_WAIT;

	ClBaseRequest	*retReq;
	boomReq->GetRequest(retReq);

	m4handle_t		sessionHandle = newReq->GetHandle();
	m4return_t		opRet = newReq->GetResult();

	delete boomReq;

	return opRet;	
}


  //## end ClUSServiceAPI%35507A7001B5.declarations
//## begin module%355071C3029F.epilog preserve=yes
//## end module%355071C3029F.epilog
