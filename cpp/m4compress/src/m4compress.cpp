//## begin module%373AE15902E3.cm preserve=no
//## end module%373AE15902E3.cm

//## begin module%373AE15902E3.cp preserve=no
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
//## end module%373AE15902E3.cp

//## Module: m4compress%373AE15902E3; Package body
//## Subsystem: M4Compress::src%37C2BE980108
//## Source file: e:\source\m4compress\version\src\m4compress.cpp

//## begin module%373AE15902E3.additionalIncludes preserve=no
//## end module%373AE15902E3.additionalIncludes

//## begin module%373AE15902E3.includes preserve=yes
//## end module%373AE15902E3.includes

// m4compress
#include <m4compress.hpp>
// m4types
#include <m4types.hpp>
//## begin module%373AE15902E3.declarations preserve=no
//## end module%373AE15902E3.declarations

//## begin module%373AE15902E3.additionalDeclarations preserve=yes
m4return_t M4DecompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile, m4pchar_t ai_Mode) {

	ClCompressFactory ComFact;

	ClCompress *FileCompress = ComFact.GetCompress(ai_Mode);

	if (!FileCompress)
		return M4_ERROR;

	return (FileCompress->DecompressFile(ai_pSourceFile,ai_pDestinationFile));
}

m4return_t M4CompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile, m4pchar_t ai_Mode) {

	ClCompressFactory ComFact;

	ClCompress *FileCompress = ComFact.GetCompress(ai_Mode);

	if (!FileCompress)
		return M4_ERROR;

	return (FileCompress->CompressFile(ai_pSourceFile,ai_pDestinationFile));
}

#ifdef USE_PKWARE
/* Routine to read uncompressed data.  Used only by implode().
** This routine reads the data that is to be compressed.
*/


unsigned int 
ReadUnCompressed(char *buff, unsigned int *size, void *Param)
{
   PARAM *Ptr = (PARAM *) Param;

   if (Ptr->UnCompressedSize == 0L)  
   {
      /* This will terminate the compression or extraction process */
      return(0);
   }

   if (Ptr->UnCompressedSize < (unsigned long)*size)
   {
      *size = (unsigned int)Ptr->UnCompressedSize;
   }

   memcpy(buff, Ptr->pSource + Ptr->SourceOffset, *size);
   Ptr->SourceOffset += (unsigned long)*size;
   Ptr->UnCompressedSize -= (unsigned long)*size;

   return(*size);
}

/* Routine to read compressed data.  Used only by explode(). 
** This routine reads the compressed data that is to be uncompressed.
*/

unsigned int
ReadCompressed(char *buff, unsigned int *size, void *Param)
{
   PARAM *Ptr = (PARAM *) Param;

   if (Ptr->CompressedSize == 0L)  
   {
      /* This will terminate the compression or extraction process */
      return(0);
   }

   if (Ptr->CompressedSize < (unsigned long)*size)
   {
      *size = (unsigned int)Ptr->CompressedSize;
   }

   memcpy(buff, Ptr->pSource + Ptr->SourceOffset, *size);
   Ptr->SourceOffset += (unsigned long)*size;
   Ptr->CompressedSize -= (unsigned long)*size;

   return(*size);
}

/* Routime to write compressed data.  Used only by implode(). 
** This routine writes the compressed data to a memory buffer.
*/

void 
WriteCompressed(char *buff, unsigned int *size, void *Param)
{
   PARAM *Ptr = (PARAM *) Param;

   if (Ptr->CompressedSize + (unsigned long)*size > Ptr->BufferSize)
   {
	   	m4uint32_t newbuffersize = Ptr->CompressedSize + 2*(unsigned long)*size ;
		m4pchar_t newbuffer=(char *)realloc(Ptr->pDestination,newbuffersize);
		Ptr->pDestination = newbuffer;
		Ptr->BufferSize=newbuffersize;
      m4TraceLevel(2,cout<<"Increase size of buffer"<<endl);
  //    exit(1);
   }
   memcpy(Ptr->pDestination + Ptr->DestinationOffset, buff, *size);
   Ptr->DestinationOffset += (unsigned long)*size;
   Ptr->CompressedSize += (unsigned long)*size;
}

/* Routine to write uncompressed data. Used only by explode().
** This routine writes the uncompressed data to a memory buffer.
*/

void 
WriteUnCompressed(char *buff, unsigned int *size, void *Param)
{
   PARAM *Ptr = (PARAM *) Param;

   if (Ptr->UnCompressedSize + (unsigned long)*size > Ptr->BufferSize)
   {
		m4uint32_t newbuffersize = Ptr->UnCompressedSize + 2*(unsigned long)*size ;
		m4pchar_t newbuffer=(char *)realloc(Ptr->pDestination,newbuffersize);
		Ptr->pDestination = newbuffer;
		Ptr->BufferSize=newbuffersize;
      m4TraceLevel(2,cout<<"Increase size of buffer"<<endl);
     // exit(1);
   }
   memcpy(Ptr->pDestination + Ptr->DestinationOffset, buff, *size);
   Ptr->DestinationOffset += (unsigned long)*size;
   Ptr->UnCompressedSize += (unsigned long)*size;

}
#endif // USE_PKWARE
//## end module%373AE15902E3.additionalDeclarations


// Class ClCompress 




ClCompress::ClCompress ()
  //## begin ClCompress::ClCompress%926606772.hasinit preserve=no
      : m_pcName(NULL), m_pcVersion(NULL), m_iCompressionLevel(0)
  //## end ClCompress::ClCompress%926606772.hasinit
  //## begin ClCompress::ClCompress%926606772.initialization preserve=yes
  //## end ClCompress::ClCompress%926606772.initialization
{
  //## begin ClCompress::ClCompress%926606772.body preserve=yes
  //## end ClCompress::ClCompress%926606772.body
}


ClCompress::~ClCompress ()
{
  //## begin ClCompress::~ClCompress%926606773.body preserve=yes
  //## end ClCompress::~ClCompress%926606773.body
}



//## Other Operations (implementation)
m4return_t ClCompress::Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClCompress::Compress%926606774.body preserve=yes
	return M4_ERROR;
  //## end ClCompress::Compress%926606774.body
}

m4return_t ClCompress::Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClCompress::Decompress%926606775.body preserve=yes
	return M4_ERROR;
  //## end ClCompress::Decompress%926606775.body
}

m4return_t ClCompress::Init (m4int_t ai_iCompressionLevel)
{
  //## begin ClCompress::Init%926606776.body preserve=yes
	return M4_ERROR;
  //## end ClCompress::Init%926606776.body
}

m4return_t ClCompress::SetCompressionLevel (m4int_t ai_iCompresseionLevel)
{
  //## begin ClCompress::SetCompressionLevel%926685383.body preserve=yes
	m_iCompressionLevel=ai_iCompresseionLevel;
	return M4_SUCCESS;
  //## end ClCompress::SetCompressionLevel%926685383.body
}

m4return_t ClCompress::GetCompressionLevel (m4int_t &ao_iCompresseionLevel)
{
  //## begin ClCompress::GetCompressionLevel%926685384.body preserve=yes
	ao_iCompresseionLevel=m_iCompressionLevel;
	return M4_SUCCESS;
  //## end ClCompress::GetCompressionLevel%926685384.body
}

m4return_t ClCompress::Free ()
{
  //## begin ClCompress::Free%926948284.body preserve=yes
	return M4_ERROR;
  //## end ClCompress::Free%926948284.body
}

m4return_t ClCompress::CompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile)
{
  //## begin ClCompress::CompressFile%935144291.body preserve=yes
	return M4_ERROR;
  //## end ClCompress::CompressFile%935144291.body
}

m4return_t ClCompress::DecompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile)
{
  //## begin ClCompress::DecompressFile%935144292.body preserve=yes
	return M4_ERROR;
  //## end ClCompress::DecompressFile%935144292.body
}

// Additional Declarations
  //## begin ClCompress%373AA67F0393.declarations preserve=yes
  //## end ClCompress%373AA67F0393.declarations

// Class ClCompressFactory 

ClCompressFactory::ClCompressFactory ()
  //## begin ClCompressFactory::ClCompressFactory%926606779.hasinit preserve=no
  //## end ClCompressFactory::ClCompressFactory%926606779.hasinit
  //## begin ClCompressFactory::ClCompressFactory%926606779.initialization preserve=yes
  //## end ClCompressFactory::ClCompressFactory%926606779.initialization
{
  //## begin ClCompressFactory::ClCompressFactory%926606779.body preserve=yes
  //## end ClCompressFactory::ClCompressFactory%926606779.body
}


ClCompressFactory::~ClCompressFactory ()
{
  //## begin ClCompressFactory::~ClCompressFactory%926606780.body preserve=yes
  //## end ClCompressFactory::~ClCompressFactory%926606780.body
}



//## Other Operations (implementation)
ClCompress * ClCompressFactory::GetCompress (m4pchar_t ai_pcName, m4int_t ai_iCompressionLevel)
{
  //## begin ClCompressFactory::GetCompress%926606781.body preserve=yes

	m4return_t iRet;
	ClCompress * pCompress;
	switch(ai_pcName[0])
	{
	case 'N':
	case 'n':
		pCompress = new ClNULLCompress;
		break;
	case 'z':
	case 'Z':
		pCompress = new ClZlibCompress;
		break;
	case 'p':
	case 'P':
#ifdef USE_PKWARE
		pCompress =new ClPKWareCompress;
#else
		pCompress =NULL;
#endif // USE_PKWARE
		break;

	default :
		m4Trace(cerr<<"Unkown compressor:"<<ai_pcName<<endl);
		return NULL;
		break;
	}
	
	if(!pCompress)
		return NULL;

	switch(ai_pcName[0])
	{
	case 'n':
	case 'N':
	case 'z':
	case 'Z':
		iRet=pCompress->Init(ai_iCompressionLevel);
		break;
	case 'p':
	case 'P':
		iRet=pCompress->Init(ai_iCompressionLevel);
		break;
	}

	if(iRet!=M4_SUCCESS)
		return NULL;
	else
		return pCompress;

  //## end ClCompressFactory::GetCompress%926606781.body
}

// Additional Declarations
  //## begin ClCompressFactory%373AAC6E01E9.declarations preserve=yes
#ifdef USE_PKWARE
  //## end ClCompressFactory%373AAC6E01E9.declarations
// Class ClPKWareCompress 






ClPKWareCompress::ClPKWareCompress ()
  //## begin ClPKWareCompress::ClPKWareCompress%926929990.hasinit preserve=no
      : m_pcWorkBuffer(NULL), m_uiType(CMP_BINARY), m_pcInternalBuffer(NULL), m_uiInternalBufferSize(0)
  //## end ClPKWareCompress::ClPKWareCompress%926929990.hasinit
  //## begin ClPKWareCompress::ClPKWareCompress%926929990.initialization preserve=yes
  //## end ClPKWareCompress::ClPKWareCompress%926929990.initialization
{
  //## begin ClPKWareCompress::ClPKWareCompress%926929990.body preserve=yes
	if(!m_pcWorkBuffer)
		m_pcWorkBuffer=new m4char_t[CMP_BUFFER_SIZE];
	m_piDicSize=new unsigned int[3];
	m_piDicSize[0]=1024;
	m_piDicSize[1]=2048;
	m_piDicSize[2]=4096;
	if(!m_pcInternalBuffer)
		m_pcInternalBuffer=(char*)malloc(CMP_BUFFER_SIZE);
	m_uiInternalBufferSize=CMP_BUFFER_SIZE;

	m_Param.BufferSize = m_uiInternalBufferSize;
  //## end ClPKWareCompress::ClPKWareCompress%926929990.body
}


ClPKWareCompress::~ClPKWareCompress ()
{
  //## begin ClPKWareCompress::~ClPKWareCompress%926929991.body preserve=yes
	if(m_pcWorkBuffer)
		delete m_pcWorkBuffer;
	delete m_piDicSize;
	if(m_pcInternalBuffer)
		free(m_pcInternalBuffer);
  //## end ClPKWareCompress::~ClPKWareCompress%926929991.body
}



//## Other Operations (implementation)
m4return_t ClPKWareCompress::Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClPKWareCompress::Compress%926929992.body preserve=yes

	m_Param.UnCompressedSize = ai_uiBufferInSize;
	m_Param.CompressedSize = 0L;

	m_Param.pSource = ai_pcBufferIn;
	m_Param.pDestination = m_pcInternalBuffer;
	m_Param.SourceOffset      = 0L;
	m_Param.DestinationOffset = 0L;
	if (m_Param.pDestination == NULL)
	{
		cerr<<"NULL output buffer"<<endl;
		return M4_ERROR;
	}


	unsigned int uiReturn;
	m4return_t iRet;

	M4_ASSERT(m_pcWorkBuffer);
	uiReturn=implode(ReadUnCompressed,WriteCompressed,m_pcWorkBuffer,&m_Param,&m_uiType,&(m_piDicSize[m_iCompressionLevel])); 

	m_pcInternalBuffer=m_Param.pDestination;
	m_uiInternalBufferSize=m_Param.BufferSize;

	switch(uiReturn)
	{
	case CMP_NO_ERROR:
		iRet=M4_SUCCESS;
		ao_uiBufferOutSize=m_Param.CompressedSize;
		ao_pcBufferOut=m_Param.pDestination;
		break;
	case CMP_INVALID_DICTSIZE:
		iRet=M4_ERROR;
		ao_uiBufferOutSize=0;
		ao_pcBufferOut=NULL;
		cerr<< "CMP_INVALID_DICTSIZE" <<endl;
		break;
	case CMP_INVALID_MODE:
		iRet=M4_ERROR;
		ao_uiBufferOutSize=0;
		ao_pcBufferOut=NULL;
		cerr<<"CMP_INVALID_MODE" <<endl;
		break;
	default:
		iRet=M4_ERROR;
		ao_uiBufferOutSize=0;
		ao_pcBufferOut=NULL;
		cerr<<"Unkown value:"<<uiReturn <<endl;
		break;
	}
	return iRet;
  //## end ClPKWareCompress::Compress%926929992.body
}

m4return_t ClPKWareCompress::Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClPKWareCompress::Decompress%926929993.body preserve=yes

	m_Param.CompressedSize = ai_uiBufferInSize;
	m_Param.UnCompressedSize =0L;

	m_Param.pSource = ai_pcBufferIn;
	m_Param.pDestination = m_pcInternalBuffer;
	m_Param.SourceOffset      = 0L;
	m_Param.DestinationOffset = 0L;


	if (m_Param.pDestination == NULL)
	{
		cerr<<"NULL output buffer"<<endl;
		return M4_ERROR;
	}

	unsigned int uiReturn;
	m4return_t iRet;

	uiReturn= explode(ReadCompressed,WriteUnCompressed,m_pcWorkBuffer,&m_Param);
	m_pcInternalBuffer=m_Param.pDestination;
	m_uiInternalBufferSize=m_Param.BufferSize;
	cout<<"final size "<<m_uiInternalBufferSize<<endl;
	switch(uiReturn)
	{
	case CMP_NO_ERROR:
		iRet=M4_SUCCESS;
		ao_pcBufferOut=m_Param.pDestination;
		ao_uiBufferOutSize=m_Param.UnCompressedSize ;
		break;
	case CMP_INVALID_DICTSIZE:
		iRet=M4_ERROR;
		ao_pcBufferOut=NULL;
		ao_uiBufferOutSize=0;
		cerr<< "CMP_INVALID_DICTSIZE" <<endl;
		break;
	case CMP_INVALID_MODE:
		iRet=M4_ERROR;
		ao_pcBufferOut=NULL;
		ao_uiBufferOutSize=0;
		cerr<< "CMP_INVALID_MODE" <<endl;
		break;
	case CMP_BAD_DATA:
		iRet=M4_ERROR;
		ao_pcBufferOut=NULL;
		ao_uiBufferOutSize=0;
		cerr<< "CMP_BAD_DATA"<<endl;
		break;
	case CMP_ABORT:
		iRet=M4_ERROR;
		ao_pcBufferOut=NULL;
		ao_uiBufferOutSize=0;
		cerr<<"CMP_ABORT" <<endl;
		break;
	default:
		iRet=M4_ERROR;
		ao_pcBufferOut=NULL;
		ao_uiBufferOutSize=0;
		cerr<<"Unkown error code:"<<uiReturn <<endl;
		break;
	}
	return iRet;
  //## end ClPKWareCompress::Decompress%926929993.body
}

m4return_t ClPKWareCompress::Init (m4int_t ai_iCompressionLevel)
{
  //## begin ClPKWareCompress::Init%926929994.body preserve=yes
	return SetCompressionLevel(ai_iCompressionLevel);
  //## end ClPKWareCompress::Init%926929994.body
}

m4return_t ClPKWareCompress::SetCompressionLevel (m4int_t ai_iCompressionLevel)
{
  //## begin ClPKWareCompress::SetCompressionLevel%926948283.body preserve=yes
	if((ai_iCompressionLevel<0) || (ai_iCompressionLevel>9))
		return M4_ERROR;
	if(ai_iCompressionLevel<6)
		m_iCompressionLevel=0;
	else 
		if(ai_iCompressionLevel<9)
			m_iCompressionLevel=1;
		else 
			m_iCompressionLevel=2;
	
	return M4_SUCCESS;
  //## end ClPKWareCompress::SetCompressionLevel%926948283.body
}

m4return_t ClPKWareCompress::Free ()
{
  //## begin ClPKWareCompress::Free%926948285.body preserve=yes
	if(m_uiInternalBufferSize>CMP_BUFFER_SIZE)
	{
		if(!m_pcInternalBuffer)
		{	
			free(m_pcInternalBuffer);
			m_pcInternalBuffer=(char*)malloc(CMP_BUFFER_SIZE);
		}
		m_uiInternalBufferSize=CMP_BUFFER_SIZE;
	}
	return M4_SUCCESS;
  //## end ClPKWareCompress::Free%926948285.body
}

// Additional Declarations
  //## begin ClPKWareCompress%373AAEBF01C3.declarations preserve=yes
#endif //USE_PKWARE
  //## end ClPKWareCompress%373AAEBF01C3.declarations
// Class ClZlibCompress 

ClZlibCompress::ClZlibCompress ()
  //## begin ClZlibCompress::ClZlibCompress%926685388.hasinit preserve=no
  //## end ClZlibCompress::ClZlibCompress%926685388.hasinit
  //## begin ClZlibCompress::ClZlibCompress%926685388.initialization preserve=yes
  //## end ClZlibCompress::ClZlibCompress%926685388.initialization
{
  //## begin ClZlibCompress::ClZlibCompress%926685388.body preserve=yes
	m_pcName="Zlib";
	m_pcVersion="1.0";
	m_poCompressor=new M4Zlib;
  //## end ClZlibCompress::ClZlibCompress%926685388.body
}


ClZlibCompress::~ClZlibCompress ()
{
  //## begin ClZlibCompress::~ClZlibCompress%926685389.body preserve=yes
	if(m_poCompressor)
		delete m_poCompressor;
  //## end ClZlibCompress::~ClZlibCompress%926685389.body
}



//## Other Operations (implementation)
m4return_t ClZlibCompress::Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClZlibCompress::Compress%926685385.body preserve=yes
      m_poCompressor->SetBufferIn((const m4puchar_t)ai_pcBufferIn,ai_uiBufferInSize);

      m_poCompressor->Compress(m_iCompressionLevel);

      ao_pcBufferOut=(m4char_t*)m_poCompressor->GetBufferOut();

      ao_uiBufferOutSize=m_poCompressor->GetBufferOutSize(); 
	  
	  return M4_SUCCESS;
  //## end ClZlibCompress::Compress%926685385.body
}

m4return_t ClZlibCompress::Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClZlibCompress::Decompress%926685386.body preserve=yes
	  m_poCompressor->SetBufferIn((const m4puchar_t)ai_pcBufferIn,ai_uiBufferInSize);

      m_poCompressor->Decompress();

      ao_pcBufferOut=(m4char_t*)m_poCompressor->GetBufferOut();
      ao_uiBufferOutSize=m_poCompressor->GetBufferOutSize();
	  return M4_SUCCESS;
  //## end ClZlibCompress::Decompress%926685386.body
}

m4return_t ClZlibCompress::Init (m4int_t ai_iCompressionLevel)
{
  //## begin ClZlibCompress::Init%926685387.body preserve=yes

	if(m_poCompressor)
	{
		m_iCompressionLevel=ai_iCompressionLevel;
		return M4_SUCCESS;
	}

	m_poCompressor=new M4Zlib;
	if(!m_poCompressor)
		return M4_ERROR;

	return M4_SUCCESS;
  //## end ClZlibCompress::Init%926685387.body
}

m4return_t ClZlibCompress::Free ()
{
  //## begin ClZlibCompress::Free%927022005.body preserve=yes
	m_poCompressor->CleanBuffers();
	return M4_SUCCESS;
  //## end ClZlibCompress::Free%927022005.body
}

m4return_t ClZlibCompress::CompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile)
{
  //## begin ClZlibCompress::CompressFile%935144293.body preserve=yes
	if((!ai_pSourceFile) || (!ai_pDestinationFile))
	{
		m4Trace(cerr<<"Invalid FileName"<<endl);
		return M4_ERROR;
	}
	return (m4return_t)m_poCompressor -> CompressFile(ai_pSourceFile,ai_pDestinationFile);
  //## end ClZlibCompress::CompressFile%935144293.body
}

m4return_t ClZlibCompress::DecompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile)
{
  //## begin ClZlibCompress::DecompressFile%935144294.body preserve=yes
	if((!ai_pSourceFile) || (!ai_pDestinationFile))
	{
		m4Trace(cerr<<"Invalid FileName"<<endl);
		return M4_ERROR;
	}
	return (m4return_t)m_poCompressor -> DecompressFile(ai_pSourceFile,ai_pDestinationFile);
  //## end ClZlibCompress::DecompressFile%935144294.body
}

// Additional Declarations
  //## begin ClZlibCompress%373AAEE70095.declarations preserve=yes
  //## end ClZlibCompress%373AAEE70095.declarations

// Class ClNULLCompress 

ClNULLCompress::ClNULLCompress ()
  //## begin ClNULLCompress::ClNULLCompress%926685390.hasinit preserve=no
  //## end ClNULLCompress::ClNULLCompress%926685390.hasinit
  //## begin ClNULLCompress::ClNULLCompress%926685390.initialization preserve=yes
  //## end ClNULLCompress::ClNULLCompress%926685390.initialization
{
  //## begin ClNULLCompress::ClNULLCompress%926685390.body preserve=yes
	m_pcName="NULLCompress";
	m_pcVersion="1.0";
  //## end ClNULLCompress::ClNULLCompress%926685390.body
}


ClNULLCompress::~ClNULLCompress ()
{
  //## begin ClNULLCompress::~ClNULLCompress%926685391.body preserve=yes
  //## end ClNULLCompress::~ClNULLCompress%926685391.body
}



//## Other Operations (implementation)
m4return_t ClNULLCompress::Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClNULLCompress::Compress%926670135.body preserve=yes
	ao_uiBufferOutSize=ai_uiBufferInSize;
	ao_pcBufferOut=ai_pcBufferIn;
	return M4_SUCCESS;
  //## end ClNULLCompress::Compress%926670135.body
}

m4return_t ClNULLCompress::Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize)
{
  //## begin ClNULLCompress::Decompress%926670136.body preserve=yes
	ao_uiBufferOutSize=ai_uiBufferInSize;
	ao_pcBufferOut=ai_pcBufferIn;
	return M4_SUCCESS;
  //## end ClNULLCompress::Decompress%926670136.body
}

m4return_t ClNULLCompress::Init (m4int_t ai_iCompressionLevel)
{
  //## begin ClNULLCompress::Init%926670137.body preserve=yes
	m_iCompressionLevel=ai_iCompressionLevel;
	return M4_SUCCESS;
  //## end ClNULLCompress::Init%926670137.body
}

// Additional Declarations
  //## begin ClNULLCompress%373AAF2A0272.declarations preserve=yes
  //## end ClNULLCompress%373AAF2A0272.declarations

//## begin module%373AE15902E3.epilog preserve=yes
//## end module%373AE15902E3.epilog
