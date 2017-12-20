//## begin module%3911AA29006D.cm preserve=no
//## end module%3911AA29006D.cm

//## begin module%3911AA29006D.cp preserve=no
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
//## end module%3911AA29006D.cp

//## Module: cldatstoiod%3911AA29006D; Package body
//## Subsystem: M4Serial::src%3911B1010152
//## Source file: d:\new\m4serial\src\cldatstoiod.cpp

//## begin module%3911AA29006D.additionalIncludes preserve=no
//## end module%3911AA29006D.additionalIncludes

//## begin module%3911AA29006D.includes preserve=yes
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                ClDatStoIOD.cpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                8/1/98
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Data Storages Driver module
//
//
//==============================================================================

#ifdef _WIN32_WINNT
#include <windows.h>
#endif

#include "m4stl.hpp"
#include "cldatstoiod.hpp"
#include "m4filemanager.hpp"


// Ojo m4date_t	= m4double_t	en la Serializacion
// Una fecha se serializa como un double

//## end module%3911AA29006D.includes

// cltadof
#include <cltadof.hpp>
// clgeniod
#include <clgeniod.hpp>
// DataStorages
#include <datastorages.hpp>
// m4types
#include <m4types.hpp>
//## begin module%3911AA29006D.declarations preserve=no
//## end module%3911AA29006D.declarations

//## begin module%3911AA29006D.additionalDeclarations preserve=yes


//////////////////////////////   WRITE   //////////////////////////////

template <class aType>
static inline void _Write(aType          ai_Value,
                          m4char_t*&     ao_pPtr,
								  m4char_t*&     ao_pInitialPtr,
								  m4uint32_t&    ao_uiOffset,
								  M4DataStorage* ai_poDataStor,
                          m4uint32_t     ai_uiMode,
								  ClDataUnit*&   ao_pDataUnit)
{
   M4_ASSERT((ai_uiMode==GEN_IO_MODE_WRITE) || (ai_uiMode==GEN_IO_MODE_APPEND));

   if (ao_pPtr==(m4char_t*)0) // First block
   {
      ai_poDataStor->GetNewDataUnit(ao_pDataUnit);
      ao_pDataUnit->SetSize(MAX_LENG_BLOCK); 	
		ao_pPtr=ao_pDataUnit->GetBuffer();
      M4_ASSERT(ao_pPtr);
      ao_pInitialPtr=ao_pPtr;
   }

   if (ao_pPtr-ao_pInitialPtr+sizeof(aType)>MAX_LENG_BLOCK)
   {
		// Ajuste del tamaño del anterior...
      ao_pDataUnit->SetSize((m4uint32_t)(ao_pPtr-ao_pInitialPtr));

		// Nuevo...
      ai_poDataStor->GetNewDataUnit(ao_pDataUnit);
      ao_pDataUnit->SetSize(MAX_LENG_BLOCK); 	
		ao_pPtr=ao_pDataUnit->GetBuffer();
      M4_ASSERT(ao_pPtr);
      ao_pInitialPtr=ao_pPtr;
   }

#ifdef _UNIX
   memcpy( (void *)ao_pPtr, (void *)&ai_Value, sizeof(aType) );
#else
   *((aType*)ao_pPtr)=ai_Value;
#endif

   ao_pPtr+=sizeof(aType);
   ao_uiOffset+=sizeof(aType);
}


//////////////////////////////   READ   //////////////////////////////

template <class aType>
static inline void _Read(aType&                ai_Value,
                         m4char_t*&            ao_pPtr,
								 m4char_t*&            ao_pInitialPtr,
								 m4uint32_t&           ao_uiOffset,
								 m4uint32_t&           ao_iBlockSize,
								 M4DataStorage*        ai_poDataStor,
                         m4uint32_t            ai_uiMode,
								 ClDataUnitInterface*& ao_pDataUnitInt,
								 m4bool_t&             ao_iEOF)
{
   M4_ASSERT(ai_uiMode==GEN_IO_MODE_READ);
   M4_ASSERT(ao_pPtr);
   M4_ASSERT(ao_iEOF==M4_FALSE);

   // now we only read if we really need
   // fix bug 0083289

   if ((m4uint32_t)(ao_pPtr-ao_pInitialPtr)>=ao_iBlockSize)
   {
	   // Reached the end of the block
	   ai_poDataStor->GetNext(ao_pDataUnitInt);
       M4_ASSERT(ao_pDataUnitInt != NULL);
       M4_ASSERT(ao_pDataUnitInt->GetType()==M4_DATA_UNIT);
	   
	   if (ao_pDataUnitInt == NULL || ao_pDataUnitInt->GetType() != M4_DATA_UNIT) {
		   ao_iEOF = M4_TRUE;
		   return;
	   }
	   
	   
	   // Habria que hacer una comprobacion de si el DataUnitInterface es de tipo ClDataUnit
	   // En caso de ser FileUnit-> ERROR
	   
	   ao_pPtr= ao_pDataUnitInt->GetBuffer();
	   ao_pInitialPtr=ao_pPtr;
	   ao_iBlockSize=ao_pDataUnitInt->GetSize();
   }

#ifdef _UNIX
   memcpy( (void *)&ai_Value, (void *)ao_pPtr, sizeof(aType) );
#else
   ai_Value=*((aType*)ao_pPtr);
#endif

   ao_pPtr+=sizeof(aType);
   ao_uiOffset+=sizeof(aType);
}


#ifdef M4_VM_CLIENT
m4uint16_t ClDataStoreIODriver::GetArchitect(void)
{
    return m_uiArchitecture;
}
#endif

//////////////////////////////   OTHER   //////////////////////////////

//## end module%3911AA29006D.additionalDeclarations


// Class ClDataStoreIODriver 
















ClDataStoreIODriver::ClDataStoreIODriver (M4DataStorage* ai_poDataStor)
  //## begin ClDataStoreIODriver::ClDataStoreIODriver%2012976625.hasinit preserve=no
  //## end ClDataStoreIODriver::ClDataStoreIODriver%2012976625.hasinit
  //## begin ClDataStoreIODriver::ClDataStoreIODriver%2012976625.initialization preserve=yes
	: m_pPtr(0)
	, m_pInitialPtr(0)
	, m_uiOffset(0)
	, m_iLastSize(-1)
	, m_oTAO(0)
	, m_oPrivateTAO(0)
	, m_poDataStor(ai_poDataStor)
	, m_iMode(GEN_IO_MODE_NONE)
	, m_iBlockSize(0)
	, m_pDataUnitInterface(0)
	, m_pDataUnit(0)
	, m_iLastType(-1)	// INVALID ONE
	, m_iEOF(M4_FALSE)
  //## end ClDataStoreIODriver::ClDataStoreIODriver%2012976625.initialization
{
	//## begin ClDataStoreIODriver::ClDataStoreIODriver%2012976625.body preserve=yes
	//## end ClDataStoreIODriver::ClDataStoreIODriver%2012976625.body
}

ClDataStoreIODriver::ClDataStoreIODriver (m4uint32_t ai_iMode, M4DataStorage* ai_poDataStor, m4uint16_t ai_iServerArchitect)
  //## begin ClDataStoreIODriver::ClDataStoreIODriver%-1934284263.hasinit preserve=no
  //## end ClDataStoreIODriver::ClDataStoreIODriver%-1934284263.hasinit
  //## begin ClDataStoreIODriver::ClDataStoreIODriver%-1934284263.initialization preserve=yes
	: m_pPtr(0)
	, m_pInitialPtr(0)
	, m_uiOffset(0)
	, m_iLastSize(-1)
	, m_oTAO(0)
	, m_oPrivateTAO(0)
	, m_poDataStor(ai_poDataStor)
	, m_iMode(ai_iMode)
	, m_iBlockSize(0)
	, m_pDataUnitInterface(0)
	, m_pDataUnit(0)
	, m_iLastType(-1)	// INVALID ONE
	, m_iEOF(M4_FALSE)
  //## end ClDataStoreIODriver::ClDataStoreIODriver%-1934284263.initialization
{
  //## begin ClDataStoreIODriver::ClDataStoreIODriver%-1934284263.body preserve=yes
	Init(ai_iMode, ai_poDataStor, ai_iServerArchitect) ;
  //## end ClDataStoreIODriver::ClDataStoreIODriver%-1934284263.body
}


ClDataStoreIODriver::~ClDataStoreIODriver ()
{
  //## begin ClDataStoreIODriver::~ClDataStoreIODriver%-290893492.body preserve=yes
   End();
  //## end ClDataStoreIODriver::~ClDataStoreIODriver%-290893492.body
}



//## Other Operations (implementation)
m4return_t ClDataStoreIODriver::Init (m4uint32_t ai_iMode, M4DataStorage* ai_poDataStor, m4uint16_t ai_iServerArchitect)
{
  //## begin ClDataStoreIODriver::Init%-207712432.body preserve=yes

	m4return_t	iRet ;
	m4int8_t	iOrder ;
	m4int8_t	iServerOrder ;


	m_iEOF=M4_TRUE; // In case we fail, all the read operations are aborted

   if (ai_poDataStor==0)
      return M4_ERROR;

   if (m_oTAO)
   {
      delete m_oTAO;
	  m_oTAO = 0;
   }

   if (m_oPrivateTAO)
   {
      delete m_oPrivateTAO;
	  m_oPrivateTAO = 0;
   }

   // jcr. Determino si hay que swapear

   iOrder = M4GetByteOrder( M4_ARCHITECTURE ) ;
   iServerOrder = M4GetByteOrder( ai_iServerArchitect ) ;

   if( iOrder != iServerOrder )
   {
	   m_bHasToSwap = M4_TRUE ;
   }
   else
   {
	   m_bHasToSwap = M4_FALSE ;
   }


   m_iMode=ai_iMode;

   switch (m_iMode)
   {
      case GEN_IO_MODE_READ : // Reading from the IODriver (de-serializing
         m_oTAO=new ClTableAddressOffset(INDEX_BY_OFFSET);
         m_oPrivateTAO=new ClTableAddressOffset(INDEX_BY_OFFSET);

         m_poDataStor=ai_poDataStor;

         m_poDataStor->InitializeIteration();
         iRet=m_poDataStor->GetCurrent(m_pDataUnitInterface);
         if (iRet!=M4_SUCCESS)
            return M4_ERROR;
         if (m_pDataUnitInterface==0)
            return M4_ERROR;

         // Lets see what type is it
         if (((m_iLastType=m_pDataUnitInterface->GetType()))==M4_DATA_UNIT)
         {
				// Buffer
            iRet=m_poDataStor->GetNext(m_pDataUnitInterface);
            if (iRet!=M4_SUCCESS)
               return M4_ERROR;

				m_pPtr=((ClDataUnit*)m_pDataUnitInterface)->GetBuffer();
				m_iBlockSize=((ClDataUnit*)m_pDataUnitInterface)->GetSize();

            if (m_pPtr==0)
               return M4_ERROR; // Otherwise, you are trying to read from a null buffer !!!
			}
			else
			{
				// File, NOTHING...
			}
       	m_iEOF=M4_FALSE;
         break;

      case GEN_IO_MODE_WRITE : // Writing (serializing)
         m_oTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         m_oPrivateTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);

         m_poDataStor=ai_poDataStor;

         m_poDataStor->Clear();
 		
         m_pPtr=0;
         m_iBlockSize=0;
       	m_iEOF=M4_FALSE;
         break;

      case GEN_IO_MODE_APPEND : // Adding to an exixting Driver (adding to a serialized object)
         return M4_ERROR; // NO LA IMPLEMENTO AUN !!!

         m_oTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         m_oPrivateTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         break;

      case GEN_IO_MODE_NONE : // We are calculating sizes and offsets only...
         return M4_ERROR; // NO LA IMPLEMENTO AUN !!!

         m_oTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         m_oPrivateTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         break;
   }

   m_pInitialPtr=m_pPtr;
   m_iLastSize=-1;
   m_uiOffset=0;

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Init%-207712432.body
}

m4return_t ClDataStoreIODriver::Write (m4uint32_t ai_iNumber)
{
  //## begin ClDataStoreIODriver::Write%-291530675.body preserve=yes
	m4uint8_t iNumber = 255 ;


	if( ai_iNumber < 253 )
	{
		iNumber = (m4uint8_t) ai_iNumber ;
	}
	else if( ai_iNumber == 0xFFFFFFFF )
	{
		iNumber = 254 ;
	}
	else if( ai_iNumber == 0x80000000 )
	{
		iNumber = 253 ;
	}

	_Write(iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

	if( iNumber == 255 )
	{
		_Interchange((char *)&ai_iNumber,sizeof(ai_iNumber));
		_Write(ai_iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);
	}

	return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%-291530675.body
}

m4return_t ClDataStoreIODriver::Write (m4int32_t ai_iNumber)
{
  //## begin ClDataStoreIODriver::Write%1146694729.body preserve=yes
   return Write(m4uint32_t(ai_iNumber));
  //## end ClDataStoreIODriver::Write%1146694729.body
}

m4return_t ClDataStoreIODriver::Write (m4uint16_t ai_iNumber)
{
  //## begin ClDataStoreIODriver::Write%2114694442.body preserve=yes

	m4uint8_t iNumber = 255 ;


	if( ai_iNumber < 255 )
	{
		iNumber = (m4uint8_t) ai_iNumber ;
	}

	_Write(iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

	if( iNumber == 255 )
	{
		_Interchange((char *)&ai_iNumber,sizeof(ai_iNumber));
		_Write(ai_iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);
	}

	return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%2114694442.body
}

m4return_t ClDataStoreIODriver::Write (m4int16_t ai_iNumber)
{
  //## begin ClDataStoreIODriver::Write%-1297262124.body preserve=yes
   return Write((m4uint16_t)ai_iNumber);
  //## end ClDataStoreIODriver::Write%-1297262124.body
}

m4return_t ClDataStoreIODriver::Write (m4uint8_t ai_cChar)
{
  //## begin ClDataStoreIODriver::Write%-1261357684.body preserve=yes
   _Write(ai_cChar,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%-1261357684.body
}

m4return_t ClDataStoreIODriver::Write (m4int8_t ai_cChar)
{
  //## begin ClDataStoreIODriver::Write%411937274.body preserve=yes
   return Write((m4uint8_t)ai_cChar);
  //## end ClDataStoreIODriver::Write%411937274.body
}

m4return_t ClDataStoreIODriver::Write (m4float_t ai_fNumber)
{
  //## begin ClDataStoreIODriver::Write%-1907242843.body preserve=yes
   _Interchange((char *)&ai_fNumber,sizeof(ai_fNumber));
   _Write(ai_fNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%-1907242843.body
}

m4return_t ClDataStoreIODriver::Write (m4double_t ai_dNumber)
{
  //## begin ClDataStoreIODriver::Write%890730920.body preserve=yes
   _Interchange((char *)&ai_dNumber,sizeof(ai_dNumber));
   _Write(ai_dNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%890730920.body
}

m4return_t ClDataStoreIODriver::Write (m4bool_t ai_iBoolean)
{
  //## begin ClDataStoreIODriver::Write%1417622219.body preserve=yes
	// Ojo !!! porque el Boolean puede ser un enumerado!!! es decir un int
   //_Interchange((char *)&ai_iBoolean,sizeof(ai_iBoolean));
	if (ai_iBoolean == M4_TRUE)
		_Write((m4uint8_t)1,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);
	else
		_Write((m4uint8_t)0,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%1417622219.body
}

m4return_t ClDataStoreIODriver::Write (void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClDataStoreIODriver::Write%-1825450731.body preserve=yes
   return WritePointer(m_oTAO,ai_pPtr,ao_pExist);
  //## end ClDataStoreIODriver::Write%-1825450731.body
}

m4return_t ClDataStoreIODriver::Write (void* ai_pPtr, m4uint32_t ai_iSize)
{
  //## begin ClDataStoreIODriver::Write%-874616728.body preserve=yes
   m4uint32_t Offset=0;

   M4_ASSERT((m_iMode==GEN_IO_MODE_WRITE) || (m_iMode==GEN_IO_MODE_APPEND));

   Write(ai_iSize);

   if (m_pPtr==0) // First block
   {
	   m_poDataStor->GetNewDataUnit(m_pDataUnit);
      m_pDataUnit->SetSize(MAX_LENG_BLOCK); 	
		m_pPtr=m_pDataUnit->GetBuffer();
      M4_ASSERT(m_pPtr);
      m_pInitialPtr=m_pPtr;

   }

   while (ai_iSize>Offset)
   {
	   // The zone what is pointed by the pointer isn't until processed completely
	
	   if ((ai_iSize-Offset)>(m4uint32_t)((MAX_LENG_BLOCK-(m_pPtr-m_pInitialPtr))))
        {
			// The size of this zone is greater or equal to the rest of place that
		    // is inside the PDU
			memcpy(m_pPtr,ai_pPtr,(MAX_LENG_BLOCK-(m_pPtr-m_pInitialPtr)));
            m4char_t* Aux=(m4char_t*)ai_pPtr;
			Aux+=(MAX_LENG_BLOCK-(m_pPtr-m_pInitialPtr));
            ai_pPtr=Aux;
			Offset+=(m4uint32_t)(MAX_LENG_BLOCK-(m_pPtr-m_pInitialPtr));
			m_uiOffset+=(m4uint32_t)(MAX_LENG_BLOCK-(m_pPtr-m_pInitialPtr));
			m_poDataStor->GetNewDataUnit(m_pDataUnit);
			m_pDataUnit->SetSize(MAX_LENG_BLOCK); 	
			m_pPtr=m_pDataUnit->GetBuffer();
			M4_ASSERT(m_pPtr);
			m_pInitialPtr=m_pPtr;
		}
		else
		{
			memcpy(m_pPtr,ai_pPtr,ai_iSize-Offset);
			m_pPtr+=(ai_iSize-Offset);
			m_uiOffset+=(ai_iSize-Offset);
			Offset+=(ai_iSize-Offset);
		}
   }

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%-874616728.body
}

m4return_t ClDataStoreIODriver::Write (m4pchar_t ai_pString)
{
  //## begin ClDataStoreIODriver::Write%-971968883.body preserve=yes
   M4_ASSERT((m_iMode==GEN_IO_MODE_WRITE) || (m_iMode==GEN_IO_MODE_APPEND));

   if (ai_pString!=0)
      return Write((void*)ai_pString,(m4uint32_t)strlen(ai_pString));
   else
   {
      m4uint32_t NullLength;

      NullLength=TAO_OFFSET_NULL;
      Write(NullLength);
   }

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Write%-971968883.body
}

m4return_t ClDataStoreIODriver::Write (m4pcchar_t ai_pConstString)
{
  //## begin ClDataStoreIODriver::Write%-1525880265.body preserve=yes
   return Write((m4pchar_t)ai_pConstString);
  //## end ClDataStoreIODriver::Write%-1525880265.body
}

m4return_t ClDataStoreIODriver::WritePrivate (void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClDataStoreIODriver::WritePrivate%-315363350.body preserve=yes
   return WritePointer(m_oPrivateTAO,ai_pPtr,ao_pExist);
  //## end ClDataStoreIODriver::WritePrivate%-315363350.body
}

m4return_t ClDataStoreIODriver::WriteFromFile (m4pcchar_t ai_pFileName)
{
  //## begin ClDataStoreIODriver::WriteFromFile%686724163.body preserve=yes
	ClFileUnit * pFileUnit;
	m_poDataStor->GetNewFileUnit(pFileUnit);
	pFileUnit->SetFile((m4char_t *)ai_pFileName);
	return M4_SUCCESS;
  //## end ClDataStoreIODriver::WriteFromFile%686724163.body
}

m4return_t ClDataStoreIODriver::Read (m4uint32_t& ai_iNumber)
{
  //## begin ClDataStoreIODriver::Read%-1688627031.body preserve=yes
	m4uint8_t iNumber = 0;

	
	if (m_iEOF==M4_TRUE)
		return M4_ERROR;

	_Read(iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
	
	if( iNumber == 255 )
	{
		if (m_iEOF==M4_TRUE)
			return M4_ERROR;

		_Read(ai_iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
		_Interchange((char *)&ai_iNumber,sizeof(ai_iNumber));
	}
	else if( iNumber == 254 )
	{
		ai_iNumber = 0xFFFFFFFF ;
	}
	else if( iNumber == 253 )
	{
		ai_iNumber = 0x80000000 ;
	}
	else
	{
		ai_iNumber = (m4uint32_t) iNumber ;
	}
	
	return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%-1688627031.body
}

m4return_t ClDataStoreIODriver::Read (m4int32_t& ai_iNumber)
{
  //## begin ClDataStoreIODriver::Read%-73745664.body preserve=yes
   m4uint32_t uiNumber = ai_iNumber;
   m4return_t iResult = Read(uiNumber);
   ai_iNumber = uiNumber;
   return iResult;
  //## end ClDataStoreIODriver::Read%-73745664.body
}

m4return_t ClDataStoreIODriver::Read (m4uint16_t& ai_iNumber)
{
  //## begin ClDataStoreIODriver::Read%1634679727.body preserve=yes
	m4uint8_t iNumber = 0;

	
	if (m_iEOF==M4_TRUE)
		return M4_ERROR;

	_Read(iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
	
	if( iNumber == 255 )
	{
		if (m_iEOF==M4_TRUE)
			return M4_ERROR;
		
		_Read(ai_iNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
		_Interchange((char *)&ai_iNumber,sizeof(ai_iNumber));
	}
	else
	{
		ai_iNumber = iNumber ;
	}

	return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%1634679727.body
}

m4return_t ClDataStoreIODriver::Read (m4int16_t& ai_iNumber)
{
  //## begin ClDataStoreIODriver::Read%1050031673.body preserve=yes
   m4uint16_t uiNumber = ai_iNumber;
   m4return_t iResult = Read(uiNumber);
   ai_iNumber = uiNumber;
   return iResult;
  //## end ClDataStoreIODriver::Read%1050031673.body
}

m4return_t ClDataStoreIODriver::Read (m4uint8_t& ai_cChar)
{
  //## begin ClDataStoreIODriver::Read%-1048403713.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   _Read(ai_cChar,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%-1048403713.body
}

m4return_t ClDataStoreIODriver::Read (m4int8_t& ai_cChar)
{
  //## begin ClDataStoreIODriver::Read%946191249.body preserve=yes
   m4uint8_t cChar = ai_cChar;
   m4return_t iResult = Read(cChar);
   ai_cChar = cChar;
   return iResult;
  //## end ClDataStoreIODriver::Read%946191249.body
}

m4return_t ClDataStoreIODriver::Read (m4float_t& ai_fNumber)
{
  //## begin ClDataStoreIODriver::Read%558489178.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   _Read(ai_fNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
   _Interchange((char *)&ai_fNumber,sizeof(ai_fNumber));
   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%558489178.body
}

m4return_t ClDataStoreIODriver::Read (m4double_t& ai_dNumber)
{
  //## begin ClDataStoreIODriver::Read%-674859141.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   _Read(ai_dNumber,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
   _Interchange((char *)&ai_dNumber,sizeof(ai_dNumber));
   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%-674859141.body
}

m4return_t ClDataStoreIODriver::Read (m4bool_t& ai_iBoolean)
{
  //## begin ClDataStoreIODriver::Read%-2135133207.body preserve=yes
	m4uint8_t iAux;

   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

	_Read(iAux,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
	// Ojo !!! porque el Boolean puede ser un enumerado!!! es decir un int
    //_Interchange((char *)&ai_iBoolean,sizeof(ai_iBoolean));

	if (iAux)
		ai_iBoolean = M4_TRUE;
	else
		ai_iBoolean = M4_FALSE;

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%-2135133207.body
}

m4return_t ClDataStoreIODriver::Read (void** ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClDataStoreIODriver::Read%-35277620.body preserve=yes
   return ReadPointer(m_oTAO,ai_pPtr,ao_pExist);
  //## end ClDataStoreIODriver::Read%-35277620.body
}

m4return_t ClDataStoreIODriver::Read (void* ao_pPtr, m4uint32_t& ao_iSize)
{
  //## begin ClDataStoreIODriver::Read%-1744485436.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   if (Read(ao_iSize)!=M4_SUCCESS)
      return M4_ERROR;

   ReadPtr(ao_pPtr,ao_iSize);

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%-1744485436.body
}

m4return_t ClDataStoreIODriver::Read (m4pchar_t ao_pString, m4uint32_t ai_iMaxLen)
{
  //## begin ClDataStoreIODriver::Read%1710873989.body preserve=yes
   m4uint32_t Size;

   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   if (Read(Size)!=M4_SUCCESS)
      return M4_ERROR;

   if (ai_iMaxLen>0 &&
       Size>ai_iMaxLen)
      return M4_ERROR;

   ReadPtr(ao_pString,Size);

   ao_pString[Size]=0;


   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%1710873989.body
}

m4return_t ClDataStoreIODriver::Read (m4pcchar_t ao_pConstString, m4uint32_t ai_iMaxLen)
{
  //## begin ClDataStoreIODriver::Read%-1845492164.body preserve=yes
   m4uint32_t Size;

   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   if (Read(Size)!=M4_SUCCESS)
      return M4_ERROR;

   if (ai_iMaxLen>0 &&
       Size>ai_iMaxLen)
      return M4_ERROR;

   ReadPtr((void*)ao_pConstString,Size);

   ((m4pchar_t)ao_pConstString)[Size]=0;


   return M4_SUCCESS;
  //## end ClDataStoreIODriver::Read%-1845492164.body
}

m4return_t ClDataStoreIODriver::ReadPrivate (void** ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClDataStoreIODriver::ReadPrivate%524631985.body preserve=yes
   return ReadPointer(m_oPrivateTAO,ai_pPtr,ao_pExist);
  //## end ClDataStoreIODriver::ReadPrivate%524631985.body
}

m4return_t ClDataStoreIODriver::ReadSize (m4uint32_t& ao_iSize)
{
  //## begin ClDataStoreIODriver::ReadSize%-1712516114.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   if (Read(ao_iSize)!=M4_SUCCESS)
      return M4_ERROR;

   m_iLastSize=ao_iSize;

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::ReadSize%-1712516114.body
}

m4return_t ClDataStoreIODriver::ReadBuffer (void* ao_pPtr)
{
  //## begin ClDataStoreIODriver::ReadBuffer%1743208874.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   if (m_iLastSize<0)
      return M4_ERROR;

   ReadPtr(ao_pPtr,m_iLastSize);

   m_iLastSize=-1;

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::ReadBuffer%1743208874.body
}

m4return_t ClDataStoreIODriver::ReadToFile (m4pchar_t ao_pFileName)
{
  //## begin ClDataStoreIODriver::ReadToFile%1635854610.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

	m_poDataStor->GetNext(m_pDataUnitInterface);
	M4_ASSERT((m_pDataUnitInterface->GetType())==M4_FILE_UNIT);

   if (((ClFileUnit *)m_pDataUnitInterface)->GetTotalSize()==M4PDUS_FILEUNIT_UNDEFINED_FILE)
      return M4_ERROR;

	strcpy(ao_pFileName,((ClFileUnit *)m_pDataUnitInterface)->GetFileName());

	/*
	Para que el ciclo de vida sea paralelo al fileiod
	Hay que decrementar el interes fuera
	*/
    ClFileManager::Instance()->SetInterestInFile(ao_pFileName);

	return M4_SUCCESS;
  //## end ClDataStoreIODriver::ReadToFile%1635854610.body
}

m4bool_t ClDataStoreIODriver::Eof (void )
{
  //## begin ClDataStoreIODriver::Eof%-1753696491.body preserve=yes
   return m_iEOF;
  //## end ClDataStoreIODriver::Eof%-1753696491.body
}

m4bool_t ClDataStoreIODriver::Flush (void )
{
  //## begin ClDataStoreIODriver::Flush%-1309285846.body preserve=yes
   if (m_pPtr>m_pInitialPtr && m_iMode == GEN_IO_MODE_WRITE)
	{
		m_pDataUnit->SetSize((m4uint32_t)(m_pPtr-m_pInitialPtr));
		m_pPtr = 0 ;
	}

    return M4_TRUE;
  //## end ClDataStoreIODriver::Flush%-1309285846.body
}

void ClDataStoreIODriver::Reset (void )
{
  //## begin ClDataStoreIODriver::Reset%1265143797.body preserve=yes
   m_pPtr=m_pInitialPtr;
	m_iEOF=M4_FALSE;
  //## end ClDataStoreIODriver::Reset%1265143797.body
}

m4return_t ClDataStoreIODriver::StoreAddress (void* ai_pPtr)
{
  //## begin ClDataStoreIODriver::StoreAddress%-957016152.body preserve=yes
   return StoreAddressPointer(m_oTAO,ai_pPtr);
  //## end ClDataStoreIODriver::StoreAddress%-957016152.body
}

m4return_t ClDataStoreIODriver::StoreAddress (void* ai_pPtr, m4uint32_t ai_iOffset)
{
  //## begin ClDataStoreIODriver::StoreAddress%447084782.body preserve=yes
   return StoreAddressPointer(m_oTAO,ai_pPtr,ai_iOffset);
  //## end ClDataStoreIODriver::StoreAddress%447084782.body
}

m4return_t ClDataStoreIODriver::StoreAddressPrivate (void* ai_pPtr)
{
  //## begin ClDataStoreIODriver::StoreAddressPrivate%-1412875484.body preserve=yes
   return StoreAddressPointer(m_oPrivateTAO,ai_pPtr);
  //## end ClDataStoreIODriver::StoreAddressPrivate%-1412875484.body
}

m4return_t ClDataStoreIODriver::StoreAddressPrivate (void* ai_pPtr, m4uint32_t ai_iOffset)
{
  //## begin ClDataStoreIODriver::StoreAddressPrivate%217573661.body preserve=yes
   return StoreAddressPointer(m_oPrivateTAO,ai_pPtr,ai_iOffset);
  //## end ClDataStoreIODriver::StoreAddressPrivate%217573661.body
}

m4uint32_t ClDataStoreIODriver::GetActualOffset (void )
{
  //## begin ClDataStoreIODriver::GetActualOffset%1235932099.body preserve=yes
	return m_uiOffset;
  //## end ClDataStoreIODriver::GetActualOffset%1235932099.body
}

m4return_t ClDataStoreIODriver::ResetPrivateInfo (void )
{
  //## begin ClDataStoreIODriver::ResetPrivateInfo%-1813344334.body preserve=yes
   m_oPrivateTAO->Reset();

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::ResetPrivateInfo%-1813344334.body
}

void ClDataStoreIODriver::End (void )
{
  //## begin ClDataStoreIODriver::End%-161200245.body preserve=yes
	m4bool_t bFlushIt=M4_FALSE;

	if (m_oTAO != 0)
	{
		delete m_oTAO;
		m_oTAO = 0;

		bFlushIt=M4_TRUE;

 	}

	if (m_oPrivateTAO != 0)
	{
		delete m_oPrivateTAO;
		m_oPrivateTAO = 0;

		bFlushIt=M4_TRUE;

 	}

	if (bFlushIt)
		Flush();
  //## end ClDataStoreIODriver::End%-161200245.body
}

m4return_t ClDataStoreIODriver::WritePointer (ClTableAddressOffset* ai_pTable, void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClDataStoreIODriver::WritePointer%-1690783765.body preserve=yes
   m4uint32_t Offset;

   M4_ASSERT((m_iMode==GEN_IO_MODE_WRITE) || (m_iMode==GEN_IO_MODE_APPEND));

   if (ai_pPtr==0)
   {
      Offset=TAO_OFFSET_NULL;
      _Interchange((char *)&Offset,sizeof(Offset));
      _Write(Offset,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

      if (ao_pExist!=0)
         *ao_pExist=M4_TRUE;

      return M4_SUCCESS;
   }

   Offset=ai_pTable->GetOffset(ai_pPtr);

   if (Offset!=0)
   {
      // Address found (previously used (serialized)...)

      // Visited previously, OK
      if (ao_pExist!=0)
         *ao_pExist=M4_TRUE;
   }
   else
   {
      // First time, add it to the table (if the user wants it), points to the next free
      // space...
      Offset=m_uiOffset+sizeof(Offset);

      if (ao_pExist!=0)
      {
         ai_pTable->Add(ai_pPtr,
                     Offset); // This is the place where we need to put the value pointed by ai_iPtr !!!
         *ao_pExist=M4_FALSE;
      }
   }

   _Interchange((char *)&Offset,sizeof(Offset));
   _Write(Offset,m_pPtr,m_pInitialPtr,m_uiOffset,m_poDataStor,m_iMode,m_pDataUnit);

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::WritePointer%-1690783765.body
}

m4return_t ClDataStoreIODriver::ReadPointer (ClTableAddressOffset* ai_pTable, void** ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClDataStoreIODriver::ReadPointer%71432984.body preserve=yes
   if (m_iEOF==M4_TRUE)
      return M4_ERROR;

   m4uint32_t Offset=0;

   M4_ASSERT(m_iMode==GEN_IO_MODE_READ);

   _Read(Offset,m_pPtr,m_pInitialPtr,m_uiOffset,m_iBlockSize,m_poDataStor,m_iMode,m_pDataUnitInterface,m_iEOF);
   _Interchange((char *)&Offset,sizeof(Offset));

   if (Offset==TAO_OFFSET_NULL)
   {
      *ai_pPtr=0;
      if (ao_pExist!=NULL)
         *ao_pExist=M4_TRUE; // Protocolo para distinguir con respecto a offsetnext

      return M4_SUCCESS;
   }
   else
      if (Offset<m_uiOffset)
      {
         // The ptr was used previously, so we can get it from the TAO
         // Read the offset
         *ai_pPtr=ai_pTable->GetAddress(Offset);

         if ((*ai_pPtr)==NULL)
            return M4_ERROR; // This case shoudn't be possible...

         if (ao_pExist!=NULL)
            *ao_pExist=M4_TRUE;

         return M4_SUCCESS;
      }
      else
         {
            *ai_pPtr=0;
            if (ao_pExist!=NULL)
               *ao_pExist=M4_FALSE;

            // PODEMOS INSERTAR EN LA TABLA AHORA, O MAS TARDE EN MAIN...
            // Por ahora, dejamos al user hacerlo según su criterio

            return M4_SUCCESS;
         }

   return M4_SUCCESS;
  //## end ClDataStoreIODriver::ReadPointer%71432984.body
}

m4return_t ClDataStoreIODriver::StoreAddressPointer (ClTableAddressOffset* ai_pTable, void* ai_pPtr)
{
  //## begin ClDataStoreIODriver::StoreAddressPointer%311046174.body preserve=yes
   if (ai_pPtr!=0)
   {
      if (m_iMode==GEN_IO_MODE_READ)
         ai_pTable->Add(m_uiOffset,
                     ai_pPtr); // This is the place where we need to put the value pointed by ai_iPtr !!!
      else
         ai_pTable->Add(ai_pPtr,
                     m_uiOffset); // This is the place where we need to put the value pointed by ai_iPtr !!!

      return M4_SUCCESS;
   }
   else
      return M4_ERROR;
  //## end ClDataStoreIODriver::StoreAddressPointer%311046174.body
}

m4return_t ClDataStoreIODriver::StoreAddressPointer (ClTableAddressOffset* ai_pTable, void* ai_pPtr, m4uint32_t ai_iOffset)
{
  //## begin ClDataStoreIODriver::StoreAddressPointer%-145059750.body preserve=yes
   if (m_iMode!=GEN_IO_MODE_READ)
      return M4_ERROR;

   if (ai_pPtr!=0)
   {
      ai_pTable->Add(ai_iOffset,
                  ai_pPtr); // This is the place where we need to put the value pointed by ai_iPtr !!!

      return M4_SUCCESS;
   }
   else
      return M4_ERROR;
  //## end ClDataStoreIODriver::StoreAddressPointer%-145059750.body
}

void ClDataStoreIODriver::ReadPtr (void* ao_pPtr, m4uint32_t ai_iSize)
{
  //## begin ClDataStoreIODriver::ReadPtr%1677070765.body preserve=yes
   m4uint32_t Offset=0;
   m4char_t* DestPtr;

   DestPtr=(m4char_t*)ao_pPtr;

   while (ai_iSize>Offset)
   {
	  // The string isn't until processed completely

     M4_ASSERT(m_iEOF==M4_FALSE);

	  if ((ai_iSize-Offset)>=(m_iBlockSize-(m_pPtr-m_pInitialPtr)))
	  {
	      // The length of the string is greater or equal to the rest of place that
	      // is inside the Data Storage
		  memcpy(DestPtr,m_pPtr,(m_iBlockSize-(m_pPtr-m_pInitialPtr)));
		  DestPtr+=(m_iBlockSize-(m_pPtr-m_pInitialPtr));
		  Offset+=(m4uint32_t)(m_iBlockSize-(m_pPtr-m_pInitialPtr));
		  m_uiOffset+=(m4uint32_t)(m_iBlockSize-(m_pPtr-m_pInitialPtr));
		  m_poDataStor->GetNext(m_pDataUnitInterface);
		  if (m_pDataUnitInterface==0)
		  {
			  m_iEOF=M4_TRUE;

			  return;
		  }
        M4_ASSERT(m_pDataUnitInterface->GetType()==M4_DATA_UNIT);

		// Habria que hacer una comprobacion de si el DataUnitInterface es de tipo ClDataUnit
		// En caso de ser FileUnit-> ERROR

		m_pPtr=m_pDataUnitInterface->GetBuffer();
		m_pInitialPtr=m_pPtr;
		m_iBlockSize=m_pDataUnitInterface->GetSize();
	  }
	  else
	  {
		  memcpy(DestPtr,m_pPtr,ai_iSize-Offset);
		  m_pPtr+=(ai_iSize-Offset);
		  m_uiOffset+=(ai_iSize-Offset);
		  Offset+=(ai_iSize-Offset);
	  }
   }
  //## end ClDataStoreIODriver::ReadPtr%1677070765.body
}

void ClDataStoreIODriver::_Interchange (m4char_t* ai_vector, m4int32_t ai_tamano)
{
  //## begin ClDataStoreIODriver::_Interchange%-1345979075.body preserve=yes
	m4uint8_t aux;
	m4int32_t i;


	if( m_bHasToSwap == M4_TRUE )
	{
		// Optimize
		for (i=0; i<(ai_tamano>>1); ++i)
		{
		  aux=ai_vector[i];
		  ai_vector[i]=ai_vector[(ai_tamano-1)-i];
		  ai_vector[(ai_tamano-1)-i]=aux;
		}
	}
  //## end ClDataStoreIODriver::_Interchange%-1345979075.body
}

// Additional Declarations
  //## begin ClDataStoreIODriver%3911AA23034A.declarations preserve=yes
  //## end ClDataStoreIODriver%3911AA23034A.declarations

//## begin module%3911AA29006D.epilog preserve=yes
//## end module%3911AA29006D.epilog
