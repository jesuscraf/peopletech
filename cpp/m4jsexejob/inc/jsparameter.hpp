//## begin module%37E602F9039B.cm preserve=no
//## end module%37E602F9039B.cm

//## begin module%37E602F9039B.cp preserve=no
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
//## end module%37E602F9039B.cp

//## Module: jsparameter%37E602F9039B; Package specification
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Source file: C:\Trabajo\Server\m4jsexejob\inc\jsparameter.hpp

#ifndef jsparameter_h
#define jsparameter_h 1

//## begin module%37E602F9039B.additionalIncludes preserve=no
//## end module%37E602F9039B.additionalIncludes

//## begin module%37E602F9039B.includes preserve=yes
#include "m4stl.hpp"
#include <m4jsexejob_dll.hpp>
//## end module%37E602F9039B.includes

// m4string
#include <m4string.hpp>

class ClJSParameterConnector;

//## begin module%37E602F9039B.declarations preserve=no
//## end module%37E602F9039B.declarations

//## begin module%37E602F9039B.additionalDeclarations preserve=yes
//## end module%37E602F9039B.additionalDeclarations


//## begin ConnectorList%379F344900FB.preface preserve=yes
//## end ConnectorList%379F344900FB.preface

//## Class: ConnectorList%379F344900FB; Instantiated Class
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%379F345800D5;ClJSParameterConnector { -> F}

typedef list< ClJSParameterConnector * > ConnectorList;

//## begin ConnectorList%379F344900FB.postscript preserve=yes
//## end ConnectorList%379F344900FB.postscript

//## begin ClJSParameter%375BE7E0031E.preface preserve=yes
//## end ClJSParameter%375BE7E0031E.preface

//## Class: ClJSParameter%375BE7E0031E
//## Category: M4JSExeJob::M4JSJob%379F03BD0354
//## Subsystem: M4JSExeJob::m4jsjob::inc%37E244DF010A
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4JSEXEJOB ClJSParameter 
{
  //## begin ClJSParameter%375BE7E0031E.initialDeclarations preserve=yes
  //## end ClJSParameter%375BE7E0031E.initialDeclarations

  public:
    //## begin ClJSParameter::eParameterType%37F4B616035E.preface preserve=yes
    //## end ClJSParameter::eParameterType%37F4B616035E.preface

    //## Class: eParameterType%37F4B616035E
    //## Category: M4JSExeJob::M4JSJob%379F03BD0354
    //## Subsystem: M4JSExeJob::m4jsjob%37E244C503BF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { TypeNumber, TypeString, TypeDate } eParameterType;

    //## begin ClJSParameter::eParameterType%37F4B616035E.postscript preserve=yes
    //## end ClJSParameter::eParameterType%37F4B616035E.postscript

    //## begin ClJSParameter::eParameterFlowType%37F4B62401EB.preface preserve=yes
    //## end ClJSParameter::eParameterFlowType%37F4B62401EB.preface

    //## Class: eParameterFlowType%37F4B62401EB
    //## Category: M4JSExeJob::M4JSJob%379F03BD0354
    //## Subsystem: M4JSExeJob::m4jsjob%37E244C503BF
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef enum { FlowInput, FlowOutput } eParameterFlowType;

    //## begin ClJSParameter::eParameterFlowType%37F4B62401EB.postscript preserve=yes
    //## end ClJSParameter::eParameterFlowType%37F4B62401EB.postscript

    //## Constructors (specified)
      //## Operation: ClJSParameter%938766923
      ClJSParameter (M4ClString &ai_stName, M4ClString &ai_stLocalName, eParameterType ai_eType, eParameterFlowType ai_eFlowType);

    //## Destructor (specified)
      //## Operation: ~ClJSParameter%939632584
      virtual ~ClJSParameter ();


    //## Other Operations (specified)
      //## Operation: AddConnector%933158864
      m4return_t AddConnector (ClJSParameterConnector *ai_poConnector);

      //## Operation: PropagateValue%939135439
      m4return_t PropagateValue ();

      //## Operation: SetValue%933158863
      m4return_t SetValue (const M4ClString &ai_stValue);

      //## Operation: GetValue%933158865
      m4return_t GetValue (M4ClString &ao_stValue);

      //## Operation: SetName%939045075
      m4return_t SetName (const M4ClString &ai_stName);

      //## Operation: GetName%939045076
      m4return_t GetName (M4ClString &ao_stName);

      //## Operation: GetLocalName%939045079
      m4return_t GetLocalName (M4ClString &ao_stLocalName);

      //## Operation: GetType%939045080
      m4return_t GetType (eParameterType &ao_eType);

      //## Operation: GetFlowType%939045081
      m4return_t GetFlowType (eParameterFlowType &ao_eFlowType);

      //## Operation: GetTypeAsString%941737615
      m4return_t GetTypeAsString (M4ClString &ao_stParamType);

      //## Operation: operator <<%939045078
      friend ostream & operator << (ostream &aio_ostream, ClJSParameter &ai_oParameter);

      //## Operation: IsValueSaved%941561493
      m4bool_t IsValueSaved ();

      //## Operation: SetValueSaved%941619583
      m4return_t SetValueSaved (m4bool_t ai_bIsValueSaved);

  public:
    // Additional Public Declarations
      //## begin ClJSParameter%375BE7E0031E.public preserve=yes
      //## end ClJSParameter%375BE7E0031E.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_eType%37F4B6790248
      //## begin ClJSParameter::m_eType%37F4B6790248.attr preserve=no  protected: eParameterType {UA} 
      eParameterType m_eType;
      //## end ClJSParameter::m_eType%37F4B6790248.attr

      //## Attribute: m_eFlowType%37F4B69D0037
      //## begin ClJSParameter::m_eFlowType%37F4B69D0037.attr preserve=no  protected: eParameterFlowType {UA} 
      eParameterFlowType m_eFlowType;
      //## end ClJSParameter::m_eFlowType%37F4B69D0037.attr

      //## Attribute: m_bIsValueSaved%381F23B30226
      //## begin ClJSParameter::m_bIsValueSaved%381F23B30226.attr preserve=no  protected: m4bool_t {UA} 
      m4bool_t m_bIsValueSaved;
      //## end ClJSParameter::m_bIsValueSaved%381F23B30226.attr

    // Data Members for Associations

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%379F34DD01EF
      //## Role: ClJSParameter::m_lConnectors%379F34DE004C
      //## begin ClJSParameter::m_lConnectors%379F34DE004C.role preserve=no  protected: ConnectorList { -> 1VHAN}
      ConnectorList m_lConnectors;
      //## end ClJSParameter::m_lConnectors%379F34DE004C.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E7700C0079
      //## Role: ClJSParameter::m_stName%37E7700C03AE
      //## begin ClJSParameter::m_stName%37E7700C03AE.role preserve=no  protected: M4ClString { -> 1VHAN}
      M4ClString m_stName;
      //## end ClJSParameter::m_stName%37E7700C03AE.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37E7702E0168
      //## Role: ClJSParameter::m_stValue%37E7702E0385
      //## begin ClJSParameter::m_stValue%37E7702E0385.role preserve=no  protected: M4ClString { -> 1VHAN}
      M4ClString m_stValue;
      //## end ClJSParameter::m_stValue%37E7702E0385.role

      //## Association: M4JSExeJob::M4JSJob::<unnamed>%37F4B53D009F
      //## Role: ClJSParameter::m_stLocalName%37F4B53D03CA
      //## begin ClJSParameter::m_stLocalName%37F4B53D03CA.role preserve=no  protected: M4ClString { -> 1VHAN}
      M4ClString m_stLocalName;
      //## end ClJSParameter::m_stLocalName%37F4B53D03CA.role

    // Additional Protected Declarations
      //## begin ClJSParameter%375BE7E0031E.protected preserve=yes
      //## end ClJSParameter%375BE7E0031E.protected

  private:
    // Additional Private Declarations
      //## begin ClJSParameter%375BE7E0031E.private preserve=yes
      //## end ClJSParameter%375BE7E0031E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClJSParameter%375BE7E0031E.implementation preserve=yes
      //## end ClJSParameter%375BE7E0031E.implementation

};

//## begin ClJSParameter%375BE7E0031E.postscript preserve=yes
//## end ClJSParameter%375BE7E0031E.postscript

// Class ClJSParameter 

//## begin module%37E602F9039B.epilog preserve=yes
//## end module%37E602F9039B.epilog


#endif
