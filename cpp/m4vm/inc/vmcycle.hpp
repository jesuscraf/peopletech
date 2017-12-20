//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           vm
// File:             vmcycle.hpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             17/06/99
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines a class to detect cycles in graphs
//
//
//==============================================================================

#ifndef _ClVMCYCLE_H
#define _ClVMCYCLE_H

#include "m4types.hpp"
#include "m4stl.hpp"


class ClVMCycle {

public:
	typedef vector<m4int32_t> vectorInt;
	
	typedef vector< vectorInt > vectorVectorInt;
	
	ClVMCycle(void){
		m_iCount = 0;
	}

	ClVMCycle(m4uint32_t n){
	
		m_iCount = n;
		m_oOrdering.reserve(m_iCount);
		m_oAdjacent.reserve(m_iCount);
		m_oGrade.reserve(m_iCount);

	}
	virtual ~ClVMCycle(){
	}

	virtual void Init(m4uint32_t n){
		m_iCount = n;
		m_oOrdering.reserve(m_iCount);
		m_oAdjacent.reserve(m_iCount);
		m_oGrade.reserve(m_iCount);
	}

	virtual m4return_t CreateAdjacentList(void)=0;
	
	virtual m4return_t DetectCyclesOrdering(void);

#ifdef _DEBUG
	virtual void DumpAdjacentList(ostream &os);
	virtual void DumpOrdering(ostream &os);
#endif

protected:

	vectorVectorInt	m_oAdjacent;
	vectorInt		m_oOrdering;
	vectorInt		m_oGrade;


	m4uint32_t m_iCount;
}
;

class ClVMCycleSimple: public ClVMCycle{

public:

	ClVMCycleSimple (void){
	}
	ClVMCycleSimple (m4int32_t *ai_piGraph, m4uint32_t ai_iCount):ClVMCycle(ai_iCount){
		m_piGraph = ai_piGraph;
		m_iCount  = ai_iCount;
	}
	void SetGraph (m4int32_t *ai_piGraph, m4uint32_t ai_iCount){
		m_piGraph = ai_piGraph;
		Init(ai_iCount);
	}

	m4return_t CreateAdjacentList(void);
private:

	m4int32_t			*m_piGraph;

};

#endif // _ClVMCYCLE_H
