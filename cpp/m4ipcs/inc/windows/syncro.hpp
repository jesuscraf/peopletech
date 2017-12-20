//## begin module%3742D01E02DA.cm preserve=no
//## end module%3742D01E02DA.cm

//## begin module%3742D01E02DA.cp preserve=no
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
//## end module%3742D01E02DA.cp

//## Module: syncro%3742D01E02DA; Package specification
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//	d:\fuentesservidor\m4ipcs\version\inc\win32
//## Source file: D:\eduardoma\m4ipcs\inc\win32\syncro.hpp

#ifndef __SEMAPHOR__HPP__
#define __SEMAPHOR__HPP__ 1

//## begin module%3742D01E02DA.additionalIncludes preserve=no
//## end module%3742D01E02DA.additionalIncludes

//## begin module%3742D01E02DA.includes preserve=yes
//#include <stdio.h>
#include <windows.h>
#include <m4ipcs_dll.hpp>
//## end module%3742D01E02DA.includes

// m4types
#include <m4types.hpp>
//## begin module%3742D01E02DA.declarations preserve=no
//## end module%3742D01E02DA.declarations

//## begin module%3742D01E02DA.additionalDeclarations preserve=yes
#define HANDLE_NULL		NULL
//## end module%3742D01E02DA.additionalDeclarations


//## begin ClSynObject%3742D01F0284.preface preserve=yes
//## end ClSynObject%3742D01F0284.preface

//## Class: ClSynObject%3742D01F0284; Abstract
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3742D0220311;m4pchar_t { -> }
//## Uses: <unnamed>%3742D0220316;m4return_t { -> }

class M4_DECL_M4IPCS ClSynObject 
{
  //## begin ClSynObject%3742D01F0284.initialDeclarations preserve=yes
;

		
  //## end ClSynObject%3742D01F0284.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSynObject%1118752233; C++
      ClSynObject (m4pchar_t ai_szName = NULL)
        //## begin ClSynObject::ClSynObject%1118752233.hasinit preserve=no
        //## end ClSynObject::ClSynObject%1118752233.hasinit
        //## begin ClSynObject::ClSynObject%1118752233.initialization preserve=yes
        //## end ClSynObject::ClSynObject%1118752233.initialization
      {
        //## begin ClSynObject::ClSynObject%1118752233.body preserve=yes
 ; 
        //## end ClSynObject::ClSynObject%1118752233.body
      }

    //## Destructor (specified)
      //## Operation: ~ClSynObject%1253969443; C++
      virtual ~ClSynObject (void )
      {
        //## begin ClSynObject::~ClSynObject%1253969443.body preserve=yes
 ; 
        //## end ClSynObject::~ClSynObject%1253969443.body
      }


    //## Other Operations (specified)
      //## Operation: Lock%-946369175; C++
      virtual m4return_t Lock (void ) = 0;

      //## Operation: Unlock%-1498751921; C++
      virtual m4return_t Unlock (void ) = 0;

    // Additional Public Declarations
      //## begin ClSynObject%3742D01F0284.public preserve=yes
      //## end ClSynObject%3742D01F0284.public

  protected:
    // Additional Protected Declarations
      //## begin ClSynObject%3742D01F0284.protected preserve=yes
      //## end ClSynObject%3742D01F0284.protected

  private:
    // Additional Private Declarations
      //## begin ClSynObject%3742D01F0284.private preserve=yes
      //## end ClSynObject%3742D01F0284.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSynObject%3742D01F0284.implementation preserve=yes
      //## end ClSynObject%3742D01F0284.implementation

};

//## begin ClSynObject%3742D01F0284.postscript preserve=yes
//## end ClSynObject%3742D01F0284.postscript

//## begin ClMutex%3742D01F0263.preface preserve=yes
//## end ClMutex%3742D01F0263.preface

//## Class: ClMutex%3742D01F0263
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3742D01F02B0;m4return_t { -> }

class M4_DECL_M4IPCS ClMutex : public ClSynObject  //## Inherits: <unnamed>%3742D01F0337
{
  //## begin ClMutex%3742D01F0263.initialDeclarations preserve=yes
  //## end ClMutex%3742D01F0263.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMutex%-1395052867; C++
      ClMutex (void );

      //## Operation: ClMutex%909182102; C++
      ClMutex (m4bool_t ao_bInit);

    //## Destructor (specified)
      //## Operation: ~ClMutex%-429500371; C++
      ~ClMutex (void );


    //## Other Operations (specified)
      //## Operation: Lock%-1740955238; C++
      m4return_t Lock (void );

      //## Operation: Unlock%718283932; C++
      m4return_t Unlock (void );

      //## Operation: Handle%1692303511; C++
      HANDLE Handle ()
      {
        //## begin ClMutex::Handle%1692303511.body preserve=yes
			return m_hMutexHandle;
		
        //## end ClMutex::Handle%1692303511.body
      }

      //## Operation: Init%1902890713; C++
      m4return_t Init (void );

    // Additional Public Declarations
      //## begin ClMutex%3742D01F0263.public preserve=yes
      //## end ClMutex%3742D01F0263.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_hMutexHandle%3742D01F032D
      //## begin ClMutex::m_hMutexHandle%3742D01F032D.attr preserve=no  protected: HANDLE {VA} 
      HANDLE m_hMutexHandle;
      //## end ClMutex::m_hMutexHandle%3742D01F032D.attr

    // Additional Protected Declarations
      //## begin ClMutex%3742D01F0263.protected preserve=yes
      //## end ClMutex%3742D01F0263.protected

  private:
    // Additional Private Declarations
      //## begin ClMutex%3742D01F0263.private preserve=yes
      //## end ClMutex%3742D01F0263.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMutex%3742D01F0263.implementation preserve=yes
      //## end ClMutex%3742D01F0263.implementation

};

//## begin ClMutex%3742D01F0263.postscript preserve=yes
//## end ClMutex%3742D01F0263.postscript

//## begin ClSemaphore%3742D01F027D.preface preserve=yes
//## end ClSemaphore%3742D01F027D.preface

//## Class: ClSemaphore%3742D01F027D
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClSemaphore : public ClSynObject  //## Inherits: <unnamed>%3742D01F036B
{
  //## begin ClSemaphore%3742D01F027D.initialDeclarations preserve=yes
  //## end ClSemaphore%3742D01F027D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSemaphore%-1086119639; C++
      ClSemaphore (m4int16_t ai_lInitialCount = 1, m4int16_t ai_lMaxCount = 1, m4pchar_t ai_pstrName = NULL);

    //## Destructor (specified)
      //## Operation: ~ClSemaphore%-771045020; C++
      ~ClSemaphore (void );


    //## Other Operations (specified)
      //## Operation: Lock%1224834127; C++
      m4return_t Lock (void );

      //## Operation: Unlock%-641722682; C++
      m4return_t Unlock (void );

      //## Operation: Open%-1129495857; C++
      m4int16_t Open (m4int16_t ai_lInitialCount, m4int16_t ai_lMaxCount, m4pchar_t ai_strName);

      //## Operation: IsSemCreate%-645605075; C++
      m4bool_t IsSemCreate (void );

      //## Operation: Handle%770195208; C++
      HANDLE Handle ()
      {
        //## begin ClSemaphore::Handle%770195208.body preserve=yes
			return m_hHandle;
		
        //## end ClSemaphore::Handle%770195208.body
      }

    // Additional Public Declarations
      //## begin ClSemaphore%3742D01F027D.public preserve=yes
      //## end ClSemaphore%3742D01F027D.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_hHandle%3742D01F0360
      //## begin ClSemaphore::m_hHandle%3742D01F0360.attr preserve=no  protected: HANDLE {VA} 
      HANDLE m_hHandle;
      //## end ClSemaphore::m_hHandle%3742D01F0360.attr

    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743CE780008
      //## Role: ClSemaphore::m_create%3743CE78033D
      //## begin ClSemaphore::m_create%3743CE78033D.role preserve=no  protected: m4bool_t { -> 1VHAN}
      m4bool_t m_create;
      //## end ClSemaphore::m_create%3743CE78033D.role

    // Additional Protected Declarations
      //## begin ClSemaphore%3742D01F027D.protected preserve=yes
      //## end ClSemaphore%3742D01F027D.protected

  private:
    // Additional Private Declarations
      //## begin ClSemaphore%3742D01F027D.private preserve=yes
      //## end ClSemaphore%3742D01F027D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSemaphore%3742D01F027D.implementation preserve=yes
      //## end ClSemaphore%3742D01F027D.implementation

};

//## begin ClSemaphore%3742D01F027D.postscript preserve=yes
//## end ClSemaphore%3742D01F027D.postscript

//## begin ClSemaphoreForWaiting%3742D022031B.preface preserve=yes
//## end ClSemaphoreForWaiting%3742D022031B.preface

//## Class: ClSemaphoreForWaiting%3742D022031B
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClSemaphoreForWaiting 
{
  //## begin ClSemaphoreForWaiting%3742D022031B.initialDeclarations preserve=yes
  //## end ClSemaphoreForWaiting%3742D022031B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSemaphoreForWaiting%1423602053; C++
      ClSemaphoreForWaiting (m4int16_t ai_iForSemaphoreWaiting);

      //## Operation: ClSemaphoreForWaiting%-1520535565; C++
      ClSemaphoreForWaiting (void );

    //## Destructor (specified)
      //## Operation: ~ClSemaphoreForWaiting%827792698; C++
      ~ClSemaphoreForWaiting (void );


    //## Other Operations (specified)
      //## Operation: Lock%-435247507; C++
      m4int16_t Lock ();

      //## Operation: Unlock%-371131849; C++
      m4int16_t Unlock ();

      //## Operation: Init%586533056; C++
      m4int16_t Init (void );

      //## Operation: SetNumSemaphoreForWaiting%-332891486; C++
      m4int16_t SetNumSemaphoreForWaiting (m4int16_t ai_iForSemaphoreWaiting);

    // Additional Public Declarations
      //## begin ClSemaphoreForWaiting%3742D022031B.public preserve=yes
      //## end ClSemaphoreForWaiting%3742D022031B.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743CBF100E4
      //## Role: ClSemaphoreForWaiting::m_smpUsing%3743CBF2005A
      //## begin ClSemaphoreForWaiting::m_smpUsing%3743CBF2005A.role preserve=no  protected: ClSemaphore {1 -> 1RHAN}
      ClSemaphore *m_smpUsing;
      //## end ClSemaphoreForWaiting::m_smpUsing%3743CBF2005A.role

      //## Association: M4Ipcs::<unnamed>%3743CC1D0119
      //## Role: ClSemaphoreForWaiting::m_smpSincro%3743CC1E0193
      //## begin ClSemaphoreForWaiting::m_smpSincro%3743CC1E0193.role preserve=no  protected: ClSemaphore { -> 1RHAN}
      ClSemaphore *m_smpSincro;
      //## end ClSemaphoreForWaiting::m_smpSincro%3743CC1E0193.role

      //## Association: M4Ipcs::<unnamed>%3743D03E02D2
      //## Role: ClSemaphoreForWaiting::m_iEnded%3743D03F0392
      //## begin ClSemaphoreForWaiting::m_iEnded%3743D03F0392.role preserve=no  protected: m4int16_t { -> 1VHAN}
      m4int16_t m_iEnded;
      //## end ClSemaphoreForWaiting::m_iEnded%3743D03F0392.role

      //## Association: M4Ipcs::<unnamed>%3743D06101B9
      //## Role: ClSemaphoreForWaiting::m_iNumTotalForWaiting%3743D0620067
      //## begin ClSemaphoreForWaiting::m_iNumTotalForWaiting%3743D0620067.role preserve=no  protected: m4int16_t { -> 1VHAN}
      m4int16_t m_iNumTotalForWaiting;
      //## end ClSemaphoreForWaiting::m_iNumTotalForWaiting%3743D0620067.role

    // Additional Protected Declarations
      //## begin ClSemaphoreForWaiting%3742D022031B.protected preserve=yes
      //## end ClSemaphoreForWaiting%3742D022031B.protected

  private:
    // Additional Private Declarations
      //## begin ClSemaphoreForWaiting%3742D022031B.private preserve=yes
      //## end ClSemaphoreForWaiting%3742D022031B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSemaphoreForWaiting%3742D022031B.implementation preserve=yes
      //## end ClSemaphoreForWaiting%3742D022031B.implementation

};

//## begin ClSemaphoreForWaiting%3742D022031B.postscript preserve=yes
//## end ClSemaphoreForWaiting%3742D022031B.postscript

//## begin ClSemBlock%3742D022034E.preface preserve=yes
//## end ClSemBlock%3742D022034E.preface

//## Class: ClSemBlock%3742D022034E
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClSemBlock 
{
  //## begin ClSemBlock%3742D022034E.initialDeclarations preserve=yes
  //## end ClSemBlock%3742D022034E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSemBlock%-826340450; C++
      ClSemBlock (ClSemaphore* a_poSem);

    //## Destructor (specified)
      //## Operation: ~ClSemBlock%528873236; C++
      ~ClSemBlock ();

    // Additional Public Declarations
      //## begin ClSemBlock%3742D022034E.public preserve=yes
      //## end ClSemBlock%3742D022034E.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743CC3C0164
      //## Role: ClSemBlock::m_poSem%3743CC3D00F8
      //## begin ClSemBlock::m_poSem%3743CC3D00F8.role preserve=no  protected: ClSemaphore { -> RHAN}
      ClSemaphore *m_poSem;
      //## end ClSemBlock::m_poSem%3743CC3D00F8.role

    // Additional Protected Declarations
      //## begin ClSemBlock%3742D022034E.protected preserve=yes
      //## end ClSemBlock%3742D022034E.protected

  private:
    // Additional Private Declarations
      //## begin ClSemBlock%3742D022034E.private preserve=yes
      //## end ClSemBlock%3742D022034E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSemBlock%3742D022034E.implementation preserve=yes
      //## end ClSemBlock%3742D022034E.implementation

};

//## begin ClSemBlock%3742D022034E.postscript preserve=yes
//## end ClSemBlock%3742D022034E.postscript

//## begin ClMutBlock2%3742D022035D.preface preserve=yes
//## end ClMutBlock2%3742D022035D.preface

//## Class: ClMutBlock2%3742D022035D
//## Category: M4Ipcs%3742D01E02E3
//## Subsystem: M4Ipcs::inc::win32%3742D01E02D9
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4IPCS ClMutBlock2 
{
  //## begin ClMutBlock2%3742D022035D.initialDeclarations preserve=yes
  //## end ClMutBlock2%3742D022035D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClMutBlock2%1908518068; C++
      ClMutBlock2 (ClMutex* a_poMut);

    //## Destructor (specified)
      //## Operation: ~ClMutBlock2%1253030407; C++
      ~ClMutBlock2 ();

    // Additional Public Declarations
      //## begin ClMutBlock2%3742D022035D.public preserve=yes
      //## end ClMutBlock2%3742D022035D.public

  protected:
    // Data Members for Associations

      //## Association: M4Ipcs::<unnamed>%3743CB8602AC
      //## Role: ClMutBlock2::m_poMut%3743CB870196
      //## begin ClMutBlock2::m_poMut%3743CB870196.role preserve=no  protected: ClMutex { -> RHAN}
      ClMutex *m_poMut;
      //## end ClMutBlock2::m_poMut%3743CB870196.role

    // Additional Protected Declarations
      //## begin ClMutBlock2%3742D022035D.protected preserve=yes
      //## end ClMutBlock2%3742D022035D.protected

  private:
    // Additional Private Declarations
      //## begin ClMutBlock2%3742D022035D.private preserve=yes
      //## end ClMutBlock2%3742D022035D.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMutBlock2%3742D022035D.implementation preserve=yes
      //## end ClMutBlock2%3742D022035D.implementation

};

//## begin ClMutBlock2%3742D022035D.postscript preserve=yes
//## end ClMutBlock2%3742D022035D.postscript

//## begin module%3742D01E02DA.epilog preserve=yes
//## end module%3742D01E02DA.epilog


#endif
