//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             executor.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    contexto del ejecutor
//
//
//==============================================================================

#include "vmcscontext.hpp"
#include "vmres.hpp"
#include "vmtypes.hpp"
#include "executor.hpp"
#include "chlog.hpp"
#include "access.hpp"
#include "iexecadaptor.hpp"
#include "file_event.hpp"
#include "cllgadap.hpp"
#include "nodedef.hpp"
#include "m4unicode.hpp"
#include "clm4objservicecontext.hpp"
#include "m4servenv.hpp"



set<IM4MessageBoxProvider*, less<IM4MessageBoxProvider*> >	ClVMCSContext::sm_oMessageBoxProviders;


ClVMCSContext::ClVMCSContext(ClM4ObjServiceContext *ai_poObjContext ,ClVMStatisticsWrapper *ai_poSttWrap, ClLConn_Provider *ai_poConnProvider, IExecAdaptor* ai_poExecAdap, m4bool_t ai_bIsSM)
{
	m_poSttWrap		= ai_poSttWrap;
	m_poConnProvider= ai_poConnProvider;
	m_poExecAdap	= ai_poExecAdap;
	m_bIsSM			= ai_bIsSM;
	m_poObjContext  = ai_poObjContext;
}


ClVMCSContext::~ClVMCSContext()
{
}


void ClVMCSContext::RegisterMessageBoxProvider(IM4MessageBoxProvider *ai_poMessageBoxProvider)
{
	sm_oMessageBoxProviders.insert(ai_poMessageBoxProvider);
}


void ClVMCSContext::UnregisterMessageBoxProvider(IM4MessageBoxProvider *ai_poMessageBoxProvider)
{
	sm_oMessageBoxProviders.erase(ai_poMessageBoxProvider);
}


m4int_t	ClVMCSContext::_MessageBox( m4pcchar_t ai_pccTitle, m4pcchar_t ai_pccMessage, m4uint32_t ai_iType )
{

	m4int_t	iResult = -1 ;
	set<IM4MessageBoxProvider*, less<IM4MessageBoxProvider*> >::iterator	it ;


	it = sm_oMessageBoxProviders.begin() ;

	while( it != sm_oMessageBoxProviders.end() )
	{
		iResult = (*it)->OnMessageBox( ai_pccTitle, ai_pccMessage, ai_iType & 0xF ) ;

		if( iResult != -1 )
		{
			return( iResult ) ;
		}
		it++ ;
	}

#ifdef _WINDOWS
	int			iLength = -1 ;
	M4XMLCh		*sMessage = NULL ;
	M4XMLCh		*sTitle = NULL ;


	// UNICODE LN4
	iLength = -1 ;
	sMessage = M4CppToUtf16( ai_pccMessage, iLength ) ;
	iLength = -1 ;
	sTitle = M4CppToUtf16( ai_pccTitle, iLength ) ;

	iResult = MessageBoxW( NULL, (LPCWSTR)sMessage, (LPCWSTR)sTitle, ai_iType ) ;

	delete [] sMessage ;
	delete [] sTitle ;
#else
	iResult = -1;
#endif

	return( iResult ) ;
}


//---------------------------------------------
//Cliente
//---------------------------------------------


	//------------
	//PreBdlMethod
	//------------
m4return_t	ClVMCSContextClient::PreBDLMethod(ClAccess *ai_poAccess, m4uint32_t ai_hItem, const char* ai_pszFunctionName)
{
	
	ClCompiledMCR	*poCMCR = ai_poAccess->GetpCMCR() ;


	/* Bug 0080526
	Se afina más el mensaje
	Si es canal es separable es porque el método no es mapped
	Si el canal es no separable y no es sm es otro error
	*/
	if( poCMCR->GetChannelCsType() != M4CL_CSTYPE_BOTH )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_LDB_METHOD_IN_CLIENT_NO_MAPPED, ai_pszFunctionName << M4ObjNodeI( poCMCR, ai_hItem ) ) ;
		return( M4_ERROR ) ;
	}
	else if( m_bIsSM == M4_FALSE )
	{
		DUMP_CHLOG_ERRORF( M4_CH_VM_LDB_METHOD_IN_CLIENT_NO_SEPARABLE, ai_pszFunctionName << M4ObjNodeI( poCMCR, ai_hItem ) ) ;
		return( M4_ERROR ) ;
	}

	return( M4_SUCCESS ) ;
}

	//--------------
	//GetServerValue
	//--------------
	
m4return_t	ClVMCSContextClient::GetServerValue (char *ai_pszPath, char *ai_pszValue, char *ao_acValue, m4uint32_t ai_iBufferLength)
{
	if (ai_iBufferLength)
	{
		ao_acValue[0]=0;
		return M4_SUCCESS;
	}
	return M4_ERROR;
}

	//----------
	//Messagebox
	//----------


m4int_t	ClVMCSContextClient::Messagebox(const char * ai_pszListArg,const char * ai_pszTitle, m4uint32_t ai_iType, m4uint32_t ai_iError, m4int16_t ai_iErrorType)
{
	return( _MessageBox( ai_pszTitle, ai_pszListArg, ai_iType ) ) ;
}


	//----------
	//RemoteCall
	//----------
m4return_t	ClVMCSContextClient::RemoteCall(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet *ai_poRecordSet,m4bool_t ai_bEnableDebug)
{

	m4return_t iResult = M4_SUCCESS ;

	
	/*
	if( m_bIsSM == M4_FALSE )
	{
		if( ai_poRecordSet != NULL )
		{
			ClChannel	*poChannel = ai_poRecordSet->GetpChannel() ;

			if( poChannel != NULL )
			{
				ClChannelManager	*poChannelManager = poChannel->GetpChannelManager() ;

				if( poChannelManager != NULL )
				{
					ClChannel	*poSessionChannel = poChannelManager->GetSessionChannel() ;

					if( poSessionChannel != NULL )
					{
						m4bool_t	bConfirm = M4_FALSE ;

						if( poSessionChannel == poChannel )
						{
							if( strcmpi( ai_poRecordSet->GetpNodeDef()->Id(), "ROOT_SESSION" ) == 0 )
							{
								if( strcmpi( ai_poRecordSet->Item[ ClHandle( ai_hItem ) ].ItemDef.Id(), "CONFIRM" ) == 0 )
								{
									bConfirm = M4_TRUE ;

									if( m_iTimeStamp == -1 )
									{
										m_iTimeStamp = 0 ;
									}
								}
							}
						}

						if( bConfirm == M4_FALSE && m_iTimeStamp != -1 )
						{
							ClLogonAdaptor	*poLogonAdaptor = poChannelManager->GetLogonAdaptor() ;

							if( poLogonAdaptor != NULL )
							{
								if( m_iTimeStamp < poLogonAdaptor->GetTimeStamp() )
								{
									m_iTimeStamp = ClPrecisionTicker::GetTimeTick();
									poLogonAdaptor->Confirm() ;
								}
							}
						}
					}
				}
			}
		}
	}
	*/

	if (ai_bEnableDebug)
	{ //activamos el modo de debug
		iResult = ai_poRecordSet->GetpAccess()->GetpChannel()->SetDebugEnabled();
	}

	/* Bug 0069358
	Hay que bloquear la notificación de eventos mientras se está haciendo
	una llamada remota para que no se desincronice la serialización.
	*/
	ClFileEventBroker::Instance()->LockEvents();

	// Execute in remote...
	iResult = GetExecAdaptor()->RemoteCall(ai_poExecutor, ai_hItem, ai_poRecordSet);

	ClFileEventBroker::Instance()->UnlockEvents();

	if( m_iTimeStamp != -1 )
	{
		m_iTimeStamp = ClPrecisionTicker::GetTimeTick();
	}

	return( iResult ) ;
}

	//----------
	//NotifyWMExecution
	//----------
void ClVMCSContextClient::NotifyVMExecution(m4bool_t ai_bIsStart)
{
	m_poObjContext->NotifyVMExecution(ai_bIsStart);
}

//---------------------------------------------
//Server
//---------------------------------------------

	//--------------
	//GetServerValue
	//--------------

m4return_t	ClVMCSContextServer::GetServerValue (char *ai_pszPath, char *ai_pszValue, char *ao_acValue, m4uint32_t ai_iBufferLength){
	m4return_t ret;

	if (m_bIsSM)
	{
		if (ai_iBufferLength)
		{
			ao_acValue[0]=0;
			ret= M4_SUCCESS;
		}
		ret= M4_ERROR;
	}
	else
	{
		ret = GetEnviron( ai_pszPath, ai_pszValue, ao_acValue, ai_iBufferLength ) ;
		
		if( ret != M4_SUCCESS )
		{
			ao_acValue[0]=0;
		}
	}
	return ret;
}

	//----------
	//Messagebox
	//----------

m4int_t	ClVMCSContextServer::Messagebox(const char * ai_pszListArg,const char * ai_pszTitle, m4uint32_t ai_iType, m4uint32_t ai_iError, m4int16_t ai_iErrorType)
{
	if( m_bIsSM == M4_TRUE )
	{
		return( _MessageBox( ai_pszTitle, ai_pszListArg, ai_iType ) ) ;
	}

	BeginLog( g_oChLog, ai_iError, ai_iErrorType ) << ai_pszListArg << ai_pszTitle << EndLog;
	return IDOK;
}


	//----------
	//RemoteCall
	//----------
m4return_t	ClVMCSContextServer::RemoteCall(ClExecutor *ai_poExecutor, m4uint32_t ai_hItem, ClAccessRecordSet *ai_poRecordSet,m4bool_t ai_bEnableDebug)
{
	DUMP_CHLOG_ERRORF(M4_CH_VM_EXECUTING_REMOTE, ai_poExecutor->DumpCallStack() );
	return M4_ERROR;
}
