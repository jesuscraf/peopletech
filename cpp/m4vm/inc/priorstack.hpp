//priorStack.hpp
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                priorStack.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                13-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the stack for dates slices with solve piority algorithm.
//
//
//==============================================================================

//Clase para implementar una pila de tramos
//que contenga prioridades de cada tramo
//y que permita operar con ellas

#ifndef _PRIOR_STACK_H
#define _PRIOR_STACK_H


#include "m4types.hpp"
#include "vmdefine.hpp"
#include "m4stl.hpp"

struct ClPriorSlice{
	m4date_t	m_dStart;
	m4date_t	m_dEnd;
	m4uchar_t	m_iPrior;
	ClPriorSlice (m4date_t ai_dStart,m4date_t ai_dEnd,m4uchar_t	ai_iPrior=M4CL_CM_DEFAULT_PRIOR){
		m_dStart	=	ai_dStart;
		m_dEnd		=	ai_dEnd;
		m_iPrior	=	ai_iPrior;
	}
	ClPriorSlice(){};
	
#ifdef _DEBUG
	void Dump(ostream *os) ;
#endif

};

//operadores (el == vale el de por defecto)
inline m4bool_t operator < (const ClPriorSlice &lhs, const ClPriorSlice &rhs) {
	return (lhs.m_dStart < rhs.m_dStart) ? M4_TRUE : M4_FALSE;
};







typedef vector<ClPriorSlice>				ClPriorSliceStorage;
typedef ClPriorSliceStorage::iterator		ClPriorSliceStorageIt;
typedef ClPriorSliceStorage::const_iterator	ClPriorSliceStorageCIt;



class ClPriorSliceStack{

public:

//básicas
		ClPriorSliceStack();	
		ClPriorSliceStack(ClPriorSliceStack &rhs);	
		virtual ~ClPriorSliceStack();


//manejo
		void Init  (m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4date_t ai_dUnit ,m4uint32_t ai_iSize=CL_VM_DEFAULT_PRIOR_SLICE_STACK_SIZE) ;
		void Clear ();
		void Clear (m4date_t ai_dStartDate,m4date_t ai_dEndDate, m4date_t ai_dUnit);
	
		m4uint32_t GetSize	() const {
			return m_oElements.capacity();
		}
		m4uint32_t GetLength() const {
			return m_oElements.size();
		}

		m4date_t   GetStartDate	() const{
			return m_dStartDate;
		}
		m4date_t   GetEndDate	() const{
			return m_dEndDate;
		}

		m4return_t AddSlice( const ClPriorSlice &ai_oElement );
		m4return_t GetSlice( m4uint32_t ai_iPosition, ClPriorSlice &ai_oElement ) const ; 

//operadores

		ClPriorSliceStack& operator=  (const ClPriorSliceStack& rhs);
		ClPriorSliceStack& operator+= (const ClPriorSliceStack& rhs);

//algoritmos
		
		m4return_t Reduce	(const ClPriorSlice &ai_oSlice);
		m4return_t Sort		();
	
#ifdef _DEBUG
		void DumpStack(ostream *os);
#endif

protected:

		m4date_t	m_dStartDate ;
		m4date_t	m_dEndDate ;
		m4date_t    m_dUnit;
		ClPriorSliceStorage	m_oElements;
		m4bool_t	m_bIsSorted;
};

#endif




