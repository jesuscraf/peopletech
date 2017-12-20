//## begin module%3911AA29017B.cm preserve=no
//## end module%3911AA29017B.cm

//## begin module%3911AA29017B.cp preserve=no
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
//## end module%3911AA29017B.cp

//## Module: clfileiod%3911AA29017B; Package body
//## Subsystem: M4Serial::src%3911B1010152
//## Source file: d:\new\m4serial\src\clfileiod.cpp

//## begin module%3911AA29017B.additionalIncludes preserve=no
//## end module%3911AA29017B.additionalIncludes

//## begin module%3911AA29017B.includes preserve=yes
//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                ClFileIOD.cpp
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
//    File IO Driver module
//    Multi-architecture support added
//
//
//==============================================================================

#include "m4stl.hpp"
#include "m4filemanager.hpp"
#include "clfileiod.hpp"
#include "basiclog.hpp"
#include "serres.hpp"


#ifndef _WINDOWS
#include <errno.h>
#endif

#define TAM_TEMP_BUFFER            (16*1024)
#define M4SERIAL_CHECK_ERROR( cond, result, id, error )			do { if( cond ) { m_iLastErrorCode = error ; SetLastErrorDescr( strerror( errno ) ) ; BL_ERRORF( id, GetLastErrorDescr() << m_pcFileName ) ; return( result ) ; } } while( 0 )		



//## end module%3911AA29017B.includes

// cltadof
#include <cltadof.hpp>
// clgeniod
#include <clgeniod.hpp>
// m4types
#include <m4types.hpp>
// stdio
#include <stdio.h>
#include "m4unicode.hpp"

//## begin module%3911AA29017B.declarations preserve=no
//## end module%3911AA29017B.declarations

//## begin module%3911AA29017B.additionalDeclarations preserve=yes

//////////////////////////////   WRITE   //////////////////////////////

template <class aType>
static inline m4return_t _Write(aType ai_Value,
                                FILE* ai_pFile, ClFileIODriver *ai_pFileIODriver)
{
   size_t iWritten;

   ClFileIODriver::_Translate((m4char_t *)&ai_Value,sizeof(ai_Value));

   if ((iWritten=ai_pFileIODriver->FWrite(&ai_Value,1,sizeof(aType),ai_pFile))!=sizeof(aType))
   {
	    ai_pFileIODriver->ResetWriteError();
		return M4_ERROR;
   }

   return M4_SUCCESS;
}


//////////////////////////////   READ   //////////////////////////////

template <class aType>
static inline m4return_t _Read(aType& ai_Value,
                               FILE*  ai_pFile, ClFileIODriver *ai_pFileIODriver)
{
   size_t iRead;

   if ((iRead=ai_pFileIODriver->FRead(&ai_Value,1,sizeof(aType),ai_pFile))!=sizeof(aType))
   {
	   ai_pFileIODriver->ResetReadError();
	   return M4_ERROR;
   }

   ClFileIODriver::_Translate((m4char_t *)&ai_Value,sizeof(ai_Value));

   return M4_SUCCESS;
}

//## end module%3911AA29017B.additionalDeclarations


// Class ClFileIODriver 






ClFileIODriver::ClFileIODriver (void )
  //## begin ClFileIODriver::ClFileIODriver%-409652940.hasinit preserve=no
  //## end ClFileIODriver::ClFileIODriver%-409652940.hasinit
  //## begin ClFileIODriver::ClFileIODriver%-409652940.initialization preserve=yes
  //## end ClFileIODriver::ClFileIODriver%-409652940.initialization
{
  //## begin ClFileIODriver::ClFileIODriver%-409652940.body preserve=yes
   m_pFile=0;
   m_iLastSize=0;
   m_oTAO=0;
   m_pcFileName=NULL;
  //## end ClFileIODriver::ClFileIODriver%-409652940.body
}

ClFileIODriver::ClFileIODriver (m4uint32_t ai_iMode, char* ai_pFileName, m4bool_t ai_bGenErrorLog)
  //## begin ClFileIODriver::ClFileIODriver%1006753913.hasinit preserve=no
  //## end ClFileIODriver::ClFileIODriver%1006753913.hasinit
  //## begin ClFileIODriver::ClFileIODriver%1006753913.initialization preserve=yes
  //## end ClFileIODriver::ClFileIODriver%1006753913.initialization
{
  //## begin ClFileIODriver::ClFileIODriver%1006753913.body preserve=yes
	m_pFile=0;
	m_oTAO=0;
   m_pcFileName=NULL;
	Init(ai_iMode,ai_pFileName,ai_bGenErrorLog);
  //## end ClFileIODriver::ClFileIODriver%1006753913.body
}


ClFileIODriver::~ClFileIODriver (void )
{
  //## begin ClFileIODriver::~ClFileIODriver%-420270528.body preserve=yes
   End(M4_FALSE);
  //## end ClFileIODriver::~ClFileIODriver%-420270528.body
}



//## Other Operations (implementation)
m4return_t ClFileIODriver::Init (m4uint32_t ai_iMode, char* ai_pFileName, m4bool_t ai_bGenErrorLog)
{

   char		cControl = -1 ;
   size_t	iWrite = 0 ;


  //## begin ClFileIODriver::Init%1969669876.body preserve=yes
   if (ai_pFileName==NULL || *ai_pFileName == '\0')
   {
		m_iLastErrorCode=GEN_ERR_INIT_WRONG_PARAMS;
      SETCODE(M4_SERIAL_WRONG_PARAMS, ERRORLOG);

		return M4_ERROR;
   }
	
   if (m_pcFileName != NULL)
   {
       delete[] m_pcFileName;
       m_pcFileName = NULL;
   }

   size_t iSize=strlen(ai_pFileName)+1;
   m_pcFileName=new m4char_t[iSize];
   if (m_pcFileName==NULL)
   {
		m_iLastErrorCode = GEN_ERR_INIT_WRONG_PARAMS;
      SETCODE(M4_SERIAL_WRONG_PARAMS, ERRORLOG);

		return M4_ERROR;
   }

   strcpy(m_pcFileName,ai_pFileName);

   if (m_pFile!=0)
   {
       fclose(m_pFile);

       m_pFile=0;
   }

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;

   switch (ai_iMode)
   {
      case GEN_IO_MODE_READ:
         m_pFile = M4Fopen( ai_pFileName, M4UniReadBinary, eEncoding ) ;
      break;

      case GEN_IO_MODE_WRITE:
         m_pFile = M4Fopen( ai_pFileName, M4UniWriteBinary, eEncoding ) ;
         break;

      case GEN_IO_MODE_REWRITE:
         m_pFile = M4Fopen( ai_pFileName, M4UniReadWriteBinary, eEncoding ) ;

         /*
         Se escribe 1 byte. Esta argucia es necesaria porque cuando se abre en modo
         lectura escritura un fichero, si lo primero que se realiza es una lectura
         ya no se puede volver a escribir. No sabemos si esto es un bug de la
         implementación de fread en NT!!!!
         */
		 /* Bug 0257386
		 Hay que comprobar que el fichero se haya podido abrir.
		 */
         if( m_pFile != NULL )
		 {
		     iWrite = fwrite( &cControl, 1, sizeof( char ), m_pFile ) ;
		     M4SERIAL_CHECK_ERROR( iWrite != sizeof( char ), M4_ERROR, M4_SERIAL_CANT_WRITE_TO_FILE, FILE_ERR_WRITE_FILE ) ;
		 }
         break;

      case GEN_IO_MODE_APPEND:
         // UNICODE FILE revised
         m_pFile = M4Fopen( ai_pFileName, M4UniAppendBinary, eEncoding ) ;
         break;
   }

	if (m_pFile==0)
	{
		m_iLastErrorCode = FILE_ERR_OPEN_FILE;
		SetLastErrorDescr(strerror(errno));

		if (ai_bGenErrorLog)
			BL_ERRORF(M4_SERIAL_CANT_OPEN_FILE, GetLastErrorDescr() << ai_pFileName);

		return M4_ERROR;
	}

   if (m_oTAO!=0)
	{
      delete m_oTAO;

      m_oTAO=0;
	}

   switch (ai_iMode)
   {
      case GEN_IO_MODE_READ : // Reading from the IODriver (de-serializing
         m_oTAO=new ClTableAddressOffset(INDEX_BY_OFFSET);
         break;

      case GEN_IO_MODE_WRITE : // Writing (serializing)
      case GEN_IO_MODE_REWRITE :
         m_oTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         break;

      case GEN_IO_MODE_APPEND : // Adding to an exixting Driver (adding to a serialized object)
         m_oTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         break;

      case GEN_IO_MODE_NONE : // We are calculating sizes and offsets only...
         m_oTAO=new ClTableAddressOffset(INDEX_BY_ADDRESS);
         break;
   }

	if (m_oTAO==0)
	{
		fclose(m_pFile);

		m_pFile=0;

		m_iLastErrorCode = GEN_ERR_OUT_OF_MEMORY;
        SETCODE(M4_SERIAL_OUT_OF_MEMORY, ERRORLOG);

		return M4_ERROR;
	}

   m_iMode=ai_iMode;
   m_iLastSize=-1;

   return M4_SUCCESS;
  //## end ClFileIODriver::Init%1969669876.body
}

void	ClFileIODriver::Reset( void )
{
	if( m_oTAO != NULL )
	{
		m_oTAO->Reset() ;
	}
}


FILE* ClFileIODriver::File (void )
{
  //## begin ClFileIODriver::File%758901826.body preserve=yes
   return m_pFile;
  //## end ClFileIODriver::File%758901826.body
}

void ClFileIODriver::ResetWriteError (void )
{
  //## begin ClFileIODriver::ResetWriteError%-1965503359.body preserve=yes
	m_iLastErrorCode = FILE_ERR_WRITE_FILE;
	SetLastErrorDescr(strerror(errno));

	BL_ERRORF(M4_SERIAL_CANT_WRITE_TO_FILE, GetLastErrorDescr() << m_pcFileName);
  //## end ClFileIODriver::ResetWriteError%-1965503359.body
}

void ClFileIODriver::ResetReadError (void )
{
  //## begin ClFileIODriver::ResetReadError%2133215742.body preserve=yes
	m_iLastErrorCode = FILE_ERR_READ_FILE;
	SetLastErrorDescr(strerror(errno));

	BL_ERRORF(M4_SERIAL_CANT_READ_FROM_FILE, GetLastErrorDescr() << m_pcFileName);
  //## end ClFileIODriver::ResetReadError%2133215742.body
}

m4return_t ClFileIODriver::Write (m4uint32_t ai_iNumber)
{
  //## begin ClFileIODriver::Write%-1016503413.body preserve=yes
   return _Write(ai_iNumber,m_pFile,this);
  //## end ClFileIODriver::Write%-1016503413.body
}

m4return_t ClFileIODriver::Write (m4int32_t ai_iNumber)
{
  //## begin ClFileIODriver::Write%715853397.body preserve=yes
   _Write(ai_iNumber,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%715853397.body
}

m4return_t ClFileIODriver::Write (m4uint16_t ai_iNumber)
{
  //## begin ClFileIODriver::Write%-1528002629.body preserve=yes
   _Write(ai_iNumber,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%-1528002629.body
}

m4return_t ClFileIODriver::Write (m4int16_t ai_iNumber)
{
  //## begin ClFileIODriver::Write%-656974324.body preserve=yes
   _Write(ai_iNumber,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%-656974324.body
}

m4return_t ClFileIODriver::Write (m4uint8_t ai_cChar)
{
  //## begin ClFileIODriver::Write%2041861160.body preserve=yes
   _Write(ai_cChar,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%2041861160.body
}

m4return_t ClFileIODriver::Write (m4int8_t ai_cChar)
{
  //## begin ClFileIODriver::Write%2093728905.body preserve=yes
   _Write(ai_cChar,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%2093728905.body
}

m4return_t ClFileIODriver::Write (m4float_t ai_fNumber)
{
  //## begin ClFileIODriver::Write%767697442.body preserve=yes
   _Write(ai_fNumber,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%767697442.body
}

m4return_t ClFileIODriver::Write (m4double_t ai_dNumber)
{
  //## begin ClFileIODriver::Write%-1997159106.body preserve=yes
   _Write(ai_dNumber,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%-1997159106.body
}

m4return_t ClFileIODriver::Write (m4bool_t ai_iBoolean)
{
  //## begin ClFileIODriver::Write%444285371.body preserve=yes
   _Write((m4uint8_t)ai_iBoolean,m_pFile,this);

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%444285371.body
}

m4return_t ClFileIODriver::Write (void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClFileIODriver::Write%264418933.body preserve=yes
   m4uint32_t Offset;

   if (ai_pPtr==0)
   {
      Offset=TAO_OFFSET_NULL;

      Write(Offset);

      if (ao_pExist!=0)
         *ao_pExist=M4_TRUE;

      return M4_SUCCESS;
   }

   Offset=m_oTAO->GetOffset(ai_pPtr);

   if (Offset!=0)
   {
      // Address found (previously used (serialized)...)

      // Visited previously, OK
      if (ao_pExist!=0)
         *ao_pExist=M4_TRUE;

      Write(Offset);
   }
   else
   {
      // First time, add it to the table (if the user wants it), points to the next free
      // space...
      Offset=(m4uint32_t)(ftell(m_pFile))+sizeof(Offset);

      if (ao_pExist!=0)
      {
         m_oTAO->Add(ai_pPtr,
                     Offset); // This is the place where we need to put the value pointed by ai_iPtr !!!
         *ao_pExist=M4_FALSE;
      }

      Write(Offset);
   }

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%264418933.body
}

m4return_t ClFileIODriver::Write (void* ai_pPtr, m4uint32_t ai_iSize)
{
  //## begin ClFileIODriver::Write%-607996546.body preserve=yes
   Write(ai_iSize);

   if (ai_iSize > 0)
      if (FWrite(ai_pPtr, 1 , ai_iSize, m_pFile) != ai_iSize)
	  {
		  m_iLastErrorCode = FILE_ERR_WRITE_FILE;
		  SetLastErrorDescr(strerror(errno));

		  BL_ERRORF(M4_SERIAL_CANT_WRITE_TO_FILE, GetLastErrorDescr() << m_pcFileName);

		  return M4_ERROR;
	  }

   return M4_SUCCESS;
  //## end ClFileIODriver::Write%-607996546.body
}

m4return_t ClFileIODriver::Write (m4pchar_t ai_pString)
{
  //## begin ClFileIODriver::Write%2124946526.body preserve=yes
   if (ai_pString==0)
      return Write((m4uint32_t)0);
   else
      return Write((void*)ai_pString,(m4uint32_t)strlen(ai_pString));
  //## end ClFileIODriver::Write%2124946526.body
}

m4return_t ClFileIODriver::Write (m4pcchar_t ai_pConstString)
{
  //## begin ClFileIODriver::Write%529186606.body preserve=yes
    return Write((m4pchar_t)ai_pConstString);
  //## end ClFileIODriver::Write%529186606.body
}

m4return_t ClFileIODriver::WritePrivate (void* ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClFileIODriver::WritePrivate%933780840.body preserve=yes
   // Por ahora
   return Write(ai_pPtr,ao_pExist);
  //## end ClFileIODriver::WritePrivate%933780840.body
}

m4return_t ClFileIODriver::WriteFromFile (m4pcchar_t ai_pFileName)
{
  //## begin ClFileIODriver::WriteFromFile%-592305341.body preserve=yes
   FILE* pFile;

   // UNICODE FILE
   eUniFileEncodingType_t eEncoding = M4UniANSI ;
   pFile = M4Fopen( ai_pFileName, M4UniReadBinary, eEncoding ) ;

   if (pFile==0)
   {
		m_iLastErrorCode = FILE_ERR_OPEN_FILE;
		SetLastErrorDescr(strerror(errno));

	    BL_ERRORF(M4_SERIAL_CANT_OPEN_FILE, GetLastErrorDescr() << m_pcFileName);

		return M4_ERROR;
   }

   m4uint8_t TempBuffer[TAM_TEMP_BUFFER];

   if (fseek(pFile,0,SEEK_END))
   {
	   m_iLastErrorCode = FILE_ERR_SEEK_FILE;
	   SetLastErrorDescr(strerror(errno));
	
	   BL_ERRORF(M4_SERIAL_CANT_SEEK_FILE, GetLastErrorDescr() << m_pcFileName);

	   return M4_ERROR;
   }

   m4uint32_t iFileSize;

   iFileSize=ftell(pFile);

   if (fseek(pFile,0,SEEK_SET))
   {
	   m_iLastErrorCode = FILE_ERR_SEEK_FILE;
	   SetLastErrorDescr(strerror(errno));
	
	   BL_ERRORF(M4_SERIAL_CANT_SEEK_FILE, GetLastErrorDescr() << m_pcFileName);
	
	   return M4_ERROR;
   }


   // Write the extension of the file name (used when deserializing)

   m4char_t pzAuxFileExtension[256];

   if (ClFileName(ai_pFileName).GetFileExtension(pzAuxFileExtension)==M4_ERROR)
   {
      fclose(pFile);

      return M4_ERROR;
   }

   if (Write(pzAuxFileExtension)!=M4_SUCCESS)
   {
      fclose(pFile);

      return M4_ERROR;
   }

   // Write it the same way as a binary buffer

   if (Write(iFileSize)!=M4_SUCCESS)
   {
      fclose(pFile);

      return M4_ERROR;
   }

   while (!feof(pFile))
   {
      size_t iRead;
      iRead=fread(TempBuffer,1,sizeof(TempBuffer),pFile);

      if (iRead>0)
         if (FWrite(TempBuffer,1,iRead,m_pFile)!=iRead)
         {
			m_iLastErrorCode = FILE_ERR_WRITE_FILE;
			SetLastErrorDescr(strerror(errno));

			BL_ERRORF(M4_SERIAL_CANT_WRITE_TO_FILE, GetLastErrorDescr() << m_pcFileName);

		    fclose(pFile);
            return M4_ERROR;
         }
   }

   fclose(pFile);

   return M4_SUCCESS;
  //## end ClFileIODriver::WriteFromFile%-592305341.body
}

m4return_t ClFileIODriver::Read (m4uint32_t& ai_iNumber)
{
  //## begin ClFileIODriver::Read%52170989.body preserve=yes
   return _Read(ai_iNumber,m_pFile,this);
  //## end ClFileIODriver::Read%52170989.body
}

m4return_t ClFileIODriver::Read (m4int32_t& ai_iNumber)
{
  //## begin ClFileIODriver::Read%131008638.body preserve=yes
   return _Read(ai_iNumber,m_pFile,this);
  //## end ClFileIODriver::Read%131008638.body
}

m4return_t ClFileIODriver::Read (m4uint16_t& ai_iNumber)
{
  //## begin ClFileIODriver::Read%-10040338.body preserve=yes
   return _Read(ai_iNumber,m_pFile,this);
  //## end ClFileIODriver::Read%-10040338.body
}

m4return_t ClFileIODriver::Read (m4int16_t& ai_iNumber)
{
  //## begin ClFileIODriver::Read%-1033068938.body preserve=yes
   return _Read(ai_iNumber,m_pFile,this);
  //## end ClFileIODriver::Read%-1033068938.body
}

m4return_t ClFileIODriver::Read (m4uint8_t& ai_cChar)
{
  //## begin ClFileIODriver::Read%-132024382.body preserve=yes
   return _Read(ai_cChar,m_pFile,this);
  //## end ClFileIODriver::Read%-132024382.body
}

m4return_t ClFileIODriver::Read (m4int8_t& ai_cChar)
{
  //## begin ClFileIODriver::Read%-487451590.body preserve=yes
   return _Read(ai_cChar,m_pFile,this);
  //## end ClFileIODriver::Read%-487451590.body
}

m4return_t ClFileIODriver::Read (m4float_t& ai_fNumber)
{
  //## begin ClFileIODriver::Read%-696517996.body preserve=yes
   return _Read(ai_fNumber,m_pFile,this);
  //## end ClFileIODriver::Read%-696517996.body
}

m4return_t ClFileIODriver::Read (m4double_t& ai_dNumber)
{
  //## begin ClFileIODriver::Read%-691472444.body preserve=yes
   return _Read(ai_dNumber,m_pFile,this);
  //## end ClFileIODriver::Read%-691472444.body
}

m4return_t ClFileIODriver::Read (m4bool_t& ai_iBoolean)
{
  //## begin ClFileIODriver::Read%307694310.body preserve=yes
   m4uint8_t iAux;

   if (Read(iAux)!=M4_SUCCESS)
      return M4_ERROR;

   if (iAux>0)
      ai_iBoolean=M4_TRUE;
   else
      ai_iBoolean=M4_FALSE;

   return M4_SUCCESS;
  //## end ClFileIODriver::Read%307694310.body
}

m4return_t ClFileIODriver::Read (void** ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClFileIODriver::Read%1983193096.body preserve=yes
   m4uint32_t Offset;

   if (Read(Offset)!=M4_SUCCESS)
      return M4_ERROR;

   if (Offset==TAO_OFFSET_NULL)
   {
      *ai_pPtr=0;
      *ao_pExist=M4_TRUE; // Protocolo para distinguir con respecto a offsetnext
   }
   else
   {
		/* Bug 0098661
		No hay que comprobar que el offset sea menor que el ftell, porque en
		el caso de un RamdomFileIODriver el ftell puede haber saltado adelante
		por paginación
		En cualquier caso esto era una optimización bastante dudosa. Se mira
		siempre en la tabla y punto
		*/
		*ai_pPtr = m_oTAO->GetAddress( Offset ) ;

		if( *ai_pPtr == NULL )
		{
			 *ao_pExist = M4_FALSE ;
		}
		else
		{
			 *ao_pExist = M4_TRUE ;
		}
   }

   return M4_SUCCESS;
  //## end ClFileIODriver::Read%1983193096.body
}

m4return_t ClFileIODriver::Read (void* ai_pPtr, m4uint32_t& ao_iSize)
{
  //## begin ClFileIODriver::Read%1278894514.body preserve=yes
   if (Read(ao_iSize)!=M4_SUCCESS)
      return M4_ERROR;

   if (ao_iSize > 0)
      if (FRead(ai_pPtr,1,ao_iSize,m_pFile) != ao_iSize)
	  {
		  m_iLastErrorCode = FILE_ERR_READ_FILE;
		  SetLastErrorDescr(strerror(errno));
		
		  BL_ERRORF(M4_SERIAL_CANT_READ_FROM_FILE, GetLastErrorDescr() << m_pcFileName);
		
		  return M4_ERROR;
	  }

   return M4_SUCCESS;
  //## end ClFileIODriver::Read%1278894514.body
}

m4return_t ClFileIODriver::Read (m4pchar_t ai_pString, m4uint32_t ai_iMaxLen)
{
  //## begin ClFileIODriver::Read%632421546.body preserve=yes
   m4uint32_t iAuxSize;

   if (ReadSize(iAuxSize)!=M4_SUCCESS)
      return M4_ERROR;

   if (ai_iMaxLen>0 && iAuxSize>ai_iMaxLen)
      return M4_ERROR;

   if (ReadBuffer(ai_pString)!=M4_SUCCESS)
      return M4_ERROR;

   ai_pString[iAuxSize]=0;

   return M4_SUCCESS;
  //## end ClFileIODriver::Read%632421546.body
}

m4return_t ClFileIODriver::Read (m4pcchar_t ai_pConstString, m4uint32_t ai_iMaxLen)
{
  //## begin ClFileIODriver::Read%-1758939566.body preserve=yes
   //return Read((m4pchar_t)ai_pConstString,ai_iMaxLen);
	return Read( ai_pConstString, ai_iMaxLen);
  //## end ClFileIODriver::Read%-1758939566.body
}

m4return_t ClFileIODriver::ReadPrivate (void** ai_pPtr, m4bool_t* ao_pExist)
{
  //## begin ClFileIODriver::ReadPrivate%1839577441.body preserve=yes
   // Por ahora
   return Read(ai_pPtr,ao_pExist);
  //## end ClFileIODriver::ReadPrivate%1839577441.body
}

m4return_t ClFileIODriver::ReadSize (m4uint32_t& ao_iSize)
{
  //## begin ClFileIODriver::ReadSize%-1216882674.body preserve=yes
   if (Read(ao_iSize)!=M4_SUCCESS)
      return M4_ERROR;

   m_iLastSize=ao_iSize;

   return M4_SUCCESS;
  //## end ClFileIODriver::ReadSize%-1216882674.body
}

m4return_t ClFileIODriver::ReadBuffer (void* ai_pPtr)
{
  //## begin ClFileIODriver::ReadBuffer%-1973378766.body preserve=yes
   if (m_iLastSize==-1)
      return M4_ERROR;

   if (m_iLastSize > 0)
      if ((m4int32_t)FRead(ai_pPtr, 1, m_iLastSize, m_pFile) != m_iLastSize)
	  {
		  m_iLastErrorCode = FILE_ERR_READ_FILE;
		  SetLastErrorDescr(strerror(errno));
		
		  BL_ERRORF(M4_SERIAL_CANT_READ_FROM_FILE, GetLastErrorDescr() << m_pcFileName);
		
		  return M4_ERROR;
	  }

   m_iLastSize=-1;

   return M4_SUCCESS;
  //## end ClFileIODriver::ReadBuffer%-1973378766.body
}

m4return_t ClFileIODriver::ReadToFile (m4pchar_t ao_pFileName)
{
  //## begin ClFileIODriver::ReadToFile%-102116644.body preserve=yes
   FILE* pTempFile;

   m4char_t pzAuxFileExtension[256];

   if (Read(pzAuxFileExtension)!=M4_SUCCESS)
      return M4_ERROR;

   ClFileName oTempFileName;

   if (ClFileManager::Instance()->CreateTempFile(oTempFileName,
                                                 "FIOD",
                                                 pzAuxFileExtension)!=M4_SUCCESS)
      return M4_ERROR;

   oTempFileName.GetFileName(ao_pFileName);

   // Read it the same way as a binary buffer
   m4uint32_t iFileSize;

   if (ReadSize(iFileSize)!=M4_SUCCESS)
      return M4_ERROR;

   // UNICODE FILE
   eUniFileEncodingType_t eEncoding = M4UniANSI ;
   pTempFile = M4Fopen( ao_pFileName, M4UniWriteBinary, eEncoding ) ;

   if (pTempFile==0)
   {
		m_iLastErrorCode = FILE_ERR_OPEN_FILE;
		SetLastErrorDescr(strerror(errno));
		
	    BL_ERRORF(M4_SERIAL_CANT_OPEN_FILE, GetLastErrorDescr() << m_pcFileName);

		return M4_ERROR;
   }

   m4uint8_t TempBuffer[TAM_TEMP_BUFFER];

   while (iFileSize>0)
   {
      m4uint32_t iToRead,
                 iRead;

      if (iFileSize>sizeof(TempBuffer))
         iToRead=sizeof(TempBuffer);
      else
         iToRead=iFileSize;

      iRead=(m4uint32_t)FRead(TempBuffer,1,iToRead,m_pFile);

      if (iRead==iToRead)
      {
         if (fwrite(TempBuffer,1,iRead,pTempFile)!=iRead)
         {
			   m_iLastErrorCode = FILE_ERR_WRITE_FILE;
			   SetLastErrorDescr(strerror(errno));

			   BL_ERRORF(M4_SERIAL_CANT_WRITE_TO_FILE, GetLastErrorDescr() << m_pcFileName);

			   fclose(pTempFile);
			   return M4_ERROR;
         }

         iFileSize-=iRead;
      }
      else
      {
         fclose(pTempFile);

         return M4_SUCCESS;
      }
   }

   fclose(pTempFile);

   return M4_SUCCESS;
  //## end ClFileIODriver::ReadToFile%-102116644.body
}

m4return_t ClFileIODriver::Close (void )
{
  //## begin ClFileIODriver::Close%-1601601788.body preserve=yes
	return M4_ERROR;
  //## end ClFileIODriver::Close%-1601601788.body
}

m4bool_t ClFileIODriver::Flush (void )
{
  //## begin ClFileIODriver::Flush%1195410010.body preserve=yes
	return ( (fflush(m_pFile)==0) ? M4_TRUE : M4_FALSE );
  //## end ClFileIODriver::Flush%1195410010.body
}

m4bool_t ClFileIODriver::Eof (void )
{
  //## begin ClFileIODriver::Eof%1937909443.body preserve=yes
	return ( (feof(m_pFile)!=0) ? M4_TRUE : M4_FALSE );
  //## end ClFileIODriver::Eof%1937909443.body
}

m4return_t ClFileIODriver::StoreAddress (void* ai_pPtr)
{
  //## begin ClFileIODriver::StoreAddress%-1340936799.body preserve=yes
   if (ai_pPtr!=0)
   {
      if (m_iMode==GEN_IO_MODE_READ)
         m_oTAO->Add(ftell(m_pFile),
                     ai_pPtr); // This is the place where we need to put the value pointed by ai_iPtr !!!
      else
         m_oTAO->Add(ai_pPtr,
                     ftell(m_pFile)); // This is the place where we need to put the value pointed by ai_iPtr !!!

      return M4_SUCCESS;
   }
   else
      return M4_ERROR;
  //## end ClFileIODriver::StoreAddress%-1340936799.body
}

m4return_t ClFileIODriver::StoreAddress (void* ai_pPtr, m4uint32_t ai_iOffset)
{
  //## begin ClFileIODriver::StoreAddress%2032249160.body preserve=yes
   M4_ASSERT(m_iMode==GEN_IO_MODE_READ);

   if (ai_pPtr!=0)
   {
      m_oTAO->Add(ai_iOffset,
                  ai_pPtr); // This is the place where we need to put the value pointed by ai_iPtr !!!

      return M4_SUCCESS;
   }
   else
      return M4_ERROR;
  //## end ClFileIODriver::StoreAddress%2032249160.body
}

m4return_t ClFileIODriver::StoreAddressPrivate (void* ai_pPtr)
{
  //## begin ClFileIODriver::StoreAddressPrivate%665104906.body preserve=yes
   // Por ahora
   return StoreAddress(ai_pPtr);
  //## end ClFileIODriver::StoreAddressPrivate%665104906.body
}

m4return_t ClFileIODriver::StoreAddressPrivate (void* ai_pPtr, m4uint32_t ai_iOffset)
{
  //## begin ClFileIODriver::StoreAddressPrivate%-744541088.body preserve=yes
   // Por ahora
   return StoreAddress(ai_pPtr,ai_iOffset);
  //## end ClFileIODriver::StoreAddressPrivate%-744541088.body
}

m4uint32_t ClFileIODriver::GetActualOffset (void )
{
  //## begin ClFileIODriver::GetActualOffset%474850445.body preserve=yes
   return ftell(m_pFile);
  //## end ClFileIODriver::GetActualOffset%474850445.body
}

void ClFileIODriver::End (m4bool_t ai_iCloseFile)
{
  //## begin ClFileIODriver::End%2087747482.body preserve=yes
   if (m_pFile!=0 && ai_iCloseFile)
      fclose(m_pFile);

   if (m_oTAO!=0)
   {
      delete m_oTAO;

      m_oTAO=0;
   }

   if (m_pcFileName != NULL)
   {
	   delete [] m_pcFileName;
	   m_pcFileName = NULL;
   }
  //## end ClFileIODriver::End%2087747482.body
}

m4return_t ClFileIODriver::ResetPrivateInfo (void )
{
  //## begin ClFileIODriver::ResetPrivateInfo%32970469.body preserve=yes
   return M4_SUCCESS;
  //## end ClFileIODriver::ResetPrivateInfo%32970469.body
}


size_t	ClFileIODriver::FRead( void* ai_pvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile )
{

	size_t	iResult = 0 ;


	iResult = fread( ai_pvPointer, ai_iNumber, ai_iSize, ai_pfFile ) ;
	return( iResult ) ;
}


size_t	ClFileIODriver::FWrite( const void* ai_cpvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile )
{

	size_t	iResult = 0 ;


	iResult = fwrite( ai_cpvPointer, ai_iNumber, ai_iSize, ai_pfFile ) ;
	return( iResult ) ;
}



// Additional Declarations
  //## begin ClFileIODriver%3911AA2503CE.declarations preserve=yes
  //## end ClFileIODriver%3911AA2503CE.declarations



// ============================================================================
//
//	Clase ClRamdomFileIODriver
//
// ============================================================================

/*
Los constructores sólo llaman al constructor del padre y después inicializan
las variables propias
*/
ClRamdomFileIODriver::ClRamdomFileIODriver( void )

: ClFileIODriver()

{
	m_lFreeSize = -1 ;
	m_lChunkStart = -1 ;
}


ClRamdomFileIODriver::ClRamdomFileIODriver( m4uint32_t ai_iMode, char* ai_pFileName, m4bool_t ai_bGenErrorLog )

: ClFileIODriver( ai_iMode, ai_pFileName, ai_bGenErrorLog )

{
	m_lFreeSize = -1 ;
	m_lChunkStart = -1 ;
}


/*
El destructor no hace nada
Ha habido que poner virtual el destructor del padre
*/
ClRamdomFileIODriver::~ClRamdomFileIODriver( void )
{
}


/*
Esta función resetea los valores de los miembros propios y llama al Reset
del padre
*/
void	ClRamdomFileIODriver::Reset( void )
{
	m_lChunkStart = -1 ;
	m_lFreeSize = -1 ;

	ClFileIODriver::Reset() ;
}



/*
Comienza la lectura en una posición que se pasa como argumento
La posición debe ser una posición válida de un bloque anteriormente escrito
Resetea los valores y delega en la función interna
*/
m4return_t	ClRamdomFileIODriver::StartReading( long ai_lPosition )
{

	m4return_t	iResult = M4_ERROR ;


	Reset() ;
	iResult = _Start( ai_lPosition, M4_TRUE ) ;
	return( iResult ) ;
}


/*
Finaliza la lectura
Delega en la función interna que en este caso no hace nada y resetea los valores
*/
m4return_t	ClRamdomFileIODriver::EndReading( void )
{
	
	m4return_t	iResult = M4_ERROR ;
	long		lPosition = -1 ;


	iResult = _End( lPosition, M4_TRUE ) ;
	Reset() ;
	return( iResult ) ;
}


/*
Comienza la escritura en una posición que se pasa como argumento
La posición puede ser una posición válida de un bloque anteriormente escrito
o puede ser un valor negativo, indicando que se quiere escribir al final del
fichero en un bloque nuevo
Resetea los valores y delega en la función interna
*/
m4return_t	ClRamdomFileIODriver::StartWritting( long ai_lPosition )
{

	m4return_t	iResult = M4_ERROR ;


	Reset() ;
	iResult = _Start( ai_lPosition, M4_FALSE ) ;
	return( iResult ) ;
}


/*
Finaliza la escritura, devolviendo la posición del último bloque escrito
Si se pidió empezar a escribir al final del fichero este valor es el comienzo
del bloque nuevo. Si se pidió escribir en un bloque existente este valor
es el comienzo del último bloque enlazado en el que se ha escrito y en este
caso ese valor se puede ignorar
Delega en la función interna y resetea los valores
*/
m4return_t	ClRamdomFileIODriver::EndWritting( long& ao_rlPosition )
{

	m4return_t	iResult = M4_ERROR ;


	iResult = _End( ao_rlPosition, M4_FALSE ) ;
	Reset() ;
	return( iResult ) ;
}


/*
Lee un valor de la posición actual
Delega en la función interna
*/
size_t	ClRamdomFileIODriver::FRead( void* ai_pvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile )
{
	return( _FReadWrite( ai_pvPointer, ai_iNumber, ai_iSize, ai_pfFile, M4_TRUE ) ) ;
}


/*
Escribe un valor en la posición actual
Delega en la función interna
*/
size_t	ClRamdomFileIODriver::FWrite( const void* ai_cpvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile )
{
	return( _FReadWrite( (void*) ai_cpvPointer, ai_iNumber, ai_iSize, ai_pfFile, M4_FALSE ) ) ;
}




/*
Función interna que realiza la inicialización necesaria para la lectura o escritura
Si es lectura o escritura se sabe por el parámetro ai_bRead
También se pasa como parámetro donde se quiere comenzar la lectura o escritura

Modo lectura
	Nos posicionamos en la posición indicada como argumento
	Se lee el tamaño del bloque
	El valor del comienzo del bloque no se asigna y se queda a -1

Modo escritura
	Bloque existente
		Nos posicionamos en la posición indicada como argumento
		Se lee el tamaño del bloque
		Se realiza un fseek ficticio a la posición actual, ya que cuando se pasa de
			un fread a un fwrite hay que hacer necesariamente un fseek. Así dejamos
			todo preparado para que se puedean hacer escrituras

	Bloque nuevo
		Si el bloque no es el primero, cosa que se sabe porque la posición
			del bloque actual existe (no es -1) se obtiene la posición del fichero
			que es el final del bloque anterior
		Nos posicionamos al final del fichero
		Leemos la posición del nuevo bloque y lo guardamos en el argumento
		Si no era el primer bloque nos posicionamos en el final del bloque anterior
			menos el tamaño del puntero al siguiente bloque y se escribe el comienzo
			del nuevo bloque. Después nos reposicionamos otra vez al final del fichero
		Se inicializa el tamaño libre a -1
		Se escribe ese tamaño del nuevo bloque (de momento -1) para reservar espacio
			para rellenarlo luego
			
	Se asigna el comienzo del bloque a la posición pasada o a la nueva posición
*/
m4return_t	ClRamdomFileIODriver::_Start( long ai_lPosition, m4bool_t ai_bRead )
{

	int		iSeek = -1 ;
	long	lPosition = -1 ;
	size_t	iRead = 0 ;
	size_t	iWrite = 0 ;


	if( ai_bRead == M4_TRUE )
	{
		iSeek = fseek( m_pFile, ai_lPosition, SEEK_SET ) ;
		M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;

		iRead = fread( &m_lFreeSize, 1, sizeof( long ), m_pFile ) ;
		M4SERIAL_CHECK_ERROR( iRead != sizeof( long ), M4_ERROR, M4_SERIAL_CANT_READ_FROM_FILE, FILE_ERR_READ_FILE ) ;
	}
	else
	{
		if( ai_lPosition >= 0 )
		{
			iSeek = fseek( m_pFile, ai_lPosition, SEEK_SET ) ;
			M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;

			iRead = fread( &m_lFreeSize, 1, sizeof( long ), m_pFile ) ;
			M4SERIAL_CHECK_ERROR( iRead != sizeof( long ), M4_ERROR, M4_SERIAL_CANT_READ_FROM_FILE, FILE_ERR_READ_FILE ) ;

			iSeek = fseek( m_pFile, 0, SEEK_CUR ) ;
			M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;
		}
		else
		{
			if( m_lChunkStart != -1 )
			{
				lPosition = ftell( m_pFile ) ;
				M4SERIAL_CHECK_ERROR( lPosition == -1, M4_ERROR, M4_SERIAL_CANT_READ_FROM_FILE, FILE_ERR_READ_FILE ) ;
			}

			iSeek = fseek( m_pFile, 0, SEEK_END ) ;
			M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;

			ai_lPosition = ftell( m_pFile ) ;
			M4SERIAL_CHECK_ERROR( ai_lPosition == -1, M4_ERROR, M4_SERIAL_CANT_READ_FROM_FILE, FILE_ERR_READ_FILE ) ;

			if( m_lChunkStart != -1 )
			{
				iSeek = fseek( m_pFile, lPosition - sizeof( long ), SEEK_SET ) ;
				M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;

				iWrite = fwrite( &ai_lPosition, 1, sizeof( long ), m_pFile ) ;
				M4SERIAL_CHECK_ERROR( iWrite != sizeof( long ), M4_ERROR, M4_SERIAL_CANT_WRITE_TO_FILE, FILE_ERR_WRITE_FILE ) ;

				iSeek = fseek( m_pFile, 0, SEEK_END ) ;
				M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;
			}

			m_lFreeSize = -1 ;
			iWrite = fwrite( &m_lFreeSize, 1, sizeof( long ), m_pFile ) ;
			M4SERIAL_CHECK_ERROR( iWrite != sizeof( long ), M4_ERROR, M4_SERIAL_CANT_WRITE_TO_FILE, FILE_ERR_WRITE_FILE ) ;
		}

		m_lChunkStart = ai_lPosition ;
	}

	return( M4_SUCCESS ) ;
}


/*
Función interna que realiza la finalización necesaria para la lectura o escritura
Si es lectura o escritura se sabe por el parámetro ai_bRead
También se pasa un parámetro de salida donde comienza el bloque actual

Se rellena el parámetro de salida con la posición del bloque actual

Modo lectura
	No se hace nada

Modo escritura

	Bloque existente
		Si el bloque ya existe se sabe porque su tamaño libre es positivo o 0
		No se hace nada

	Bloque nuevo
		Se escribe el comienzo del siguiente bloque enlazado, que de momento
			al no existir es -1
		Nos posicionamos en el comienzo del bloque para rellenar su tamaño
		Su tamaño se calcula a partir del tamaño libre, que es negativo y empezó en -1
		Se escribe el tamaño
*/
m4return_t	ClRamdomFileIODriver::_End( long &ai_rlPosition, m4bool_t ai_bRead )
{

	int		iSeek = -1 ;
	long	lNextChunk = -1 ;
	size_t	iWrite = 0 ;


	ai_rlPosition = m_lChunkStart ;

	if( ai_bRead == M4_TRUE )
	{
	}
	else
	{
		if( m_lFreeSize >= 0 )
		{
		}
		else
		{
			lNextChunk = -1 ;
			iWrite = fwrite( &lNextChunk, 1, sizeof( long ), m_pFile ) ;
			M4SERIAL_CHECK_ERROR( iWrite != sizeof( long ), M4_ERROR, M4_SERIAL_CANT_WRITE_TO_FILE, FILE_ERR_WRITE_FILE ) ;

			iSeek = fseek( m_pFile, m_lChunkStart, SEEK_SET ) ;
			M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;

			m_lFreeSize = -1 - m_lFreeSize ;
			iWrite = fwrite( &m_lFreeSize, 1, sizeof( long ), m_pFile ) ;
			M4SERIAL_CHECK_ERROR( iWrite != sizeof( long ), M4_ERROR, M4_SERIAL_CANT_WRITE_TO_FILE, FILE_ERR_WRITE_FILE ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


/*
Función interna que realiza las lecturas o escrituras
Si es lectura o escritura se sabe por el parámetro ai_bRead

Se calcula el tamaño a escribir
Se calcula la parte que cabe en el bloque actual y el resto. Si estamos escribiendo
al final del fichero el tamaño libre es negativo y siempre cabe en el bloque actual
Si hay algo que leer o escribir en el bloque actual se lee o escribe y se resta el
número de bytes procesados al tamaño libre
Si hay algo que leer o escribir en el siguiente bloque se lee la posición del
siguiente bloque y nos posicionamos en ella (llamando a la función de comienzo).
Si no existe el siguiente bloque la posición es -1 y esta función ya crea un
bloque nuevo y se posiciona en él (si es modo escritura).
Se llama a esta misma función recursivamente con el nuevo tamaño a escribir y la
nueva dirección de memoria
Se de vuelven los bytes procesados, que son los del bloque actual más los de los
siguientes bloques

Nota: si estamos en modo escritura, antes de leer la posición del siguiente bloque
hay que hacer un fseek ficticio a la posición actual porque se pasa de un fwrite a
un fread. Después no hay que hacer otro fseek porque la función start ya lo hace
siempre.
*/
size_t	ClRamdomFileIODriver::_FReadWrite( void* ai_pvPointer, size_t ai_iNumber, size_t ai_iSize, FILE* ai_pfFile, m4bool_t ai_bRead )
{

	m4return_t	iResult = M4_ERROR ;
	int			iSeek = -1 ;
	long		lNextChunk = -1 ;
	size_t		iSize = 0 ;
	size_t		iMissSize = 0 ;
	size_t		iProcessed = 0 ;
	size_t		iRead = 0 ;


	iSize = ai_iNumber * ai_iSize ;

	if( m_lFreeSize < 0 || iSize <= m_lFreeSize )
	{
		iMissSize = 0 ;
	}
	else
	{
		iMissSize = iSize - m_lFreeSize ;
		iSize = m_lFreeSize ;
	}

	if( iSize > 0 )
	{
		if( ai_bRead == M4_TRUE )
		{
			iProcessed = fread( ai_pvPointer, 1, iSize, m_pFile ) ;
			M4SERIAL_CHECK_ERROR( iProcessed != iSize, 0, M4_SERIAL_CANT_READ_FROM_FILE, FILE_ERR_READ_FILE ) ;
		}
		else
		{
			iProcessed = fwrite( ai_pvPointer, 1, iSize, m_pFile ) ;
			M4SERIAL_CHECK_ERROR( iProcessed != iSize, 0, M4_SERIAL_CANT_WRITE_TO_FILE, FILE_ERR_WRITE_FILE ) ;
		}

		m_lFreeSize -= (long)iProcessed ;
	}

	if( iMissSize > 0 )
	{
		if( ai_bRead == M4_FALSE )
		{
			iSeek = fseek( m_pFile, 0, SEEK_CUR ) ;
			M4SERIAL_CHECK_ERROR( iSeek != 0, M4_ERROR, M4_SERIAL_CANT_SEEK_FILE, FILE_ERR_SEEK_FILE ) ;
		}

		iRead = fread( &lNextChunk, 1, sizeof( long ), m_pFile ) ;
		M4SERIAL_CHECK_ERROR( iRead != sizeof( long ), iSize, M4_SERIAL_CANT_READ_FROM_FILE, FILE_ERR_READ_FILE ) ;

		iResult = _Start( lNextChunk, ai_bRead ) ;

		if( iResult != M4_SUCCESS )
		{
			return( iSize ) ;
		}

		iProcessed = _FReadWrite( (char*) ai_pvPointer + iSize, 1, iMissSize, m_pFile, ai_bRead ) ;

		if( iProcessed != iMissSize )
		{
			return( iSize + iProcessed ) ;
		}
	}

	return( iSize + iMissSize ) ;
}





//## begin module%3911AA29017B.epilog preserve=yes
//## end module%3911AA29017B.epilog
