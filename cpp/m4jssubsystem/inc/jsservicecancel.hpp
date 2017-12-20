//## begin module%37D7C65E02B5.cm preserve=no
//## end module%37D7C65E02B5.cm

//## begin module%37D7C65E02B5.cp preserve=no
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
//## end module%37D7C65E02B5.cp

//## Module: JSServiceCancel%37D7C65E02B5; Package specification
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Source file: D:\eduardoma\m4jssubsystem\inc\jsservicecancel.hpp

#ifndef jsservicecancel_h
#define jsservicecancel_h 1

//## begin module%37D7C65E02B5.additionalIncludes preserve=no
//## end module%37D7C65E02B5.additionalIncludes

//## begin module%37D7C65E02B5.includes preserve=yes
//## end module%37D7C65E02B5.includes

// JSSCancel
#include <jsscancel.hpp>
// clservicebasess
#include <clservicebasess.hpp>
//## begin module%37D7C65E02B5.declarations preserve=no
//## end module%37D7C65E02B5.declarations

//## begin module%37D7C65E02B5.additionalDeclarations preserve=yes
//## end module%37D7C65E02B5.additionalDeclarations


//## begin ClJSServiceCancel%37D7C4E90092.preface preserve=yes
//## end ClJSServiceCancel%37D7C4E90092.preface

//## Class: ClJSServiceCancel%37D7C4E90092
//## Category: M4JSSubsystem%35F51CAB0378
//## Subsystem: M4JSSubsystem::inc%37BD2F0A02AD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37D7C5C9026B;ClJSSCancel { -> }

class ClJSServiceCancel : public ClServiceBaseSS  //## Inherits: <unnamed>%37D7C55102D7
{
  //## begin ClJSServiceCancel%37D7C4E90092.initialDeclarations preserve=yes
  //## end ClJSServiceCancel%37D7C4E90092.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClJSServiceCancel%936888037
      ClJSServiceCancel (ClConfiguration *ai_poConfiguration, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);


    //## Other Operations (specified)
      //## Operation: Initialize%936888038
      m4return_t Initialize ();

      //## Operation: ExecuteCommand%936888040
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%936888041
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetCommandsId%936888042
      m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GenerateSubsystem%936888043
      ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: Stop%937812720
      m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: SetDumpFile%940237694; C++
      m4return_t SetDumpFile (m4pchar_t ai_pDumpFile);

      //## Operation: SetIdentification%940337548
      m4return_t SetIdentification (M4ClString& ai_ServerName, M4ClString& ai_ServiceName);

    // Additional Public Declarations
      //## begin ClJSServiceCancel%37D7C4E90092.public preserve=yes
	  void SetOutputQ ( ClOutputQ * aiOutputQ ) 
	  {
		  m_poOutputQ = aiOutputQ ;
	  }
      //## end ClJSServiceCancel%37D7C4E90092.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pcDumpFile%380AE5E10342
      //## begin ClJSServiceCancel::m_pcDumpFile%380AE5E10342.attr preserve=no  protected: m4pchar_t {UA} 
      m4pchar_t m_pcDumpFile;
      //## end ClJSServiceCancel::m_pcDumpFile%380AE5E10342.attr

      //## Attribute: m_oServerName%380C88B7015D
      //## begin ClJSServiceCancel::m_oServerName%380C88B7015D.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServerName;
      //## end ClJSServiceCancel::m_oServerName%380C88B7015D.attr

      //## Attribute: m_oServiceName%380C88BF01EB
      //## begin ClJSServiceCancel::m_oServiceName%380C88BF01EB.attr preserve=no  protected: M4ClString {UA} 
      M4ClString m_oServiceName;
      //## end ClJSServiceCancel::m_oServiceName%380C88BF01EB.attr

    // Additional Protected Declarations
      //## begin ClJSServiceCancel%37D7C4E90092.protected preserve=yes
      //## end ClJSServiceCancel%37D7C4E90092.protected

  private:
    // Additional Private Declarations
      //## begin ClJSServiceCancel%37D7C4E90092.private preserve=yes
	  ClOutputQ * m_poOutputQ ;
      //## end ClJSServiceCancel%37D7C4E90092.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSServiceCancel%37D7C4E90092.implementation preserve=yes
      //## end ClJSServiceCancel%37D7C4E90092.implementation

};

//## begin ClJSServiceCancel%37D7C4E90092.postscript preserve=yes
//## end ClJSServiceCancel%37D7C4E90092.postscript

// Class ClJSServiceCancel 

//## begin module%37D7C65E02B5.epilog preserve=yes
//## end module%37D7C65E02B5.epilog


#endif


// Detached code regions:
// WARNING: this code will be lost if code is regenerated.
#if 0
//## begin ClJSServiceCancel::SetInputQueue%937417863.body preserve=no
	SetServiceQ((ClServiceQ *)ai_pJSQueue);
	return M4_SUCCESS;
//## end ClJSServiceCancel::SetInputQueue%937417863.body

//## begin ClJSServiceCancel::SetOutputQueue%937417864.body preserve=no
	SetOutputQ((ClOutputQ *)ai_pJSQueue);
	return M4_SUCCESS;
//## end ClJSServiceCancel::SetOutputQueue%937417864.body

#endif
