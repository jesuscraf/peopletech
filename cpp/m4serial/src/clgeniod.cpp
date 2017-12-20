//## begin module%3911AA2901CB.cm preserve=no
//## end module%3911AA2901CB.cm

//## begin module%3911AA2901CB.cp preserve=no
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
//## end module%3911AA2901CB.cp

//## Module: clgeniod%3911AA2901CB; Package body
//## Subsystem: M4Serial::src%3911B1010152
//## Source file: D:\evol\m4serial\src\clgeniod.cpp

//## begin module%3911AA2901CB.additionalIncludes preserve=no
//## end module%3911AA2901CB.additionalIncludes

//## begin module%3911AA2901CB.includes preserve=yes
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                ClGenIOD.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                10/10/97
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Generic IO Driver module
//
//
//==============================================================================

#include <stdlib.h>
#include <string.h>
#include "clgeniod.hpp"

//## end module%3911AA2901CB.includes

//## begin module%3911AA2901CB.declarations preserve=no
//## end module%3911AA2901CB.declarations

//## begin module%3911AA2901CB.additionalDeclarations preserve=yes
//## end module%3911AA2901CB.additionalDeclarations


// Class ClGenericIODriver 






ClGenericIODriver::ClGenericIODriver ()
  //## begin ClGenericIODriver::ClGenericIODriver%-493449199.hasinit preserve=no
      : m_pTempBuffer(0), m_iTempBufferSize(0), m_iLastErrorCode(GEN_ERR_OK), m_pcLastErrorDescr(0)
  //## end ClGenericIODriver::ClGenericIODriver%-493449199.hasinit
  //## begin ClGenericIODriver::ClGenericIODriver%-493449199.initialization preserve=yes
  //## end ClGenericIODriver::ClGenericIODriver%-493449199.initialization
{
  //## begin ClGenericIODriver::ClGenericIODriver%-493449199.body preserve=yes
  //## end ClGenericIODriver::ClGenericIODriver%-493449199.body
}


ClGenericIODriver::~ClGenericIODriver ()
{
  //## begin ClGenericIODriver::~ClGenericIODriver%137094710.body preserve=yes
   if (m_pTempBuffer)
      free(m_pTempBuffer);

   if (m_pcLastErrorDescr != NULL)
	   delete [] m_pcLastErrorDescr;
  //## end ClGenericIODriver::~ClGenericIODriver%137094710.body
}



//## Other Operations (implementation)
const char* ClGenericIODriver::GetLastErrorDescr ()
{
  //## begin ClGenericIODriver::GetLastErrorDescr%181559525.body preserve=yes
	if (m_pcLastErrorDescr != NULL)
		return m_pcLastErrorDescr;
	return "";
  //## end ClGenericIODriver::GetLastErrorDescr%181559525.body
}

const m4uint32_t ClGenericIODriver::GetLastErrorCode ()
{
  //## begin ClGenericIODriver::GetLastErrorCode%2108973108.body preserve=yes
	return m_iLastErrorCode;
  //## end ClGenericIODriver::GetLastErrorCode%2108973108.body
}

m4return_t ClGenericIODriver::Write (m4uint32_t ai_iNumber)
{
  //## begin ClGenericIODriver::Write%-1355858342.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%-1355858342.body
}

m4return_t ClGenericIODriver::Write (m4int32_t ai_iNumber)
{
  //## begin ClGenericIODriver::Write%-1152310899.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%-1152310899.body
}

m4return_t ClGenericIODriver::Write (m4uint16_t ai_iNumber)
{
  //## begin ClGenericIODriver::Write%-16723042.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%-16723042.body
}

m4return_t ClGenericIODriver::Write (m4int16_t ai_iNumber)
{
  //## begin ClGenericIODriver::Write%-1089248297.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%-1089248297.body
}

m4return_t ClGenericIODriver::Write (m4uint8_t ai_cChar)
{
  //## begin ClGenericIODriver::Write%-101039236.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%-101039236.body
}

m4return_t ClGenericIODriver::Write (m4int8_t ai_cChar)
{
  //## begin ClGenericIODriver::Write%1698443656.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%1698443656.body
}

m4return_t ClGenericIODriver::Write (m4float_t ai_fNumber)
{
  //## begin ClGenericIODriver::Write%1558274491.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%1558274491.body
}

m4return_t ClGenericIODriver::Write (m4double_t ai_dNumber)
{
  //## begin ClGenericIODriver::Write%1821767255.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%1821767255.body
}

m4return_t ClGenericIODriver::Write (m4bool_t ai_iBoolean)
{
  //## begin ClGenericIODriver::Write%924562132.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%924562132.body
}

m4return_t ClGenericIODriver::Write (void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClGenericIODriver::Write%-1836580144.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%-1836580144.body
}

m4return_t ClGenericIODriver::Write (void* ai_pPtr, m4uint32_t ai_iSize)
{
  //## begin ClGenericIODriver::Write%-456901579.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%-456901579.body
}

m4return_t ClGenericIODriver::Write (m4pchar_t ai_pString)
{
  //## begin ClGenericIODriver::Write%808989820.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%808989820.body
}

m4return_t ClGenericIODriver::Write (m4pcchar_t ai_pConstString)
{
  //## begin ClGenericIODriver::Write%1472642975.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Write%1472642975.body
}

m4return_t ClGenericIODriver::WritePrivate (void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClGenericIODriver::WritePrivate%-943504583.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::WritePrivate%-943504583.body
}

m4return_t ClGenericIODriver::WriteFromFile (m4pcchar_t ai_pFileName)
{
  //## begin ClGenericIODriver::WriteFromFile%1907835107.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::WriteFromFile%1907835107.body
}

m4return_t ClGenericIODriver::Read (m4uint32_t& ai_iNumber)
{
  //## begin ClGenericIODriver::Read%-863581831.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-863581831.body
}

m4return_t ClGenericIODriver::Read (m4int32_t& ai_iNumber)
{
  //## begin ClGenericIODriver::Read%74911768.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%74911768.body
}

m4return_t ClGenericIODriver::Read (m4uint16_t& ai_iNumber)
{
  //## begin ClGenericIODriver::Read%-142653055.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-142653055.body
}

m4return_t ClGenericIODriver::Read (m4int16_t& ai_iNumber)
{
  //## begin ClGenericIODriver::Read%-480822432.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-480822432.body
}

m4return_t ClGenericIODriver::Read (m4uint8_t& ai_cChar)
{
  //## begin ClGenericIODriver::Read%1846969027.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%1846969027.body
}

m4return_t ClGenericIODriver::Read (m4int8_t& ai_cChar)
{
  //## begin ClGenericIODriver::Read%1603485412.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%1603485412.body
}

m4return_t ClGenericIODriver::Read (m4float_t& ai_fNumber)
{
  //## begin ClGenericIODriver::Read%814359385.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%814359385.body
}

m4return_t ClGenericIODriver::Read (m4double_t& ai_dNumber)
{
  //## begin ClGenericIODriver::Read%-235357315.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-235357315.body
}

m4return_t ClGenericIODriver::Read (m4bool_t& ai_iBoolean)
{
  //## begin ClGenericIODriver::Read%-193158775.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-193158775.body
}

m4return_t ClGenericIODriver::Read (void** ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClGenericIODriver::Read%-877100280.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-877100280.body
}

m4return_t ClGenericIODriver::Read (void* ai_pPtr, m4uint32_t& ao_iSize)
{
  //## begin ClGenericIODriver::Read%-522227279.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-522227279.body
}

m4return_t ClGenericIODriver::Read (m4pchar_t ao_pString, m4uint32_t ai_iMaxLen)
{
  //## begin ClGenericIODriver::Read%-243869449.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%-243869449.body
}

m4return_t ClGenericIODriver::Read (m4pcchar_t ao_pConstString, m4uint32_t ai_iMaxLen)
{
  //## begin ClGenericIODriver::Read%280845154.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::Read%280845154.body
}

m4return_t ClGenericIODriver::ReadPrivate (void** ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClGenericIODriver::ReadPrivate%-417107426.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::ReadPrivate%-417107426.body
}

m4return_t ClGenericIODriver::ReadSize (m4uint32_t& ao_iSize)
{
  //## begin ClGenericIODriver::ReadSize%-430691484.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::ReadSize%-430691484.body
}

m4return_t ClGenericIODriver::ReadBuffer (void* ai_pPtr)
{
  //## begin ClGenericIODriver::ReadBuffer%-797567939.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::ReadBuffer%-797567939.body
}

m4return_t ClGenericIODriver::ReadToFile (m4pchar_t ao_pFileName)
{
  //## begin ClGenericIODriver::ReadToFile%1479812601.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::ReadToFile%1479812601.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4uint32_t ai_iNumber)
{
  //## begin ClGenericIODriver::SizeOf%-508808661.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-508808661.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4int32_t ai_iNumber)
{
  //## begin ClGenericIODriver::SizeOf%-1588301982.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-1588301982.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4uint16_t ai_iNumber)
{
  //## begin ClGenericIODriver::SizeOf%-544320935.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-544320935.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4int16_t ai_iNumber)
{
  //## begin ClGenericIODriver::SizeOf%-1399257039.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-1399257039.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4uint8_t ai_cChar)
{
  //## begin ClGenericIODriver::SizeOf%-172172140.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-172172140.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4int8_t ai_cChar)
{
  //## begin ClGenericIODriver::SizeOf%903564943.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%903564943.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4float_t ai_fNumber)
{
  //## begin ClGenericIODriver::SizeOf%1155894597.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%1155894597.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4double_t ai_dNumber)
{
  //## begin ClGenericIODriver::SizeOf%653334044.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%653334044.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4bool_t ai_iBoolean)
{
  //## begin ClGenericIODriver::SizeOf%-1434413233.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-1434413233.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4uint32_t ai_iNumber, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%-1879628015.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-1879628015.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4int32_t ai_iNumber, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%-696095098.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-696095098.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4uint16_t ai_iNumber, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%-509598442.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-509598442.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4int16_t ai_iNumber, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%-1175665006.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-1175665006.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4uint8_t ai_cChar, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%660741921.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%660741921.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4int8_t ai_cChar, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%1822153809.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%1822153809.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4float_t ai_fNumber, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%-1455118132.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-1455118132.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4double_t ai_dNumber, m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOf%1818636905.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%1818636905.body
}

m4uint32_t ClGenericIODriver::SizeOf (void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClGenericIODriver::SizeOf%2072738788.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%2072738788.body
}

m4uint32_t ClGenericIODriver::SizeOf (void* ai_pPtr, m4uint32_t ai_iSize)
{
  //## begin ClGenericIODriver::SizeOf%101527962.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%101527962.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4pchar_t ai_pString)
{
  //## begin ClGenericIODriver::SizeOf%248480791.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%248480791.body
}

m4uint32_t ClGenericIODriver::SizeOf (m4pcchar_t ai_pConstString)
{
  //## begin ClGenericIODriver::SizeOf%-382869546.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOf%-382869546.body
}

m4uint32_t ClGenericIODriver::SizeOfArrayPtr (m4uint32_t ai_iNumb)
{
  //## begin ClGenericIODriver::SizeOfArrayPtr%-835836625.body preserve=yes
   return 0;
  //## end ClGenericIODriver::SizeOfArrayPtr%-835836625.body
}

m4uint32_t ClGenericIODriver::TotalSize (void )
{
  //## begin ClGenericIODriver::TotalSize%459220464.body preserve=yes
   return 0;
  //## end ClGenericIODriver::TotalSize%459220464.body
}

void ClGenericIODriver::Reset (void )
{
  //## begin ClGenericIODriver::Reset%-1079325739.body preserve=yes
  //## end ClGenericIODriver::Reset%-1079325739.body
}

m4bool_t ClGenericIODriver::Eof (void )
{
  //## begin ClGenericIODriver::Eof%-829403883.body preserve=yes
   return M4_FALSE;
  //## end ClGenericIODriver::Eof%-829403883.body
}

m4bool_t ClGenericIODriver::Flush (void )
{
  //## begin ClGenericIODriver::Flush%-454387608.body preserve=yes
   return M4_TRUE;
  //## end ClGenericIODriver::Flush%-454387608.body
}

m4uint32_t ClGenericIODriver::Status (void )
{
  //## begin ClGenericIODriver::Status%2054196245.body preserve=yes
   return 0;
  //## end ClGenericIODriver::Status%2054196245.body
}

m4return_t ClGenericIODriver::StoreAddress (void* ai_pPtr)
{
  //## begin ClGenericIODriver::StoreAddress%675300645.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::StoreAddress%675300645.body
}

m4return_t ClGenericIODriver::StoreAddress (void* ai_pPtr, m4uint32_t ai_iOffset)
{
  //## begin ClGenericIODriver::StoreAddress%-2089641674.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::StoreAddress%-2089641674.body
}

m4return_t ClGenericIODriver::StoreAddressPrivate (void* ai_pPtr)
{
  //## begin ClGenericIODriver::StoreAddressPrivate%-118756418.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::StoreAddressPrivate%-118756418.body
}

m4return_t ClGenericIODriver::StoreAddressPrivate (void* ai_pPtr, m4uint32_t ai_iOffset)
{
  //## begin ClGenericIODriver::StoreAddressPrivate%-617548594.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::StoreAddressPrivate%-617548594.body
}

m4uint32_t ClGenericIODriver::GetActualOffset (void )
{
  //## begin ClGenericIODriver::GetActualOffset%1582231088.body preserve=yes
   return 0;
  //## end ClGenericIODriver::GetActualOffset%1582231088.body
}

void* ClGenericIODriver::GetTempBuffer (const m4uint32_t ai_iSize)
{
  //## begin ClGenericIODriver::GetTempBuffer%-791740797.body preserve=yes
   if (ai_iSize>m_iTempBufferSize)
   {
      if (m_pTempBuffer)
         free(m_pTempBuffer);

      if ((m_pTempBuffer=(void*)malloc(ai_iSize)))
         m_iTempBufferSize=ai_iSize;
      else
         m_iTempBufferSize=0;
   }

   return m_pTempBuffer;
  //## end ClGenericIODriver::GetTempBuffer%-791740797.body
}

m4uint16_t ClGenericIODriver::GetArchitect (void )
{
  //## begin ClGenericIODriver::GetArchitect%752440951.body preserve=yes
   return 0;
  //## end ClGenericIODriver::GetArchitect%752440951.body
}

m4return_t ClGenericIODriver::ResetPrivateInfo (void )
{
  //## begin ClGenericIODriver::ResetPrivateInfo%361153502.body preserve=yes
   return M4_SUCCESS;
  //## end ClGenericIODriver::ResetPrivateInfo%361153502.body
}

void ClGenericIODriver::SetLastErrorDescr (const char* ai_pcDescr)
{
  //## begin ClGenericIODriver::SetLastErrorDescr%1729258322.body preserve=yes
	if (ai_pcDescr == NULL)
		return;

	size_t size = strlen(ai_pcDescr);
	if (m_pcLastErrorDescr != NULL && strlen(m_pcLastErrorDescr) < size)
	{
		delete [] m_pcLastErrorDescr;
		m_pcLastErrorDescr = NULL;
	}

	if (m_pcLastErrorDescr == NULL)
		m_pcLastErrorDescr = new m4char_t[size+1];

	if (m_pcLastErrorDescr == NULL)
	{
		 m_iLastErrorCode = GEN_ERR_OUT_OF_MEMORY;
		 return;
	}

	strcpy(m_pcLastErrorDescr,ai_pcDescr);
  //## end ClGenericIODriver::SetLastErrorDescr%1729258322.body
}

// Additional Declarations
  //## begin ClGenericIODriver%3911AA24022F.declarations preserve=yes
  //## end ClGenericIODriver%3911AA24022F.declarations

//## begin module%3911AA2901CB.epilog preserve=yes
//## end module%3911AA2901CB.epilog
