//## begin module%38D884560065.cm preserve=no
//## end module%38D884560065.cm

//## begin module%38D884560065.cp preserve=no
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
//## end module%38D884560065.cp

//## Module: TransportLayerSynchronous%38D884560065; Package specification
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Source file: c:\Componentes\m4transport\inc\transportlayersynchronous.hpp

#ifndef transportlayersynchronous_h
#define transportlayersynchronous_h 1

//## begin module%38D884560065.additionalIncludes preserve=no
//## end module%38D884560065.additionalIncludes

//## begin module%38D884560065.includes preserve=yes
#include "m4transport_dll.hpp"
//## end module%38D884560065.includes

// TransportLayerContainer
#include <transportlayercontainer.hpp>
// TransportLayer
#include <transportlayer.hpp>
// tsap
#include <tsap.hpp>
//## begin module%38D884560065.declarations preserve=no
//## end module%38D884560065.declarations

//## begin module%38D884560065.additionalDeclarations preserve=yes
//## end module%38D884560065.additionalDeclarations


//## begin ClTransportLayerSynchronous%38D0B6390241.preface preserve=yes
//## end ClTransportLayerSynchronous%38D0B6390241.preface

//## Class: ClTransportLayerSynchronous%38D0B6390241
//	Transport Layer Communications
//## Category: m4transport::M4CTL%38D0B5F00369
//## Subsystem: m4transport::M4CTL::inc%38D1FCBC0200
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4TRANSPORT ClTransportLayerSynchronous : public ClTransportLayer  //## Inherits: <unnamed>%38D736C80291
{
  //## begin ClTransportLayerSynchronous%38D0B6390241.initialDeclarations preserve=yes
  //## end ClTransportLayerSynchronous%38D0B6390241.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClTransportLayerSynchronous%954835554; C++
      ClTransportLayerSynchronous (int ai_iControlPort = 6666);

    //## Destructor (specified)
      //## Operation: ~ClTransportLayerSynchronous%958576518
      ~ClTransportLayerSynchronous ();


    //## Other Operations (specified)
      //## Operation: OpenPort%954835546; C++
      m4return_t OpenPort (int ai_iPort, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: OpenSSLPort%956570164; C++
      m4return_t OpenSSLPort (int ai_iPort, ClIOEventHandler *ai_pIOEventHandler, char *ai_pCertificateFile, char *ai_pPrivateKey, char *ai_pCipher);

      //## Operation: ClosePort%954835547; C++
      m4return_t ClosePort (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: BeginRead%954835548; C++
      m4return_t BeginRead (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: Write%954835549; C++
      m4return_t Write (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: StopReading%954835550; C++
      m4return_t StopReading (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: StopWriting%954835551; C++
      m4return_t StopWriting (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: ConnectSSL%956570158
      m4return_t ConnectSSL (char *ai_Host, int ai_iPort, ClIOEventHandler *ai_pIOEventHandler, char *ai_pCertificateFile, char *ai_Cipher = NULL);

      //## Operation: Connect%954835552; C++
      m4return_t Connect (char *ai_Host, int ai_iPort, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: StopConnection%954835553; C++
      m4return_t StopConnection (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: Run%954835555
      void Run ();

      //## Operation: StopTransportLayer%956912718
      m4return_t StopTransportLayer (ClIOEventHandler *ai_pIOEventHandler);

      //## Operation: GetTSAPError%957367977
      int GetTSAPError (m4tliid_t ai_Identifier);

      //## Operation: GetTransferedBytes%958059749
      m4return_t GetTransferedBytes (m4tliid_t ai_Identifier, ClIOEventHandler *ai_pIOEventHandler);

    // Additional Public Declarations
      //## begin ClTransportLayerSynchronous%38D0B6390241.public preserve=yes
	  m4return_t GetTLIHostname (m4tliid_t ai_Identifier,m4pchar_t ai_szAddress, int ai_iMaxLen);
      //## end ClTransportLayerSynchronous%38D0B6390241.public

  protected:
    // Data Members for Associations

      //## Association: m4transport::M4CTL::<unnamed>%38E9FE9E03DE
      //## Role: ClTransportLayerSynchronous::m_ControlTSAP%38E9FE9F0372
      //## begin ClTransportLayerSynchronous::m_ControlTSAP%38E9FE9F0372.role preserve=no  protected: ClTSAP { -> RHAN}
      ClTSAP *m_ControlTSAP;
      //## end ClTransportLayerSynchronous::m_ControlTSAP%38E9FE9F0372.role

      //## Association: m4transport::M4CTL::<unnamed>%38EB0F0E0038
      //## Role: ClTransportLayerSynchronous::m_Container%38EB0F0E033B
      //## begin ClTransportLayerSynchronous::m_Container%38EB0F0E033B.role preserve=no  protected: ClTransportLayerContainer { -> RHAN}
      ClTransportLayerContainer *m_Container;
      //## end ClTransportLayerSynchronous::m_Container%38EB0F0E033B.role

    // Additional Protected Declarations
      //## begin ClTransportLayerSynchronous%38D0B6390241.protected preserve=yes
      //## end ClTransportLayerSynchronous%38D0B6390241.protected

  private:

    //## Other Operations (specified)
      //## Operation: UseSSL%956570156
      m4return_t UseSSL (int ai_iMode, char *ai_pCertificateFile, char *ai_pPrivateKey);

    // Additional Private Declarations
      //## begin ClTransportLayerSynchronous%38D0B6390241.private preserve=yes
      //## end ClTransportLayerSynchronous%38D0B6390241.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClTransportLayerSynchronous%38D0B6390241.implementation preserve=yes
      //## end ClTransportLayerSynchronous%38D0B6390241.implementation

};

//## begin ClTransportLayerSynchronous%38D0B6390241.postscript preserve=yes
//## end ClTransportLayerSynchronous%38D0B6390241.postscript

// Class ClTransportLayerSynchronous 

//## begin module%38D884560065.epilog preserve=yes
//## end module%38D884560065.epilog


#endif
