//## begin module%3919152202E7.cm preserve=no
//## end module%3919152202E7.cm

//## begin module%3919152202E7.cp preserve=no
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
//## end module%3919152202E7.cp

//## Module: actioninvoker%3919152202E7; Package specification
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Source file: F:\integration\m4serverbase\inc\actioninvoker.hpp

#ifndef actioninvoker_h
#define actioninvoker_h 1

//## begin module%3919152202E7.additionalIncludes preserve=no
//## end module%3919152202E7.additionalIncludes

//## begin module%3919152202E7.includes preserve=yes
#include <m4serverbase_dll.hpp>
//## end module%3919152202E7.includes

// iexecutable
#include <iexecutable.hpp>
// clbaseaction
#include <clbaseaction.hpp>
// ParamList
#include <paramlist.hpp>
// garbagecontainer
#include <garbagecontainer.hpp>
//## begin module%3919152202E7.declarations preserve=no
//## end module%3919152202E7.declarations

//## begin module%3919152202E7.additionalDeclarations preserve=yes
//## end module%3919152202E7.additionalDeclarations


//## begin ActionInformation%3918254F0368.preface preserve=yes
//## end ActionInformation%3918254F0368.preface

//## Class: ActionInformation%3918254F0368; Parameterized Class
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class _T>
class ActionInformation 
{
  //## begin ActionInformation%3918254F0368.initialDeclarations preserve=yes
  //## end ActionInformation%3918254F0368.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ActionInformation%957885103
      explicit ActionInformation (ClActionIdentifier ai_oActionIdentifier, _T ai_poExecutive);


    //## Other Operations (specified)
      //## Operation: GetExecutive%957885104
      _T GetExecutive ();

      //## Operation: GetIdentifier%957885105
      ClActionIdentifier GetIdentifier ();

    // Additional Public Declarations
      //## begin ActionInformation%3918254F0368.public preserve=yes
      //## end ActionInformation%3918254F0368.public

  protected:
    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%391826B90174
      //## Role: ActionInformation::m_oActionIdentifier%391826BA0036
      //## begin ActionInformation::m_oActionIdentifier%391826BA0036.role preserve=no  protected: ClActionIdentifier { -> VHAN}
      ClActionIdentifier m_oActionIdentifier;
      //## end ActionInformation::m_oActionIdentifier%391826BA0036.role

    // Additional Protected Declarations
      //## begin ActionInformation%3918254F0368.protected preserve=yes
      //## end ActionInformation%3918254F0368.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_poExecutive%39182947020B
      //## begin ActionInformation::m_poExecutive%39182947020B.attr preserve=no  private: _T {UA} 
      _T m_poExecutive;
      //## end ActionInformation::m_poExecutive%39182947020B.attr

    // Additional Private Declarations
      //## begin ActionInformation%3918254F0368.private preserve=yes
      //## end ActionInformation%3918254F0368.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ActionInformation%3918254F0368.implementation preserve=yes
      //## end ActionInformation%3918254F0368.implementation

};

//## begin ActionInformation%3918254F0368.postscript preserve=yes
//## end ActionInformation%3918254F0368.postscript

//## begin ExecutiveSSMap%38F211C000A6.preface preserve=yes
//## end ExecutiveSSMap%38F211C000A6.preface

//## Class: ExecutiveSSMap%38F211C000A6; Parameterized Class
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Persistent
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39182FFC00A1;ActionInformation { -> }

template <class _T>
class ExecutiveSSMap 
{
  //## begin ExecutiveSSMap%38F211C000A6.initialDeclarations preserve=yes
  //## end ExecutiveSSMap%38F211C000A6.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: Insert%955384513
      void Insert (m4pchar_t ai_szName, ActionInformation <_T> *ai_oActionInformation);

      //## Operation: GetActionInformation%955384514
      m4return_t GetActionInformation (m4pchar_t ai_szName, ClActionIdentifier &ao_oActionIdentifier, _T &ao_poExecutive);

    // Additional Public Declarations
      //## begin ExecutiveSSMap%38F211C000A6.public preserve=yes
      //## end ExecutiveSSMap%38F211C000A6.public

  protected:
    // Additional Protected Declarations
      //## begin ExecutiveSSMap%38F211C000A6.protected preserve=yes
      //## end ExecutiveSSMap%38F211C000A6.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_oExemap%391832E702D6
      //## begin ExecutiveSSMap::m_oExemap%391832E702D6.attr preserve=no  private: map <string , ActionInformation < _T> *, less <string> > {UA} 
      map <string , ActionInformation < _T> *, less <string> > m_oExemap;
      //## end ExecutiveSSMap::m_oExemap%391832E702D6.attr

    // Additional Private Declarations
      //## begin ExecutiveSSMap%38F211C000A6.private preserve=yes
      //## end ExecutiveSSMap%38F211C000A6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ExecutiveSSMap%38F211C000A6.implementation preserve=yes
      //## end ExecutiveSSMap%38F211C000A6.implementation

};

//## begin ExecutiveSSMap%38F211C000A6.postscript preserve=yes
//## end ExecutiveSSMap%38F211C000A6.postscript

//## begin ClActionInvoker%391829C90009.preface preserve=yes
//## end ClActionInvoker%391829C90009.preface

//## Class: ClActionInvoker%391829C90009; Parameterized Class
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%39182A5702DE;ClActionIdentifier { -> }
//## Uses: <unnamed>%3919268A021C;ClParamList { -> }

template <class _T>
class ClActionInvoker : public ClExecutableInterface  //## Inherits: <unnamed>%391ABB4F0008
{
  //## begin ClActionInvoker%391829C90009.initialDeclarations preserve=yes
  //## end ClActionInvoker%391829C90009.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: ExecuteAction%957885108
      m4return_t ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList);

      //## Operation: RegisterAction%957885109
      m4return_t RegisterAction (m4pchar_t ai_szAction, ClActionIdentifier ai_oIdentifier, _T ai_poExecutive);

      //## Operation: FinishRegistration%957885110
      m4return_t FinishRegistration ();

    // Additional Public Declarations
      //## begin ClActionInvoker%391829C90009.public preserve=yes
      //## end ClActionInvoker%391829C90009.public

  protected:
    //## Constructors (specified)
      //## Operation: ClActionInvoker%957885106
      explicit ClActionInvoker ();

    // Additional Protected Declarations
      //## begin ClActionInvoker%391829C90009.protected preserve=yes
      //## end ClActionInvoker%391829C90009.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: ai_bEnableRegistration%39183654024B
      //## begin ClActionInvoker::ai_bEnableRegistration%39183654024B.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t ai_bEnableRegistration;
      //## end ClActionInvoker::ai_bEnableRegistration%39183654024B.attr

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%38F212ED02E4
      //## Role: ClActionInvoker::m_oExecutivesMap%38F212EE0188
      //## begin ClActionInvoker::m_oExecutivesMap%38F212EE0188.role preserve=no  private: ExecutiveSSMap { -> VHAN}
      ExecutiveSSMap <_T> m_oExecutivesMap;
      //## end ClActionInvoker::m_oExecutivesMap%38F212EE0188.role

    // Additional Private Declarations
      //## begin ClActionInvoker%391829C90009.private preserve=yes
      //## end ClActionInvoker%391829C90009.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClActionInvoker%391829C90009.implementation preserve=yes
      //## end ClActionInvoker%391829C90009.implementation

};

//## begin ClActionInvoker%391829C90009.postscript preserve=yes
//## end ClActionInvoker%391829C90009.postscript

//## begin ClInvokerCacheParams%39B797F800C6.preface preserve=yes
//## end ClInvokerCacheParams%39B797F800C6.preface

//## Class: ClInvokerCacheParams%39B797F800C6; Parameterized Class
//## Category: M4ServerBase%35EEAFAA0123
//## Subsystem: M4ServerBase::inc%378B07C10202
//## Persistence: Transient
//## Cardinality/Multiplicity: n

template <class _T>
class ClInvokerCacheParams : public ClActionInvoker<_T>  //## Inherits: <unnamed>%39B7985D0003
{
  //## begin ClInvokerCacheParams%39B797F800C6.initialDeclarations preserve=yes
  //## end ClInvokerCacheParams%39B797F800C6.initialDeclarations

  public:

    //## Other Operations (specified)
      //## Operation: GetParamList%968661317
      m4return_t GetParamList (ClParamList *&ao_poParamList);

      //## Operation: SetParamList%968661316
      m4return_t SetParamList (ClParamList &ai_oParamList);

    // Additional Public Declarations
      //## begin ClInvokerCacheParams%39B797F800C6.public preserve=yes
      //## end ClInvokerCacheParams%39B797F800C6.public

  protected:
    //## Constructors (specified)
      //## Operation: ClInvokerCacheParams%968661318
      ClInvokerCacheParams (m4int_t ai_iParamsContainerSize = DefRecycleSize);

    // Data Members for Associations

      //## Association: M4ServerBase::<unnamed>%39BCCBAB01BA
      //## Role: ClInvokerCacheParams::m_oParamContainer%39BCCBAC02DE
      //## begin ClInvokerCacheParams::m_oParamContainer%39BCCBAC02DE.role preserve=no  protected: ClGarbageContainer { -> VHAN}
      ClGarbageContainer m_oParamContainer;
      //## end ClInvokerCacheParams::m_oParamContainer%39BCCBAC02DE.role

    // Additional Protected Declarations
      //## begin ClInvokerCacheParams%39B797F800C6.protected preserve=yes
      //## end ClInvokerCacheParams%39B797F800C6.protected

  private:
    // Additional Private Declarations
      //## begin ClInvokerCacheParams%39B797F800C6.private preserve=yes
      //## end ClInvokerCacheParams%39B797F800C6.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClInvokerCacheParams%39B797F800C6.implementation preserve=yes
      //## end ClInvokerCacheParams%39B797F800C6.implementation

};

//## begin ClInvokerCacheParams%39B797F800C6.postscript preserve=yes
//## end ClInvokerCacheParams%39B797F800C6.postscript

// Parameterized Class ActionInformation 

template <class _T>
inline ActionInformation<_T>::ActionInformation (ClActionIdentifier ai_oActionIdentifier, _T ai_poExecutive)
  //## begin ActionInformation::ActionInformation%957885103.hasinit preserve=no
  //## end ActionInformation::ActionInformation%957885103.hasinit
  //## begin ActionInformation::ActionInformation%957885103.initialization preserve=yes
  //## end ActionInformation::ActionInformation%957885103.initialization
{
  //## begin ActionInformation::ActionInformation%957885103.body preserve=yes
	m_poExecutive = ai_poExecutive ;
	m_oActionIdentifier = ai_oActionIdentifier ;
  //## end ActionInformation::ActionInformation%957885103.body
}



//## Other Operations (inline)
template <class _T>
inline _T ActionInformation<_T>::GetExecutive ()
{
  //## begin ActionInformation::GetExecutive%957885104.body preserve=yes
	return m_poExecutive ;
  //## end ActionInformation::GetExecutive%957885104.body
}

template <class _T>
inline ClActionIdentifier ActionInformation<_T>::GetIdentifier ()
{
  //## begin ActionInformation::GetIdentifier%957885105.body preserve=yes
	return m_oActionIdentifier ;
  //## end ActionInformation::GetIdentifier%957885105.body
}

// Parameterized Class ExecutiveSSMap 


//## Other Operations (inline)
template <class _T>
inline void ExecutiveSSMap<_T>::Insert (m4pchar_t ai_szName, ActionInformation <_T> *ai_oActionInformation)
{
  //## begin ExecutiveSSMap::Insert%955384513.body preserve=yes
	m_oExemap.insert ( map <string , ActionInformation < _T> *, less <string> >::value_type ( ai_szName, ai_oActionInformation ) ) ;
  //## end ExecutiveSSMap::Insert%955384513.body
}

template <class _T>
inline m4return_t ExecutiveSSMap<_T>::GetActionInformation (m4pchar_t ai_szName, ClActionIdentifier &ao_oActionIdentifier, _T &ao_poExecutive)
{
  //## begin ExecutiveSSMap::GetActionInformation%955384514.body preserve=yes
	map <string , ActionInformation < _T> *, less <string> >::iterator Iterator ;

	Iterator = m_oExemap.find ( ai_szName ) ;

	if ( Iterator != m_oExemap.end() )
	{
		ao_poExecutive= (*Iterator).second -> GetExecutive ( );

		ao_oActionIdentifier = (*Iterator).second -> GetIdentifier () ;

		return M4_SUCCESS ;
	}

	ao_poExecutive = NULL ;

	return M4_ERROR ;

  //## end ExecutiveSSMap::GetActionInformation%955384514.body
}

// Parameterized Class ClActionInvoker 

template <class _T>
inline ClActionInvoker<_T>::ClActionInvoker ()
  //## begin ClActionInvoker::ClActionInvoker%957885106.hasinit preserve=no
      : ai_bEnableRegistration(M4_TRUE)
  //## end ClActionInvoker::ClActionInvoker%957885106.hasinit
  //## begin ClActionInvoker::ClActionInvoker%957885106.initialization preserve=yes
  //## end ClActionInvoker::ClActionInvoker%957885106.initialization
{
  //## begin ClActionInvoker::ClActionInvoker%957885106.body preserve=yes
  //## end ClActionInvoker::ClActionInvoker%957885106.body
}



//## Other Operations (inline)
template <class _T>
inline m4return_t ClActionInvoker<_T>::ExecuteAction (m4pchar_t ai_oActionString, ClParamList *ai_poParamList)
{
  //## begin ClActionInvoker::ExecuteAction%957885108.body preserve=yes
	_T  poExecutive ;
	ClBaseAction * poAction = NULL ;
	ClActionIdentifier ActionIdentifier ;
	m4return_t iRet ;


	if ( M4_SUCCESS == m_oExecutivesMap.GetActionInformation ( ai_oActionString , ActionIdentifier , poExecutive ) )
	{
/*		if ( M4_SUCCESS == poExecutive -> CreateAction ( ai_oActionString , poAction ) )
		{
			((ClBaseAction*)poAction) ->SetParams (  ai_poParamList ) ;

			return poExecutive -> ProcessAction ( *poAction ) ;
		}
*/		iRet = poExecutive -> ProcessAction ( ActionIdentifier , ai_poParamList , poAction ) ;
		
		if ( poAction )
			delete poAction ;

		return iRet ;
	}

	return  M4_ERROR ;
  //## end ClActionInvoker::ExecuteAction%957885108.body
}

template <class _T>
inline m4return_t ClActionInvoker<_T>::RegisterAction (m4pchar_t ai_szAction, ClActionIdentifier ai_oIdentifier, _T ai_poExecutive)
{
  //## begin ClActionInvoker::RegisterAction%957885109.body preserve=yes
	_T  poExecutive ;
	ActionInformation <_T> * poActionInformation = new ActionInformation <_T>( ai_oIdentifier , ai_poExecutive ) ;
	ClActionIdentifier oActionIdentifier ;


	if ( ! ai_bEnableRegistration )
		return M4_ERROR ;


	if ( M4_SUCCESS == m_oExecutivesMap.GetActionInformation ( ai_szAction, oActionIdentifier , poExecutive ) )
	{
		return M4_ERROR ;
	}

	m_oExecutivesMap.Insert ( ai_szAction, poActionInformation ) ;

	return M4_SUCCESS ;

  //## end ClActionInvoker::RegisterAction%957885109.body
}

template <class _T>
inline m4return_t ClActionInvoker<_T>::FinishRegistration ()
{
  //## begin ClActionInvoker::FinishRegistration%957885110.body preserve=yes
	if ( ! ai_bEnableRegistration )
		return M4_ERROR ;

	ai_bEnableRegistration = M4_FALSE ;

	return M4_SUCCESS ;
  //## end ClActionInvoker::FinishRegistration%957885110.body
}

// Parameterized Class ClInvokerCacheParams 

template <class _T>
inline ClInvokerCacheParams<_T>::ClInvokerCacheParams (m4int_t ai_iParamsContainerSize)
  //## begin ClInvokerCacheParams::ClInvokerCacheParams%968661318.hasinit preserve=no
  //## end ClInvokerCacheParams::ClInvokerCacheParams%968661318.hasinit
  //## begin ClInvokerCacheParams::ClInvokerCacheParams%968661318.initialization preserve=yes
  : m_oParamContainer ( ai_iParamsContainerSize )
  //## end ClInvokerCacheParams::ClInvokerCacheParams%968661318.initialization
{
  //## begin ClInvokerCacheParams::ClInvokerCacheParams%968661318.body preserve=yes
  //## end ClInvokerCacheParams::ClInvokerCacheParams%968661318.body
}



//## Other Operations (inline)
template <class _T>
inline m4return_t ClInvokerCacheParams<_T>::GetParamList (ClParamList *&ao_poParamList)
{
  //## begin ClInvokerCacheParams::GetParamList%968661317.body preserve=yes
	if ( M4_ERROR ==  m_oParamContainer.GetElement ( (IRecycleable *&)ao_poParamList ) )
	{
		ao_poParamList = new ClParamList ;

//		m_oParamContainer.SetElement ( ai_poParamList ) ;
	}

	return M4_SUCCESS ;

  //## end ClInvokerCacheParams::GetParamList%968661317.body
}

template <class _T>
inline m4return_t ClInvokerCacheParams<_T>::SetParamList (ClParamList &ai_oParamList)
{
  //## begin ClInvokerCacheParams::SetParamList%968661316.body preserve=yes
	return m_oParamContainer.SetElement ( ai_oParamList ) ;
  //## end ClInvokerCacheParams::SetParamList%968661316.body
}

//## begin module%3919152202E7.epilog preserve=yes
//## end module%3919152202E7.epilog


#endif
