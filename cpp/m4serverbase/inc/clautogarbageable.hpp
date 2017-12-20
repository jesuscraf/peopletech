//## begin module%3821B13402BF.cm preserve=no
//## end module%3821B13402BF.cm

//## begin module%3821B13402BF.cp preserve=no
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
//## end module%3821B13402BF.cp

//## Module: clautogarbageable%3821B13402BF; Package specification
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Source file: D:\eduardoma\m4serverbase\inc\clautogarbageable.hpp

#ifndef clautogarbageable_h
#define clautogarbageable_h 1

//## begin module%3821B13402BF.additionalIncludes preserve=no
//## end module%3821B13402BF.additionalIncludes

//## begin module%3821B13402BF.includes preserve=yes
#include <m4serverbase_dll.hpp>
//## end module%3821B13402BF.includes

// garbagecollectorinterface
#include <garbagecollectorinterface.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>
//## begin module%3821B13402BF.declarations preserve=no
//## end module%3821B13402BF.declarations

//## begin module%3821B13402BF.additionalDeclarations preserve=yes
//## end module%3821B13402BF.additionalDeclarations


//## begin ClAutoGarbageable%3821B0B60000.preface preserve=yes
//## end ClAutoGarbageable%3821B0B60000.preface

//## Class: ClAutoGarbageable%3821B0B60000
//	Esta clase herada de Clautogarbageable, pero tiene la peculiaridad de que
//	mediante la invocación de un método (SendToBin) el mismo objeto es
//	introducido en el Garbage Collector sin necesidad de saber cual es este o de
//	que tipo es.
//## Category: M4ServerBase::GarbageCollector%3820504A014C
//## Subsystem: M4ServerBase::garbagecollector::inc%3821540B01FD
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClAutoGarbageable : public IGarbageable  //## Inherits: <unnamed>%3821B235008D
{
  //## begin ClAutoGarbageable%3821B0B60000.initialDeclarations preserve=yes
  //## end ClAutoGarbageable%3821B0B60000.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClAutoGarbageable%941721573
      ClAutoGarbageable ();


    //## Other Operations (specified)
      //## Operation: SetBin%941721574
      //	Es un método estático que será llamado por un agente encargado de asignar a
      //	todos los objetos autogarbageables, una referencia (también estáitca) al
      //	Garbage Collector
      static void SetBin (IGarbageCollector &ai_oBin);

      //## Operation: SendToBin%941721575
      //	método que provoca que el objeto sobre el que se ejecuta sea introducido en
      //	el garbage collector correspondiente en espera de que pueda ser destruido
      m4return_t SendToBin ();

    // Additional Public Declarations
      //## begin ClAutoGarbageable%3821B0B60000.public preserve=yes
      //## end ClAutoGarbageable%3821B0B60000.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::GarbageCollector::<unnamed>%3821AC75027B
      //## Role: ClAutoGarbageable::m_poBin%3821AC7600D8
      //	Referencia al Garbage Collector en el que este tipo de objetos van a ser
      //	introducidos tras la llamada al método SendToBin
      //## begin ClAutoGarbageable::m_poBin%3821AC7600D8.role preserve=no  protected: static IGarbageCollector { -> RHAN}
      static IGarbageCollector *m_poBin;
      //## end ClAutoGarbageable::m_poBin%3821AC7600D8.role

    // Additional Protected Declarations
      //## begin ClAutoGarbageable%3821B0B60000.protected preserve=yes
      //## end ClAutoGarbageable%3821B0B60000.protected

  private:
    // Additional Private Declarations
      //## begin ClAutoGarbageable%3821B0B60000.private preserve=yes
      //## end ClAutoGarbageable%3821B0B60000.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClAutoGarbageable%3821B0B60000.implementation preserve=yes
      //## end ClAutoGarbageable%3821B0B60000.implementation

};

//## begin ClAutoGarbageable%3821B0B60000.postscript preserve=yes
//## end ClAutoGarbageable%3821B0B60000.postscript

// Class ClAutoGarbageable 

//## begin module%3821B13402BF.epilog preserve=yes
//## end module%3821B13402BF.epilog


#endif
