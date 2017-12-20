//## begin module%349295B1030F.cm preserve=no
//## end module%349295B1030F.cm

//## begin module%349295B1030F.cp preserve=no
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
//## end module%349295B1030F.cp

//## Module: Introspective%349295B1030F; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\introspective.hpp

#ifndef introspective_h
#define introspective_h 1

//## begin module%349295B1030F.additionalIncludes preserve=no
//## end module%349295B1030F.additionalIncludes

//## begin module%349295B1030F.includes preserve=yes
//## end module%349295B1030F.includes

#include "m4stl.hpp"
// clssbase
#include "clssbase.hpp"

class ClSubsystem;
class ClLoadTables;

//## begin module%349295B1030F.declarations preserve=no
//## end module%349295B1030F.declarations

//## begin module%349295B1030F.additionalDeclarations preserve=yes
#define PROPERTIES_ATTRIBUTES_PATH	"ATTRIBUTES.DESCRIPTORS"
#define ATTRIBUTE					"ATTRIBUTE"
#define ATTRIBUTES					"ATTRIBUTES"
#define PROPERTIES_STATISTICS_PATH	"STATISTICS.DESCRIPTORS"
#define STATISTIC					"STATISTIC"
#define STATISTICS					"STATISTICS"
#define PROPERTIES_COMMANDS_PATH	"COMMANDS.DESCRIPTORS"
#define COMMANDS					"COMMANDS"
//## end module%349295B1030F.additionalDeclarations


//## begin SYSTEMSMAP%34B6603902EE.preface preserve=yes
//## end SYSTEMSMAP%34B6603902EE.preface

//## Class: SYSTEMSMAP%34B6603902EE
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34BB8282032C;ClSubsystem { -> F}
//## Uses: <unnamed>%34BCEF8800D4;map { -> }

typedef map <m4uint32_t , ClSubsystemBase * , less < m4uint32_t > > SYSTEMSMAP;

//## begin SYSTEMSMAP%34B6603902EE.postscript preserve=yes
//## end SYSTEMSMAP%34B6603902EE.postscript

//## begin ClIntrospective%348FFB60009A.preface preserve=yes
//## end ClIntrospective%348FFB60009A.preface

//## Class: ClIntrospective%348FFB60009A
//	This class is used to request the subsystems for their type, configuration,
//	attributes and statistics but only about witch are they and not about their
//	values.
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%351B98EC00C4;ClLoadTables { -> F}

class ClIntrospective : public ClSubsystemBase  //## Inherits: <unnamed>%34D6FA47025E
{
  //## begin ClIntrospective%348FFB60009A.initialDeclarations preserve=yes
  //## end ClIntrospective%348FFB60009A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClIntrospective%942231832
      ClIntrospective (ClConfiguration *ai_poConfiguration, 	// Objeto configuración que sirve para inicializar tanto el subsistema como el
      	// componente
      ClTables *ai_poTables	// Puntero a la tabla de constantes y literales que usan los subsistemas
      );

    //## Destructor (specified)
      //## Operation: ~ClIntrospective%882023704
      ~ClIntrospective ();


    //## Other Operations (specified)
      //## Operation: GetStatisticAttribsId%884689679
      //	This method returns a list of statistics of this subsystem, for each
      //	statistic the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetStatisticAttribsId (M4DataStorage *ao_poDataStorage);

      //## Operation: GetSubsystemAttribsId%885989063
      //	This method returns a list of subsystem attributes of this subsystem, for
      //	each atribute the metod returns its id, type, literal and importance scale.
      virtual m4return_t GetSubsystemAttribsId (M4DataStorage *ao_poDataStorage);

      //## Operation: SetName%885989066
      //	Sets the name string atribute
      virtual void SetName (m4pchar_t ai_szName);

      //## Operation: GetName%885989067
      //	Returns the subsystem name string
      virtual m4pchar_t GetName ();

      //## Operation: GetSubsystem%886518778
      virtual ClSubsystemBase * GetSubsystem ();

      //## Operation: GetType%886945699
      //	Returns one integer that is the type of the subsystem. There are predefined
      //	constants for each type.
      virtual m4uint16_t GetType ();

      //## Operation: SetType%886945700
      //	Sets the subsystem type by an integer parameter.
      virtual void SetType (m4uint16_t ai_iType);

      //## Operation: SetType%886945701
      //	Sets
      virtual void SetType (m4pchar_t ai_szType);

      //## Operation: GetCommandsId%900930588
      virtual m4return_t GetCommandsId (ClAdminTagList &aio_oCommandList);

      //## Operation: GetCommandParams%901005785
      virtual m4return_t GetCommandParams (m4char_t ai_cCommand, ClAdminTagList &aio_oParamList);

      //## Operation: GetSubsystemsId%920285556
      //	This method returns a list of Id of all the son nodes of this subsystem
      virtual m4return_t GetSubsystemsId (vector <m4uint32_t > &ao_poTags);

      //## Operation: AttachInterfaceImplementation%936688277
      void AttachInterfaceImplementation (void *ai_poImlementation);

      //## Operation: FindSubsystemByType%936784756
      //	Returns the type of the subsystem
      ClSubsystemBase * FindSubsystemByType (m4uint32_t ai_iType);

      //## Operation: GetStats%937209555
      ClStatistic * GetStats ();

      //## Operation: SetStats%937209556
      void SetStats (ClStatistic *ai_poStats);

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%3492BE130205
      //## Role: ClIntrospective::m_oSSList%3492BE14030B
      //## begin ClIntrospective::m_oSSList%3492BE14030B.role preserve=no  public: SYSTEMSMAP { -> VHN}
      SYSTEMSMAP m_oSSList;
      //## end ClIntrospective::m_oSSList%3492BE14030B.role

      //## Association: Subsystem::<unnamed>%34D6FA250363
      //## Role: ClIntrospective::m_poSubsystem%34D6FA270320
      //## begin ClIntrospective::m_poSubsystem%34D6FA270320.role preserve=no  public: ClSSStatsContainer { -> RHAN}
      ClSSStatsContainer *m_poSubsystem;
      //## end ClIntrospective::m_poSubsystem%34D6FA270320.role

    // Additional Public Declarations
      //## begin ClIntrospective%348FFB60009A.public preserve=yes
      //## end ClIntrospective%348FFB60009A.public

  protected:
    // Additional Protected Declarations
      //## begin ClIntrospective%348FFB60009A.protected preserve=yes
      //## end ClIntrospective%348FFB60009A.protected

  private:
    //## Constructors (specified)
      //## Operation: ClIntrospective%882023703
      ClIntrospective ();

    // Data Members for Class Attributes

      //## Attribute: m_szName%34CF607203B5
      //## begin ClIntrospective::m_szName%34CF607203B5.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szName;
      //## end ClIntrospective::m_szName%34CF607203B5.attr

      //## Attribute: m_iType%34DDB70D0288
      //## begin ClIntrospective::m_iType%34DDB70D0288.attr preserve=no  private: m4uint16_t {UA} 
      m4uint16_t m_iType;
      //## end ClIntrospective::m_iType%34DDB70D0288.attr

    // Additional Private Declarations
      //## begin ClIntrospective%348FFB60009A.private preserve=yes
      //## end ClIntrospective%348FFB60009A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClIntrospective%348FFB60009A.implementation preserve=yes
      //## end ClIntrospective%348FFB60009A.implementation

};

//## begin ClIntrospective%348FFB60009A.postscript preserve=yes
//## end ClIntrospective%348FFB60009A.postscript

// Class ClIntrospective 


//## Other Operations (inline)
inline ClStatistic * ClIntrospective::GetStats ()
{
  //## begin ClIntrospective::GetStats%937209555.body preserve=yes
	return m_poSubsystem -> GetStats () ;
  //## end ClIntrospective::GetStats%937209555.body
}

inline void ClIntrospective::SetStats (ClStatistic *ai_poStats)
{
  //## begin ClIntrospective::SetStats%937209556.body preserve=yes
  //## end ClIntrospective::SetStats%937209556.body
}

//## begin module%349295B1030F.epilog preserve=yes
//## end module%349295B1030F.epilog


#endif
