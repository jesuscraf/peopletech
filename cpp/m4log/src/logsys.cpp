//## begin module%377C88570153.cm preserve=no
//## end module%377C88570153.cm

//## begin module%377C88570153.cp preserve=no
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
//## end module%377C88570153.cp

//## Module: logsys%377C88570153; Package body
//## Subsystem: M4Log::src%377C88560044
//	d:\compon\m4log\version\src
//## Source file: D:\Work\v600\m4log\src\logsys.cpp

//## begin module%377C88570153.additionalIncludes preserve=no
//## end module%377C88570153.additionalIncludes

//## begin module%377C88570153.includes preserve=yes

//#include "logsys.hpp"
//#include "logmanag.hpp"
//#include "errortail.hpp"
//#include "m4date.hpp"


//## end module%377C88570153.includes

// logmanag
#include <logmanag.hpp>
// debuglog
#include <debuglog.hpp>
// perslog
#include <perslog.hpp>
// dbgresou
#include <dbgresou.hpp>
// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// syncro
#include <syncro.hpp>
//## begin module%377C88570153.declarations preserve=no
//## end module%377C88570153.declarations

//## begin module%377C88570153.additionalDeclarations preserve=yes





//## end module%377C88570153.additionalDeclarations


// Class ClLogSystem 




ClLogSystem::ClLogSystem ()
  //## begin ClLogSystem::ClLogSystem%857762628.hasinit preserve=no
      : m_IdThread(0)
  //## end ClLogSystem::ClLogSystem%857762628.hasinit
  //## begin ClLogSystem::ClLogSystem%857762628.initialization preserve=yes
	//	: m_lappoints(M4_FALSE) // POR DEFECTO NO SON MULTITHREAD
  //## end ClLogSystem::ClLogSystem%857762628.initialization
{
  //## begin ClLogSystem::ClLogSystem%857762628.body preserve=yes

	m_bactivated = M4_TRUE;
	m_ilevelInfo = ALL_STORAGES;
	m_plogManager = NULL;
	m_icode = 0;
	m_pappoint = NULL;
	m_plstorages = NULL;
	m_dUnitTime = 0;
	m_sTypeFlags = 15;
	m_sLocalQueue = M4_TRUE;
	m_creturn = '\n';
	m_cEndMessage = 0;
	m_sstorages=0;
	m_bshowTime = M4_FALSE;
	m_UserId="";
	m_bShared =  M4_FALSE;
	m_iOverwriteMask = M4LOG_OVER_NONE;
	m_poListener = NULL;

	m_pcServerName = NULL;
	ResetUserTimeAudit();

	if (m_bShared== M4_TRUE)
	{
		m_AccessMutex.Init();
	}

  //## end ClLogSystem::ClLogSystem%857762628.body
}

ClLogSystem::ClLogSystem (ClLogManager* plogManager, m4bool_t ai_bshowTime, m4bool_t ai_Shared)
  //## begin ClLogSystem::ClLogSystem%1779839184.hasinit preserve=no
      : m_IdThread(0)
  //## end ClLogSystem::ClLogSystem%1779839184.hasinit
  //## begin ClLogSystem::ClLogSystem%1779839184.initialization preserve=yes
	, m_lappoints(ai_Shared)
  //## end ClLogSystem::ClLogSystem%1779839184.initialization
{
  //## begin ClLogSystem::ClLogSystem%1779839184.body preserve=yes

	m_bShared =  ai_Shared;
	if (m_bShared== M4_TRUE) 
			m_AccessMutex.Init();

	m_sTypeFlags = 15;
	m_sstorages=0;
	m_bactivated = M4_TRUE;
	m_ilevelInfo =  ALL_STORAGES;
	m_plogManager = plogManager;
	m_icode = 0; 
	m_pappoint = NULL;
	m_bshowTime = ai_bshowTime;
	m_creturn = m_plogManager -> GetCReturn();
	m_cEndMessage = m_plogManager -> GetEndMessageChar();
	m_dUnitTime = 0;
	m_bStorageAlways = m_plogManager -> StorageAlways();
// incializo los medios de almacenamiento

	m_plstorages = m_plogManager -> GetStorageList();
	m_lappoints.SetStorageList( m_plstorages );

	m_lappoints.SetCReturn( m_creturn );
	m_lappoints.SetMaxQueueSize(m_plogManager -> GetMaxQueueSize());
	m_sLocalQueue = M4_TRUE;
	m_UserId="";
	ResetOverwriteMask( m_plogManager ) ;
	m_poListener = NULL;

	m_pcServerName = NULL;
	ResetUserTimeAudit();

  //## end ClLogSystem::ClLogSystem%1779839184.body
}


ClLogSystem::~ClLogSystem ()
{
  //## begin ClLogSystem::~ClLogSystem%163138753.body preserve=yes

	m_plogManager = NULL;
	if ( m_pappoint ) {
		delete m_pappoint;
		m_pappoint = NULL;
	};
  	
	if( m_pcServerName != NULL )
	{
		delete m_pcServerName;
	}
  //## end ClLogSystem::~ClLogSystem%163138753.body
}



//## Other Operations (implementation)
m4int16_t ClLogSystem::GetDistance (m4pchar_t& cad, m4uint32_t Size)
{
  //## begin ClLogSystem::GetDistance%1658248374.body preserve=yes


	m4uint32_t ret=0, cnt=0;


	while (1) {
	  if (cnt>Size) return -1;
	  if ((*(cad+cnt)==1)||(*(cad+cnt)==2)) {
		break;
	  } else {
        cnt++;
	    ret++;
	  };
    };
	
	return (m4int16_t) ret;


  //## end ClLogSystem::GetDistance%1658248374.body
}

m4return_t ClLogSystem::SetLevelInfo (m4uint32_t ilevelInfo)
{
  //## begin ClLogSystem::SetLevelInfo%1837704381.body preserve=yes

	m_ilevelInfo = ilevelInfo;
	return M4_SUCCESS;

  //## end ClLogSystem::SetLevelInfo%1837704381.body
}

void ClLogSystem::AddTime (m4double_t dTime)
{
  //## begin ClLogSystem::AddTime%721590391.body preserve=yes

	m_dUnitTime += dTime;

  //## end ClLogSystem::AddTime%721590391.body
}

m4return_t ClLogSystem::SetAttributeValue (const M4ClString& ai_attName, const M4ClString& ai_attValue)
{
  //## begin ClLogSystem::SetAttributeValue%817204118.body preserve=yes


	if (ai_attName=="M4_LOGSYSTEM_USERID") {
		m_UserId = ai_attValue;
		return M4_TRUE;
	}

	return M4_FALSE;


  //## end ClLogSystem::SetAttributeValue%817204118.body
}

M4ClString* ClLogSystem::GetAttributeValue (const M4ClString& ai_attName)
{
  //## begin ClLogSystem::GetAttributeValue%-1155364905.body preserve=yes

	if (ai_attName=="M4_LOGSYSTEM_USERID") {
		return &m_UserId;
	}

	return NULL;

  //## end ClLogSystem::GetAttributeValue%-1155364905.body
}

m4bool_t ClLogSystem::IsActive ()
{
  //## begin ClLogSystem::IsActive%-1455066329.body preserve=yes

	return m_bactivated;

  //## end ClLogSystem::IsActive%-1455066329.body
}

void ClLogSystem::SetActive ()
{
  //## begin ClLogSystem::SetActive%1242350385.body preserve=yes

	m_bactivated = M4_TRUE;

  //## end ClLogSystem::SetActive%1242350385.body
}

void ClLogSystem::SetDesactive ()
{
  //## begin ClLogSystem::SetDesactive%-1136887259.body preserve=yes

	m_bactivated = M4_FALSE;

  //## end ClLogSystem::SetDesactive%-1136887259.body
}

m4return_t ClLogSystem::StartCode (m4int32_t icode, m4int16_t irango)
{
  //## begin ClLogSystem::StartCode%-478318419.body preserve=yes

	m4uint32_t	ilevelInfo = 0;
	m4bool_t	btime = M4_FALSE;

	
	if ( ! m_bactivated )
		return M4_ERROR;

	if ( ! m_plogManager )
		return M4_ERROR;

	/* Bug 0108256
	Se tiene en cuenta lo que diga el nivel del logsys
	*/
	switch ( irango )
	{
	case WARNINGLOG:
		if ( ( m_iOverwriteMask & M4LOG_OVER_WARNING ) == 0 )
			return M4_ERROR;
		break;
	case DEBUGINFOLOG:
		if ( ( m_iOverwriteMask & M4LOG_OVER_DEBUG ) == 0 )
			return M4_ERROR;
		break;
	case ERRORLOG:
		if ( ( m_iOverwriteMask & M4LOG_OVER_ERROR ) == 0)
			return M4_ERROR;
		break;
	case TRACEINFOLOG:
		if ( ( m_iOverwriteMask & M4LOG_OVER_TRACE ) == 0 )
			return M4_ERROR;
		break;

	default:
		return M4_ERROR;
	}

	if ( ( m_plogManager -> GetModuleList() -> GetLevelInfo (this, irango, icode, ilevelInfo) ) != M4_SUCCESS)
		ilevelInfo = m_plogManager -> GetDefaultLevelInfo();
		
	m_icode = icode;
	ilevelInfo = m_ilevelInfo & ilevelInfo;
	btime = getShowTime();

	M4ClString	strType;
	switch ( irango )
	{
	case ERRORLOG:
		strType = "_error_";
		break;
	case WARNINGLOG:
		strType = "_warning_";
		break;
	case DEBUGINFOLOG:
		strType = "_debugInfo_";
		break;
	case TRACEINFOLOG:
		strType = "_traceInfo_";
		break;

	}

	if ( m_pappoint != NULL )
		delete m_pappoint;

	m_pappoint = new ClAppItem( m_icode,ilevelInfo, m_creturn, btime, strType );

	return M4_SUCCESS;

  //## end ClLogSystem::StartCode%-478318419.body
}

m4return_t ClLogSystem::AddText (const M4ClString& text)
{
  //## begin ClLogSystem::AddText%1816164496.body preserve=yes


	if ( ! m_bactivated )
		return M4_ERROR;

	if ( ! m_pappoint )
		return M4_ERROR;
	
	m_pappoint -> AddText( text );
	return M4_SUCCESS;

  //## end ClLogSystem::AddText%1816164496.body
}

m4int16_t ClLogSystem::SetCode ()
{
  //## begin ClLogSystem::SetCode%-1971197118.body preserve=yes

	if ( ! m_bactivated )
		return M4_ERROR;
	
	if ( ! m_pappoint )
		return M4_ERROR;
	
	if ( ! m_ilevelInfo )
	{
	
		m_pappoint -> ClearBuffer();
		return M4_ERROR;
	}

	if ( m_pappoint -> IsActive_Time() )
	{
		M4ClString stime = "";
		M4ClTimeStamp	clock;

		clock.now();
		clock.get( stime );
		m_pappoint -> SetTime( stime );
	}

	if ( ! m_icode )
	{

		// ERRORES INTERNOS DE LA M4LOG codigo 0 volcados directamente a defaultLogFile storage		
		m_plogManager->addInternalMessage(m_pappoint);
		delete m_pappoint;
		m_pappoint = NULL;
		return M4_SUCCESS;

	}


	m4int8_t auxFlags=0;
	/* Bug 0108256
	Se tiene en cuenta lo que diga el nivel del logsys
	*/
	if( m_iOverwriteMask & M4LOG_OVER_DEBUG && strcmpi( m_pappoint->GetErrType().c_str(), "_debugInfo_") == 0 )
	{
		auxFlags += 1;
	}

    if( m_iOverwriteMask & M4LOG_OVER_WARNING && strcmpi( m_pappoint->GetErrType().c_str(), "_warning_") == 0 )
	{
		auxFlags += 2;
	}

	if( m_iOverwriteMask & M4LOG_OVER_ERROR && strcmpi( m_pappoint->GetErrType().c_str(), "_error_") == 0 )
	{
		auxFlags += 4;
	}

	if( m_iOverwriteMask & M4LOG_OVER_TRACE && strcmpi( m_pappoint->GetErrType().c_str(), "_traceInfo_") == 0 )
	{
		auxFlags += 8;
	}
		
	if( (m_sTypeFlags & auxFlags) !=0 )
	{
	
		// Automaticamente volcamos a los medios los appoints sii StorageAlways()
	
		if (m_plogManager -> StorageAlways())
		{
			m_lappoints.GetStorageList()->StoreTo( ALL_STORAGES, m_pappoint );
		}
	
		if (m_lappoints.GetStorageList()->StoreToLocalQueue(m_pappoint) == M4_TRUE)
		{
			/* Bug 0147755
			Si el mensaje anterior es el de desconexión del server 3538981, ya no se vuelcan más errores
			*/
			m4bool_t bAdd = M4_TRUE ;
			m4int32_t iSize = m_lappoints.GetErrorQueueSize() ;

			if( iSize > 0 )
			{
				m4int32_t iErrorCode = 0 ;
				m4return_t iResult = m_lappoints.GetLastErrorCode( iErrorCode ) ;

				if( iResult == M4_SUCCESS && (iErrorCode == 3538981 || iErrorCode == 3211320) )
				{
					m4char_t acBuffer[ 128 + 1 ] ;

					iResult = m_lappoints.GetLastErrorType( acBuffer, 128, iSize ) ;

					if( iResult == M4_SUCCESS && strcmpi( acBuffer, "_error_" ) == 0 )
					{
						bAdd = M4_FALSE ;
					}
				}
			}

			if( bAdd == M4_TRUE )
			{
				// Se llama al listener si lo hay registrado
				if( m_poListener != NULL )
				{
					m_poListener->LogEvent( m_pappoint->GetErrType(), m_pappoint->GetErrCode(), m_pappoint->GetBuffer() ) ;
				}

				m_lappoints.AddAppoint( m_pappoint );
				m_pappoint = NULL;
			}
		}
    }
	
	// Si no se ha volcado a la pila, lo borramos.

	if (m_pappoint!=NULL)  {
		delete m_pappoint;
		m_pappoint = NULL;
	}
		
	m_plogManager -> IncMessageCounter();
	return M4_SUCCESS;

  //## end ClLogSystem::SetCode%-1971197118.body
}

m4return_t ClLogSystem::SetCode (m4int32_t ai_icode, m4int16_t ai_irango, const M4ClString& ai_text)
{
  //## begin ClLogSystem::SetCode%940267791.body preserve=yes
 
	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Lock();
	
	StartCode (ai_icode,ai_irango);
	AddText(ai_text);
	SetCode();

	if (m_bShared==M4_TRUE) 
		m_AccessMutex.Unlock(); 
	
	return M4_SUCCESS; 
	
  //## end ClLogSystem::SetCode%940267791.body
}

m4int16_t ClLogSystem::StoreAppoints (m4uint32_t sstorages)
{
  //## begin ClLogSystem::StoreAppoints%725848232.body preserve=yes

	if ( ! m_bactivated )
		return M4_ERROR;
	
	m_lappoints.StoreAppoints( sstorages );
	return M4_SUCCESS;

  //## end ClLogSystem::StoreAppoints%725848232.body
}

m4int16_t ClLogSystem::StoreAppoint (m4uint32_t sstorages)
{
  //## begin ClLogSystem::StoreAppoint%1122231137.body preserve=yes

	if ( ! m_bactivated )
		return M4_ERROR;
	
	m_lappoints.StoreAppoint( sstorages );
	return M4_SUCCESS;

  //## end ClLogSystem::StoreAppoint%1122231137.body
}

m4int16_t ClLogSystem::AddAppoints (ClAppointments* pappoints)
{
  //## begin ClLogSystem::AddAppoints%-485397852.body preserve=yes

	if ( ! m_bactivated )
		return M4_ERROR;

	if ( pappoints == &m_lappoints )
		return M4_ERROR;

	pappoints -> AddAppointsTo( &m_lappoints );

	return M4_SUCCESS;


  //## end ClLogSystem::AddAppoints%-485397852.body
}

ClAppointments* ClLogSystem::GetAppointments ()
{
  //## begin ClLogSystem::GetAppointments%1565900245.body preserve=yes

	return &m_lappoints;

  //## end ClLogSystem::GetAppointments%1565900245.body
}

m4char_t ClLogSystem::GetCReturn ()
{
  //## begin ClLogSystem::GetCReturn%517091994.body preserve=yes

	return m_creturn;

  //## end ClLogSystem::GetCReturn%517091994.body
}

void ClLogSystem::SetCReturn (m4char_t creturn)
{
  //## begin ClLogSystem::SetCReturn%-505633360.body preserve=yes

	m_creturn = creturn;

  //## end ClLogSystem::SetCReturn%-505633360.body
}

void ClLogSystem::SetStorageFlags (m4int8_t sstorages)
{
  //## begin ClLogSystem::SetStorageFlags%-1931512826.body preserve=yes

	if ( ( sstorages >= 0 ) && ( sstorages < 16 ) )
		m_sstorages = sstorages;

  //## end ClLogSystem::SetStorageFlags%-1931512826.body
}

m4int8_t ClLogSystem::GetStorageFlags ()
{
  //## begin ClLogSystem::GetStorageFlags%-496079555.body preserve=yes

	return m_sstorages;

  //## end ClLogSystem::GetStorageFlags%-496079555.body
}

m4int16_t ClLogSystem::AddStorage (ClPersistentStorage* pstorage)
{
  //## begin ClLogSystem::AddStorage%-60102562.body preserve=yes

	if ( ! m_bactivated )
		return M4_ERROR;
	
	m_plstorages -> AddStorage( pstorage );
	return M4_SUCCESS;

  //## end ClLogSystem::AddStorage%-60102562.body
}

m4bool_t ClLogSystem::getShowTime ()
{
  //## begin ClLogSystem::getShowTime%1275998738.body preserve=yes


	return m_plogManager->getShowTime();

  //## end ClLogSystem::getShowTime%1275998738.body
}

void ClLogSystem::setShowTime (m4bool_t ai_bshowTime)
{
  //## begin ClLogSystem::setShowTime%1678935944.body preserve=yes


	m_bshowTime = ai_bshowTime;

  //## end ClLogSystem::setShowTime%1678935944.body
}

void ClLogSystem::EnableDebugInfoLogs ()
{
  //## begin ClLogSystem::EnableDebugInfoLogs%-1855435205.body preserve=yes


	 m_sTypeFlags += 1;

  //## end ClLogSystem::EnableDebugInfoLogs%-1855435205.body
}

void ClLogSystem::EnableErrorLogs ()
{
  //## begin ClLogSystem::EnableErrorLogs%-1725523882.body preserve=yes


	m_sTypeFlags += 4;

  //## end ClLogSystem::EnableErrorLogs%-1725523882.body
}

void ClLogSystem::EnableWarningLogs ()
{
  //## begin ClLogSystem::EnableWarningLogs%921707927.body preserve=yes


	 m_sTypeFlags += 2;

  //## end ClLogSystem::EnableWarningLogs%921707927.body
}

void ClLogSystem::EnableTraceInfo ()
{
  //## begin ClLogSystem::EnableTraceInfo%-375840880.body preserve=yes


	m_sTypeFlags += 8;

  //## end ClLogSystem::EnableTraceInfo%-375840880.body
}

void ClLogSystem::DisableDebugInfoLogs ()
{
  //## begin ClLogSystem::DisableDebugInfoLogs%-818867976.body preserve=yes


	m_sTypeFlags -= 1;

  //## end ClLogSystem::DisableDebugInfoLogs%-818867976.body
}

void ClLogSystem::DisableErrorLogs ()
{
  //## begin ClLogSystem::DisableErrorLogs%1806787717.body preserve=yes


	m_sTypeFlags -= 4;

  //## end ClLogSystem::DisableErrorLogs%1806787717.body
}

void ClLogSystem::DisableWarningLogs ()
{
  //## begin ClLogSystem::DisableWarningLogs%-453396481.body preserve=yes


	m_sTypeFlags -= 2;

  //## end ClLogSystem::DisableWarningLogs%-453396481.body
}

void ClLogSystem::DisableTraceInfo ()
{
  //## begin ClLogSystem::DisableTraceInfo%649385769.body preserve=yes


	m_sTypeFlags -= 8;

  //## end ClLogSystem::DisableTraceInfo%649385769.body
}

void ClLogSystem::EnableAllLogs ()
{
  //## begin ClLogSystem::EnableAllLogs%-1090641485.body preserve=yes


	m_sTypeFlags = 7;

  //## end ClLogSystem::EnableAllLogs%-1090641485.body
}

void ClLogSystem::DisableAllLogs ()
{
  //## begin ClLogSystem::DisableAllLogs%-2043793796.body preserve=yes


	m_sTypeFlags = 0;

  //## end ClLogSystem::DisableAllLogs%-2043793796.body
}

void ClLogSystem::EnableLocalQueue ()
{
  //## begin ClLogSystem::EnableLocalQueue%-1767120967.body preserve=yes


	m_sLocalQueue = M4_TRUE;
	

  //## end ClLogSystem::EnableLocalQueue%-1767120967.body
}

void ClLogSystem::DisableLocalQueue ()
{
  //## begin ClLogSystem::DisableLocalQueue%2050630872.body preserve=yes


	m4return_t iret;

	m_sLocalQueue = M4_FALSE;

	while (m_lappoints.Empty() == M4_FALSE)
		iret = m_lappoints.Pop();

	

  //## end ClLogSystem::DisableLocalQueue%2050630872.body
}

m4bool_t ClLogSystem::LocalQueue ()
{
  //## begin ClLogSystem::LocalQueue%1795830953.body preserve=yes

	return m_sLocalQueue;

  //## end ClLogSystem::LocalQueue%1795830953.body
}

m4int32_t ClLogSystem::GetErrorQueueSize ()
{
  //## begin ClLogSystem::GetErrorQueueSize%-489249013.body preserve=yes

	return m_lappoints.GetErrorQueueSize();

  //## end ClLogSystem::GetErrorQueueSize%-489249013.body
}

m4return_t ClLogSystem::GetErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetErrorTime%-49906718.body preserve=yes

	
	
	return m_lappoints.GetErrorTime( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

	
	/*
	
	
	ClPersistentStorage	*pStorage = NULL;
	for ( m4int16_t ipos = 0; ipos <= 3; ipos ++ )
	{
		pStorage = m_plstorages -> GetStorage( ipos );
		if (pStorage!=NULL)
		if ( pStorage -> m_uiStorageType == M4_STORAGE_TYPE_QUEUE )
		{
			return ( ( ClErrorTail * ) pStorage ) -> GetErrorTime( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );
		}		
	}
	return M4_ERROR;
*/


  //## end ClLogSystem::GetErrorTime%-49906718.body
}

m4return_t ClLogSystem::GetLastErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetLastErrorTime%930282884.body preserve=yes

	return m_lappoints.GetLastErrorTime( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetLastErrorTime%930282884.body
}

m4return_t ClLogSystem::GetMarkedErrorTime (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetMarkedErrorTime%1695511415.body preserve=yes

	return m_lappoints.GetMarkedErrorTime( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetMarkedErrorTime%1695511415.body
}

m4return_t ClLogSystem::Get_n_ErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_ErrorTime%-1892375191.body preserve=yes

	return m_lappoints.Get_n_ErrorTime( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::Get_n_ErrorTime%-1892375191.body
}

m4return_t ClLogSystem::Get_n_LastErrorTime (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_LastErrorTime%-1060953250.body preserve=yes

	return m_lappoints.Get_n_LastErrorTime( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::Get_n_LastErrorTime%-1060953250.body
}

m4return_t ClLogSystem::GetErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetErrorCode%-206691613.body preserve=yes

	return m_lappoints.GetErrorCode( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetErrorCode%-206691613.body
}

m4return_t ClLogSystem::GetErrorCode (m4int32_t& ao_lErrorCode)
{
  //## begin ClLogSystem::GetErrorCode%580516220.body preserve=yes

	return m_lappoints.GetErrorCode( ao_lErrorCode );


  //## end ClLogSystem::GetErrorCode%580516220.body
}

m4return_t ClLogSystem::GetLastErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetLastErrorCode%-1974388481.body preserve=yes

	return m_lappoints.GetLastErrorCode( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetLastErrorCode%-1974388481.body
}

m4return_t ClLogSystem::GetLastErrorCode (m4int32_t& ao_lErrorCode)
{
  //## begin ClLogSystem::GetLastErrorCode%121857368.body preserve=yes

	return m_lappoints.GetLastErrorCode( ao_lErrorCode );


  //## end ClLogSystem::GetLastErrorCode%121857368.body
}

m4return_t ClLogSystem::GetMarkedErrorCode (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetMarkedErrorCode%-2081079982.body preserve=yes

	return m_lappoints.GetMarkedErrorCode( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetMarkedErrorCode%-2081079982.body
}

m4return_t ClLogSystem::Get_n_ErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_ErrorCode%304627155.body preserve=yes

	return m_lappoints.Get_n_ErrorCode( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::Get_n_ErrorCode%304627155.body
}

m4return_t ClLogSystem::Get_n_ErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode)
{
  //## begin ClLogSystem::Get_n_ErrorCode%-83220392.body preserve=yes

	return m_lappoints.Get_n_ErrorCode( ai_lIterator, ao_lErrorCode );


  //## end ClLogSystem::Get_n_ErrorCode%-83220392.body
}

m4return_t ClLogSystem::Get_n_LastErrorCode (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_LastErrorCode%488314268.body preserve=yes

	return m_lappoints.Get_n_LastErrorCode( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::Get_n_LastErrorCode%488314268.body
}

m4return_t ClLogSystem::Get_n_LastErrorCode (m4int32_t ai_lIterator, m4int32_t& ao_lErrorCode)
{
  //## begin ClLogSystem::Get_n_LastErrorCode%-1926845934.body preserve=yes

	return m_lappoints.Get_n_LastErrorCode( ai_lIterator, ao_lErrorCode );


  //## end ClLogSystem::Get_n_LastErrorCode%-1926845934.body
}

m4return_t ClLogSystem::GetErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetErrorType%1253421162.body preserve=yes

	return m_lappoints.GetErrorType( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::GetErrorType%1253421162.body
}

m4return_t ClLogSystem::GetLastErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetLastErrorType%1736829055.body preserve=yes

	
	return m_lappoints.GetLastErrorType( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::GetLastErrorType%1736829055.body
}

m4return_t ClLogSystem::GetMarkedErrorType (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetMarkedErrorType%-1860410302.body preserve=yes

	return m_lappoints.GetMarkedErrorType( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetMarkedErrorType%-1860410302.body
}

m4return_t ClLogSystem::Get_n_ErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_ErrorType%1716573295.body preserve=yes

	return m_lappoints.Get_n_ErrorType( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::Get_n_ErrorType%1716573295.body
}

m4return_t ClLogSystem::Get_n_LastErrorType (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_LastErrorType%1411860021.body preserve=yes

	return m_lappoints.Get_n_LastErrorType( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::Get_n_LastErrorType%1411860021.body
}

m4return_t ClLogSystem::GetErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetErrorText%-1356696851.body preserve=yes

	return m_lappoints.GetErrorText( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );


  //## end ClLogSystem::GetErrorText%-1356696851.body
}

m4return_t ClLogSystem::GetLastErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetLastErrorText%355415495.body preserve=yes

	return m_lappoints.GetLastErrorText( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetLastErrorText%355415495.body
}

m4return_t ClLogSystem::GetMarkedErrorText (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::GetMarkedErrorText%1092053750.body preserve=yes

	return m_lappoints.GetMarkedErrorText( ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::GetMarkedErrorText%1092053750.body
}

m4return_t ClLogSystem::Get_n_ErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_ErrorText%-1521933552.body preserve=yes

	return m_lappoints.Get_n_ErrorText( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::Get_n_ErrorText%-1521933552.body
}

m4return_t ClLogSystem::Get_n_LastErrorText (m4int32_t ai_lIterator, m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4int32_t& ao_lSizeCopied)
{
  //## begin ClLogSystem::Get_n_LastErrorText%341476931.body preserve=yes

	return m_lappoints.Get_n_LastErrorText( ai_lIterator,ai_szBuffer, ai_lBufferSize, ao_lSizeCopied );

  //## end ClLogSystem::Get_n_LastErrorText%341476931.body
}

m4int16_t ClLogSystem::SetSerialCode (m4int32_t ai_Size, m4pchar_t ai_SerialCad)
{
  //## begin ClLogSystem::SetSerialCode%-1544686540.body preserve=yes

	

	int cnt= 0;
	int st = 0;
    m4pchar_t Dtype=NULL, Dtime=NULL;
    m4pchar_t pAux = ai_SerialCad;
	m4int32_t icode;
	m4int16_t irango = -1;
	m4pchar_t pmessage=NULL;
	m4int32_t IntSize = 16;
	m4pchar_t  sIntCad = (m4char_t *) calloc (IntSize, sizeof(m4char_t));


	if ((st=GetDistance(pAux,ai_Size))>0) {
	  Dtype = (m4pchar_t)malloc (st+1);
	  *(Dtype+st)=0;
	  memcpy(Dtype,pAux,st);
	  if (strcmp(Dtype,"_error_")==0)
		  irango=ERRORLOG;
	  if (strcmp(Dtype,"_debugInfo_")==0)
		  irango=DEBUGINFOLOG;
	  if (strcmp(Dtype,"_warning_")==0)
		  irango=WARNINGLOG;
	  if (strcmp(Dtype,"_traceInfo_")==0)
		  irango=TRACEINFOLOG;
	  pAux+=st;
	};

	if (Dtype!=NULL) free(Dtype);


	if (st!=-1) {
	  pAux++;
	  if ((st=GetDistance(pAux,ai_Size))>0) {
	    Dtime = (m4pchar_t)malloc (st+1);
		*(Dtime+st)=0;
		memcpy(Dtime,pAux,st);
		pAux+=st;
		
	  };
	};

	if (Dtime!=NULL) free(Dtime);
	
	if (st!=-1) {
	  pAux++;
	  if ((st=GetDistance(pAux,ai_Size))>0) {
		
		memcpy(sIntCad,pAux,st);		
		icode= atoi(sIntCad);
		pAux+=st;
		
	  };
	};

	if (st!=-1) {
	  pAux++;
	  if ((st=GetDistance(pAux,ai_Size))>0) {
	    pmessage = (m4pchar_t)malloc (st+1);
		*(pmessage+st)=0;
		memcpy(pmessage,pAux,st);
		pAux+=st;		
	  } else {	
		// En caso de que no tenga mensaje --> ""
		pmessage = (m4pchar_t)malloc (1);
		*(pmessage)=0;
	  };
	}

	free(sIntCad);

	if (st==-1) return M4_ERROR;

    StartCode(icode , irango);
	AddText(pmessage);
	SetCode();

	if (pmessage!=NULL) free(pmessage);

	return M4_SUCCESS;



  //## end ClLogSystem::SetSerialCode%-1544686540.body
}

m4int16_t ClLogSystem::SetSerialCodeString (m4pchar_t ai_SerialCad)
{
  //## begin ClLogSystem::SetSerialCodeString%533173649.body preserve=yes


	m4uint32_t cnt=0, TotalSize=0,LocalSize=0;
    m4pchar_t pAux = ai_SerialCad;
	m4int32_t IntSize = 16;
	m4pchar_t  sIntCad = (m4char_t *) calloc (IntSize, sizeof(m4char_t));


	memcpy(sIntCad,pAux,IntSize);
	TotalSize= atoi (sIntCad);
	cnt+=IntSize;
	pAux+=IntSize;

	while (cnt<TotalSize) {
	  memset(sIntCad,0,IntSize);
	  memcpy(sIntCad,pAux,IntSize);
	  LocalSize = atoi(sIntCad);
	  cnt+=IntSize;
	  pAux+=IntSize;
	  if (SetSerialCode( LocalSize, pAux )==M4_ERROR) {
		  return M4_ERROR;
	  };
	  pAux+=LocalSize;
	  cnt+=LocalSize;
	};
	
	free (sIntCad);

	return M4_SUCCESS;


  //## end ClLogSystem::SetSerialCodeString%533173649.body
}

m4return_t ClLogSystem::SetMark ()
{
  //## begin ClLogSystem::SetMark%-467947494.body preserve=yes

	return m_lappoints.SetMark();

  //## end ClLogSystem::SetMark%-467947494.body
}

m4return_t ClLogSystem::MoveMark ()
{
  //## begin ClLogSystem::MoveMark%-1001893605.body preserve=yes

	return m_lappoints.MoveMark();

  //## end ClLogSystem::MoveMark%-1001893605.body
}

m4return_t ClLogSystem::IsValidMark ()
{
  //## begin ClLogSystem::IsValidMark%1941229095.body preserve=yes

	return m_lappoints.IsValidMark();

  //## end ClLogSystem::IsValidMark%1941229095.body
}

m4return_t ClLogSystem::Pop ()
{
  //## begin ClLogSystem::Pop%1104823095.body preserve=yes

	return m_lappoints.Pop();

  //## end ClLogSystem::Pop%1104823095.body
}

m4return_t ClLogSystem::Pop_back ()
{
  //## begin ClLogSystem::Pop_back%-771935752.body preserve=yes


	return m_lappoints.Pop_back();

  //## end ClLogSystem::Pop_back%-771935752.body
}

m4bool_t ClLogSystem::Empty ()
{
  //## begin ClLogSystem::Empty%-1359050984.body preserve=yes

	return m_lappoints.Empty();

  //## end ClLogSystem::Empty%-1359050984.body
}

m4return_t ClLogSystem::toString (m4pchar_t aio_FileName, m4uint32_t ai_stringSize)
{
  //## begin ClLogSystem::toString%241386615.body preserve=yes


	m4uint32_t	size=0, iter=0,offset = 0;
	m4int32_t	current_offset = 0;
	m4pchar_t	aux_buffer= NULL;
	m4pchar_t	sInterMess	= "\n\n";
	m4uint32_t	alignSize	= 20;
	m4char_t	msgMask		= ' ';
	m4uint32_t	globalIter  = 0;
	
//	if (m_bShared==M4_TRUE) 
//		m_AccessMutex.Lock();

	size = m_lappoints.GetSize();

	if (!size) {
//		if (m_bShared==M4_TRUE) 
//			m_AccessMutex.Unlock();
		return M4_SUCCESS;
	}
	else {

		aux_buffer = (m4pchar_t) malloc (MAX_BUFFER_SIZE);
		memset(aux_buffer,msgMask,MAX_BUFFER_SIZE);
		while (iter < size) {
			offset = 0;
			
			Get_n_ErrorTime(iter,aux_buffer+offset,MAX_BUFFER_SIZE,current_offset);
			*(aux_buffer+offset+current_offset) = msgMask;
			offset+=alignSize;

			Get_n_ErrorType(iter,aux_buffer+offset,MAX_BUFFER_SIZE,current_offset);
			*(aux_buffer+offset+current_offset) = msgMask;
			offset+=alignSize;

			Get_n_ErrorCode(iter,aux_buffer+offset,MAX_BUFFER_SIZE,current_offset);
			*(aux_buffer+offset+current_offset) = msgMask;
			offset+=alignSize;

			Get_n_ErrorText(iter,aux_buffer+offset,MAX_BUFFER_SIZE,current_offset);
			
			strcpy(aux_buffer+offset+current_offset,sInterMess);
			offset+= (m4uint32_t) strlen(sInterMess);
			
			if (globalIter+offset >  ai_stringSize) {
//				if (m_bShared==M4_TRUE) 
//					m_AccessMutex.Unlock();
				return M4_ERROR;
			};
			 
			strcat(aio_FileName,aux_buffer);
			globalIter+=offset;
			iter++;
		}	
	
		free(aux_buffer);
	}
	
//	if (m_bShared==M4_TRUE) 
//			m_AccessMutex.Unlock();
	return M4_SUCCESS;


  //## end ClLogSystem::toString%241386615.body
}

m4return_t ClLogSystem::GetSerialStringSizeFromMessage (m4uint32_t& ao_StringSize, m4uint32_t ai_MessageCounter)
{
  //## begin ClLogSystem::GetSerialStringSizeFromMessage%938426076.body preserve=yes
 
	return m_lappoints.GetSerialStringSizeFromMessage (ao_StringSize,ai_MessageCounter);
	
  //## end ClLogSystem::GetSerialStringSizeFromMessage%938426076.body
}

m4return_t ClLogSystem::GetSerialStringFromMessage (m4pchar_t ai_szBuffer, m4int32_t ai_lBufferSize, m4uint32_t& ao_lastMessageIter, m4uint32_t ai_MessageCounter)
{
  //## begin ClLogSystem::GetSerialStringFromMessage%938426077.body preserve=yes
  
	return m_lappoints.GetSerialStringFromMessage (ai_szBuffer,ai_lBufferSize,ao_lastMessageIter,ai_MessageCounter);
	
  //## end ClLogSystem::GetSerialStringFromMessage%938426077.body
}

m4int32_t ClLogSystem::GetMessageCounter ()
{
  //## begin ClLogSystem::GetMessageCounter%938426078.body preserve=yes
  
	return m_lappoints.GetMessageCounter ();
	
  //## end ClLogSystem::GetMessageCounter%938426078.body
}


m4return_t	ClLogSystem::ResetOverwriteMask( ClLogManager* ai_pLogManager )
{

	ClOutputPattern	*poPattern = NULL ;


	if( ai_pLogManager == NULL )
	{
		return( M4_ERROR ) ;
	}

	m_iOverwriteMask = M4LOG_OVER_NONE ;

	if( ai_pLogManager->Error() == M4_TRUE )
	{
		m_iOverwriteMask |= M4LOG_OVER_ERROR ;
	}

	if( ai_pLogManager->Warning() == M4_TRUE )
	{
		m_iOverwriteMask |= M4LOG_OVER_WARNING ;
	}

	if( ai_pLogManager->DebugInfo() == M4_TRUE )
	{
		m_iOverwriteMask |= M4LOG_OVER_DEBUG ;
	}

	if( ai_pLogManager->TraceInfo() == M4_TRUE )
	{
		m_iOverwriteMask |= M4LOG_OVER_TRACE ;
	}

	poPattern = ai_pLogManager->GetModuleList()->getOutputPatternMap()->getOutputPatternByName( "defaultPattern" ) ;

	if( poPattern == NULL )
	{
		return( M4_ERROR ) ;
	}

	if( poPattern->GetErrorLevel() != 0 )
	{
		m_iOverwriteMask |= M4LOG_OVER_DEFAULT_ERROR ;
	}

	if( poPattern->GetWarningLevel() != 0 )
	{
		m_iOverwriteMask |= M4LOG_OVER_DEFAULT_WARNING ;
	}

	if( poPattern->GetDebugLevel() != 0 )
	{
		m_iOverwriteMask |= M4LOG_OVER_DEFAULT_DEBUG ;
	}

	if( poPattern->GetTraceLevel() != 0 )
	{
		m_iOverwriteMask |= M4LOG_OVER_DEFAULT_TRACE ;
	}

	return( M4_SUCCESS ) ;
}

void ClLogSystem::ResetUserTimeAudit()
{
	m_pcSessionKey = NULL;

	if( m_pcServerName != NULL )
	{
		delete m_pcServerName;
		m_pcServerName = NULL;
	}

	m_iServerPort = -1;
	m_iSessionId = -1;
	m_iClickNum = -1;
	m_pcReqUniqueId = NULL;
}

m4pchar_t ClLogSystem::GetSessionKey()
{
	return m_pcSessionKey;
}

m4pchar_t ClLogSystem::GetServerName()
{
	return m_pcServerName;
}

int ClLogSystem::GetServerPort()
{
	return m_iServerPort;
}

int ClLogSystem::GetSessionId()
{
	return m_iSessionId;
}

int ClLogSystem::GetClickNum()
{
	return m_iClickNum;
}

m4pchar_t ClLogSystem::GetReqUniqueId()
{
	return m_pcReqUniqueId;
}

void ClLogSystem::SetSessionKey( m4pchar_t ai_pcSessionKey )
{
	m_pcSessionKey = ai_pcSessionKey;
}

void ClLogSystem::SetServerName( m4pcchar_t ai_pcServerName )
{
	size_t	iLen;

	if( ai_pcServerName == NULL )
	{
		if( m_pcServerName != NULL )
		{
			delete m_pcServerName;
			m_pcServerName = NULL;
		}

		return;
	}

	iLen = strlen( ai_pcServerName );

	if( m_pcServerName == NULL )
	{
		m_pcServerName = new m4char_t[ iLen + 1 ];
	}
	else if( iLen > strlen( m_pcServerName ) )
	{
		delete m_pcServerName;
		m_pcServerName = new m4char_t[ iLen + 1 ];
	}

	strcpy( m_pcServerName, ai_pcServerName );
}

void ClLogSystem::SetServerPort( int ai_iServerPort )
{
	m_iServerPort = ai_iServerPort;
}

void ClLogSystem::SetSessionId( int ai_iSessionId )
{
	m_iSessionId = ai_iSessionId;
}

void ClLogSystem::SetClickNum( int ai_iClickNum )
{
	m_iClickNum = ai_iClickNum;
}

void ClLogSystem::SetReqUniqueId( m4pchar_t ai_pcReqUniqueId )
{
	m_pcReqUniqueId = ai_pcReqUniqueId;
}

// Additional Declarations
  //## begin ClLogSystem%377C884F02D8.declarations preserve=yes
  //## end ClLogSystem%377C884F02D8.declarations

//## begin module%377C88570153.epilog preserve=yes
//## end module%377C88570153.epilog
