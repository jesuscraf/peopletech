//## begin module%392959000226.cm preserve=no
//## end module%392959000226.cm

//## begin module%392959000226.cp preserve=no
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
//## end module%392959000226.cp

//## Module: loadrule%392959000226; Package specification
//## Subsystem: version::inc%379ECBD70299
//## Source file: F:\integration\m4comsrv\inc\loadrule.hpp

#ifndef loadrule_h
#define loadrule_h 1

//## begin module%392959000226.additionalIncludes preserve=no
//## end module%392959000226.additionalIncludes
#include <paramlist.hpp>
//## begin module%392959000226.includes preserve=yes

//## end module%392959000226.includes

// clssinterfaz
#include <clssinterfaz.hpp>
//## begin module%392959000226.declarations preserve=no
//## end module%392959000226.declarations

//## begin module%392959000226.additionalDeclarations preserve=yes
//## end module%392959000226.additionalDeclarations


//## begin ParamNameList%393BCB9D02BE.preface preserve=yes
//## end ParamNameList%393BCB9D02BE.preface

//## Class: ParamNameList%393BCB9D02BE
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef list <string> ParamNameList;

//## begin ParamNameList%393BCB9D02BE.postscript preserve=yes
//## end ParamNameList%393BCB9D02BE.postscript

//## begin ClLoadRule%393BCB1400A4.preface preserve=yes
//## end ClLoadRule%393BCB1400A4.preface

//## Class: ClLoadRule%393BCB1400A4
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClLoadRule 
{
  //## begin ClLoadRule%393BCB1400A4.initialDeclarations preserve=yes
  //## end ClLoadRule%393BCB1400A4.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClLoadRule%960220697
      ClLoadRule (m4pcchar_t ai_szCommandName);


    //## Other Operations (specified)
      //## Operation: GetSubsystemLoad%960220698
      m4return_t GetSubsystemLoad (ClParamList &ao_oFinalParamList);

      //## Operation: Initialize%960220699
      m4return_t Initialize (ClNavigatorFirewall *ai_poNavigator, m4pchar_t ai_szSubsystemName);

      //## Operation: SetParamName%960220700
      m4return_t SetParamName (m4pcchar_t ai_szParamName);

    // Additional Public Declarations
      //## begin ClLoadRule%393BCB1400A4.public preserve=yes
      ~ClLoadRule ();
      //## end ClLoadRule%393BCB1400A4.public

  protected:
    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%393BCD1D011C
      //## Role: ClLoadRule::m_oParamNameList%393BCD1E0010
      //## begin ClLoadRule::m_oParamNameList%393BCD1E0010.role preserve=no  protected: ParamNameList { -> VHAN}
      ParamNameList m_oParamNameList;
      //## end ClLoadRule::m_oParamNameList%393BCD1E0010.role

    // Additional Protected Declarations
      //## begin ClLoadRule%393BCB1400A4.protected preserve=yes
      //## end ClLoadRule%393BCB1400A4.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iCommandId%393BCD400393
      //## begin ClLoadRule::m_iCommandId%393BCD400393.attr preserve=no  private: m4uint32_t {UA} 
      m4uint32_t m_iCommandId;
      //## end ClLoadRule::m_iCommandId%393BCD400393.attr

      //## Attribute: m_szCommandName%393BCD510243
      //## begin ClLoadRule::m_szCommandName%393BCD510243.attr preserve=no  private: m4pchar_t {UA} 
      m4pchar_t m_szCommandName;
      //## end ClLoadRule::m_szCommandName%393BCD510243.attr

    // Data Members for Associations

      //## Association: Subsystem::<unnamed>%393BCD860086
      //## Role: ClLoadRule::m_poSubsystem%393BCD860267
      //## begin ClLoadRule::m_poSubsystem%393BCD860267.role preserve=no  private: ClSSInterfaz { -> RHAN}
      ClSSInterfaz *m_poSubsystem;
      //## end ClLoadRule::m_poSubsystem%393BCD860267.role

    // Additional Private Declarations
      //## begin ClLoadRule%393BCB1400A4.private preserve=yes
      //## end ClLoadRule%393BCB1400A4.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClLoadRule%393BCB1400A4.implementation preserve=yes
      //## end ClLoadRule%393BCB1400A4.implementation

};

//## begin ClLoadRule%393BCB1400A4.postscript preserve=yes
//## end ClLoadRule%393BCB1400A4.postscript

//## begin ClLoadRulesTable%393BD3FE027C.preface preserve=yes
//## end ClLoadRulesTable%393BD3FE027C.preface

//## Class: ClLoadRulesTable%393BD3FE027C
//## Category: Subsystem%348FEFFE016C
//## Subsystem: version::inc%379ECBD70299
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%393BD45E01DA;ClLoadRule { -> }

typedef list < ClLoadRule *> ClLoadRulesTable;

//## begin ClLoadRulesTable%393BD3FE027C.postscript preserve=yes
//## end ClLoadRulesTable%393BD3FE027C.postscript

// Class ClLoadRule 

//## begin module%392959000226.epilog preserve=yes
//## end module%392959000226.epilog


#endif
