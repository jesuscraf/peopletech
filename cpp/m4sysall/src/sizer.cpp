//## begin module%3912C585030C.cm preserve=no
//## end module%3912C585030C.cm

//## begin module%3912C585030C.cp preserve=no
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
//## end module%3912C585030C.cp

//## Module: sizer%3912C585030C; Package body
//## Subsystem: m4sysall::src%3912C5DD0060
//## Source file: d:\new\m4sysall\src\sizer.cpp

//## begin module%3912C585030C.additionalIncludes preserve=no
//## end module%3912C585030C.additionalIncludes

//## begin module%3912C585030C.includes preserve=yes

#include "sizer.hpp"
#include "sizerutils.hpp"

//## end module%3912C585030C.includes

// m4types
#include <m4types.hpp>
#include "m4unicode.hpp"
//## begin module%3912C585030C.declarations preserve=no
//## end module%3912C585030C.declarations

//## begin module%3912C585030C.additionalDeclarations preserve=yes
//## end module%3912C585030C.additionalDeclarations


// Class ClSize 




ClSize::ClSize ()
  //## begin ClSize::ClSize%965426979.hasinit preserve=no
  //## end ClSize::ClSize%965426979.hasinit
  //## begin ClSize::ClSize%965426979.initialization preserve=yes
  //## end ClSize::ClSize%965426979.initialization
{
  //## begin ClSize::ClSize%965426979.body preserve=yes
	m_iSize = 0;
	m_pofs =0;
  //## end ClSize::ClSize%965426979.body
}

ClSize::ClSize (m4pchar_t ai_pFile)
  //## begin ClSize::ClSize%-1336692163.hasinit preserve=no
  //## end ClSize::ClSize%-1336692163.hasinit
  //## begin ClSize::ClSize%-1336692163.initialization preserve=yes
  //## end ClSize::ClSize%-1336692163.initialization
{
  //## begin ClSize::ClSize%-1336692163.body preserve=yes
	m_iSize = 0;
	m_pofs =0;

	if (ai_pFile)
	{
		// UNICODE FILE
		m_pofs = new ofstream;

		eUniFileEncodingType_t eEncoding = M4UniNative ;
		M4Open( *m_pofs, ai_pFile, M4UniWrite, eEncoding );
		
		if (m_pofs->rdbuf()->is_open() == 0)
		{
			delete m_pofs ;
			m_pofs = 0;
		}
	}

  //## end ClSize::ClSize%-1336692163.body
}


ClSize::~ClSize ()
{
  //## begin ClSize::~ClSize%-1295812321.body preserve=yes
	if (m_pofs)
	{
		m_pofs->close();
		delete m_pofs;
		m_pofs = 0;
	}
  //## end ClSize::~ClSize%-1295812321.body
}



//## Other Operations (implementation)
void ClSize::operator += (m4uint32_t ai_i)
{
  //## begin ClSize::operator +=%1171636332.body preserve=yes
	if (m_pofs)
	{
		*m_pofs <<"+"<<ai_i<<"\t\tbytes."<<endl;
	}
	m_iSize+=ai_i;
  //## end ClSize::operator +=%1171636332.body
}

void ClSize::operator -= (m4uint32_t ai_i)
{
  //## begin ClSize::operator -=%-1407478999.body preserve=yes
	if (m_pofs)
	{
		*m_pofs <<"\t-"<<ai_i<<"\tbytes."<<endl;
	}
	m_iSize-=ai_i;
  //## end ClSize::operator -=%-1407478999.body
}

void ClSize::Label (m4pcchar_t ai_label, m4bool_t ai_bShowSize)
{
  //## begin ClSize::Label%475172456.body preserve=yes
	if (m_pofs && ai_label)
	{
		*m_pofs<<ai_label;

		if (ai_bShowSize)
		{
			*m_pofs<<":\t"<<m_iSize<<endl;
		}
	}
  //## end ClSize::Label%475172456.body
}

void ClSize::operator = (ClSize& ai_size)
{
  //## begin ClSize::operator =%931297147.body preserve=yes
	m_iSize=ai_size.m_iSize;
  //## end ClSize::operator =%931297147.body
}

void ClSize::Attach (m4pcchar_t ai_pFile)
{
  //## begin ClSize::Attach%7807315.body preserve=yes
	if (ai_pFile==0)
	{
		return;
	}
	
	if (m_pofs)
	{
		m_pofs->close();
	}
	else
	{
		m_pofs = new ofstream();
	}

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniNative ;
	M4Open( *m_pofs, ai_pFile, M4UniWrite, eEncoding );

	if (m_pofs->rdbuf()->is_open() == 0)
	{
		delete m_pofs ;
		m_pofs = 0;
	}
  //## end ClSize::Attach%7807315.body
}

// Additional Declarations
  //## begin ClSize%3912C584035B.declarations preserve=yes
  //## end ClSize%3912C584035B.declarations

//## begin module%3912C585030C.epilog preserve=yes
//## end module%3912C585030C.epilog
