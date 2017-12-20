//## begin module%407ABB81030B.cm preserve=no
//## end module%407ABB81030B.cm

//## begin module%407ABB81030B.cp preserve=no
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
//## end module%407ABB81030B.cp

//## Module: m4saxtransformer%407ABB81030B; Package body
//## Subsystem: M4MDTransfer::M4Transformer::src%407ABB4D003C
//## Source file: D:\Work\v600\m4mdtransfer\src\m4saxtransformer.cpp

//## begin module%407ABB81030B.additionalIncludes preserve=no
//## end module%407ABB81030B.additionalIncludes

//## begin module%407ABB81030B.includes preserve=yes
//## end module%407ABB81030B.includes

#include <m4transctx.hpp>
#include <m4imdtparameter.hpp>
#include <m4imdtdictionary.hpp>
#include <m4saxtransformer.hpp>
#include <xmlstreamgenerator.hpp>
#include <m4xmlbase.hpp>
#include <m4mdtres.hpp>
#include <m4mdtdef.hpp>
#include <m4mdtutils.hpp>
#include <m4imdtservice.hpp>
#include "m4unicode.hpp"

#include <xercesc/parsers/SAXParser.hpp>



// Class ClMDTDataSAXTransformer 




ClMDTDataSAXTransformer::ClMDTDataSAXTransformer (IMDTService* ai_poService, string& ai_sConnectionId)
  //## begin ClMDTDataSAXTransformer::ClMDTDataSAXTransformer%1081852904.hasinit preserve=no
      :
	m_iPKs(0),
	m_iChilds(0),
	m_bInit(M4_FALSE),
	m_poService(ai_poService),
	m_iLevel(0),
	m_oCManager(ai_poService,ai_sConnectionId,m_poOutput),
	m_poOutput(NULL),
	m_poOutStream(NULL),
	m_poExclusionOutStream(NULL)

  //## end ClMDTDataSAXTransformer::ClMDTDataSAXTransformer%1081852904.hasinit
  //## begin ClMDTDataSAXTransformer::ClMDTDataSAXTransformer%1081852904.initialization preserve=yes
  //## end ClMDTDataSAXTransformer::ClMDTDataSAXTransformer%1081852904.initialization
{
  //## begin ClMDTDataSAXTransformer::ClMDTDataSAXTransformer%1081852904.body preserve=yes

	// RegisterElementProc("command", (ElementProcStart)&M4SAXAdmClient::ProcCommand);

	// Methods registration: start
	RegisterElementProc("object",(ElementProcStart)&ClMDTDataSAXTransformer::procObject);
	RegisterElementProc("call-table",(ElementProcStart)&ClMDTDataSAXTransformer::procCallTable);
	RegisterElementProc("read-table",(ElementProcStart)&ClMDTDataSAXTransformer::procReadTable);
	RegisterElementProc("with-pk",(ElementProcStart)&ClMDTDataSAXTransformer::procWithPK);
	RegisterElementProc("field",(ElementProcStart)&ClMDTDataSAXTransformer::procField);
	RegisterElementProc("include-object",(ElementProcStart)&ClMDTDataSAXTransformer::procIncludeObject);
	RegisterElementProc("with-param",(ElementProcStart)&ClMDTDataSAXTransformer::procWithParam);
	RegisterElementProc("call-object",(ElementProcStart)&ClMDTDataSAXTransformer::procCallObject);
	RegisterElementProc("call-template",(ElementProcStart)&ClMDTDataSAXTransformer::procCallTemplate);
	RegisterElementProc("with-arg",(ElementProcStart)&ClMDTDataSAXTransformer::procWithArg);
	RegisterElementProc("generate-funtions",(ElementProcStart)&ClMDTDataSAXTransformer::procGenerateFunctions);
	RegisterElementProc("update-funtions",(ElementProcStart)&ClMDTDataSAXTransformer::procUpdateFunctions);
	RegisterElementProc("calculated-fields",(ElementProcStart)&ClMDTDataSAXTransformer::procCalculatedFields);
	RegisterElementProc("option",(ElementProcStart)&ClMDTDataSAXTransformer::procOptions);
	RegisterElementProc("param",(ElementProcStart)&ClMDTDataSAXTransformer::procParam);

	// Methods registration: end
	RegisterElementEndProc("object",(ElementProcEnd)&ClMDTDataSAXTransformer::procEndObject);
	RegisterElementEndProc("call-table",(ElementProcEnd)&ClMDTDataSAXTransformer::procEndCallTable);
	RegisterElementEndProc("read-table",(ElementProcEnd)&ClMDTDataSAXTransformer::procEndReadTable);
	RegisterElementEndProc("include-object",(ElementProcEnd)&ClMDTDataSAXTransformer::procEndIncludeObject);
	RegisterElementEndProc("call-object",(ElementProcEnd)&ClMDTDataSAXTransformer::procEndCallObject);
	RegisterElementEndProc("calculated-fields",(ElementProcEnd)&ClMDTDataSAXTransformer::procEndCalculatedFields);
	
  //## end ClMDTDataSAXTransformer::ClMDTDataSAXTransformer%1081852904.body
}


ClMDTDataSAXTransformer::~ClMDTDataSAXTransformer ()
{
  //## begin ClMDTDataSAXTransformer::~ClMDTDataSAXTransformer%1081852911.body preserve=yes
	Release();

	// checks for void file or errors
	if (m_oCManager.GetHasData() == M4_FALSE || GetError() == M4_ERROR) {
		// removes the output file
		m4int32_t iRes = unlink(m_sOutputName.c_str());
		M4MDT_CHECK_ERROR2( iRes < 0, M4MDTERROR_NO_DELETE_FILE, m_sOutputName.c_str(), strerror( errno ), "%s#%s" ) ;
	}

	// checks for void file or errors
	if (m_oCManager.GetHasExcludedData() == M4_FALSE || GetError() == M4_ERROR) {
		// removes the output file
		m4int32_t iRes = unlink(m_sExclusionOutputName.c_str());
		M4MDT_CHECK_ERROR2( iRes < 0, M4MDTERROR_NO_DELETE_FILE, m_sExclusionOutputName.c_str(), strerror( errno ), "%s#%s" ) ;
	}
  //## end ClMDTDataSAXTransformer::~ClMDTDataSAXTransformer%1081852911.body
}



//## Other Operations (implementation)
m4return_t ClMDTDataSAXTransformer::GetValue (const string& ai_sName, string& aio_sValue)
{
  //## begin ClMDTDataSAXTransformer::GetValue%1086694050.body preserve=yes
	if (aio_sValue.c_str()[0] != '$') {
		return M4_SUCCESS;
	}

	ClMDTValue sRes;
	m4return_t iRes = m_oCManager.GetCurrent().ProcValue(aio_sValue,0,sRes);

	if (iRes != M4_SUCCESS) {
		SetError(M4_ERROR);
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_INVALID_PARAM, ai_sName.c_str(), "%s");
		SetError(M4_ERROR);
		return M4_ERROR;
	}

	aio_sValue = sRes;
	return M4_SUCCESS;
  //## end ClMDTDataSAXTransformer::GetValue%1086694050.body
}

m4void_t ClMDTDataSAXTransformer::endDocument ()
{
  //## begin ClMDTDataSAXTransformer::endDocument%1081852902.body preserve=yes
  //## end ClMDTDataSAXTransformer::endDocument%1081852902.body
}

m4void_t ClMDTDataSAXTransformer::startDocument ()
{
  //## begin ClMDTDataSAXTransformer::startDocument%1081852903.body preserve=yes
	m4return_t iRes = WriteInitMessage();

	M4MDT_VOID_ERROR0(iRes != M4_SUCCESS , M4MDTERROR_TR_NOT_INIT);
	
  //## end ClMDTDataSAXTransformer::startDocument%1081852903.body
}

m4return_t ClMDTDataSAXTransformer::Reset (m4pcchar_t ai_pccConnectionId, m4pcchar_t ai_pccObjId, m4int32_t ai_iPKs, m4pcchar_t ai_pccXMLFileName, m4pcchar_t ai_pccXMLExclusionFileName, m4int32_t ao_iChilds)
{
  //## begin ClMDTDataSAXTransformer::Reset%1081852905.body preserve=yes

	M4XMLStreamGenerator	*poStreamGenerator = NULL;
	M4XMLStreamGenerator	*poExclusionStreamGenerator = NULL;

	Release();

	m_sOutputName = ai_pccXMLFileName;

	// Si ya existe el fichero no se genera otra vez y sólo se sacan los hijos
	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	FILE	*pfFile = M4Fopen( ai_pccXMLFileName, M4UniRead, eEncoding ) ;

	if( pfFile != NULL )
	{
		poStreamGenerator = NULL;
		fclose( pfFile ) ;
	}
	else
	{
		m_poOutStream = new ofstream;
		M4MDT_RETURN_ERROR1( m_poOutStream == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( ofstream ), "%d" ) ;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		M4Open( *m_poOutStream, ai_pccXMLFileName, M4UniWriteBinary, eEncoding ) ;

		M4MDT_RETURN_ERROR2(m_poOutStream->is_open() == 0 , M4_ERROR, M4MDTERROR_NO_OPEN_FILE, ai_pccXMLFileName, strerror(errno), "%s#%s");

		// UNICODE XML
		poStreamGenerator = new M4Utf8XMLStreamGenerator(*m_poOutStream);
	}

	m_sExclusionOutputName = ai_pccXMLExclusionFileName;

	// Sólo abrimos el fichero de campos excluidos, si el path no está vacío.
	if (m_sExclusionOutputName.empty() == M4_FALSE)
	{
		// UNICODE FILE
		eEncoding = M4UniANSI ;
		FILE	*pfExclusionFile = M4Fopen( ai_pccXMLExclusionFileName, M4UniRead, eEncoding ) ;
		
		if( pfExclusionFile != NULL )
		{
			poExclusionStreamGenerator = NULL;
			fclose( pfExclusionFile ) ;
		}
		else
		{
			m_poExclusionOutStream = new ofstream;
			M4MDT_RETURN_ERROR1( m_poExclusionOutStream == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof( ofstream ), "%d" ) ;
			
			// UNICODE FILE
			eUniFileEncodingType_t eEncoding = M4UniANSI ;
			M4Open( *m_poExclusionOutStream, ai_pccXMLExclusionFileName, M4UniWriteBinary, eEncoding ) ;

			M4MDT_RETURN_ERROR2(m_poExclusionOutStream->is_open() == 0 , M4_ERROR, M4MDTERROR_NO_OPEN_FILE, ai_pccXMLExclusionFileName, strerror(errno), "%s#%s");
			
			// UNICODE XML
			poExclusionStreamGenerator = new M4Utf8XMLStreamGenerator(*m_poExclusionOutStream);
		}
	}
	
	m_poOutput = new ClDoubleXMLStreamGenerator( poStreamGenerator, poExclusionStreamGenerator ) ;

	// gets the handle reference
	m_sObjId = ai_pccObjId;
	m_iPKs = ai_iPKs;
	m_iChilds = ao_iChilds;
	m_sConnectionId = ai_pccConnectionId;

	return M4_SUCCESS;
  //## end ClMDTDataSAXTransformer::Reset%1081852905.body
}

m4return_t ClMDTDataSAXTransformer::Release ()
{
  //## begin ClMDTDataSAXTransformer::Release%1084529907.body preserve=yes
	m_bInit = M4_FALSE;
	if (m_poOutput != NULL) {
		delete m_poOutput;
		m_poOutput = NULL;
	}

	if (m_poOutStream != NULL) {
		m_poOutStream->flush();
		delete m_poOutStream;
		m_poOutStream = NULL;
	}

	if (m_poExclusionOutStream != NULL) {
		m_poExclusionOutStream->flush();
		delete m_poExclusionOutStream;
		m_poExclusionOutStream = NULL;
	}

	return M4_SUCCESS;
  //## end ClMDTDataSAXTransformer::Release%1084529907.body
}

m4return_t ClMDTDataSAXTransformer::WriteInitMessage ()
{
  //## begin ClMDTDataSAXTransformer::WriteInitMessage%1081852906.body preserve=yes

	if (m_iLevel == 0) {
		// UNICODE XML
		m_poOutput->MakeXMLDeclaration(M4MDT_DATAXML_ALL);
	}

	return M4_SUCCESS;

  //## end ClMDTDataSAXTransformer::WriteInitMessage%1081852906.body
}

m4return_t ClMDTDataSAXTransformer::InitParameters ()
{
  //## begin ClMDTDataSAXTransformer::InitParameters%1081852921.body preserve=yes

	// scans input parameters
	m4uint32_t iSize = 0;
	m4int32_t iNParam = m_poService->GetIReadParameterManager()->GetParameterSize(m_iPKs,iSize);
	m4pcchar_t pccName = NULL, pccValue = NULL;

	for (m4uint32_t i=0; i<iSize; i++)
	{
		m_poService->GetIReadParameterManager()->GetParameterValue(m_iPKs,i,pccName,pccValue);

		// checks null values
		M4MDT_RETURN_ERROR0(pccName == NULL  , M4_ERROR, M4MDTERROR_TR_PARAM_NAME_NULL);
		M4MDT_RETURN_ERROR1(pccValue == NULL , M4_ERROR, M4MDTERROR_TR_PARAM_VALUE_NULL,pccName,"%s");
		
		m_oCManager.GetCurrent().AddParameter(pccName,pccValue);
	}

	return M4_SUCCESS;
  //## end ClMDTDataSAXTransformer::InitParameters%1081852921.body
}

m4void_t ClMDTDataSAXTransformer::procObject (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procObject%1081852910.body preserve=yes

	// gets the object name
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error


	// new object
	m_oCManager.AddContext(sName);

	// the first object is an special case were the arguments came from api
	if (m_bInit == M4_FALSE) {
		InitParameters();
	}
	
	// writes the data header
	if (m_iLevel == 0) {
		m_poOutput->MakeElement("object-data", M4MDT_DATAXML_ALL);
		m_poOutput->AddElementAttribute("name",m_sObjId.c_str(), M4MDT_DATAXML_ALL);
		m_poOutput->EndElement(M4MDT_DATAXML_ALL);
	}

  //## end ClMDTDataSAXTransformer::procObject%1081852910.body
}

m4void_t ClMDTDataSAXTransformer::procEndObject (const XMLCh * const name)
{
  //## begin ClMDTDataSAXTransformer::procEndObject%1081852909.body preserve=yes

	// writes the data footer
	if (m_iLevel == 0) {
		m_poOutput->CloseElement("object-data", M4MDT_DATAXML_ALL);
	}

	// release the context
	m4return_t iRes = m_oCManager.LeaveContext();

	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}

  //## end ClMDTDataSAXTransformer::procEndObject%1081852909.body
}

m4void_t ClMDTDataSAXTransformer::procCallTable (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procCallTable%1081852923.body preserve=yes

	// gets the table name
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error


	// adds a object
	iRes = m_oCManager.AddContext(sName);
	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}

	// adds the DB setting
	string sDBName ;
	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChDB, attributes, sDBName ) ;
	// error


	if (sDBName.size()) {
		m_oCManager.GetCurrent().SetDBName(sDBName);
	}

  //## end ClMDTDataSAXTransformer::procCallTable%1081852923.body
}

m4void_t ClMDTDataSAXTransformer::procEndCallTable (const XMLCh * const name)
{
  //## begin ClMDTDataSAXTransformer::procEndCallTable%1081852924.body preserve=yes

	// dumps the current table
	m4return_t iRes = m_oCManager.LeaveContext();

	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTDataSAXTransformer::procEndCallTable%1081852924.body
}

m4void_t ClMDTDataSAXTransformer::procWithPK (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procWithPK%1081852928.body preserve=yes

	// gets the PK values
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	string sSelect ;
	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChSelect, attributes, sSelect ) ;
	// error

	// checks references
	if (GetValue(sName,sSelect) != M4_SUCCESS) {
		return;
	}

	m_oCManager.GetCurrent().AddPK(sName,sSelect);

	string sOperation ;
	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChOperation, attributes, sOperation ) ;
	// error

	/* Bug 0121653
	Si existe el operando se añade.
	De momento sólo el distinct
	*/
	if( iRes != M4_ERROR )
	{
		if( strcmpi( sOperation.c_str(), "distinct" ) == 0 )
		{
			m_oCManager.GetCurrent().AddPKOperation( sName, "<>" );
		}
	}

  //## end ClMDTDataSAXTransformer::procWithPK%1081852928.body
}

m4void_t ClMDTDataSAXTransformer::procField (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procField%1083066177.body preserve=yes
	string sName ;
	string sVisible ;
	string sMultiline ;
	string sContentType ;
	string sIsPrintable ;
	string sValue ;


	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChVisible, attributes, sVisible ) ;

	if( iRes == M4_SUCCESS )
	{
		m_oCManager.GetCurrent().AddFieldProperty(sName,"visible",sVisible);
	}

	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChMultiline, attributes, sMultiline ) ;

	if( iRes == M4_SUCCESS )
	{
		m_oCManager.GetCurrent().AddFieldProperty(sName,"multiline",sMultiline);
	}

	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChContentType, attributes, sContentType ) ;

	if( iRes == M4_SUCCESS )
	{
		m_oCManager.GetCurrent().AddFieldProperty(sName,"content-type",sContentType);
	}

	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChIsPrintable, attributes, sIsPrintable ) ;

	if( iRes == M4_SUCCESS )
	{
		m_oCManager.GetCurrent().AddFieldProperty(sName,"is-printable",sIsPrintable);
	}

	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChValue, attributes, sValue ) ;

	if( iRes == M4_SUCCESS )
	{
		if (GetValue(sName,sValue) != M4_SUCCESS) {
			return;
		}
		m_oCManager.GetCurrent().AddFieldProperty(sName,"value",sValue);
	}
	
	// check calculated fields
	if (m_oCManager.GetCurrent().GetCalculatedFields()) {
		m_oCManager.GetCurrent().AddCalculatedField(sName);
	}
  //## end ClMDTDataSAXTransformer::procField%1083066177.body
}

m4void_t ClMDTDataSAXTransformer::procReadTable (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procReadTable%1083150386.body preserve=yes
	procCallTable(name,attributes);
	m_oCManager.GetCurrent().SetIsReadTable(M4_TRUE);
  //## end ClMDTDataSAXTransformer::procReadTable%1083150386.body
}

m4void_t ClMDTDataSAXTransformer::procEndReadTable (const XMLCh * const name)
{
  //## begin ClMDTDataSAXTransformer::procEndReadTable%1083150387.body preserve=yes
	procEndCallTable(name);
  //## end ClMDTDataSAXTransformer::procEndReadTable%1083150387.body
}

m4void_t ClMDTDataSAXTransformer::procIncludeObject (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procIncludeObject%1083166611.body preserve=yes
	// gets the table name
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	// adds a object
	iRes = m_oCManager.AddContext(sName);
	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTDataSAXTransformer::procIncludeObject%1083166611.body
}

m4void_t ClMDTDataSAXTransformer::procEndIncludeObject (const XMLCh * const name)
{
  //## begin ClMDTDataSAXTransformer::procEndIncludeObject%1083166612.body preserve=yes

	// gets the current object definition
	InputSource *poObjSource = NULL;
	string sName = m_oCManager.GetCurrent().GetType();
	m4return_t iRes = m_poService->GetIDictionary()->GetXMLObject(sName.c_str(),poObjSource);

	M4MDT_VOID_ERROR1(iRes != M4_SUCCESS , M4MDTERROR_TR_OBJ_GENERATION, sName.c_str(), "%s");

	// invoques a specific parser
	// creates the SAX parser without validation (improve speed)
	SAXParser *poParser = new SAXParser;
	poParser->setValidationScheme(SAXParser::Val_Never);

	// invoques the parser
    poParser->setDocumentHandler(this);
    poParser->setErrorHandler(this);

	// parses the xml input
	++m_iLevel;
    try
    {
        poParser->parse(*poObjSource);
    }

	catch( const XMLException& e )
	{
		// UNICODE XML
		m4pchar_t pcException = M4XMLToCpp( (M4XMLCh*)e.getMessage() ) ;
		M4MDT_DUMP_ERROR1( M4MDTERROR_EXCEPTION, pcException, "%s" ) ;
		delete [] pcException ;

        iRes = M4_ERROR;
    }
	--m_iLevel;

	// deletes the parser
	delete poParser;

	iRes = m_poService->GetIDictionary()->ReleaseXMLObject( poObjSource ) ;

	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR2( M4MDTERROR_INTERNAL_ERROR, "ClMDTDataSAXTransformer::procEndIncludeObject", "ReleaseXMLObject", "%s#%s");
		SetError(M4_ERROR);
	}

	// release the object
	iRes = m_oCManager.LeaveContext();

	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}

  //## end ClMDTDataSAXTransformer::procEndIncludeObject%1083166612.body
}

m4void_t ClMDTDataSAXTransformer::procWithParam (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procWithParam%1083166613.body preserve=yes

	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	string sSelect ;
	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChSelect, attributes, sSelect ) ;
	// error

	// checks references
	if (GetValue(sName,sSelect) != M4_SUCCESS) {
		return;
	}

	m_oCManager.GetCurrent().AddParameter(sName,sSelect);
  //## end ClMDTDataSAXTransformer::procWithParam%1083166613.body
}

m4void_t ClMDTDataSAXTransformer::procCallObject (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procCallObject%1083244863.body preserve=yes
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	// adds a object
	iRes = m_oCManager.AddContext(sName);
	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTDataSAXTransformer::procCallObject%1083244863.body
}

m4void_t ClMDTDataSAXTransformer::procEndCallObject (const XMLCh * const name)
{
  //## begin ClMDTDataSAXTransformer::procEndCallObject%1083244864.body preserve=yes
	m4return_t iRes = m_oCManager.GetCurrent().DumpChilds(m_iChilds);
	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}

	// release the context
	iRes = m_oCManager.LeaveContext();

	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTDataSAXTransformer::procEndCallObject%1083244864.body
}

m4void_t ClMDTDataSAXTransformer::procCallTemplate (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procCallTemplate%1084288691.body preserve=yes
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	iRes = m_oCManager.GetCurrent().AddTemplate(sName, m_sOutputName);
	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTDataSAXTransformer::procCallTemplate%1084288691.body
}

m4void_t ClMDTDataSAXTransformer::procWithArg (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procWithArg%1084288692.body preserve=yes
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	string sInsert ;
	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChInsert, attributes, sInsert ) ;
	// error

	string sSelect ;
	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChSelect, attributes, sSelect ) ;
	// error


	iRes = m_oCManager.GetCurrent().AddTemplateArg(sName,sInsert.size()?sInsert:sSelect);
	if (iRes != M4_SUCCESS) {
		M4MDT_DUMP_ERROR1(M4MDTERROR_TR_GENERATE_TABLE, m_oCManager.GetCurrent().GetType().c_str(),"%s");
		SetError(M4_ERROR);
	}
  //## end ClMDTDataSAXTransformer::procWithArg%1084288692.body
}

m4void_t ClMDTDataSAXTransformer::procGenerateFunctions (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procGenerateFunctions%1084288693.body preserve=yes
	m_oCManager.GetCurrent().SetGenerateFunction(M4_TRUE);
  //## end ClMDTDataSAXTransformer::procGenerateFunctions%1084288693.body
}

m4void_t ClMDTDataSAXTransformer::procUpdateFunctions (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procUpdateFunctions%1084288694.body preserve=yes
	m_oCManager.GetCurrent().SetGenerateFunction(M4_FALSE);
  //## end ClMDTDataSAXTransformer::procUpdateFunctions%1084288694.body
}

m4void_t ClMDTDataSAXTransformer::procCalculatedFields (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procCalculatedFields%1084347795.body preserve=yes
	m_oCManager.GetCurrent().SetCalculatedFields(M4_TRUE);
  //## end ClMDTDataSAXTransformer::procCalculatedFields%1084347795.body
}

m4void_t ClMDTDataSAXTransformer::procEndCalculatedFields (const XMLCh* const name)
{
  //## begin ClMDTDataSAXTransformer::procEndCalculatedFields%1084347796.body preserve=yes
	m_oCManager.GetCurrent().SetCalculatedFields(M4_FALSE);
  //## end ClMDTDataSAXTransformer::procEndCalculatedFields%1084347796.body
}

m4void_t ClMDTDataSAXTransformer::procOptions (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procOptions%1086775446.body preserve=yes
	string sName ;
	m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, attributes, sName ) ;
	// error

	string sValue ;
	iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChValue, attributes, sValue ) ;
	// error

	m_oCManager.GetCurrent().AddOption(sName,sValue);
  //## end ClMDTDataSAXTransformer::procOptions%1086775446.body
}

m4void_t ClMDTDataSAXTransformer::procParam (const XMLCh* const name, AttributeList& attributes)
{
  //## begin ClMDTDataSAXTransformer::procParam%1086778455.body preserve=yes
		
	// dump initial parameters
	if (m_bInit == M4_FALSE) {

		m4return_t iRes = m_oCManager.GetCurrent().DumpParameters();
		if (iRes != M4_SUCCESS) {
			M4MDT_DUMP_ERROR1( M4MDTERROR_TR_WRITE_TAG, "parameter.start", "%s" ) ;
			SetError(M4_ERROR);
			return;
		}

		// resets init flag
		m_bInit = M4_TRUE;
	}
  //## end ClMDTDataSAXTransformer::procParam%1086778455.body
}

// Additional Declarations
  //## begin ClMDTDataSAXTransformer%40740F5B0334.declarations preserve=yes
  //## end ClMDTDataSAXTransformer%40740F5B0334.declarations

//## begin module%407ABB81030B.epilog preserve=yes
//## end module%407ABB81030B.epilog
