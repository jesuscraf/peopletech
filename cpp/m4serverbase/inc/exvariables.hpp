//## begin module%3439FEAC0167.cm preserve=no
//## end module%3439FEAC0167.cm

//## begin module%3439FEAC0167.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
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
//	=
//## end module%3439FEAC0167.cp

//## Module: ExVariables%3439FEAC0167; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\exvariables.hpp

#ifndef exvariables_h
#define exvariables_h 1

//## begin module%3439FEAC0167.additionalIncludes preserve=no
#include <m4serverbase_dll.hpp>
#include <m4types.hpp>
#include <syncro.hpp>
#include <m4clock.hpp>


class ClBooleanCondition;

//## begin module%3439FEAC0167.declarations preserve=no
//## end module%3439FEAC0167.declarations

//## begin module%3439FEAC0167.additionalDeclarations preserve=yes
//## end module%3439FEAC0167.additionalDeclarations


//## Class: ClStatus%3439FAD80154
//	This class is usefull for take in each object a reference to its current
//	status. As the status can be modify for many threads there must be
//	synchronize mechanism for avoid simultaneous acceses.
//	But semaphores are limited resources so we can used then uncensoredly so the
//	class has an static semaphore and in that way all the acceses for all the
//	status objects in the server pass through the same semaphore.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3806F9250195;ClPrecisionTicker { -> }
//## Uses: <unnamed>%380AFFCA0314;ClMutex { -> }

class M4_DECL_M4SERVERBASE ClStatus 
{
  //## begin ClStatus%3439FAD80154.initialDeclarations preserve=yes
  //## end ClStatus%3439FAD80154.initialDeclarations

  public:
    //## Class: EnumIntrStatus%36D6BFE90227
    //## Category: <Top Level>
    //## Subsystem: M4ServerBase::inc%378B07C10202
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {STATUS_INTR_NORMAL=0, STATUS_INTR_PAUSING, STATUS_INTR_PAUSED, STATUS_INTR_RESUMING, STATUS_INTR_QUIT, STATUS_INTR_ABORT} EnumIntrStatus;

    //## begin ClStatus::EnumIntrStatus%36D6BFE90227.postscript preserve=yes
    //## end ClStatus::EnumIntrStatus%36D6BFE90227.postscript

  public:
    //## Constructors (specified)
      //## Operation: ClStatus%905450087
      ClStatus (m4uint32_t ai_lIdObject);

    //## Destructor (specified)
      //## Operation: ~ClStatus%905450088
      ~ClStatus ();


    //## Other Operations (specified)
      //## Operation: GetValue%876216047
      m4int16_t GetValue ();

      //## Operation: SetValue%876216048
      void SetValue (m4int16_t ai_iValue);

      //## Operation: GetIntrValue%920037517
      m4int16_t GetIntrValue ();

      //## Operation: SetIntrValue%920037518
      //	Mapa que define los posibles cambios de estado cuando se llama a la funcion
      //	SetIntrValue().
      //
      //			 | NORMAL     | PAUSING    | PAUSED     | RESUMING   |
      //	---------+------------+------------+------------+------------|
      //	NORMAL	 | M4_WARNING | M4_SUCCESS | M4_ERROR   | M4_ERROR   |
      //	---------+------------+------------+------------+------------|
      //	PAUSING  | M4_ERROR	  | M4_WARNING | M4_ERROR   | M4_SUCCESS |
      //	---------+------------+------------+------------+------------|
      //	PAUSED   | M4_ERROR	  |	M4_ERROR   | M4_WARNING | M4_SUCCESS |
      //	---------+------------+------------+------------+------------|
      //	RESUMING | M4_ERROR	  |	M4_SUCCESS | M4_ERROR   | M4_WARNING |
      //	---------+------------+------------+------------+------------|
      //
      //	Actualmente cuando el estado inicial y final es el mismo se esta devolviendo
      //	M4_SUCCESS en vez de M4_WARNING.
      m4return_t SetIntrValue (m4int16_t ai_iNewStatus);

      //## Operation: GetIdObject%928833186
      m4uint32_t GetIdObject ();

      //## Operation: SetBoolCondIntrVal%939229284
      void SetBoolCondIntrVal (ClBooleanCondition* const ai_pBoolCond);

      //## Operation: GetBoolCondIntrVal%939229285
      ClBooleanCondition* GetBoolCondIntrVal () const;

      //## Operation: SetTimeOut%939981144; C++
      //	This method set the number of milliseconds to go from the time it is called.
      //	Returns M4_ERROR if Status != Normal, M4_SUCCESS otherwise.
      m4return_t SetTimeOut (m4uint_t ai_iTimeout);

      //## Operation: ResetTimeOut%939981145
      //	This operation resets the time out value (Infinite)
      void ResetTimeOut ();

      //## Operation: GetTimeToGo%939981146; C++
      //	This operation gets the milliseconds to go before timeout if there is one.
      //	Returns M4_ERROR if timeout has been reached, M4_WARNING if there is some
      //	time to go and M4_SUCCESS if no timeout has been set.
      m4return_t GetTimeToGo (m4uint_t &ao_iTimeout);

      //## Operation: HasTimedOut%940323118; C++
      m4bool_t HasTimedOut ();

      //## Operation: UpdateTime%948725626
      static void UpdateTime ();

      //## Operation: IncrPauseLevel%991075928
      void IncrPauseLevel (m4int16_t ai_iIncrLevel);

      //## Operation: GetIntrValueForAdmin%1057061436
      m4int16_t GetIntrValueForAdmin ();

	  ClStatus* GetNextStatus (void) const;
	  void SetNextStatus (ClStatus* ai_poNextStatus);

    // Data Members for Class Attributes

      //## Attribute: InfiniteTimeOut%3847A815039E
      //## begin ClStatus::InfiniteTimeOut%3847A815039E.attr preserve=no  public: static m4uint_t {VAC} 0
      static const m4uint_t  InfiniteTimeOut;
      //## end ClStatus::InfiniteTimeOut%3847A815039E.attr

    // Additional Public Declarations
      //## begin ClStatus%3439FAD80154.public preserve=yes
      //## end ClStatus%3439FAD80154.public

  protected:
    // Data Members for Class Attributes

      //## begin ClStatus::PauseLevel%3B129BA40066.attr preserve=no  private: m4int16_t {U} 0
      m4int16_t m_iPauseLevel;
      //## end ClStatus::PauseLevel%3B129BA40066.attr

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%37FB7C9201F5
      //## Role: ClStatus::m_pbcIntrVal%37FB7C930048
      //## begin ClStatus::m_pbcIntrVal%37FB7C930048.role preserve=no  protected: ClBooleanCondition { -> 1RFHAN}
      ClBooleanCondition *m_pbcIntrVal;
      //## end ClStatus::m_pbcIntrVal%37FB7C930048.role

      //## Association: M4ServerBase::<unnamed>%3806FA7F03C3
      //## Role: ClStatus::m_iStartExecTime%3806FA800144
      //## begin ClStatus::m_iStartExecTime%3806FA800144.role preserve=no  protected: m4millisec_t { -> 1VHAN}
      m4millisec_t m_iStartExecTime;
      //## end ClStatus::m_iStartExecTime%3806FA800144.role

      //## Association: M4ServerBase::<unnamed>%380B34E70149
      //## Role: ClStatus::m_bHasTimedOut%380B34E8019A
      //## begin ClStatus::m_bHasTimedOut%380B34E8019A.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_bHasTimedOut;
      //## end ClStatus::m_bHasTimedOut%380B34E8019A.role

      //## Association: M4ServerBase::<unnamed>%3847A55B0368
      //## Role: ClStatus::m_iTimeToGo%3847A55C014D
      //## begin ClStatus::m_iTimeToGo%3847A55C014D.role preserve=no  protected: m4uint32_t { -> 1VHAN}
      m4uint32_t m_iTimeToGo;
      //## end ClStatus::m_iTimeToGo%3847A55C014D.role

      //## Association: M4ServerBase::<unnamed>%388C67CA00C2
      //## Role: ClStatus::m_iCurrentTime%388C67CA0249
      //## begin ClStatus::m_iCurrentTime%388C67CA0249.role preserve=no  protected: static m4uint32_t {n -> 1VHAN}
      static m4uint32_t m_iCurrentTime;
      //## end ClStatus::m_iCurrentTime%388C67CA0249.role

    // Additional Protected Declarations
      //## begin ClStatus%3439FAD80154.protected preserve=yes
      //## end ClStatus%3439FAD80154.protected

  private:
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: PauseLevel%3B129BA40066
      const m4int16_t& GetPauseLevel () const;

    // Data Members for Class Attributes

      //## Attribute: m_Status%3439FAEC01CB
      //	This variable contain the status of its connection, the status can take the
      //	values of connecting, active or desconnected.
      //## begin ClStatus::m_Status%3439FAEC01CB.attr preserve=no  private: m4int16_t {UA} 
      m4int16_t m_Status;
      //## end ClStatus::m_Status%3439FAEC01CB.attr

      //## Attribute: m_oMutex%3439FBCE03A6
      //	This is a static member because can't be a semaphore for each connection in
      //	the system.
      //## begin ClStatus::m_oMutex%3439FBCE03A6.attr preserve=no  private: static ClMutex {UA} 
      static ClMutex m_oMutex;
      //## end ClStatus::m_oMutex%3439FBCE03A6.attr

      //## Attribute: m_bInitialized%35F8122402D5
      //## begin ClStatus::m_bInitialized%35F8122402D5.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
      static m4bool_t m_bInitialized;
      //## end ClStatus::m_bInitialized%35F8122402D5.attr

      //## Attribute: m_IntrStatus%36D6B66900A3
      //	This variable contain the status of its connection, the status can take the
      //	values of connecting, active or desconnected.
      //## begin ClStatus::m_IntrStatus%36D6B66900A3.attr preserve=no  private: m4int16_t {UA} STATUS_INTR_NORMAL
      m4int16_t m_IntrStatus;
      //## end ClStatus::m_IntrStatus%36D6B66900A3.attr

      //## Attribute: m_lIdObject%375BCC7700AC
      //## begin ClStatus::m_lIdObject%375BCC7700AC.attr preserve=no  private: m4uint32_t {UA} ai_lIdObject
      m4uint32_t m_lIdObject;
      //## end ClStatus::m_lIdObject%375BCC7700AC.attr

      ClStatus *m_poNextStatus;

    // Additional Private Declarations
      //## begin ClStatus%3439FAD80154.private preserve=yes
      //## end ClStatus%3439FAD80154.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClStatus%3439FAD80154.implementation preserve=yes
      //## end ClStatus%3439FAD80154.implementation

};

//## begin ClStatus%3439FAD80154.postscript preserve=yes
//## end ClStatus%3439FAD80154.postscript

//## Class: ClEnable%343B7B780063
//	This class has exactly the same philosophy that the status class and it is
//	used in the same way.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClEnable 
{
  //## begin ClEnable%343B7B780063.initialDeclarations preserve=yes
  //## end ClEnable%343B7B780063.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClEnable%905450089
      ClEnable ();

    //## Destructor (specified)
      //## Operation: ~ClEnable%905450090
      ~ClEnable ();


    //## Other Operations (specified)
      //## Operation: GetValue%876313635
      m4bool_t GetValue ();

      //## Operation: SetValue%876313636
      void SetValue (m4bool_t ai_bValue);

    // Additional Public Declarations
      //## begin ClEnable%343B7B780063.public preserve=yes
      //## end ClEnable%343B7B780063.public

  protected:
    // Additional Protected Declarations
      //## begin ClEnable%343B7B780063.protected preserve=yes
      //## end ClEnable%343B7B780063.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_Enable%343B7B860063
      //## begin ClEnable::m_Enable%343B7B860063.attr preserve=no  private: m4bool_t {UA} 
      m4bool_t m_Enable;
      //## end ClEnable::m_Enable%343B7B860063.attr

      //## Attribute: m_oMutex%343B7B9C0354
      //## begin ClEnable::m_oMutex%343B7B9C0354.attr preserve=no  private: static ClSemaphore {UA} 
      static ClMutex m_oMutex;
      //## end ClEnable::m_oMutex%343B7B9C0354.attr

      //## Attribute: m_bInitialized%35F8121B0083
      //## begin ClEnable::m_bInitialized%35F8121B0083.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
      static m4bool_t m_bInitialized;
      //## end ClEnable::m_bInitialized%35F8121B0083.attr

    // Additional Private Declarations
      //## begin ClEnable%343B7B780063.private preserve=yes
      //## end ClEnable%343B7B780063.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClEnable%343B7B780063.implementation preserve=yes
      //## end ClEnable%343B7B780063.implementation

};

//## begin ClEnable%343B7B780063.postscript preserve=yes
//## end ClEnable%343B7B780063.postscript

//## Class: ClNumRequest%343BA96500CD
//	The same as the clstatus and the clenable classes.
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClNumRequest 
{
  //## begin ClNumRequest%343BA96500CD.initialDeclarations preserve=yes
  //## end ClNumRequest%343BA96500CD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNumRequest%905450091
      ClNumRequest ();

    //## Destructor (specified)
      //## Operation: ~ClNumRequest%905450092
      ~ClNumRequest ();


    //## Other Operations (specified)
      //## Operation: GetValue%876334195
      m4int32_t GetValue ();

      //## Operation: SetValue%876334196
      void SetValue (m4int32_t ai_lValue);

      //## Operation: operator++%876334197
      ClNumRequest & operator ++ ();

      //## Operation: operator--%876334198
      ClNumRequest & operator -- ();

    // Additional Public Declarations
      //## begin ClNumRequest%343BA96500CD.public preserve=yes
      //## end ClNumRequest%343BA96500CD.public

  protected:
    // Additional Protected Declarations
      //## begin ClNumRequest%343BA96500CD.protected preserve=yes
      //## end ClNumRequest%343BA96500CD.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iNumRequest%343BA974025F
      //## begin ClNumRequest::m_iNumRequest%343BA974025F.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iNumRequest;
      //## end ClNumRequest::m_iNumRequest%343BA974025F.attr

      //## Attribute: m_oMutex%343BA99B01D9
      //## begin ClNumRequest::m_oMutex%343BA99B01D9.attr preserve=no  private: static ClMutex {VA} 
      static ClMutex m_oMutex;
      //## end ClNumRequest::m_oMutex%343BA99B01D9.attr

      //## Attribute: m_bInitialized%35F81203011F
      //## begin ClNumRequest::m_bInitialized%35F81203011F.attr preserve=no  private: static m4bool_t {UA} M4_FALSE
      static m4bool_t m_bInitialized;
      //## end ClNumRequest::m_bInitialized%35F81203011F.attr

    // Additional Private Declarations
      //## begin ClNumRequest%343BA96500CD.private preserve=yes
      //## end ClNumRequest%343BA96500CD.private

  private:  //## implementation
    // Additional Implementation Declarations
      //## begin ClNumRequest%343BA96500CD.implementation preserve=yes
      //## end ClNumRequest%343BA96500CD.implementation

};

//## begin ClNumRequest%343BA96500CD.postscript preserve=yes
//## end ClNumRequest%343BA96500CD.postscript

// Class ClStatus 


//## Other Operations (inline)
inline m4uint32_t ClStatus::GetIdObject ()
{
  //## begin ClStatus::GetIdObject%928833186.body preserve=yes
	return m_lIdObject ;
  //## end ClStatus::GetIdObject%928833186.body
}

inline m4return_t ClStatus::SetTimeOut (m4uint_t ai_iTimeout)
{
  //## begin ClStatus::SetTimeOut%939981144.body preserve=yes
	if (STATUS_INTR_NORMAL != m_IntrStatus)		return M4_ERROR;

	m_iTimeToGo = ai_iTimeout;
	m_iStartExecTime = m4millisec_t(m_iCurrentTime) * 1000;
	m_bHasTimedOut = M4_FALSE;

	return M4_SUCCESS;
  //## end ClStatus::SetTimeOut%939981144.body
}

inline void ClStatus::ResetTimeOut ()
{
  //## begin ClStatus::ResetTimeOut%939981145.body preserve=yes
	m_iTimeToGo = 0;
	m_iStartExecTime = 0;
	m_bHasTimedOut = M4_FALSE;
  //## end ClStatus::ResetTimeOut%939981145.body
}

inline m4bool_t ClStatus::HasTimedOut ()
{
  //## begin ClStatus::HasTimedOut%940323118.body preserve=yes
	if (M4_TRUE == m_bHasTimedOut)	return M4_TRUE;

	m4uint_t		msecs;
	m4bool_t		ret = (M4_ERROR == GetTimeToGo(msecs)? M4_TRUE : M4_FALSE);

	return ret;
  //## end ClStatus::HasTimedOut%940323118.body
}

inline void ClStatus::IncrPauseLevel (m4int16_t ai_iIncrLevel)
{
  //## begin ClStatus::IncrPauseLevel%991075928.body preserve=yes
	m_iPauseLevel += ai_iIncrLevel;

	if (m_iPauseLevel < 0)
		m_iPauseLevel = 0;
  //## end ClStatus::IncrPauseLevel%991075928.body
}

//## Get and Set Operations for Class Attributes (inline)

inline const m4int16_t& ClStatus::GetPauseLevel () const
{
  //## begin ClStatus::GetPauseLevel%3B129BA40066.get preserve=no
  return m_iPauseLevel;
  //## end ClStatus::GetPauseLevel%3B129BA40066.get
}

// Class ClEnable 

// Class ClNumRequest 

//## begin module%3439FEAC0167.epilog preserve=yes
//## end module%3439FEAC0167.epilog


#endif
