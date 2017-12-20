//## begin module%3830034B02B4.cm preserve=no
//## end module%3830034B02B4.cm

//## begin module%3830034B02B4.cp preserve=no
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
//## end module%3830034B02B4.cp

//## Module: SerGenProxy%3830034B02B4; Package specification
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Source file: D:\Work\v500\m4exeforchannel\inc\sergenproxy.hpp

#ifndef sergenproxy_h
#define sergenproxy_h 1

//## begin module%3830034B02B4.additionalIncludes preserve=no
//## end module%3830034B02B4.additionalIncludes

//## begin module%3830034B02B4.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
//## end module%3830034B02B4.includes

// clssimplementacion
#include <clssimplementacion.hpp>
// Generic_Proxy
#include <genproxy.hpp>

class ClCache;
class ClMutex;
class ClProxySpacesSS;
class ClServerProxyManager;

//## begin module%3830034B02B4.declarations preserve=no
//## end module%3830034B02B4.declarations

//## begin module%3830034B02B4.additionalDeclarations preserve=yes
//## end module%3830034B02B4.additionalDeclarations


//## begin ClServerProxySpaceSS%380B45C701ED.preface preserve=yes
//## end ClServerProxySpaceSS%380B45C701ED.preface

//## Class: ClServerProxySpaceSS%380B45C701ED
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3922AFE002FE;ClServerProxyManager { -> F}
//## Uses: <unnamed>%3922BC12009B;ClM4ObjService { -> F}

class ClServerProxySpaceSS : public ClProxySpace, //## Inherits: <unnamed>%380B4607007C
                            	public ClSSImplementation  //## Inherits: <unnamed>%380B471B02FA
{
  //## begin ClServerProxySpaceSS%380B45C701ED.initialDeclarations preserve=yes
  //## end ClServerProxySpaceSS%380B45C701ED.initialDeclarations

  public:
    //## begin ClServerProxySpaceSS::eProxySpaceCommands%380B54130286.preface preserve=yes
    //## end ClServerProxySpaceSS::eProxySpaceCommands%380B54130286.preface

    //## Class: eProxySpaceCommands%380B54130286
    //## Category: M4ExeForChannel::Proxy%392272AB0394
    //## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {COMMAND_DESTROY_PX_SPACE_BY_ID=ClSSImplementation::TOTAL_COMMANDS, COMMAND_ADD_NEW_PROXY_SPACE, COMMAND_USED_MEMORY, TOTAL_COMMANDS} eProxySpaceCommands;

    //## begin ClServerProxySpaceSS::eProxySpaceCommands%380B54130286.postscript preserve=yes
    //## end ClServerProxySpaceSS::eProxySpaceCommands%380B54130286.postscript

    //## Constructors (specified)
      //## Operation: ClServerProxySpaceSS%943008106
      ClServerProxySpaceSS (ClServerProxyManager* ai_pServerProxyManager, ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator);


    //## Other Operations (specified)
      //## Operation: GetCommandsId%943376569
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: SetSessionId%943439079
      virtual m4return_t SetSessionId (m4uint32_t ai_iSessionId, m4pcchar_t ai_pcUserName);

      //## Operation: SetUserName%943439080
      virtual void SetUserName (m4pcchar_t ai_pcUserName);

      //## Operation: SetInvalidState%951838050
      void SetInvalidState (m4bool_t ai_bState = M4_TRUE);

      //## Operation: FreeLogicalConnection%958548748
      //	Libera la conexión lógica asociada al espacio proxy.
      virtual m4return_t FreeLogicalConnection ();

  public:
    // Additional Public Declarations
      //## begin ClServerProxySpaceSS%380B45C701ED.public preserve=yes
      //## end ClServerProxySpaceSS%380B45C701ED.public

  protected:
    // Additional Protected Declarations
      //## begin ClServerProxySpaceSS%380B45C701ED.protected preserve=yes
      //## end ClServerProxySpaceSS%380B45C701ED.protected

  private:
    // Additional Private Declarations
      //## begin ClServerProxySpaceSS%380B45C701ED.private preserve=yes
      //## end ClServerProxySpaceSS%380B45C701ED.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServerProxySpaceSS%380B45C701ED.implementation preserve=yes
      //## end ClServerProxySpaceSS%380B45C701ED.implementation

};

//## begin ClServerProxySpaceSS%380B45C701ED.postscript preserve=yes
//## end ClServerProxySpaceSS%380B45C701ED.postscript

//## begin ClProxyNotify%35C084650114.preface preserve=yes
//## end ClProxyNotify%35C084650114.preface

//## Class: ClProxyNotify%35C084650114
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClProxyNotify 
{
  //## begin ClProxyNotify%35C084650114.initialDeclarations preserve=yes
  //## end ClProxyNotify%35C084650114.initialDeclarations

  public:
    //## begin ClProxyNotify::EventType%35C089DC00E4.preface preserve=yes
    //## end ClProxyNotify::EventType%35C089DC00E4.preface

    //## Class: EventType%35C089DC00E4
    //## Category: M4ExeForChannel::Proxy%392272AB0394
    //## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {PX_EVENT_UPDATE} EventType;

    //## begin ClProxyNotify::EventType%35C089DC00E4.postscript preserve=yes
    //## end ClProxyNotify::EventType%35C089DC00E4.postscript

    //## begin ClProxyNotify::EventParam%35C08A3A0360.preface preserve=yes
    //## end ClProxyNotify::EventParam%35C08A3A0360.preface

    //## Class: EventParam%35C08A3A0360
    //## Category: M4ExeForChannel::Proxy%392272AB0394
    //## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef void *EventParam;

    //## begin ClProxyNotify::EventParam%35C08A3A0360.postscript preserve=yes
    //## end ClProxyNotify::EventParam%35C08A3A0360.postscript

    //## begin ClProxyNotify::EventHandler%35C08BB7008C.preface preserve=yes
    //## end ClProxyNotify::EventHandler%35C08BB7008C.preface

    //## Class: EventHandler%35C08BB7008C
    //## Category: M4ExeForChannel::Proxy%392272AB0394
    //## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef m4return_t (*EventHandler)(EventType ai_iEventType, EventParam ai_oEventParam);

    //## begin ClProxyNotify::EventHandler%35C08BB7008C.postscript preserve=yes
    //## end ClProxyNotify::EventHandler%35C08BB7008C.postscript

    //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.preface preserve=yes
    //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.preface

    //## Class: ClGenericHost%35C0AF0F01FF
    //## Category: M4ExeForChannel::Proxy%392272AB0394
    //## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    class ClGenericHost 
    {
      //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.initialDeclarations preserve=yes
      //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.initialDeclarations

      public:
        // Additional Public Declarations
          //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.public preserve=yes
          //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.public

      protected:
        // Additional Protected Declarations
          //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.protected preserve=yes
          //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.protected

      private:
        // Additional Private Declarations
          //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.private preserve=yes
          //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.private

      private: //## implementation
        // Additional Implementation Declarations
          //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.implementation preserve=yes
          //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.implementation

    };

    //## begin ClProxyNotify::ClGenericHost%35C0AF0F01FF.postscript preserve=yes
    //## end ClProxyNotify::ClGenericHost%35C0AF0F01FF.postscript

    //## Constructors (specified)
      //## Operation: ClProxyNotify%901810341
      ClProxyNotify ();

    //## Destructor (specified)
      //## Operation: ~ClProxyNotify%901810342
      ~ClProxyNotify ();


    //## Other Operations (specified)
      //## Operation: RegisterReceiver%901810340
      m4return_t RegisterReceiver (EventHandler ai_pEventHandler, ClGenericHost *ai_poGenericHost);

      //## Operation: SendEvent%901810343
      m4return_t SendEvent (EventType ai_iEventType, EventParam ai_oEventParam);

      //## Operation: GetHost%901810345
      ClProxyNotify::ClGenericHost * GetHost ();

  public:
    // Additional Public Declarations
      //## begin ClProxyNotify%35C084650114.public preserve=yes
      //## end ClProxyNotify%35C084650114.public

  protected:
    // Additional Protected Declarations
      //## begin ClProxyNotify%35C084650114.protected preserve=yes
      //## end ClProxyNotify%35C084650114.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oEventHandler%35C08E4200E3
      //## begin ClProxyNotify::m_oEventHandler%35C08E4200E3.attr preserve=no  private: EventHandler {UA} NULL
      EventHandler m_oEventHandler;
      //## end ClProxyNotify::m_oEventHandler%35C08E4200E3.attr

      //## Attribute: m_poClGenericHost%35C0B120012E
      //## begin ClProxyNotify::m_poClGenericHost%35C0B120012E.attr preserve=no  private: ClGenericHost * {UA} NULL
      ClGenericHost *m_poClGenericHost;
      //## end ClProxyNotify::m_poClGenericHost%35C0B120012E.attr

    // Additional Private Declarations
      //## begin ClProxyNotify%35C084650114.private preserve=yes
      //## end ClProxyNotify%35C084650114.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProxyNotify%35C084650114.implementation preserve=yes
      //## end ClProxyNotify%35C084650114.implementation

};

//## begin ClProxyNotify%35C084650114.postscript preserve=yes
//## end ClProxyNotify%35C084650114.postscript

//## Class: ClServerProxyManager%3507E672030D
//## Category: M4ExeForChannel::Proxy%392272AB0394
//## Subsystem: M4ExeForChannel::Proxy::inc%3922742E0123
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%364AEF28032C;ClMutBlock { -> F}
//## Uses: <unnamed>%3922BBC4002A;ClChannelManager { -> F}
//## Uses: <unnamed>%3922BC5803BD;ClLConn_Provider { -> F}

class M4_DECL_M4EXEFORCHANNEL ClServerProxyManager : public ClProxyManager  //## Inherits: <unnamed>%3821CDCE00FF
{
  public:
    //## Constructors (specified)
      //## Operation: ClServerProxyManager%889780470
      ClServerProxyManager (ClCache *ai_poClCache = NULL);

    //## Destructor (specified)
      //## Operation: ~ClServerProxyManager%889780471
      ~ClServerProxyManager ();


    //## Other Operations (specified)
      //## Operation: GetSpace%889702422
      //	Devuelve un espacio proxy. Este se selecciona por el parámetro ai_pcClProxy
      //	SpaceID.
      //	Si el espacio proxy no existe se crea uno nuevo si ai_bCreateNew == M4_TRUE.
      //
      //	@param ai_pcClProxySpaceID    Id del espacio proxy que se quiere.
      //	@param ai_bCreateNew    Si no existe el espacio proxy y se pasa el valor M4_
      //	TRUE --> se crea un nuevo espacio proxy.
      //	@return ClProxySpace    Espacio proxy. Si no se encontro y ai_bCreateNew ==
      //	M4_FALSE --> se devuelve NULL.
      virtual ClProxySpace* const GetSpace (const m4char_t* const ai_pcClProxySpaceID, const m4bool_t ai_bCreateNew = M4_FALSE);

      //## Operation: GetSpace%941105291
      //	Busca un ClProxySpace por el SessionId. Si lo encuentra nos devuelve un
      //	puntero a él.
      //
      //	Si no lo encuentra nos devuelve NULL.
      ClProxySpace * GetSpace (const m4uint32_t ai_iSessionId);

      //## Operation: ReleaseSpace%889702423
      virtual m4return_t ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock = M4_TRUE);

      //## Operation: ReleaseSpace%942076255
      m4return_t ReleaseSpace (const m4uint32_t ai_iSessionId);

      //## Operation: IsServerMode%897990369
      virtual m4bool_t IsServerMode () const;

      //## Operation: GetpMutAccess%901615380
      ClMutex * GetpMutAccess ();

      //## Operation: SendEvent%901872838
      virtual m4return_t SendEvent ();

      //## Operation: SetMaxNSpaces%916914831
      void SetMaxNSpaces (const m4int32_t ai_iMaxNSpaces);

      //## Operation: GetMaxNSpaces%945168301
      m4int32_t GetMaxNSpaces () const;

      //## Operation: ReleaseActiveSpace%926076992
      virtual m4return_t ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID);

      //## Operation: SetProxySpacesSS%943008107
      void SetProxySpacesSS (ClProxySpacesSS& ai_oProxySpacesSS);

      //## Operation: Detach%951403969
      m4return_t Detach (ClProxySpace* ai_poProxySpace);

      //## Operation: ReleaseSpace%994267998
      m4return_t ReleaseSpace (const m4uint32_t ai_iSessionId, const m4uint32_t ai_iRequestId);

    // Data Members for Associations

      //## Association: M4ExeForChannel::Proxy::<unnamed>%35C166B90294
      //## Role: ClServerProxyManager::m_oClProxyNotify%35C166BC00A3
      //## begin ClServerProxyManager::m_oClProxyNotify%35C166BC00A3.role preserve=no  public: static ClProxyNotify {1 -> 1VHAN}
      static ClProxyNotify m_oClProxyNotify;
      //## end ClServerProxyManager::m_oClProxyNotify%35C166BC00A3.role

  protected:
  private:
    // Data Members for Class Attributes

      //## Attribute: m_bStatus%3528F92900C4
      //## begin ClServerProxyManager::m_bStatus%3528F92900C4.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bStatus;
      //## end ClServerProxyManager::m_bStatus%3528F92900C4.attr

      //## Attribute: m_iMaxNSpaces%36A71D66031B
      //## begin ClServerProxyManager::m_iMaxNSpaces%36A71D66031B.attr preserve=no  private: m4int32_t {UA} -1
      m4int32_t m_iMaxNSpaces;
      //## end ClServerProxyManager::m_iMaxNSpaces%36A71D66031B.attr

      //## Attribute: m_iNSpaces%36A729C00386
      //## begin ClServerProxyManager::m_iNSpaces%36A729C00386.attr preserve=no  private: m4int32_t {UA} 0
      m4int32_t m_iNSpaces;
      //## end ClServerProxyManager::m_iNSpaces%36A729C00386.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::Proxy::<unnamed>%38229CDF028B
      //## Role: ClServerProxyManager::m_pProxySpacesSS%38229CE00110
      //## begin ClServerProxyManager::m_pProxySpacesSS%38229CE00110.role preserve=no  private: ClProxySpacesSS {1 -> 1RFHGAN}
      ClProxySpacesSS *m_pProxySpacesSS;
      //## end ClServerProxyManager::m_pProxySpacesSS%38229CE00110.role

      //## Association: M4ExeForChannel::Proxy::<unnamed>%359CF5B0006B
      //## Role: ClServerProxyManager::m_poClCache%359CF5B002C4
      //## begin ClServerProxyManager::m_poClCache%359CF5B002C4.role preserve=no  private: ClCache {n -> 1RFHAN}
      ClCache *m_poClCache;
      //## end ClServerProxyManager::m_poClCache%359CF5B002C4.role

      //## Association: M4ExeForChannel::Proxy::<unnamed>%364AEA9502B0
      //## Role: ClServerProxyManager::m_poMutAccess%364AEA960153
      //## begin ClServerProxyManager::m_poMutAccess%364AEA960153.role preserve=no  private: ClMutex {1 -> 1RFHAN}
      ClMutex *m_poMutAccess;
      //## end ClServerProxyManager::m_poMutAccess%364AEA960153.role

  private: //## implementation
};

// Class ClServerProxySpaceSS 

// Class ClProxyNotify::ClGenericHost 

// Class ClProxyNotify 

// Class ClServerProxyManager 


//## Other Operations (inline)
inline m4bool_t ClServerProxyManager::IsServerMode () const
{
  //## begin ClServerProxyManager::IsServerMode%897990369.body preserve=yes
	return M4_TRUE;
  //## end ClServerProxyManager::IsServerMode%897990369.body
}

inline m4int32_t ClServerProxyManager::GetMaxNSpaces () const
{
  //## begin ClServerProxyManager::GetMaxNSpaces%945168301.body preserve=yes
	return m_iMaxNSpaces;
  //## end ClServerProxyManager::GetMaxNSpaces%945168301.body
}

//## begin module%3830034B02B4.epilog preserve=yes
//## end module%3830034B02B4.epilog


#endif
