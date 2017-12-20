//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=yes
//	==============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:			Directory
//	 File:				Directory.cpp
//	 Project:			M4ComSrv
//	 Author:			Meta Software M.S. , S.A
//	 Date:				971003
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
//## end module.cp

//## Module: Directory; Package body
//## Subsystem: CPM4ComSrv::version
//## Source file: x:\m4rel\m4source\3x_work\core\communication\working\eduardoma\M4ComSrv\version\src\Directory.cpp

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
//## end module.includes

// Directory
#include <directory.hpp>
#include <stdio.h>
//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
//## end module.additionalDeclarations


// Class ClExObl 




ClExObl::ClExObl (void )
  //## begin ClExObl::ClExObl%875521753.hasinit preserve=no
  //## end ClExObl::ClExObl%875521753.hasinit
  //## begin ClExObl::ClExObl%875521753.initialization preserve=yes
  //## end ClExObl::ClExObl%875521753.initialization
{
  //## begin ClExObl::ClExObl%875521753.body preserve=yes
	m_oFilter.Init () ;
  //## end ClExObl::ClExObl%875521753.body
}

ClExObl::ClExObl (UINT a_uiHandle)
  //## begin ClExObl::ClExObl%875521754.hasinit preserve=no
  //## end ClExObl::ClExObl%875521754.hasinit
  //## begin ClExObl::ClExObl%875521754.initialization preserve=yes
  : m_oM4Obl(a_uiHandle)
  //## end ClExObl::ClExObl%875521754.initialization
{
  //## begin ClExObl::ClExObl%875521754.body preserve=yes
	m_oFilter.Init () ;
  //## end ClExObl::ClExObl%875521754.body
}


ClExObl::~ClExObl (void )
{
  //## begin ClExObl::~ClExObl%875521755.body preserve=yes
  //## end ClExObl::~ClExObl%875521755.body
}



//## Other Operations (implementation)
int ClExObl::Init (UINT a_uiHandle, int a_iAllocated)
{
  //## begin ClExObl::Init%875521756.body preserve=yes

	//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.Init(a_uiHandle, a_iAllocated);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::Init%875521756.body
}

int ClExObl::Init (M4ClObl& a_rclObl)
{
  //## begin ClExObl::Init%875521757.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.Init(a_rclObl);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::Init%875521757.body
}

int ClExObl::Init (char** a_ppszText, int a_iLength)
{
  //## begin ClExObl::Init%875521758.body preserve=yes
//	printf("\nInit\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.Init(a_ppszText, a_iLength);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::Init%875521758.body
}

int ClExObl::Init (const char* ca_pszFile, size_t a_szReadSize, size_t a_szWriteSize)
{
  //## begin ClExObl::Init%875521759.body preserve=yes
//	printf("\ninit\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.Init(ca_pszFile, a_szReadSize, a_szWriteSize);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::Init%875521759.body
}

int ClExObl::GetClass (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClExObl::GetClass%875521760.body preserve=yes
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetClass (ao_pszString, ai_iStringSize, ao_riOutput);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetClass%875521760.body
}

int ClExObl::GetInstance (char* ao_pszString, size_t ai_iStringSize, size_t& ao_riOutput)
{
  //## begin ClExObl::GetInstance%875521761.body preserve=yes
//	printf("\nGetinstance\n" );
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetInstance (ao_pszString, ai_iStringSize, ao_riOutput);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetInstance%875521761.body
}

int ClExObl::GetNumericValue (char* ai_pszPath, char* ai_pszName, double& ao_rdNumber)
{
  //## begin ClExObl::GetNumericValue%875521762.body preserve=yes
//	printf("\nGetnumericvalue\n" );
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetNumericValue (ai_pszPath, ai_pszName, ao_rdNumber);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetNumericValue%875521762.body
}

int ClExObl::GetNumericValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdNumber)
{
  //## begin ClExObl::GetNumericValue%875521763.body preserve=yes
//	printf("\nGetnumericvalue\n" );
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetNumericValue (ai_hBlockHandle, ai_pszName, ao_rdNumber);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetNumericValue%875521763.body
}

int ClExObl::GetStringValue (char* ai_pszPath, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClExObl::GetStringValue%875521764.body preserve=yes
//	m_oFilter.EnterReader();
//	printf("\nGetstringvalue\n" );
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetStringValue (ai_pszPath, ai_pszName, ao_pszString, ai_iStringSize, ao_riStringLength);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetStringValue%875521764.body
}

int ClExObl::GetStringValue (UINT ai_hBlockHandle, char* ai_pszName, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClExObl::GetStringValue%875521765.body preserve=yes
//	printf("\nGetstringvalue\n" );
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetStringValue (ai_hBlockHandle, ai_pszName, ao_pszString, ai_iStringSize, ao_riStringLength);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetStringValue%875521765.body
}

int ClExObl::GetDateValue (char* ai_pszPath, char* ai_pszName, double& ao_rdDate)
{
  //## begin ClExObl::GetDateValue%875521766.body preserve=yes
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetDateValue (ai_pszPath, ai_pszName, ao_rdDate);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetDateValue%875521766.body
}

int ClExObl::GetDateValue (UINT ai_hBlockHandle, char* ai_pszName, double& ao_rdDate)
{
  //## begin ClExObl::GetDateValue%875521767.body preserve=yes
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetDateValue (ai_hBlockHandle, ai_pszName, ao_rdDate);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetDateValue%875521767.body
}

int ClExObl::SetNumericValue (char* a_pszPath, char* a_pszName, double a_dNumber)
{
  //## begin ClExObl::SetNumericValue%875521770.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.SetNumericValue (a_pszPath, a_pszName, a_dNumber);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::SetNumericValue%875521770.body
}

int ClExObl::SetNumericValue (UINT ai_hBlockHandle, char* a_pszName, double a_dNumber)
{
  //## begin ClExObl::SetNumericValue%875521771.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.SetNumericValue (ai_hBlockHandle, a_pszName, a_dNumber);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::SetNumericValue%875521771.body
}

int ClExObl::SetStringValue (char* a_pszPath, char* a_pszName, char* a_pszString)
{
  //## begin ClExObl::SetStringValue%875521772.body preserve=yes
//	printf("\nSetstringvalue\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.SetStringValue (a_pszPath, a_pszName, a_pszString);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::SetStringValue%875521772.body
}

int ClExObl::SetStringValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszString)
{
  //## begin ClExObl::SetStringValue%875521773.body preserve=yes
//	printf("\nSetstringvalue\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.SetStringValue (ai_hBlockHandle, a_pszName, a_pszString);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::SetStringValue%875521773.body
}

int ClExObl::SetDateValue (char* a_pszPath, char* a_pszName, char* a_pszDate)
{
  //## begin ClExObl::SetDateValue%875521774.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.SetDateValue (a_pszPath, a_pszName, a_pszDate);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::SetDateValue%875521774.body
}

int ClExObl::SetDateValue (UINT ai_hBlockHandle, char* a_pszName, char* a_pszDate)
{
  //## begin ClExObl::SetDateValue%875521775.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.SetDateValue (ai_hBlockHandle, a_pszName, a_pszDate);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::SetDateValue%875521775.body
}

int ClExObl::GetHandleToBlock (char* a_pszPath, UINT& ao_rhHandle)
{
  //## begin ClExObl::GetHandleToBlock%875521776.body preserve=yes
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetHandleToBlock (a_pszPath, ao_rhHandle);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetHandleToBlock%875521776.body
}

int ClExObl::GetBlockInstance (UINT ai_hBlockHandle, char* ao_pszString, size_t ai_iStringSize, size_t& ao_riStringLength)
{
  //## begin ClExObl::GetBlockInstance%875521777.body preserve=yes
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetBlockInstance (ai_hBlockHandle, ao_pszString, ai_iStringSize, ao_riStringLength);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetBlockInstance%875521777.body
}

int ClExObl::AddNumericProperty (char* a_pszPath, char* a_pszName, double a_dNumber)
{
  //## begin ClExObl::AddNumericProperty%875521779.body preserve=yes
//	printf("\naddnumericproperty\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.AddNumericProperty (a_pszPath, a_pszName, a_dNumber);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::AddNumericProperty%875521779.body
}

int ClExObl::AddNumericProperty (UINT a_hBlockHandle, char* a_pszName, double a_dNumber)
{
  //## begin ClExObl::AddNumericProperty%875521780.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.AddNumericProperty (a_hBlockHandle, a_pszName, a_dNumber);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::AddNumericProperty%875521780.body
}

int ClExObl::AddStringProperty (char* a_pszPath, char* a_pszName, char* a_pszString)
{
  //## begin ClExObl::AddStringProperty%875521781.body preserve=yes
//	printf("\nAddstringproperty\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.AddStringProperty (a_pszPath, a_pszName, a_pszString);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::AddStringProperty%875521781.body
}

int ClExObl::AddStringProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszString)
{
  //## begin ClExObl::AddStringProperty%875521782.body preserve=yes
//	printf("\naddstringproperty\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.AddStringProperty (a_hBlockHandle, a_pszName, a_pszString);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::AddStringProperty%875521782.body
}

int ClExObl::AddDateProperty (char* a_pszPath, char* a_pszName, char* a_pszDate)
{
  //## begin ClExObl::AddDateProperty%875521783.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.AddDateProperty (a_pszPath, a_pszName, a_pszDate);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::AddDateProperty%875521783.body
}

int ClExObl::AddDateProperty (UINT a_hBlockHandle, char* a_pszName, char* a_pszDate)
{
  //## begin ClExObl::AddDateProperty%875521784.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.AddDateProperty (a_hBlockHandle, a_pszName, a_pszDate);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::AddDateProperty%875521784.body
}

int ClExObl::AddBlock (char* a_pszPath, char* pszClass, char* pszInstance, UINT& ao_rhHandle)
{
  //## begin ClExObl::AddBlock%875521785.body preserve=yes
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.AddBlock (a_pszPath, pszClass, pszInstance, ao_rhHandle);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::AddBlock%875521785.body
}

int ClExObl::GetAllMatchingBlocks (char* ai_pszPath, UINT* ao_auiBlockHandles, size_t ai_iHandleSize, size_t& ao_riHandleLength)
{
  //## begin ClExObl::GetAllMatchingBlocks%875521786.body preserve=yes
//	printf("\nGetallmatchingblocks\n" );
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetAllMatchingBlocks (ai_pszPath, ao_auiBlockHandles, ai_iHandleSize, ao_riHandleLength);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetAllMatchingBlocks%875521786.body
}

int ClExObl::WriteOut (char* ao_szTextOut, UINT ai_iLenBuf, UINT& ao_iLenOut)
{
  //## begin ClExObl::WriteOut%875521787.body preserve=yes
//	printf("\nwriteout\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.WriteOut (ao_szTextOut, ai_iLenBuf, ao_iLenOut);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::WriteOut%875521787.body
}

int ClExObl::GetPathBlq (unsigned int ai_hBlq, char* ao_szPathBlq)
{
  //## begin ClExObl::GetPathBlq%875521788.body preserve=yes
//	m_oFilter.EnterReader();
	m_oFilter.Lock () ;

	int res = m_oM4Obl.GetPathBlq (ai_hBlq, ao_szPathBlq);

//	m_oFilter.LeaveReader();
	m_oFilter.Unlock () ;

	return res;
  //## end ClExObl::GetPathBlq%875521788.body
}

void ClExObl::operator = (M4ClObl& a_rclObl)
{
  //## begin ClExObl::operator =%875521789.body preserve=yes
//	printf("\noperator =\n" );
//	m_oFilter.EnterWriter();
	m_oFilter.Lock () ;

	m_oM4Obl.operator =(a_rclObl);

//	m_oFilter.LeaveWriter();
	m_oFilter.Unlock () ;
  //## end ClExObl::operator =%875521789.body
}

// Additional Declarations
  //## begin ClExObl.declarations preserve=yes
  //## end ClExObl.declarations





// Class ClDirectory 



ClDirectory::ClDirectory ()
  //## begin ClDirectory::ClDirectory%877020690.hasinit preserve=no
  //## end ClDirectory::ClDirectory%877020690.hasinit
  //## begin ClDirectory::ClDirectory%877020690.initialization preserve=yes
  //## end ClDirectory::ClDirectory%877020690.initialization
{
  //## begin ClDirectory::ClDirectory%877020690.body preserve=yes
  //## end ClDirectory::ClDirectory%877020690.body
}


ClDirectory::~ClDirectory ()
{
  //## begin ClDirectory::~ClDirectory%877020691.body preserve=yes
  //## end ClDirectory::~ClDirectory%877020691.body
}



//## Other Operations (implementation)
m4bool_t ClDirectory::FindHost (m4uint16_t ai_iHostId)
{
  //## begin ClDirectory::FindHost%877020692.body preserve=yes
	return M4_TRUE;
  //## end ClDirectory::FindHost%877020692.body
}

m4bool_t ClDirectory::FindUser (m4uint16_t ai_iHostId)
{
  //## begin ClDirectory::FindUser%877020693.body preserve=yes
	return M4_TRUE;
  //## end ClDirectory::FindUser%877020693.body
}

m4bool_t ClDirectory::FindUser (m4pchar_t &ai_pUserId)
{
  //## begin ClDirectory::FindUser%880396146.body preserve=yes
	return M4_TRUE;
  //## end ClDirectory::FindUser%880396146.body
}

m4bool_t ClDirectory::Find (m4uint16_t ai_iItemId, ClDirectory::e_DirElements ai_eItemType)
{
  //## begin ClDirectory::Find%877020694.body preserve=yes
	return M4_TRUE;
  //## end ClDirectory::Find%877020694.body
}

m4bool_t ClDirectory::CheckLogin (m4pchar_t ai_pUser, m4pchar_t ai_pPasswd)
{
  //## begin ClDirectory::CheckLogin%880396147.body preserve=yes
	return M4_TRUE;
  //## end ClDirectory::CheckLogin%880396147.body
}

// Additional Declarations
  //## begin ClDirectory.declarations preserve=yes
  //## end ClDirectory.declarations



//## begin module.epilog preserve=yes
//## end module.epilog
