//## begin module%3507CDCE000A.cm preserve=no
//## end module%3507CDCE000A.cm

//## begin module%3507CDCE000A.cp preserve=no
//	=============================================================================
//
//	(c) Copyright  1991-1997 Meta Software M.S., S.A
//	All rights reserved.
//
//	Module:            <module>
//	File:              genproxy.hpp
//	Project:           M4OeInit
//	Author:            Meta Software M.S. , S.A
//	Date:
//	Language:          C++
//	Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	Design Document:
//
//	Definition:
//
//	   This module defines...
//
//
//	=============================================================================
//## end module%3507CDCE000A.cp

//## Module: Generic_Proxy%3507CDCE000A; Package specification
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Source file: D:\Work\v600\m4oeinit\inc\genproxy.hpp

#ifndef generic_proxy_h
#define generic_proxy_h 1

//## begin module%3507CDCE000A.additionalIncludes preserve=no
//## end module%3507CDCE000A.additionalIncludes

//## begin module%3507CDCE000A.includes preserve=yes
#include "m4oeinit_dll.hpp"
#include "m4ipcs_dll.hpp"
#include "iproxymanager.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"
//## end module%3507CDCE000A.includes

// ClCache
#include <clcache.hpp>

class ClM4ObjService;
class ClAppClientAdap;
class M4_DECL_M4OEINIT ClProxySpaceContainer;
class ClDataStoreIODriver;
class ClAccess;
class ClChannel;
class ClChannelManager;

//## begin module%3507CDCE000A.declarations preserve=no
//## end module%3507CDCE000A.declarations

//## begin module%3507CDCE000A.additionalDeclarations preserve=yes
#define PROXY_MAX_USERNAME	64
const m4int16_t PROXY_MAX_ID		= 128;
const m4int16_t PROXY_MAX_VIEW_ID	= 128;
//## end module%3507CDCE000A.additionalDeclarations


//## begin ClProxyManager%3507E576022F.preface preserve=yes
//## end ClProxyManager%3507E576022F.preface

//## Class: ClProxyManager%3507E576022F
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OEINIT ClProxyManager : public IProxyManager  //## Inherits: <unnamed>%3821CDC70195
{
  //## begin ClProxyManager%3507E576022F.initialDeclarations preserve=yes
  //## end ClProxyManager%3507E576022F.initialDeclarations

  public:
    //## Class: ExecTypeCommand%353734ED003C
    //## Category: M4OeInit%392113790074
    //## Subsystem: M4Oeinit::inc%38F5D6290254
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { PROXY_GET_SPACE=0, PROXY_GET_VIEW, PROXY_RELEASE_SPACE, PROXY_RELEASE_VIEW, PROXY_DESTROY_SPACE, PROXY_DESTROY_VIEW, PROXY_EXECUTE_COMMAND, PROXY_GET_LIST, PROXY_GET_DEBUG_VIEW, PROXY_END_DEBUG_METHOD, PROXY_ERASE_ALL_L2_INSTANCES } ExecTypeCommand;

    //## Constructors (specified)
      //## Operation: ClProxyManager%897069668
      ClProxyManager ();

    //## Destructor (specified)
      //## Operation: ~ClProxyManager%897069669
      virtual ~ClProxyManager ();


    //## Other Operations (specified)
      //## Operation: SetCaches%897069670
      virtual void SetCaches (ClCache* const ai_poCMCRCache, ClCache* const ai_poCSCRCache, ClCache* const ai_poPresentationCache, ClCache* const ai_poMapCache, ClCache* const ai_poDatacache);

      //## Operation: GetCaches%897069671
      virtual void GetCaches (ClCache * &ao_poCMCRCache, ClCache * &ao_poCSCRCache, ClCache * &ao_poPresentationCache, ClCache * &ao_poMapCache, ClCache * &ao_poDatacache) const;

      //## Operation: SendEvent%910879750
      virtual m4return_t SendEvent ();

      //## Operation: Reset%994166232
      virtual void Reset ();

      //## Operation: EraseAllL2Instances%1025256483
      virtual m4return_t EraseAllL2Instances (const m4char_t* const ai_pcClProxySpaceID, m4uint32_t ai_iRemoteId);

  public:
    // Additional Public Declarations
      //## begin ClProxyManager%3507E576022F.public preserve=yes
      //## end ClProxyManager%3507E576022F.public

  protected:
    // Additional Protected Declarations
      //## begin ClProxyManager%3507E576022F.protected preserve=yes
      //## end ClProxyManager%3507E576022F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poCMCRCache%357830F1003E
      //## begin ClProxyManager::m_poCMCRCache%357830F1003E.attr preserve=no  private: ClCache * {UA} NULL
      ClCache *m_poCMCRCache;
      //## end ClProxyManager::m_poCMCRCache%357830F1003E.attr

      //## Attribute: m_poCSCRCache%357831060279
      //## begin ClProxyManager::m_poCSCRCache%357831060279.attr preserve=no  private: ClCache * {UA} NULL
      ClCache *m_poCSCRCache;
      //## end ClProxyManager::m_poCSCRCache%357831060279.attr

      //## Attribute: m_poPresentationCache%35A5161D0241
      //## begin ClProxyManager::m_poPresentationCache%35A5161D0241.attr preserve=no  private: ClCache * {UA} NULL
      ClCache *m_poPresentationCache;
      //## end ClProxyManager::m_poPresentationCache%35A5161D0241.attr

      ClCache *m_poMapCache;

      //## Attribute: m_poDatacache%35A5162100E8
      //## begin ClProxyManager::m_poDatacache%35A5162100E8.attr preserve=no  private: ClCache * {UA} NULL
      ClCache *m_poDatacache;
      //## end ClProxyManager::m_poDatacache%35A5162100E8.attr

    // Additional Private Declarations
      //## begin ClProxyManager%3507E576022F.private preserve=yes
      //## end ClProxyManager%3507E576022F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProxyManager%3507E576022F.implementation preserve=yes
      //## end ClProxyManager%3507E576022F.implementation

};

//## begin ClProxyManager%3507E576022F.postscript preserve=yes
//## end ClProxyManager%3507E576022F.postscript

//## begin ClProxyList%35800E7E014F.preface preserve=yes
//## end ClProxyList%35800E7E014F.preface

//## Class: ClProxyList%35800E7E014F
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OEINIT ClProxyList 
{
  //## begin ClProxyList%35800E7E014F.initialDeclarations preserve=yes
  //## end ClProxyList%35800E7E014F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClProxyList%897898134
      ClProxyList ();

    //## Destructor (specified)
      //## Operation: ~ClProxyList%897898135
      virtual ~ClProxyList ();


    //## Other Operations (specified)
      //## Operation: SetId%897898136
      m4return_t SetId (m4pchar_t ai_pcId);

      //## Operation: SetLocalUniqueId%897898137
      void SetLocalUniqueId (m4uint32_t ai_iLocalUniqueId);

      //## Operation: SetRemoteUniqueId%897898138
      void SetRemoteUniqueId (m4uint32_t ai_iRemoteUniqueId);

      //## Operation: Insert%897898139
      m4return_t Insert (ClProxyList *ai_poItem);

      //## Operation: InsertChild%897898145
      m4return_t InsertChild (ClProxyList *ai_poItem);

      //## Operation: Count%897898140
      m4int32_t Count ();

      //## Operation: Get%897898141
      ClProxyList * Get (m4int32_t ai_iIndex);

      //## Operation: GetChild%897898146
      ClProxyList * GetChild (m4int32_t ai_iIndex);

      //## Operation: GetId%897898142
      const m4pchar_t GetId ();

      //## Operation: GetLocalUniqueId%897898143
      m4uint32_t GetLocalUniqueId () const;

      //## Operation: GetRemoteUniqueId%897898144
      m4uint32_t GetRemoteUniqueId () const;

    // Additional Public Declarations
      //## begin ClProxyList%35800E7E014F.public preserve=yes
      //## end ClProxyList%35800E7E014F.public

  protected:
    // Additional Protected Declarations
      //## begin ClProxyList%35800E7E014F.protected preserve=yes
      //## end ClProxyList%35800E7E014F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poNext%35801B1B02E2
      //## begin ClProxyList::m_poNext%35801B1B02E2.attr preserve=no  private: ClProxyList * {UA} NULL
      ClProxyList *m_poNext;
      //## end ClProxyList::m_poNext%35801B1B02E2.attr

      //## Attribute: m_poChild%3584E4D90128
      //## begin ClProxyList::m_poChild%3584E4D90128.attr preserve=no  private: ClProxyList * {UA} NULL
      ClProxyList *m_poChild;
      //## end ClProxyList::m_poChild%3584E4D90128.attr

      //## Attribute: m_pcId%358017580193
      //## begin ClProxyList::m_pcId%358017580193.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pcId;
      //## end ClProxyList::m_pcId%358017580193.attr

      //## Attribute: m_iLocalUniqueId%3580178B0269
      //## begin ClProxyList::m_iLocalUniqueId%3580178B0269.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iLocalUniqueId;
      //## end ClProxyList::m_iLocalUniqueId%3580178B0269.attr

      //## Attribute: m_iRemoteUniqueId%3580179A0260
      //## begin ClProxyList::m_iRemoteUniqueId%3580179A0260.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iRemoteUniqueId;
      //## end ClProxyList::m_iRemoteUniqueId%3580179A0260.attr

    // Additional Private Declarations
      //## begin ClProxyList%35800E7E014F.private preserve=yes
      //## end ClProxyList%35800E7E014F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClProxyList%35800E7E014F.implementation preserve=yes
      //## end ClProxyList%35800E7E014F.implementation

};

//## begin ClProxyList%35800E7E014F.postscript preserve=yes
//## end ClProxyList%35800E7E014F.postscript

//## Class: ClProxySpace%3507CEE801E6
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3565A6B1037D;ClChannel { -> F}
//## Uses: <unnamed>%358545FF021A;ClDataStoreIODriver { -> F}
//## Uses: <unnamed>%39225475020D;ClChannelManager { -> F}
//## Uses: <unnamed>%3D1C436B033F;ClAccess { -> F}

class M4_DECL_M4OEINIT ClProxySpace 
{
  public:
    //## Constructors (specified)
      //## Operation: ClProxySpace%889780466
      ClProxySpace (ClProxyManager* const ai_poClProxyManager, ClM4ObjService* const ai_pObjService);

    //## Destructor (specified)
      //## Operation: ~ClProxySpace%889780467
      ~ClProxySpace ();


    //## Other Operations (specified)
      //## Operation: SetContainer%891866790
      m4return_t SetContainer (ClProxySpaceContainer *ai_poClProxySpaceContainer);

      //## Operation: GetProxyManager%892460577
      ClProxyManager * GetProxyManager ();

      //## Operation: SetUserName%931259996
      virtual void SetUserName (m4pcchar_t ai_pcUserName);

      //## Operation: GetProxySpaceContainer%892460578
      ClProxySpaceContainer * GetProxySpaceContainer ();

      //## Operation: GetObjService%1031648364
      ClM4ObjService * GetObjService () const;

      //## Operation: GetChannelManager%892460579
      ClChannelManager * GetChannelManager ();

      //## Operation: GetUniqueID%892624819
      const m4pchar_t GetUniqueID () const;

      //## Operation: GetBaseID%892624820
      const m4pchar_t GetBaseID () const;

      //## Operation: SetBaseID%892624821
      m4return_t SetBaseID (const m4char_t* const ai_pcID);

      //## Operation: SetUniqueID%892624822
      m4return_t SetUniqueID (const m4char_t* const ai_pcID);

      //## Operation: SetUseStatus%892624823
      void SetUseStatus (const m4bool_t ai_bUseStatus);

      //## Operation: GetUseStatus%892624824
      m4bool_t GetUseStatus ();

      //## Operation: ResetUniqueID%893671198
      m4return_t ResetUniqueID ();

      //## Operation: SetSessionChannel%897069672
      m4return_t SetSessionChannel (ClChannel *ai_poSessionChannel, m4uint32_t ai_iSessionId, m4pcchar_t ai_pcUserName);

      //## Operation: GetProxyView%895830822
      m4return_t GetProxyView (ClChannel *aio_poChannel);

      //## Operation: GetProxyDebugView%898022272
      ClChannel * GetProxyDebugView (m4pcchar_t ai_pcLocalId, const m4uint32_t ai_iRemoteId);

      //## Operation: EndDebugMethod%964538634
      m4return_t EndDebugMethod ();

      //## Operation: IsServerMode%896103145
      m4bool_t IsServerMode ();

      //## Operation: GetProxyList%897898147
      ClProxyList * GetProxyList ();

      //## Operation: ReadProxyList%897898148
      m4return_t ReadProxyList (ClDataStoreIODriver *ai_poIOD, ClProxyList *ai_poClProxyList = NULL);

      //## Operation: GetSessionId%901533963
      m4uint32_t GetSessionId ();

      //## Operation: RequestToAbort%928757333
      void RequestToAbort ();

      //## Operation: SetSessionId%901533964
      virtual m4return_t SetSessionId (m4uint32_t ai_iSessionId, m4pcchar_t ai_pcUserName);

      //## Operation: IsRequestToAbort%901615386
      m4bool_t IsRequestToAbort () const;

      //## Operation: SetRequestToAbort%928757332
      m4return_t SetRequestToAbort (m4bool_t ai_bRequestToAbort = M4_TRUE);

      //## Operation: GetUserName%931259995
      m4pcchar_t GetUserName ();

      //## Operation: FreeLogicalConnection%958471562
      //	Libera la conexión lógica asociada al espacio proxy.
      virtual m4return_t FreeLogicalConnection ();

      //## Operation: EraseAllL2Instances%1025256484
      m4return_t EraseAllL2Instances (m4uint32_t ai_iRemoteId);

      //## Operation: ReleaseSessionAcceses%1025256485
      m4return_t ReleaseSessionAcceses ();

    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: RequestId%3B43512102B8
      const m4uint32_t& GetRequestId () const;
      void SetRequestId (const m4uint32_t& value);

      //## Attribute: bIsRunning%3EDC587E008E
      const m4bool_t& GetIsRunning () const;
      void SetIsRunning (const m4bool_t& value);

  protected:
    // Data Members for Class Attributes

      //## begin ClProxySpace::bIsRunning%3EDC587E008E.attr preserve=no  public: m4bool_t {U} M4_FALSE
      m4bool_t m_bIsRunning;
      //## end ClProxySpace::bIsRunning%3EDC587E008E.attr

    // Data Members for Associations

      //## Association: M4OeInit::<unnamed>%3921817C005C
      //## Role: ClProxySpace::m_pObjService%3921817C032D
      //## begin ClProxySpace::m_pObjService%3921817C032D.role preserve=no  protected: ClM4ObjService { -> 1RFHAN}
      ClM4ObjService *m_pObjService;
      //## end ClProxySpace::m_pObjService%3921817C032D.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pcBaseID%3507CF6B013A
      //## begin ClProxySpace::m_pcBaseID%3507CF6B013A.attr preserve=no  private: m4pchar_t {UA} 0
      m4pchar_t m_pcBaseID;
      //## end ClProxySpace::m_pcBaseID%3507CF6B013A.attr

      //## Attribute: m_bStatus%3528E45C0045
      //## begin ClProxySpace::m_bStatus%3528E45C0045.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bStatus;
      //## end ClProxySpace::m_bStatus%3528E45C0045.attr

      //## Attribute: m_pcUniqueID%3534B119032D
      //## begin ClProxySpace::m_pcUniqueID%3534B119032D.attr preserve=no  private: m4pchar_t {UA} 0
      m4pchar_t m_pcUniqueID;
      //## end ClProxySpace::m_pcUniqueID%3534B119032D.attr

      //## Attribute: m_bUseStatus%3534CF630251
      //## begin ClProxySpace::m_bUseStatus%3534CF630251.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bUseStatus;
      //## end ClProxySpace::m_bUseStatus%3534CF630251.attr

      //## Attribute: m_bObjServiceIsLocal%356BE4090138
      //## begin ClProxySpace::m_bObjServiceIsLocal%356BE4090138.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bObjServiceIsLocal;
      //## end ClProxySpace::m_bObjServiceIsLocal%356BE4090138.attr

      //## Attribute: m_pcUserName%3782207F00BB
      //## begin ClProxySpace::m_pcUserName%3782207F00BB.attr preserve=no  private: m4pchar_t {UA} 0
      m4pchar_t m_pcUserName;
      //## end ClProxySpace::m_pcUserName%3782207F00BB.attr

      //## Attribute: m_iSessionId%35BC501802CB
      //## begin ClProxySpace::m_iSessionId%35BC501802CB.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iSessionId;
      //## end ClProxySpace::m_iSessionId%35BC501802CB.attr

      //## Attribute: m_bRequestToAbort%375BB542032E
      //## begin ClProxySpace::m_bRequestToAbort%375BB542032E.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bRequestToAbort;
      //## end ClProxySpace::m_bRequestToAbort%375BB542032E.attr

      //## begin ClProxySpace::RequestId%3B43512102B8.attr preserve=no  public: m4uint32_t {U} 0
      m4uint32_t m_uiRequestId;
      //## end ClProxySpace::RequestId%3B43512102B8.attr

    // Data Members for Associations

      //## Association: M4OeInit::<unnamed>%3528D3F203AA
      //## Role: ClProxySpace::m_poClProxySpaceContainer%3528D3F301B6
      //## begin ClProxySpace::m_poClProxySpaceContainer%3528D3F301B6.role preserve=no  private: ClProxySpaceContainer {1 -> 1RFHGAN}
      ClProxySpaceContainer *m_poClProxySpaceContainer;
      //## end ClProxySpace::m_poClProxySpaceContainer%3528D3F301B6.role

      //## Association: M4OeInit::<unnamed>%3584F9EB02F1
      //## Role: ClProxySpace::m_poClProxyList%3584F9EE009C
      //## begin ClProxySpace::m_poClProxyList%3584F9EE009C.role preserve=no  private: ClProxyList {1 -> 1RHAN}
      ClProxyList *m_poClProxyList;
      //## end ClProxySpace::m_poClProxyList%3584F9EE009C.role

      //## Association: M4OeInit::<unnamed>%3507E6A3015F
      //## Role: ClProxySpace::m_poClProxyManager%3507E6A40125
      //## begin ClProxySpace::m_poClProxyManager%3507E6A40125.role preserve=no  private: ClProxyManager {1 -> 1RHAN}
      ClProxyManager *m_poClProxyManager;
      //## end ClProxySpace::m_poClProxyManager%3507E6A40125.role

      //## Association: M4OeInit::<unnamed>%3D1C309202BF
      //## Role: ClProxySpace::m_vpSessionAccesses%3D1C309300C2
      //## begin ClProxySpace::m_vpSessionAccesses%3D1C309300C2.role preserve=no  private: vector {1 -> VHAN}
      vector<ClAccess*> m_vpSessionAccesses;
      //## end ClProxySpace::m_vpSessionAccesses%3D1C309300C2.role

  private: //## implementation
};

//## Class: ClClientProxyManager%3507E638009D
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClClientProxyManager : public ClProxyManager  //## Inherits: <unnamed>%3507E66D0266
{
    //## Class: <unnamed>%39216FBA0240; private
    //## Category: M4OeInit%392113790074
    //## Subsystem: M4Oeinit%38F5D618035E
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { MAX_NUM_PROXY_SPACES = 2 } ;

  public:
    //## Constructors (specified)
      //## Operation: ClClientProxyManager%889780468
      ClClientProxyManager (ClM4ObjService* const ai_pObjService, ClAppClientAdap* const ai_pClClientAPI);

    //## Destructor (specified)
      //## Operation: ~ClClientProxyManager%889780469
      ~ClClientProxyManager ();


    //## Other Operations (specified)
      //## Operation: GetSpace%889702424
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

      //## Operation: ReleaseSpace%889702425
      virtual m4return_t ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock = M4_TRUE);

      //## Operation: GetClientAPI%896180929
      ClAppClientAdap* const GetClientAPI ();

      //## Operation: GetContext%896180931
      m4handle_t GetContext () const;

      //## Operation: SetServerArch%897554919
      void SetServerArch (const m4uint16_t ai_iServerArch);

      //## Operation: GetServerArch%897554920
      m4uint16_t GetServerArch () const;

      //## Operation: IsServerMode%897990368
      virtual m4bool_t IsServerMode () const;

      //## Operation: ReleaseActiveSpace%949597711
      virtual m4return_t ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID);

      //## Operation: Reset%994165400
      virtual void Reset ();

  protected:
  private:

    //## Other Operations (specified)
      //## Operation: CreateSpace%958471561
      //	Crea un nuevo espacio proxy, tanto en el lado cliente como en el lado
      //	servidor.
      //
      //	@param ai_pProxySpaceID    Nombre base del espacio proxy que se quiere
      //	crear. El lado servidor determinará cual será el nombre único.
      //	@return ClProxySpace* const
      ClProxySpace* const CreateSpace (const m4char_t* const ai_pProxySpaceID);

    // Data Members for Class Attributes

      //## Attribute: m_hContextID%353DE6BC0170
      //## begin ClClientProxyManager::m_hContextID%353DE6BC0170.attr preserve=no  private: m4handle_t {UA} 0
      m4handle_t m_hContextID;
      //## end ClClientProxyManager::m_hContextID%353DE6BC0170.attr

      //## Attribute: m_iServerArch%357F99160088
      //## begin ClClientProxyManager::m_iServerArch%357F99160088.attr preserve=no  private: m4uint16_t {UA} -1
      m4uint16_t m_iServerArch;
      //## end ClClientProxyManager::m_iServerArch%357F99160088.attr

    // Data Members for Associations

      //## Association: M4OeInit::<unnamed>%39216AD70010
      //## Role: ClClientProxyManager::m_poClientAPI%39216AD702A5
      //## begin ClClientProxyManager::m_poClientAPI%39216AD702A5.role preserve=no  private: ClAppClientAdap { -> 1RFHAN}
      ClAppClientAdap *m_poClientAPI;
      //## end ClClientProxyManager::m_poClientAPI%39216AD702A5.role

      //## Association: M4OeInit::<unnamed>%39216BDE036C
      //## Role: ClClientProxyManager::m_pObjService%39216BDF02D7
      //	Sólo es necesario en la parte cliente para pasarselo al espacio proxy.
      //	En servidor cada espacio proxy se crea el suyo propio.
      //## begin ClClientProxyManager::m_pObjService%39216BDF02D7.role preserve=no  private: ClM4ObjService { -> 1RFHAN}
      ClM4ObjService *m_pObjService;
      //## end ClClientProxyManager::m_pObjService%39216BDF02D7.role

      //## Association: M4OeInit::<unnamed>%39218C220016
      //## Role: ClClientProxyManager::m_vProxySpace%39218C2203AF
      //	Vector de dos posiciones para guardar los espacios proxy en cliente.
      //	La primera posición para un espacio proxy normal (en el se encuentran los
      //	canales proxy).
      //	La segunda posición para el espacio proxy para depurar canales OLTP.
      //## begin ClClientProxyManager::m_vProxySpace%39218C2203AF.role preserve=no  private: ClProxySpace { -> RHAN}
      ClProxySpace *m_vProxySpace[MAX_NUM_PROXY_SPACES];
      //## end ClClientProxyManager::m_vProxySpace%39218C2203AF.role

  private: //## implementation
};

//## Class: ClProxySpaceContainer%3528D0F702D0
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OEINIT ClProxySpaceContainer : public ClCacheableObject  //## Inherits: <unnamed>%3528D2270205
{
  public:
    //## Constructors (specified)
      //## Operation: ClProxySpaceContainer%891866786
      ClProxySpaceContainer (ClProxySpace *ai_poClProxySpace);

    //## Destructor (specified)
      //## Operation: ~ClProxySpaceContainer%891866787
      ~ClProxySpaceContainer ();


    //## Other Operations (specified)
      //## Operation: GetKey%891866788
      m4return_t GetKey (ClCacheKey * &  ao_poClCacheKey);

      //## Operation: SetKey%891866789
      m4return_t SetKey (ClCacheKey *ai_poClCacheKey);

      //## Operation: GetProxySpace%891950692
      ClProxySpace * GetProxySpace ();

  protected:
  private:
    // Data Members for Class Attributes

      //## Attribute: m_poClCacheKey%352A15C20353
      //## begin ClProxySpaceContainer::m_poClCacheKey%352A15C20353.attr preserve=no  private: ClCacheKey * {UA} 
      ClCacheKey *m_poClCacheKey;
      //## end ClProxySpaceContainer::m_poClCacheKey%352A15C20353.attr

    // Data Members for Associations

      //## Association: M4OeInit::<unnamed>%3528D3F203AA
      //## Role: ClProxySpaceContainer::m_poClProxySpace%3528D3F30243
      //## begin ClProxySpaceContainer::m_poClProxySpace%3528D3F30243.role preserve=no  private: ClProxySpace {1 -> 1RHAN}
      ClProxySpace *m_poClProxySpace;
      //## end ClProxySpaceContainer::m_poClProxySpace%3528D3F30243.role

  private: //## implementation
};

//## begin ClServerProxyManagerEmul%3899B3CC0296.preface preserve=yes
//## end ClServerProxyManagerEmul%3899B3CC0296.preface

//## Class: ClServerProxyManagerEmul%3899B3CC0296
//## Category: M4OeInit%392113790074
//## Subsystem: M4Oeinit::inc%38F5D6290254
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClServerProxyManagerEmul : public ClProxyManager  //## Inherits: <unnamed>%3899B3E20175
{
  //## begin ClServerProxyManagerEmul%3899B3CC0296.initialDeclarations preserve=yes
  //## end ClServerProxyManagerEmul%3899B3CC0296.initialDeclarations

    //## Class: <unnamed>%39217D36003B; private
    //## Category: M4OeInit%392113790074
    //## Subsystem: M4Oeinit%38F5D618035E
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { MAX_NUM_PROXY_SPACES = 2 } ;

  public:
    //## Constructors (specified)
      //## Operation: ClServerProxyManagerEmul%949597696
      ClServerProxyManagerEmul ();

    //## Destructor (specified)
      //## Operation: ~ClServerProxyManagerEmul%949597697
      virtual ~ClServerProxyManagerEmul ();


    //## Other Operations (specified)
      //## Operation: GetSpace%949597698
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

      //## Operation: GetSpace%949597699
      //	Busca un ClProxySpace por el SessionId. Si lo encuentra nos devuelve un
      //	puntero a él.
      //
      //	Si no lo encuentra nos devuelve NULL.
      ClProxySpace * GetSpace (const m4uint32_t ai_iSessionId);

      //## Operation: ReleaseSpace%949597700
      virtual m4return_t ReleaseSpace (ClProxySpace* const ai_poClProxySpace, const m4bool_t ai_bDoLock = M4_TRUE);

      //## Operation: ReleaseSpace%949597701
      m4return_t ReleaseSpace (const m4uint32_t ai_iSessionId);

      //## Operation: IsServerMode%949597708
      virtual m4bool_t IsServerMode () const;

      //## Operation: GetpMutAccess%949597702
      ClMutex * GetpMutAccess ();

      //## Operation: SendEvent%949597703
      virtual m4return_t SendEvent ();

      //## Operation: SetMaxNSpaces%949597704
      void SetMaxNSpaces (const m4int32_t ai_iMaxNSpaces);

      //## Operation: GetMaxNSpaces%949597709
      m4int32_t GetMaxNSpaces () const;

      //## Operation: ReleaseActiveSpace%949597705
      virtual m4return_t ReleaseActiveSpace (const m4char_t* const ai_pcClProxySpaceID);

      //## Operation: SetSessionChannel%949597710
      void SetSessionChannel (ClChannel* ai_poChannel);

      //## Operation: GetSessionChannel%949662092
      ClChannel* GetSessionChannel () const;

    // Additional Public Declarations
      //## begin ClServerProxyManagerEmul%3899B3CC0296.public preserve=yes
      //## end ClServerProxyManagerEmul%3899B3CC0296.public

  protected:
    // Additional Protected Declarations
      //## begin ClServerProxyManagerEmul%3899B3CC0296.protected preserve=yes
      //## end ClServerProxyManagerEmul%3899B3CC0296.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poSessionChannel%3899D86B00D3
      //## begin ClServerProxyManagerEmul::m_poSessionChannel%3899D86B00D3.attr preserve=no  private: ClChannel* {UA} NULL
      ClChannel* m_poSessionChannel;
      //## end ClServerProxyManagerEmul::m_poSessionChannel%3899D86B00D3.attr

    // Data Members for Associations

      //## Association: M4OeInit::<unnamed>%39218C2C033B
      //## Role: ClServerProxyManagerEmul::m_vProxySpace%39218C2D018E
      //	Vector de dos posiciones para guardar los espacios proxy en emulación en el
      //	lado servidor.
      //	Un espacio proxy normal (en el se encuentran los canales proxy).
      //	Otro espacio proxy para depurar canales OLTP.
      //	No tienen orden, el que se cree primero es el que ocupará la primera
      //	posición.
      //## begin ClServerProxyManagerEmul::m_vProxySpace%39218C2D018E.role preserve=no  private: ClProxySpace { -> RHAN}
      ClProxySpace *m_vProxySpace[MAX_NUM_PROXY_SPACES];
      //## end ClServerProxyManagerEmul::m_vProxySpace%39218C2D018E.role

    // Additional Private Declarations
      //## begin ClServerProxyManagerEmul%3899B3CC0296.private preserve=yes
      //## end ClServerProxyManagerEmul%3899B3CC0296.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClServerProxyManagerEmul%3899B3CC0296.implementation preserve=yes
      //## end ClServerProxyManagerEmul%3899B3CC0296.implementation

};

//## begin ClServerProxyManagerEmul%3899B3CC0296.postscript preserve=yes
//## end ClServerProxyManagerEmul%3899B3CC0296.postscript

// Class ClProxyManager 


//## Other Operations (inline)
inline m4return_t ClProxyManager::SendEvent ()
{
  //## begin ClProxyManager::SendEvent%910879750.body preserve=yes
	return M4_SUCCESS;
  //## end ClProxyManager::SendEvent%910879750.body
}

// Class ClProxyList 

// Class ClProxySpace 


//## Other Operations (inline)
inline ClM4ObjService * ClProxySpace::GetObjService () const
{
  //## begin ClProxySpace::GetObjService%1031648364.body preserve=yes
	return( m_pObjService ) ;
  //## end ClProxySpace::GetObjService%1031648364.body
}

inline m4bool_t ClProxySpace::IsRequestToAbort () const
{
  //## begin ClProxySpace::IsRequestToAbort%901615386.body preserve=yes
	return m_bRequestToAbort;
  //## end ClProxySpace::IsRequestToAbort%901615386.body
}

inline m4return_t ClProxySpace::SetRequestToAbort (m4bool_t ai_bRequestToAbort)
{
  //## begin ClProxySpace::SetRequestToAbort%928757332.body preserve=yes
	m_bRequestToAbort = ai_bRequestToAbort;
	return M4_SUCCESS;
  //## end ClProxySpace::SetRequestToAbort%928757332.body
}

//## Get and Set Operations for Class Attributes (inline)

inline const m4uint32_t& ClProxySpace::GetRequestId () const
{
  //## begin ClProxySpace::GetRequestId%3B43512102B8.get preserve=no
  return m_uiRequestId;
  //## end ClProxySpace::GetRequestId%3B43512102B8.get
}

inline void ClProxySpace::SetRequestId (const m4uint32_t& value)
{
  //## begin ClProxySpace::SetRequestId%3B43512102B8.set preserve=no
  m_uiRequestId = value;
  //## end ClProxySpace::SetRequestId%3B43512102B8.set
}

inline const m4bool_t& ClProxySpace::GetIsRunning () const
{
  //## begin ClProxySpace::GetIsRunning%3EDC587E008E.get preserve=no
  return m_bIsRunning;
  //## end ClProxySpace::GetIsRunning%3EDC587E008E.get
}

inline void ClProxySpace::SetIsRunning (const m4bool_t& value)
{
  //## begin ClProxySpace::SetIsRunning%3EDC587E008E.set preserve=no
  m_bIsRunning = value;
  //## end ClProxySpace::SetIsRunning%3EDC587E008E.set
}

// Class ClClientProxyManager 


//## Other Operations (inline)
inline m4bool_t ClClientProxyManager::IsServerMode () const
{
  //## begin ClClientProxyManager::IsServerMode%897990368.body preserve=yes
	return M4_FALSE;
  //## end ClClientProxyManager::IsServerMode%897990368.body
}

// Class ClProxySpaceContainer 

// Class ClServerProxyManagerEmul 


//## Other Operations (inline)
inline m4bool_t ClServerProxyManagerEmul::IsServerMode () const
{
  //## begin ClServerProxyManagerEmul::IsServerMode%949597708.body preserve=yes
	return M4_TRUE;
  //## end ClServerProxyManagerEmul::IsServerMode%949597708.body
}

inline m4int32_t ClServerProxyManagerEmul::GetMaxNSpaces () const
{
  //## begin ClServerProxyManagerEmul::GetMaxNSpaces%949597709.body preserve=yes
	return 1;
  //## end ClServerProxyManagerEmul::GetMaxNSpaces%949597709.body
}

inline void ClServerProxyManagerEmul::SetSessionChannel (ClChannel* ai_poChannel)
{
  //## begin ClServerProxyManagerEmul::SetSessionChannel%949597710.body preserve=yes
	m_poSessionChannel = ai_poChannel;
  //## end ClServerProxyManagerEmul::SetSessionChannel%949597710.body
}

inline ClChannel* ClServerProxyManagerEmul::GetSessionChannel () const
{
  //## begin ClServerProxyManagerEmul::GetSessionChannel%949662092.body preserve=yes
	return m_poSessionChannel;
  //## end ClServerProxyManagerEmul::GetSessionChannel%949662092.body
}

//## begin module%3507CDCE000A.epilog preserve=yes
//## end module%3507CDCE000A.epilog


#endif
