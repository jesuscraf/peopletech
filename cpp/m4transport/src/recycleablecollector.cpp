//## begin module%395090C5031E.cm preserve=no
//## end module%395090C5031E.cm

//## begin module%395090C5031E.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
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
//## end module%395090C5031E.cp

//## Module: RecycleableCollector%395090C5031E; Package body
//## Subsystem: RecycleableCollector::src%395090B40369
//## Source file: F:\integration\m4transport\src\recycleablecollector.cpp

//## begin module%395090C5031E.additionalIncludes preserve=no
//## end module%395090C5031E.additionalIncludes

//## begin module%395090C5031E.includes preserve=yes
//## end module%395090C5031E.includes

// RecycleableCollector
#include <recycleablecollector.hpp>
// garbagecollectorinterface
#include <garbagecollectorinterface.hpp>
// garbageableinterface
#include <garbageableinterface.hpp>
//## begin module%395090C5031E.declarations preserve=no
//## end module%395090C5031E.declarations

//## begin module%395090C5031E.additionalDeclarations preserve=yes
#include "m4trace.hpp"
#include <stdio.h>
#include <transportlayerbase.hpp>
//## end module%395090C5031E.additionalDeclarations


// Class ClRecycleableCollector 



ClRecycleableCollector::~ClRecycleableCollector ()
{
  //## begin ClRecycleableCollector::~ClRecycleableCollector%962001309.body preserve=yes
	Purge();
  //## end ClRecycleableCollector::~ClRecycleableCollector%962001309.body
}



//## Other Operations (implementation)
IGarbageable * ClRecycleableCollector::Recover (int ai_iType)
{
  //## begin ClRecycleableCollector::Recover%961582754.body preserve=yes
//	static contador = 0;
	IGarbageable	*pGarbageable;
	ClMultimapGarbageable::iterator ite;
	if ((ite = m_MultimapGarbageable.find(ai_iType)) 
		== m_MultimapGarbageable.end())
		return (IGarbageable*)NULL;

	pGarbageable= (IGarbageable*)((*ite).second);

	m_MultimapGarbageable.erase(ite);

//	m4Trace(fprintf(stderr, "He ahorrado %d news\n", ++contador));

	return pGarbageable;
  //## end ClRecycleableCollector::Recover%961582754.body
}

m4return_t ClRecycleableCollector::DisposeOf (IGarbageable &ai_oGarbage)
{
  //## begin ClRecycleableCollector::DisposeOf%961667108.body preserve=yes
	pair< int, IGarbageable* > p = 
		make_pair((int)(ai_oGarbage.GetType()), &ai_oGarbage);
	ClMultimapGarbageable::iterator ret; 

	ret = m_MultimapGarbageable.insert(p);
	if (ret == m_MultimapGarbageable.end())
	{
		m4Trace(fprintf(stderr, "Error insertando en MultiMapGarbageablede tipo %d\n", ai_oGarbage.GetType()));

		return M4_ERROR;
	}
	return M4_SUCCESS;
  //## end ClRecycleableCollector::DisposeOf%961667108.body
}

m4uint32_t ClRecycleableCollector::Purge ()
{
  //## begin ClRecycleableCollector::Purge%961667109.body preserve=yes
	ClTransportAction *p = NULL;
	ClMultimapGarbageable::iterator ite =m_MultimapGarbageable.begin(); 
    m4uint32_t iCounter = 0;
	while (ite != m_MultimapGarbageable.end())
	{
        iCounter++;
		p = (ClTransportAction*)((*ite).second);
		if (p != NULL)
			delete p;
		m_MultimapGarbageable.erase(ite);
		ite = m_MultimapGarbageable.begin();
	}

    return iCounter;
  //## end ClRecycleableCollector::Purge%961667109.body
}

m4return_t ClRecycleableCollector::GetElement (IRecycleable *&ao_poElement)
{
  //## begin ClRecycleableCollector::GetElement%968774985.body preserve=yes
	IRecycleable  *pRecycleable;
	ClMultimapGarbageable::iterator ite;
	if ((ite = m_MultimapGarbageable.begin()) 
		== m_MultimapGarbageable.end())
		return M4_ERROR ;

	pRecycleable= (IGarbageable*)((*ite).second);

	m_MultimapGarbageable.erase(ite);

//	m4Trace(fprintf(stderr, "He ahorrado %d news\n", ++contador));

	return M4_SUCCESS ;
  //## end ClRecycleableCollector::GetElement%968774985.body
}

m4return_t ClRecycleableCollector::SetElement (IRecycleable &ai_oElement)
{
  //## begin ClRecycleableCollector::SetElement%968774986.body preserve=yes
	pair< int, IGarbageable* > p = 
		make_pair((int)(ai_oElement.GetType()), (IGarbageable *)&ai_oElement);
	ClMultimapGarbageable::iterator ret; 

	ret = m_MultimapGarbageable.insert(p);
	if (ret == m_MultimapGarbageable.end())
	{
		m4Trace(fprintf(stderr, "Error insertando en MultiMapGarbageablede tipo %d\n", ai_oElement.GetType()));

		return M4_ERROR;
	}
	return M4_SUCCESS;
  //## end ClRecycleableCollector::SetElement%968774986.body
}

// Additional Declarations
  //## begin ClRecycleableCollector%39508FB302C9.declarations preserve=yes
/*
m4return_t ClRecycleableCollector::Recover (IRecycleable *ai_pRecycleable, int ai_iType)
{
	IRecycleable *p;
	typedef ClMultimapRecycleable::iterator I;
	I ite;

		
	pair<I,I> ret = m_MultimapRecycleable.equal_range(ai_iType);
	ite = ret.first;
	if (ite == m_MultimapRecycleable.end())
		return (IRecycleable *)NULL;

	p = (IRecycleable *)((*ite).second);

	m_MultimapRecycleable.erase(ite);

	return p;
}
*/
  //## end ClRecycleableCollector%39508FB302C9.declarations
//## begin module%395090C5031E.epilog preserve=yes
//## end module%395090C5031E.epilog
