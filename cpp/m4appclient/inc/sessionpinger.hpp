//	=========================================================
//
//	 (c) Copyright  1991-2015 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              sessionpinger.hpp
//	 Project:           M4AppClient
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
//	=========================================================

//## Module: sessionpinger
//## Subsystem: M4AppClient::inc
//## Source file: c:\m4server\m4appclient\inc\sessionpinger.hpp

#ifndef sessionpinger_h
#define sessionpinger_h 1

#include "m4appclient_dll.hpp"
// clientbasicdef
#include "clientbasicdef.hpp"
// m4thread
#include "m4thread.hpp"
// cldates
#include "cldates.hpp"

class ClCCSession;

#ifdef _WINDOWS
class IM4TimeoutEventProvider
{
public:
	virtual m4int_t	OnCountdownForTimeOut( m4int_t ai_iSeconds ) = 0 ;
};

class M4_DECL_M4APPCLIENT ClSessionPingerEvents
{
  public:
	  static void RegisterTimeoutEventProvider(IM4TimeoutEventProvider *ai_poTimeOutEventProvider);
	  static void UnregisterTimeoutEventProvider(IM4TimeoutEventProvider *ai_poTimeOutEventProvider);

protected:
	  static set<IM4TimeoutEventProvider*, less<IM4TimeoutEventProvider*> >	sm_oTimeOutEventProviders;
};

class ClSessionPinger : public M4Thread, protected ClSessionPingerEvents
{
  public:
	  ClSessionPinger(ClCCSession* ai_oSession, m4double_t dSessionTimeout);

	  void Run();

	  void NotifyVMExecution(m4bool_t ai_bIsStart);

	  void NotifyPDUCommunication(m4bool_t ai_bIsStart);

	  void FinishPinger();

  private:
	  /// Referencia al objeto sesión al que está asociado
	  ClCCSession* m_oSession;
	  /// Timeout definido en el servidor
	  m4uint32_t m_uiSessionTimeOut;
	  /// Tiempo de cuenta atrás definido
	  m4uint32_t m_uiDefinedCountdown;
	  /// Margen en segundos de la gestión del Timeout en cliente respecto al server
	  m4uint32_t m_uiSecondsMargin;
	  /// Si hay una petición ejecutándose en server
	  m4bool_t m_bPDUInServer;
	  /// Timestamp de la última comunicación con el server
	  m4date_t m_dLastPDUComunication;
	  /// Nivel de ejecución de LN4
	  m4uint32_t m_uiVMReentries;
	  /// Timestamp de la última ejecución LN4
	  m4date_t m_dLastVMExecution;
	  /// Timestamp del último chequeo del pinger
	  m4date_t m_dLastCheck;
	  /// Mutex de sincronización del pinger
	  ClMutex m_oPingerMutex;
};

#else
// Dummy class
class ClSessionPinger
{
  public:
	  ClSessionPinger(ClCCSession* ai_oSession, m4double_t dSessionTimeout) {}

	  void Run() {}

	  void NotifyVMExecution(m4bool_t ai_bIsStart) {}

	  void NotifyPDUCommunication(m4bool_t ai_bIsStart) {}

	  void FinishPinger() {}

	  void Start() {} // Dummy M4Thread method
};
#endif

#endif

