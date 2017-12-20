//## begin module%38102BD20092.cm preserve=no
//## end module%38102BD20092.cm

//## begin module%38102BD20092.cp preserve=no
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
//## end module%38102BD20092.cp

//## Module: ClJSChannelHandler30%38102BD20092; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: E:\maite_work\m4jsexejob\version\inc\cljschannelhandler30.hpp

#ifndef cljschannelhandler30_h
#define cljschannelhandler30_h 1

//## begin module%38102BD20092.additionalIncludes preserve=no
//## end module%38102BD20092.additionalIncludes

//## begin module%38102BD20092.includes preserve=yes
//## end module%38102BD20092.includes

// ClJSChannelHandler
#include <cljschannelhandler.hpp>
//## begin module%38102BD20092.declarations preserve=no
//## end module%38102BD20092.declarations

//## begin module%38102BD20092.additionalDeclarations preserve=yes
//## end module%38102BD20092.additionalDeclarations


//## begin ClJSChannelHandler30%38102A23015B.preface preserve=yes
//## end ClJSChannelHandler30%38102A23015B.preface

//## Class: ClJSChannelHandler30%38102A23015B
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClJSChannelHandler30 : public ClJSChannelHandler  //## Inherits: <unnamed>%38102A690238
{
  //## begin ClJSChannelHandler30%38102A23015B.initialDeclarations preserve=yes
  //## end ClJSChannelHandler30%38102A23015B.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSChannelHandler30%940584197
      ClJSChannelHandler30 ();


    //## Other Operations (specified)
      //## Operation: BuildChannel%940584198
      m4return_t BuildChannel (m4pcchar_t ai_pccChannel, ClChannel *&ao_poChannel, ClAccess *&ao_poAccess);

    // Additional Public Declarations
      //## begin ClJSChannelHandler30%38102A23015B.public preserve=yes
      //## end ClJSChannelHandler30%38102A23015B.public

  protected:
    // Additional Protected Declarations
      //## begin ClJSChannelHandler30%38102A23015B.protected preserve=yes
      //## end ClJSChannelHandler30%38102A23015B.protected

  private:
    // Additional Private Declarations
      //## begin ClJSChannelHandler30%38102A23015B.private preserve=yes
      //## end ClJSChannelHandler30%38102A23015B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSChannelHandler30%38102A23015B.implementation preserve=yes
      //## end ClJSChannelHandler30%38102A23015B.implementation

};

//## begin ClJSChannelHandler30%38102A23015B.postscript preserve=yes
//## end ClJSChannelHandler30%38102A23015B.postscript

// Class ClJSChannelHandler30 

//## begin module%38102BD20092.epilog preserve=yes
//## end module%38102BD20092.epilog


#endif
