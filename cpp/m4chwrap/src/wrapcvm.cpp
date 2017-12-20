//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           
// File:             wrapcvm.cpp
// Project:          Channel Wrapper
// Author:           Meta Software M.S. , S.A
// Date:             06/10/97
// Language:         C++
// Operating System: WINDOWS
// Design Document:  
//
// Definition:
//     Implementation of CWrapCVM
//
//==============================================================================


#include "wores.hpp"
#include "stdafx.h"
#include "m4chwrap.h"
#include "wrapcvm.h"
#include "wrapcolprop.h"
#include "wrapchannel.h"
#include "wrapaccess.h"
#include "wrapnode.h"
#include "wrapproxylistit.h"
#include "newinterface.h"
#include "wrapdef.h"
#include "rerror.h"
#include "cvdate.h"
#include "presfac.hpp"
#include "clpresen.hpp"
#include "cllgadap.hpp"
#include "m4ldb.hpp"
#include "wrapvmsign.h"
#include "wrapbookmark.h"
#include "cacheitfz.hpp"
#include "cachedir.hpp"
#include "wrapexecutor.h"
#include "executor.hpp"
#include "clfactm4objservice.hpp"
#include "m4objreg.hpp"
#include "channel.hpp"
#include "m4objglb.hpp"
#include "access.hpp"
#include "clm4objservicecontext.hpp"
#include "cldmcontext.hpp"
#include "genproxy.hpp"
#include "m4cstrace.hpp"
#include "m4hsparser.hpp"	
#include "checkmem.hpp"
#include "file_manager.hpp"
#include "clm4objservice.hpp"
#include "clappclientadap.hpp"
#include "licensemanagerinterface.hpp"
#include "m4mdfac.hpp"
#include "cldbgutil.hpp"
#include "m4unicode.hpp"
#include "m4buildversion.hpp"
#include "m4unicode.hpp"
#include "m4ini.hpp"
#include "m4xmlreg.hpp"
#include "usractaudit.hpp"
#include "m4loadll.hpp"
#include "replaceliterals.hpp"
#include "clextman.hpp"
#include "m4hook.h"
#include "cacherep.hpp"
#include "m4stl.hpp"

extern ICPropBag* ConstructPBFromASN1(unsigned char * tmpBuffer, const char* ai_pccOrganization, ClChannelManager *ai_poChannelManager) ;


/* Bug 0265241
La engine no se debe descargar nunca.
Se pasa a una variable estática que se carga la primera vez que se usa.
*/
M4ClLoaDll	*g_poEngine = NULL ;

M4ClLoaDll*	GetEngine( void )
{
	if( g_poEngine == NULL )
	{
		M4ClExternObjectManager::RunCallBack( "engindll" ) ;

		g_poEngine = new M4ClLoaDll() ;

		if( g_poEngine->Open( "engindll" ) != M4_SUCCESS )
		{
			delete( g_poEngine ) ;
			g_poEngine = NULL ;
		}
	}

	return( g_poEngine ) ;
}



/////////////////////////////////////////////////////////////////////////////
// Manejo de excepciones

// Definiciones no necesarias en VS2015
typedef LONG (NTAPI *PVECTORED_EXCEPTION_HANDLER)(struct _EXCEPTION_POINTERS *ExceptionInfo);

extern "C" PVOID WINAPI AddVectoredExceptionHandler(ULONG First, PVECTORED_EXCEPTION_HANDLER Handler);
extern "C" ULONG WINAPI RemoveVectoredExceptionHandler(PVOID Handler);


static	m4int32_t							s_iExceptionDepth = 0;
static	PVOID								s_hExceptionHandler = NULL;
static	set<CWrapCVM*, less<CWrapCVM*> >	s_oWrapCVMSet;


LONG WINAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
{

	m4char_t			acDumpExe[ MAX_PATH + 1 ] ;
	m4char_t			acDumpFile[ MAX_PATH + 1 ] ;
	m4char_t			acCommand[ 2048 + 1 ] ;
	BOOL				bResult = FALSE ;
	DWORD				dwResult = 1 ;
	FILE*				pfFile = NULL ;
	SYSTEMTIME			st ;
    STARTUPINFO			si ;
    PROCESS_INFORMATION	pi ;

	set<CWrapCVM*, less<CWrapCVM*> >::iterator	it ;


	switch( pExceptionInfo->ExceptionRecord->ExceptionCode )
	{
		case 0xc0000005:	// Access Violation
		case 0xc00000fd:	// Stack Overflow

			dwResult = GetModuleFileName( NULL, acDumpExe, MAX_PATH ) ;

			if( dwResult > 0 )
			{
				bResult = PathRemoveFileSpec( acDumpExe ) ;

				if( bResult = TRUE )
				{
					strcat( acDumpExe, "\\procdump.exe" ) ;

					pfFile = fopen( acDumpExe, "rb" ) ;

					if( pfFile != NULL )
					{
						// Sólo se maneja el error si está presente el procdump
						fclose( pfFile ) ;

						GetSystemTime( &st ) ;
						GetTempPath( MAX_PATH, acDumpFile ) ;

						sprintf( acDumpFile + strlen( acDumpFile ), "M4PeopleNet_%04d%02d%02d_%02d%02d%02d_%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds ) ;
						sprintf( acCommand, "%s -accepteula -ma %d %s", acDumpExe, GetCurrentProcessId(), acDumpFile ) ;
						
						ZeroMemory( &si, sizeof(si) ) ;
						si.cb = sizeof(si) ;
						ZeroMemory( &pi, sizeof(pi) ) ;

						if( CreateProcess( NULL, acCommand, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi ) != 0 )
						{
							WaitForSingleObject( pi.hProcess, INFINITE ) ;
							bResult = GetExitCodeProcess( pi.hProcess, &dwResult ) ;

							CloseHandle( pi.hProcess ) ;

							if( bResult == TRUE && dwResult == 1 )
							{
								it = s_oWrapCVMSet.begin() ;

								while( it != s_oWrapCVMSet.end() )
								{
									(*it)->OnCppException( acDumpFile ) ;
									it++ ;
								}

								// exit( -1 ) ; Cuando nos atrevamos
							}
						}
					}
				}
			}
			break;

//		case 0x40010005:	// Control-C
//		case 0x40010008:	// Control-Break
//		case 0x80000002:	// Datatype Misalignment
//		case 0xc0000006:	// In Page Error
//		case 0xc0000008:	// Invalid Handle
//		case 0xc0000017:	// No Memory
//		case 0xc000001d:	// Illegal Instruction
//		case 0xc0000025:	// Noncontinuable Exception
//		case 0xc0000026:	// Invalid Disposition
//		case 0xc000008c:	// Array Bounds Exceeded
//		case 0xc000008d:	// Float Denormal Operand
//		case 0xc000008e:	// Float Divide by Zero
//		case 0xc000008f:	// Float Inexact Result
//		case 0xc0000090:	// Float Invalid Operation
//		case 0xc0000091:	// Float Overflow
//		case 0xc0000092:	// Float Stack Check
//		case 0xc0000093:	// Float Underflow
//		case 0xc0000094:	// Integer Divide by Zero
//		case 0xc0000095:	// Integer Overflow
//		case 0xc0000096:	// Privileged Instruction
//		case 0xc0000135:	// DLL Not Found
//		case 0xc0000142:	// DLL Initialization Failed
//		case 0xc06d007e:	// Module Not Found
//		case 0xc06d007f:	// Procedure Not Found
//		case 0xe06d7363:	// Microsoft C++ Exception
		default:
			break;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}


void	M4CatchExceptions( void )
{
	if( s_iExceptionDepth == 0 )
	{
		s_hExceptionHandler = AddVectoredExceptionHandler( 1 /*CALL_FIRST*/, VectoredExceptionHandler ) ;
	}
	s_iExceptionDepth++ ;
}


void	M4UncatchExceptions( void )
{
	if( s_iExceptionDepth > 0 )
	{
		s_iExceptionDepth-- ;

		if( s_iExceptionDepth == 0 )
		{
			if( s_hExceptionHandler != NULL )
			{
				RemoveVectoredExceptionHandler( s_hExceptionHandler ) ;
				s_hExceptionHandler = NULL ;
			}
		}
	}
}


void	M4ResetExceptions( void )
{
	if( s_iExceptionDepth > 0 )
	{
		s_iExceptionDepth = 1 ;
		M4UncatchExceptions() ;
	}
}



/////////////////////////////////////////////////////////////////////////////
// CWrapCVM

STDMETHODIMP	CWrapCVM::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = { &IID_IWrapIMap };

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
		{
			return S_OK;
		}
	}
	return S_FALSE;
}


CWrapCVM::CWrapCVM()
{
	m_poDM = NULL;
	m_poExecutor = NULL;
	m_poObjService = NULL;

	s_oWrapCVMSet.insert(this);
	ClVMCSContext::RegisterMessageBoxProvider(this);
	ClSessionPingerEvents::RegisterTimeoutEventProvider(this);
}


CWrapCVM::~CWrapCVM()
{
	s_oWrapCVMSet.erase(this);
	ClVMCSContext::UnregisterMessageBoxProvider(this);
	ClSessionPingerEvents::UnregisterTimeoutEventProvider(this);

	if (m_poExecutor)
	{
        m_poExecutor->Release();
        m_poExecutor = NULL;
    }
	
	if (m_poObjService)
	{
		ClFactM4ObjService::DestroyObjService(m_poObjService);
		m_poObjService = NULL;
	}
	
	m_poDM = NULL;
}


STDMETHODIMP CWrapCVM::get_coProps(IWrapColProp** pVal)
{
	m4return_t Status;
	
	ENTER_METHOD(CWrapCVM::get_coProps);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_coProps, E_UNEXPECTED);
	}
	
	Status = NewInterface(CLSID_WrapColProp, IID_IWrapColProp, (IUnknown**)pVal);
	
	if (Status != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapCVM::get_coProps, E_UNEXPECTED);
	}
	else
	{
		((CWrapColProp*)(*pVal))->Init(&m_poDM->Property);
		
		LEAVE_METHOD(CWrapCVM::get_coProps, S_OK);
	}
}


STDMETHODIMP CWrapCVM::Startup(short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::Startup);
	
	*ao_RetVal =M4_ERROR;
	
	m_poObjService = ClFactM4ObjService::CreateObjService( NULL, NULL, NULL, NULL, NULL, M4_FALSE );
	
	if (m_poObjService){
		*ao_RetVal =M4_SUCCESS;
		m_poDM=m_poObjService->GetpChannelManager();
	}
	
	LEAVE_METHOD(CWrapCVM::Startup, S_OK);
}


STDMETHODIMP CWrapCVM::Shutdown(short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::Shutdown);
	
	if (m_poExecutor)
	{
        m_poExecutor->Release();
        m_poExecutor = 0;
    }
	
	*ao_RetVal = ClFactM4ObjService::DestroyObjService(m_poObjService);
	
	m_poDM=0;
	
	LEAVE_METHOD(CWrapCVM::Shutdown, S_OK);
}


STDMETHODIMP CWrapCVM::CreateChannel(IWrapChannel** ao_channel)
{
	ClChannel* pChannel;
	
	ENTER_METHOD(CWrapCVM::CreateChannel);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CreateChannel, E_UNEXPECTED);
	}
	
	m4return_t RetVal = m_poDM->CreateChannel(pChannel);
	
	if (RetVal == M4_SUCCESS)
	{
		
#ifdef M4_NEWINTERFACE
		RetVal = NewInterface(CLSID_WrapChannel, IID_IWrapChannel, (IUnknown**)ao_channel);
#else
		HRESULT hRes = M4_CREATE_INTERFACE(CWrapChannel,ao_channel);
		RetVal = SUCCEEDED(hRes)? M4_SUCCESS : M4_ERROR;
#endif		
		if (RetVal != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapCVM::CreateChannel, E_UNEXPECTED);
		}
		
		((CWrapChannel*)(*ao_channel))->Init(pChannel, true);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CREATE_CHANNEL, "Could not create channel");
		LEAVE_METHOD(CWrapCVM::CreateChannel, E_UNEXPECTED);
	}
	
	LEAVE_METHOD(CWrapCVM::CreateChannel, S_OK);
}


STDMETHODIMP CWrapCVM::RegisterChannel(IWrapChannel* ai_Channel, short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::RegisterChannel);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::RegisterChannel" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapCVM::RegisterChannel, S_OK);
}


STDMETHODIMP CWrapCVM::AttachDocNodeToTrackNode(IWrapNode* ai_poWrapDocNode
								   , IWrapNode* ai_poWrapTrackNode
								   , short* ao_pRetVal)
{
	ClNode* pDocNode;
	ClNode* pTrackNode;
	
	ENTER_METHOD(CWrapCVM::AttachDocNodeToTrackNode);
	
	if (!m_poDM){
		*ao_pRetVal =M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::AttachDocNodeToTrackNode, S_OK);
	}
	
	//
	// Get a pointer to the DocNode 
	//
	((CWrapNode*)ai_poWrapDocNode)->Unwrap(&pDocNode);
	
	//
	// .. and now to the TrackNode
	//
	((CWrapNode*)ai_poWrapTrackNode)->Unwrap(&pTrackNode);
	
	//
	// Attach this node as document to the node 
	// passed as track
	//
    *ao_pRetVal = m_poDM->LaneManager.ConnectIt(pDocNode, pTrackNode);
	
	LEAVE_METHOD(CWrapNode::AttachDocNodeToTrackNode, S_OK);
}

STDMETHODIMP CWrapCVM::DetachDocNodeToTrackNode(IWrapNode* ai_poWrapDocNode
								   , IWrapNode* ai_poWrapTrackNode
								   , short* ao_pRetVal)
{
	ClNode* pDocNode;
	ClNode* pTrackNode;
	
	ENTER_METHOD(CWrapCVM::DetachDocNodeToTrackNode);
	
	if (!m_poDM){
		*ao_pRetVal =M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::DetachDocNodeToTrackNode, S_OK);
	}
	
	//
	// Get a pointer to the DocNode 
	//
	((CWrapNode*)ai_poWrapDocNode)->Unwrap(&pDocNode);
	
	//
	// .. and now to the TrackNode
	//
	((CWrapNode*)ai_poWrapTrackNode)->Unwrap(&pTrackNode);
	
	//
	// Attach this node as document to the node 
	// passed as track
	//
    *ao_pRetVal = m_poDM->LaneManager.DisConnectIt(pDocNode, pTrackNode);
	
	LEAVE_METHOD(CWrapNode::DetachDocNodeToTrackNode, S_OK);
}

STDMETHODIMP CWrapCVM::Logon
(
	BSTR ai_stUsername,
	BSTR ai_stPassword,
	long ai_iLanguage,
	BSTR ai_stLogonServer,
	long ai_iPortNumber,
	VARIANT_BOOL ai_bUseDefaultRole,
	short* ao_RetVal
	)
{
	ENTER_METHOD( CWrapCVM::Logon ) ;
	
	*ao_RetVal = M4_ERROR ;
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::Logon, S_OK);
	}
	
	gstring	tmpUserName( ai_stUsername ) ;
	gstring	tmpPassword( ai_stPassword ) ;
	gstring	tmpServername( ai_stLogonServer ) ;
	
	if( tmpUserName != NULL && tmpPassword != NULL && tmpServername != NULL )
	{
		*ao_RetVal = m_poDM->GetLogonAdaptor()->Logon
			(
			tmpUserName,
			tmpPassword,
			ai_iLanguage,
			tmpServername,
			ai_iPortNumber,
			( ai_bUseDefaultRole == VARIANT_TRUE ),
			NULL,
			M4_FALSE
			) ;
	}
	else
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Cannot create string" ) ;
	}
	
	LEAVE_METHOD( CWrapCVM::Logon, S_OK ) ;
}


STDMETHODIMP CWrapCVM::Logoff(short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::Logoff);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::Logoff, S_OK);
	}
	
	*ao_RetVal = m_poDM->GetLogonAdaptor()->LogOff();
	
	TRACE2("Returning from logoff %d", *ao_RetVal);
	LEAVE_METHOD(CWrapCVM::Logon, S_OK);
}


STDMETHODIMP CWrapCVM::ChangePassword(BSTR ai_stOldPassword, BSTR ai_stNewPassword, short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::ChangePassword);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::ChangePassword, S_OK);
	}
	
	gstring tmpOldPassword(ai_stOldPassword);
	gstring	tmpNewPassword( ai_stNewPassword ) ;
	
	if( tmpOldPassword != NULL && tmpNewPassword != NULL )
	{
		*ao_RetVal = m_poDM->GetLogonAdaptor()->ChangePassword( tmpOldPassword, tmpNewPassword ) ;
	}
	else
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Cannot create string" ) ;
		*ao_RetVal = -1 ;
	}
	
	LEAVE_METHOD(CWrapCVM::ChangePassword, S_OK);
}


STDMETHODIMP CWrapCVM::get_RoleNameByPos(long ai_iPos, BSTR* pVal)
{
	ENTER_METHOD(CWrapCVM::get_RoleNameByPos);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_RoleNameByPos,E_UNEXPECTED);
	}
	
	TRACE2("Trying to get role with position %d", ai_iPos);
	
	gstring tmpName(m_poDM->GetLogonAdaptor()->GetRoleNameByPos(ai_iPos));
	
	TRACE2("Role returned %s", tmpName);
	
	*pVal = tmpName; //???
	
	LEAVE_METHOD(CWrapCVM::get_RoleNameByPos, S_OK);	
}

STDMETHODIMP CWrapCVM::get_RoleIDByPos(long ai_iPos, BSTR* pVal)
{
	ENTER_METHOD(CWrapCVM::get_RoleIDByPos);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_RoleIDByPos,E_UNEXPECTED);
	}
	
	TRACE2("Trying to get role with position %d", ai_iPos);
	
	gstring tmpName(m_poDM->GetLogonAdaptor()->GetIdRoleByPos(ai_iPos));
	
	*pVal = tmpName; //???
	
	TRACE2("Role returned %d", tmpName);
	LEAVE_METHOD(CWrapCVM::get_RoleIDByPos, S_OK);
}

STDMETHODIMP CWrapCVM::get_NumRoles(long* pVal)
{
	ENTER_METHOD(CWrapCVM::get_NumRoles);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_NumRoles,E_UNEXPECTED);
	}
	
	*pVal = m_poDM->GetLogonAdaptor()->GetNumberOfRoles();
	
	TRACE2("Number of roles returned %d", *pVal);
	LEAVE_METHOD(CWrapCVM::get_NumRoles, S_OK);
}

STDMETHODIMP CWrapCVM::SetRoleByPos(long ai_iPos, VARIANT_BOOL ai_bSetAsDefaultRole, short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::SetRoleByPos);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::SetRoleByPos,S_OK);
	}
	
	TRACE2("Trying to set role with position %d", ai_iPos);
	
	*ao_RetVal = m_poDM->GetLogonAdaptor()->SetRole(ai_iPos, (ai_bSetAsDefaultRole == VARIANT_TRUE));
	
	TRACE2("Role returned %d", *ao_RetVal);
	LEAVE_METHOD(CWrapCVM::SetRoleByPos, S_OK);
}

STDMETHODIMP CWrapCVM::get_SessionChannel(IWrapChannel** pVal)
{
	ENTER_METHOD(CWrapCVM::get_SessionChannel);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_SessionChannel,E_UNEXPECTED);
	}
	
	m4return_t RetVal;
	
	ClChannel* pChannel = m_poDM->GetLogonAdaptor()->GetSessionChannel();
	
	RetVal = pChannel ? M4_SUCCESS : M4_ERROR;
	
	if (RetVal == M4_SUCCESS)
	{
		RetVal = NewInterface(CLSID_WrapChannel, IID_IWrapChannel, (IUnknown**)pVal);
		
		if (RetVal == M4_SUCCESS)
		{
			((CWrapChannel*)(*pVal))->Init(pChannel);
		}
	}
	
	if (RetVal == M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapCVM::get_SessionChannel, S_OK);
	}
	else
	{
		FormatError(M4_CH_WO_NO_SESSION_CHANNEL, "Unable to get the session channel");
		LEAVE_METHOD(CWrapCVM::get_SessionChannel, E_UNEXPECTED);
	}
}



// ============================================================================
//
//	_BuildPresentationFromID
//
// Función básica para construir una presentación en un idioma
//
// ============================================================================

short	_BuildPresentationFromID
(
	ClChannelManager	*ai_poChannelManager,
	BSTR				ai_PresentationID,
	BSTR				ai_Organization,
	BSTR				ai_RoleId,
	DATE				*aio_dtDate,
	SAFEARRAY			**ao_Presentation,
	m4uint8_t			ai_iLanguage,
	m4bool_t			ai_bDefinition,
	short				&aio_riType,
	short				&ao_riStyle,
	BSTR				*ao_pXML
)
{
	
	m4return_t	iResult = M4_SUCCESS ;

	if( ao_pXML != NULL )
	{
		*ao_pXML = NULL;
	}

	if( ai_poChannelManager == NULL )
	{
		FormatError( M4_CH_WO_CVM_NOT_STARTED, "CVM is not started." ) ;
		return( M4_ERROR ) ;
	}

	if(	ai_iLanguage == M4CL_LANGUAGE_NONE )
	{
		ai_iLanguage = (m4uint8_t) ai_poChannelManager->GetLogonAdaptor()->GetSessionLanguage( "", M4CL_LANGUAGE_NONE ) ;
	}

	gstring pszOrganization( ai_Organization ) ;
	
	if( pszOrganization == NULL )
	{
		ClLogonAdaptor *poLogonAdaptor = ai_poChannelManager->GetLogonAdaptor() ;

		if( poLogonAdaptor != NULL )
		{
			pszOrganization = poLogonAdaptor->GetOrganization() ;
		}
	}

	if( pszOrganization == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Could not create string for Organization" ) ;
		return( M4_ERROR ) ;
	}

	gstring pszPresentationID( ai_PresentationID ) ;
	
	if( pszPresentationID == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Could not create string for Presentation ID" ) ;
		return( M4_ERROR ) ;
	}

	//Role de ejecución, nos lo pasan por parámetro .
	gstring pszRoleID( ai_RoleId ) ;	
	
	m4pchar_t pcRoleID = pszRoleID ;

    if( pcRoleID == NULL || *pcRoleID == '\0')
	{
		//leemos el de la sessión
		ClLogonAdaptor *poLogonAdaptor = ai_poChannelManager->GetLogonAdaptor() ;

		if( poLogonAdaptor != NULL )
		{
			pszRoleID = poLogonAdaptor->GetRole() ;
		}
	}

	if( pszRoleID == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Could not create string for Role" ) ;
		return( M4_ERROR ) ;
	}

	// Auditoría de usuario
	ai_poChannelManager->UserActionsAudit.AddAction( pszOrganization, pszRoleID, ClUserAction::EV_PRESENTATION, pszPresentationID, NULL, NULL, M4_FALSE ) ;


	ClPresentationFactory* poClPresentationFactory = ai_poChannelManager->GetPresFac();
			
	if( poClPresentationFactory == NULL )
	{
		FormatError( M4_CH_WO_CANT_GET_PRES_FAC, "Could not get Presentation factory from DM" ) ;
		return( M4_ERROR ) ;
	}

	ClPresentation*	poPresentation = 0 ;
				
	ATLTRACE( "Trying to build presentation with Id %s, Role %s, Languaje %d\n", m4pchar_t(pszPresentationID), pszRoleID, ai_iLanguage ) ;
				
	iResult = poClPresentationFactory->BuildPresentationFromId( pszPresentationID, pszOrganization, pszRoleID, ai_iLanguage, M4_TRUE, ai_bDefinition, poPresentation ) ;

	if( iResult != M4_SUCCESS || poPresentation == NULL )
	{
		return( M4_ERROR ) ;
	}

	ao_riStyle = poPresentation->GetStyle() ;

	// Se calcula la versión de la presentación
	m4uint32_t i, iT3s, iIncludes ;
	m4date_t dNewVersion ;
	m4date_t dT3Version ;
	m4date_t dIncludeVersion ;
	m4date_t dLiteralsVersion ;

	dNewVersion = poPresentation->GetVersion() ;

	if( dNewVersion > M4CL_MINUS_INFINITE_IN_JULIAN + M4_LN4_DATE_PRECISION )
	{
		iT3s = poPresentation->GetNumT3() ;

		for(  i = 0 ; i < iT3s ; i++ )
		{
			dT3Version = poPresentation->GetVersionT3ByIndex( i ) ;

			if( dNewVersion < dT3Version )
			{
				dNewVersion = dT3Version ;
			}
		}

		iIncludes = poPresentation->GetNumInclude() ;

		for( i = 0 ; i < iIncludes ; i++ )
		{
			dIncludeVersion = poPresentation->GetVersionIncludeByPos( i ) ;

			if( dNewVersion < dIncludeVersion )
			{
				dNewVersion = dIncludeVersion ;
			}
		}

		if( aio_riType == M4CL_PRESENTATION_VERSION || aio_riType == M4CL_PRESENTATION_XML )
		{
			iResult = M4LiteralsVersion( ai_poChannelManager, pszOrganization, dLiteralsVersion ) ;

			if( iResult == M4_SUCCESS )
			{
				if( dNewVersion < dLiteralsVersion )
				{
					dNewVersion = dLiteralsVersion ;
				}
			}
		}
	}

	if( *aio_dtDate - dNewVersion >= -M4_LN4_DATE_PRECISION )
	{
		// Si ya se tiene la fecha mayor o igual no se manda otra vez
		return M4_SUCCESS;
	}

	*aio_dtDate = dNewVersion ;

	if( aio_riType == M4CL_PRESENTATION_VERSION )
	{
		// Si sólo se pide la versión se retorna
		return M4_SUCCESS;
	}

	m4pchar_t		pcXML = NULL ;
	m4pcchar_t		pccBuffer = NULL ;
	m4int32_t		iLength = 0 ;
	ClLongString	oXML ;

	pccBuffer = (m4pcchar_t) poPresentation->GetBuffer() ;
	iLength = poPresentation->GetSize() ;

	if( pccBuffer == NULL || iLength == 0 )
	{
		FormatError( M4_CH_WO_CANT_GET_PRES_BUFFER, "Could not get the buffer for the presentation" ) ;
		poPresentation->Destroy();
		if( pcXML != NULL ) delete pcXML ;
		return( M4_ERROR ) ;
	}

	// Si existe la presentación y se pide de xml se traduce
	if( aio_riType == M4CL_PRESENTATION_XML )
	{
		if( iLength != 9 || strncmp( pccBuffer, "NOT_FOUND", 9 ) != 0 )
		{
			iResult = poPresentation->BuildXML( oXML, pszOrganization, ai_poChannelManager );

			if( iResult != M4_SUCCESS )
			{
				poPresentation->Destroy();
				return( M4_ERROR ) ;
			}

			pccBuffer = oXML.GetString() ;
			iLength = oXML.GetLength() ;

			if( pccBuffer == NULL || iLength == 0 )
			{
				FormatError( M4_CH_WO_CANT_GET_PRES_BUFFER, "Could not get xml from the presentation" ) ;
				poPresentation->Destroy();
				return( M4_ERROR ) ;
			}
		}
		else
		{
			// Si no existe la presentación se devuelve un buffer vacío
			pccBuffer = NULL ;
			iLength = 0 ;
		}
	}

	if (ao_Presentation != NULL)
	{
		*ao_Presentation = 0 ;
		*ao_Presentation = ::SafeArrayCreateVector( VT_UI1, 0, iLength) ;

		if( *ao_Presentation == NULL )
		{
			FormatError( M4_CH_WO_CANT_CREATE_SAFE_ARRAY, "Could not create safe array" ) ;
			poPresentation->Destroy();
			if( pcXML != NULL ) delete pcXML ;
			return( M4_ERROR ) ;
		}
							
		m4uchar_t*	poTmpBuffer;
		HRESULT		hr = SafeArrayAccessData( *ao_Presentation, (void**)&poTmpBuffer) ;

		if( SUCCEEDED(hr) )
		{
			(void)memcpy( poTmpBuffer, pccBuffer, iLength) ;
			
			hr = SafeArrayUnaccessData( *ao_Presentation ) ;
			
			if( SUCCEEDED(hr) )
			{
				iResult = M4_SUCCESS ;
			}
			else
			{
				iResult = M4_ERROR ;
			}
		}
		else
		{
			iResult = M4_ERROR ;
		}

		if( iResult != M4_SUCCESS )
		{
			::SafeArrayDestroy( *ao_Presentation ) ;
			*ao_Presentation = 0 ;
			poPresentation->Destroy();
			if( pcXML != NULL ) delete pcXML ;
			return( M4_ERROR ) ;
		}
	}

	// generates the XML
	// Por compatibilidad. Esto habrá que quitarlo
	if (ao_pXML != NULL && pcXML != NULL)
	{
		gstring gsResult(pcXML);
		*ao_pXML = gsResult;
	}

	if( pcXML != NULL ) delete pcXML ;

	poPresentation->Destroy();

	return( M4_SUCCESS ) ;	
}

STDMETHODIMP CWrapCVM::BuildPresentationDefFromID( BSTR ai_PresentationId, BSTR ai_Organization, BSTR ai_UseId, SAFEARRAY** ao_Presentation, short* ao_RetVal )
{
	ENTER_METHOD( CWrapCVM::BuildPresentationDefFromID ) ;
	
	//Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::BuildPresentationDefFromID" << "M4ChWrap.dll" );
	LEAVE_METHOD( CWrapCVM::BuildPresentationDefFromID, S_OK ) ;
}


STDMETHODIMP CWrapCVM::BuildPresentationFromID( BSTR ai_sPresentationId, BSTR ai_sOrganization, BSTR ai_sRoleId, short ai_iLanguage, VARIANT_BOOL ai_bDefinition, SAFEARRAY** ao_abPresentation, short* ao_iRetVal)
{
	ENTER_METHOD( CWrapCVM::BuildPresentationFromID ) ;
	
	short iType = M4CL_PRESENTATION_OBL ;
	short iStyle = 0;
	m4date_t dVersion = 0 ;

	*ao_iRetVal = _BuildPresentationFromID( m_poDM, ai_sPresentationId, ai_sOrganization, ai_sRoleId, &dVersion, ao_abPresentation, m4uint8_t( ai_iLanguage ), ai_bDefinition, iType, iStyle, NULL ) ;

	LEAVE_METHOD( CWrapCVM::BuildPresentationFromID, S_OK ) ;
}


STDMETHODIMP CWrapCVM::BuildPresFromIdByLang( BSTR ai_PresentationId, BSTR ai_Organization, BSTR ai_UseId, short ai_iLanguage, SAFEARRAY **ao_Presentation, short *ao_iRetVal )
{
	ENTER_METHOD( CWrapCVM::BuildPresFromIdByLang ) ;
	
	//Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::BuildPresFromIdByLang" << "M4ChWrap.dll" );
	LEAVE_METHOD( CWrapCVM::BuildPresFromIdByLang, S_OK ) ;
}


STDMETHODIMP CWrapCVM::BuildPresFromIdByLangType( BSTR ai_PresentationId, BSTR ai_Organization, BSTR ai_UseId, short ai_iLangType, SAFEARRAY **ao_Presentation, short *ao_iRetVal )
{
	ENTER_METHOD( CWrapCVM::BuildPresFromIdByLangType ) ;
	
	//Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::BuildPresFromIdByLangType" << "M4ChWrap.dll" );
	LEAVE_METHOD( CWrapCVM::BuildPresFromIdByLangType, S_OK ) ;
}

STDMETHODIMP CWrapCVM::GetXMLFromPresentationId( BSTR ai_PresentationId, BSTR ai_Organization, BSTR ai_UseId, BSTR *ao_pXML, short* ao_iRetVal)
{
	ENTER_METHOD( CWrapCVM::GetXMLFromPresentationId ) ;

    DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::GetXMLFromPresentationId" << "M4ChWrap.dll" );
	LEAVE_METHOD( CWrapCVM::GetXMLFromPresentationId, E_UNEXPECTED );
}


STDMETHODIMP CWrapCVM::BuildTypedPresentationFromId(BSTR ai_sPresentationId, BSTR ai_sOrganization, BSTR ai_sRoleId, short ai_iLanguage, DATE* aio_dtDate, SAFEARRAY** ao_abPresentation, short* aio_iType, short* ao_iStyle, short* ao_iRetVal)
{
	ENTER_METHOD( CWrapCVM::BuildTypedPresentationFromId ) ;
	
	if( m_poObjService == NULL )
	{
		FormatError( M4_CH_WO_CVM_NOT_STARTED, "CVM is not started." ) ;
		*ao_iRetVal = M4_ERROR ;
		LEAVE_METHOD( CWrapCVM::BuildTypedPresentationFromId, S_FALSE ) ;
	}

	m4date_t dVersion = DateToM4date( *aio_dtDate ) ;
	*ao_iRetVal = _BuildPresentationFromID( m_poDM, ai_sPresentationId, ai_sOrganization, ai_sRoleId, &dVersion, ao_abPresentation, ai_iLanguage, M4_FALSE, *aio_iType, *ao_iStyle, NULL ) ;
	*aio_dtDate = M4dateToDate( dVersion ) ;

	LEAVE_METHOD( CWrapCVM::BuildTypedPresentationFromId, S_OK ) ;
}


STDMETHODIMP CWrapCVM::CleanLDB(short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CleanLDB);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CleanLDB,S_OK);
	}
	
	*ao_RetVal = m_poObjService->GetpLDB()->ClearAll();
	LEAVE_METHOD(CWrapCVM::CleanLDB, S_OK);
}

STDMETHODIMP CWrapCVM::CleanAllCaches(short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CleanAllCaches);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CleanAllCaches,S_OK);
	}
	
	//*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches();

	//Bug 0075517
	//Para no duplicar código, invocamos al miembro pasando que elimine todas las caches.
	//Quedaría mucho mejor que VB invocara a CacheClean de esta forma CleanAllCaches sobraría.
	CacheClean(CACHETYPE_ALL, ao_RetVal);
	
	LEAVE_METHOD(CWrapCVM::CleanAllCaches, S_OK);
}

STDMETHODIMP CWrapCVM::CleanCMCRCache(short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CleanCMCRCache);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CleanCMCRCache,S_OK);
	}
	
	*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_CMCR_CACHE_TYPE);
	
	LEAVE_METHOD(CWrapCVM::CleanCMCRCache, S_OK);
}

STDMETHODIMP CWrapCVM::CleanCSCRCache(short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CleanCSCRCache);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CleanCSCRCache,S_OK);
	}
	
	*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_CSCR_CACHE_TYPE);
	
	LEAVE_METHOD(CWrapCVM::CleanCSCRCache, S_OK);
}

STDMETHODIMP CWrapCVM::CleanPresentationCache(short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CleanPresentationCache);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CleanPresentationCache,S_OK);
	}
	
	*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_PRES_CACHE_TYPE);
	
	LEAVE_METHOD(CWrapCVM::CleanPresentationCache, S_OK);
}

STDMETHODIMP CWrapCVM::CleanDataCache(short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CleanDataCache);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CleanDataCache,S_OK);
	}
	
	*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_DATA_CACHE_TYPE);
	
	LEAVE_METHOD(CWrapCVM::CleanDataCache, S_OK);
}

STDMETHODIMP CWrapCVM::CacheClean(CACHETYPE CacheType, short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CacheClean);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CacheClean,S_OK);
	}
	
	switch (CacheType)
	{
	case CACHETYPE_CMCR:
		*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_CMCR_CACHE_TYPE);
		break;
		
	case CACHETYPE_CSCR:
		*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_CSCR_CACHE_TYPE);
		break;
		
	case CACHETYPE_PRESENTATION:
		*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_PRES_CACHE_TYPE);
		break;
		
	case CACHETYPE_MAP:
		*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_MAP_CACHE_TYPE);
		break;
		
	case CACHETYPE_DATA:
		*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_DATA_CACHE_TYPE);
		break;
		
	case CACHETYPE_LDB:
		*ao_RetVal = m_poObjService->GetpLDB()->ClearAll();
		break;
		
	case CACHETYPE_LITERALS:
		*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_LITERALS_CACHE_TYPE);
		break;
		
	case CACHETYPE_ALL:
		*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_ALL_CACHE_TYPE);
		*ao_RetVal = m_poObjService->GetpLDB()->ClearAll();
		break;
		
	default:
		*ao_RetVal = M4_ERROR;
		break;
	}	
	
	LEAVE_METHOD(CWrapCVM::CacheClean, S_OK);
}

STDMETHODIMP CWrapCVM::get_DefaultCSType(CSTYPE* pVal)
{
	ENTER_METHOD(CWrapCVM::get_DefaultCSType);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_DefaultCSType, E_UNEXPECTED);
	}
	
	m4double_t db = m_poDM->GetDefaultCSType();
	
	*pVal = CSTYPE(m4uint32_t(db));
	
	LEAVE_METHOD(CWrapCVM::get_DefaultCSType, S_OK);
}

STDMETHODIMP CWrapCVM::put_DefaultCSType(CSTYPE newVal)
{
	ENTER_METHOD(CWrapCVM::put_DefaultCSType);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::put_DefaultCSType, E_UNEXPECTED);
	}
	
	m4VariantType var(newVal);
	
	m_poDM->SetDefaultCSType(var);
	
	LEAVE_METHOD(CWrapCVM::put_DefaultCSType, S_OK);
}

STDMETHODIMP CWrapCVM::get_VMSign(IWrapVMSign** pVal)
{
	m4return_t Status;
	
	ENTER_METHOD(CWrapCVM::get_VMSign);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_VMSign, E_UNEXPECTED);
	}
	
	Status = NewInterface(CLSID_WrapVMSign,	IID_IWrapVMSign, (IUnknown**)pVal);
	
	if (Status != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapCVM::get_VMSign, E_UNEXPECTED);
	}
	else
	{
		((CWrapVMSign*)(*pVal))->Init(m_poDM->GetDMContext());
		
		LEAVE_METHOD(CWrapCVM::get_VMSign, S_OK);
	}
}

STDMETHODIMP CWrapCVM::get_ProxyListIt(IWrapProxyListIt** pVal)
{
	ENTER_METHOD(CWrapCVM::get_ProxyListIt);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_ProxyListIt, E_UNEXPECTED);
	}
	
	if (m_poDM->GetProxySpace() != NULL)
	{
		ClProxyList *poProxyList = m_poDM->GetProxySpace()->GetProxyList();

		if (poProxyList != NULL)
		{
			NewInterface(CLSID_WrapProxyListIt, IID_IWrapProxyListIt, (IUnknown**)pVal);
			((CWrapProxyListIt*)(*pVal))->Init(poProxyList);
			LEAVE_METHOD(CWrapCVM::get_ProxyListIt, S_OK);
		}
		else
		{
			LEAVE_METHOD(CWrapCVM::get_ProxyListIt, E_UNEXPECTED);
		}
	}
	else
	{
		LEAVE_METHOD(CWrapCVM::get_ProxyListIt, E_UNEXPECTED);
	}
}

STDMETHODIMP CWrapCVM::GetProxyDebugView(BSTR ai_LocalId, BSTR ai_RemoteId, IWrapChannel** pVal)
{
	
	ENTER_METHOD(CWrapCVM::GetProxyDebugView);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetProxyDebugView, E_UNEXPECTED);
	}
	gstring	tmpRemoteId( ai_RemoteId ) ;
	
	if (m_poDM->GetProxySpace() != NULL)
	{
		ClChannel *poChannel = m_poDM->GetProxySpace()->GetProxyDebugView(gstring(ai_LocalId), atoi(tmpRemoteId));

		if (poChannel != NULL)
		{
			NewInterface(CLSID_WrapChannel, IID_IWrapChannel, (IUnknown**)pVal);
			((CWrapChannel*)(*pVal))->Init(poChannel,M4_TRUE);
			LEAVE_METHOD(CWrapCVM::GetProxyDebugView, S_OK);
		}
		else
		{
			LEAVE_METHOD(CWrapCVM::GetProxyDebugView, E_UNEXPECTED);
		}
	}
	else
	{
		LEAVE_METHOD(CWrapCVM::get_ProxyListIt, E_UNEXPECTED);
	}
}


STDMETHODIMP CWrapCVM::GetMultiValidationChannel(IWrapChannel** ao_poChannel)
{
	ENTER_METHOD(CWrapCVM::GetMultiValidationChannel);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetMultiValidationChannel, E_UNEXPECTED);
	}
	
	ClChannel* poChannel = m_poDM->GetMultiValidationChannel();
	
	if (ao_poChannel)
	{
		ao_poChannel=(IWrapChannel**)poChannel;
		
		LEAVE_METHOD(CWrapCVM::GetMultiValidationChannel, S_OK);
	}
	else
	{
		// Cannot create access
		FormatError(M4_CH_WO_CANT_CREATE_CHANNEL, "Unable to get SCH_MULTI_VALIDATE channel");
		LEAVE_METHOD(CWrapCVM::GetMultiValidationChannel, E_UNEXPECTED);
	}
}



STDMETHODIMP CWrapCVM::get_EmulationLevel(EMULATE_TYPE* pVal)
{
	ENTER_METHOD(CWrapCVM::get_EmulationLevel);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_EmulationLevel, E_UNEXPECTED);
	}
	
	m4uint32_t db = m_poDM->EmulationLevel();
	
	*pVal = EMULATE_TYPE(m4uint32_t(db));
	
    ATLTRACE("Emulation <%d>\n", *pVal);
	LEAVE_METHOD(CWrapCVM::get_EmulationLevel, S_OK);
}

STDMETHODIMP CWrapCVM::DebugCounters(short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::DebugCounters);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::DebugCounters, S_OK);
	}
	
    m4pchar_t buffer = new m4char_t [512];
	
    m4uint32_t cmu = 0;
    m4uint32_t cna = 0;
    //m4uint32_t hwc;
	
	/*
	#ifdef _DEBUG
    _CrtMemState s;
    g_StMemInfo.HeapCheckpoint (&s);
	
	  cmu = s.lSizes[_CRT_BLOCK] + s.lSizes[_NORMAL_BLOCK];
	  cna = s.lCounts[_CRT_BLOCK] + s.lCounts[_NORMAL_BLOCK];
	  //hwc = s.lHighWaterCount;
	  #endif
	*/
	
    sprintf (buffer,
		"Access: \t\t%i\nChannel_Data: \t%i\nBlock: \t\t%i\nRegisterIt: \t\t%i\nItemIt: \t\t%i\n\nHeapAllocSpace: \t%i\nHeapAllocCount: \t%i",
		g_StMemInfo.iAccess,
		g_StMemInfo.iChannel,
		g_StMemInfo.iBlock_Channel,
		g_StMemInfo.iRegister,
		g_StMemInfo.iItem,
		cmu,
		cna
		);
	
#ifdef _DEBUG
    MessageBox (NULL, buffer, "ChannelWrapper Counters", MB_OK);
#endif
	
    ofstream ofs;
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;

	M4Open( ofs, "c:\\temp\\mem_xxx.txt", M4UniAppend, eEncoding );

    ofs << g_StMemInfo.iAccess << '\t';
    ofs << g_StMemInfo.iChannel << '\t';
    ofs << g_StMemInfo.iBlock_Channel << '\t';
    ofs << g_StMemInfo.iRegister << '\t';
    ofs << g_StMemInfo.iItem << '\t';
    ofs << cmu << '\t';
    ofs << cna << "\t\n";
    ofs.close ();
	
    delete []buffer;
	
    *ao_RetVal = M4_SUCCESS;
	
	LEAVE_METHOD(CWrapCVM::DebugCounters, S_OK);
}

STDMETHODIMP CWrapCVM::DebugMemLDB(BSTR ai_Message)
{
	ENTER_METHOD(CWrapCVM::DebugMemLDB);
	// A extinguir.
	LEAVE_METHOD(CWrapCVM::DebugMemLDB, S_OK);
}

STDMETHODIMP CWrapCVM::EnableTimeOut(short bEnable)
{
	ENTER_METHOD(CWrapCVM::EnableTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::EnableTimeOut, E_UNEXPECTED);
	}
	
	m_poObjService->GetpM4ObjServiceContext()->EnableTimeOut(bEnable?M4_TRUE:M4_FALSE);
	
	LEAVE_METHOD(CWrapCVM::EnableTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::SetDefaultCSTimeOut(long iTimeOut)
{
	ENTER_METHOD(CWrapCVM::SetDefaultCSTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::SetDefaultCSTimeOut, E_UNEXPECTED);
	}
	
	m_poDM->GetEnvironment()->SetDefaultCSTimeOut(iTimeOut*1000);
	
	LEAVE_METHOD(CWrapCVM::SetDefaultCSTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::SetNextCSTimeOut(long iTimeOut)
{
	ENTER_METHOD(CWrapCVM::SetNextCSTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::SetNextCSTimeOut, E_UNEXPECTED);
	}
	
	m_poDM->GetEnvironment()->SetNextCSTimeOut(iTimeOut*1000);
	
	LEAVE_METHOD(CWrapCVM::SetNextCSTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::SetGenCSTimeOut(long iTimeOut)
{
	ENTER_METHOD(CWrapCVM::SetGenCSTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::SetGenCSTimeOut, E_UNEXPECTED);
	}
	
	m_poDM->GetEnvironment()->SetGenCSTimeOut(iTimeOut*1000);
	
	LEAVE_METHOD(CWrapCVM::SetGenCSTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::GetDefaultCSTimeOut(long * piTimeOut)
{
	ENTER_METHOD(CWrapCVM::GetDefaultCSTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetDefaultCSTimeOut, E_UNEXPECTED);
	}
	
	*piTimeOut = m_poDM->GetEnvironment()->GetDefaultCSTimeOut() / 1000;
	
	LEAVE_METHOD(CWrapCVM::GetDefaultCSTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::GetNextCSTimeOut(long * piTimeOut)
{
	ENTER_METHOD(CWrapCVM::GetNextCSTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetNextCSTimeOut, E_UNEXPECTED);
	}
	
	*piTimeOut = m_poDM->GetEnvironment()->GetNextCSTimeOut() / 1000;
	
	LEAVE_METHOD(CWrapCVM::GetNextCSTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::GetGenCSTimeOut(long * piTimeOut)
{
	ENTER_METHOD(CWrapCVM::GetGenCSTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetGenCSTimeOut, E_UNEXPECTED);
	}
	
	*piTimeOut = m_poDM->GetEnvironment()->GetGenCSTimeOut() / 1000;
	
	LEAVE_METHOD(CWrapCVM::GetGenCSTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::ResetNextCSTimeOut()
{
	ENTER_METHOD(CWrapCVM::ResetNextCSTimeOut);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::ResetNextCSTimeOut, E_UNEXPECTED);
	}
	
	m_poDM->GetEnvironment()->ResetNextCSTimeOut();
	
	LEAVE_METHOD(CWrapCVM::ResetNextCSTimeOut, S_OK);
}


STDMETHODIMP CWrapCVM::SetCSTimeoutFunction(BSTR sDLLName, BSTR sFuncName)
{
	ENTER_METHOD(CWrapCVM::SetCSTimeoutFunction);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::SetCSTimeoutFunction, E_UNEXPECTED);
	}
	
	gstring tmpDLLName(sDLLName);
	gstring tmpFuncName(sFuncName);
	
	m4return_t ret = m_poObjService->GetpM4ObjServiceContext()->SetCSTimeoutFunction(m4pchar_t(tmpDLLName),m4pchar_t(tmpFuncName));
	
	LEAVE_METHOD(CWrapCVM::SetCSTimeoutFunction, (ret == M4_SUCCESS ? S_OK : S_FALSE));
}

STDMETHODIMP CWrapCVM::IsDCData(VARIANT_BOOL * pVal)
{
	ENTER_METHOD(CWrapCVM::IsDCData);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::IsDCData, E_UNEXPECTED);
	}
	
	*pVal = (m_poDM->IsDCData()) ? VARIANT_TRUE : VARIANT_FALSE;
	
	LEAVE_METHOD(CWrapCVM::IsDCData, S_OK);
}

STDMETHODIMP CWrapCVM::GetDCData(IWrapBookMark * pBookMark, long * pNodeHandle, long * pItemHandle)
{
	ENTER_METHOD(CWrapCVM::GetDCData);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetDCData, E_UNEXPECTED);
	}
	
	if (m_poDM->IsDCData() == M4_FALSE)
	{
		LEAVE_METHOD(CWrapCVM::GetDCData, S_FALSE);
	}
	
	// we dont have the operator = for bookmarks so we use a temp access
	
	ClChannel *poChannel;
	ClAccessBookMark *poBookMark;
	ClHandle *poNodeHandle, *poItemHandle;
	
	m_poDM->GetDCData(poChannel, poBookMark, poNodeHandle, poItemHandle);
	
	ClAccess *poAccess;
	poChannel->CreateAccess(poAccess);
	
	poBookMark->ApplyTo(*poAccess);
	
	((CWrapBookMark*)pBookMark)->GetFrom(*poAccess);
	
	poAccess->Destroy();
	
	// save the handles
	*pNodeHandle = m4uint32_t(*poNodeHandle);
	*pItemHandle = m4uint32_t(*poItemHandle);
	
	LEAVE_METHOD(CWrapCVM::GetDCData, S_OK);
}

STDMETHODIMP CWrapCVM::SetDCEnabled(VARIANT_BOOL ai_Flag)
{
	ENTER_METHOD(CWrapCVM::SetDCEnabled);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::SetDCEnabled, E_UNEXPECTED);
	}
	
	m_poDM->SetDCEnabled((ai_Flag == VARIANT_TRUE) ? M4_TRUE : M4_FALSE);
	
	LEAVE_METHOD(CWrapCVM::SetDCEnabled, S_OK);
}

STDMETHODIMP CWrapCVM::IsDCEnabled(VARIANT_BOOL * ao_pFlag)
{
	ENTER_METHOD(CWrapCVM::IsDCEnabled);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::IsDCEnabled, E_UNEXPECTED);
	}
	
	*ao_pFlag = (m_poDM->IsDCEnabled()) ? VARIANT_TRUE : VARIANT_FALSE;
	
	LEAVE_METHOD(CWrapCVM::IsDCEnabled, S_OK);
}

STDMETHODIMP CWrapCVM::ConvertOneWay( BSTR ai_pcInString, BSTR * ao_pcOutString, short* ao_RetVal )
{
	ENTER_METHOD(CWrapCVM::ConvertOneWay);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::ConvertOneWay, S_OK);
	}
	
	m4return_t	iResult ;
	m4pcchar_t	pccInString ;
	m4char_t	acOutString[ 2 * M4_MAX_LEN_ONE_WAY + 1 ] ;
	
	gstring		tmpInString( ai_pcInString ) ;
	
	
	if( tmpInString )
	{
		pccInString = m4pcchar_t( tmpInString ) ;
		
		// Se asume que el usuario ya viene concatenado antes de la password
		iResult = M4ConvertPassword( "", pccInString, acOutString, 2 * M4_MAX_LEN_ONE_WAY ) ;
		
		if( iResult == M4_SUCCESS )
		{
			gstring		tmpOutString( acOutString ) ;
			
			if( tmpOutString )
			{
				*ao_pcOutString = (BSTR) tmpOutString ;
				*ao_RetVal = M4_SUCCESS ;
			}
			else
			{
				*ao_RetVal = M4_ERROR ;
			}
		}
		else
		{
			*ao_RetVal = M4_ERROR ;
		}
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CREATE_STRING, "Cannot create in string");
		*ao_RetVal = M4_WARNING ;
	}
	
	LEAVE_METHOD(CWrapCVM::ConvertOneWay, S_OK);
	
}

STDMETHODIMP CWrapCVM::Convert(BSTR ai_pcInString, BSTR * ao_pcOutString, short * ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::Convert);

	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::Convert" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapCVM::Convert, S_OK);
}


STDMETHODIMP CWrapCVM::NewLogon( BSTR ai_stUsername, BSTR ai_stPassword, long ai_iLanguage, BSTR ai_stLogonServer, long ai_iPortNumber, VARIANT_BOOL ai_bUseDefaultRole, BSTR ai_stSSL, VARIANT_BOOL ai_bUseSSL, short * ao_RetVal )
{
	ENTER_METHOD( CWrapCVM::NewLogon ) ;
	
	*ao_RetVal = M4_ERROR ;
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::NewLogon, S_OK);
	}
	
	gstring	tmpUserName( ai_stUsername ) ;
	gstring	tmpPassword( ai_stPassword ) ;
	gstring	tmpServername( ai_stLogonServer ) ;
	gstring	tmpSSL( ai_stSSL ) ;

	*ao_RetVal = m_poDM->GetLogonAdaptor()->Logon
			(
			tmpUserName,
			tmpPassword,
			ai_iLanguage,
			tmpServername,
			ai_iPortNumber,
			( ai_bUseDefaultRole == VARIANT_TRUE ) ,
			tmpSSL,
			( ai_bUseSSL == VARIANT_TRUE )
			) ;
	
	/*
	if (*ao_RetVal != M4_ERROR)
	{
	m4return_t ret;
	IWrapExecutor *pExec;
	ret = get_Executor(&pExec);
	if (M4_SUCCESS == ret)
	{
	pExec->SetSttStatus(63,this);
	}
}*/
	
	LEAVE_METHOD( CWrapCVM::NewLogon, S_OK ) ;
}

STDMETHODIMP CWrapCVM::CacheRemoveByObjectId(CACHETYPE CacheType, BSTR ai_stObjectId, short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CacheRemoveByObjectId);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CacheRemoveByObjectId ,S_OK);
	}
	
	m4uint16_t iCacheType = M4_CACHE_NONE_CACHE_TYPE;
	
	gstring	tmpObjectId( ai_stObjectId ) ;
	
	if( tmpObjectId )
	{
		switch (CacheType)
		{
		case CACHETYPE_CMCR:
			iCacheType = M4_CACHE_CMCR_CACHE_TYPE;
			break;
			
		case CACHETYPE_CSCR:
			iCacheType = M4_CACHE_CSCR_CACHE_TYPE;
			break;
			
		case CACHETYPE_PRESENTATION:
			iCacheType = M4_CACHE_PRES_CACHE_TYPE;
			break;
			
		case CACHETYPE_MAP:
			iCacheType = M4_CACHE_MAP_CACHE_TYPE;
			break;
			
		case CACHETYPE_DATA:
			iCacheType = M4_CACHE_DATA_CACHE_TYPE;
			break;
			
		case CACHETYPE_ALL:
			iCacheType = M4_CACHE_ALL_CACHE_TYPE;
			break;
			
		case CACHETYPE_LDB:
			break;
			
		default:
			break;
		}	
	}
	
	if (iCacheType != M4_CACHE_NONE_CACHE_TYPE)
	{
		*ao_RetVal = m_poDM->GetCacheDirectory()->RemoveObjectById(iCacheType, tmpObjectId, M4_TRUE);
	}
	else
	{
		*ao_RetVal = M4_ERROR;
	}
	
	LEAVE_METHOD(CWrapCVM::CacheRemoveByObjectId, S_OK);
}

STDMETHODIMP CWrapCVM::NewCacheRemoveByObjectId(CACHETYPE CacheType, BSTR ai_stObjectId, VARIANT_BOOL ai_bLocalOnly, short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::NewCacheRemoveByObjectId);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::NewCacheRemoveByObjectId ,S_OK);
	}
	
	m4bool_t bLocalOnly = (ai_bLocalOnly == VARIANT_TRUE );
	
	m4uint16_t iCacheType = M4_CACHE_NONE_CACHE_TYPE;
	
	gstring	tmpObjectId( ai_stObjectId ) ;
	
	if( tmpObjectId )
	{
		switch (CacheType)
		{
		case CACHETYPE_CMCR:
			iCacheType = M4_CACHE_CMCR_CACHE_TYPE;
			break;
			
		case CACHETYPE_CSCR:
			iCacheType = M4_CACHE_CSCR_CACHE_TYPE;
			break;
			
		case CACHETYPE_PRESENTATION:
			iCacheType = M4_CACHE_PRES_CACHE_TYPE;
			break;
			
		case CACHETYPE_MAP:
			iCacheType = M4_CACHE_MAP_CACHE_TYPE;
			break;
			
		case CACHETYPE_DATA:
			iCacheType = M4_CACHE_DATA_CACHE_TYPE;
			break;
			
		case CACHETYPE_ALL:
			iCacheType = M4_CACHE_ALL_CACHE_TYPE;
			break;
			
		case CACHETYPE_LDB:
			break;
			
		default:
			break;
		}	
	}
	
	if (iCacheType != M4_CACHE_NONE_CACHE_TYPE)
	{
		*ao_RetVal = m_poDM->GetCacheDirectory()->RemoveObjectById(iCacheType, tmpObjectId, bLocalOnly);
	}
	else
	{
		*ao_RetVal = M4_ERROR;
	}
	
	LEAVE_METHOD(CWrapCVM::NewCacheRemoveByObjectId, S_OK);
}

STDMETHODIMP CWrapCVM::CacheRefresh(short* ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CacheRefresh);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CacheRefresh, S_OK);
	}
	
	*ao_RetVal = m_poDM->GetCacheDirectory()->Refresh();
	
	LEAVE_METHOD(CWrapCVM::CacheRefresh, S_OK);
}


STDMETHODIMP CWrapCVM::ClearFiles()
{
	ENTER_METHOD(CWrapCVM::ClearFiles);
	
	ClFileManager *poFileManager = ClFileManager::Instance();
	
	m4return_t res = poFileManager->CollectGarbage();
	
	if (res == M4_SUCCESS)
		LEAVE_METHOD(CWrapCVM::ClearFiles, S_OK);
	else
		LEAVE_METHOD(CWrapCVM::ClearFiles, S_FALSE);
}


STDMETHODIMP CWrapCVM::SetConnectionParam(BSTR ai_stParamName, BSTR ai_stParamValue, short * ao_iRetVal)
{
	ENTER_METHOD( CWrapCVM:: SetConnectionParam) ;
	
	*ao_iRetVal = M4_ERROR ;
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::SetConnectionParam, S_OK);
	}
	
	gstring	tmpParamName( ai_stParamName ) ;
	gstring	tmpParamValue( ai_stParamValue ) ;
	
	if( tmpParamName != NULL && tmpParamValue != NULL )
	{
		*ao_iRetVal = m_poDM->GetLogonAdaptor()->SetConnectionParam(tmpParamName, tmpParamValue) ;
	}
	else
	{
		// FormatError( M4_CH_WO_CANT_CREATE_STRING, "Cannot create string" ) ;
	}
	
	LEAVE_METHOD( CWrapCVM::SetConnectionParam, S_OK ) ;
}

STDMETHODIMP CWrapCVM::get_Executor(IWrapExecutor **pVal)
{
	ENTER_METHOD(CWrapCVM::get_Executor);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_Executor, E_UNEXPECTED);
	}
	
	if (!m_poExecutor)
	{
		TRACE("Allocating new executor");
		
		if (NewInterface(CLSID_WrapExecutor, IID_IWrapExecutor, (IUnknown**) &m_poExecutor) != M4_SUCCESS)
		{
			LEAVE_METHOD(CWrapRgIt::get_Executor, E_UNEXPECTED);
		}
		
		// Inicializar  el ejecutor con la factoría para los breakpoints
		if( m_poDM != NULL )
		{
			((CWrapExecutor*)m_poExecutor)->SetFactory( m_poDM->GetpFactory() ) ;
		}

		m4return_t ret = ((CWrapExecutor*)m_poExecutor)->Init((ClExecutor*)m_poObjService->GetpExecutor());

		if (ret!=M4_SUCCESS){
			LEAVE_METHOD(CWrapCVM::get_Executor, E_UNEXPECTED);
		}
	}
	
	TRACE2("The executor has address %p", m_poExecutor);
	
	*pVal = m_poExecutor;
	
	((IUnknown*)*pVal)->AddRef();
	
	LEAVE_METHOD(CWrapCVM::get_Executor, S_OK);
}

STDMETHODIMP CWrapCVM::CSTraceEnable(VARIANT_BOOL ai_EnableIt)
{
	ENTER_METHOD(CWrapCVM::CSTraceEnable);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::CSTraceEnable" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapCVM::CSTraceEnable, S_OK);
}

STDMETHODIMP CWrapCVM::CSTraceBeginTag(BSTR ai_stNewTag)
{
	ENTER_METHOD(CWrapCVM::CSTraceBeginTag);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CSTraceBeginTag, E_UNEXPECTED);
	}
	
	gstring	tmpTag( ai_stNewTag ) ;
	
	if (tmpTag!=NULL)
		m_poObjService->GetpCsTrace()->SetTagName(tmpTag);
	else
		m_poObjService->GetpCsTrace()->ResetTagName();
	
	LEAVE_METHOD(CWrapCVM::CSTraceBeginTag, S_OK);
}

STDMETHODIMP CWrapCVM::CSTraceResetTag()
{
	ENTER_METHOD(CWrapCVM::CSTraceResetTag);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CSTraceResetTag, E_UNEXPECTED);
	}
	
	m_poObjService->GetpCsTrace()->ResetTagName();
	
	LEAVE_METHOD(CWrapCVM::CSTraceResetTag, S_OK);
}

STDMETHODIMP CWrapCVM::CSTraceSetOutputFileName(BSTR ai_stFileName)
{
	ENTER_METHOD(CWrapCVM::CSTraceSetOutputFileName);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::CSTraceSetOutputFileName" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapCVM::CSTraceSetOutputFileName, S_OK);
}

STDMETHODIMP CWrapCVM::CSTraceGetOutputFileName(BSTR* ao_stFileName)
{
	ENTER_METHOD(CWrapCVM::CSTraceGetOutputFileName);

	// Bug 0113644 Deprecated. No longer supported.
	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::CSTraceGetOutputFileName" << "M4ChWrap.dll" );
	LEAVE_METHOD(CWrapCVM::CSTraceGetOutputFileName, S_OK);
}

STDMETHODIMP CWrapCVM::CSTraceShowCachedOperations(VARIANT_BOOL ai_ShowIt)
{
	ENTER_METHOD(CWrapCVM::CSTraceShowCachedOperations);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CSTraceShowCachedOperations, E_UNEXPECTED);
	}
	
	if (ai_ShowIt!=M4_FALSE)
		m_poObjService->GetpCsTrace()->EnableCachedOperations();
	else
		m_poObjService->GetpCsTrace()->DisableCachedOperations();
	
	LEAVE_METHOD(CWrapCVM::CSTraceShowCachedOperations, S_OK);
}


STDMETHODIMP CWrapCVM::get_IsMultiOrganization(short *pVal)
{
	ENTER_METHOD(CWrapCVM::get_IsMultiOrganization);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_IsMultiOrganization, E_UNEXPECTED);
	}
	
	*pVal = m_poDM->GetLogonAdaptor()->GetIsMultiOrganization() ;
	
	LEAVE_METHOD(CWrapCVM::get_IsMultiOrganization, S_OK);
}

STDMETHODIMP CWrapCVM::get_Credential(BSTR *pVal)
{
	ENTER_METHOD(CWrapCVM::get_Credential);
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::get_Credential, E_UNEXPECTED);
	}
	
	m4pchar_t	pcCredential ;
	
	
	pcCredential = m_poDM->GetLogonAdaptor()->GetCredential() ;
	
	if( pcCredential == NULL )
	{
		pcCredential = "" ;
	}
	
	gstring	tmpOutString( pcCredential ) ;
	
	*pVal = (BSTR) tmpOutString ;
	
	LEAVE_METHOD(CWrapCVM::get_Credential, S_OK);
}

STDMETHODIMP CWrapCVM::RestoreLogon(BSTR ai_stCredential, long ai_iLanguage, BSTR ai_stLogonServer, long ai_iPortNumber, BSTR ai_stSSL, VARIANT_BOOL ai_bUseSSL, short *ao_RetVal)
{
	ENTER_METHOD( CWrapCVM::RestoreLogon ) ;
	
	*ao_RetVal = M4_ERROR ;
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::RestoreLogon, S_OK);
	}
	
	gstring	tmpCredential( ai_stCredential ) ;
	gstring	tmpServername( ai_stLogonServer ) ;
	gstring	tmpSSL( ai_stSSL ) ;
	
	if( ai_stCredential != NULL && tmpServername != NULL && tmpSSL != NULL )
	{
		*ao_RetVal = m_poDM->GetLogonAdaptor()->RestoreLogon
			(
			tmpCredential,
			ai_iLanguage,
			tmpServername,
			ai_iPortNumber,
			tmpSSL,
			( ai_bUseSSL == VARIANT_TRUE )
			) ;
	}
	else
	{
		FormatError( M4_CH_WO_CANT_CREATE_STRING, "Cannot create string" ) ;
	}
	
	LEAVE_METHOD( CWrapCVM::RestoreLogon, S_OK ) ;
}



STDMETHODIMP CWrapCVM::GetChannelCount(/*[out, retval] */long *ao_iCount)
{
	ENTER_METHOD( CWrapCVM::GetChannelCount ) ;
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetChannelCount, E_UNEXPECTED);
	}
	
	*ao_iCount=m_poDM->Channel.Count();
	
	LEAVE_METHOD( CWrapCVM::GetChannelCount, S_OK ) ;
}

STDMETHODIMP CWrapCVM::GetChannelByPosition(/*[in]*/ long ai_iPosition, /*[out, retval]*/ IWrapChannel** ao_channel)
{
	ENTER_METHOD(CWrapCVM::GetChannelByPosition);
	
	ClChannel* pChannel;
	m4return_t ret;
	
	if (!m_poDM){
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::GetChannelByPosition, E_UNEXPECTED);
	}
	
	pChannel = m_poDM->Channel[ai_iPosition];
	
	if (pChannel)
	{
#ifdef M4_NEWINTERFACE
		ret = NewInterface(CLSID_WrapChannel, IID_IWrapChannel,(IUnknown**)ao_channel);
#else
		HRESULT hRes = M4_CREATE_INTERFACE(CWrapChannel,ao_channel);
		ret = SUCCEEDED(hRes)? M4_SUCCESS : M4_ERROR;
#endif		
		if (ret != M4_SUCCESS){
			LEAVE_METHOD(CWrapCVM::GetChannelByPosition, E_UNEXPECTED);
		}
		
		((CWrapChannel*)(*ao_channel))->Init(pChannel);
	}
	else
	{
		FormatError(M4_CH_WO_CANT_CREATE_CHANNEL,"Could not create channel");
		
		LEAVE_METHOD(CWrapCVM::GetChannelByPosition, E_UNEXPECTED);
	}
	
	LEAVE_METHOD(CWrapCVM::GetChannelByPosition, S_OK);
}


STDMETHODIMP CWrapCVM::SentenceMerge(/*[in]*/BSTR ai_stChild, 
									 /*[in]*/BSTR ai_stParent, 
									 /*[out]*/BSTR *ao_stResult, 
									 /*[out, retval]*/short *ao_pReturn)
{
	ENTER_METHOD( CWrapCVM::SentenceMerge ) ;
	
	*ao_pReturn = M4_ERROR ;
	
	gstring	tmpChild (ai_stChild) ;
	gstring	tmpParent (ai_stParent) ;

	istrstream oChild (tmpChild);
	istrstream oParent (tmpParent);
	ostrstream oResult;

	M4HsParser oParser;

	*ao_pReturn = oParser.SentenceMerge (oChild, oParent, oResult);

	if (*ao_pReturn == M4_ERROR)
	{
		LEAVE_METHOD( CWrapCVM::SentenceMerge, S_OK ) ;
	}

	gstring tmpResult(oResult.str());
	*ao_stResult = tmpResult;
	
	LEAVE_METHOD( CWrapCVM::SentenceMerge, S_OK ) ;
}

STDMETHODIMP CWrapCVM::SetCallBack(long ai_fCallBack)
{
	ENTER_METHOD( CWrapCVM::SetCallBack ) ;

	/* Bug 0069900
	Hay que ver que no nos llamen cuando el dm no esté construido
	*/
	if( m_poDM != NULL )
	{
		m_poDM->SetCallBack( ai_fCallBack ) ;
	}

	LEAVE_METHOD( CWrapCVM::SetCallBack, S_OK ) ;
}


STDMETHODIMP CWrapCVM::get_SessionContext(BSTR *ao_sX509Certificate, BSTR *ao_sSessionContext, short *pVal)
{
	ENTER_METHOD( CWrapCVM::get_SessionContext ) ;

	m4return_t		iResult = M4_SUCCESS ;
	m4uint32_t		iCertificateLength = 0 ; 
	m4uint32_t		iContextLength = 0 ; 
	m4pchar_t		pcCertificate = NULL ;
	m4pchar_t		pcContext = NULL ;
	ClAppClientAdap	*poClientAdapter = NULL ;


	if( m_poObjService == NULL )
	{
		FormatError( M4_CH_WO_CVM_NOT_STARTED, "CVM is not started." ) ;
		*pVal = M4_ERROR ;
		LEAVE_METHOD( CWrapCVM::get_SessionContext, S_OK ) ;
	}

	poClientAdapter = m_poObjService->GetpAppClientAdap() ;

	if( poClientAdapter == NULL )
	{
		// Mensaje de error???
		*pVal = M4_ERROR ;
		LEAVE_METHOD( CWrapCVM::get_SessionContext, S_OK ) ;
	}

	iResult = poClientAdapter->GetSessionContext( pcCertificate, iCertificateLength, pcContext, iContextLength ) ;

	if( iResult != M4_SUCCESS )
	{
		// Mensaje de error???
		*pVal = M4_ERROR ;
		LEAVE_METHOD( CWrapCVM::get_SessionContext, S_OK ) ;
	}

	// UNICODE VB
	*ao_sX509Certificate = M4CppToVB( pcCertificate ) ;
	*ao_sSessionContext = M4CppToVB( pcContext ) ;

	delete[] pcCertificate ;
	delete[] pcContext ;

	if( ao_sX509Certificate == NULL || ao_sSessionContext == NULL )
	{
		// Mensaje de error???
		*pVal = M4_ERROR ;
		LEAVE_METHOD( CWrapCVM::get_SessionContext, S_OK ) ;
	}

	*pVal = M4_SUCCESS ;
	LEAVE_METHOD( CWrapCVM::get_SessionContext, S_OK ) ;
}

STDMETHODIMP CWrapCVM::GetpILicenseManager(long *ao_pILicenseManager, short *ao_RetVal)
{
	//Retornamos un puntero al licence manager.
	ENTER_METHOD(CWrapCVM::GetpILicenseManager);

	*ao_RetVal = M4_ERROR;
	*ao_pILicenseManager = (long) NULL;

	if (m_poObjService != NULL)
	{
		ILicenseManager * pILicenseManager = m_poObjService->GetpLicenseManager();

		if (pILicenseManager != NULL) {
			*ao_pILicenseManager = (long) pILicenseManager;
			*ao_RetVal = M4_SUCCESS;
		}
	}

	LEAVE_METHOD(CWrapCVM::GetpILicenseManager, S_OK);
}

STDMETHODIMP CWrapCVM::GetStateValidateExecutionNp(long ai_pILicenseManager, short *ao_State, short *ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::GetStateValidateExecutionNp);
	
	*ao_RetVal = M4_ERROR;	
	*ao_State = (short) STATE_VALIDATE_EXECUTION_NP_RED;

	ILicenseManager * pILicenseManager = (ILicenseManager *) ai_pILicenseManager;

	if (pILicenseManager != NULL) {
		eStateValidateExecutionNp_t eStateValidateExecutionNp = pILicenseManager->GetStateValidateExecutionNp();
		*ao_State = (short) eStateValidateExecutionNp;
		*ao_RetVal = M4_SUCCESS;
	}

	// De momento no se debe mostar ventanas aunque no se esté correcto
	*ao_State = (short) STATE_VALIDATE_EXECUTION_NP_GREEN;

	LEAVE_METHOD(CWrapCVM::GetStateValidateExecutionNp, S_OK);
}

STDMETHODIMP CWrapCVM::GetPropertyValueById(long ai_pILicenseManager, BSTR ai_pPropertyId, BSTR *ao_pPropertyValue, short *ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::GetPropertyValueById);	

	*ao_RetVal = M4_ERROR;	
	gstring	tmpPropertyId( ai_pPropertyId ) ;	

	ILicenseManager * pILicenseManager = (ILicenseManager *) ai_pILicenseManager;

	if (pILicenseManager != NULL)
	{
		m4pcchar_t pPropertyValue = pILicenseManager->GetPropertyValueById(tmpPropertyId);

		if (pPropertyValue != NULL)
		{
			//UNICODE VB
			*ao_pPropertyValue =  M4CppToVB( pPropertyValue ) ;
			*ao_RetVal = M4_SUCCESS;
		}
	}

	LEAVE_METHOD(CWrapCVM::GetPropertyValueById, S_OK);
}

STDMETHODIMP CWrapCVM::GetPropertyTransById(long ai_pILicenseManager, BSTR ai_pPropertyId, BSTR *ao_pPropertyTrans, short *ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::GetPropertyTransById);

	*ao_RetVal = M4_SUCCESS;
	*ao_pPropertyTrans = M4CppToVB( "" ) ;

	LEAVE_METHOD(CWrapCVM::GetPropertyTransById, S_OK);
}

STDMETHODIMP CWrapCVM::IsAllowed(long ai_pILicenseManager, BSTR ai_sMeta4Object, BSTR ai_sIdNodeStructure, BSTR ai_sIdItem, short *ao_bIsAllowed, short *ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::IsAllowed);

	*ao_RetVal = M4_SUCCESS;
	*ao_bIsAllowed = M4_TRUE;

	LEAVE_METHOD(CWrapCVM::IsAllowed, S_OK);
}

STDMETHODIMP CWrapCVM::SetDownloadCallBack(long ai_fCallBack)
{
	ENTER_METHOD(CWrapCVM::SetDownloadCallBack);

	M4ClExternObjectManager::SetCallBack( ai_fCallBack ) ;

	LEAVE_METHOD(CWrapCVM::SetDownloadCallBack, S_OK);
}

STDMETHODIMP CWrapCVM::CleanMapCache(short *ao_RetVal)
{
	ENTER_METHOD(CWrapCVM::CleanMapCache);
	
	if (!m_poDM){
		*ao_RetVal=M4_ERROR;
		FormatError(M4_CH_WO_CVM_NOT_STARTED,"CVM is not started.");
		LEAVE_METHOD(CWrapCVM::CleanCMCRCache, S_OK);
	}
	
	*ao_RetVal = m_poDM->GetCacheDirectory()->CleanCaches(M4_CACHE_MAP_CACHE_TYPE);
	
	LEAVE_METHOD(CWrapCVM::CleanMapCache, S_OK);
}

STDMETHODIMP CWrapCVM::GetMappingById(BSTR ai_pcMapId, BSTR ai_pcOrganization, BSTR ai_pcSource, BSTR *ao_pcMapping, short *ao_RetVal)
{
	ENTER_METHOD( CWrapCVM::GetMappingById ) ;
	
	*ao_RetVal = M4_SUCCESS ;
	
	gstring	tmpMapId( ai_pcMapId ) ;
	gstring	tmpOrganization( ai_pcOrganization ) ;
	gstring	tmpSource( ai_pcSource ) ;

	m4pchar_t pcMappingId = m_poObjService->GetpCMCRFactory()->MDFac_GetMappingById( tmpMapId, tmpOrganization, tmpSource, M4_TRUE ) ;

	gstring	tmpOutString( pcMappingId ) ;

	if( pcMappingId != NULL )
	{
		delete [] pcMappingId ;
	}
	
	*ao_pcMapping = (BSTR) tmpOutString ;

	LEAVE_METHOD( CWrapCVM::GetMappingById, S_OK ) ;
}

STDMETHODIMP CWrapCVM::IsEqualOrg(BSTR ai_pcMapId, BSTR ai_pcOrganization1, BSTR ai_pcOrganization2, BSTR ai_pcDestination, VARIANT_BOOL *ao_bIsEquivalent, short *ao_RetVal)
{
	ENTER_METHOD( CWrapCVM::IsEqualOrg ) ;
	
	*ao_RetVal = M4_SUCCESS ;
	*ao_bIsEquivalent = VARIANT_FALSE ;

	gstring	tmpMapId( ai_pcMapId ) ;
	gstring	tmpOrganization1( ai_pcOrganization1 ) ;
	gstring	tmpOrganization2( ai_pcOrganization2 ) ;
	gstring	tmpDestination( ai_pcDestination ) ;

	m4bool_t bIsEqual = M4_FALSE ;
	m4return_t iResult = m_poObjService->GetpCMCRFactory()->MDFac_IsEqualOrg( tmpMapId, tmpOrganization1, tmpOrganization2, tmpDestination, M4_TRUE, bIsEqual ) ;

	if( iResult != M4_SUCCESS )
	{
		*ao_RetVal = M4_ERROR ;
	}
	else
	{
		*ao_RetVal = M4_SUCCESS ;

		if( bIsEqual == M4_TRUE )
		{
			*ao_bIsEquivalent = VARIANT_TRUE ;
		}
		else
		{
			*ao_bIsEquivalent = VARIANT_FALSE ;
		}
	}

	LEAVE_METHOD( CWrapCVM::IsEqualOrg, S_OK ) ;
}

STDMETHODIMP CWrapCVM::PrintVBTrace(SAFEARRAY **ai_saArguments, short *ao_RetVal)
{
	HRESULT hresult;
	BSTR VVar;
	m4bool_t bOK = M4_TRUE;
	m4return_t iRet = M4_SUCCESS;
	gstring * pArguments;
	m4uint32_t i,j;
	
	
	ENTER_METHOD(CWrapCVM::PrintVBTrace);
	
	if ((*ai_saArguments)->cDims != 1)
	{
		*ao_RetVal = M4_ERROR;
		LEAVE_METHOD(CWrapCVM::PrintVBTrace, S_OK);
	}	
	
	m4uint32_t iSize = (*ai_saArguments)->rgsabound[0].cElements;
	m4uint32_t iLBound = (*ai_saArguments)->rgsabound[0].lLbound;
	m4uint32_t iUBound = iLBound + iSize;
	
	pArguments = new gstring[iSize];
	if (pArguments == NULL)
	{
		*ao_RetVal = M4_ERROR;
		LEAVE_METHOD(CWrapCVM::PrintVBTrace, S_OK);
	}
	
	for (j = 0, i = iLBound; i < iUBound; i++, j++)
	{
		hresult = SafeArrayGetElement(*ai_saArguments, (m4int32_t *) &i, &VVar);
		if (FAILED(hresult))
		{
			bOK = M4_FALSE;
			break;
		}
		
		pArguments[j] = VVar;
	}
	
	if (bOK == M4_TRUE)
	{
		// Establecer argumentos de entrada para imprimir traza de usuario.
		// El orden de los argumentos está predeterminado y es el siguiente:
		// SERVICE, OBJECT, FUNCTION, ACTION, INIT_TIME, END_TIME, ELAPSED.
		string sService, sObject, sFunction, sAction, sInitTime, sEndTime, sElapsed;
		sService = pArguments[0];
		sObject = pArguments[1];
		sFunction = pArguments[2];
		sAction = pArguments[3];
		sInitTime = pArguments[4];
		sEndTime = pArguments[5];
		sElapsed = pArguments[6];

		// Role.
		m4pchar_t pcRole = m_poDM->GetLogonAdaptor()->GetRole() ;

		// Imprimir traza de usuario.
		m_poObjService->GetpCsTrace()->PrintVBTrace(sService.c_str(), sObject.c_str(), sFunction.c_str(), sAction.c_str(), pcRole, sInitTime.c_str(), sEndTime.c_str(), sElapsed.c_str());
	}
	
	// Liberar array.
	delete [] pArguments;
	
	*ao_RetVal = M4_SUCCESS;
	LEAVE_METHOD(CWrapCVM::PrintVBTrace, S_OK);
}

STDMETHODIMP CWrapCVM::LogCount(long *ao_plCount)
{
	// Pos si se han descuadrado las excepciones se van reseteando
	M4ResetExceptions() ;

	ENTER_METHOD( CWrapCVM::LogCount ) ;

	*ao_plCount = GET_ERROR_QUEUE_SIZE() ;

	LEAVE_METHOD( CWrapCVM::LogCount, S_OK ) ;
}

STDMETHODIMP CWrapCVM::LogGet(long ai_lLanguage, long ai_lPosition, long *ao_plCode, short *ao_piType, DATE *ao_pdTime, BSTR *ao_psTextById, BSTR *ao_psTextByName, short *ao_iRetVal)
{
	ENTER_METHOD( CWrapCVM::LogGet ) ;

	m4return_t		iResult = M4_SUCCESS ;
	m4bool_t		bModified = M4_FALSE ;
	m4pchar_t		pcReplaced = NULL ;
	m4date_t		dDate = 0 ;
	m4int32_t		iCode = 0 ;
	m4int32_t		iLength = 0 ;
	m4int32_t		iCopied = 0 ;
	m4uint32_t		iNeeded = 0 ;
	m4language_t	iLanguage = 0 ;
	m4pchar_t		pccBuffer = NULL ;
	const m4int32_t	iSize = 2000 ;
	m4char_t		acBuffer[ iSize + 4 + 1 ] ;


	*ao_iRetVal = M4_ERROR ;
	*ao_plCode = 0 ;
	*ao_piType = 0 ;
	*ao_pdTime = 0 ;

	if( *ao_psTextById != NULL )
	{
		SysFreeString( *ao_psTextById ) ;
		*ao_psTextById = NULL ;
	}

	if( *ao_psTextByName != NULL )
	{
		SysFreeString( *ao_psTextByName ) ;
		*ao_psTextByName = NULL ;
	}

	iResult = GETH_DECIMAL_N_ERRORCODE( ai_lPosition, iCode ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
	}

	*ao_plCode = iCode ;


	iResult = GETH_N_ERRORTYPE( ai_lPosition, acBuffer, iSize, iCopied ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
	}

	if( strcmp( acBuffer, "_error_" ) == 0 )
	{
		*ao_piType = ERRORLOG ;
	}
	else if( strcmp( acBuffer, "_warning_" ) == 0 )
	{
		*ao_piType = WARNINGLOG ;
	}
	else
	{
		*ao_piType = DEBUGINFOLOG ;
	}


	iResult = GETH_N_ERRORTIME( ai_lPosition, acBuffer, iSize, iCopied ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
	}

	dDate = ClStringToDate( acBuffer ) ;
	*ao_pdTime = M4dateToDate( dDate ) ;

	iResult = GETH_N_ERRORTEXT( ai_lPosition, acBuffer, iSize, iCopied ) ;

	if( iResult != M4_SUCCESS )
	{
		// Puede ser que el error sea por tamaño
		// LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;

		if( iCopied == iSize - 1 )
		{
			// Bug 0289889 Si se ha cortado ponemos el # final
			strcat( acBuffer, "...#" ) ;
		}
	}

	iLanguage = ai_lLanguage ;

	iResult = M4GetErrorString( iCode, acBuffer, 0, m_poDM, NULL, iNeeded, iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
	}

	pccBuffer = new m4char_t[ iNeeded ] ;

	iResult = M4GetErrorString( iCode, acBuffer, 0, m_poDM, pccBuffer, iNeeded, iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		delete [] pccBuffer ;
		LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
	}

	iResult = M4ReplaceLiterals( m_poDM, pccBuffer, pcReplaced, NULL, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcReplaced != NULL )
	{
		delete [] pccBuffer ;
		pccBuffer = pcReplaced;
	}

	// UNICODE VB
	*ao_psTextById = M4CppToVB( pccBuffer ) ;
	delete [] pccBuffer ;
	pccBuffer = NULL ;


	iResult = M4GetErrorString( iCode, acBuffer, 1, m_poDM, NULL, iNeeded, iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
	}

	pccBuffer = new m4char_t[ iNeeded ] ;

	iResult = M4GetErrorString( iCode, acBuffer, 1, m_poDM, pccBuffer, iNeeded, iLanguage ) ;

	if( iResult != M4_SUCCESS )
	{
		delete [] pccBuffer ;
		LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
	}

	iResult = M4ReplaceLiterals( m_poDM, pccBuffer, pcReplaced, NULL, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcReplaced != NULL )
	{
		delete [] pccBuffer ;
		pccBuffer = pcReplaced;
	}

	// UNICODE VB
	*ao_psTextByName = M4CppToVB( pccBuffer ) ;
	delete [] pccBuffer ;
	pccBuffer = NULL ;

	
	*ao_iRetVal = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::LogGet, S_OK ) ;
}

STDMETHODIMP CWrapCVM::LogClean()
{
	ENTER_METHOD( CWrapCVM::LogClean ) ;

	CLEAN();

	LEAVE_METHOD( CWrapCVM::LogClean, S_OK ) ;
}

STDMETHODIMP CWrapCVM::get_BuildVersion(BSTR* pVal)
{
	ENTER_METHOD(CWrapCVM::get_BuildVersion);

	m4char_t acBV[ 200 + 1 ] ;

	if ( M4_ERROR == GetBuildVersion (acBV) )
	{
		LEAVE_METHOD(CWrapCVM::get_BuildVersion, S_FALSE);
	}

	gstring tmpBV(acBV);
			
	*pVal = tmpBV;

	TRACE2("Build version returned %s", *pVal);
	LEAVE_METHOD(CWrapCVM::get_BuildVersion, S_OK);
}

STDMETHODIMP CWrapCVM::BuildXMLFromId(BSTR ai_sM4ObjId, long ai_lMask, short ai_CsType, short ai_iLanguage, SAFEARRAY** ao_psaXML, short *ao_iRetVal)
{
	ENTER_METHOD( CWrapCVM::BuildXMLFromId ) ;

	DUMP_CHLOG_ERRORF( M4_CH_WO_NOT_SUPPORTED_FUNCTION, "CWrapCVM::BuildXMLFromId" << "M4ChWrap.dll" );

	LEAVE_METHOD( CWrapCVM::BuildXMLFromId, S_OK ) ;
}

STDMETHODIMP CWrapCVM::GetRegistryValue(BSTR ai_sKey, BSTR ai_sValue, BSTR *ai_psData, short *ao_piReturn)
{
	ENTER_METHOD( CWrapCVM::GetRegistryValue ) ;

	m4return_t	iResult = M4_ERROR ;
	m4pchar_t	pcData = NULL ;
	gstring		pszKey( ai_sKey ) ;
	gstring		pszValue( ai_sValue ) ;


	*ao_piReturn = M4_ERROR ;

	if( *ai_psData != NULL )
	{
		SysFreeString( *ai_psData ) ;
		*ai_psData = NULL ;
	}

	iResult = M4XmlRegistry::Instance()->GetValue( pszKey, pszValue, pcData ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::GetRegistryValue, S_OK ) ;
	}

	*ai_psData = M4CppToVB( pcData ) ;
	delete [] pcData ;

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::GetRegistryValue, S_OK ) ;
}

STDMETHODIMP CWrapCVM::SetRegistryValue(BSTR ai_sKey, BSTR ai_sValue, BSTR ai_sData, short *ao_piReturn)
{
	ENTER_METHOD( CWrapCVM::SetRegistryValue ) ;

	m4return_t	iResult = M4_ERROR ;
	gstring		pszKey( ai_sKey ) ;
	gstring		pszValue( ai_sValue ) ;
	gstring		pszData( ai_sData ) ;


	*ao_piReturn = M4_ERROR ;

	iResult = M4XmlRegistry::Instance()->SetValue( pszKey, pszValue, pszData ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::SetRegistryValue, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::SetRegistryValue, S_OK ) ;
}

STDMETHODIMP CWrapCVM::GetRegistryValueNames(BSTR ai_sKey, SAFEARRAY **ao_psaValues, short *ao_piReturn)
{
	ENTER_METHOD( CWrapCVM::GetRegistryValueNames ) ;

	HRESULT			hResult = S_OK ;
	m4return_t		iResult = M4_ERROR ;
	size_t			i = 0 ;
	size_t			iLength = 0 ;
	m4pchar_t		pccValueName = NULL ;
	BSTR			*psValues = NULL ;
	gstring			pszKey( ai_sKey ) ;
	list<m4pchar_t>	oValues ;

	list<m4pchar_t>::iterator	itIterator ;


	*ao_piReturn = M4_ERROR ;

	if( *ao_psaValues != NULL )
	{
		SafeArrayDestroy( *ao_psaValues ) ;
		*ao_psaValues = NULL ;
	}

	iResult = M4XmlRegistry::Instance()->GetValueNames( pszKey, oValues ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::GetRegistryValueNames, S_OK ) ;
	}

	iLength = oValues.size() ;

	*ao_psaValues = SafeArrayCreateVector( VT_BSTR, 0, iLength ) ;

	if( *ao_psaValues == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
		LEAVE_METHOD( CWrapCVM::GetRegistryValueNames, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_psaValues, (void**) &psValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_psaValues ) ;
		*ao_psaValues = NULL ;
		LEAVE_METHOD( CWrapCVM::GetRegistryValueNames, S_OK ) ;
	}

	i = 0 ;
	itIterator = oValues.begin() ;

	while( itIterator != oValues.end() )
	{
		pccValueName = *itIterator ;
		psValues[ i++ ] = M4CppToVB( pccValueName ) ;
		delete [] pccValueName ;
		itIterator++ ;
	}
	oValues.clear() ;

	hResult = SafeArrayUnaccessData( *ao_psaValues ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
		SafeArrayDestroy( *ao_psaValues ) ;
		*ao_psaValues = NULL ;
		LEAVE_METHOD( CWrapCVM::GetRegistryValueNames, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::GetRegistryValueNames, S_OK ) ;
}

STDMETHODIMP CWrapCVM::GetRegistrySubKeyNames(BSTR ai_sKey, SAFEARRAY **ao_psaKeys, short *ao_piReturn)
{
	ENTER_METHOD( CWrapCVM::GetRegistrySubKeyNames ) ;

	HRESULT			hResult = S_OK ;
	m4return_t		iResult = M4_ERROR ;
	size_t			i = 0 ;
	size_t			iLength = 0 ;
	m4pchar_t		pccKeyName = NULL ;
	BSTR			*psKeys = NULL ;
	gstring			pszKey( ai_sKey ) ;
	list<m4pchar_t>	oKeys ;

	list<m4pchar_t>::iterator	itIterator ;


	*ao_piReturn = M4_ERROR ;

	if( *ao_psaKeys != NULL )
	{
		SafeArrayDestroy( *ao_psaKeys ) ;
		*ao_psaKeys = NULL ;
	}

	iResult = M4XmlRegistry::Instance()->GetSubKeyNames( pszKey, oKeys ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::GetRegistrySubKeyNames, S_OK ) ;
	}

	iLength = oKeys.size() ;

	*ao_psaKeys = SafeArrayCreateVector( VT_BSTR, 0, iLength ) ;

	if( *ao_psaKeys == NULL )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot create SafeArray" ) ;
		LEAVE_METHOD( CWrapCVM::GetRegistrySubKeyNames, S_OK ) ;
	}

	hResult = SafeArrayAccessData( *ao_psaKeys, (void**) &psKeys ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot access SafeArray" ) ;
		SafeArrayDestroy( *ao_psaKeys ) ;
		*ao_psaKeys = NULL ;
		LEAVE_METHOD( CWrapCVM::GetRegistrySubKeyNames, S_OK ) ;
	}

	i = 0 ;
	itIterator = oKeys.begin() ;

	while( itIterator != oKeys.end() )
	{
		pccKeyName = *itIterator ;
		psKeys[ i++ ] = M4CppToVB( pccKeyName ) ;
		delete [] pccKeyName ;
		itIterator++ ;
	}
	oKeys.clear() ;

	hResult = SafeArrayUnaccessData( *ao_psaKeys ) ;

	if( hResult != S_OK )
	{
		FormatError( M4_CH_WO_CANT_CREATE_OBJECT, "Cannot unaccess SafeArray" ) ;
		SafeArrayDestroy( *ao_psaKeys ) ;
		*ao_psaKeys = NULL ;
		LEAVE_METHOD( CWrapCVM::GetRegistrySubKeyNames, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::GetRegistrySubKeyNames, S_OK ) ;
}

STDMETHODIMP CWrapCVM::SaveRegistry(short *ao_piReturn)
{
	ENTER_METHOD( CWrapCVM::SaveRegistry ) ;

	m4return_t	iResult = M4_ERROR ;


	*ao_piReturn = M4_ERROR ;

	iResult = M4XmlRegistry::Instance()->Save( NULL ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::SaveRegistry, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::SaveRegistry, S_OK ) ;
}


STDMETHODIMP CWrapCVM::DeleteRegistryKey(BSTR ai_sKey, short *ao_piReturn)
{

	ENTER_METHOD( CWrapCVM::DeleteRegistryKey ) ;

	m4return_t	iResult = M4_ERROR ;
	gstring		pszKey( ai_sKey ) ;


	*ao_piReturn = M4_ERROR ;

	iResult = M4XmlRegistry::Instance()->DeleteKey( pszKey ) ;

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::DeleteRegistryKey, S_OK ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::DeleteRegistryKey, S_OK ) ;
}

STDMETHODIMP CWrapCVM::ReplaceLiterals(BSTR ai_sInput, BSTR *ai_psOutput, short *ao_piReturn)
{
	ENTER_METHOD( CWrapCVM::ReplaceLiterals ) ;

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bModified = M4_FALSE ;
	m4pchar_t	pcOutput = NULL ;
	gstring		pszInput( ai_sInput ) ;


	*ao_piReturn = M4_ERROR ;

	if( *ai_psOutput != NULL )
	{
		SysFreeString( *ai_psOutput ) ;
		*ai_psOutput = NULL ;
	}

	iResult = M4ReplaceLiterals( m_poDM, pszInput, pcOutput, NULL, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
	{
		*ai_psOutput = M4CppToVB( pcOutput ) ;
		delete [] pcOutput ;
	}
	else
	{
		*ai_psOutput = M4CppToVB( pszInput ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::ReplaceLiterals, S_OK ) ;
}


STDMETHODIMP CWrapCVM::ReplaceOrgLiterals(BSTR ai_sInput, BSTR *ai_psOutput, BSTR ai_sOrganization, short *ao_piReturn)
{
	ENTER_METHOD( CWrapCVM::ReplaceOrgLiterals ) ;

	m4return_t	iResult = M4_ERROR ;
	m4bool_t	bModified = M4_FALSE ;
	m4pchar_t	pcOutput = NULL ;
	gstring		pszInput( ai_sInput ) ;
	gstring		pszOrganization( ai_sOrganization ) ;


	*ao_piReturn = M4_ERROR ;

	if( *ai_psOutput != NULL )
	{
		SysFreeString( *ai_psOutput ) ;
		*ai_psOutput = NULL ;
	}

	iResult = M4ReplaceLiterals( m_poDM, pszInput, pcOutput, pszOrganization, bModified ) ;

	if( iResult == M4_SUCCESS && bModified == M4_TRUE && pcOutput != NULL )
	{
		*ai_psOutput = M4CppToVB( pcOutput ) ;
		delete [] pcOutput ;
	}
	else
	{
		*ai_psOutput = M4CppToVB( pszInput ) ;
	}

	*ao_piReturn = M4_SUCCESS ;

	LEAVE_METHOD( CWrapCVM::ReplaceOrgLiterals, S_OK ) ;
}


STDMETHODIMP CWrapCVM::get_PtrFullInterface(long* retVal)
{
	ENTER_METHOD(CWrapCVM::get_PtrFullInterface);

	*retVal = (long)this;

	LEAVE_METHOD( CWrapCVM::get_PtrFullInterface, S_OK ) ;
}

STDMETHODIMP CWrapCVM::get_SessionTimeOut(double *pVal)
{
	ENTER_METHOD(CWrapCVM::get_SessionTimeOut);

	*pVal = 0;

	if( m_poDM != NULL )
	{
		ClLogonAdaptor	*poLogonAdaptor = m_poDM->GetLogonAdaptor() ;

		if( poLogonAdaptor != NULL )
		{
			*pVal = poLogonAdaptor->GetSessionTimeout() ;
		}
	}
	
	LEAVE_METHOD(CWrapCVM::get_SessionTimeOut, S_OK);
}

STDMETHODIMP CWrapCVM::AddUserAction(BSTR ai_sEvent, BSTR ai_sObjectId, BSTR ai_sObjectName, BSTR ai_sArguments, BSTR ai_sOrganization, BSTR ai_sRole, short *ao_piReturn)
{
	ENTER_METHOD(CWrapCVM::AddUserAction);

	m4return_t	iResult = M4_ERROR ;

	*ao_piReturn = M4_ERROR ;

	if( m_poDM == NULL )
	{
		LEAVE_METHOD( CWrapCVM::AddUserAction, S_OK ) ;
	}

	gstring	sEvent( ai_sEvent ) ;

	if( !strcmpi( sEvent, "TASK" ) )
	{
		gstring	sObjectId( ai_sObjectId ) ;
		gstring	sObjectName( ai_sObjectName ) ;
		gstring	sArguments( ai_sArguments ) ;
		gstring	sOrganization( ai_sOrganization ) ;
		gstring sRole( ai_sRole ) ;

		m_poDM->UserActionsAudit.AddAction( sOrganization, sRole, ClUserAction::EV_TASK, sObjectId, sObjectName, sArguments, M4_FALSE ) ;

		*ao_piReturn = M4_SUCCESS ;
	}

	LEAVE_METHOD(CWrapCVM::AddUserAction, S_OK);
}


STDMETHODIMP CWrapCVM::ArrayToPropertyBag(BSTR ai_sOrganization, SAFEARRAY **ai_abArray, ICPropBag **ao_ppoPropertyBag)
{
	ENTER_METHOD(CWrapCVM::ArrayToPropertyBag);

	HRESULT		hResult = S_OK ;
	m4pchar_t	pcBuffer = NULL ;
	gstring		pszOrganization( ai_sOrganization ) ;


	*ao_ppoPropertyBag = NULL ;

	hResult = SafeArrayAccessData( *ai_abArray, (void**)&pcBuffer) ;

	if( SUCCEEDED(hResult) )
	{
		*ao_ppoPropertyBag = ConstructPBFromASN1( (unsigned char*)pcBuffer, pszOrganization, m_poDM ) ;
	
		hResult = SafeArrayUnaccessData( *ai_abArray ) ;
	}

	LEAVE_METHOD(CWrapCVM::ArrayToPropertyBag, S_OK);
}

STDMETHODIMP CWrapCVM::get_CacheDirectory(BSTR *ao_sDirectory)
{
	ENTER_METHOD(CWrapCVM::get_CacheDirectory);	

	*ao_sDirectory = M4CppToVB( m_poDM->GetEnvironment()->GetCacheDirectory() ) ;

	LEAVE_METHOD(CWrapCVM::get_CacheDirectory, S_OK);
}


STDMETHODIMP CWrapCVM::ReportViewerInit(BSTR ai_pcCommand, short ai_iLinkManager, short ai_iLinkDesign, short ai_iLinkData, IWrapAccess* ai_poDesignAccess, IWrapAccess* ai_poDataAccess, unsigned long *ao_plTaskId, BSTR *ao_pcOutput, short *ao_plErrors, short *ao_plWarnings, unsigned long *ao_plBreaks, short *ao_piMoreBreaks, short *ao_iRetVal)
{
	ENTER_METHOD(CWrapCVM::ReportViewerInit);

	m4int16_t		iResult = 0 ;
	m4int16_t		iLength = 0 ;
	m4int32_t		iDataHandle = 0 ;
	m4int32_t		iDesignHandle = 0 ;
	m4char_t		acBuffer[ MAX_PATH + 1 ] = "" ;
	M4ClLoaDll		*poEngine = NULL ;
	m4int16_t		(*poViewerInitReport)( m4uint32_t* , m4char_t*, m4int16_t, m4int16_t, m4int16_t, m4uint32_t, m4uint32_t, m4char_t*, m4int16_t*, m4int16_t*, m4int16_t*,	m4uint32_t*, m4int16_t* ) = NULL ;
	m4typefunreturn	poFunction = NULL ;

	*ao_iRetVal = M4_ERROR ;

	if( *ao_pcOutput != NULL )
	{
		SysFreeString( *ao_pcOutput ) ;
		*ao_pcOutput = NULL ;
	}

	poEngine = GetEngine() ;

	if( poEngine == NULL )
	{
		LEAVE_METHOD( CWrapCVM::ReportViewerInit, S_OK ) ;
	}

	if( poEngine->GetFunc( "ViewerInitReport", poFunction ) != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::ReportViewerInit, S_OK ) ;
	}

	poViewerInitReport = ( m4int16_t (*)( m4uint32_t* , m4char_t*, m4int16_t, m4int16_t, m4int16_t, m4uint32_t, m4uint32_t, m4char_t*, m4int16_t*, m4int16_t*, m4int16_t*,	m4uint32_t*, m4int16_t* ) ) poFunction ;

	if( ai_poDesignAccess != NULL )
	{
		ai_poDesignAccess->get_Handle( &iDesignHandle ) ;
	}

	if( ai_poDataAccess != NULL )
	{
		ai_poDataAccess->get_Handle( &iDataHandle ) ;
	}

	gstring	pcCommand( ai_pcCommand ) ;
	iResult = (*poViewerInitReport)( ao_plTaskId, pcCommand, ai_iLinkManager, ai_iLinkDesign, ai_iLinkData, iDesignHandle, iDataHandle, acBuffer, &iLength, ao_plErrors, ao_plWarnings, ao_plBreaks, ao_piMoreBreaks ) ;

	if( iLength > 0 && *acBuffer != '\0' )
	{
		*ao_pcOutput = M4CppToVB( acBuffer ) ;
	}

	if( iResult == 1 )
	{
		*ao_iRetVal = M4_SUCCESS ;
	}

	LEAVE_METHOD(CWrapCVM::ReportViewerInit, S_OK);
}


STDMETHODIMP CWrapCVM::ReportViewerEnd(unsigned long ai_lTaskId, BSTR *ao_pcTrace, short *ao_plErrors, short *ao_plWarnings, short *ao_iRetVal)
{
	ENTER_METHOD(CWrapCVM::ReportViewerEnd);

	m4int16_t		iResult = 0 ;
	m4int16_t		iLength = 0 ;
	m4char_t		acBuffer[ MAX_PATH + 1 ] = "" ;
	M4ClLoaDll		*poEngine = NULL ;
	m4int16_t		(*poViewerEnd)( m4uint32_t, m4int16_t*, m4int16_t*, m4char_t*, m4int16_t* ) = NULL ;
	m4typefunreturn	poFunction = NULL ;


	*ao_iRetVal = M4_ERROR ;

	if( *ao_pcTrace != NULL )
	{
		SysFreeString( *ao_pcTrace ) ;
		*ao_pcTrace = NULL ;
	}

	poEngine = GetEngine() ;

	if( poEngine == NULL )
	{
		LEAVE_METHOD( CWrapCVM::ReportViewerEnd, S_OK ) ;
	}

	if( poEngine->GetFunc( "ViewerEnd", poFunction ) != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::ReportViewerEnd, S_OK ) ;
	}

	poViewerEnd = ( m4int16_t (*)( m4uint32_t, m4int16_t*, m4int16_t*, m4char_t*, m4int16_t* ) ) poFunction ;
	iResult = (*poViewerEnd)( ai_lTaskId, ao_plErrors, ao_plWarnings, acBuffer, &iLength ) ;

	if( iLength > 0 && *acBuffer != '\0' )
	{
		*ao_pcTrace = M4CppToVB( acBuffer ) ;
	}

	if( iResult == 1 )
	{
		*ao_iRetVal = M4_SUCCESS ;
	}

	LEAVE_METHOD(CWrapCVM::ReportViewerEnd, S_OK);
}

STDMETHODIMP CWrapCVM::RD_OpenSession(short ai_iVersion, BSTR ai_pcCommand, short ai_iLinkManager, IWrapAccess * ai_poAccess, long * ao_plSessionHandler, short * ao_iRetVal)
{
	ENTER_METHOD(CWrapCVM::RD_OpenSession);

	m4int16_t		iResult = 0;
	M4ClLoaDll		*poEngine = NULL;
	m4int16_t(*poRDOpenSession)(m4int16_t, m4char_t *, m4int16_t, ClAccess *, m4int32_t *) = NULL;
	m4typefunreturn	poFunction = NULL;
	ClAccess		* pAccess = NULL;
	
	*ao_iRetVal = 0;

	poEngine = GetEngine();

	if (poEngine == NULL)
	{
		LEAVE_METHOD(CWrapCVM::RD_OpenSession, S_OK);
	}

	if (poEngine->GetFunc("RD_OpenSession", poFunction) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapCVM::RD_OpenSession, S_OK);
	}

	poRDOpenSession = (m4int16_t(*)(m4int16_t, m4char_t *, m4int16_t, ClAccess *, m4int32_t *)) poFunction;
	
	((CWrapAccess*)ai_poAccess)->Unwrap(&pAccess);

	gstring	pcCommand(ai_pcCommand);
	iResult = (*poRDOpenSession)(ai_iVersion, pcCommand, ai_iLinkManager, pAccess, ao_plSessionHandler);
	
	if (iResult == 1)
	{
		*ao_iRetVal = 1;
	}

	LEAVE_METHOD(CWrapCVM::RD_OpenSession, S_OK);
}

STDMETHODIMP CWrapCVM::GetAccessByHandle(long ai_lHandle, IWrapAccess** ao_poAccess)
{
	ENTER_METHOD(CWrapCVM::GetAccessByHandle);

	ClAccess* poAccess = m_poDM->GetAccessByHandle( ai_lHandle );

	if (NewInterface(CLSID_WrapAccess, IID_IWrapAccess, (IUnknown**)ao_poAccess) != M4_SUCCESS)
	{
		LEAVE_METHOD(CWrapCVM::GetAccessByHandle, E_UNEXPECTED);
	}
	else
	{
		((CWrapAccess*)(*ao_poAccess))->Init(poAccess, M4_FALSE);
		LEAVE_METHOD(CWrapCVM::GetAccessByHandle, S_OK);
	}
}

STDMETHODIMP CWrapCVM::AddUserTimedAction(long ai_iEventNum, DATE aio_dtEventTime, BSTR ai_sOrganization, BSTR ai_sEventSenderContainer, BSTR ai_sEventSender, BSTR ai_sParentEventSenderContainer, BSTR ai_sParentEventSender, long ai_iTimeElapsed, BSTR ai_sBuffer, VARIANT_BOOL ai_bIsValid, short *ao_piReturn)
{
	ENTER_METHOD(CWrapCVM::AddUserTimedAction);

	m4return_t	iResult = M4_ERROR ;

	*ao_piReturn = M4_ERROR ;

	if( m_poDM == NULL )
	{
		LEAVE_METHOD( CWrapCVM::AddUserTimedAction, S_OK ) ;
	}

	m4date_t	dEventTime = DateToM4date( aio_dtEventTime ) ;
	gstring		sOrganization( ai_sOrganization ) ;
	gstring		sEventSenderContainer( ai_sEventSenderContainer ) ;
	gstring		sEventSender( ai_sEventSender ) ;
	gstring		sParentEventSenderContainer( ai_sParentEventSenderContainer ) ;
	gstring		sParentEventSender( ai_sParentEventSender ) ;
	gstring		sBuffer( ai_sBuffer ) ;
	m4bool_t	bIsValid = (ai_bIsValid == VARIANT_TRUE) ? M4_TRUE : M4_FALSE ;

	m_poDM->UserActionsAudit.AddTimedAction( ai_iEventNum, dEventTime, sOrganization, sEventSenderContainer, sEventSender, sParentEventSenderContainer, sParentEventSender, ai_iTimeElapsed, sBuffer, bIsValid ) ;

	*ao_piReturn = M4_SUCCESS ;
	

	LEAVE_METHOD(CWrapCVM::AddUserTimedAction, S_OK);
}

STDMETHODIMP CWrapCVM::GetEventsCounter(long *ao_plCounter)
{
	ENTER_METHOD(CWrapCVM::GetEventsCounter);

	*ao_plCounter = GetUserEventsCounter();

	LEAVE_METHOD(CWrapCVM::GetEventsCounter, S_OK);
}

STDMETHODIMP CWrapCVM::ReplaceCache(BSTR ai_stInstallHash, short *ao_piReturn)
{
	ENTER_METHOD(CWrapCVM::ReplaceCache);

	if( !m_poDM )
	{
		FormatError( M4_CH_WO_CVM_NOT_STARTED, "CVM is not started." );
		LEAVE_METHOD( CWrapCVM::ReplaceCache, E_UNEXPECTED );
	}

	m4return_t	iResult;
	
	*ao_piReturn = M4_ERROR;

	ClCacheReplacer	oReplacer( m_poDM );

	iResult = oReplacer.ReplaceCache();

	if( iResult != M4_SUCCESS )
	{
		LEAVE_METHOD( CWrapCVM::ReplaceCache, S_OK );
	}

	*ao_piReturn = M4_SUCCESS;

	LEAVE_METHOD(CWrapCVM::ReplaceCache, S_OK);
}


m4int_t	CWrapCVM::OnMessageBox( m4pcchar_t ai_pccTitle, m4pcchar_t ai_pccMessage, m4uint32_t ai_iType )
{

	INT		iResult = -1 ;
	BSTR	sTitle ;
	BSTR	sMessage ;


	sTitle = M4CppToVB( ai_pccTitle ) ;
	sMessage = M4CppToVB( ai_pccMessage ) ;

	Fire_MessageBox( sTitle, sMessage, ai_iType, &iResult ) ;

	if( sTitle != NULL )
	{
		SysFreeString( sTitle ) ;
		sTitle = NULL ;
	}

	if( sMessage != NULL )
	{
		SysFreeString( sMessage ) ;
		sMessage = NULL ;
	}

	return( iResult ) ;
}


m4int_t	CWrapCVM::OnCountdownForTimeOut( m4int_t ai_iSeconds )
{
	INT		iResult = -1 ;

	Fire_CountdownForTimeOut( ai_iSeconds, &iResult ) ;

	return( iResult ) ;
}


m4int_t	CWrapCVM::OnCppException( m4pcchar_t ai_pccDumpFile )
{

	INT		iResult = -1 ;
	BSTR	sDumpFile ;


	sDumpFile = M4CppToVB( ai_pccDumpFile ) ;

	Fire_CppException( sDumpFile, &iResult ) ;

	if( sDumpFile != NULL )
	{
		SysFreeString( sDumpFile ) ;
		sDumpFile = NULL ;
	}

	return( iResult ) ;
}


