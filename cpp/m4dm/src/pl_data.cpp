//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             pl_data.cpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             04/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Simple filter code
//
//
//==============================================================================

#include "clgeniod.hpp"
#include "pl_data.hpp"
#include "sizer.hpp"



ClPartialLoadData::ClPartialLoadData(m4uint32_t ai_iMaxReg)
{
    m_iFirstVisible = 0;//Inicializacion: valida tanto con o sin autoload
    m_iLastVisible = 0; //debido a que los valores corresponden a no haber cargado nada.
    m_iNextFV = 0;
    m_iNextLV = 0;
    m_iFirstToWrite = 0;
    m_iNumberToWrite = ai_iMaxReg; //tamaño de ventana
    m_iFirstToRead = 0;
    m_iReadRegister = 0;
    m_iCurrentRegister = 0;
    m_iEnd = 0;
    m_iPrep = 0;
	m_iMaxRegisters = -1;
    m_bLocked = M4_FALSE;
    m_bEOR = M4_FALSE;
    m_bFirstWindowLoaded = M4_FALSE;
    m_bLoadFromPageFault = M4_FALSE;
}


ClPartialLoadData::~ClPartialLoadData( void )
{
}

m4int32_t ClPartialLoadData::GetFirstVisible(void) const
{
    return m_iFirstVisible;
}

m4int32_t ClPartialLoadData::GetLastVisible(void) const
{
    return m_iLastVisible;
}

m4int32_t ClPartialLoadData::GetNextFV(void) const
{
    return m_iNextFV;
}

m4int32_t ClPartialLoadData::GetNextLV(void) const
{
    return m_iNextLV;
}

m4int32_t ClPartialLoadData::GetFirstToWrite(void) const
{
    return m_iFirstToWrite;
}

m4int32_t ClPartialLoadData::GetNumberToWrite(void) const
{
    return m_iNumberToWrite;
}

m4int32_t ClPartialLoadData::GetFirstToRead(void) const
{
    return m_iFirstToRead;
}

m4int32_t ClPartialLoadData::GetReadRegister(void) const
{
    return m_iReadRegister;
}

m4int32_t ClPartialLoadData::GetCurrentRegister(void) const
{
    return m_iCurrentRegister;
}

m4int32_t ClPartialLoadData::GetEnd(void) const
{
    return m_iEnd;
}

m4int32_t ClPartialLoadData::GetPrepareIndexPL (void) const
{
    return m_iPrep;
}

m4int32_t ClPartialLoadData::GetMaxRegisters(void) const
{
    return m_iMaxRegisters;
}

m4bool_t ClPartialLoadData::GetLocked(void) const
{
    return m_bLocked;
}

m4bool_t  ClPartialLoadData::GetEndOfRegisters(void) const
{
    return m_bEOR;
}

m4bool_t  ClPartialLoadData::GetFirstWindowLoaded(void) const
{
    return m_bFirstWindowLoaded;
}

m4bool_t  ClPartialLoadData::GetLoadFromPageFault(void) const
{
    return m_bLoadFromPageFault;
}

M4PLFileMap_t&  ClPartialLoadData::GetFileMap(void)
{
    return m_oFileMap;
}


void ClPartialLoadData::SetFirstVisible(m4int32_t ai_iFV)
{
    m_iFirstVisible = ai_iFV;
}

void ClPartialLoadData::SetLastVisible(m4int32_t ai_iLV)
{
    m_iLastVisible = ai_iLV;
}

void ClPartialLoadData::SetNextFV(m4int32_t ai_iNFV)
{
    m_iNextFV = ai_iNFV;
}

void ClPartialLoadData::SetNextLV(m4int32_t ai_iNLV)
{
    m_iNextLV = ai_iNLV;
}

void ClPartialLoadData::SetFirstToWrite(m4int32_t ai_iFW)
{
    m_iFirstToWrite = ai_iFW;
}

void ClPartialLoadData::SetNumberToWrite(m4int32_t ai_iNW)
{
    m_iNumberToWrite = ai_iNW;
}

void ClPartialLoadData::SetFirstToRead(m4int32_t ai_iFR)
{
    m_iFirstToRead = ai_iFR;
}

void ClPartialLoadData::SetReadRegister(m4int32_t ai_iRR)
{
    m_iReadRegister = ai_iRR;
}

void ClPartialLoadData::SetCurrentRegister(m4int32_t ai_iCR)
{
    m_iCurrentRegister = ai_iCR;
}

void ClPartialLoadData::SetEnd(m4int32_t ai_iEnd)
{
    m_iEnd = ai_iEnd;
}

void ClPartialLoadData::SetPrepareIndexPL(m4int32_t ai_iPrep)
{
    m_iPrep = ai_iPrep;
}

void ClPartialLoadData::SetMaxRegisters( void )
{
	if( m_bEOR == M4_TRUE )
	{
		m_iMaxRegisters = m_iLastVisible;
	}
}

void ClPartialLoadData::SetLocked(m4bool_t ai_bLocked)
{
    m_bLocked = ai_bLocked;
}

void ClPartialLoadData::SetEndOfRegisters(m4bool_t ai_bEOR)
{
    m_bEOR = ai_bEOR;
}

void ClPartialLoadData::SetFirstWindowLoaded(m4bool_t ai_bFirstWindowLoaded)
{
    m_bFirstWindowLoaded = ai_bFirstWindowLoaded;
}

void ClPartialLoadData::SetLoadFromPageFault(m4bool_t ai_bLoadFromPageFault)
{
    m_bLoadFromPageFault = ai_bLoadFromPageFault;
}


m4return_t ClPartialLoadData::Serialize(ClGenericIODriver& IOD, m4bool_t ai_bWritePLFileMap)
{

	m4uint32_t	iLength = 0 ;
	m4uint32_t	iRegister = 0 ;
	long		lPosition = -1 ;

	M4PLFileMap_t::iterator	iterator ;


   if (IOD.Write(m_iFirstVisible)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iLastVisible)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iNextFV)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iNextLV)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iFirstToWrite)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iNumberToWrite)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iFirstToRead)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iReadRegister)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iCurrentRegister)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iEnd)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_bLocked)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iPrep)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_iMaxRegisters)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_bEOR)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_bFirstWindowLoaded)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Write(m_bLoadFromPageFault)!=M4_SUCCESS)
      return M4_ERROR;


	if( ai_bWritePLFileMap == M4_FALSE )
	{
	   iLength = 0 ;
	}
	else
	{
	   iLength = m_oFileMap.size() ;
	}

	if( IOD.Write( iLength ) != M4_SUCCESS ) { return ( M4_ERROR ) ; }

	if( iLength > 0 )
	{
		iterator = m_oFileMap.begin() ;

		while( iterator != m_oFileMap.end() )
		{
			iRegister = (*iterator).first ;
			lPosition = (*iterator).second ;

			if( IOD.Write( iRegister ) != M4_SUCCESS ) { return ( M4_ERROR ) ; }
			if( IOD.Write( lPosition ) != M4_SUCCESS ) { return ( M4_ERROR ) ; }

			iterator++ ;
		}
	}

   return M4_SUCCESS;
}

m4return_t ClPartialLoadData::DeSerialize(ClGenericIODriver& IOD)
{

	m4uint32_t	i = 0 ;
	m4uint32_t	iLength = 0 ;
	m4uint32_t	iRegister = 0 ;
	long		lPosition = -1 ;


	if (IOD.Read(m_iFirstVisible)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iLastVisible)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iNextFV)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iNextLV)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iFirstToWrite)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iNumberToWrite)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iFirstToRead)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iReadRegister)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iCurrentRegister)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iEnd)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_bLocked)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iPrep)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_iMaxRegisters)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_bEOR)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_bFirstWindowLoaded)!=M4_SUCCESS)
      return M4_ERROR;

   if (IOD.Read(m_bLoadFromPageFault)!=M4_SUCCESS)
      return M4_ERROR;


	if( IOD.Read( iLength ) != M4_SUCCESS ) { return ( M4_ERROR ) ; }

	if( iLength > 0 )
	{
		m_oFileMap.clear() ;

		for( i = 0 ; i < iLength ; i++ )
		{
			if( IOD.Read( iRegister ) != M4_SUCCESS ) { return ( M4_ERROR ) ; }
			if( IOD.Read( lPosition ) != M4_SUCCESS ) { return ( M4_ERROR ) ; }

			m_oFileMap[ iRegister ] = lPosition ;
		}
	}


   return M4_SUCCESS;
}

m4return_t 
ClPartialLoadData::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClPartialLoadData);

	return M4_SUCCESS;
}

