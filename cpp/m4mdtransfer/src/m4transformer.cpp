//## begin module%407BEBBD0157.cm preserve=no
//## end module%407BEBBD0157.cm

//## begin module%407BEBBD0157.cp preserve=no
//	=====================================================================
//
//	 (c) Copyright 2004 Meta4 Spain S.A.
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              <file>.cpp
//	 Project:           <project>
//	 Author:            Meta4 Spain S.A.
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS, UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=====================================================================
//## end module%407BEBBD0157.cp

//## Module: m4transformer%407BEBBD0157; Package body
//## Subsystem: M4MDTransfer::M4Transformer::src%407ABB4D003C
//## Source file: C:\m4server\m4mdtransfer\src\m4transformer.cpp

//## begin module%407BEBBD0157.additionalIncludes preserve=no
//## end module%407BEBBD0157.additionalIncludes

//## begin module%407BEBBD0157.includes preserve=yes
//## end module%407BEBBD0157.includes

#include <m4imdtdictionary.hpp>
#include <m4saxtransformer.hpp>
#include <m4mdtsaxmerger.hpp>
#include <m4mdtsaxrichtransformer.hpp>
#include <m4itransformer.hpp>
#include <m4transformer.hpp>
#include <m4mdtdef.hpp>
#include <m4mdtres.hpp>
#include <m4imdtservice.hpp>
#include "m4mdtsaxupdater.hpp"
#include "m4unicode.hpp"

#include <xercesc/parsers/SAXParser.hpp>



// Class ClMDTTransformer 



ClMDTTransformer::ClMDTTransformer ()
  //## begin ClMDTTransformer::ClMDTTransformer%1081852893.hasinit preserve=no
      : m_bInit(M4_FALSE), m_poService(NULL)
  //## end ClMDTTransformer::ClMDTTransformer%1081852893.hasinit
  //## begin ClMDTTransformer::ClMDTTransformer%1081852893.initialization preserve=yes
  //## end ClMDTTransformer::ClMDTTransformer%1081852893.initialization
{
  //## begin ClMDTTransformer::ClMDTTransformer%1081852893.body preserve=yes
  //## end ClMDTTransformer::ClMDTTransformer%1081852893.body
}



//## Other Operations (implementation)
m4return_t ClMDTTransformer::Init (IMDTService* ai_poService)
{
  //## begin ClMDTTransformer::Init%1081852892.body preserve=yes
	m_poService = ai_poService;
	m_bInit = M4_TRUE;
	return M4_SUCCESS;
  //## end ClMDTTransformer::Init%1081852892.body
}

m4return_t ClMDTTransformer::GenerateXMLObjectData (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccIdObject, m4int32_t ai_iPKs, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccXMLExclusionFileName, m4int32_t ao_iChilds)
{
  //## begin ClMDTTransformer::GenerateXMLObjectData%1081852891.body preserve=yes
	M4MDT_RETURN_ERROR0(m_bInit == M4_FALSE , M4_ERROR, M4MDTERROR_TR_NOT_INITIALIZED);

	try
	{
		// gets the current object definition
		InputSource *poObjSource = NULL;
		m4return_t iRes = m_poService->GetIDictionary()->GetXMLObject(ai_pccIdObject,poObjSource);

		M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_OBJ_GENERATION, ai_pccIdObject, "%s");

		iRes = ParseObject(ai_pccIdConnection,ai_pccIdObject,poObjSource,ai_iPKs,ai_pccXMLFileName,ai_pccXMLExclusionFileName,ao_iChilds);

		m4return_t iResult = m_poService->GetIDictionary()->ReleaseXMLObject( poObjSource ) ;
		M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::GenerateXMLObjectData", "ReleaseXMLObject", "%s#%s" ) ;

		M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_PARSING_OBJ, ai_pccIdObject, "%s");

	}
	catch( const XMLException& e )
	{
		// UNICODE XML
		m4pchar_t pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;

		return( M4_ERROR ) ;
	}

	return M4_SUCCESS;
  //## end ClMDTTransformer::GenerateXMLObjectData%1081852891.body
}

m4return_t ClMDTTransformer::GenerateRichXMLObjectData (m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName)
{
  //## begin ClMDTTransformer::GenerateRichXMLObjectData%1084182898.body preserve=yes
	m4return_t iRes = M4_SUCCESS;
	m4pchar_t	pcException = NULL ;

	// Check XML data file.
	ifstream ifStream;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	M4Open( ifStream, ai_pccXMLFileName, M4UniRead, eEncoding ) ;

	M4MDT_RETURN_ERROR1(ifStream.rdbuf()->is_open() == 0, M4_ERROR, M4MDTERROR_NO_XML_DATA_FILE, ai_pccXMLFileName, "%s");
	ifStream.close();

	try
	{
		// Create SAX rich transformer.
		ClMDTSAXRichTransformer oSAXRichTransformer;
		
		// Init SAX rich transformer.
		if (oSAXRichTransformer.Init(m_poService, ai_pccRichXMLFileName) != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::GenerateRichXMLObjectData", "oSAXRichTransformer.Init", "%s#%s");
			return M4_ERROR;
		}

		// Create the SAX parser without validation.
		SAXParser oSaxHandler;
		oSaxHandler.setValidationScheme(SAXParser::Val_Never);

		// Invoque the parser
		oSaxHandler.setDocumentHandler(&oSAXRichTransformer);
		oSaxHandler.setErrorHandler(&oSAXRichTransformer);

		// Parse the xml input
		oSaxHandler.parse(ai_pccXMLFileName);

		iRes = oSAXRichTransformer.GetError();
	}
	catch( const XMLException& e )
	{
		// UNICODE XML
		pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;

		iRes = M4_ERROR;
	}

	if (iRes != M4_SUCCESS)
	{
		// If something goes wrong, remove XML rich data file.
		remove(ai_pccRichXMLFileName);
		M4MDT_RETURN_ERROR3(iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::GenerateRichXMLObjectData", "oSaxHandler.parse", ai_pccXMLFileName, "%s#%s(%s)");
	}

	return M4_SUCCESS;
  //## end ClMDTTransformer::GenerateRichXMLObjectData%1084182898.body
}

m4return_t ClMDTTransformer::UpdateDB (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName)
{
  //## begin ClMDTTransformer::UpdateDB%1084188477.body preserve=yes
	
	m4return_t	iResult = M4_ERROR ;
	m4pchar_t	pcException = NULL ;
	
	
	try
	{
		iResult = m_poService->GetIODBC()->BeginTransaction( ai_pccIdConnection ) ;
		M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::UpdateDB", "BeginTransaction", "%s#%s" ) ;

		iResult = _UpdateDBNoTransaction( ai_pccIdConnection, ai_pccXMLFileName, ai_pccRichXMLFileName ) ;
	}
	catch( const XMLException& e )
	{
		// UNICODE XML
		pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;

		iResult = M4_ERROR ;
	}
	
	if( iResult != M4_SUCCESS )
	{
		iResult = m_poService->GetIODBC()->RollBack( ai_pccIdConnection ) ;
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::UpdateDB", "_UpdateDBNoTransaction", "%s#%s" ) ;
		return( M4_ERROR ) ;
	}

	iResult = m_poService->GetIODBC()->Commit( ai_pccIdConnection ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::UpdateDB", "EndTransaction", "%s#%s" ) ;

	
	return( M4_SUCCESS ) ;
  //## end ClMDTTransformer::UpdateDB%1084188477.body
}

m4return_t ClMDTTransformer::_UpdateDBNoTransaction (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccRichXMLFileName)
{
  //## begin ClMDTTransformer::_UpdateDBNoTransaction%1084288689.body preserve=yes

	m4return_t	iResult = M4_ERROR ;
	XMLCh		*wsFile = NULL ;
	
	
	ClMDTSaxDeleter	oSaxDeleterHandler;
	
	iResult = _UpdateDB( ai_pccIdConnection, ai_pccXMLFileName, ai_pccXMLFileName, oSaxDeleterHandler ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::UpdateDB", "_UpdateDB", "%s#%s" ) ;
	
	
	ClMDTSaxInserter	oSaxInserterHandler;
	
	/* Bug 0100099
	El directorio para los ficheros externos debe ser el mismo del que se hace el parseo
	*/
	iResult = _UpdateDB( ai_pccIdConnection, ai_pccRichXMLFileName, ai_pccRichXMLFileName, oSaxInserterHandler ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::UpdateDB", "_UpdateDB", "%s#%s" ) ;
	
	
	return( M4_SUCCESS ) ;
  //## end ClMDTTransformer::_UpdateDBNoTransaction%1084288689.body
}

m4return_t ClMDTTransformer::_UpdateDB (m4pcchar_t ai_pccIdConnection, m4pcchar_t ai_pccXMLBaseFileName, m4pcchar_t ai_pccXMLFileName, ClMDTSaxUpdater& ai_roSaxUpdater)
{
  //## begin ClMDTTransformer::_UpdateDB%1084188478.body preserve=yes

	m4return_t	iResult = M4_ERROR ;


	iResult = ai_roSaxUpdater.Init( ai_pccIdConnection, m_poService->GetSessionDate(), ai_pccXMLBaseFileName, m_poService->GetDebugDir(), m_poService->GetIODBC(), m_poService->GetIDictionary(), m_poService->GetIPresentationTool() ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::_UpdateDB", "ai_roSaxUpdater.Init", "%s#%s" ) ;

	SAXParser	oParser;

	// void
	oParser.setValidationScheme(SAXParser::Val_Never);
	oParser.setDocumentHandler( &ai_roSaxUpdater ) ;
	oParser.setErrorHandler( &ai_roSaxUpdater ) ;

	oParser.parse( ai_pccXMLFileName ) ;

	iResult = ai_roSaxUpdater.GetError() ;
	M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::_UpdateDB", "oParser.parse", ai_pccXMLFileName, "%s#%s(%s)" ) ;

	return( M4_SUCCESS ) ;
  //## end ClMDTTransformer::_UpdateDB%1084188478.body
}

m4return_t ClMDTTransformer::ParseObject (m4pcchar_t ai_pccConnectionId, m4pcchar_t ai_pccObjId, InputSource* ai_oInput, m4int32_t ai_iPKs, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccXMLExclusionFileName, m4int32_t ao_iChilds)
{
  //## begin ClMDTTransformer::ParseObject%1081852894.body preserve=yes

	m4return_t iRes = M4_SUCCESS;

	// creates the Handler
	ClMDTDataSAXTransformer oSaxHandler(m_poService,string(ai_pccConnectionId));
	iRes = oSaxHandler.Reset(ai_pccConnectionId,ai_pccObjId,ai_iPKs,ai_pccXMLFileName,ai_pccXMLExclusionFileName,ao_iChilds);
	M4MDT_RETURN_ERROR0(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_RESET);
		
	// creates the SAX parser without validation (improve speed)
	SAXParser *poParser = new SAXParser;
	poParser->setValidationScheme(SAXParser::Val_Never);

	// invoques the parser
    poParser->setDocumentHandler(&oSaxHandler);
    poParser->setErrorHandler(&oSaxHandler);

	// parses the xml input
    poParser->parse(*ai_oInput);

	iRes = oSaxHandler.GetError();
	M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::ParseObject", "poParser->parse", "%s#%s" ) ;

	// deletes the parser
	delete poParser;

	return M4_SUCCESS;
  //## end ClMDTTransformer::ParseObject%1081852894.body
}

m4return_t ClMDTTransformer::MergeXMLObjectData (m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccExclusionXMLFileName, m4pcchar_t ai_pccMergedXMLFileName)
{
  //## begin ClMDTTransformer::MergeXMLObjectData%1174296500.body preserve=yes
	m4return_t iRes = M4_SUCCESS;
	m4pchar_t	pcException = NULL ;

	// Check XML data file.
	ifstream ifStream;

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	M4Open( ifStream, ai_pccXMLFileName, M4UniRead, eEncoding ) ;

	M4MDT_RETURN_ERROR1(ifStream.rdbuf()->is_open() == 0, M4_ERROR, M4MDTERROR_NO_XML_DATA_FILE, ai_pccXMLFileName, "%s");
	ifStream.close();

	try
	{
		// Create SAX merger.
		ClMDTSAXMerger oSAXMerger;
		
		// Init SAX merger.
		if (oSAXMerger.Init(m_poService, ai_pccExclusionXMLFileName, ai_pccMergedXMLFileName) != M4_SUCCESS)
		{
			M4MDT_DUMP_ERROR2(M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::MergeXMLObjectData", "oSAXRichTransformer.Init", "%s#%s");
			return M4_ERROR;
		}

		// Create the SAX parser without validation.
		SAXParser oSaxHandler;
		oSaxHandler.setValidationScheme(SAXParser::Val_Never);

		// Invoque the parser
		oSaxHandler.setDocumentHandler(&oSAXMerger);
		oSaxHandler.setErrorHandler(&oSAXMerger);

		// Parse the xml input
		oSaxHandler.parse(ai_pccXMLFileName);

		iRes = oSAXMerger.GetError();
	}
	catch(const XMLException& e)
	{
		// UNICODE XML
		pcException = M4XMLToCpp((M4XMLCh*)e.getMessage()) ;
		M4MDT_DUMP_ERROR1(M4MDTERROR_EXCEPTION, pcException, "%s");
		delete [] pcException;

		iRes = M4_ERROR;
	}

	if (iRes != M4_SUCCESS)
	{
		// If something goes wrong, remove XML rich data file.
		remove(ai_pccMergedXMLFileName);
		M4MDT_RETURN_ERROR3(iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformer::MergeXMLObjectData", "oSaxHandler.parse", ai_pccXMLFileName, "%s#%s(%s)");
	}

	return M4_SUCCESS;
  //## end ClMDTTransformer::MergeXMLObjectData%1174296500.body
}

m4void_t ClMDTTransformer::End ()
{
  //## begin ClMDTTransformer::End%1081852908.body preserve=yes
	// nothing to do!
  //## end ClMDTTransformer::End%1081852908.body
}

// Additional Declarations
  //## begin ClMDTTransformer%407BEB2C004E.declarations preserve=yes
  //## end ClMDTTransformer%407BEB2C004E.declarations

//## begin module%407BEBBD0157.epilog preserve=yes
//## end module%407BEBBD0157.epilog
