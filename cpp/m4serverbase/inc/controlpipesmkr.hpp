//## begin module%37E6782803B6.cm preserve=no
//## end module%37E6782803B6.cm

//## begin module%37E6782803B6.cp preserve=no
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
//## end module%37E6782803B6.cp

//## Module: controlpipesmkr%37E6782803B6; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\controlpipesmkr.hpp

#ifndef controlpipesmkr_h
#define controlpipesmkr_h 1

//## begin module%37E6782803B6.additionalIncludes preserve=no
//## end module%37E6782803B6.additionalIncludes

//## begin module%37E6782803B6.includes preserve=yes
#include <m4serverbase_dll.hpp>

//## end module%37E6782803B6.includes

// tsap
#include <tsap.hpp>
//## begin module%37E6782803B6.declarations preserve=no
//## end module%37E6782803B6.declarations

//## begin module%37E6782803B6.additionalDeclarations preserve=yes
//## end module%37E6782803B6.additionalDeclarations


//## begin ClControlPipesMaker%37E6757A021F.preface preserve=yes
//## end ClControlPipesMaker%37E6757A021F.preface

//## Class: ClControlPipesMaker%37E6757A021F
//	Esta clase es una utilidad para obtener puertos de uso interno de un proceso
//	cualquiera. Una vez inicializado con el número del puerto que debe usar para
//	establecer los sockets de control, devolverá por cada llamada un par de
//	sockets conectados entre si, de tal forma que se pueden usa com puertos de
//	control de cualquier elemento interno del servidor (por ejemplo senders y
//	receivers)
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37E678A600A0;ClTSAP { -> }

class M4_DECL_M4SERVERBASE ClControlPipesMaker 
{
  //## begin ClControlPipesMaker%37E6757A021F.initialDeclarations preserve=yes
  //## end ClControlPipesMaker%37E6757A021F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClControlPipesMaker%937841143
      ClControlPipesMaker ();


    //## Other Operations (specified)
      //## Operation: GetPipes%937841140
      //	Este método retorna como parámetros de salida, los dos extremos de la
      //	comunicación establecidos al abrir un socket sobre un puerto del que
      //	estábamos ya escuchando.
      static m4return_t GetPipes (ClTSAP *&ao_poControlA, ClTSAP *&ao_poControlB);

      //## Operation: Init%937841142
      //	inicializa el ServerTSAP que tiene la clase con el número de puerto que se
      //	le pasa
      static m4return_t Init (m4uint32_t ai_iPort);

      //## Operation: Close%947499514
      static m4return_t Close ();

    // Additional Public Declarations
      //## begin ClControlPipesMaker%37E6757A021F.public preserve=yes
      //## end ClControlPipesMaker%37E6757A021F.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%37E675AE029C
      //## Role: ClControlPipesMaker::m_poControlMaker%37E675B001FE
      //## begin ClControlPipesMaker::m_poControlMaker%37E675B001FE.role preserve=no  protected: static ClControlPipesMaker { -> RHAN}
      static ClControlPipesMaker *m_poControlMaker;
      //## end ClControlPipesMaker::m_poControlMaker%37E675B001FE.role

      //## Association: M4ServerBase::<unnamed>%37E677270302
      //## Role: ClControlPipesMaker::m_poServer%37E67728017D
      //	Este objeto sirve como parte servidora a la hora de escuchar por el puerto
      //	indicado. Es estático puesto que todos los sockets son abiertos sobre el
      //	mismo puerto
      //## begin ClControlPipesMaker::m_poServer%37E67728017D.role preserve=no  protected: static ClServerTSAP { -> RHAN}
      static ClServerTSAP *m_poServer;
      //## end ClControlPipesMaker::m_poServer%37E67728017D.role

    // Additional Protected Declarations
      //## begin ClControlPipesMaker%37E6757A021F.protected preserve=yes
      //## end ClControlPipesMaker%37E6757A021F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iPort%37E6770102CB
      //## begin ClControlPipesMaker::m_iPort%37E6770102CB.attr preserve=no  private: static m4uint32_t {UA} 
      static m4uint32_t m_iPort;
      //## end ClControlPipesMaker::m_iPort%37E6770102CB.attr

    // Additional Private Declarations
      //## begin ClControlPipesMaker%37E6757A021F.private preserve=yes
      //## end ClControlPipesMaker%37E6757A021F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClControlPipesMaker%37E6757A021F.implementation preserve=yes
      //## end ClControlPipesMaker%37E6757A021F.implementation

};

//## begin ClControlPipesMaker%37E6757A021F.postscript preserve=yes
//## end ClControlPipesMaker%37E6757A021F.postscript

// Class ClControlPipesMaker 

//## begin module%37E6782803B6.epilog preserve=yes
//## end module%37E6782803B6.epilog


#endif
