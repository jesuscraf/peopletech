//## begin module%3507B4990264.cm preserve=no
//## end module%3507B4990264.cm

//## begin module%3507B4990264.cp preserve=no
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
//## end module%3507B4990264.cp

//## Module: Executive_Proxy%3507B4990264; Package specification
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Source file: D:\eduardoma\m4exeforchannel\inc\exeforproxy.hpp

#ifndef executive_proxy_h
#define executive_proxy_h 1

//## begin module%3507B4990264.additionalIncludes preserve=no
//## end module%3507B4990264.additionalIncludes

//## begin module%3507B4990264.includes preserve=yes
#include "m4exeforchannel_dll.hpp"

class ClServerProxyManager;

//## end module%3507B4990264.includes

// executiveforoe
#include <executiveforoe.hpp>
// SerGenProxy
#include <sergenproxy.hpp>
//## begin module%3507B4990264.declarations preserve=no
//## end module%3507B4990264.declarations

//## begin module%3507B4990264.additionalDeclarations preserve=yes
//## end module%3507B4990264.additionalDeclarations


//## Class: ClExecutiveForProxy%3507B8520296
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%392AB49901CE;ClMutBlock { -> F}

class M4_DECL_M4EXEFORCHANNEL ClExecutiveForProxy : public ClExecutiveForOE  //## Inherits: <unnamed>%3906BD4F006E
{
  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForProxy%889698662
      ClExecutiveForProxy (ClEngine *ai_pEngine);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForProxy%889698663
      ~ClExecutiveForProxy ();


    //## Other Operations (specified)
      //## Operation: StartUp%889698664
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: Execute%889698665
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

      //## Operation: ShutDown%889698666
      m4return_t ShutDown ();

      //## Operation: Reset%889698667; C++
      m4return_t Reset ();

      //## Operation: ExecuteCommand%922189097
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: ReleaseResources%942686704; C++
      virtual m4return_t ReleaseResources ();

  protected:
    // Data Members for Associations

      //## Association: M4ExeForChannel::<unnamed>%3906C3D502DC
      //## Role: ClExecutiveForProxy::m_poClServerProxyManager%3906C3D60143
      //## begin ClExecutiveForProxy::m_poClServerProxyManager%3906C3D60143.role preserve=no  protected: ClServerProxyManager { -> 1RHAN}
      ClServerProxyManager *m_poClServerProxyManager;
      //## end ClExecutiveForProxy::m_poClServerProxyManager%3906C3D60143.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pProxySubSystem%36ED457B032E
      //## begin ClExecutiveForProxy::m_pProxySubSystem%36ED457B032E.attr preserve=no  private: void * {UA} 
      void *m_pProxySubSystem;
      //## end ClExecutiveForProxy::m_pProxySubSystem%36ED457B032E.attr

      //## Attribute: m_poConfiguration%3846610E01B6
      //## begin ClExecutiveForProxy::m_poConfiguration%3846610E01B6.attr preserve=no  private: ClConfiguration* {UA} 
      ClConfiguration* m_poConfiguration;
      //## end ClExecutiveForProxy::m_poConfiguration%3846610E01B6.attr

  private: //## implementation
};

// Class ClExecutiveForProxy 

//## begin module%3507B4990264.epilog preserve=yes
//## end module%3507B4990264.epilog


#endif
