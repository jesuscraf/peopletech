

#include "m4unicode.hpp"
#include <compressdatastorage.hpp>
#include <m4compress.hpp>
#define  M4_COMPRESS_FILE_BUFFER_NEW_SIZE 32000
#define  M4_COMPRESS_FILE_BUFFER_MAX_NEW_SIZE 64000
#include "file_manager.hpp"

m4return_t M4Compress ( M4DataStorage & ao_CompressDataStorage  , 
										M4DataStorage & ai_DeCompressDataStorage ,
										ClCompress  * ai_poCompress,
										m4uint32_t ai_uiCompressionType)
{
	if(ai_uiCompressionType==M4_COMPRESSION_TYPE_NULL)
	{
		m4Trace(cerr<<"no deberia ser llamada para nada"<<endl);
		return M4_ERROR;
	}
	ai_DeCompressDataStorage.InitializeIteration();
	ao_CompressDataStorage.Clear();
	m4uint32_t uiPlainSize=0,uiCompressSize=0;
	m4pchar_t pcPlainBuffer = NULL,pcCompressBuffer = NULL,pcBufferSal=NULL;
	m4return_t iRet,iReturn;
	
	ClDataUnitInterface *pDI;
m4bool_t bPreviousSerializacionState=ai_DeCompressDataStorage.m_bFinishSerialization;
ai_DeCompressDataStorage.m_bFinishSerialization=M4_TRUE;
	while( (iRet = ai_DeCompressDataStorage.GetNext(pDI)) != M4_ERROR )
	{
		switch(pDI->GetType())
		{
		case M4_DATA_UNIT:
			
			ClDataUnit		*poMemDataUnit;
			
			poMemDataUnit = (ClDataUnit *)pDI;
			uiPlainSize=poMemDataUnit->GetSize();
			pcPlainBuffer=poMemDataUnit->GetBuffer();
			ao_CompressDataStorage.GetNewDataUnit(poMemDataUnit); // La reutilizo
			
			iReturn=ai_poCompress->Compress(pcPlainBuffer,uiPlainSize,pcCompressBuffer,uiCompressSize);
			if((iReturn!=M4_SUCCESS) || (!pcCompressBuffer))
			{
				m4Trace(cerr<<"Error comprimiendo"<<endl);
				break;
			}
			poMemDataUnit->SetSize(uiCompressSize);
			pcBufferSal=poMemDataUnit->GetBuffer();
			memcpy(pcBufferSal,pcCompressBuffer,uiCompressSize);
			break;

		case M4_CHECK_UNIT:

			ClCheckUnit*	poMemCheckUnit;
			
			poMemCheckUnit = (ClCheckUnit*) pDI;
			uiPlainSize=poMemCheckUnit->GetSize();
			pcPlainBuffer=poMemCheckUnit->GetBuffer();
			ao_CompressDataStorage.GetNewCheckUnit(poMemCheckUnit); // La reutilizo
			
			iReturn=ai_poCompress->Compress(pcPlainBuffer,uiPlainSize,pcCompressBuffer,uiCompressSize);
			if((iReturn!=M4_SUCCESS) || (!pcCompressBuffer))
			{
				m4Trace(cerr<<"Error comprimiendo"<<endl);
				break;
			}
			poMemCheckUnit->SetSize(uiCompressSize);
			pcBufferSal=poMemCheckUnit->GetBuffer();
			memcpy(pcBufferSal,pcCompressBuffer,uiCompressSize);
			break;

		case M4_FILE_UNIT:
			/*
            if(ai_uiCompressionType==M4_COMPRESSION_TYPE_DATA)
			{
				m4Trace(cerr<<"No hago nada con los ficheros!!!!!\nLos copio tal cual"<<endl);
				ClFileUnit	*poFileUnit;
				m4pchar_t	pszFilename;
				
				poFileUnit = (ClFileUnit *)pDI;
				pszFilename = poFileUnit->GetFileName();
				
				ao_CompressDataStorage.GetNewFileUnit(poFileUnit);
				poFileUnit->SetFile(pszFilename);
			}
			else
            */
			{
				ClFileUnit	*poFileUnit;
				m4pchar_t	pszFilename;
				int iLength = -1 ;
				m4pchar_t pcANSIFile = NULL ;
				
				poFileUnit = (ClFileUnit *)pDI;
				pszFilename = poFileUnit->GetFileName();
				if(!pszFilename)
				{
					m4Trace(cerr<< "NULL filename"<<endl)
				}

				m4return_t	iRet=M4_SUCCESS;
				m4uint32_t	uiHeaderSize=sizeof(uiHeaderSize);
				m4int_t		readed=1,written=0;
				size_t		iSizeBlock = M4_COMPRESS_FILE_BUFFER_NEW_SIZE;
				m4int_t		totalreaded=0;
				m4pchar_t	pcBuffer=new m4char_t[M4_COMPRESS_FILE_BUFFER_NEW_SIZE];
				m4pchar_t	pCompressBuffer;
				m4uint32_t	uiCompressSize;
				m4uint32_t	uiNChunks=0;
				m4pchar_t	outFileName,pcExtension;
				size_t		iExtensionLen = 0;

				pcExtension=poFileUnit->GetFileExtension();
				ClFileManager* myFileManager = ClFileManager::Instance();
				ClFileName myManaFile;
				
				myFileManager -> CreateTempFile(myManaFile, "~cpdu", pcExtension, NULL);
				if ( pcExtension )
					iExtensionLen = strlen ( pcExtension ) ;

				outFileName = new m4char_t[myManaFile.GetLength() + 1 + iExtensionLen +1];
				myManaFile.GetFileName(outFileName);

				iLength = -1 ;
				pcANSIFile = M4CppToANSI( outFileName, iLength ) ;
				ofstream *output=new ofstream(pcANSIFile, ios::out | ios::trunc | ios::binary );
				delete( pcANSIFile ) ;

				if(!output)
				{
					m4Trace(cerr<<"Cannot create "<<outFileName<<endl);
					continue;
				}

				iLength = -1 ;
				pcANSIFile = M4CppToANSI( pszFilename, iLength ) ;
				ifstream *input=new ifstream(pcANSIFile, ios::in  | M4_IOS_OPEN_MODE_NO_CREATE | ios::binary ) ;
				delete( pcANSIFile ) ;

				if(!input)
				{
					m4Trace(cerr<<"Cannot read from "<<pszFilename<<endl);
					continue;
				}	
				
				while(readed)
				{
					input->read( pcBuffer , iSizeBlock );
					readed = (m4int_t) input->gcount();
					if(readed)
					{
						ai_poCompress->Compress(pcBuffer,readed,pCompressBuffer,uiCompressSize);
						m4TraceLevel(2,cout<<"De "<<readed<<" a "<<uiCompressSize<<endl);
					
// Usamos formato independiente de little o big endian
						m4char_t pcHeader[sizeof(uiCompressSize)];
						m4uint32_t  aux_uiCompressSize=uiCompressSize;
						for(int i=0;i<sizeof(uiCompressSize);i++)
						{
							pcHeader[i]=aux_uiCompressSize%256;
							aux_uiCompressSize/=256;
						}
						
						output->write(pcHeader,uiHeaderSize);

						output->write(pCompressBuffer,uiCompressSize);
						written+=uiHeaderSize;
						written+=uiCompressSize;
						totalreaded+=readed;
					}
				}
				m4TraceLevel(2,cout<<"Readed "<<totalreaded<<" bytes."<<endl);
				m4TraceLevel(2,cout<<"Written "<<written<<" bytes."<<endl);
				m4TraceLevel(2,cout<<"Ratio:"<<  100.0*(float)written/(float)totalreaded<<endl);
				
				if(pcBuffer)
					delete pcBuffer;
				
				// Ahora hago que el datastorage apunte al nuevo fichero comprimido.
				ao_CompressDataStorage.GetNewFileUnit(poFileUnit);
				output->close();
				input->close();
				poFileUnit->SetFile(outFileName);
				poFileUnit->SetCompressed();

				myFileManager ->UnSetInterestInFile(myManaFile);
				
				if(output)
				{ delete output;
				output=NULL; }

				if(input)
				{ delete input;
				input=NULL;}

				if(outFileName)
					delete outFileName;
			}
			
			break;
			
		default:
			M4_ASSERT(0);
			break;
			
		}
		
	}

ai_DeCompressDataStorage.m_bFinishSerialization=bPreviousSerializacionState;

	return iReturn;
}

m4return_t M4Decompress ( M4DataStorage & ai_CompressDataStorage  , 
										  M4DataStorage & ao_DeCompressDataStorage,
										  ClCompress  * ai_poCompress ,
										  m4uint32_t ai_uiCompressionType) 
{
	ai_CompressDataStorage.InitializeIteration();
	ao_DeCompressDataStorage.Clear();
	m4uint32_t uiPlainSize=0,uiCompressSize=0;
	m4pchar_t pcPlainBuffer = NULL,pcCompressBuffer = NULL,pcBufferSal=NULL;
	m4return_t iRet,iReturn;
	
	ClDataUnitInterface *pDI;
m4bool_t bPreviousSerializacionState=ai_CompressDataStorage.m_bFinishSerialization;
ai_CompressDataStorage.m_bFinishSerialization=M4_TRUE;	
	while( (iRet = ai_CompressDataStorage.GetNext(pDI)) != M4_ERROR )
	{
		switch(pDI->GetType())
		{
		case M4_DATA_UNIT:
			
			ClDataUnit		*poMemDataUnit;
			
			poMemDataUnit = (ClDataUnit *)pDI;
			uiPlainSize=poMemDataUnit->GetSize();
			pcPlainBuffer=poMemDataUnit->GetBuffer();
			
			iReturn=ai_poCompress->Decompress(pcPlainBuffer,uiPlainSize,pcCompressBuffer,uiCompressSize);
			if((iReturn!=M4_SUCCESS) || (!pcCompressBuffer))
			{
				m4Trace(cerr<<"Error comprimiendo"<<endl);
				break;
			}
			
			ao_DeCompressDataStorage.GetNewDataUnit(poMemDataUnit); // La reutilizo
			poMemDataUnit->SetSize(uiCompressSize);
			pcBufferSal=poMemDataUnit->GetBuffer();
			memcpy(pcBufferSal,pcCompressBuffer,uiCompressSize);
			break;

		case M4_CHECK_UNIT:
			
			ClCheckUnit		*poMemCheckUnit;
			
			poMemCheckUnit = (ClCheckUnit*)pDI;
			uiPlainSize=poMemCheckUnit->GetSize();
			pcPlainBuffer=poMemCheckUnit->GetBuffer();
			
			iReturn=ai_poCompress->Decompress(pcPlainBuffer,uiPlainSize,pcCompressBuffer,uiCompressSize);
			if((iReturn!=M4_SUCCESS) || (!pcCompressBuffer))
			{
				m4Trace(cerr<<"Error descomprimiendo"<<endl);
				break;
			}
			
			ao_DeCompressDataStorage.GetNewCheckUnit(poMemCheckUnit); // La reutilizo
			poMemCheckUnit->SetSize(uiCompressSize);
			pcBufferSal=poMemCheckUnit->GetBuffer();
			memcpy(pcBufferSal,pcCompressBuffer,uiCompressSize);
			break;

		case M4_FILE_UNIT:
			/*
            if(ai_uiCompressionType==M4_COMPRESSION_TYPE_DATA)
			{
				m4Trace(cerr<<"No hago nada con los ficheros!!!!!\nLos copio tal cual"<<endl);
				ClFileUnit	*poFileUnit;
				m4pchar_t	pszFilename;
				
				poFileUnit = (ClFileUnit *)pDI;
				pszFilename = poFileUnit->GetFileName();
				
				ao_DeCompressDataStorage.GetNewFileUnit(poFileUnit);
				poFileUnit->SetFile(pszFilename);
			}
			else
            */
			{
				ClFileUnit	*poFileUnit;
				m4pchar_t	pszFilename;
				int iLength = -1 ;
				m4pchar_t pcANSIFile = NULL ;
				
				poFileUnit = (ClFileUnit *)pDI;
				pszFilename = poFileUnit->GetFileName();
				if(!pszFilename)
				{
					m4Trace(cerr<< "NULL filename"<<endl)
				}
				/// COMPRESION

				iLength = -1 ;
				pcANSIFile = M4CppToANSI( pszFilename, iLength ) ;
				ifstream *input=new ifstream(pcANSIFile, ios::in  | M4_IOS_OPEN_MODE_NO_CREATE | ios::binary ) ;
				delete( pcANSIFile ) ;

				if(!input)
				{
					m4Trace(cerr<<"Cannot read from "<<pszFilename<<endl);
					continue;
				}
				m4pchar_t	outFileName,pcExtension;
				size_t		iExtensionLen = 0;
				
				pcExtension=poFileUnit->GetFileExtension();
				ClFileManager* myFileManager = ClFileManager::Instance();
				ClFileName myManaFile;
				
				myFileManager -> CreateTempFile(myManaFile, "~dpdu", pcExtension, NULL);
				if ( pcExtension )
					iExtensionLen = strlen ( pcExtension ) ;
				outFileName = new m4char_t[myManaFile.GetLength() + 1 + iExtensionLen +1];
				myManaFile.GetFileName(outFileName);

				iLength = -1 ;
				pcANSIFile = M4CppToANSI( outFileName, iLength ) ;
				ofstream *output=new ofstream(pcANSIFile, ios::out | ios::trunc | ios::binary );
				delete( pcANSIFile ) ;

				if(!output)
				{
					m4Trace(cerr<<"Cannot create "<<outFileName<<endl);
					continue;
				}
				m4return_t iRet=M4_SUCCESS;
				m4uint32_t uiHeaderSize=sizeof(uiHeaderSize);
				m4int_t readed=1,written=0,iSizeBlock=0;
				m4int_t totalreaded=0;
				m4pchar_t pcBuffer= NULL;
				m4pchar_t pDeCompressBuffer;
				m4uint32_t uiDeCompressSize;
				m4uint32_t uiChunksize=0;
				m4uint32_t uiNChunks=0;
				m4uint32_t partialreaded=0;
				while(readed)
				{
					uiChunksize=0;

// Usamos formato independiente de little o big endian
					m4char_t pcHeader[sizeof(uiHeaderSize)];
					input->read(pcHeader , uiHeaderSize );
					
					for(int i=sizeof(uiHeaderSize)-1;i>=0;i--)
					{
						uiChunksize*=256;
						uiChunksize+=(unsigned char)pcHeader[i];
					}
						
					readed= (m4int_t) input->gcount();
					if(readed==uiHeaderSize)
					{
						if(uiChunksize>M4_COMPRESS_FILE_BUFFER_MAX_NEW_SIZE)
						{
							m4Trace(cerr<<"Error.Buffer too small"<<endl);
							iRet=M4_ERROR;
							break;
						}
						else
						{
							// fix bug 0081085
							// allocates memory
							if (uiChunksize > iSizeBlock) {
								iSizeBlock = uiChunksize;
								if (iSizeBlock < M4_COMPRESS_FILE_BUFFER_NEW_SIZE) {
									iSizeBlock = M4_COMPRESS_FILE_BUFFER_NEW_SIZE;
								}
								if (pcBuffer != NULL) {
									delete [] pcBuffer;
								}
								pcBuffer = new m4char_t [iSizeBlock];
							}

							partialreaded=0;
							while(partialreaded!=uiChunksize)
							{
								input->read(pcBuffer+partialreaded,uiChunksize-partialreaded);
								readed= (m4int_t) input->gcount();
								partialreaded+=readed;
								if((partialreaded!=uiChunksize) && (!readed))
								{
									m4Trace(cerr<<"Error!!!!!"<<endl);
									iRet=M4_ERROR;
									break;
								}
							}
							ai_poCompress->Decompress(pcBuffer,uiChunksize,pDeCompressBuffer,uiDeCompressSize);
							m4TraceLevel(2,cout<<"De "<<uiChunksize<<" a "<<uiDeCompressSize<<endl);
							output->write(pDeCompressBuffer,uiDeCompressSize);
							written+=uiDeCompressSize;
							totalreaded+=readed;
							totalreaded+=uiHeaderSize;
						}
					}
					else
					{
						if(readed)
						{
							m4Trace(cerr<<"Invalid format"<<endl);
							iRet=M4_ERROR;
							break;
						}
					}
				}
				m4TraceLevel(2,cout<<"Readed "<<totalreaded<<" bytes."<<endl);
				m4TraceLevel(2,cout<<"Written "<<written<<" bytes."<<endl);
				m4TraceLevel(2,cout<<"Ratio:"<<  100.0*(float)written/(float)totalreaded<<endl);
				
				if(pcBuffer) {
					delete [] pcBuffer;
				}
				
				ao_DeCompressDataStorage.GetNewFileUnit(poFileUnit);
				output->close();
				input->close();
				poFileUnit->SetFile(outFileName);
				myFileManager -> UnSetInterestInFile(myManaFile);
				if(input)
				{ delete input;
				input=NULL;	}
				if(output)
				{ delete output;
				output=NULL;}
				if(outFileName)
					delete outFileName;
			}
			break;
			
		default:
			M4_ASSERT(0);
			break;
			
		}
	}

ai_CompressDataStorage.m_bFinishSerialization=bPreviousSerializacionState;	
ao_DeCompressDataStorage.m_bFinishSerialization=M4_TRUE;	
	return iReturn;
}

m4return_t M4DSDecompress ( M4DataStorage & ai_CompressDataStorage, 
											M4DataStorage & ao_DeCompressDataStorage,
											m4pchar_t ai_pcCompressType,
											m4uint32_t ai_uiCompressionType) 
{
	// if no compression, return.
    if(M4_COMPRESSION_TYPE_NULL == ai_uiCompressionType)
	{
		return M4_ERROR;
	}
    
    // if zdelta compression, but there is not files, return.
    // Compression will be done at the communication level.
    m4bool_t hasFiles = ai_CompressDataStorage.HasFileUnits();
    if ((M4_COMPRESSION_TYPE_ZDELTA == ai_uiCompressionType) && (M4_FALSE == hasFiles))
    {
		return M4_ERROR;
    }

	ClCompressFactory thefactory;
	ClCompress* theCompressor = thefactory.GetCompress(ai_pcCompressType, NULL);
    if(NULL == theCompressor)
    {
		return M4_ERROR;
    }

	m4return_t iRet=M4Decompress(ai_CompressDataStorage,ao_DeCompressDataStorage,theCompressor,ai_uiCompressionType);
	if (theCompressor)
	{
		delete theCompressor;
		theCompressor=NULL;
	}

	return iRet;
}

m4return_t M4DSCompress ( M4DataStorage & ao_CompressDataStorage  , 
										  M4DataStorage & ai_DeCompressDataStorage ,
										  m4pchar_t ai_pcCompressType,
										  m4uint32_t ai_uiCompressionType,
										  m4uint32_t ai_uiCompressionLevel)
{
	// if no compression, return.
    if(M4_COMPRESSION_TYPE_NULL == ai_uiCompressionType)
	{
		return M4_ERROR;
	}
    
    // if zdelta compression, but there is not files, return.
    // Compression will be done at the communication level.
    m4bool_t hasFiles = ai_DeCompressDataStorage.HasFileUnits();
    if ((M4_COMPRESSION_TYPE_ZDELTA == ai_uiCompressionType) && (M4_FALSE == hasFiles))
    {
		return M4_ERROR;
    }

    // otherwise, use zlib compression.
    ClCompressFactory thefactory;
	ClCompress* theCompressor=thefactory.GetCompress(ai_pcCompressType, NULL);
    if(NULL == theCompressor)
    {
		return M4_ERROR;
    }
    theCompressor->SetCompressionLevel(9);

	m4return_t iRet = M4Compress(ao_CompressDataStorage, ai_DeCompressDataStorage, theCompressor, ai_uiCompressionType);
	if (NULL != theCompressor)
	{
		delete theCompressor;
		theCompressor=NULL;
	}

	return iRet;
}

