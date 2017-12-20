//## begin module%3822A0EC0098.cm preserve=no
//## end module%3822A0EC0098.cm

//## begin module%3822A0EC0098.cp preserve=no
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
//## end module%3822A0EC0098.cp

//## Module: ProxSpacsSS%3822A0EC0098; Package specification
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Source file: D:\Work\v600\m4exeforchannel\inc\proxspacsss.hpp

#ifndef proxspacsss_h
#define proxspacsss_h 1

//## begin module%3822A0EC0098.additionalIncludes preserve=no
//## end module%3822A0EC0098.additionalIncludes

//## begin module%3822A0EC0098.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
//## end module%3822A0EC0098.includes

// ClConfiguration
#include <clconfiguration.hpp>
// clssimplementacion
#include <clssimplementacion.hpp>
// IProxyManager
#include <iproxymanager.hpp>

class ClServerProxySpaceSS;
class ClServerProxyManager;

//## begin module%3822A0EC0098.declarations preserve=no
//## end module%3822A0EC0098.declarations

//## begin module%3822A0EC0098.additionalDeclarations preserve=yes
//## end module%3822A0EC0098.additionalDeclarations


//## begin ClProxySpacesSS%38229C71036A.preface preserve=yes
//## end ClProxySpacesSS%38229C71036A.preface

//## Class: ClProxySpacesSS%38229C71036A
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38353E5F0285;ClServerProxySpaceSS { -> F}
//## Uses: <unnamed>%3835448F0337;ClConfiguration { -> }

class M4_DECL_M4EXEFORCHANNEL ClProxySpacesSS : public ClSSImplementation, //## Inherits: <unnamed>%38229CD4004B
                            	public IProxyManager  //## Inherits: <unnamed>%38229CD6018E
{
  //## begin ClProxySpacesSS%38229C71036A.initialDeclarations preserve=yes
  //## end ClProxySpacesSS%38229C71036A.initialDeclarations

  public:
    //## begin ClProxySpacesSS::eProxySpacesComands%3822B4CD0352.preface preserve=yes
    //## end ClProxySpacesSS::eProxySpacesComands%3822B4CD0352.preface

    //## Class: eProxySpacesComands%3822B4CD0352
    //## Category: M4ExeForChannel::Proxy%392272AB0394
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {DESTROY_PX_SPACE_BY_SESSION_ID=ClSSImplementation::TOTAL_COMMANDS, COMMAND_ADD_NEW_PROXY_SPACE, COMMAND_USED_MEMORY, TOTAL_COMMANDS} eProxySpacesComands;

    //## begin ClProxySpacesSS::eProxySpacesComands%3822B4CD0352.postscript preserve=yes
    //## end ClProxySpacesSS::eProxySpacesComands%3822B4CD0352.postscript

    //## Constructors (specified)
      //## Operation: ClProxySpacesSS%941744286
      ClProxySpacesSS (ClConfiguration* ai_poConfiguration, ClTables* ai_poTables, ClNavigatorFirewall* ai_poNavigator);

    //## Destructor (specified)
      //## Operation: ~ClProxySpacesSS%941744287
      ~ClProxySpacesSS ();


    //## Other Operations (specified)
      //## Operation: GetSpace%941744280
      //	Devuelve un espacio proxy. Este se selecciona por el parámetro ai_pcClProxy
      //	SpaceID.
      //	Si el espacio proxy no existe se crea uno nuevo si ai_bCreateNew == M4_TRUE.
      //
      //	@param ai_pcClProxySpaceID    Id del espacio proxy que se quiere.
      //	@param ai_bCreateNew    Si no existe el espacio proxy y se pasa el valor M4_
      //	TRUE --> se crea un nuevo espacio proxy.
      //	@return ClProxySpace    Espacio proxy. Si no se encontro y ai_bCreateNew ==
      //	M4_FALSE --> se devuelve NULL.
      virtual ClProxySpace* const GetSpace (const m4char_t* const ai_pcClProxySpaceID, const m4bool_t ai_bCreateNew = M4_TRUE);

      //## Operation: ReleaseSpace%941744281
      //	Libera el espacio proxy que se pasa como parametro.
      virtual m4return_t ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock = M4_TRUE);

      //## Operation: SetCaches%941744282
      //	Fija las caches para el gestor de espacios proxy.
      virtual void SetCaches (ClCache* const ai_poCMCRCache, ClCache* const ai_poCSCRCache, ClCache* const ai_poPresentationCache, ClCache* const ai_poMapCache, ClCache* const ai_poDatacache);

      //## Operation: GetCaches%941744283
      virtual void GetCaches (ClCache * &ao_poCMCRCache, ClCache * &ao_poCSCRCache, ClCache * &ao_poPresentationCache, ClCache * &ao_poMapCache, ClCache * &ao_poDatacache) const;

      //## Operation: IsServerMode%941744284
      //	Indica si estamos en el lado servidor (válido para DIC o SM).
      virtual m4bool_t IsServerMode () const;

      //## Operation: SendEvent%941744285
      virtual m4return_t SendEvent ();

      //## Operation: ExecuteCommand%941744288
      virtual m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetCommandParams%941744289
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: Start%941744292
      virtual m4return_t Start ();

      //## Operation: Synchronize%941744294
      //	Expands the synchronize method to all of subsystem sons and then calls to
      //	the ClSSStatsContainer derived class Synchronize method.
      virtual m4return_t Synchronize ();

      //## Operation: Update%942076256
      void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: GetCommandsId%942076257
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetElement%942403829
      void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: CreateServerProxySpaceSS%943008105
      //	Crea un proxy space que es, ademas, un subsistema.
      //	Hace las operaciones propias para crear y añadir el subsistema.
      ClServerProxySpaceSS* CreateServerProxySpaceSS ();

      //## Operation: Initialize%945276898
      //	A virtual method that only calls to the derived class from ClSSStats
      //	Container Initialize method
      virtual m4return_t Initialize ();

      //## Operation: ReleaseActiveSpace%956737462
      virtual m4return_t ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID);

      //## Operation: Reset%994184752
      //	Hace un reset de los miembros internos después de un logoff, por ejemplo.
      void Reset ();

      //## Operation: EraseAllL2Instances%1025261934
      virtual m4return_t EraseAllL2Instances (const m4char_t* const ai_pcClProxySpaceID, m4uint32_t ai_iRemoteId);

  public:
    // Additional Public Declarations
      //## begin ClProxySpacesSS%38229C71036A.public preserve=yes
      //## end ClProxySpacesSS%38229C71036A.public

  protected:
    // Additional Protected Declarations
      //## begin ClProxySpacesSS%38229C71036A.protected preserve=yes
      //## end ClProxySpacesSS%38229C71036A.protected

  private:

    //## Other Operations (specified)
      //## Operation: IncrementMaxNSpaces%945276897
      m4return_t IncrementMaxNSpaces (const m4int32_t ai_iIncrement);

    // Data Members for Associations

      //## Association: M4ExeForChannel::Proxy::<unnamed>%38229CDF028B
      //## Role: ClProxySpacesSS::m_pServProxMana%38229CE00111
      //## begin ClProxySpacesSS::m_pServProxMana%38229CE00111.role preserve=no  private: ClServerProxyManager {1 -> 1RFHAN}
      ClServerProxyManager *m_pServProxMana;
      //## end ClProxySpacesSS::m_pServProxMana%38229CE00111.role

    // Additional Private Declarations
      //## begin ClProxySpacesSS%38229C71036A.private preserve=yes
      //## end ClProxySpacesSS%38229C71036A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProxySpacesSS%38229C71036A.implementation preserve=yes
      //## end ClProxySpacesSS%38229C71036A.implementation

};

//## begin ClProxySpacesSS%38229C71036A.postscript preserve=yes
//## end ClProxySpacesSS%38229C71036A.postscript

// Class ClProxySpacesSS 

//## begin module%3822A0EC0098.epilog preserve=yes
//## end module%3822A0EC0098.epilog


#endif
