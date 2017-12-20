//## begin module%343E4E890273.cm preserve=no
//## end module%343E4E890273.cm

//## begin module%343E4E890273.cp preserve=no
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
//## end module%343E4E890273.cp

//## Module: ExVariables%343E4E890273; Package body
//## Subsystem: M4ServerBase::src%379431BE02D0
//## Source file: F:\integration\m4serverbase\src\exvariables.cpp

//## begin module%343E4E890273.additionalIncludes preserve=no
//## end module%343E4E890273.additionalIncludes

//## begin module%343E4E890273.includes preserve=yes
#include <boolcondition.hpp>
//## end module%343E4E890273.includes

// m4types
#include <m4types.hpp>
// syncro
#include <syncro.hpp>
// m4clock
#include <m4clock.hpp>
// ExVariables
#include <exvariables.hpp>


//## begin module%343E4E890273.declarations preserve=no
//## end module%343E4E890273.declarations

//## begin module%343E4E890273.additionalDeclarations preserve=yes
//## end module%343E4E890273.additionalDeclarations


// Class ClStatus 


//## begin ClStatus::m_oMutex%3439FBCE03A6.attr preserve=no  private: static ClMutex {UA} 
ClMutex ClStatus::m_oMutex;
//## end ClStatus::m_oMutex%3439FBCE03A6.attr

//## begin ClStatus::m_bInitialized%35F8122402D5.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
m4bool_t ClStatus::m_bInitialized = M4_FALSE;
//## end ClStatus::m_bInitialized%35F8122402D5.attr



//## begin ClStatus::InfiniteTimeOut%3847A815039E.attr preserve=no  public: static m4uint_t {VAC} 0
const m4uint_t  ClStatus::InfiniteTimeOut = 0;
//## end ClStatus::InfiniteTimeOut%3847A815039E.attr






//## begin ClStatus::m_iCurrentTime%388C67CA0249.role preserve=no  protected: static m4uint32_t {n -> 1VHAN}
m4uint32_t ClStatus::m_iCurrentTime = 0;
//## end ClStatus::m_iCurrentTime%388C67CA0249.role


ClStatus::ClStatus (m4uint32_t ai_lIdObject)
  //## begin ClStatus::ClStatus%905450087.hasinit preserve=no
      : m_IntrStatus(STATUS_INTR_NORMAL), m_lIdObject(ai_lIdObject), m_iPauseLevel(0), m_pbcIntrVal(NULL)
  //## end ClStatus::ClStatus%905450087.hasinit
  //## begin ClStatus::ClStatus%905450087.initialization preserve=yes
	, m_iTimeToGo(InfiniteTimeOut), m_bHasTimedOut(M4_FALSE), m_poNextStatus(NULL)
  //## end ClStatus::ClStatus%905450087.initialization
{
  //## begin ClStatus::ClStatus%905450087.body preserve=yes
	if (! m_bInitialized )
	{
		m_oMutex.Init();

		m_bInitialized = M4_TRUE ;
	}
	m_Status = 0 ;
  //## end ClStatus::ClStatus%905450087.body
}


ClStatus::~ClStatus ()
{
  //## begin ClStatus::~ClStatus%905450088.body preserve=yes
  //## end ClStatus::~ClStatus%905450088.body
}



//## Other Operations (implementation)
m4int16_t ClStatus::GetValue ()
{
  //## begin ClStatus::GetValue%876216047.body preserve=yes
		  m4int16_t iValue ;

		  m_oMutex.Lock ( ) ;
		  iValue = m_Status ;
		  m_oMutex.Unlock ( ) ;
		  return iValue ;
  //## end ClStatus::GetValue%876216047.body
}

void ClStatus::SetValue (m4int16_t ai_iValue)
{
  //## begin ClStatus::SetValue%876216048.body preserve=yes
		  m_oMutex.Lock ( ) ;
		  m_Status = ai_iValue ;
		  ResetTimeOut();
		  m_oMutex.Unlock ( ) ;
  //## end ClStatus::SetValue%876216048.body
}

m4int16_t ClStatus::GetIntrValue ()
{
  //## begin ClStatus::GetIntrValue%920037517.body preserve=yes
	m4int16_t iIntrStatus = m_IntrStatus;
    
    // Precondicion: Debo estar dentro de una ejecucion
    // (el ejecutor me ha asignado un ClBooleanCondition)
    if (NULL == m_pbcIntrVal) 				return iIntrStatus;
	if (STATUS_INTR_QUIT == iIntrStatus)	return STATUS_INTR_QUIT;

	//	EMN	991018	HAY QUE REVISAR LA CONDICION YA QUE m_Status NO TIENE SEMANTICA
	if (STATUS_INTR_NORMAL == iIntrStatus)
	{
		if (m_Status || (M4_TRUE == HasTimedOut()))
		{
			m_IntrStatus = STATUS_INTR_QUIT;
			return STATUS_INTR_QUIT;
		}

		return STATUS_INTR_NORMAL;
	}

	if (iIntrStatus == STATUS_INTR_PAUSING)
	{
	// BoolCondition del ejecutor que esta ejecutando esta clase. Con lo cual no hay que bloquear
		m_pbcIntrVal->Lock();

		m_IntrStatus = STATUS_INTR_PAUSED;
		m_pbcIntrVal->Notify(M4_TRUE);	// Despierto a todos los que estan esperando para que me pause

		m_pbcIntrVal->Wait();			// Me duermo indefinidamente

		// Solo puedo pasar a estado normal si no me han cancelado mientras estaba dormido
		M4_ASSERT(m_IntrStatus == STATUS_INTR_RESUMING || m_IntrStatus == STATUS_INTR_QUIT || m_IntrStatus == STATUS_INTR_ABORT);
		if (m_IntrStatus == STATUS_INTR_RESUMING)
			m_IntrStatus = STATUS_INTR_NORMAL;

		m_pbcIntrVal->Unlock();

		return STATUS_INTR_NORMAL;
	}

    return iIntrStatus;

  //## end ClStatus::GetIntrValue%920037517.body
}

m4return_t ClStatus::SetIntrValue (m4int16_t ai_iNewStatus)
{
  //## begin ClStatus::SetIntrValue%920037518.body preserve=yes
    // Precondicion: Si me voy a mover a mi mismo estado no tengo nada mas que hacer
    if (ai_iNewStatus == m_IntrStatus)	return M4_SUCCESS;		// Podria devolver M4_WARNING para indicar esta situacion

    // Precondicion: Debo estar dentro de una ejecucion
   // (el ejecutor me ha asignado un ClBooleanCondition)
    if (m_pbcIntrVal == NULL)
    {
		m_IntrStatus = ai_iNewStatus;
        return M4_SUCCESS;
    }

	m4return_t iRet = M4_SUCCESS;

    m_pbcIntrVal->Lock();

	switch (ai_iNewStatus)
	{
	case STATUS_INTR_PAUSING:

		if (GetPauseLevel() == 0)
		{
			// Solo es valido si estoy en STATUS_INTR_NORMAL o STATUS_INTR_RESUMING
			if (m_IntrStatus == STATUS_INTR_NORMAL || m_IntrStatus == STATUS_INTR_RESUMING)
			{
				// checks the pause level
				m_IntrStatus = ai_iNewStatus;
				iRet = m_pbcIntrVal->Wait(10);			// Devuelve M4_WARNING por time-out
			}
			else
				iRet = M4_ERROR;
		}

		// sets the pause level
		if (iRet != M4_ERROR)
		{
			IncrPauseLevel(1);
		}

		break;
        
	case STATUS_INTR_RESUMING:

		// checks the pause level
		if (GetPauseLevel() == 1)
		{
			// Solo es valido si estoy en STATUS_INTR_PAUSED o STATUS_INTR_PAUSING
			if (m_IntrStatus == STATUS_INTR_PAUSED)
			{
				m_IntrStatus = ai_iNewStatus;
				iRet = m_pbcIntrVal->Notify(M4_TRUE);	// Despierto al otro thread que esta en la pausa
			}
			else
			{
				if (m_IntrStatus == STATUS_INTR_PAUSING)
					m_IntrStatus = STATUS_INTR_NORMAL;	// Como no estaba parado paso directamente a normal
				else
					iRet = M4_ERROR;
			}
		}

		// checks the error status
		if (iRet != M4_ERROR)
			IncrPauseLevel(-1);

		break;
		
	case STATUS_INTR_QUIT:
	case STATUS_INTR_ABORT:
		{
			m4int16_t iOldIntrStatus = m_IntrStatus;
			m_IntrStatus = ai_iNewStatus;

			// Si estaba en pausa antes de cancelar, tengo que despertar al thread que duerme
			if (iOldIntrStatus == STATUS_INTR_PAUSED)
				iRet = m_pbcIntrVal->Notify(M4_TRUE);	// Despierto al otro thread que esta en la pausa

			break;
		}
		
	default:
		iRet = M4_ERROR;
	}
    m_pbcIntrVal->Unlock();

    return iRet;

  //## end ClStatus::SetIntrValue%920037518.body
}

void ClStatus::SetBoolCondIntrVal (ClBooleanCondition* const ai_pBoolCond)
{
  //## begin ClStatus::SetBoolCondIntrVal%939229284.body preserve=yes
	// fix bug 0175560, take 2
	m_oMutex.Lock ( ) ;
	ClStatus *poCurrentStatus = this;
	while ( poCurrentStatus != NULL )
	{
		poCurrentStatus->m_pbcIntrVal = ai_pBoolCond;
		poCurrentStatus = poCurrentStatus->m_poNextStatus;
	}
	m_oMutex.Unlock ( ) ;
  //## end ClStatus::SetBoolCondIntrVal%939229284.body
}

ClBooleanCondition* ClStatus::GetBoolCondIntrVal () const
{
  //## begin ClStatus::GetBoolCondIntrVal%939229285.body preserve=yes
	m_oMutex.Lock ( ) ;
	ClBooleanCondition* pBoolCond = m_pbcIntrVal;
	m_oMutex.Unlock ( ) ;
    return pBoolCond;
  //## end ClStatus::GetBoolCondIntrVal%939229285.body
}

m4return_t ClStatus::GetTimeToGo (m4uint_t &ao_iTimeout)
{
  //## begin ClStatus::GetTimeToGo%939981146.body preserve=yes
	if (InfiniteTimeOut == m_iTimeToGo)	return M4_SUCCESS;
	if (M4_TRUE == m_bHasTimedOut)		return M4_ERROR;

	m4millisec_t	iNow = m4millisec_t(m_iCurrentTime) * 1000;
	m4uint_t		iTimeToGo =  (m4uint_t)(iNow - m_iStartExecTime);
	if (iTimeToGo > m_iTimeToGo)
	{
		m_bHasTimedOut = M4_TRUE;
		return M4_ERROR;
	}

	ao_iTimeout = (m4uint_t)(m_iTimeToGo - iTimeToGo);
	return M4_WARNING;

	//## end ClStatus::GetTimeToGo%939981146.body
}

void ClStatus::UpdateTime ()
{
  //## begin ClStatus::UpdateTime%948725626.body preserve=yes
	m_iCurrentTime = m4uint32_t(ClPrecisionTicker::GetTimeTick() / 1000);
  //## end ClStatus::UpdateTime%948725626.body
}

m4int16_t ClStatus::GetIntrValueForAdmin ()
{
  //## begin ClStatus::GetIntrValueForAdmin%1057061436.body preserve=yes
    return m_IntrStatus;
  //## end ClStatus::GetIntrValueForAdmin%1057061436.body
}

ClStatus* ClStatus::GetNextStatus (void) const
{
	return m_poNextStatus;
}

void ClStatus::SetNextStatus (ClStatus* ai_poNextStatus)
{
	m_poNextStatus = ai_poNextStatus;
}


// Additional Declarations
  //## begin ClStatus%3439FAD80154.declarations preserve=yes
  //## end ClStatus%3439FAD80154.declarations

// Class ClEnable 


//## begin ClEnable::m_oMutex%343B7B9C0354.attr preserve=no  private: static ClSemaphore {UA} 
ClMutex ClEnable::m_oMutex;
//## end ClEnable::m_oMutex%343B7B9C0354.attr

//## begin ClEnable::m_bInitialized%35F8121B0083.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
m4bool_t ClEnable::m_bInitialized = M4_FALSE;
//## end ClEnable::m_bInitialized%35F8121B0083.attr

ClEnable::ClEnable ()
  //## begin ClEnable::ClEnable%905450089.hasinit preserve=no
  //## end ClEnable::ClEnable%905450089.hasinit
  //## begin ClEnable::ClEnable%905450089.initialization preserve=yes
  //## end ClEnable::ClEnable%905450089.initialization
{
  //## begin ClEnable::ClEnable%905450089.body preserve=yes
  //## end ClEnable::ClEnable%905450089.body
}


ClEnable::~ClEnable ()
{
  //## begin ClEnable::~ClEnable%905450090.body preserve=yes
  //## end ClEnable::~ClEnable%905450090.body
}



//## Other Operations (implementation)
m4bool_t ClEnable::GetValue ()
{
  //## begin ClEnable::GetValue%876313635.body preserve=yes
		  m4bool_t bValue ;

		  m_oMutex.Lock ( ) ;
		  bValue = m_Enable ;
		  m_oMutex.Unlock ( ) ;
		  return bValue ;
  //## end ClEnable::GetValue%876313635.body
}

void ClEnable::SetValue (m4bool_t ai_bValue)
{
  //## begin ClEnable::SetValue%876313636.body preserve=yes
		  m_oMutex.Lock ( ) ;
		  m_Enable = ai_bValue ;
		  m_oMutex.Unlock ( ) ;
  //## end ClEnable::SetValue%876313636.body
}

// Additional Declarations
  //## begin ClEnable%343B7B780063.declarations preserve=yes
  //## end ClEnable%343B7B780063.declarations

// Class ClNumRequest 


//## begin ClNumRequest::m_oMutex%343BA99B01D9.attr preserve=no  private: static ClMutex {VA} 
ClMutex ClNumRequest::m_oMutex;
//## end ClNumRequest::m_oMutex%343BA99B01D9.attr

//## begin ClNumRequest::m_bInitialized%35F81203011F.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
m4bool_t ClNumRequest::m_bInitialized = M4_FALSE;
//## end ClNumRequest::m_bInitialized%35F81203011F.attr


ClNumRequest::ClNumRequest ()
  //## begin ClNumRequest::ClNumRequest%905450091.hasinit preserve=no
      : m_iNumRequest(0)
  //## end ClNumRequest::ClNumRequest%905450091.hasinit
  //## begin ClNumRequest::ClNumRequest%905450091.initialization preserve=yes
  //## end ClNumRequest::ClNumRequest%905450091.initialization
{
  //## begin ClNumRequest::ClNumRequest%905450091.body preserve=yes
	if (! m_bInitialized )
	{
		m_oMutex.Init () ;
		m_bInitialized = M4_TRUE ;
	}
  //## end ClNumRequest::ClNumRequest%905450091.body
}


ClNumRequest::~ClNumRequest ()
{
  //## begin ClNumRequest::~ClNumRequest%905450092.body preserve=yes
  //## end ClNumRequest::~ClNumRequest%905450092.body
}



//## Other Operations (implementation)
m4int32_t ClNumRequest::GetValue ()
{
  //## begin ClNumRequest::GetValue%876334195.body preserve=yes
	m4int32_t iValue;

	m_oMutex.Lock();
	iValue = m_iNumRequest ;
	m_oMutex.Unlock ( );
	return iValue ;
  //## end ClNumRequest::GetValue%876334195.body
}

void ClNumRequest::SetValue (m4int32_t ai_lValue)
{
  //## begin ClNumRequest::SetValue%876334196.body preserve=yes
	m_oMutex.Lock();
	m_iNumRequest = ai_lValue;
	m_oMutex.Unlock ( );
  //## end ClNumRequest::SetValue%876334196.body
}

ClNumRequest & ClNumRequest::operator ++ ()
{
  //## begin ClNumRequest::operator++%876334197.body preserve=yes
	m_oMutex.Lock();
	m_iNumRequest ++ ;
	m_oMutex.Unlock ( );

	return *this ;

//	return *this ;
  //## end ClNumRequest::operator++%876334197.body
}

ClNumRequest & ClNumRequest::operator -- ()
{
  //## begin ClNumRequest::operator--%876334198.body preserve=yes
	m_oMutex.Lock();
	m_iNumRequest --;
	m_oMutex.Unlock ( );

	return *this ;
  //## end ClNumRequest::operator--%876334198.body
}

// Additional Declarations
  //## begin ClNumRequest%343BA96500CD.declarations preserve=yes
  //## end ClNumRequest%343BA96500CD.declarations

//## begin module%343E4E890273.epilog preserve=yes
//## end module%343E4E890273.epilog
