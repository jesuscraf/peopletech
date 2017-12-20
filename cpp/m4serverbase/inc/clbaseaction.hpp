//## begin module%38F4422C0028.cm preserve=no
//## end module%38F4422C0028.cm

//## begin module%38F4422C0028.cp preserve=no
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
//## end module%38F4422C0028.cp

//## Module: clbaseaction%38F4422C0028; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\clbaseaction.hpp

#ifndef clbaseaction_h
#define clbaseaction_h 1

//## begin module%38F4422C0028.additionalIncludes preserve=no
//## end module%38F4422C0028.additionalIncludes

//## begin module%38F4422C0028.includes preserve=yes
#include <m4trace.hpp>
#include <m4types.hpp>
#include <garbageableinterface.hpp>
#include <m4serverbase_dll.hpp>

//## end module%38F4422C0028.includes

// ParamList
#include <paramlist.hpp>

class ClBaseActionExecutive;

//## begin module%38F4422C0028.declarations preserve=no
//## end module%38F4422C0028.declarations

//## begin module%38F4422C0028.additionalDeclarations preserve=yes
//## end module%38F4422C0028.additionalDeclarations


//## begin ClBaseAction%38E2441302AC.preface preserve=yes
//## end ClBaseAction%38E2441302AC.preface

//## Class: ClBaseAction%38E2441302AC
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERVERBASE ClBaseAction 
{
  //## begin ClBaseAction%38E2441302AC.initialDeclarations preserve=yes
  //## end ClBaseAction%38E2441302AC.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClBaseAction%955531864
      ClBaseAction (ClBaseActionExecutive *ai_poExecutive, m4bool_t ai_bDeleteParamList = M4_FALSE);

    //## Destructor (specified)
      //## Operation: ~ClBaseAction%956915209; C++
      ~ClBaseAction ();


    //## Other Operations (specified)
      //## Operation: Execute%954342263
      virtual m4return_t Execute ();

      //## Operation: SetParams%956825069; C++
      m4return_t SetParams (ClParamList *ai_poParamList);

      //## Operation: RemoveParams%957429934
      m4return_t RemoveParams ();

      //## Operation: GetParams%980856681
      ClParamList * GetParams ();

    // Additional Public Declarations
      //## begin ClBaseAction%38E2441302AC.public preserve=yes
      //## end ClBaseAction%38E2441302AC.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%38F4443601FF
      //## Role: ClBaseAction::m_poExecutive%38F4443701B0
      //## begin ClBaseAction::m_poExecutive%38F4443701B0.role preserve=no  protected: ClBaseActionExecutive { -> RFHAN}
      ClBaseActionExecutive *m_poExecutive;
      //## end ClBaseAction::m_poExecutive%38F4443701B0.role

      //## Association: M4ServerBase::<unnamed>%39080D430388
      //## Role: ClBaseAction::m_pParamList%39080D4401F9
      //## begin ClBaseAction::m_pParamList%39080D4401F9.role preserve=no  protected: ClParamList {1 -> 1RHAN}
      ClParamList *m_pParamList;
      //## end ClBaseAction::m_pParamList%39080D4401F9.role

    // Additional Protected Declarations
      //## begin ClBaseAction%38E2441302AC.protected preserve=yes
      //## end ClBaseAction%38E2441302AC.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bDeleteParamList%3A13FA810385
      //	If TRUE, then the destructor for this object will destroy de ParamList with
      //	a delete.
      //## begin ClBaseAction::m_bDeleteParamList%3A13FA810385.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bDeleteParamList;
      //## end ClBaseAction::m_bDeleteParamList%3A13FA810385.attr

    // Additional Private Declarations
      //## begin ClBaseAction%38E2441302AC.private preserve=yes
      //## end ClBaseAction%38E2441302AC.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBaseAction%38E2441302AC.implementation preserve=yes
      //## end ClBaseAction%38E2441302AC.implementation

};

//## begin ClBaseAction%38E2441302AC.postscript preserve=yes
//## end ClBaseAction%38E2441302AC.postscript

//## begin ClActionIdentifier%3918268E02BD.preface preserve=yes
//## end ClActionIdentifier%3918268E02BD.preface

//## Class: ClActionIdentifier%3918268E02BD
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

typedef m4uint32_t ClActionIdentifier;

//## begin ClActionIdentifier%3918268E02BD.postscript preserve=yes
//## end ClActionIdentifier%3918268E02BD.postscript

//## begin ClBaseActionExecutive%38F4406E00C7.preface preserve=yes
//## end ClBaseActionExecutive%38F4406E00C7.preface

//## Class: ClBaseActionExecutive%38F4406E00C7
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%38F4407D038F;ClBaseAction { -> }
//## Uses: <unnamed>%391947280146;ClActionIdentifier { -> }

class ClBaseActionExecutive 
{
  //## begin ClBaseActionExecutive%38F4406E00C7.initialDeclarations preserve=yes
  //## end ClBaseActionExecutive%38F4406E00C7.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ProcessAction%955531863
      virtual m4return_t ProcessAction (ClActionIdentifier ai_oActionIdentifier, ClParamList *ai_poParamList, ClBaseAction *&ao_poAction) = 0;

    // Additional Public Declarations
      //## begin ClBaseActionExecutive%38F4406E00C7.public preserve=yes
      //## end ClBaseActionExecutive%38F4406E00C7.public

  protected:
    // Additional Protected Declarations
      //## begin ClBaseActionExecutive%38F4406E00C7.protected preserve=yes
      //## end ClBaseActionExecutive%38F4406E00C7.protected

  private:
    // Additional Private Declarations
      //## begin ClBaseActionExecutive%38F4406E00C7.private preserve=yes
      //## end ClBaseActionExecutive%38F4406E00C7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClBaseActionExecutive%38F4406E00C7.implementation preserve=yes
      //## end ClBaseActionExecutive%38F4406E00C7.implementation

};

//## begin ClBaseActionExecutive%38F4406E00C7.postscript preserve=yes
//## end ClBaseActionExecutive%38F4406E00C7.postscript

// Class ClBaseAction 

inline ClBaseAction::ClBaseAction (ClBaseActionExecutive *ai_poExecutive, m4bool_t ai_bDeleteParamList)
  //## begin ClBaseAction::ClBaseAction%955531864.hasinit preserve=no
      : m_bDeleteParamList(M4_FALSE), m_pParamList(NULL)
  //## end ClBaseAction::ClBaseAction%955531864.hasinit
  //## begin ClBaseAction::ClBaseAction%955531864.initialization preserve=yes
  ,m_poExecutive ( ai_poExecutive )
  //## end ClBaseAction::ClBaseAction%955531864.initialization
{
  //## begin ClBaseAction::ClBaseAction%955531864.body preserve=yes
	m_bDeleteParamList = ai_bDeleteParamList ;
  //## end ClBaseAction::ClBaseAction%955531864.body
}



//## Other Operations (inline)
inline m4return_t ClBaseAction::Execute ()
{
  //## begin ClBaseAction::Execute%954342263.body preserve=yes
	return M4_ERROR ;
  //## end ClBaseAction::Execute%954342263.body
}

inline m4return_t ClBaseAction::RemoveParams ()
{
  //## begin ClBaseAction::RemoveParams%957429934.body preserve=yes
	m_pParamList=NULL;
	return M4_SUCCESS;
  //## end ClBaseAction::RemoveParams%957429934.body
}

inline ClParamList * ClBaseAction::GetParams ()
{
  //## begin ClBaseAction::GetParams%980856681.body preserve=yes
	return m_pParamList ;
  //## end ClBaseAction::GetParams%980856681.body
}

// Class ClBaseActionExecutive 

//## begin module%38F4422C0028.epilog preserve=yes
//## end module%38F4422C0028.epilog


#endif
