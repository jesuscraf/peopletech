//## begin module%37A5558F030A.cm preserve=no
//## end module%37A5558F030A.cm

//## begin module%37A5558F030A.cp preserve=no
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
//## end module%37A5558F030A.cp

//## Module: clssinterfaz%37A5558F030A; Package specification
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Source file: z:\m4subsystems\inc\clssinterfaz.hpp

#ifndef clssinterfaz_h
#define clssinterfaz_h 1

//## begin module%37A5558F030A.additionalIncludes preserve=no
//## end module%37A5558F030A.additionalIncludes

//## begin module%37A5558F030A.includes preserve=yes
#include <m4subsystems_dll.hpp>
//## end module%37A5558F030A.includes

// ClAdminTag
#include <cladmintag.hpp>
// NavigatorF
#include <navigatorf.hpp>
// ClConfiguration
#include <clconfiguration.hpp>
// Statistics
#include <statistics.hpp>
// clbaseobject
#include <clbaseobject.hpp>
// serverevent
#include <serverevent.hpp>
// DataStorages
#include <datastorages.hpp>

class M4_DECL_M4SUBSYSTEMS ClSSImplementation;

//## begin module%37A5558F030A.declarations preserve=no
//## end module%37A5558F030A.declarations

//## begin module%37A5558F030A.additionalDeclarations preserve=yes
class M4ISnmpObject;
//## end module%37A5558F030A.additionalDeclarations


//## begin ClSSInterfaz%37A5533500A2.preface preserve=yes
//## end ClSSInterfaz%37A5533500A2.preface

//## Class: ClSSInterfaz%37A5533500A2
//	Esta es la clase interfaz de todo subsistema. Los métodos de esta clase no
//	tienen semántica sino que redireccionan l allmada a la imlementación del
//	subsistema.
//## Category: M4Subsystems%37D39FC60059
//## Subsystem: M4Subsystems::inc%37D3A21301DC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37A5838D017A;ClAdminTagList { -> }
//## Uses: <unnamed>%39211EEA016D;ClSSImplementation { -> F}
//## Uses: <unnamed>%392123A401EF;M4DataStorage { -> }
//## Uses: <unnamed>%39212427002B;ClStatistic { -> }

class M4_DECL_M4SUBSYSTEMS ClSSInterfaz : public ClServerEvent, //## Inherits: <unnamed>%3921253C0033
                            	public ClSSInterfazFirewall  //## Inherits: <unnamed>%39239BA9010D
{
  //## begin ClSSInterfaz%37A5533500A2.initialDeclarations preserve=yes
  //## end ClSSInterfaz%37A5533500A2.initialDeclarations

  public:
    //## begin ClSSInterfaz::eStatus%3921159E037D.preface preserve=yes
    //## end ClSSInterfaz::eStatus%3921159E037D.preface

    //## Class: eStatus%3921159E037D
    //## Category: M4Subsystems%37D39FC60059
    //## Subsystem: M4Subsystems%37D39FD700DF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum {SS_INACTIVE = 0 , SS_ACTIVE , SS_PAUSED , SS_SHUTING_DOWN , SS_NOT_ACCESIBLE ,MAX_STATUS} eStatus;

    //## begin ClSSInterfaz::eStatus%3921159E037D.postscript preserve=yes
    //## end ClSSInterfaz::eStatus%3921159E037D.postscript

    //## Constructors (specified)
      //## Operation: ClSSInterfaz%933665016
      ClSSInterfaz (ClSSImplementation *ai_poImplementation);

    //## Destructor (specified)
      //## Operation: ~ClSSInterfaz%960286622
      ~ClSSInterfaz ();


    //## Other Operations (specified)
      //## Operation: ShutDown%933578409
      //	Hace el ShutDown del subsistema
      m4return_t ShutDown ();

      //## Operation: Synchronize%933578410
      //	Synchroniza los datos del subsistem, como por ejemplo la actualización de
      //	las estadísticas
      m4return_t Synchronize ();

      //## Operation: Initialize%933578411
      //	inicializa el subsistema creando los objetos estadñisticas y visitor
      m4return_t Initialize ();

      //## Operation: SetConfiguration%933578412
      //	Asigna el apuntador al objeto configuración del subsistema
      void SetConfiguration (ClConfiguration *ai_poConfiguration);

      //## Operation: GetElement%933578413
      void * GetElement (m4pchar_t ai_szElement = NULL);

      //## Operation: GetCommandsId%933578414
      m4return_t GetCommandsId (m4pchar_t ai_szPath, ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%933578415
      //	Devuelve los parámetros que pueda precisar el subsistema para ejecutar un
      //	comando concreto.
      m4return_t GetCommandParams (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &aio_oCommnadList);

      //## Operation: ExecuteCommand%933578416
      m4return_t ExecuteCommand (m4pchar_t ai_szPath, m4char_t ai_cCommand, ClAdminTagList &ai_oParamList, ClAdminTagList &ao_oResultList);

      //## Operation: GetId%933578417
      //	Returns the subsystem id
      m4objid_t GetId ();

      //## Operation: SetId%933578418
      //	Sets the subsystem id
      void SetId (m4objid_t ai_lId, m4pchar_t ai_szFatherPath);

      //## Operation: Activate%933578419
      void Activate ();

      //## Operation: FindSubsystemByType%933665004
      //	Returns the type of the subsystem
      ClSSInterfaz* FindSubsystemByType (m4uint32_t ai_iType);

      //## Operation: FindSubsystemById%933665005
      //	This method has the same functionality of the other FindSubsystem method but
      //	the diference is that in this one the argument is a single id so the
      //	functions compares the arguments to its own id and if it isn't propagates
      //	the calling to each of its sons or return error if there are no sons.
      ClSSInterfaz* FindSubsystemById (m4objid_t ai_lId	// The id of the subsystem to look for.
      );

      //## Operation: FindSubsystemByName%933665006
      //	This method has the same functionality of the other FindSubsystem method but
      //	the diference is that in this one the argument is a single id so the
      //	functions compares the arguments to its own id and if it isn't propagates
      //	the calling to each of its sons or return error if there are no sons.
      ClSSInterfaz* FindSubsystemByName (m4pchar_t ai_szName	// The id of the subsystem to look for.
      );

      //## Operation: Start%933665008
      m4return_t Start ();

      //## Operation: Stop%933665009
      m4return_t Stop (m4uint32_t ai_lDeferedTime = 0);

      //## Operation: IsReadyToClose%933665010
      m4bool_t IsReadyToClose ();

      //## Operation: Init%933665015
      virtual m4return_t Init (ClConfiguration *ai_poConfiguration);

      //## Operation: GetName%933760131
      //	Returns the subsystem name string
      m4pchar_t GetName ();

      //## Operation: SetName%933760132
      //	Sets the name string atribute
      void SetName (m4pchar_t ai_szName);

      //## Operation: GetSubsystemsId%936281637
      //	This method returns a list of Id of all the son nodes of this subsystem
      m4return_t GetSubsystemsId (m4pchar_t ai_szPath, vector <m4objid_t > &ao_poTags);

      //## Operation: FindSubsystem%936613835
      //	This method has the same functionality of the other FindSubsystem method but
      //	the diference is that in this one the argument is a single id so the
      //	functions compares the arguments to its own id and if it isn't propagates
      //	the calling to each of its sons or return error if there are no sons.
      ClSSInterfaz* FindSubsystem (m4objid_t ai_lId	// The id of the subsystem to look for.
      );

      //## Operation: GetSubsystem%936613836
      ClSSInterfaz* GetSubsystem ();

      //## Operation: AddSubsystem%936613837
      //	Add a subsystem to this node. The pointer to the subsystem to be added is
      //	passes as an argument.
      m4return_t AddSubsystem (ClSSInterfaz *ai_poSubsystem);

      //## Operation: RemoveSubsystem%936613838
      //	Removes a determinated subsystem from this subsystem
      m4return_t RemoveSubsystem (m4objid_t ai_lSubsystemId	// The id of the subsystem to be removed
      );

      //## Operation: GetType%936613839
      //	Returns one integer that is the type of the subsystem. There are predefined
      //	constants for each type.
      m4uint16_t GetType ();

      //## Operation: SetType%936613842
      //	Sets the subsystem type by an integer parameter.
      void SetType (m4uint16_t ai_iType);

      //## Operation: AttachInterfaceImplementation%936688276
      void AttachInterfaceImplementation (void *ai_poImlementation);

      //## Operation: GetStats%937209551
      ClStatistic * GetStats ();

      //## Operation: SetStats%937209552
      void SetStats (ClStatistic *ai_poStats);

      //## Operation: SetState%937811412
      m4return_t SetState (eStatus ai_iStatus, m4pchar_t ai_szState);

      //## Operation: Update%942826988
      void Update (EVENTTYPE ai_iEvent, ClServerEvent *ai_oSubject);

      //## Operation: AttachObserver%942850406
      void AttachObserver (ClServerEvent *ai_poObserver);

      //## Operation: GetState%954231002
      void GetState (eStatus &ao_eState);

      //## Operation: GetConfigNumericValue%954231000
      m4return_t GetConfigNumericValue (m4pchar_t ai_pszName, m4double_t &ao_rdNumber);

      //## Operation: GetConfigStringValue%954231001
      m4return_t GetConfigStringValue (m4pchar_t ai_pszName, m4pchar_t ao_pszString, m4size_t ai_iStringSize, m4size_t& ao_riStringLength);

      //## Operation: SetConfigNumericProperty%954231005
      m4return_t SetConfigNumericProperty (m4pchar_t ai_szProperty, m4double_t ai_iValue);

      //## Operation: SetConfigStringProperty%954231006
      m4return_t SetConfigStringProperty (m4pchar_t ai_szProperty, m4pchar_t ai_szValue);

      //## Operation: GetInstance%954231008; C++
      int GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput);

      //## Operation: SetElement%955100218
      m4return_t SetElement (m4pchar_t ai_szElement, void *ai_poElement);

      //## Operation: GetConfiguration%958463176
      //	Devuelve el puntero a la configuración
      virtual ClConfiguration * GetConfiguration ();

      //## Operation: GetStatisticAttribValue%994763177
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetStatisticAttribValue (m4pchar_t ai_szPath, m4uint32_t ai_iStatId, M4DataStorage *ao_poDataStorage);

      //## Operation: GetStatisticAttribsId%994763178
      //	This method returns a list of statistics of this subsystem, for each
      //	statistic the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetStatisticAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemAttribValue%994763179
      //	Calls to the same function of the member attribute derived from ClSSStats
      //	Container.
      virtual m4return_t GetSubsystemAttribValue (m4pchar_t ai_szPath, m4uint16_t ai_iAttribId, M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemAttribsId%994763180
      //	This method returns a list of subsystem attributes of this subsystem, for
      //	each atribute the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetSubsystemAttribsId (m4pchar_t ai_szPath, M4DataStorage *ao_poDataStorage);

      //## Operation: GetssId%994922160
      m4pchar_t GetssId ();

      //## Operation: GetPath%994922161
      m4pchar_t GetPath ();

      //## Operation: GetSnmpObject%1067856734
      M4ISnmpObject* GetSnmpObject ();

    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%37A54F3F0343
      //## Role: ClSSInterfaz::m_poImplementation%37A54F4002CC
      //## begin ClSSInterfaz::m_poImplementation%37A54F4002CC.role preserve=no  public: ClSSImplementation { -> RFHGAN}
      ClSSImplementation *m_poImplementation;
      //## end ClSSInterfaz::m_poImplementation%37A54F4002CC.role

  public:
    // Additional Public Declarations
      //## begin ClSSInterfaz%37A5533500A2.public preserve=yes
      //## end ClSSInterfaz%37A5533500A2.public

  protected:
    // Data Members for Associations

      //## Association: M4Subsystems::<unnamed>%37A55BFB01A6
      //## Role: ClSSInterfaz::m_lId%37A55BFC0139
      //## begin ClSSInterfaz::m_lId%37A55BFC0139.role preserve=no  protected: m4objid_t { -> VHAN}
      m4objid_t m_lId;
      //## end ClSSInterfaz::m_lId%37A55BFC0139.role

      //## Association: M4Subsystems::<unnamed>%3921266A0222
      //## Role: ClSSInterfaz::m_poConfiguration%3921266B016F
      //## begin ClSSInterfaz::m_poConfiguration%3921266B016F.role preserve=no  protected: ClConfiguration { -> RHAN}
      ClConfiguration *m_poConfiguration;
      //## end ClSSInterfaz::m_poConfiguration%3921266B016F.role

    // Additional Protected Declarations
      //## begin ClSSInterfaz%37A5533500A2.protected preserve=yes
      //## end ClSSInterfaz%37A5533500A2.protected

  private:
    // Additional Private Declarations
      //## begin ClSSInterfaz%37A5533500A2.private preserve=yes
      //## end ClSSInterfaz%37A5533500A2.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSSInterfaz%37A5533500A2.implementation preserve=yes
      //## end ClSSInterfaz%37A5533500A2.implementation

};

//## begin ClSSInterfaz%37A5533500A2.postscript preserve=yes
//## end ClSSInterfaz%37A5533500A2.postscript

// Class ClSSInterfaz 


//## Other Operations (inline)
inline ClConfiguration * ClSSInterfaz::GetConfiguration ()
{
  //## begin ClSSInterfaz::GetConfiguration%958463176.body preserve=yes
	return m_poConfiguration ;
  //## end ClSSInterfaz::GetConfiguration%958463176.body
}

//## begin module%37A5558F030A.epilog preserve=yes
//## end module%37A5558F030A.epilog


#endif
