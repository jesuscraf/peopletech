//	==============================================================================
//
//	 (c) Copyright  1991-2015 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              sessionpinger.cpp
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
//	==============================================================================

//## Module: sessionpinger
//## Subsystem: M4AppClient::inc
//## Source file: c:\m4server\m4appclient\src\sessionpinger.cpp

#ifdef _WINDOWS

#include "sessionpinger.hpp"
#include "clientsession.hpp"
#include "m4hook.h"
#include "m4date.hpp"

// Descomentar para trazas
//#define TRACE_DEBUG

#ifdef TRACE_DEBUG

#define TRACE_FILE "c:/temp/m4clientping.log"

#define PINGER_DEBUG(a,b) InternalPingerDebug(a,b)
#define PINGER_DEBUG_MUTEX(a,b,c) InternalPingerDebugMutex(a,b,c)

static FILE *tracer = NULL;

void InternalPingerDebug(const char * ai_pcString, m4int32_t ai_ivalue)
{
	if (NULL == tracer)
	{
		tracer = fopen(TRACE_FILE, "wt");
	}

	SYSTEMTIME stTime;
	GetSystemTime(&stTime);

	fprintf(tracer, "[%04d-%02d-%02d %02d:%02d:%02d] %s: %d second(s)\n",
		stTime.wYear, stTime.wMonth, stTime.wDay,
		stTime.wHour, stTime.wMinute, stTime.wSecond,
		ai_pcString, ai_ivalue);

	fflush(tracer);
}

void InternalPingerDebugMutex(ClMutex& ai_oMutex, const char * ai_pcString, m4int32_t ai_ivalue)
{
	ai_oMutex.Lock();
	InternalPingerDebug(ai_pcString, ai_ivalue);
	ai_oMutex.Unlock();
}

#else

#define PINGER_DEBUG(a,b) {}
#define PINGER_DEBUG_MUTEX(a,b,c) {}

#endif

#define DEFAULT_SECONDS_FOR_COUNTDOWN	60
#define DEFAULT_SECONDS_FOR_MARGIN		15

inline m4date_t SecondsToJulians(m4int32_t ai_iSeconds)
{
	return m4date_t(ai_iSeconds) / 86400.0;
}

inline m4int32_t JuliansToSeconds(m4date_t ai_dJulians)
{
	return m4int32_t(ai_dJulians * 86400.0);
}

ClSessionPinger::ClSessionPinger(ClCCSession* ai_oSession, m4double_t dSessionTimeout) :
	m_oSession(ai_oSession), m_uiSessionTimeOut(m4int32_t(dSessionTimeout * 60)),
		m_bPDUInServer(M4_FALSE), m_uiVMReentries(0)
{
		m_dLastPDUComunication = m_dLastVMExecution = ClActualDate();
		m_uiDefinedCountdown = min(DEFAULT_SECONDS_FOR_COUNTDOWN, m_uiSessionTimeOut / 3);
		m_uiSecondsMargin = DEFAULT_SECONDS_FOR_MARGIN;
		m_oPingerMutex.Init();
}

void ClSessionPinger::Run()
{
	// Timestamp de la última acción de usuario (por click o por LN4)
	m4date_t dLastActionTS;
	// Indica el tiempo adicional que es necesario añadir a la sesión del server (0 -> No es necesario ping)
	m4uint32_t iPendingTimeOut;
	// Número de segundos restantes hasta que se llegue al umbral definido
	m4int32_t iSecondsForNextCheck;
	// Tiempo disponible para la cuenta atrás
	m4int32_t iCountdownTime;
	// Tiempo de sesión activo del servidor
	m4uint32_t iActualSessionTimeOut = m_uiSessionTimeOut;

	m4bool_t bContinue = m_oSession != NULL ? M4_TRUE : M4_FALSE;

	m4int_t	iResult = -1;
	set<IM4TimeoutEventProvider*, less<IM4TimeoutEventProvider*> >::iterator it;


	PINGER_DEBUG_MUTEX(m_oPingerMutex, "Pinger start", -1);

	while (bContinue)
	{
		// Último chequeo antes de quedarse a la espera
		m_dLastCheck = ClActualDate();

		iSecondsForNextCheck = iActualSessionTimeOut - JuliansToSeconds(m_dLastCheck - m_dLastPDUComunication) - m_uiSecondsMargin;

		if (sm_oTimeOutEventProviders.size() > 0)
		{
			// Si hay proveedor de cuenta atrás, se descuenta su duración
			iSecondsForNextCheck -= m_uiDefinedCountdown;
		}

		if (iSecondsForNextCheck <= 0) iSecondsForNextCheck = 1; // Modo de pooling por seguridad

		PINGER_DEBUG_MUTEX(m_oPingerMutex, "Waiting", iSecondsForNextCheck);

		m4sleep(iSecondsForNextCheck);

		PINGER_DEBUG_MUTEX(m_oPingerMutex, "Stop waiting", iSecondsForNextCheck);

		m_oPingerMutex.Lock();
		if (m_oSession != NULL)
		{
			// Por defecto no se va a lanzar el ping
			iPendingTimeOut = 0;

			if (m_bPDUInServer == M4_TRUE)
			{
				// Si la PDU está en server no es necesario ningún PING.
				// Se ajusta el TS asociado PDU para evitar la ejecución por segundo de la comprobación
				m_dLastPDUComunication = ClActualDate();
				iActualSessionTimeOut = m_uiSessionTimeOut;
				PINGER_DEBUG("PDU in server. Set timeout", m_uiSessionTimeOut);
			}
			else if (m_dLastPDUComunication > m_dLastCheck)
			{
				// Hay envio de PDU posterior al último chequeo. El timeout es el normal
				iActualSessionTimeOut = m_uiSessionTimeOut;
				PINGER_DEBUG("PDU after last check. Time elapsed", JuliansToSeconds(ClActualDate() - m_dLastPDUComunication));
			}
			else if (m_uiVMReentries > 0)
			{
				// Se está ejecutando LN4 => Se debe lanzar el Ping sin necesidad de ninguna comprobación adicional
				iPendingTimeOut = m_uiSessionTimeOut;
				PINGER_DEBUG("Executing LN4. Setting timeout with PDU Ping", m_uiSessionTimeOut);
			}
			else
			{
				// Se analiza el TS de tiempo de usuario
				dLastActionTS = GetUserEventsTimeStamp();
				// y si finalizó alguna ejecución LN4 posterior a la acción de usuario
				if (m_dLastVMExecution > dLastActionTS) dLastActionTS = m_dLastVMExecution;

				if (dLastActionTS > m_dLastPDUComunication)
				{
					// Hay un acción posterior a la última PDU. Se ajusta el Timeout pendiente
					iPendingTimeOut = m_uiSessionTimeOut - JuliansToSeconds(ClActualDate() - dLastActionTS);
					PINGER_DEBUG("User action after PDU send", JuliansToSeconds(dLastActionTS - m_dLastPDUComunication));
				}
				else
				{
					// Vence el TimeOut (o se inicia la cuenta atrás si hay proveedor)
					// Por defecto el bucle de pinger finalizará (si no se indica continuar con la sesión)
					bContinue = M4_FALSE;
					if (sm_oTimeOutEventProviders.size() > 0)
					{
						iCountdownTime = iActualSessionTimeOut - JuliansToSeconds(ClActualDate() - m_dLastPDUComunication) - m_uiSecondsMargin;
						if (iCountdownTime < 0) iCountdownTime = 0; // Notificación de TimeOut. No hay margen

						PINGER_DEBUG("Invoke countdown", iCountdownTime);

						it = sm_oTimeOutEventProviders.begin();

						while (it != sm_oTimeOutEventProviders.end())
						{
							iResult = (*it)->OnCountdownForTimeOut(iCountdownTime);

							if (iResult != -1)
							{
								if (iResult == M4_TRUE)
								{
									// Se ha aceptado la ventana de continuar la sesión. Se lanza el ping
									iPendingTimeOut = m_uiSessionTimeOut;
									bContinue = M4_TRUE;
								}
								break;
							}
							it++ ;
						}
					}
				}
			}

			if (iPendingTimeOut > 0)
			{
				// Se lanza la PDU de Ping
				m_oSession->SendPing(m_uiSessionTimeOut - iPendingTimeOut);
				iActualSessionTimeOut = iPendingTimeOut;
				PINGER_DEBUG("Sending Ping PDU. ElapsedTime", m_uiSessionTimeOut - iPendingTimeOut);
			}
		}
		else
		{
			bContinue = M4_FALSE;
		}
		m_oPingerMutex.Unlock();
	}

	// Se ha dejado morir la sesión
	PINGER_DEBUG("Pinger stop", -1);
}

void ClSessionPinger::NotifyVMExecution(m4bool_t ai_bIsStart)
{
	if (ai_bIsStart == M4_TRUE)
	{
		m_uiVMReentries++;
	}
	else if (m_uiVMReentries > 0)
	{
		if (--m_uiVMReentries == 0)
		{
			m_dLastVMExecution = ClActualDate();
		}
	}
}

void ClSessionPinger::NotifyPDUCommunication(m4bool_t ai_bIsStart)
{
	m_dLastPDUComunication = ClActualDate();
	m_bPDUInServer = ai_bIsStart;
}

void ClSessionPinger::FinishPinger()
{
	m_oPingerMutex.Lock();
	m_oSession = NULL;
	PINGER_DEBUG("Pinger finish", -1);
	m_oPingerMutex.Unlock();
}

// ==============================================================================

set<IM4TimeoutEventProvider*, less<IM4TimeoutEventProvider*> >	ClSessionPingerEvents::sm_oTimeOutEventProviders;

void ClSessionPingerEvents::RegisterTimeoutEventProvider(IM4TimeoutEventProvider *ai_poTimeOutEventProvider)
{
	sm_oTimeOutEventProviders.insert(ai_poTimeOutEventProvider);
}

void ClSessionPingerEvents::UnregisterTimeoutEventProvider(IM4TimeoutEventProvider *ai_poTimeOutEventProvider)
{
	sm_oTimeOutEventProviders.erase(ai_poTimeOutEventProvider);
}

#endif