//## begin module%3422406300B2.cm preserve=no
//## end module%3422406300B2.cm

//## begin module%3422406300B2.cp preserve=no
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
//## end module%3422406300B2.cp

//## Module: clserviceinfo%3422406300B2; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\clserviceinfo.hpp

#ifndef CLSERVICEINFO_HPP
#define CLSERVICEINFO_HPP 1

//## begin module%3422406300B2.additionalIncludes preserve=no
//## end module%3422406300B2.additionalIncludes

//## begin module%3422406300B2.includes preserve=yes
#include <clserviceinterface.hpp>
#include <exvariables.hpp>
#include <m4log.hpp>
#include <fifoq.hpp>
#include <serviceq.hpp>
#include <cllist.hpp>

class ClServiceVisitor;




//## begin module%3422406300B2.declarations preserve=no
//## end module%3422406300B2.declarations

//## begin module%3422406300B2.additionalDeclarations preserve=yes
#define SERVICE_DEF_STRING_SIZE		50
#define M4_INTERNAL_SERVICE			0
//## end module%3422406300B2.additionalDeclarations


//## begin ClServiceInfo%3422406402B6.preface preserve=yes
//## end ClServiceInfo%3422406402B6.preface

//## Class: ClServiceInfo%3422406402B6
//	This class contain all service refered information separed into General and
//	bath groups, has a flag that means weather the service is abailable or not,
//	too.
//## Category: Dictionary%34183E8601C0
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%345713DF0195;ClServiceVisitor { -> F}
//## Uses: <unnamed>%350E4D9E037D;ClServiceStats { -> F}

class ClServiceInfo : public ClServiceInterface  //## Inherits: <unnamed>%37FB76D90260
{
  //## begin ClServiceInfo%3422406402B6.initialDeclarations preserve=yes
  //## end ClServiceInfo%3422406402B6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClServiceInfo%876496985
      //	Defect constructor with one required parameter that is the output queue. The
      //	other parameters are optionals and set the service queues lenght.
      ClServiceInfo (ClLocalMonitor *ai_poLocalMonitor, ClStatistic *ai_poStatistic, m4uint16_t ai_iMaxNumQItems = 1024);

    //## Destructor (specified)
      //## Operation: ~ClServiceInfo%876496986
      ~ClServiceInfo ();


    //## Other Operations (specified)
      //## Operation: Enable%876496987
      //	Enables the service for proccess requests. It's uses when the user want to
      //	change to enable a disable service.
      //	Inded this method only put a mark in the service.
      void Enable ();

      //## Operation: Disable%876496988
      //	Disables a service. When a service has been disabled won't proccess any more
      //	request, and it will be erased by the Local monitor thread when no request
      //	remind inside.
      void Disable ();

      //## Operation: Remove%876496989; C++
      //	Forces a service to be erased even if it's processing any request.
      virtual m4return_t Remove ();

      //## Operation: Down%876496990; C++
      virtual m4return_t Down ();

      //## Operation: Up%876496991; C++
      virtual m4return_t Up ();

      //## Operation: Accept%878029378
      //	Accepts the visit of a visitor that calculates some statistics about the
      //	object visited.
      void Accept (ClServiceVisitor &ai_oVisitor);

      //## Operation: GetServiceQ%884338124
      ClServiceQ * GetServiceQ ();

      //## Operation: GetQueuedItems%889521619
      ClStatNumericalType GetQueuedItems ();

      //## Operation: GetOutputQ%927559636
      ClOutputQ * GetOutputQ ();

      //## Operation: SetOutputQ%927564337; C++
      void SetOutputQ (ClOutputQ *ai_pOutputQ);

      //## Operation: SetServiceQ%936606376
      void SetServiceQ (ClServiceQ *ai_poService);

      //## Operation: Recycle%941721559; C++
      //	Returns M4_ERROR if CanRecycle() != M4_TRUE, M4_SUCCESS if Recycle is
      //	completed. Other return values may be defined.
      virtual m4return_t Recycle ();

      //## Operation: CanRecycle%941721560
      //	Returns M4_TRUE if the object can be recycled, M4_FALSE otherwise.
      virtual m4bool_t CanRecycle ();

      //## Operation: Destroy%941721561
      //	Returns M4_ERROR if CanDestroy() != M4_TRUE, M4_SUCCESS if Destroy is
      //	completed. Other return Values may be defined.
      virtual m4return_t Destroy ();

      //## Operation: CanDestroy%941721562
      //	Returns M4_TRUE if the object may be destroyed, M4_FALSE otherwise.
      virtual m4bool_t CanDestroy ();

    // Data Members for Associations

      //## Association: Dictionary::<unnamed>%343B7C010132
      //## Role: ClServiceInfo::m_oEnable%343B7C020170
      //## begin ClServiceInfo::m_oEnable%343B7C020170.role preserve=no  public: ClEnable { -> VHAN}
      ClEnable m_oEnable;
      //## end ClServiceInfo::m_oEnable%343B7C020170.role

      //## Association: Dictionary::<unnamed>%34B5F5700140
      //## Role: ClServiceInfo::m_poServiceQueue%34B5F57102F0
      //## begin ClServiceInfo::m_poServiceQueue%34B5F57102F0.role preserve=no  public: ClServiceQ { -> RHAN}
      ClServiceQ *m_poServiceQueue;
      //## end ClServiceInfo::m_poServiceQueue%34B5F57102F0.role

      //## Association: Dictionary::<unnamed>%34B5FAC200B2
      //## Role: ClServiceInfo::m_oLog%34B5FAC30230
      //## begin ClServiceInfo::m_oLog%34B5FAC30230.role preserve=no  public: ClLog { -> VHN}
      ClLog m_oLog;
      //## end ClServiceInfo::m_oLog%34B5FAC30230.role

      //## Association: Dictionary::<unnamed>%34D0655600FD
      //## Role: ClServiceInfo::m_oLauncherList%34D06557014F
      //## begin ClServiceInfo::m_oLauncherList%34D06557014F.role preserve=no  public: ClList { -> VHN}
      ClList m_oLauncherList;
      //## end ClServiceInfo::m_oLauncherList%34D06557014F.role

      //## Association: Dictionary::<unnamed>%37496E0B0190
      //## Role: ClServiceInfo::m_poOutputQueue%37496E0C000B
      //## begin ClServiceInfo::m_poOutputQueue%37496E0C000B.role preserve=no  public: ClOutputQ {1 -> 1RHAN}
      ClOutputQ *m_poOutputQueue;
      //## end ClServiceInfo::m_poOutputQueue%37496E0C000B.role

    // Additional Public Declarations
      //## begin ClServiceInfo%3422406402B6.public preserve=yes
      //## end ClServiceInfo%3422406402B6.public

  protected:
    // Additional Protected Declarations
      //## begin ClServiceInfo%3422406402B6.protected preserve=yes
      //## end ClServiceInfo%3422406402B6.protected

  private:
    // Additional Private Declarations
      //## begin ClServiceInfo%3422406402B6.private preserve=yes
      //## end ClServiceInfo%3422406402B6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServiceInfo%3422406402B6.implementation preserve=yes
      //## end ClServiceInfo%3422406402B6.implementation

};

//## begin ClServiceInfo%3422406402B6.postscript preserve=yes
//## end ClServiceInfo%3422406402B6.postscript

// Class ClServiceInfo 


//## Other Operations (inline)
inline void ClServiceInfo::Enable ()
{
  //## begin ClServiceInfo::Enable%876496987.body preserve=yes
	m_oEnable.SetValue ( M4_TRUE ) ;
  //## end ClServiceInfo::Enable%876496987.body
}

inline void ClServiceInfo::Disable ()
{
  //## begin ClServiceInfo::Disable%876496988.body preserve=yes
	m_oEnable.SetValue ( M4_FALSE ) ;
  //## end ClServiceInfo::Disable%876496988.body
}

//## begin module%3422406300B2.epilog preserve=yes
//## end module%3422406300B2.epilog


#endif
