//## begin module%3911AA240391.cm preserve=no
//## end module%3911AA240391.cm

//## begin module%3911AA240391.cp preserve=no
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
//## end module%3911AA240391.cp

//## Module: clsrlzbl%3911AA240391; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: d:\new\m4serial\inc\clsrlzbl.hpp

#ifndef clsrlzbl_h
#define clsrlzbl_h 1

//## begin module%3911AA240391.additionalIncludes preserve=no
//## end module%3911AA240391.additionalIncludes

//## begin module%3911AA240391.includes preserve=yes

#include "m4serial_dll.hpp"
#include "m4types.hpp"

#include "m4types.hpp"
#include "clgeniod.hpp"


// ---- ClSerializableObject ----------------------------------------

//## end module%3911AA240391.includes

//## begin module%3911AA240391.declarations preserve=no
//## end module%3911AA240391.declarations

//## begin module%3911AA240391.additionalDeclarations preserve=yes
//## end module%3911AA240391.additionalDeclarations


//## begin ClSerializableObject%3911AA24038B.preface preserve=yes
//## end ClSerializableObject%3911AA24038B.preface

//## Class: ClSerializableObject%3911AA24038B; Abstract
//	---- ClSerializableObject ----------------------------------------
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClSerializableObject 
{
  //## begin ClSerializableObject%3911AA24038B.initialDeclarations preserve=yes
  //## end ClSerializableObject%3911AA24038B.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Serialize%-309433110; C++
      virtual m4return_t Serialize (ClGenericIODriver& ai_roIOD) = 0;

      //## Operation: DeSerialize%-595576155; C++
      //	virtual m4uint16_t GetSerializeVersion(void)=0;
      virtual m4return_t DeSerialize (ClGenericIODriver& ai_roIOD) = 0;

    // Additional Public Declarations
      //## begin ClSerializableObject%3911AA24038B.public preserve=yes
      //## end ClSerializableObject%3911AA24038B.public

  protected:
    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA280097
      //## Role: ClSerializableObject::m_iSerializeVersion%3911AA280099
      //## begin ClSerializableObject::m_iSerializeVersion%3911AA280099.role preserve=no  protected: static m4uint16_t {1 -> 1VHPNC}
      static const m4uint16_t  m_iSerializeVersion;
      //## end ClSerializableObject::m_iSerializeVersion%3911AA280099.role

    // Additional Protected Declarations
      //## begin ClSerializableObject%3911AA24038B.protected preserve=yes
      //## end ClSerializableObject%3911AA24038B.protected

  private:
    // Additional Private Declarations
      //## begin ClSerializableObject%3911AA24038B.private preserve=yes
      //## end ClSerializableObject%3911AA24038B.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSerializableObject%3911AA24038B.implementation preserve=yes
      //## end ClSerializableObject%3911AA24038B.implementation

};

//## begin ClSerializableObject%3911AA24038B.postscript preserve=yes
//## end ClSerializableObject%3911AA24038B.postscript

// Class ClSerializableObject 

//## begin module%3911AA240391.epilog preserve=yes
//## end module%3911AA240391.epilog


#endif
