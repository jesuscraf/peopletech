//## begin module%3831719C03A6.cm preserve=no
//## end module%3831719C03A6.cm

//## begin module%3831719C03A6.cp preserve=no
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
//## end module%3831719C03A6.cp

//## Module: SubSystem_M4XML%3831719C03A6; Package specification
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Source file: D:\Work\v400\m4exeforchannel\inc\m4xmlsubsys.hpp

#ifndef subsystem_m4xml_h
#define subsystem_m4xml_h 1

//## begin module%3831719C03A6.additionalIncludes preserve=no
//## end module%3831719C03A6.additionalIncludes

//## begin module%3831719C03A6.includes preserve=yes
//## end module%3831719C03A6.includes

#include "m4stl.hpp"
#include <executiveforoess.hpp>
#include <clservicebasess.hpp>

class ClConfiguration;
class ClNavigatorFirewall;
class ClExecutorStateManager;
class ClTables;

//## begin module%3831719C03A6.declarations preserve=no
//## end module%3831719C03A6.declarations

//## begin module%3831719C03A6.additionalDeclarations preserve=yes
#include "m4exeforchannel_dll.hpp"

#define M4_XML_SUBSYSTEM_NAME				"XML_SUBSYSTEM"
#define M4_XML_SUBSISTEM_DUMPFILE			"DUMP_FILE"
#define M4XML_SS_ELM_STATE_CACHE			"STATE_CACHE"
//## end module%3831719C03A6.additionalDeclarations


//## begin ClM4XMLSubSystem%383170EA0223.preface preserve=yes
//## end ClM4XMLSubSystem%383170EA0223.preface

//## Class: ClM4XMLSubSystem%383170EA0223
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3831786A00E4;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%383178B00211;ClConfiguration { -> F}
//## Uses: <unnamed>%3831796203D9;ClTables { -> F}
//## Uses: <unnamed>%383179BF03C9;ClOblConfiguration { -> F}
//## Uses: <unnamed>%38468D1E0176;string { -> }
//## Uses: <unnamed>%391855A902F6;ClExecutiveForM4XMLSS { -> F}

class M4_DECL_M4EXEFORCHANNEL ClM4XMLSubSystem : public ClServiceBaseSS  //## Inherits: <unnamed>%3846557B0266
{
  //## begin ClM4XMLSubSystem%383170EA0223.initialDeclarations preserve=yes
  //## end ClM4XMLSubSystem%383170EA0223.initialDeclarations

  public:
    //## begin ClM4XMLSubSystem::eXMLCommandsId%3854D4DB028C.preface preserve=yes
    //## end ClM4XMLSubSystem::eXMLCommandsId%3854D4DB028C.preface

    //## Class: eXMLCommandsId%3854D4DB028C
    //## Category: M4ExeForChannel::XML%3826C2F5004A
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {XML_SS_STATE_GET_RAM_CURRENT_SIZE=ClServiceBaseSS::TOTAL_COMMANDS,XML_SS_STATE_FLUSH,XML_SS_STATE_GET_N_OBJECTS,XML_SS_STATE_DEL_BY_SESSION_ID,XML_SS_TOTAL_COMMANDS} eXMLCommandsId;

    //## begin ClM4XMLSubSystem::eXMLCommandsId%3854D4DB028C.postscript preserve=yes
    //## end ClM4XMLSubSystem::eXMLCommandsId%3854D4DB028C.postscript

    //## Constructors (specified)
      //## Operation: ClM4XMLSubSystem%942765228
      ClM4XMLSubSystem (ClNavigatorFirewall *ai_pNavigator, ClConfiguration *ai_poOblConfiguration, ClTables* ai_poTables = NULL);

    //## Destructor (specified)
      //## Operation: ~ClM4XMLSubSystem%942765223
      virtual ~ClM4XMLSubSystem ();


    //## Other Operations (specified)
      //## Operation: GetCommandsId%944132612
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%945083773
      //	Devuelve los parámetros que pueda precisar el subsistema para ejecutar un
      //	comando concreto.
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: ExecuteCommand%942765224
      //	this method is only for compabilitity with old versions. Actually only call
      //	to ClSubsystemBase::ExecuteCommand()
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: ShutDown%942765225
      //	Hace el ShutDown del subsistema
      m4return_t ShutDown ();

      //## Operation: Initialize%942765227
      //	inicializa el subsistema creando los objetos estadñisticas y visitor
      m4return_t Initialize ();

      //## Operation: Update%944126336
      virtual void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: GetStateCache%944132615
      ClExecutorStateManager* GetStateCache () const;

      //## Operation: ClearSession%944132616
      m4return_t ClearSession (m4uint32_t ai_iSessionId);

      //## Operation: GenerateSubsystem%957882365
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

      //## Operation: GetElement%959683121
      //	Función virtual (al igual que todas las de esta clase) que sobrecargada en
      //	otra clase que herede de esta, devolverá culaquier elemento contenido por el
      //	subsistema, en forma de un puntero a void.
      //	El elemento que ha de devolver, se indica con una cadena pasada como
      //	parámetro de entrada.
      //	Elementos pueden ser todas aquellas referencias que el subsistema contenga,
      //	que por agrupación formen el conjunto lógico denominado subsistema.
      //	Por ejemplo, el subsistema sesión tendrá un elemento que es la sesión, el
      //	subsistema servicio tendrá otro que sea la cola de servicio y otro que sea
      //	en propio servicio, etc...
      virtual void * GetElement (m4pchar_t ai_szElement = NULL);

  public:
    // Additional Public Declarations
      //## begin ClM4XMLSubSystem%383170EA0223.public preserve=yes
      //## end ClM4XMLSubSystem%383170EA0223.public

  protected:
    // Additional Protected Declarations
      //## begin ClM4XMLSubSystem%383170EA0223.protected preserve=yes
      //## end ClM4XMLSubSystem%383170EA0223.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bIsEnabled%3831759C03E4
      //## begin ClM4XMLSubSystem::m_bIsEnabled%3831759C03E4.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bIsEnabled;
      //## end ClM4XMLSubSystem::m_bIsEnabled%3831759C03E4.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::XML::<unnamed>%38466B510052
      //## Role: ClM4XMLSubSystem::m_poStateManager%38466B5101C4
      //## begin ClM4XMLSubSystem::m_poStateManager%38466B5101C4.role preserve=no  private: ClExecutorStateManager {1 -> 1RFHAN}
      ClExecutorStateManager *m_poStateManager;
      //## end ClM4XMLSubSystem::m_poStateManager%38466B5101C4.role

    // Additional Private Declarations
      //## begin ClM4XMLSubSystem%383170EA0223.private preserve=yes
      //## end ClM4XMLSubSystem%383170EA0223.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClM4XMLSubSystem%383170EA0223.implementation preserve=yes
      //## end ClM4XMLSubSystem%383170EA0223.implementation

};

//## begin ClM4XMLSubSystem%383170EA0223.postscript preserve=yes
//## end ClM4XMLSubSystem%383170EA0223.postscript

//## begin ClExecutiveForM4XMLSS%391854F90252.preface preserve=yes
//## end ClExecutiveForM4XMLSS%391854F90252.preface

//## Class: ClExecutiveForM4XMLSS%391854F90252
//## Category: M4ExeForChannel::XML%3826C2F5004A
//## Subsystem: M4ExeForChannel::XML::inc%3826C59C03B7
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%391855830098;ClNavigatorFirewall { -> F}
//## Uses: <unnamed>%391855890065;ClOblConfiguration { -> F}
//## Uses: <unnamed>%3918558E036F;ClTables { -> F}
//## Uses: <unnamed>%391855A00180;ClExecutiveForM4XML { -> F}
//## Uses: <unnamed>%3919063B0399;ClLauncherForOLTP { -> F}
//## Uses: <unnamed>%391906FA0181;ClOblConfiguration { -> F}

class ClExecutiveForM4XMLSS : public ClExecutiveForOESS  //## Inherits: <unnamed>%391855160330
{
  //## begin ClExecutiveForM4XMLSS%391854F90252.initialDeclarations preserve=yes
  //## end ClExecutiveForM4XMLSS%391854F90252.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForM4XMLSS%957882366
      //	constructor de la clase
      ClExecutiveForM4XMLSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForM4XMLSS%957882367
      ~ClExecutiveForM4XMLSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%957882368
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

    // Additional Public Declarations
      //## begin ClExecutiveForM4XMLSS%391854F90252.public preserve=yes
      //## end ClExecutiveForM4XMLSS%391854F90252.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: s_szXMLInstanceOrder%391855300143
      //## begin ClExecutiveForM4XMLSS::s_szXMLInstanceOrder%391855300143.attr preserve=no  protected: static m4size_t {UA} 0
      static m4size_t s_szXMLInstanceOrder;
      //## end ClExecutiveForM4XMLSS::s_szXMLInstanceOrder%391855300143.attr

    // Additional Protected Declarations
      //## begin ClExecutiveForM4XMLSS%391854F90252.protected preserve=yes
      //## end ClExecutiveForM4XMLSS%391854F90252.protected

  private:
    // Additional Private Declarations
      //## begin ClExecutiveForM4XMLSS%391854F90252.private preserve=yes
      //## end ClExecutiveForM4XMLSS%391854F90252.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForM4XMLSS%391854F90252.implementation preserve=yes
      //## end ClExecutiveForM4XMLSS%391854F90252.implementation

};

//## begin ClExecutiveForM4XMLSS%391854F90252.postscript preserve=yes
//## end ClExecutiveForM4XMLSS%391854F90252.postscript

// Class ClM4XMLSubSystem 


//## Other Operations (inline)
inline ClExecutorStateManager* ClM4XMLSubSystem::GetStateCache () const
{
  //## begin ClM4XMLSubSystem::GetStateCache%944132615.body preserve=yes
	return m_poStateManager;
  //## end ClM4XMLSubSystem::GetStateCache%944132615.body
}

// Class ClExecutiveForM4XMLSS 

//## begin module%3831719C03A6.epilog preserve=yes
//## end module%3831719C03A6.epilog


#endif
