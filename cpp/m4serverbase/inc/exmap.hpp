//## begin module%344642190141.cm preserve=no
//## end module%344642190141.cm

//## begin module%344642190141.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Utilities
//	 File:				ExMap.hpp
//	 Project:			M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971006
//	 Language:			C++
//	 Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	==============================================================================
//## end module%344642190141.cp

//## Module: ExMap%344642190141; Package specification
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Source file: d:\eduardoma\m4serverbase\version\inc\exmap.hpp

#ifndef exmap_h
#define exmap_h 1

//## begin module%344642190141.additionalIncludes preserve=no
//## end module%344642190141.additionalIncludes

//## begin module%344642190141.includes preserve=yes
#include <m4stl.hpp>
//## end module%344642190141.includes

//## begin module%344642190141.declarations preserve=no
//## end module%344642190141.declarations

//## begin module%344642190141.additionalDeclarations preserve=yes
#ifdef _HP 
#define M4_TYPENAME typename
#else
#define M4_TYPENAME	
#endif
//## end module%344642190141.additionalDeclarations


//## Class: ExMap%342FEA8902CE; Parameterized Class
//	This class encapsulates the basic functionality for an stl map with a custom
//	exclusion algorithm and a custom automatic-access mechanism.
//## Category: M4ServerBase::Utilities%34183CFC01EF
//## Subsystem: M4ServerBase::ServerQ::inc%3794AA370061
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%342FEABC0270;map { -> }

template <class _K, class _Ty, class _RW, class _Pr>
class ExMap 
{
  public:
    //## Constructors (specified)
      //## Operation: ExMap%875521794; C++
      //	Parameterized constructor for class AutoMap.
      //	Parameters
      //	- _Pr: An object of class  _P containing the ordering algorithm.
      ExMap (const _Pr& _Pred = _Pr());


    //## Other Operations (specified)
      //## Operation: Insert%875521795; C++
      //	Inserts a new element at some entry in the map.
      //	Return Values
      //	No return value.
      //	Remarks
      //	This function is intended to block if the queue is full
      m4bool_t Insert (const _Ty &ai_oEntry, const _K &ai_oKey);

      //## Operation: Erase%875521796; C++
      m4bool_t Erase (const _K &ai_oKey);

      //## Operation: GetEntry%875521797; C++
      _Ty & GetEntry (const _K &ai_oKey);

      //## Operation: SetEntry%875521798; C++
      void SetEntry (const _K &ai_oKey, _Ty &ai_oEntry);

      //## Operation: Size%876126184; C++
      //	Retrieves the number of entries in the map.
      //	No return value.
      //	The number of entries if the map is not empty.
      //	Remarks
      //	This function is intended to block if the queue is full
      m4size_t Size ();

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_oContainer%342FEDEF004D
      //## begin ExMap::m_oContainer%342FEDEF004D.attr preserve=no  protected: map<_K,_Ty,_Pr> {VA} 
      map<_K,_Ty,_Pr> m_oContainer;
      //## end ExMap::m_oContainer%342FEDEF004D.attr

      //## Attribute: m_Filter%343A29F6000B
      //	This attribute instantiates the Mutual Exclusion algorithm to prevent
      //	threads from performing incompatible actions.
      //## begin ExMap::m_Filter%343A29F6000B.attr preserve=no  protected: _RW {UA} 
      _RW m_Filter;
      //## end ExMap::m_Filter%343A29F6000B.attr

  private:
  private: //## implementation
};

// Parameterized Class ExMap 

// Parameterized Class ExMap 



template <class _K, class _Ty, class _RW, class _Pr>
ExMap<_K,_Ty,_RW,_Pr>::ExMap (const _Pr& _Pred)
  //## begin ExMap::ExMap%875521794.hasinit preserve=no
  //## end ExMap::ExMap%875521794.hasinit
  //## begin ExMap::ExMap%875521794.initialization preserve=yes
  : m_oContainer(_Pred)
  //## end ExMap::ExMap%875521794.initialization
{
  //## begin ExMap::ExMap%875521794.body preserve=yes
  //## end ExMap::ExMap%875521794.body
}



//## Other Operations (implementation)
template <class _K, class _Ty, class _RW, class _Pr>
m4bool_t ExMap<_K,_Ty,_RW,_Pr>::Insert (const _Ty &ai_oEntry, const _K &ai_oKey)
{
  //## begin ExMap::Insert%875521795.body preserve=yes
	m_Filter.EnterWriter();

//	MOD	EMN	980312	pair<key_type, value_type> entry(_k,_X);
#if defined ( _AIX ) || defined ( _HP )
	pair<_K, _Ty> entry(ai_oKey,ai_oEntry);
#else
	M4_TYPENAME	value_type entry(ai_oKey,ai_oEntry);
#endif
	pair<typename map<_K,_Ty,_Pr>::iterator, bool> ref = m_oContainer.insert(entry);

	m_Filter.LeaveWriter();

	return (ref.second)?M4_TRUE:M4_FALSE;
  //## end ExMap::Insert%875521795.body
}

template <class _K, class _Ty, class _RW, class _Pr>
m4bool_t ExMap<_K,_Ty,_RW,_Pr>::Erase (const _K &ai_oKey)
{
  //## begin ExMap::Erase%875521796.body preserve=yes
	m_Filter.EnterWriter();

	m4size_t	ref = m_oContainer.erase(ai_oKey);

	m_Filter.LeaveWriter();

	//	ADD	EMN	981028	Warnings Solaris
//	m4bool_t	bRet = (0 < ref);
	return (0 < ref )?M4_TRUE:M4_FALSE;
  //## end ExMap::Erase%875521796.body
}

template <class _K, class _Ty, class _RW, class _Pr>
_Ty & ExMap<_K,_Ty,_RW,_Pr>::GetEntry (const _K &ai_oKey)
{
  //## begin ExMap::GetEntry%875521797.body preserve=yes
	m_Filter.EnterReader();

	m4size_t	isEmpty = m_oContainer.count(ai_oKey);
	_Ty &ref = m_oContainer[ai_oKey];
	if (0 == ref)
		ref = NULL;

	m_Filter.LeaveReader();

	return ref;
  //## end ExMap::GetEntry%875521797.body
}

template <class _K, class _Ty, class _RW, class _Pr>
void ExMap<_K,_Ty,_RW,_Pr>::SetEntry (const _K &ai_oKey, _Ty &ai_oEntry)
{
  //## begin ExMap::SetEntry%875521798.body preserve=yes
	m_Filter.EnterWriter();

	M4_TYPENAME	map<_K,_Ty,_Pr>::value_type entry(ai_oKey,ai_oEntry);
	typedef typename map<_K,_Ty,_Pr>::iterator mapiterator;
	pair<mapiterator, bool> ref = m_oContainer.insert(entry);

	m_Filter.LeaveWriter();
  //## end ExMap::SetEntry%875521798.body
}

template <class _K, class _Ty, class _RW, class _Pr>
m4size_t ExMap<_K,_Ty,_RW,_Pr>::Size ()
{
  //## begin ExMap::Size%876126184.body preserve=yes
	m_Filter.EnterReader();

	m4size_t	ref = (m4size_t)m_oContainer.size();

	m_Filter.LeaveReader();

//	DEL	!COMORR! EMN	990927	return (0 < ref);
	return ref;
  //## end ExMap::Size%876126184.body
}

//## begin module%344642190141.epilog preserve=yes
//## end module%344642190141.epilog


#endif
