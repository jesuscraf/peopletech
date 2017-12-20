//## begin module%396AE23F030C.cm preserve=no
//## end module%396AE23F030C.cm

//## begin module%396AE23F030C.cp preserve=no
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
//## end module%396AE23F030C.cp

//## Module: cltimer%396AE23F030C; Package specification
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Source file: C:\WorkingFolder\m4dispatcher\inc\cltimer.hpp

#ifndef cltimer_h
#define cltimer_h 1

//## begin module%396AE23F030C.additionalIncludes preserve=no
//## end module%396AE23F030C.additionalIncludes

//## begin module%396AE23F030C.includes preserve=yes
//## end module%396AE23F030C.includes

#include "m4stl.hpp"
// m4thread
#include <m4thread.hpp>
// iexecutable
#include <iexecutable.hpp>
//## begin module%396AE23F030C.declarations preserve=no
//## end module%396AE23F030C.declarations

//## begin module%396AE23F030C.additionalDeclarations preserve=yes
//## end module%396AE23F030C.additionalDeclarations


//## begin eActionNameList%396ADFEC03BB.preface preserve=yes
//## end eActionNameList%396ADFEC03BB.preface

//## Class: eActionNameList%396ADFEC03BB
//## Category: M4Dispatcher%3923B57F0208
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef vector <m4string_t> eActionNameList;

//## begin eActionNameList%396ADFEC03BB.postscript preserve=yes
//## end eActionNameList%396ADFEC03BB.postscript

//## begin ClTimer%396ADE890067.preface preserve=yes
//## end ClTimer%396ADE890067.preface

//## Class: ClTimer%396ADE890067
//## Category: M4Dispatcher%3923B57F0208
//## Subsystem: M4Dispatcher::inc%3923BBF7032C
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClTimer : public M4Thread  //## Inherits: <unnamed>%396AE2B00318
{
  //## begin ClTimer%396ADE890067.initialDeclarations preserve=yes
  //## end ClTimer%396ADE890067.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTimer%963302389
      ClTimer (ClExecutableInterface *ai_poExecutive, m4uint32_t ai_iSlapsedTime, eActionNameList *ai_vActionList);

    //## Destructor (specified)
      //## Operation: ~ClTimer%972579512; C++
      //	constructor
      virtual ~ClTimer ();


    //## Other Operations (specified)
      //## Operation: Run%963302390; C++
      void Run (void );

    // Additional Public Declarations
      //## begin ClTimer%396ADE890067.public preserve=yes
      //## end ClTimer%396ADE890067.public

  protected:
    // Data Members for Associations

      //## Association: M4Dispatcher::<unnamed>%396ADEF30222
      //## Role: ClTimer::m_poExecutiveObj%396ADEF4013D
      //## begin ClTimer::m_poExecutiveObj%396ADEF4013D.role preserve=no  protected: ClExecutableInterface { -> RHAN}
      ClExecutableInterface *m_poExecutiveObj;
      //## end ClTimer::m_poExecutiveObj%396ADEF4013D.role

      //## Association: M4Dispatcher::<unnamed>%396AE1570222
      //## Role: ClTimer::m_vActions%396AE15900EE
      //## begin ClTimer::m_vActions%396AE15900EE.role preserve=no  protected: eActionNameList { -> RHAN}
      eActionNameList *m_vActions;
      //## end ClTimer::m_vActions%396AE15900EE.role

    // Additional Protected Declarations
      //## begin ClTimer%396ADE890067.protected preserve=yes
      //## end ClTimer%396ADE890067.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iSlapsedTime%396ADFCD006D
      //## begin ClTimer::m_iSlapsedTime%396ADFCD006D.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iSlapsedTime;
      //## end ClTimer::m_iSlapsedTime%396ADFCD006D.attr

    // Additional Private Declarations
      //## begin ClTimer%396ADE890067.private preserve=yes
      //## end ClTimer%396ADE890067.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTimer%396ADE890067.implementation preserve=yes
      //## end ClTimer%396ADE890067.implementation

};

//## begin ClTimer%396ADE890067.postscript preserve=yes
//## end ClTimer%396ADE890067.postscript

// Class ClTimer 

//## begin module%396AE23F030C.epilog preserve=yes
//## end module%396AE23F030C.epilog


#endif
