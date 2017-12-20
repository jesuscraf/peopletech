//==============================================================================
//
// (c) Copyright 2014 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                changelistener.hpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                03-07-2014
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Definitions used in change listening
//
//==============================================================================

#ifndef __CHANGELISTENER_HPP__
#define __CHANGELISTENER_HPP__


#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4clock.hpp"




class IChangeListener
{
public:

	// To be called when change happens
	virtual void SetChange( void ) = 0 ;
};



class M4_DECL_M4DM ClChangeListener : public IChangeListener
{
public:

	ClChangeListener( void )
	{
		if( IsActive() == 1 )
		{
			m_bListenToChanges = M4_TRUE ;
		}
		else
		{
			m_bListenToChanges = M4_FALSE ;
		}
		m_iTimeStamp = 0 ;
	}

	void	SetChange( void )
	{
		if( m_bListenToChanges == M4_TRUE )
		{
			m_iTimeStamp = g_iLastTimeStamp++ ;
		}
	}

	m4uint64_t GetTimeStamp( void ) const
	{
		return( m_iTimeStamp ) ;
	}

	static m4int8_t		IsActive( void ) ;
	static m4uint64_t	GetLastTimeStamp( void ) ;

protected:

	m4bool_t			m_bListenToChanges ;
	m4uint64_t			m_iTimeStamp ;
	
	static m4uint64_t	g_iLastTimeStamp ;
};


#endif

