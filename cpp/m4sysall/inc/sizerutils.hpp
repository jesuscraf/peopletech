//## begin module%3912C5850136.cm preserve=no
//## end module%3912C5850136.cm

//## begin module%3912C5850136.cp preserve=no
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
//## end module%3912C5850136.cp

//## Module: sizerutils%3912C5850136; Package specification
//## Subsystem: m4sysall::inc%3912C5D60363
//## Source file: d:\new\m4sysall\inc\sizerutils.hpp

#ifndef __SIZERUTILS_HPP_
#define __SIZERUTILS_HPP_ 1

//## begin module%3912C5850136.additionalIncludes preserve=no
//## end module%3912C5850136.additionalIncludes

//## begin module%3912C5850136.includes preserve=yes

#include "m4sysall_dll.hpp"
#include "m4stl.hpp"
//## end module%3912C5850136.includes

// sizer
#include <sizer.hpp>
// m4types
#include <m4types.hpp>
//## begin module%3912C5850136.declarations preserve=no
//## end module%3912C5850136.declarations

//## begin module%3912C5850136.additionalDeclarations preserve=yes

class ClSize;


template <class PointerVector>
m4return_t
GetVectorSize(vector<PointerVector> &ai_vT,ClSize &ao_size)
{
	M4_TYPE_ITERATOR vector<PointerVector>::iterator	it;
	if (GetVectorStructureSize(ai_vT,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}
	for (it=ai_vT.begin();it!=ai_vT.end();++it)
	{
		if ( (*it)->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}

template <class AnyVector>
m4return_t
GetVectorStructureSize(vector<AnyVector> &ai_vT,ClSize &ao_size)
{
	ao_size +=sizeof(ai_vT);
	ao_size+=ai_vT.capacity()*sizeof(AnyVector);
	return M4_SUCCESS;
}

template <class PointerList>
m4return_t
GetListSize(list<PointerList> &ai_lT,ClSize &ao_size)
{
	M4_TYPE_ITERATOR list<PointerList>::iterator	it;

	if (GetListStructureSize(ai_lT,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	for (it=ai_lT.begin();it!=ai_lT.end();++it)
	{
		if ( (*it)->GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}

template <class Object>
m4return_t
GetObjectListSize(list<Object> &ai_lT,ClSize &ao_size)
{
	M4_TYPE_ITERATOR list<Object>::iterator	it;

	if (GetListStructureSize(ai_lT,ao_size)!=M4_SUCCESS){
		return M4_ERROR;
	}

	for (it=ai_lT.begin();it!=ai_lT.end();++it)
	{
		if ( (*it).GetSize(ao_size)!=M4_SUCCESS){
			return M4_ERROR;
		}
	}
	return M4_SUCCESS;
}

template <class PointerList>
m4return_t
GetListStructureSize(list<PointerList> &ai_lT,ClSize &ao_size)
{
	m4uint32_t node_size;
	M4_TYPE_ITERATOR list<PointerList>::iterator	it;
	
	ao_size +=sizeof(ai_lT);

	node_size = sizeof(*it);
	node_size += 2*sizeof(void *); // two pointer for each node.
	ao_size+=node_size;	// root_node
	ao_size+=node_size*ai_lT.size();
	return M4_SUCCESS;
}

template <class KeyMap, class PointerMap, class CompareMap>
m4return_t
GetMapStructureSize(map<KeyMap , PointerMap, CompareMap> &ai_map,ClSize &ao_size)
{
	m4uint32_t node_size;
	M4_TYPE_ITERATOR map<KeyMap , PointerMap, CompareMap>::iterator	it;

	ao_size+=sizeof(ai_map);

	node_size = sizeof( (*it).first);
	node_size += sizeof( (*it).second);
	node_size+= 3*sizeof(void *);	// por cada elemento del mapa, se crean tres punteros : left, right, father
	node_size+= sizeof(int);			// color (en OS).
	
	ao_size+=node_size;	//header
	ao_size+=node_size;	//nil_node
	
	ao_size+=node_size*ai_map.size();
	

	return M4_SUCCESS;
}

//## end module%3912C5850136.additionalDeclarations


//## begin ClSizerLabel%3912C585015E.preface preserve=yes
//## end ClSizerLabel%3912C585015E.preface

//## Class: ClSizerLabel%3912C585015E
//## Category: m4sysall%3912C5830151
//## Subsystem: m4sysall::inc%3912C5D60363
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SYSALL ClSizerLabel 
{
  //## begin ClSizerLabel%3912C585015E.initialDeclarations preserve=yes
  //## end ClSizerLabel%3912C585015E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClSizerLabel%148922384; C++
      ClSizerLabel (ClSize& ao_size, m4pcchar_t ai_label)
        //## begin ClSizerLabel::ClSizerLabel%148922384.hasinit preserve=no
            : m_pSizer(&ao_size), m_pLabel(ai_label)
        //## end ClSizerLabel::ClSizerLabel%148922384.hasinit
        //## begin ClSizerLabel::ClSizerLabel%148922384.initialization preserve=yes
        //## end ClSizerLabel::ClSizerLabel%148922384.initialization
      {
        //## begin ClSizerLabel::ClSizerLabel%148922384.body preserve=yes
		m_pSizer->Label("Begin\t",M4_FALSE);
		m_pSizer->Label(m_pLabel);
		m_pSizer=&ao_size;
	
        //## end ClSizerLabel::ClSizerLabel%148922384.body
      }

    //## Destructor (specified)
      //## Operation: ~ClSizerLabel%232021020; C++
      ~ClSizerLabel ()
      {
        //## begin ClSizerLabel::~ClSizerLabel%232021020.body preserve=yes
		m_pSizer->Label("End\t",M4_FALSE);
		m_pSizer->Label(m_pLabel);
	
        //## end ClSizerLabel::~ClSizerLabel%232021020.body
      }

    // Additional Public Declarations
      //## begin ClSizerLabel%3912C585015E.public preserve=yes
      //## end ClSizerLabel%3912C585015E.public

  protected:
    // Additional Protected Declarations
      //## begin ClSizerLabel%3912C585015E.protected preserve=yes
      //## end ClSizerLabel%3912C585015E.protected

  private:
    // Data Members for Associations

      //## Association: m4sysall::<unnamed>%3912C5850168
      //## Role: ClSizerLabel::m_pSizer%3912C585016A
      //## begin ClSizerLabel::m_pSizer%3912C585016A.role preserve=no  private: ClSize {1 -> 0..1RHAPN}
      ClSize *m_pSizer;
      //## end ClSizerLabel::m_pSizer%3912C585016A.role

      //## Association: m4sysall::<unnamed>%3912C58501A4
      //## Role: ClSizerLabel::m_pLabel%3912C58501A6
      //## begin ClSizerLabel::m_pLabel%3912C58501A6.role preserve=no  private: m4pcchar_t {1 -> 1VHAPN}
      m4pcchar_t m_pLabel;
      //## end ClSizerLabel::m_pLabel%3912C58501A6.role

    // Additional Private Declarations
      //## begin ClSizerLabel%3912C585015E.private preserve=yes
      //## end ClSizerLabel%3912C585015E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClSizerLabel%3912C585015E.implementation preserve=yes
      //## end ClSizerLabel%3912C585015E.implementation

};

//## begin ClSizerLabel%3912C585015E.postscript preserve=yes
//## end ClSizerLabel%3912C585015E.postscript

//## begin module%3912C5850136.epilog preserve=yes
//## end module%3912C5850136.epilog


#endif


