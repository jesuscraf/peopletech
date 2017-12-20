//## begin module%35AB20E60012.cm preserve=no
//## end module%35AB20E60012.cm

//## begin module%35AB20E60012.cp preserve=no
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
//## end module%35AB20E60012.cp

//## Module: ClSecurity%35AB20E60012; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: D:\Fuentes\m4jsexejob\inc\clsecurity.hpp

#ifndef clsecurity_h
#define clsecurity_h 1

//## begin module%35AB20E60012.additionalIncludes preserve=no
//## end module%35AB20E60012.additionalIncludes

//## begin module%35AB20E60012.includes preserve=yes
#include <m4types.hpp>
//## end module%35AB20E60012.includes


class ClM4ObjService;
class ClChannelManager;

//## begin module%35AB20E60012.declarations preserve=no
//## end module%35AB20E60012.declarations

//## begin module%35AB20E60012.additionalDeclarations preserve=yes
//## end module%35AB20E60012.additionalDeclarations


//## begin ClSecurity%35AB12F3005E.preface preserve=yes
//## end ClSecurity%35AB12F3005E.preface

//## Class: ClSecurity%35AB12F3005E
//	Este clase es abstracta . De aqui se derivaran las clase  que implementan la
//	seguridad de la 2.8 y de la 3.0.Contiene un método virtual puro que permite
//	cambiar la seguridad de la máquina virtual y que se implementa de manera
//	diferente según estemos en la 3.0 o en la 2.8
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClSecurity 
{
  //## begin ClSecurity%35AB12F3005E.initialDeclarations preserve=yes
  //## end ClSecurity%35AB12F3005E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSecurity%900415026
      ClSecurity ();

    //## Destructor (specified)
      //## Operation: ~ClSecurity%900415027
      ~ClSecurity ();


    //## Other Operations (specified)
      //## Operation: ChangeSecurity%900415028
      virtual m4return_t  ChangeSecurity (m4pchar_t ai_oSecurityString) = 0;

      //## Operation: ReleaseConn%900489437
      virtual m4return_t  ReleaseConn () = 0;

      //## Operation: ReleaseResources%942231907
      virtual m4return_t ReleaseResources () = 0;

    // Additional Public Declarations
      //## begin ClSecurity%35AB12F3005E.public preserve=yes
      //## end ClSecurity%35AB12F3005E.public

  protected:
    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%3614A3E00186
      //## Role: ClSecurity::m_poChannelManager%3614A3E100C9
      //## begin ClSecurity::m_poChannelManager%3614A3E100C9.role preserve=no  protected: ClChannelManager {1 -> 1RFHAN}
      ClChannelManager *m_poChannelManager;
      //## end ClSecurity::m_poChannelManager%3614A3E100C9.role

      //## Association: M4JSExeJob::<unnamed>%3614A41F03A3
      //## Role: ClSecurity::m_poObjService%3614A420020A
      //## begin ClSecurity::m_poObjService%3614A420020A.role preserve=no  protected: ClM4ObjService {1 -> 1RFHAN}
      ClM4ObjService *m_poObjService;
      //## end ClSecurity::m_poObjService%3614A420020A.role

    // Additional Protected Declarations
      //## begin ClSecurity%35AB12F3005E.protected preserve=yes
      //## end ClSecurity%35AB12F3005E.protected

  private:
    // Additional Private Declarations
      //## begin ClSecurity%35AB12F3005E.private preserve=yes
      //## end ClSecurity%35AB12F3005E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSecurity%35AB12F3005E.implementation preserve=yes
      //## end ClSecurity%35AB12F3005E.implementation

};

//## begin ClSecurity%35AB12F3005E.postscript preserve=yes
//## end ClSecurity%35AB12F3005E.postscript

// Class ClSecurity 

//## begin module%35AB20E60012.epilog preserve=yes
//## end module%35AB20E60012.epilog


#endif
