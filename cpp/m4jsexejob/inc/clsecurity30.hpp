//## begin module%35AB212B02CF.cm preserve=no
//## end module%35AB212B02CF.cm

//## begin module%35AB212B02CF.cp preserve=no
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
//## end module%35AB212B02CF.cp

//## Module: ClSecurity30%35AB212B02CF; Package specification
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Source file: D:\Work\v600\m4jsexejob\inc\clsecurity30.hpp

#ifndef clsecurity30_h
#define clsecurity30_h 1

//## begin module%35AB212B02CF.additionalIncludes preserve=no
//## end module%35AB212B02CF.additionalIncludes

//## begin module%35AB212B02CF.includes preserve=yes
class ClLogonInterface;
//## end module%35AB212B02CF.includes

// access
#include <access.hpp>
// channel
#include <channel.hpp>
// ClSecurity
#include <clsecurity.hpp>

class ClUSSessionInterface;
class ClStatsJSEXECUTE;

//## begin module%35AB212B02CF.declarations preserve=no
//## end module%35AB212B02CF.declarations

//## begin module%35AB212B02CF.additionalDeclarations preserve=yes
#include <clbaseobject.hpp>
//## end module%35AB212B02CF.additionalDeclarations


//## begin ClSecurity30%35AB149F015D.preface preserve=yes
//## end ClSecurity30%35AB149F015D.preface

//## Class: ClSecurity30%35AB149F015D
//	Esta clase se ocupa de la seguridad  de la 3.0 . Obtiene el canal de usuario
//	para el usuario que nos indique el job definiton y se lo asigna a la máquina
//	virtual.
//## Category: M4JSExeJob%37F8BA0B0245
//## Subsystem: M4JSExeJob::inc%37F8BA610325
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%373811720122;IExecutor { -> F}

class ClSecurity30 : public ClSecurity  //## Inherits: <unnamed>%35AB16B8023C
{
  //## begin ClSecurity30%35AB149F015D.initialDeclarations preserve=yes
  //## end ClSecurity30%35AB149F015D.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSecurity30%900415030
      ClSecurity30 (ClM4ObjService* ai_poObjService, ClLogonInterface *ai_poLogon, ClStatsJSEXECUTE *ai_poStats);

    //## Destructor (specified)
      //## Operation: ~ClSecurity30%900415029
      ~ClSecurity30 ();


    //## Other Operations (specified)
      //## Operation: ChangeSecurity%900415034
      //	Esta función llama a los método :ChngeChannelUser y GetUserRole.
      m4return_t  ChangeSecurity (m4pchar_t ai_poSecurityString);

      //## Operation: ReleaseConn%900489435
      //	Lo único que hace este método es soltar la conexión lógica
      m4return_t ReleaseConn ();

      //## Operation: ReleaseResources%942231906
      m4return_t ReleaseResources ();

      //## Operation: NotifyLogon%1079025394
      static m4void_t NotifyLogon (m4objid_t ai_liSessionId);

      //## Operation: NotifyLogoff%1079025395
      static m4void_t NotifyLogoff (m4bool_t ai_bDoFree);

    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%38F5FDB200F9
      //## Role: ClSecurity30::m_poChannelCredential%38F5FDB2030C
      //## begin ClSecurity30::m_poChannelCredential%38F5FDB2030C.role preserve=no  public: ClChannel { -> RHAN}
      ClChannel *m_poChannelCredential;
      //## end ClSecurity30::m_poChannelCredential%38F5FDB2030C.role

      //## Association: M4JSExeJob::<unnamed>%38F5FDDE0215
      //## Role: ClSecurity30::m_poAccessUSer%38F5FDDF0049
      //## begin ClSecurity30::m_poAccessUSer%38F5FDDF0049.role preserve=no  public: ClAccess { -> RHAN}
      ClAccess *m_poAccessUSer;
      //## end ClSecurity30::m_poAccessUSer%38F5FDDF0049.role

    // Additional Public Declarations
      //## begin ClSecurity30%35AB149F015D.public preserve=yes
      //## end ClSecurity30%35AB149F015D.public

  protected:
    // Additional Protected Declarations
      //## begin ClSecurity30%35AB149F015D.protected preserve=yes
      //## end ClSecurity30%35AB149F015D.protected

  private:

    //## Other Operations (specified)
      //## Operation: GetUserRole%900415031
      //	Esta función obtiene de la cadena que nos pasan en el campo credential del
      //	canal de job definition , el usuario y el role de ese usuario . La cadena
      //	que nos pasan tiene que tener la siguiente estructura :
      //
      //	           cadena1 | cadena2 | parámetros para configurar el canal sesión
      //
      //	Si tiene la estructura : cadena , la función devuelve un error
      m4return_t  GetUserRole (m4pchar_t  ai_pBuffer, m4pchar_t  ai_pUser, m4pchar_t  ai_pRole, m4pchar_t ai_pOrganization, m4pchar_t ai_pParamsChannelUser);

      //## Operation: ChangeChannelUser%900415032
      //	Este es el método que nos da el canal de usuarios para el usuario y el role
      //	que le indiquemos . Lo que hece la función con este canal es asignarlo a la
      //	máquina virtual , estableciendo una nueva seguridad.
      m4return_t  ChangeChannelUser (m4pchar_t  ai_pUser, m4pchar_t  ai_pRole, m4pchar_t ai_pOrganization, m4pchar_t ai_pParamsChannelUser);

    // Data Members for Associations

      //## Association: M4JSExeJob::<unnamed>%3614A56E00DE
      //## Role: ClSecurity30::m_poUSSesion%3614A56E0323
      //## begin ClSecurity30::m_poUSSesion%3614A56E0323.role preserve=no  private: ClUSSessionInterface {1 -> 1RFHAN}
      ClUSSessionInterface *m_poUSSesion;
      //## end ClSecurity30::m_poUSSesion%3614A56E0323.role

      //## Association: M4JSExeJob::<unnamed>%3695FD11009F
      //## Role: ClSecurity30::m_poChannelUser%3695FD120155
      //## begin ClSecurity30::m_poChannelUser%3695FD120155.role preserve=no  private: ClChannel {1 -> 1RFHAN}
      ClChannel *m_poChannelUser;
      //## end ClSecurity30::m_poChannelUser%3695FD120155.role

      //## Association: M4JSExeJob::<unnamed>%37D5275401E5
      //## Role: ClSecurity30::m_poStats%37D527550132
      //## begin ClSecurity30::m_poStats%37D527550132.role preserve=no  private: ClStatsJSEXECUTE { -> RFHAN}
      ClStatsJSEXECUTE *m_poStats;
      //## end ClSecurity30::m_poStats%37D527550132.role

    // Additional Private Declarations
      //## begin ClSecurity30%35AB149F015D.private preserve=yes
	  ClLogonInterface *m_poLogon;
      //## end ClSecurity30%35AB149F015D.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSecurity30%35AB149F015D.implementation preserve=yes
      //## end ClSecurity30%35AB149F015D.implementation

};

//## begin ClSecurity30%35AB149F015D.postscript preserve=yes
//## end ClSecurity30%35AB149F015D.postscript

// Class ClSecurity30 

//## begin module%35AB212B02CF.epilog preserve=yes
//## end module%35AB212B02CF.epilog


#endif
