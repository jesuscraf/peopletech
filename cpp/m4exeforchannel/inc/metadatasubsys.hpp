//## begin module%35A24234036D.cm preserve=no
//## end module%35A24234036D.cm

//## begin module%35A24234036D.cp preserve=no
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
//## end module%35A24234036D.cp

//## Module: MetaData_SubSystem%35A24234036D; Package specification
//## Subsystem: M4ExeForChannel::Metadata::inc%3922767C0294
//## Source file: f:\v400\m4exeforchannel\inc\metadatasubsys.hpp

#ifndef metadata_subsystem_h
#define metadata_subsystem_h 1

//## begin module%35A24234036D.additionalIncludes preserve=no
//## end module%35A24234036D.additionalIncludes

//## begin module%35A24234036D.includes preserve=yes
#include "m4exeforchannel_dll.hpp"
#include <m4cppobl.hpp>
#include <m4srvres.hpp>
//## end module%35A24234036D.includes

// ClConfiguration
#include <clconfiguration.hpp>
// ExecutiveForOESS
#include <executiveforoess.hpp>
// executiveforoe
#include <executiveforoe.hpp>
// propertiestable
#include <propertiestable.hpp>
// clservicebasess
#include <clservicebasess.hpp>

class ClNavigatorFirewall;
class ClCache;
class ClExecutiveForMetaData;
class ClM4ObjRegServer;

//## begin module%35A24234036D.declarations preserve=no
//## end module%35A24234036D.declarations

//## begin module%35A24234036D.additionalDeclarations preserve=yes
class ClMDUpdateList;

//#define M4_MDSUBSYSTEM_CLEAN_CACHE					(0 + ClSubsystemBase::TOTAL_COMMANDS)
//#define M4_MDSUBSYSTEM_REFRESH_UPDATE_LIST			(1 + ClSubsystemBase::TOTAL_COMMANDS)
//#define M4_MDSUBSYSTEM_QUIT							(2 + ClSubsystemBase::TOTAL_COMMANDS)

#define METADATA_MAX_USERNAME						128
#define MAX_MD_INFO_ITEM							1024
#define M4_MDSUBSYSTEM_NAME							"METADATA_SUBSYSTEM"
//## end module%35A24234036D.additionalDeclarations


//## begin ClExecutiveForMetaDataSS%3912D84401EE.preface preserve=yes
//## end ClExecutiveForMetaDataSS%3912D84401EE.preface

//## Class: ClExecutiveForMetaDataSS%3912D84401EE
//## Category: M4ExeForChannel::Metadata%392274DC01D8
//## Subsystem: M4ExeForChannel::Metadata::inc%3922767C0294
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3912F13A0051;ClLauncherForOLTP { -> F}
//## Uses: <unnamed>%3922757D0157;ClExecutiveForMetaData { -> F}

class ClExecutiveForMetaDataSS : public ClExecutiveForOESS  //## Inherits: <unnamed>%3912D85B036D
{
  //## begin ClExecutiveForMetaDataSS%3912D84401EE.initialDeclarations preserve=yes
  //## end ClExecutiveForMetaDataSS%3912D84401EE.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForMetaDataSS%957518544
      ClExecutiveForMetaDataSS (ClConfiguration *ai_poConfiguratoin, ClTables *ai_poTables, ClNavigatorFirewall *ai_poNavigator, ClServiceBaseSS *ai_poServiceSS);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForMetaDataSS%957518545
      ~ClExecutiveForMetaDataSS ();


    //## Other Operations (specified)
      //## Operation: Initialize%957518546
      //	En este método cada subsistema debe inicializarse  a partir d su
      //	configuración y chequear al mismo tiempo que esta sea válida. Si es
      //	necesario además inicializará sus elementos con dicha configuración.
      virtual m4return_t Initialize ();

    // Additional Public Declarations
      //## begin ClExecutiveForMetaDataSS%3912D84401EE.public preserve=yes
      //## end ClExecutiveForMetaDataSS%3912D84401EE.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: s_szMetaDataInstanceOrder%3912EC2E0220
      //## begin ClExecutiveForMetaDataSS::s_szMetaDataInstanceOrder%3912EC2E0220.attr preserve=no  protected: static m4size_t {UA} 0
      static m4size_t s_szMetaDataInstanceOrder;
      //## end ClExecutiveForMetaDataSS::s_szMetaDataInstanceOrder%3912EC2E0220.attr

    // Additional Protected Declarations
      //## begin ClExecutiveForMetaDataSS%3912D84401EE.protected preserve=yes
      //## end ClExecutiveForMetaDataSS%3912D84401EE.protected

  private:
    // Additional Private Declarations
      //## begin ClExecutiveForMetaDataSS%3912D84401EE.private preserve=yes
      //## end ClExecutiveForMetaDataSS%3912D84401EE.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForMetaDataSS%3912D84401EE.implementation preserve=yes
      //## end ClExecutiveForMetaDataSS%3912D84401EE.implementation

};

//## begin ClExecutiveForMetaDataSS%3912D84401EE.postscript preserve=yes
//## end ClExecutiveForMetaDataSS%3912D84401EE.postscript

//## begin ClExecutiveForMDCaches%391296AE038F.preface preserve=yes
//## end ClExecutiveForMDCaches%391296AE038F.preface

//## Class: ClExecutiveForMDCaches%391296AE038F
//## Category: M4ExeForChannel::Metadata%392274DC01D8
//## Subsystem: M4ExeForChannel::Metadata::inc%3922767C0294
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39198BA70221;ClM4ObjService { -> F}
//## Uses: <unnamed>%39198BE70133;ClChannelManager { -> F}
//## Uses: <unnamed>%39227D65036A;ClNavigatorFirewall { -> F}

class M4_DECL_M4EXEFORCHANNEL ClExecutiveForMDCaches : public ClExecutiveForOE  //## Inherits: <unnamed>%3912971F016B
{
  //## begin ClExecutiveForMDCaches%391296AE038F.initialDeclarations preserve=yes
  //## end ClExecutiveForMDCaches%391296AE038F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClExecutiveForMDCaches%957518525
      ClExecutiveForMDCaches (ClEngine *ai_pEngine, ClStatistic *ai_poStats = NULL);

    //## Destructor (specified)
      //## Operation: ~ClExecutiveForMDCaches%957518526
      ~ClExecutiveForMDCaches ();


    //## Other Operations (specified)
      //## Operation: StartUp%957518527
      m4return_t StartUp (ClNavigatorFirewall *ai_pNavigator = NULL, ClConfiguration *ai_pConfig = NULL);

      //## Operation: ShutDown%957518528
      m4return_t ShutDown ();

      //## Operation: Reset%957518529; C++
      m4return_t Reset ();

      //## Operation: RefreshUpdateList%957518530
      m4return_t RefreshUpdateList (ClConfiguration *ai_pConfig = NULL);

      //## Operation: GetUpdateList%957518531
      ClMDUpdateList * GetUpdateList ();

      //## Operation: GetCMCRCache%957518532
      ClCache *  GetCMCRCache ();

      //## Operation: GetCSCRCache%957518533
      ClCache *  GetCSCRCache ();

      //## Operation: CleanCaches%957518534
      m4return_t CleanCaches ();

      //## Operation: GetPresentationCache%957518535
      ClCache *  GetPresentationCache ();

      ClCache *  GetMapCache ();

      //## Operation: Execute%957518536
      //	Remarks
      //	The context information provided is updated upon execution for the caller to
      //	return the status and some other meaningful information.
      m4return_t Execute (ClImage &aio_pImage);

    // Data Members for Class Attributes

      //## Attribute: M4ExecutiveForMDCMCRCache%3912EF4E002C
      //## begin ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache%3912EF4E002C.attr preserve=no  public: static m4pchar_t {UAC} "CMCR_CACHE"
      static const m4pchar_t  M4ExecutiveForMDCMCRCache;
      //## end ClExecutiveForMDCaches::M4ExecutiveForMDCMCRCache%3912EF4E002C.attr

      //## Attribute: M4ExecutiveForMDCSCRCache%3912EF4F031C
      //## begin ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache%3912EF4F031C.attr preserve=no  public: static m4pchar_t {UAC} "CSCR_CACHE"
      static const m4pchar_t  M4ExecutiveForMDCSCRCache;
      //## end ClExecutiveForMDCaches::M4ExecutiveForMDCSCRCache%3912EF4F031C.attr

      //## Attribute: M4ExecutiveForMDDataCache%3912EF51001C
      //## begin ClExecutiveForMDCaches::M4ExecutiveForMDDataCache%3912EF51001C.attr preserve=no  public: static m4pchar_t {UAC} "DATA_CACHE"
      static const m4pchar_t  M4ExecutiveForMDDataCache;
      //## end ClExecutiveForMDCaches::M4ExecutiveForMDDataCache%3912EF51001C.attr

      //## Attribute: M4ExecutiveForMDPresentationCache%3912EF7C015E
      //## begin ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache%3912EF7C015E.attr preserve=no  public: static m4pchar_t {UAC} "PRES_CACHE"
      static const m4pchar_t  M4ExecutiveForMDPresentationCache;
      //## end ClExecutiveForMDCaches::M4ExecutiveForMDPresentationCache%3912EF7C015E.attr

      static const m4pchar_t  M4ExecutiveForMDMapCache;

      //## Attribute: M4ExecutiveForMDUpdateList%392BC55201CF
      //## begin ClExecutiveForMDCaches::M4ExecutiveForMDUpdateList%392BC55201CF.attr preserve=no  public: static m4pchar_t {UAC} "UPDATE_LIST"
      static const m4pchar_t  M4ExecutiveForMDUpdateList;
      //## end ClExecutiveForMDCaches::M4ExecutiveForMDUpdateList%392BC55201CF.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::Metadata::<unnamed>%391297C5030E
      //## Role: ClExecutiveForMDCaches::m_pPresentationCache%391297C50319
      //## begin ClExecutiveForMDCaches::m_pPresentationCache%391297C50319.role preserve=no  public: ClCache { -> RFHAN}
      ClCache *m_pPresentationCache;
      //## end ClExecutiveForMDCaches::m_pPresentationCache%391297C50319.role

      ClCache *m_pMapCache;

    // Additional Public Declarations
      //## begin ClExecutiveForMDCaches%391296AE038F.public preserve=yes
      //## end ClExecutiveForMDCaches%391296AE038F.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: M4ExecutiveForMDCachesId%3912AE850218
      //## begin ClExecutiveForMDCaches::M4ExecutiveForMDCachesId%3912AE850218.attr preserve=no  protected: static m4pchar_t {VAC} "ExecutiveForMDCaches"
      static const m4pchar_t  M4ExecutiveForMDCachesId;
      //## end ClExecutiveForMDCaches::M4ExecutiveForMDCachesId%3912AE850218.attr

    // Data Members for Associations

      //## Association: M4ExeForChannel::Metadata::<unnamed>%38F71EEE0305
      //## Role: ClExecutiveForMDCaches::m_pRegistry%38F71EEF0267
      //## begin ClExecutiveForMDCaches::m_pRegistry%38F71EEF0267.role preserve=no  protected: ClM4ObjRegServer { -> 1RFHAN}
      ClM4ObjRegServer *m_pRegistry;
      //## end ClExecutiveForMDCaches::m_pRegistry%38F71EEF0267.role

    // Additional Protected Declarations
      //## begin ClExecutiveForMDCaches%391296AE038F.protected preserve=yes
	  ClMDUpdateList * m_pUpdateList;
      //## end ClExecutiveForMDCaches%391296AE038F.protected
  private:
    // Data Members for Associations

      //## Association: M4ExeForChannel::Metadata::<unnamed>%391297C80218
      //## Role: ClExecutiveForMDCaches::m_pCSCRCache%391297C80236
      //## begin ClExecutiveForMDCaches::m_pCSCRCache%391297C80236.role preserve=no  private: ClCache { -> RFHAN}
      ClCache *m_pCSCRCache;
      //## end ClExecutiveForMDCaches::m_pCSCRCache%391297C80236.role

      //## Association: M4ExeForChannel::Metadata::<unnamed>%391297CA01C1
      //## Role: ClExecutiveForMDCaches::m_pCMCRCache%391297CA01E9
      //## begin ClExecutiveForMDCaches::m_pCMCRCache%391297CA01E9.role preserve=no  private: ClCache { -> RFHAN}
      ClCache *m_pCMCRCache;
      //## end ClExecutiveForMDCaches::m_pCMCRCache%391297CA01E9.role

    // Additional Private Declarations
      //## begin ClExecutiveForMDCaches%391296AE038F.private preserve=yes
      //## end ClExecutiveForMDCaches%391296AE038F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClExecutiveForMDCaches%391296AE038F.implementation preserve=yes
      //## end ClExecutiveForMDCaches%391296AE038F.implementation

};

//## begin ClExecutiveForMDCaches%391296AE038F.postscript preserve=yes
//## end ClExecutiveForMDCaches%391296AE038F.postscript

//## begin ClMetaDataSubsystem%35A239CD0306.preface preserve=yes
//## end ClMetaDataSubsystem%35A239CD0306.preface

//## Class: ClMetaDataSubsystem%35A239CD0306
//## Category: M4ExeForChannel::Metadata%392274DC01D8
//## Subsystem: M4ExeForChannel::Metadata::inc%3922767C0294
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%35A23E0803DF;ClOblConfiguration { -> F}
//## Uses: <unnamed>%35A23E9B0200;ClTables { -> }
//## Uses: <unnamed>%367A7BB9027F;ClConfiguration { -> }
//## Uses: <unnamed>%38F72D1F031D;ClExecutiveForMetaData { -> F}
//## Uses: <unnamed>%39087802009C;ClConfiguration { -> F}
//## Uses: <unnamed>%3911B56A00AB; { -> F}
//## Uses: <unnamed>%3912ECD001F1;ClExecutiveForMetaDataSS { -> F}

class M4_DECL_M4EXEFORCHANNEL ClMetaDataSubsystem : public ClServiceBaseSS  //## Inherits: <unnamed>%3911942702C4
{
  //## begin ClMetaDataSubsystem%35A239CD0306.initialDeclarations preserve=yes
  //## end ClMetaDataSubsystem%35A239CD0306.initialDeclarations

  public:
    //## Class: eMetadataServiceCommands%3911B2830161
    //## Category: M4ExeForChannel::Metadata%392274DC01D8
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {CLEAN_CACHE =  ClServiceBaseSS::TOTAL_COMMANDS, REFRESH_UPDATE_LIST, TOTAL_COMMANDS} eMetadataServiceCommands;

    //## Constructors (specified)
      //## Operation: ClMetaDataSubsystem%899899318
      ClMetaDataSubsystem (ClConfiguration *  ai_poConfiguration, ClTables *  ai_poTables, ClNavigatorFirewall *  ai_pNavigator);

    //## Destructor (specified)
      //## Operation: ~ClMetaDataSubsystem%899899319
      ~ClMetaDataSubsystem ();


    //## Other Operations (specified)
      //## Operation: GetCommandsId%899899320
      m4return_t GetCommandsId (ClAdminTagList &  aio_oCommandList);

      //## Operation: ExecuteCommand%899899321
      m4return_t ExecuteCommand (m4char_t ai_cCommand, ClAdminTagList &  ai_oParamList, ClAdminTagList &  ao_oResultList);

      //## Operation: Initialize%899899323
      m4return_t Initialize ();

      //## Operation: ShutDown%899899324
      m4return_t ShutDown ();

      //## Operation: GetCommandParams%899899325
      m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &  aio_oParamList);

      //## Operation: GetElement%899899333
      void *  GetElement (m4pchar_t ai_szElement);

      //## Operation: GetCMCRCache%957518537
      ClCache *  GetCMCRCache ();

      //## Operation: GetCSCRCache%957518538
      ClCache *  GetCSCRCache ();

      //## Operation: GetPresentationCache%957518539
      ClCache *  GetPresentationCache ();

      ClCache *  GetMapCache ();

      //## Operation: RefreshUpdateList%957518540
      m4return_t RefreshUpdateList ();

      //## Operation: CleanCaches%957518541
      m4return_t CleanCaches ();

      //## Operation: GetUpdateList%957518542
      ClMDUpdateList * GetUpdateList ();

      //## Operation: GenerateSubsystem%957518543
      //	En este método se  construye el subsistema que va a ser hijo de este, el
      //	cual debe saber construir.
      //	Se trata deuna factoría de subsistemas,pero de ámbito particular de cada
      //	subsistema. Es decir es una factoría  de sus propios hijos.
      //	El retorno de esta función será un interfaz del subsitema creado en caso ser
      //	exitoso el resultado o un nulo en caso contrario.
      //	Los parámetros de entrada son el tipo del subsistema a crear, la
      //	configuración con la que se inicializará dicho subsistema y la referencia a
      //	objeto tables genérico.
      virtual ClSSInterfaz * GenerateSubsystem (m4uint32_t ai_lType, ClConfiguration *ai_poConfiguration, ClTables *ai_poTables);

    // Data Members for Class Attributes

      //## Attribute: M4MetaDataSubsystemName%3911A7D2018D
      //## begin ClMetaDataSubsystem::M4MetaDataSubsystemName%3911A7D2018D.attr preserve=no  public: static m4pchar_t {VAC} "METADATA_SERVICE"
      static const m4pchar_t  M4MetaDataSubsystemName;
      //## end ClMetaDataSubsystem::M4MetaDataSubsystemName%3911A7D2018D.attr

  public:
    // Additional Public Declarations
      //## begin ClMetaDataSubsystem%35A239CD0306.public preserve=yes
      //## end ClMetaDataSubsystem%35A239CD0306.public

  protected:
    // Data Members for Associations

      //## Association: M4ExeForChannel::Metadata::<unnamed>%3912AD6500C0
      //## Role: ClMetaDataSubsystem::m_oExecutiveForMDCaches%3912AD650282
      //## begin ClMetaDataSubsystem::m_oExecutiveForMDCaches%3912AD650282.role preserve=no  protected: ClExecutiveForMDCaches { -> 1VHAN}
      ClExecutiveForMDCaches m_oExecutiveForMDCaches;
      //## end ClMetaDataSubsystem::m_oExecutiveForMDCaches%3912AD650282.role

      //## Association: M4ExeForChannel::Metadata::<unnamed>%3912A6E500E5
      //## Role: ClMetaDataSubsystem::m_pConfigurationForStartup%3912A6E503C0
      //## begin ClMetaDataSubsystem::m_pConfigurationForStartup%3912A6E503C0.role preserve=no  protected: ClConfiguration { -> 1RFHAN}
      ClConfiguration *m_pConfigurationForStartup;
      //## end ClMetaDataSubsystem::m_pConfigurationForStartup%3912A6E503C0.role

      //## Association: M4ExeForChannel::Metadata::<unnamed>%391299BE0043
      //## Role: ClMetaDataSubsystem::m_pNavigatorForStartup%391299BE0238
      //## begin ClMetaDataSubsystem::m_pNavigatorForStartup%391299BE0238.role preserve=no  protected: ClNavigatorFirewall { -> 1RFHAN}
      ClNavigatorFirewall *m_pNavigatorForStartup;
      //## end ClMetaDataSubsystem::m_pNavigatorForStartup%391299BE0238.role

    // Additional Protected Declarations
      //## begin ClMetaDataSubsystem%35A239CD0306.protected preserve=yes
      //## end ClMetaDataSubsystem%35A239CD0306.protected

  private:
    // Additional Private Declarations
      //## begin ClMetaDataSubsystem%35A239CD0306.private preserve=yes
      //## end ClMetaDataSubsystem%35A239CD0306.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClMetaDataSubsystem%35A239CD0306.implementation preserve=yes
      //## end ClMetaDataSubsystem%35A239CD0306.implementation

};

//## begin ClMetaDataSubsystem%35A239CD0306.postscript preserve=yes
//## end ClMetaDataSubsystem%35A239CD0306.postscript

// Class ClExecutiveForMetaDataSS 

// Class ClExecutiveForMDCaches 


//## Other Operations (inline)
inline m4return_t ClExecutiveForMDCaches::Execute (ClImage &aio_pImage)
{
  //## begin ClExecutiveForMDCaches::Execute%957518536.body preserve=yes
	return M4_ERROR;
  //## end ClExecutiveForMDCaches::Execute%957518536.body
}

// Class ClMetaDataSubsystem 

//## begin module%35A24234036D.epilog preserve=yes
//## end module%35A24234036D.epilog


#endif
