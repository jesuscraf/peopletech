//## begin module%377768480361.cm preserve=no
//## end module%377768480361.cm

//## begin module%377768480361.cp preserve=no
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
//## end module%377768480361.cp

//## Module: windowsselectmask%377768480361; Package body
//## Subsystem: M4Tsap::src::win32%377768480360
//	d:\compon\m4tsap\version\src\win32
//## Source file: d:\compon\m4tsap\version\src\win32\windowsselectmask.cpp

//## begin module%377768480361.additionalIncludes preserve=no
//## end module%377768480361.additionalIncludes

//## begin module%377768480361.includes preserve=yes
#include "tsap.hpp"
#include "windowsselectmask.hpp"

//## end module%377768480361.includes

// windowsselectmask
#include <windowsselectmask.hpp>
// m4types
#include <m4types.hpp>
//## begin module%377768480361.declarations preserve=no
//## end module%377768480361.declarations

//## begin module%377768480361.additionalDeclarations preserve=yes
//## end module%377768480361.additionalDeclarations


// Class ClWindowsSelectMask 










ClWindowsSelectMask::ClWindowsSelectMask (unsigned int size)
  //## begin ClWindowsSelectMask::ClWindowsSelectMask%1916352889.hasinit preserve=no
  //## end ClWindowsSelectMask::ClWindowsSelectMask%1916352889.hasinit
  //## begin ClWindowsSelectMask::ClWindowsSelectMask%1916352889.initialization preserve=yes
  //## end ClWindowsSelectMask::ClWindowsSelectMask%1916352889.initialization
{
  //## begin ClWindowsSelectMask::ClWindowsSelectMask%1916352889.body preserve=yes
	  m_iLastUsed = -1;
	  m_iSetSize = FD_SETSIZE;
	  FD_ZERO(&m_stFdSet);
  //## end ClWindowsSelectMask::ClWindowsSelectMask%1916352889.body
}



//## Other Operations (implementation)
m4bool_t ClWindowsSelectMask::Set (unsigned int n)
{
  //## begin ClWindowsSelectMask::Set%-93488872.body preserve=yes
	if (m_stFdSet.fd_count < m_iSetSize )
	{
		if (IsSet(n) == M4_FALSE)
		{
			FD_SET(n, &m_stFdSet);

			return M4_TRUE;
		}
		else
			return M4_FALSE;
	}
	
	return M4_FALSE;
  //## end ClWindowsSelectMask::Set%-93488872.body
}

void ClWindowsSelectMask::Clear (unsigned int n)
{
  //## begin ClWindowsSelectMask::Clear%-1537213506.body preserve=yes
		FD_CLR(n, &m_stFdSet);
  //## end ClWindowsSelectMask::Clear%-1537213506.body
}

m4bool_t ClWindowsSelectMask::IsSet (unsigned int n) const
{
  //## begin ClWindowsSelectMask::IsSet%1495745771.body preserve=yes
	return  (FD_ISSET(n , &m_stFdSet) == 1)?M4_TRUE:M4_FALSE;

  //## end ClWindowsSelectMask::IsSet%1495745771.body
}

int ClWindowsSelectMask::NextSet (unsigned int n)
{
  //## begin ClWindowsSelectMask::NextSet%-118406073.body preserve=yes
	
	m_iLastUsed++;


	  // En caso de haber llegado al final devolvemos un error.
	if ((m4uint16_t )m_iLastUsed >= (m4uint16_t )m_stFdSet.fd_count )
	{
		m_iLastUsed = -1;
		return -1;
	}

	return (int) m_stFdSet.fd_array [ m_iLastUsed ];
  //## end ClWindowsSelectMask::NextSet%-118406073.body
}

void ClWindowsSelectMask::Zero ()
{
  //## begin ClWindowsSelectMask::Zero%677878077.body preserve=yes
	FD_ZERO(&m_stFdSet);
  //## end ClWindowsSelectMask::Zero%677878077.body
}

unsigned int ClWindowsSelectMask::GetSize () const
{
  //## begin ClWindowsSelectMask::GetSize%1783046207.body preserve=yes
	  return m_iSetSize;
  //## end ClWindowsSelectMask::GetSize%1783046207.body
}

void* ClWindowsSelectMask::GetMask () const
{
  //## begin ClWindowsSelectMask::GetMask%-1729912024.body preserve=yes
	  return ( void * )&m_stFdSet ;
  //## end ClWindowsSelectMask::GetMask%-1729912024.body
}

void ClWindowsSelectMask::CopyFrom (const ClWindowsSelectMask& ai_right)
{
  //## begin ClWindowsSelectMask::CopyFrom%613146576.body preserve=yes
		memcpy((void *)&m_stFdSet, ai_right.GetMask(), sizeof(m_stFdSet));
  //## end ClWindowsSelectMask::CopyFrom%613146576.body
}

m4bool_t ClWindowsSelectMask::IsIn (unsigned int ai_iIdentificador)
{
  //## begin ClWindowsSelectMask::IsIn%1952606541.body preserve=yes
	unsigned int	i;

	for (i = 0; i < m_stFdSet.fd_count; i++)
		if (m_stFdSet.fd_array[i] == ai_iIdentificador)
			return M4_TRUE;

	return M4_FALSE;
  //## end ClWindowsSelectMask::IsIn%1952606541.body
}

ClWindowsSelectMask& ClWindowsSelectMask::operator = (const ClWindowsSelectMask& ai_right)
{
  //## begin ClWindowsSelectMask::operator =%91679548.body preserve=yes
	    CopyFrom (  ai_right );
	
		return (*this);

  //## end ClWindowsSelectMask::operator =%91679548.body
}

// Additional Declarations
  //## begin ClWindowsSelectMask%3777684500EF.declarations preserve=yes
  //## end ClWindowsSelectMask%3777684500EF.declarations

//## begin module%377768480361.epilog preserve=yes
//## end module%377768480361.epilog
