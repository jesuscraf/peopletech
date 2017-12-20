//## begin module%3777684501FD.cm preserve=no
//## end module%3777684501FD.cm

//## begin module%3777684501FD.cp preserve=no
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
//## end module%3777684501FD.cp

//## Module: windowstsapselectimpl%3777684501FD; Package specification
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//	d:\compon\m4tsap\version\inc\win32
//## Source file: d:\compon\m4tsap\version\inc\win32\windowstsapselectimpl.hpp

#ifndef windowstsapselec_h
#define windowstsapselec_h 1

//## begin module%3777684501FD.additionalIncludes preserve=no
//## end module%3777684501FD.additionalIncludes

//## begin module%3777684501FD.includes preserve=yes

#include "m4stl.hpp"
#include "syncro.hpp"
#include "m4types.hpp"
#include "tsap.hpp"
#include "windowsselectmask.hpp"
#include "windowstsapimpl.hpp"
#include "selectimpl.hpp"
//## end module%3777684501FD.includes

// windowsselectmask
#include "windowsselectmask.hpp"
//## begin module%3777684501FD.declarations preserve=no
//## end module%3777684501FD.declarations

//## begin module%3777684501FD.additionalDeclarations preserve=yes





//## end module%3777684501FD.additionalDeclarations


//## Class: ClListPTSAP%377768470022; Instantiated Class
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef map< int , ClTSAP * , less < int > > ClListPTSAP;

//## begin ClWindowsTSAPSelectImpl%377768450258.preface preserve=yes
//## end ClWindowsTSAPSelectImpl%377768450258.preface

//## Class: ClWindowsTSAPSelectImpl%377768450258
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc::win32%3777684500DB
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%377768450281;ClTSAP { -> }
//## Uses: <unnamed>%377768450282;m4bool_t { -> }
//## Uses: <unnamed>%377768450293;m4return_t { -> }
//## Uses: <unnamed>%3777684603AD;ClMutex { -> }
//## Uses: <unnamed>%3777684603CB;m4int8_t { -> }

class ClWindowsTSAPSelectImpl : public ClTSAPSelectImpl  //## Inherits: <unnamed>%377768470040
{
  //## begin ClWindowsTSAPSelectImpl%377768450258.initialDeclarations preserve=yes
  //## end ClWindowsTSAPSelectImpl%377768450258.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClWindowsTSAPSelectImpl%-568568961; C++
      ClWindowsTSAPSelectImpl (unsigned int nTSAPs = 0);

    //## Destructor (specified)
      //## Operation: ~ClWindowsTSAPSelectImpl%-1022061054; C++
      ~ClWindowsTSAPSelectImpl ();


    //## Other Operations (specified)
      //## Operation: Add%688489493; C++
      void Add (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite, m4bool_t ai_bExceptions);

      //## Operation: Remove%-312137576; C++
      m4return_t Remove (ClTSAP* s);
      m4return_t Remove (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite,m4bool_t ai_bExceptions);

      //## Operation: Select%1685621972; C++
      int Select (ClMutex* ao_pMutex, unsigned long ai_Segundos = 0L, unsigned long ai_Microsegundos = 0L, m4bool_t ai_bRead = M4_TRUE, m4bool_t ai_bWrite = M4_TRUE, m4bool_t ai_bExceptions = M4_TRUE);

      //## Operation: NextRead%1071327791; C++
      ClTSAP* NextRead ();

      //## Operation: NextWrite%525744225; C++
      ClTSAP* NextWrite ();

      //## Operation: NextExceptions%-1452603615; C++
      ClTSAP* NextExceptions ();

      //## Operation: IsInMask%-983664596; C++
      m4bool_t IsInMask (ClTSAP* ai_poTSAP, m4int8_t ai_iTipoMascara);

    // Additional Public Declarations
      //## begin ClWindowsTSAPSelectImpl%377768450258.public preserve=yes
      //## end ClWindowsTSAPSelectImpl%377768450258.public

  protected:
    // Additional Protected Declarations
      //## begin ClWindowsTSAPSelectImpl%377768450258.protected preserve=yes
      //## end ClWindowsTSAPSelectImpl%377768450258.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: iNumTSAPs%3777684603D5
      //## begin ClWindowsTSAPSelectImpl::iNumTSAPs%3777684603D5.attr preserve=no  private: unsigned int {VA} 
      unsigned int iNumTSAPs;
      //## end ClWindowsTSAPSelectImpl::iNumTSAPs%3777684603D5.attr

      //## Attribute: iNextRead%3777684603DF
      //## begin ClWindowsTSAPSelectImpl::iNextRead%3777684603DF.attr preserve=no  private: unsigned int {VA} 
      unsigned int iNextRead;
      //## end ClWindowsTSAPSelectImpl::iNextRead%3777684603DF.attr

      //## Attribute: iNextWrite%3777684603E0
      //## begin ClWindowsTSAPSelectImpl::iNextWrite%3777684603E0.attr preserve=no  private: unsigned int {VA} 
      unsigned int iNextWrite;
      //## end ClWindowsTSAPSelectImpl::iNextWrite%3777684603E0.attr

      //## Attribute: iNextExceptions%3777684603E1
      //## begin ClWindowsTSAPSelectImpl::iNextExceptions%3777684603E1.attr preserve=no  private: unsigned int {VA} 
      unsigned int iNextExceptions;
      //## end ClWindowsTSAPSelectImpl::iNextExceptions%3777684603E1.attr

    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%377768470001
      //## Role: ClWindowsTSAPSelectImpl::m_poMasterReadMask%377768470003
      //## begin ClWindowsTSAPSelectImpl::m_poMasterReadMask%377768470003.role preserve=no  private: ClWindowsSelectMask {1 -> 1RHAPN}
      ClWindowsSelectMask *m_poMasterReadMask;
      //## end ClWindowsTSAPSelectImpl::m_poMasterReadMask%377768470003.role

      //## Association: M4Tsap::<unnamed>%377768470004
      //## Role: ClWindowsTSAPSelectImpl::m_poMasterWriteMask%37776847000B
      //## begin ClWindowsTSAPSelectImpl::m_poMasterWriteMask%37776847000B.role preserve=no  private: ClWindowsSelectMask {1 -> 1RHAPN}
      ClWindowsSelectMask *m_poMasterWriteMask;
      //## end ClWindowsTSAPSelectImpl::m_poMasterWriteMask%37776847000B.role

      //## Association: M4Tsap::<unnamed>%37776847000C
      //## Role: ClWindowsTSAPSelectImpl::m_poMasterExceptionsMask%37776847000E
      //## begin ClWindowsTSAPSelectImpl::m_poMasterExceptionsMask%37776847000E.role preserve=no  private: ClWindowsSelectMask {1 -> 1RHAPN}
      ClWindowsSelectMask *m_poMasterExceptionsMask;
      //## end ClWindowsTSAPSelectImpl::m_poMasterExceptionsMask%37776847000E.role

      //## Association: M4Tsap::<unnamed>%37776847000F
      //## Role: ClWindowsTSAPSelectImpl::m_poReadMask%377768470011
      //## begin ClWindowsTSAPSelectImpl::m_poReadMask%377768470011.role preserve=no  private: ClWindowsSelectMask {1 -> 1RHAPN}
      ClWindowsSelectMask *m_poReadMask;
      //## end ClWindowsTSAPSelectImpl::m_poReadMask%377768470011.role

      //## Association: M4Tsap::<unnamed>%377768470015
      //## Role: ClWindowsTSAPSelectImpl::m_poWriteMask%377768470017
      //## begin ClWindowsTSAPSelectImpl::m_poWriteMask%377768470017.role preserve=no  private: ClWindowsSelectMask {1 -> 1RHAPN}
      ClWindowsSelectMask *m_poWriteMask;
      //## end ClWindowsTSAPSelectImpl::m_poWriteMask%377768470017.role

      //## Association: M4Tsap::<unnamed>%37776847001F
      //## Role: ClWindowsTSAPSelectImpl::m_poExceptionsMask%377768470021
      //## begin ClWindowsTSAPSelectImpl::m_poExceptionsMask%377768470021.role preserve=no  private: ClWindowsSelectMask {1 -> 1RHAPN}
      ClWindowsSelectMask *m_poExceptionsMask;
      //## end ClWindowsTSAPSelectImpl::m_poExceptionsMask%377768470021.role

      //## Association: M4Tsap::<unnamed>%37776847003D
      //## Role: ClWindowsTSAPSelectImpl::m_oaTSAPList%37776847003F
      //## begin ClWindowsTSAPSelectImpl::m_oaTSAPList%37776847003F.role preserve=no  private: ClListPTSAP {1 -> 1VHAPN}
      ClListPTSAP m_oaTSAPList;
      //## end ClWindowsTSAPSelectImpl::m_oaTSAPList%37776847003F.role

    // Additional Private Declarations
      //## begin ClWindowsTSAPSelectImpl%377768450258.private preserve=yes
      //## end ClWindowsTSAPSelectImpl%377768450258.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClWindowsTSAPSelectImpl%377768450258.implementation preserve=yes
      //## end ClWindowsTSAPSelectImpl%377768450258.implementation

};

//## begin ClWindowsTSAPSelectImpl%377768450258.postscript preserve=yes
//## end ClWindowsTSAPSelectImpl%377768450258.postscript

// Class ClWindowsTSAPSelectImpl 

//## begin module%3777684501FD.epilog preserve=yes


typedef ClTSAP* _Ty;


//## end module%3777684501FD.epilog


#endif
