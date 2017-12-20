//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         CVM - DM
// File:           IndexList.hpp
// Project:        CVM
// Author:         Meta Software M.S. , S.A
// Date:            27 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//		Permite maneja una lista de valores m4uint32_t.
//   
////
//==============================================================================


#ifndef _INDEXLIST_HPP
#define _INDEXLIST_HPP

#include "m4types.hpp"

class ClIndexList{

public:
	ClIndexList ( void )
	{
		m_iMax = 0;

		m_iCurrent = 0;

		m_piValue = NULL;
	};

	ClIndexList ( m4uint32_t ai_MaxSize )
	{
		m_iMax = ai_MaxSize;

		m_iCurrent = 0;

		if ( m_iMax > 0 )
			m_piValue = new m4uint32_t [ m_iMax ];
		else 
			m_piValue = NULL;
	};

	void Init ( m4uint32_t ai_MaxSize )
	{
		if ( m_piValue != NULL )
			delete [] m_piValue;

		m_iMax = ai_MaxSize;

		m_iCurrent = 0;

		if ( m_iMax > 0 )
			m_piValue = new m4uint32_t [ m_iMax ];
		else 
			m_piValue = NULL;
	};

	~ClIndexList ( void )
	{
		if ( m_piValue != NULL )
			delete [] m_piValue;
	};

	void Reset ( void )
	{
		m_iCurrent = 0;
	}

	m4uint32_t Count ( void )
	{
		return m_iCurrent;
	};

	void Add ( m4uint32_t ai_Value )
	{
		if ( m_iCurrent < m_iMax )
		{
			m_piValue [ m_iCurrent++ ] = ai_Value;
		}
	}

	m4uint32_t Get ( m4uint32_t ai_Index )
	{
		if ( ai_Index < m_iCurrent )
		{
			return m_piValue [ ai_Index ];
		}
		else return 0;
	};

private:

	m4uint32_t m_iMax;
	m4uint32_t m_iCurrent;
	m4uint32_t * m_piValue;

};

#endif
