//## begin module%407D0C8F037A.cm preserve=no
//## end module%407D0C8F037A.cm

//## begin module%407D0C8F037A.cp preserve=no
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
//## end module%407D0C8F037A.cp

//## Module: m4transctx%407D0C8F037A; Package body
//## Subsystem: M4MDTransfer::M4Transformer::src%407ABB4D003C
//## Source file: D:\Work\v600\m4mdtransfer\src\m4transctx.cpp

//## begin module%407D0C8F037A.additionalIncludes preserve=no
//## end module%407D0C8F037A.additionalIncludes

//## begin module%407D0C8F037A.includes preserve=yes
//## end module%407D0C8F037A.includes

// m4mdtodbc
#include <m4mdtodbc.hpp>
// m4transctx
#include <m4transctx.hpp>
// xmlstreamgenerator
#include <xmlstreamgenerator.hpp>
#include "m4unicode.hpp"


//## begin module%407D0C8F037A.declarations preserve=no
//## end module%407D0C8F037A.declarations

//## begin module%407D0C8F037A.additionalDeclarations preserve=yes
#include <m4mdtdef.hpp>
#include <m4mdtres.hpp>
#include <m4mdtutils.hpp>
#include <m4imdtservice.hpp>
#include <m4mdtdictionary.hpp>
#include <m4imdtparameter.hpp>
#include <m4imdtprestool.hpp>
#include <defmain.hpp>
#include <stdio.h>

#include <xercesc/dom/DOMElement.hpp>


// global functions
//=================================================================================
//
// Compara 2 registros segun una lista de columnas
//
// @param ai_pcvRow1
//
//		Puntero al primer registro
//
// @param ai_pcvRow1
//
//		Puntero al segundo registro
//
// @return
//
//		< 0 si el primer registro va antes que el segundo
//		> 0 si el primer registro va después que el segundo
//		= 0 si los dos registros son iguales
//
// Pasos
//
// Se validan los parámetros
// Se inicia un bucle sobre los elementos del vector de ordenación
// Se el vector retorna -1 se ha acabado y devuelve un 0 (equivalente)
// Se hay una diferencia se retorna este valor.
//=================================================================================

static int	M4MDTGenericCompareFields( m4pcvoid_t ai_pcvRow1, m4pcvoid_t ai_pcvRow2 )
{

	m4return_t	iResult = M4_ERROR ;
	int			iDifference = 0 ;
	int			iValue1 = 0 ;
	int			iValue2 = 0 ;
	ClMDTValue	vValue1 ;
	ClMDTValue	vValue2 ;
	M4Row		*poRow1 = NULL ;
	M4Row		*poRow2 = NULL ;

	poRow1 = (*( M4SorteableRow** ) ai_pcvRow1)->m_poRow ;
	poRow2 = (*( M4SorteableRow** ) ai_pcvRow2)->m_poRow ;
	const m4int32_t *viOrder = (*( M4SorteableRow** ) ai_pcvRow1)->m_viSortOrder;

	M4MDT_RETURN_ERROR2( viOrder == NULL, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTGenericCompareFields", "m_viSortOrder", "%s#%s = <NULL>" ) ;

	for (int i=0; viOrder[i] >=0; ++i) {
		
		iResult = poRow1->GetValue( viOrder[i], vValue1 ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTGenericCompareFields", "GetValue", "ID_REAL_OBJECT", "%s#%s - Index = <%d>" ) ;
		
		iResult = poRow2->GetValue( viOrder[i], vValue2 ) ;
		M4MDT_RETURN_ERROR3( iResult != M4_SUCCESS, 0, M4MDTERROR_INTERNAL_ERROR, "M4MDTGenericCompareFields", "GetValue", "ID_REAL_OBJECT", "%s#%s - Index = <%d>" ) ;
		
		iDifference = strcmpi( vValue1.c_str(), vValue2.c_str() ) ;

		if (iDifference) {
			return iDifference;
		}
		
	}

	return 0 ;
}


//=================================================================================
//
// Genera el checksum de un fichero
//
// @param ai_pccFile
//
//		Path completo al fichero del que se quiere calcular el checksum
//
// @return
//
//		El número que representa el checksum o 0 si ha habido algún error
//
// Pasos
//
// Se inicializan las variables del checksum
// Se validan los parámetros
// Se abre el fichero
// Se recorre cada caracter del fichero calculando el checksum
//=================================================================================

static m4uint32_t	M4MDTCalculateCheckSum( m4pcchar_t ai_pccFile )
{

	m4uint32_t	iResult = 0 ;
	m4uint8_t	c = 0 ;
	m4uint8_t	iCipher = 0 ;
	m4uint16_t	iRadix = 23645 ;
	m4uint16_t	iConstant1 = 38287 ;
	m4uint16_t	iConstant2 = 28876 ;
	FILE		*pfFile = NULL ;


	if( ai_pccFile == NULL || *ai_pccFile == '\0' )
	{
		return( 0 ) ;
	}

	// UNICODE FILE
	eUniFileEncodingType_t eEncoding = M4UniANSI ;
	pfFile = M4Fopen( ai_pccFile, M4UniReadBinary, eEncoding ) ;

	if( pfFile == NULL )
	{
		return( 0 ) ;
	}

	iResult = 0 ;
	c = fgetc( pfFile ) ;

	while( feof( pfFile ) == 0 )
	{
		// Se calcula el checksum con un algoritmillo que hemos pillado de internet
		iCipher = c ^ ( iRadix >> 8 ) ;
		iResult += iCipher ;
		iRadix = iConstant1 * ( iCipher + iRadix ) + iConstant2 ;

		c = fgetc( pfFile ) ;
	}

	fclose( pfFile ) ;

	return( iResult ) ;
}


//## end module%407D0C8F037A.additionalDeclarations


// Class ClMDTTransformerContext 








//=================================================================================
//
// ClDoubleXMLStreamGenerator
//
//=================================================================================


//=================================================================================
// Funciones de inicialización
//=================================================================================


ClDoubleXMLStreamGenerator::ClDoubleXMLStreamGenerator( M4XMLStreamGenerator *ai_poFirstStream, M4XMLStreamGenerator *ai_poSecondStream )
{
	m_poFirstStream = ai_poFirstStream ;
	m_poSecondStream = ai_poSecondStream ;
}


ClDoubleXMLStreamGenerator::~ClDoubleXMLStreamGenerator( void )
{
	if( m_poFirstStream != NULL )
	{
		delete( m_poFirstStream ) ;
		m_poFirstStream = NULL ;
	}
	if( m_poSecondStream != NULL )
	{
		delete( m_poSecondStream ) ;
		m_poSecondStream = NULL ;
	}
}


//=================================================================================
// Funciones de mapeo
//=================================================================================

m4return_t ClDoubleXMLStreamGenerator::MakeXMLDeclaration( m4uint8_t ai_iMode )
{

	m4return_t	iResult = NULL ;


	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		iResult = m_poFirstStream->MakeXMLDeclaration() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		iResult = m_poSecondStream->MakeXMLDeclaration() ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClDoubleXMLStreamGenerator::MakeElement( m4pcchar_t ai_pccElement, m4uint8_t ai_iMode )
{

	m4return_t	iResult = NULL ;


	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		iResult = m_poFirstStream->MakeElement( ai_pccElement ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		iResult = m_poSecondStream->MakeElement( ai_pccElement ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClDoubleXMLStreamGenerator::AddElementAttribute( m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue, m4uint8_t ai_iMode )
{

	m4return_t	iResult = NULL ;


	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		iResult = m_poFirstStream->AddElementAttribute( ai_pccAttribute, ai_pccAttValue ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		iResult = m_poSecondStream->AddElementAttribute( ai_pccAttribute, ai_pccAttValue ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClDoubleXMLStreamGenerator::AddElementValue( m4pcchar_t ai_pccElementValue, m4uint8_t ai_iMode )
{

	m4return_t	iResult = NULL ;


	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		iResult = m_poFirstStream->AddElementValue( ai_pccElementValue ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		iResult = m_poSecondStream->AddElementValue( ai_pccElementValue ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClDoubleXMLStreamGenerator::AddElementValue( m4pcchar_t ai_pccElementValue, size_t ai_iLength, m4uint8_t ai_iMode )
{

	m4return_t	iResult = NULL ;


	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		iResult = m_poFirstStream->AddElementValue( ai_pccElementValue, ai_iLength ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		iResult = m_poSecondStream->AddElementValue( ai_pccElementValue, ai_iLength ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClDoubleXMLStreamGenerator::AddEscapedElementAttribute( m4pcchar_t ai_pccAttribute, m4pcchar_t ai_pccAttValue, size_t ai_iLength, m4uint8_t ai_iMode )
{

	m4return_t	iResult = NULL ;


	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		iResult = m_poFirstStream->AddEscapedElementAttribute( ai_pccAttribute, ai_pccAttValue, ai_iLength ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		iResult = m_poSecondStream->AddEscapedElementAttribute( ai_pccAttribute, ai_pccAttValue, ai_iLength ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


m4return_t ClDoubleXMLStreamGenerator::CloseElement( m4pcchar_t ai_pccElement, m4uint8_t ai_iMode )
{

	m4return_t	iResult = NULL ;


	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		iResult = m_poFirstStream->CloseElement( ai_pccElement ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		iResult = m_poSecondStream->CloseElement( ai_pccElement ) ;

		if( iResult != M4_SUCCESS )
		{
			return( M4_ERROR ) ;
		}
	}

	return( M4_SUCCESS ) ;
}


void ClDoubleXMLStreamGenerator::CloseSingleClosedElement( m4uint8_t ai_iMode )
{
	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		m_poFirstStream->CloseSingleClosedElement() ;
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		m_poSecondStream->CloseSingleClosedElement() ;
	}
}


void ClDoubleXMLStreamGenerator::EndElement( m4uint8_t ai_iMode )
{
	if( m_poFirstStream != NULL && ( ai_iMode & M4MDT_DATAXML_FIRST ) != 0 )
	{
		m_poFirstStream->EndElement() ;
	}

	if( m_poSecondStream != NULL && ( ai_iMode & M4MDT_DATAXML_SECOND ) != 0 )
	{
		m_poSecondStream->EndElement() ;
	}
}



















ClMDTTransformerContext::ClMDTTransformerContext (string& ai_sType, ClMDTTransformerContext* ai_poParent, IMDTService* ai_poService, string& ai_sConnectionId, ClDoubleXMLStreamGenerator* &ai_poOutput)
  //## begin ClMDTTransformerContext::ClMDTTransformerContext%1081852930.hasinit preserve=no
      :
	m_sType(ai_sType),
	m_poParent(ai_poParent),
	m_poChild(NULL),
	m_poService(ai_poService),
	m_sConnectionId(ai_sConnectionId),
	m_bIsFlushed(M4_FALSE),
	m_bIsReadTable(M4_FALSE),
	m_bGenerateFunction(M4_FALSE),
	m_bCalculatedFields(M4_FALSE),
	m_bHasData(M4_FALSE),
	m_bHasExcludedData(M4_FALSE),
	m_oLoader(ai_poService,ai_poOutput),
	m_poOutput(ai_poOutput),
	m_oTemplateImpl(m_oLoader,*ai_poService)
  //## end ClMDTTransformerContext::ClMDTTransformerContext%1081852930.hasinit
  //## begin ClMDTTransformerContext::ClMDTTransformerContext%1081852930.initialization preserve=yes
  //## end ClMDTTransformerContext::ClMDTTransformerContext%1081852930.initialization
{
  //## begin ClMDTTransformerContext::ClMDTTransformerContext%1081852930.body preserve=yes
	m_oLoader.SetContext(this);

	// reservers
	m_oTemplateVector.reserve(10);

	// initializes the template functions map
	m_oTemplateImplMap.insert(ClMDTStringTemplateMap::value_type(M4MDT_PRES2OBL_NAME,(ClMDTProcTemplate)&ClMDTProcTemplateImpl::Presentation2OBL));
	m_oTemplateImplMap.insert(ClMDTStringTemplateMap::value_type(M4MDT_BLOB2FILE_NAME,(ClMDTProcTemplate)&ClMDTProcTemplateImpl::BLOB2File));
	m_oTemplateImplMap.insert(ClMDTStringTemplateMap::value_type(M4MDT_BLOB2EXTERNAL_NAME,(ClMDTProcTemplate)&ClMDTProcTemplateImpl::BLOB2External));
	m_oTemplateImplMap.insert(ClMDTStringTemplateMap::value_type(M4MDT_PRESENTATIONPACK2EXTERNAL_NAME,(ClMDTProcTemplate)&ClMDTProcTemplateImpl::PresentationPackage2External));
	m_oTemplateImplMap.insert(ClMDTStringTemplateMap::value_type(M4MDT_FIELD2FILE_NAME,(ClMDTProcTemplate)&ClMDTProcTemplateImpl::Field2File));

  //## end ClMDTTransformerContext::ClMDTTransformerContext%1081852930.body
}


ClMDTTransformerContext::~ClMDTTransformerContext ()
{
  //## begin ClMDTTransformerContext::~ClMDTTransformerContext%1081852916.body preserve=yes
	if (m_poChild != NULL) {
		delete m_poChild;
	}

	// deletes template functions
	m4int32_t iNTemplates = m_oTemplateVector.size();
	for (m4int32_t i=0; i<iNTemplates; i++) {
		delete (ClMDTTemplateFunction*) m_oTemplateVector[i];
	}
  //## end ClMDTTransformerContext::~ClMDTTransformerContext%1081852916.body
}



//## Other Operations (implementation)
m4void_t ClMDTTransformerContext::AddParameter (string ai_sName, string ai_sValue)
{
  //## begin ClMDTTransformerContext::AddParameter%1081852922.body preserve=yes
	m_oParamMap.insert(ClMDTStringMap::value_type(ai_sName,ai_sValue));
  //## end ClMDTTransformerContext::AddParameter%1081852922.body
}

m4void_t ClMDTTransformerContext::AddOption (string ai_sName, string ai_sValue)
{
  //## begin ClMDTTransformerContext::AddOption%1086775445.body preserve=yes
	m_oOptionsMap.insert(ClMDTStringMap::value_type(ai_sName,ai_sValue));
  //## end ClMDTTransformerContext::AddOption%1086775445.body
}

m4return_t ClMDTTransformerContext::FlushTable ()
{
  //## begin ClMDTTransformerContext::FlushTable%1081852925.body preserve=yes

	if (m_oPKMap.empty() || m_bIsFlushed) {
		return M4_SUCCESS;
	}
	
	// set flag
	m_bIsFlushed = M4_TRUE;

	m4return_t iRes = M4_ERROR;

	iRes = GenerateTableFields();
	M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_TABLE_LOAD, m_sType.c_str(), "%s");

	iRes = m_oLoader.Load();
	M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_TABLE_LOAD, m_sType.c_str(), "%s");

	iRes = ProcGenerateFunctions();
	M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_TABLE_LOAD, m_sType.c_str(), "%s");

	if (GetIsReadTable() == M4_FALSE) {
		iRes = m_oLoader.Sort();
		M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_TABLE_DUMP, m_sType.c_str(), "%s");

		iRes = m_oLoader.Dump();
		M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_TABLE_DUMP, m_sType.c_str(), "%s");
	}

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::FlushTable%1081852925.body
}

m4return_t ClMDTTransformerContext::ProcValue (string& ai_sValue, m4int32_t ai_iParentReg, ClMDTValue& ao_sRes)
{
  //## begin ClMDTTransformerContext::ProcValue%1081852929.body preserve=yes

	ao_sRes = "";
	ao_sRes.SetNull();
	
	// checks for a standard value
	if (ai_sValue.empty()) {
		return M4_SUCCESS;
	}

	m4pcchar_t pcValue = ai_sValue.c_str();
	switch (*pcValue) {
	case '$':
		{
			ClMDTTransformerContext *poParent = GetParent();
			M4MDT_RETURN_ERROR2( poParent == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformerContext::ProcValue", ai_sValue.c_str(), "%s#Argument value = <%d>" ) ;

			// gets the argument from my parent node
			return poParent->GetParameter(pcValue+1,ao_sRes);
			break;
		}
	case '@':
		{
			ClMDTTransformerContext *poParent = GetParent();
			M4MDT_RETURN_ERROR2( poParent == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformerContext::ProcValue", ai_sValue.c_str(), "%s#Argument value = <%d>" ) ;
			
			return poParent->GetField(pcValue+1,ai_iParentReg,ao_sRes);
			break;
		}
	default:
			ao_sRes = ai_sValue;
			return M4_SUCCESS;
	}
  //## end ClMDTTransformerContext::ProcValue%1081852929.body
}

m4return_t ClMDTTransformerContext::GetParameter (m4pcchar_t ai_pccParam, ClMDTValue& ao_sRes)
{
  //## begin ClMDTTransformerContext::GetParameter%1082385407.body preserve=yes

	ao_sRes = "";
	ao_sRes.SetNull();
	
	// scans the parameter vector
	ClMDTStringMap::iterator it;
	it = m_oParamMap.find(ai_pccParam);

	if (it == m_oParamMap.end()) {
		// checks in the father node
		ClMDTTransformerContext *poParent = GetParent();
		M4MDT_RETURN_ERROR1(poParent == NULL , M4_ERROR, M4MDTERROR_TR_INVALID_PARAM, ai_pccParam, "%s");

		// calls the father
		return poParent->GetParameter(ai_pccParam,ao_sRes);
	}

	// returns the parameter
	ao_sRes.Set((*it).second);
	return M4_SUCCESS;
  //## end ClMDTTransformerContext::GetParameter%1082385407.body
}

m4return_t ClMDTTransformerContext::GetOption (m4pcchar_t ai_pccParam, string& ao_sRes)
{
  //## begin ClMDTTransformerContext::GetOption%1086775447.body preserve=yes

	ao_sRes = "";
	
	// scans the option map
	ClMDTStringMap::iterator oIt = m_oOptionsMap.find(ai_pccParam);
	if (oIt == m_oOptionsMap.end()) {
		return M4_ERROR;
	}

	ao_sRes = (*oIt).second;

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::GetOption%1086775447.body
}

m4return_t ClMDTTransformerContext::GetField (string ai_sName, m4int32_t ai_iReg, ClMDTValue& ao_vRes)
{
  //## begin ClMDTTransformerContext::GetField%1082967064.body preserve=yes
	
	// is it in my scope?
	if (m_oLoader.GetField(ai_sName,ai_iReg,ao_vRes) == M4_SUCCESS) {
		return M4_SUCCESS;
	}

	// may in parent node. Gets the current register
	ClMDTTransformerContext *poParent = GetParent();
	M4MDT_RETURN_ERROR2(poParent == NULL , M4_ERROR, M4MDTERROR_TR_INVALID_PARENT, ai_sName.c_str(), ai_iReg, "%s#%d");

	// gets the parent register
	m4int32_t iParentRecord = m_oLoader.GetParentRecord(ai_iReg);
	M4MDT_RETURN_ERROR2(iParentRecord , M4_ERROR, M4MDTERROR_TR_INVALID_PARENT, ai_sName.c_str(), ai_iReg, "%s#%d");
	
	return poParent->GetField(ai_sName,iParentRecord,ao_vRes);
  //## end ClMDTTransformerContext::GetField%1082967064.body
}

m4return_t ClMDTTransformerContext::GenerateTableFields ()
{
  //## begin ClMDTTransformerContext::GenerateTableFields%1082385409.body preserve=yes
	m4return_t iRes = M4_ERROR;
	xercesc::DOMDocument *poDoc = NULL;

	iRes = m_poService->GetIDictionary()->GetDOMTable(m_sType.c_str(), poDoc);
	M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS || poDoc == NULL, M4_ERROR, M4MDTERROR_TR_TABLE_DEF, m_sType.c_str(), "%s");

	DOMNode *poScan = poDoc->getDocumentElement()->getFirstChild();

	while (XMLString::compareString(poScan->getNodeName(), IMDTService::sm_sXMLChLogicObject) != 0) {
		poScan = poScan->getNextSibling();
		if (poScan == NULL) {
			break;
		}
	}

	m_oLoader.SetLogicObject( poScan ) ;


	poScan = poDoc->getDocumentElement()->getFirstChild();

	while (XMLString::compareString(poScan->getNodeName(), IMDTService::sm_sXMLChLogicFields) != 0) {
		poScan = poScan->getNextSibling();
		if (poScan == NULL) {
			break;
		}
	}

	DOMNode *poLogicalField = poScan->getFirstChild();

	// iterates scanning for pk's
	for (; poLogicalField != NULL; poLogicalField = poLogicalField->getNextSibling()) {

		// checks the field name
		if (XMLString::compareString(poLogicalField->getNodeName(), IMDTService::sm_sXMLChLogicField) != 0) {
			continue;
		}

		// check for PK
		DOMNamedNodeMap *poMap = poLogicalField->getAttributes();

		if (poMap == NULL) {
			continue;
		}

		// saves the field
		m_oLoader.AddField(poLogicalField);
	}

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::GenerateTableFields%1082385409.body
}

m4bool_t ClMDTTransformerContext::CheckFieldProp (string& ai_sFieldName, string& ai_sProp, string& aio_sValue)
{
  //## begin ClMDTTransformerContext::CheckFieldProp%1083066179.body preserve=yes
	string sId = ai_sFieldName;
	sId.append(":");
	sId.append(ai_sProp);

	ClMDTStringMap::iterator oIt = m_oFieldPropMap.find(sId);
	if (oIt == m_oFieldPropMap.end()) {
		return M4_FALSE;
	}

	// checks the value
	if (aio_sValue.size()) {
		return (*oIt).second.compare(aio_sValue) == 0;
	}
	else {
		aio_sValue = (*oIt).second;
		return M4_TRUE;
	}
  //## end ClMDTTransformerContext::CheckFieldProp%1083066179.body
}

m4return_t ClMDTTransformerContext::DumpParameters ()
{
  //## begin ClMDTTransformerContext::DumpParameters%1083145763.body preserve=yes

	// checks the dump options
	string oOption;
	if (GetOption("dump-parameters",oOption) == M4_SUCCESS) {
		if (oOption == "false") {
			return M4_SUCCESS;
		}
	}

	ClMDTStringMap::iterator oIt = m_oParamMap.begin();
	m4return_t iRes = M4_ERROR;
	while (oIt != m_oParamMap.end()) {

		iRes = m_poOutput->MakeElement("param-value", M4MDT_DATAXML_FIRST);
		M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "parameter.start", "%s" ) ;

		iRes = m_poOutput->AddElementAttribute("name",(*oIt).first.c_str(), M4MDT_DATAXML_FIRST);
		M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "parameter:name", "%s" ) ;

		m_poOutput->AddEscapedElementAttribute("value",(*oIt).second.c_str(), (*oIt).second.size(), M4MDT_DATAXML_FIRST);
		M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "parameter:value", "%s" ) ;

		// void
		m_poOutput->CloseSingleClosedElement(M4MDT_DATAXML_FIRST);

		++oIt;
	}

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::DumpParameters%1083145763.body
}

m4return_t ClMDTTransformerContext::DumpChilds (m4int32_t ai_iChilds)
{
  //## begin ClMDTTransformerContext::DumpChilds%1083244865.body preserve=yes
	
	// gets the number of parent records
	m4int32_t iNRegs = 1;
	if (IsRoot() == M4_FALSE) {
		iNRegs = GetParent()->GetNRecords();

		// sorts my father registers before exporting
		m4return_t iRes = GetParent()->m_oLoader.Sort();
		M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_TABLE_DUMP, m_sType.c_str(), "%s");
	}

	// sorts the registers
	for (m4int32_t iReg=0; iReg<iNRegs; iReg++) {

		// creates a parameter
		m4int32_t iHandler = 0;
		m4return_t iRes = m_poService->GetIWriteParameterManager()->CreateParameter(iHandler);
		M4MDT_RETURN_ERROR0( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_CREATE_PARAMETER ) ;

		// adds the parameter
		iRes = m_poService->GetIWriteParameterManager()->AddParameterValue(ai_iChilds,m_sType.c_str(),iHandler);
		M4MDT_RETURN_ERROR0( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_CREATE_PARAMETER ) ;

		// adds the parameter values for each register
		ClMDTStringMap::iterator oIt = m_oParamMap.begin();
		m4bool_t bIsNull = M4_TRUE;
		
		while (oIt != m_oParamMap.end()) {
			string sName = (*oIt).first;
			ClMDTValue sValue;

			iRes = ProcValue((*oIt).second,iReg,sValue);
			M4MDT_RETURN_ERROR0( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_CREATE_PARAMETER ) ;

			if (! sValue.IsNull()) {
				bIsNull = M4_FALSE;
			}
			else {
				// aborts
				bIsNull = M4_TRUE;
				break;
			}
	
			iRes = m_poService->GetIWriteParameterManager()->AddParameterValue(iHandler,sName.c_str(),sValue.c_str());
			M4MDT_RETURN_ERROR0( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_CREATE_PARAMETER ) ;

			++oIt;
		}

		// checks for null values
		if (bIsNull) {

			// removes the reference
			iRes = m_poService->GetIWriteParameterManager()->RemoveParameterValue(ai_iChilds,m_sType.c_str(),iHandler);
			M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_RELEASE_PARAMETER, m_sType.c_str(), iHandler, "%s#%d") ;

			// deletes the child
			iRes = m_poService->GetIWriteParameterManager()->ReleaseParameter(iHandler);
			M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_RELEASE_PARAMETER, m_sType.c_str(), iHandler, "%s#%d") ;
		}
	}

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::DumpChilds%1083244865.body
}

m4return_t ClMDTTransformerContext::AddTemplate (string& ai_sName, const string& ai_sOutputName)
{
  //## begin ClMDTTransformerContext::AddTemplate%1084288696.body preserve=yes
	// creates a new template container
	ClMDTTemplateFunction *poTemplate = new ClMDTTemplateFunction;
	M4MDT_RETURN_ERROR1(poTemplate == NULL, M4_ERROR, M4MDTERROR_NO_MEMORY, sizeof(ClMDTTemplateFunction), "%d");
	
	poTemplate->m_sName = ai_sName;
	poTemplate->m_sOutputName = ai_sOutputName;
	poTemplate->m_bIsGenerate = m_bGenerateFunction;

	// adds to the current vector
	m_oTemplateVector.push_back(poTemplate);

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::AddTemplate%1084288696.body
}

m4return_t ClMDTTransformerContext::AddTemplateArg (string& ai_sName, string& ai_sValue)
{
  //## begin ClMDTTransformerContext::AddTemplateArg%1084288697.body preserve=yes

	// gets the last element
	m4int32_t iNTemplates = m_oTemplateVector.size();
	M4MDT_RETURN_ERROR0(iNTemplates == 0, M4_ERROR, M4MDTERROR_TR_CREATE_TEMPLATE_ARG);

	m_oTemplateVector[iNTemplates-1]->AddArg(ai_sName,ai_sValue);
	return M4_SUCCESS;
	
  //## end ClMDTTransformerContext::AddTemplateArg%1084288697.body
}

m4void_t ClMDTTransformerContext::AddCalculatedField (string& ai_sName)
{
  //## begin ClMDTTransformerContext::AddCalculatedField%1084347794.body preserve=yes
	m_oCalculatedFieldsSet.insert(ai_sName);
  //## end ClMDTTransformerContext::AddCalculatedField%1084347794.body
}

string ClMDTTransformerContext::GenerateUpdateFunction ()
{
  //## begin ClMDTTransformerContext::GenerateUpdateFunction%1084347797.body preserve=yes
	// checks for update function
	string oRes;
	ClMDTTemplateVector::iterator oIt = m_oTemplateVector.begin();
	while (oIt != m_oTemplateVector.end()) {

		// skips the generate mode
		if ((*oIt)->m_bIsGenerate == M4_TRUE) {
			++oIt;
			continue;
		}

		// adds the separator
		if (oRes.size()) {
			oRes.append(";");
		}

		// generates the string
		ClMDTTemplateFunction *poTemplate = *oIt;
		oRes.append(poTemplate->m_sName);
		oRes.append("(");

		// loop for args
		ClMDTStringMap::iterator oItArg = poTemplate->m_oArgs.begin();
		m4bool_t bFirst = M4_TRUE;
		while (oItArg != poTemplate->m_oArgs.end()) {
			if (bFirst) {
				bFirst = M4_FALSE;
			}
			else {
				oRes.append(",");
			}

			oRes.append((*oItArg).first);
			oRes.append("=");
			oRes.append((*oItArg).second);

			// next
			++oItArg;
		}
		oRes.append(")");

		// next
		++oIt;
	}

	return oRes;
  //## end ClMDTTransformerContext::GenerateUpdateFunction%1084347797.body
}

m4return_t ClMDTTransformerContext::ProcGenerateFunctions ()
{
  //## begin ClMDTTransformerContext::ProcGenerateFunctions%1084353848.body preserve=yes
	if (PopulateCalculatedFields() != M4_SUCCESS) {
		return M4_ERROR;
	}

	return ExecuteTemplates();
  //## end ClMDTTransformerContext::ProcGenerateFunctions%1084353848.body
}

m4return_t ClMDTTransformerContext::PopulateCalculatedFields ()
{
  //## begin ClMDTTransformerContext::PopulateCalculatedFields%1084353849.body preserve=yes
	// scans calculated fields
	ClMDTStringSet::iterator oIt = m_oCalculatedFieldsSet.begin();
	m4return_t iRes = M4_ERROR;
	while (oIt != m_oCalculatedFieldsSet.end()) {
		string sName = *oIt;
		iRes = m_oLoader.AddNewField(sName);
		if (iRes != M4_SUCCESS) {
			return iRes;
		}

		// next
		++oIt;
	}

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::PopulateCalculatedFields%1084353849.body
}

m4return_t ClMDTTransformerContext::ExecuteTemplates ()
{
  //## begin ClMDTTransformerContext::ExecuteTemplates%1084353852.body preserve=yes

	m4return_t iRes = M4_SUCCESS;

	ClMDTTemplateVector::iterator oIt = m_oTemplateVector.begin();
	for (;oIt != m_oTemplateVector.end(); ++ oIt) {

		// checks for update mode. Gets the first one
		if ((*oIt)->m_bIsGenerate == M4_FALSE) {
			continue;
		}

		// checks the function name (only 1 supported)
		ClMDTTemplateFunction *poTemplate = *oIt;
		ClMDTStringTemplateMap::iterator oItImpl = m_oTemplateImplMap.find(poTemplate->m_sName);

		// check function
		M4MDT_RETURN_ERROR1( oItImpl == m_oTemplateImplMap.end(), M4_ERROR, M4MDTERROR_GENERATE_UPDATE_FUNCTION, poTemplate->m_sName.c_str(), "%s" ) ;
		
		// processes all registers
		m4int32_t iNRegs = m_oLoader.GetNRecords();
		for (m4int32_t iReg = 0; iReg < iNRegs; iReg++)  {
			m4return_t iRes = ((&m_oTemplateImpl)->*((*oItImpl).second))(iReg,poTemplate);
			M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTransformerContext::ExecuteTemplates", "(*oItImpl).second(iReg,poTemplate,m_oLoader)", "%s#%s" ) ;
		}
	}

	return M4_SUCCESS;
  //## end ClMDTTransformerContext::ExecuteTemplates%1084353852.body
}

// Additional Declarations
  //## begin ClMDTTransformerContext%407D0C0E0119.declarations preserve=yes
  //## end ClMDTTransformerContext%407D0C0E0119.declarations

// Class ClMDTContextManager 








ClMDTContextManager::ClMDTContextManager (IMDTService* ai_poService, string& ai_sConnectionId, ClDoubleXMLStreamGenerator* &ai_poOutput)
  //## begin ClMDTContextManager::ClMDTContextManager%1081852936.hasinit preserve=no
      :
	m_poService(ai_poService),
	m_sConnectionId(ai_sConnectionId),
	m_bHasData(M4_FALSE),
	m_bHasExcludedData(M4_FALSE),
	m_poContextList(NULL),
	m_poCurrent(NULL),
	m_poOutput(ai_poOutput)
  //## end ClMDTContextManager::ClMDTContextManager%1081852936.hasinit
  //## begin ClMDTContextManager::ClMDTContextManager%1081852936.initialization preserve=yes
  //## end ClMDTContextManager::ClMDTContextManager%1081852936.initialization
{
  //## begin ClMDTContextManager::ClMDTContextManager%1081852936.body preserve=yes
  //## end ClMDTContextManager::ClMDTContextManager%1081852936.body
}



//## Other Operations (implementation)
m4return_t ClMDTContextManager::AddContext (string& ai_sType)
{
  //## begin ClMDTContextManager::AddContext%1081852932.body preserve=yes

	m4return_t iRes = M4_SUCCESS;

	// Flushes previous context
	if (m_poCurrent != NULL) {
		iRes = m_poCurrent->FlushTable();
		M4MDT_CHECK_ERROR1(iRes != M4_SUCCESS,M4MDTERROR_TR_GENERATE_TABLE, GetCurrent().GetType().c_str(),"%s");	
	}

	// creates a new context
	ClMDTTransformerContext *poNew = new ClMDTTransformerContext(ai_sType,m_poCurrent,m_poService,m_sConnectionId,m_poOutput);
	if (m_poContextList == NULL) {
		m_poContextList = poNew;
	}
	else {
		m_poCurrent->AddChild(poNew);
	}
	m_poCurrent = poNew;
	
	return iRes;
  //## end ClMDTContextManager::AddContext%1081852932.body
}

m4return_t ClMDTContextManager::LeaveContext ()
{
  //## begin ClMDTContextManager::LeaveContext%1081852933.body preserve=yes
	if (m_poCurrent == NULL) {
		return M4_SUCCESS;
	}

	// Flushes the current context
	m4return_t iRes = m_poCurrent->FlushTable();
	M4MDT_CHECK_ERROR1(iRes != M4_SUCCESS,M4MDTERROR_TR_GENERATE_TABLE, GetCurrent().GetType().c_str(),"%s");	

	// gets the parent
	ClMDTTransformerContext *poParent = m_poCurrent->GetParent();

	// propagate flags
	if (m_poCurrent->GetHasData()) {
		SetHasData(M4_TRUE);
	}

	if (m_poCurrent->GetHasExcludedData()) {
		SetHasExcludedData(M4_TRUE);
	}

	delete m_poCurrent;
	m_poCurrent = poParent;

	if (m_poCurrent != NULL) {
		m_poCurrent->AddChild(NULL);
	}

	return iRes;
  //## end ClMDTContextManager::LeaveContext%1081852933.body
}

// Additional Declarations
  //## begin ClMDTContextManager%407D5089020F.declarations preserve=yes
  //## end ClMDTContextManager%407D5089020F.declarations

// Class ClMDTTemplateFunction 




ClMDTTemplateFunction::ClMDTTemplateFunction()
  //## begin ClMDTTemplateFunction::ClMDTTemplateFunction%.hasinit preserve=no
      : m_bIsGenerate(M4_FALSE)
  //## end ClMDTTemplateFunction::ClMDTTemplateFunction%.hasinit
  //## begin ClMDTTemplateFunction::ClMDTTemplateFunction%.initialization preserve=yes
  //## end ClMDTTemplateFunction::ClMDTTemplateFunction%.initialization
{
  //## begin ClMDTTemplateFunction::ClMDTTemplateFunction%.body preserve=yes
  //## end ClMDTTemplateFunction::ClMDTTemplateFunction%.body
}



//## Other Operations (implementation)
m4void_t ClMDTTemplateFunction::AddArg (string& ai_sName, string& ai_sValue)
{
  //## begin ClMDTTemplateFunction::AddArg%1084288695.body preserve=yes
	m_oArgs.insert(ClMDTStringMap::value_type(ai_sName,ai_sValue));
  //## end ClMDTTemplateFunction::AddArg%1084288695.body
}

m4return_t ClMDTTemplateFunction::GetArg (string& ai_sName, string& ao_sValue)
{
  //## begin ClMDTTemplateFunction::GetArg%1084353856.body preserve=yes
	ClMDTStringMap::iterator oIt = m_oArgs.find(ai_sName);
	if (oIt == m_oArgs.end()) {
		ao_sValue = "" ;
		return M4_ERROR;
	}

	ao_sValue = (*oIt).second ;
	return M4_SUCCESS;
  //## end ClMDTTemplateFunction::GetArg%1084353856.body
}

// Additional Declarations
  //## begin ClMDTTemplateFunction%40A0F86E019C.declarations preserve=yes
  //## end ClMDTTemplateFunction%40A0F86E019C.declarations

// Class ClMDTProcTemplateImpl 




ClMDTProcTemplateImpl::ClMDTProcTemplateImpl (ClMDTTableLoader& ai_oLoader, IMDTService& ai_oService)
  //## begin ClMDTProcTemplateImpl::ClMDTProcTemplateImpl%1088582842.hasinit preserve=no
      : m_oLoader(ai_oLoader), m_oService(ai_oService)
  //## end ClMDTProcTemplateImpl::ClMDTProcTemplateImpl%1088582842.hasinit
  //## begin ClMDTProcTemplateImpl::ClMDTProcTemplateImpl%1088582842.initialization preserve=yes
  //## end ClMDTProcTemplateImpl::ClMDTProcTemplateImpl%1088582842.initialization
{
  //## begin ClMDTProcTemplateImpl::ClMDTProcTemplateImpl%1088582842.body preserve=yes
  //## end ClMDTProcTemplateImpl::ClMDTProcTemplateImpl%1088582842.body
}



//## Other Operations (implementation)
m4return_t ClMDTProcTemplateImpl::Presentation2OBL (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate)
{
  //## begin ClMDTProcTemplateImpl::Presentation2OBL%1088582841.body preserve=yes

	// gets the arguments
	const m4int32_t iNArgs = 10;
	static m4pcchar_t vpccArgNames[iNArgs] = {"presentation","package","bra","eng","esp","fra","gen","ger","ita","obl"};
	m4return_t iRes = M4_SUCCESS;

	string sParam;
	string sArgName;

	ClMDTValue vsParamValues[iNArgs];

	m4pchar_t pcObl = NULL;
	size_t iObl = 0;


	// processes all args
	for (m4int32_t i=0; i<iNArgs-1; i++) {

		sArgName = vpccArgNames[i];
		iRes = ai_poTemplate->GetArg(sArgName, sParam);
		M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Presentation2OBL", "ai_poTemplate->GetArg", "%s#%s" ) ;

		iRes = m_oLoader.GetValue(ai_iReg,sParam,vsParamValues[i]);
		M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Presentation2OBL", "m_oLoader.GetValue", "%s#%s" ) ;
	}

	// Obtener las configuraciones activas.
	vector<string> vEnableConfigurationIds;
	vEnableConfigurationIds.reserve(10);
	iRes = m_oService.GetIDictionary()->GetEnableConfigurationIds(vEnableConfigurationIds);
	M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::GetEnableConfigurationIds", "poPresentationTool->Presentation2OBL", "%s#%s" ) ;

	// Generar el OBL a partir de los packages. A la hora de generar este OBL,
	// se tendrán en cuenta las configuraciones de los idiomas que estén activas.
	IMDTPresentationTool *poPresentationTool = m_oService.GetIPresentationTool();
	iRes = poPresentationTool->Presentation2OBL(
		vsParamValues[1].c_str(),vsParamValues[1].size(),
		vsParamValues[2].c_str(),vsParamValues[2].size(),
		vsParamValues[3].c_str(),vsParamValues[3].size(),
		vsParamValues[4].c_str(),vsParamValues[4].size(),
		vsParamValues[5].c_str(),vsParamValues[5].size(),
		vsParamValues[6].c_str(),vsParamValues[6].size(),
		vsParamValues[7].c_str(),vsParamValues[7].size(),
		vsParamValues[8].c_str(),vsParamValues[8].size(),
		pcObl,iObl,
		vsParamValues[0].c_str(), vEnableConfigurationIds);
	
	M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS || pcObl == NULL, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Presentation2OBL", "poPresentationTool->Presentation2OBL", "%s#%s" ) ;

	vsParamValues[9].Set( pcObl, iObl);
	delete[] pcObl;

	sArgName = vpccArgNames[9];
	iRes = ai_poTemplate->GetArg(sArgName, sParam);
	M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Presentation2OBL", "ai_poTemplate->GetArg", "%s#%s" ) ;

	// updates the output field
	iRes = m_oLoader.SetValue(ai_iReg,sParam,vsParamValues[9]);
	M4MDT_RETURN_ERROR2( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Presentation2OBL", "m_oLoader.SetValue", "%s#%s" ) ;

	return M4_SUCCESS;
  //## end ClMDTProcTemplateImpl::Presentation2OBL%1088582841.body
}

m4return_t ClMDTProcTemplateImpl::BLOB2File (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate)
{
  //## begin ClMDTProcTemplateImpl::BLOB2File%1089021409.body preserve=yes

	m4return_t	iResult = M4_ERROR ;
	size_t		iSize = 0 ;
	m4pcchar_t	pccValue = NULL ;
	m4pcchar_t	pccTemp = NULL ;
	string		sName ;
	string		sParam ;
	ClMDTValue	vBlob ;
	ClMDTValue	vDescription ;
	ClMDTValue	vExtension ;
	ClMDTValue	vContent ;


	// Se parsea el argumento de entrada
	sName = "blob" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.GetValue( ai_iReg, sParam, vBlob ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "m_oLoader.GetValue", "%s#%s" ) ;

	// Si es nulo se ponen todos los de salida a nulo
	if( vBlob.IsNull() == M4_TRUE )
	{
		vDescription.SetNull() ;
		vExtension.SetNull() ;
		vContent.SetNull() ;
	}
	else
	{
		pccValue = vBlob.c_str() ;
		pccTemp = pccValue ;

		if( strncmp( pccTemp, M4LDB_BLOB_DESCR_PREFIX, M4LDB_BLOB_DESCR_LEN ) == 0 )
		{
			pccTemp += M4LDB_BLOB_DESCR_LEN ;
			vDescription.Set( pccTemp ) ;
			pccTemp += strlen( pccTemp ) + 1 ;
		}
		else
		{
			vDescription.SetNull() ;
		}

		vExtension.Set( pccTemp ) ;
		pccTemp += strlen( pccTemp ) + 1 ;

		iSize = vBlob.size() - ( pccTemp - pccValue ) ;

		vContent.Set( pccTemp, iSize ) ;
	}


	// Se rellenan los argumentos de salida
	sName = "description" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.SetValue( ai_iReg, sParam, vDescription ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "m_oLoader.SetValue", "%s#%s" ) ;

	sName = "extension" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.SetValue( ai_iReg, sParam, vExtension ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "m_oLoader.SetValue", "%s#%s" ) ;

	sName = "file" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.SetValue( ai_iReg, sParam, vContent ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "m_oLoader.SetValue", "%s#%s" ) ;

	return( M4_SUCCESS ) ;
  //## end ClMDTProcTemplateImpl::BLOB2File%1089021409.body
}

m4return_t ClMDTProcTemplateImpl::BLOB2External (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate)
{
  //## begin ClMDTProcTemplateImpl::BLOB2File%1089021409.body preserve=yes

	m4return_t	iResult = M4_ERROR ;
	char		c = 0 ;
	size_t		iWrite = 0 ;
	size_t		iSize = 0 ;
	string		sName ;
	string		sParam ;
	m4pcchar_t	pccValue = NULL ;
	m4pcchar_t	pccExtension = NULL ;
	m4pcchar_t	pccFile = NULL ;
	m4pcchar_t	pccTemp = NULL ;
	FILE		*pfFile = NULL ;
	ClMDTValue	vBlob ;
	ClMDTValue	vPk ;
	ClMDTValue	vDescription ;
	ClMDTValue	vExtension ;
	ClMDTValue	vFile ;


	// Se parsea el argumento de entrada
	sName = "blob" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2External", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.GetValue( ai_iReg, sParam, vBlob ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2External", "m_oLoader.GetValue", "%s#%s" ) ;

	// Si es nulo se ponen todos los de salida a nulo
	if( vBlob.IsNull() == M4_TRUE )
	{
		vDescription.SetNull() ;
		vExtension.SetNull() ;
		vFile.SetNull() ;
	}
	else
	{
		pccValue = vBlob.c_str() ;
		pccTemp = pccValue ;

		if( strncmp( pccTemp, M4LDB_BLOB_DESCR_PREFIX, M4LDB_BLOB_DESCR_LEN ) == 0 )
		{
			pccTemp += M4LDB_BLOB_DESCR_LEN ;
			vDescription.Set( pccTemp ) ;
			pccTemp += strlen( pccTemp ) + 1 ;
		}
		else
		{
			vDescription.SetNull() ;
		}

		pccExtension = pccTemp ;
		vExtension.Set( pccTemp ) ;
		pccTemp += strlen( pccTemp ) + 1 ;

		iSize = vBlob.size() - ( pccTemp - pccValue ) ;


		// Se obtiene el nombre del fichero, que se compone con el nombre de la
		// tabla, el del campo y el de las pks de la tabla
		vFile.reserve( 1024 ) ;
		vFile = ai_poTemplate->m_sOutputName ;
		vFile.append( "." ) ;
		vFile.append( m_oLoader.GetContext()->GetType() ) ;
		vFile.append( "@" ) ;
		vFile.append( sParam.c_str() + 1 ) ;

		c = '1' ;
		iResult = M4_SUCCESS ;

		do
		{
			sName = "pk" ;
			sName.append( 1, c ) ;
			iResult = ai_poTemplate->GetArg( sName, sParam ) ;

			if( iResult == M4_SUCCESS )
			{
				iResult = m_oLoader.GetValue( ai_iReg, sParam, vPk ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2External", "m_oLoader.GetValue", "%s#%s" ) ;

				vFile.append( "@" ) ;
				vFile.append( vPk ) ;
			}

			c++ ;

		} while( iResult == M4_SUCCESS ) ;

		vFile.append( "." ) ;
		vFile.append( pccExtension ) ;


		// Se guarda el contenido del fichero
		pccFile = vFile.c_str() ;

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		pfFile = M4Fopen( pccFile, M4UniWriteBinary, eEncoding ) ;
		M4MDT_RETURN_ERROR2( pfFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, pccFile, strerror( errno ), "%s#%s" ) ;

		iWrite = fwrite( pccTemp, sizeof( m4char_t ), iSize, pfFile ) ;
		fclose( pfFile ) ;
		M4MDT_RETURN_ERROR2( iWrite != iSize, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, pccFile, strerror( errno ), "%s#%s" ) ;
	}

	
	// Se rellenan los argumentos de salida
	sName = "description" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2External", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.SetValue( ai_iReg, sParam, vDescription ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "m_oLoader.SetValue", "%s#%s" ) ;

	sName = "extension" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.SetValue( ai_iReg, sParam, vExtension ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2File", "m_oLoader.SetValue", "%s#%s" ) ;

	sName = "file" ;
	iResult = ai_poTemplate->GetArg( sName, sParam ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2External", "ai_poTemplate->GetArg", "%s#%s" ) ;

	iResult = m_oLoader.SetValue( ai_iReg, sParam, vFile ) ;
	M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::BLOB2External", "m_oLoader.SetValue", "%s#%s" ) ;


	return( M4_SUCCESS ) ;
  //## end ClMDTProcTemplateImpl::BLOB2File%1089021409.body
}

m4return_t ClMDTProcTemplateImpl::PresentationPackage2External (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate)
{
	m4return_t	iResult = M4_ERROR;
	size_t		iWrite = 0; 
	size_t		iSize = 0;
	string		sName;
	string		sParam;
	m4pcchar_t	pccValue = NULL;
	m4pcchar_t	pccFile = NULL;
	FILE		*pfFile = NULL;
	ClMDTValue	vPackage;
	ClMDTValue	vPk;
	ClMDTValue	vFile;

	// Se parsea el argumento de entrada.
	sName = "package";
	iResult = ai_poTemplate->GetArg(sName, sParam);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::PresentationPackage2External", "ai_poTemplate->GetArg", "%s#%s");

	iResult = m_oLoader.GetValue(ai_iReg, sParam, vPackage);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::PresentationPackage2External", "m_oLoader.GetValue", "%s#%s");

	// Si es nulo se ponen todos los de salida a nulo.
	if (vPackage.IsNull() == M4_TRUE)
	{
		vFile.SetNull();
	}
	else
	{
		pccValue = vPackage.c_str();
		iSize = vPackage.size();

		// Se obtiene el nombre del fichero, que se compone con el nombre de la
		// tabla, el del campo y la pk de la tabla.
		vFile.reserve(1024);
		vFile = ai_poTemplate->m_sOutputName;
		vFile.append(".");
		vFile.append(m_oLoader.GetContext()->GetType());
		vFile.append("@");
		vFile.append(sParam.c_str() + 1);

		sName = "pk1";
		iResult = ai_poTemplate->GetArg(sName, sParam);
		if (iResult == M4_SUCCESS)
		{
			iResult = m_oLoader.GetValue(ai_iReg, sParam, vPk);
			M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::PresentationPackage2External", "m_oLoader.GetValue", "%s#%s");

			vFile.append("@");
			vFile.append(vPk);
		}

		vFile.append(".txt");

		// Se guarda el contenido del fichero
		pccFile = vFile.c_str();

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		pfFile = M4Fopen( pccFile, M4UniWriteBinary, eEncoding ) ;
		M4MDT_RETURN_ERROR2(pfFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, pccFile, strerror(errno), "%s#%s");

		iWrite = fwrite(pccValue, sizeof(m4char_t), iSize, pfFile);
		fclose(pfFile);
		M4MDT_RETURN_ERROR2(iWrite != iSize, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, pccFile, strerror(errno), "%s#%s");
	}

	// Se rellenan los argumentos de salida
	sName = "file";
	iResult = ai_poTemplate->GetArg(sName, sParam);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::PresentationPackage2External", "ai_poTemplate->GetArg", "%s#%s");

	iResult = m_oLoader.SetValue(ai_iReg, sParam, vFile);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::PresentationPackage2External", "m_oLoader.SetValue", "%s#%s");

	return M4_SUCCESS;
}

m4return_t ClMDTProcTemplateImpl::Field2File (m4int32_t ai_iReg, ClMDTTemplateFunction* ai_poTemplate)
{
	m4return_t	iResult = M4_ERROR;
	size_t		iWrite = 0; 
	int			iLength = 0;
	string		sName;
	string		sParam;
	m4pchar_t	pcValue = NULL;
	m4pcchar_t	pccFile = NULL;
	FILE		*pfFile = NULL;
	ClMDTValue	vField;
	ClMDTValue	vFile;

	// Se parsea el argumento de entrada.
	sName = "field";
	iResult = ai_poTemplate->GetArg(sName, sParam);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Field2File", "ai_poTemplate->GetArg", "%s#%s");

	iResult = m_oLoader.GetValue(ai_iReg, sParam, vField);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Field2File", "m_oLoader.GetValue", "%s#%s");

	// Si es nulo se ponen todos los de salida a nulo.
	if (vField.IsNull() == M4_TRUE)
	{
		vFile.SetNull();
	}
	else
	{
		// Se obtiene el nombre del fichero a partir del nombre del campo.
		vFile.reserve(1024);
		vFile = ai_poTemplate->m_sOutputName;
		vFile.append(".");
		vFile.append(sParam.c_str() + 1);
		vFile.append(".txt");

		// Se guarda el contenido del fichero
		pccFile = vFile.c_str();

		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniANSI ;
		pfFile = M4Fopen( pccFile, M4UniWriteBinary, eEncoding ) ;
		M4MDT_RETURN_ERROR2(pfFile == NULL, M4_ERROR, M4MDTERROR_NO_OPEN_FILE, pccFile, strerror(errno), "%s#%s");

		// Bug 164684. El formato debe quedar en formato UTF-8 independientemente de que
		// estemos en build Unicode o no (VB lo interpretará así)
		iLength = vField.size();
		pcValue = M4CppToUtf8( vField.c_str(), iLength ) ;

		iWrite = fwrite(pcValue, sizeof(m4char_t), iLength, pfFile);
		fclose(pfFile);
		delete [] pcValue ;

		M4MDT_RETURN_ERROR2(iWrite != iLength, M4_ERROR, M4MDTERROR_NO_WRITE_FILE, pccFile, strerror(errno), "%s#%s");
	}

	// Se rellenan los argumentos de salida
	sName = "file";
	iResult = ai_poTemplate->GetArg(sName, sParam);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Field2File", "ai_poTemplate->GetArg", "%s#%s");

	iResult = m_oLoader.SetValue(ai_iReg, sParam, vFile);
	M4MDT_RETURN_ERROR2(iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTProcTemplateImpl::Field2File", "m_oLoader.SetValue", "%s#%s");

	return M4_SUCCESS;
}

// Additional Declarations
  //## begin ClMDTProcTemplateImpl%40E2781C021C.declarations preserve=yes
  //## end ClMDTProcTemplateImpl%40E2781C021C.declarations

// Class ClMDTTableLoader 














ClMDTTableLoader::ClMDTTableLoader (IMDTService* ai_poService, ClDoubleXMLStreamGenerator* &ai_poOutput)
  //## begin ClMDTTableLoader::ClMDTTableLoader%1082553358.hasinit preserve=no
      : m_poService(ai_poService), m_poContext(NULL), m_poOutput(ai_poOutput)
  //## end ClMDTTableLoader::ClMDTTableLoader%1082553358.hasinit
  //## begin ClMDTTableLoader::ClMDTTableLoader%1082553358.initialization preserve=yes
  //## end ClMDTTableLoader::ClMDTTableLoader%1082553358.initialization
{
  //## begin ClMDTTableLoader::ClMDTTableLoader%1082553358.body preserve=yes
	m_oParameters.Init(M4MDT_VECTOR_DEFAULT_SIZE);

	// reservers
	m_oFieldsVector.reserve(128);
	m_oPKRefVector.reserve(16);
	m_oRegisterRef.reserve(128);
  //## end ClMDTTableLoader::ClMDTTableLoader%1082553358.body
}


ClMDTTableLoader::~ClMDTTableLoader ()
{
  //## begin ClMDTTableLoader::~ClMDTTableLoader%1086770005.body preserve=yes
  //## end ClMDTTableLoader::~ClMDTTableLoader%1086770005.body
}



//## Other Operations (implementation)
m4void_t ClMDTTableLoader::AddField (DOMNode *ai_poNode)
{
  //## begin ClMDTTableLoader::AddField%1082385413.body preserve=yes
	m_oFieldsVector.push_back(ai_poNode);
  //## end ClMDTTableLoader::AddField%1082385413.body
}

m4void_t	ClMDTTableLoader::SetLogicObject( DOMNode *ai_poNode )
{
	m_poLogicObject = ai_poNode ;
}

m4return_t ClMDTTableLoader::AddNewField (string& ai_sName)
{
  //## begin ClMDTTableLoader::AddNewField%1084353850.body preserve=yes
	// checks if field already defined
	ClMDTStringIntMap::iterator oIt = m_oFieldMap.find(ai_sName);
	if (oIt != m_oFieldMap.end()) {
		// already included. Skip
		return M4_SUCCESS;
	}

	// adds a new reference 
	m4int32_t iNewRef = m_oFieldMap.size();
	m_oFieldMap.insert(ClMDTStringIntMap::value_type(ai_sName,iNewRef));
	ClMDTValue vNull;

	// adds a new reference for all registers
	m4int32_t iNRegs = m_oResultSet.GetRowNumber();
	for (m4int32_t i=0; i< iNRegs; i++) {
		m4return_t iRes = m_oResultSet.SetValue(i,iNewRef,vNull);
		M4MDT_RETURN_ERROR0(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_ADD_COLUMN);
	}

	return M4_SUCCESS;
  //## end ClMDTTableLoader::AddNewField%1084353850.body
}

m4return_t ClMDTTableLoader::Load ()
{
  //## begin ClMDTTableLoader::Load%1082385414.body preserve=yes

	m4return_t iRes = M4_ERROR;
	
	iRes = GenerateLoadSelect();
	M4MDT_RETURN_ERROR0(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_TABLE_SELECT);

	// the virtual number of parent registers of a root node
	m4int32_t iNRegs = 1;

	// in case of non root node we get the number of parent registers
	if (m_poContext->IsRoot() == M4_FALSE) {
		iNRegs = GetParentNRecords();
	}

	for (m4int32_t i=0; i<iNRegs; i++) {
		m4return_t iRes = LoadBlock(i);
		M4MDT_RETURN_ERROR1(iRes != M4_SUCCESS , M4_ERROR, M4MDTERROR_TR_BLOCK_LOAD, i, "%d");
	}

	return M4_SUCCESS;
  //## end ClMDTTableLoader::Load%1082385414.body
}

m4return_t ClMDTTableLoader::GenerateLoadSelect ()
{
  //## begin ClMDTTableLoader::GenerateLoadSelect%1082385417.body preserve=yes
	// scans all fields

	m4return_t		iResult = M4_ERROR ;
	m4bool_t		bFirstFilter = M4_FALSE ;
	m4uint32_t		i = 0 ;
	m4uint32_t		iLength = 0 ;
	m4int32_t		iLogicPos = -1 ;
	m4int32_t		iPKPos = -1 ;
	m4int32_t		iHasWhere = -1 ;
	m4int32_t		iFilter = -1 ;
	ClCharHolder	pcLogicPosition ;
	ClCharHolder	pcPKPosition ;
	ClCharHolder	pcAlias ;
	ClCharHolder	pcRealField ;
	ClCharHolder	pcRealObject ;
	ClCharHolder	pcM4Type ;
	ClCharHolder	pcPrecision ;
	ClCharHolder	pcScale ;
	ClCharHolder	pcHasWhere ;
	string			sName ;
	string			sPK ;
	string			sPKOperation ;
	string			sFilter ;
	DOMNode			*poNode = NULL ;
	ClMDTStringSet	oParamPKSet;


	sFilter.reserve( 1024 ) ;

	iFilter = 0 ;
	bFirstFilter = M4_TRUE ;

	iLength = m_oFieldsVector.size() ;

	for( i = 0 ; i < iLength ; i++ )
	{

		poNode = m_oFieldsVector[ i ] ;

		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, poNode, sName ) ;
		// error
		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChLogicPosition, poNode, pcLogicPosition ) ;
		// error
		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChPkPosition, poNode, pcPKPosition ) ;
		// error

		iLogicPos = atoi( pcLogicPosition ) ;

		if( iLogicPos != 0 )
		{
			m_oFieldMap.insert(ClMDTStringIntMap::value_type(sName, iLogicPos - 1 )) ;

			iPKPos = atoi( pcPKPosition ) ;

			if( iPKPos != 0 )
			{
				m_oPKPos.push_back( iLogicPos - 1 ) ;
			}

			iResult = m_poContext->GetPK( sName, sPK ) ;

			if( iResult == M4_SUCCESS )
			{
				oParamPKSet.insert( sName ) ;

				m_oPKRefVector.push_back( sPK ) ;
				
				if( bFirstFilter == M4_TRUE )
				{
					bFirstFilter = M4_FALSE ;
				}
				else
				{
					sFilter.append( " AND " ) ;
				}

				iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChAlias, poNode, pcAlias ) ;
				// error
				iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChRealField, poNode, pcRealField ) ;
				// error
				iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChM4Type, poNode, pcM4Type ) ;
				// error
				iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChPrecision, poNode, pcPrecision ) ;
				// error
				iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChScale, poNode, pcScale ) ;
				// error

				sFilter.append( pcAlias ) ;
				sFilter.append( "." ) ;
				sFilter.append( pcRealField ) ;
				sFilter.append( " " ) ;

				/*Bug 0121653
				Se obtiene el operando
				No hay error nunca porque siempre se devuelve al menos el "="
				*/
				iResult = m_poContext->GetPKOperation( sName, sPKOperation ) ;
				sFilter.append( sPKOperation ) ;
				sFilter.append( " ?" ) ;

				m4uint8_t iM4Type = atoi( pcM4Type );
				m4uint32_t iPrecision = atoi( pcPrecision );

				iResult = m_oParameters.SetFieldDescription( iFilter, iM4Type, iPrecision, atoi( pcScale ) ) ;
				M4MDT_RETURN_ERROR2( iResult != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::GenerateLoadSelect", iFilter, "%s#SetFieldDescription parameter = <%d>" ) ;

				iFilter++ ;
			}
		}
	}


	// checks the pk consistency: all pk must be used
	if (oParamPKSet.size() != m_poContext->GetNPKs()) {

		// dumps the error message for all non valid param pk's
		ClMDTStringMap::const_iterator oIt = m_poContext->GetParamPKIterator();
		m4int32_t iNParamPKs = m_poContext->GetNPKs();

		for (m4int32_t i=0; i<iNParamPKs; ++i,++oIt) {
			
			string sName = (*oIt).first;
			if (oParamPKSet.find(sName) == oParamPKSet.end()) {
				M4MDT_DUMP_ERROR2( M4MDTERROR_TR_INVALID_PARAM_PK, sName.c_str(), m_poContext->GetType().c_str(), "%s#%s" ) ;
			}
		}

		return M4_ERROR;
	}


	iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChSelect, m_poLogicObject, m_sSelect ) ;
	// error

	if( iFilter > 0 )
	{
		iResult = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChHasWhere, m_poLogicObject, pcHasWhere ) ;
		// error

		iHasWhere = atoi( pcHasWhere ) ;

		if( iHasWhere == 1 )
		{
			m_sSelect.append( " AND " ) ;
		}
		else
		{
			m_sSelect.append( " WHERE " ) ;
		}

		m_sSelect.append( sFilter ) ;
	}


	return M4_SUCCESS;
  //## end ClMDTTableLoader::GenerateLoadSelect%1082385417.body
}

m4return_t ClMDTTableLoader::LoadBlock (m4int32_t ai_iParentReg)
{
  //## begin ClMDTTableLoader::LoadBlock%1082553357.body preserve=yes
	
	// prepares the load parameters
	m4int32_t iNPKs = m_oPKRefVector.size();
	m4int32_t i=0;

	for (; i<iNPKs; i++) {
		ClMDTValue vParam;
		m4return_t iRes = m_poContext->ProcValue(m_oPKRefVector[i],ai_iParentReg,vParam);
		M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::LoadBlock", "%s#ProcValue" ) ;

		// checks for null pk
		if (vParam.IsNull()) {
			
			// if we are in read mode it is ok, just returns success
			if (m_poContext->GetIsReadTable()) {
				return M4_SUCCESS;
			}

			// we are in a call table mode. The null pk is a model/data error
			M4MDT_DUMP_ERROR1( M4MDTERROR_TR_CALL_TABLE_NULL_PK, m_oPKRefVector[i].c_str(), "%s" ) ;
			return M4_ERROR;
		}
		m_oParameters.SetValue(i,vParam);
	}

	// invokes the load method
	m4return_t iRes = M4_ERROR;
	iRes = m_poService->GetIODBC()->ExecuteQuery( m_poContext->GetConnectionId(), m_sSelect, &m_oParameters, M4_TRUE, m_oResultSet ) ;
	M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::LoadBlock", "%s#ExecuteQuery" ) ;

	// updates the register-block reference
	m4int32_t iNReg = m_oResultSet.GetRowNumber();
	for (i=0; i<iNReg; i++) {
		m_oRegisterRef.push_back(ai_iParentReg);
	}

	return iRes;
  //## end ClMDTTableLoader::LoadBlock%1082553357.body
}

m4int32_t ClMDTTableLoader::GetParentNRecords ()
{
  //## begin ClMDTTableLoader::GetParentNRecords%1082721865.body preserve=yes
	// gets the parent context
	ClMDTTransformerContext *poParent = m_poContext->GetParent();
	if (poParent == NULL) {
		return 0;
	}

	return poParent->GetNRecords();
  //## end ClMDTTableLoader::GetParentNRecords%1082721865.body
}

m4int32_t ClMDTTableLoader::GetParentRecord (m4int32_t ai_iReg)
{
  //## begin ClMDTTableLoader::GetParentRecord%1082967065.body preserve=yes
	if (ai_iReg >= (m4int32_t)m_oRegisterRef.size()) {
		
		// just indicates EOF
		return -1;
	}
	
	return m_oRegisterRef[ai_iReg];
  //## end ClMDTTableLoader::GetParentRecord%1082967065.body
}

m4return_t ClMDTTableLoader::GetField (const string& ai_sName, m4int32_t ai_iReg, ClMDTValue& ao_oRes)
{
  //## begin ClMDTTableLoader::GetField%1082967066.body preserve=yes

	ao_oRes = "";

	// gets the alias map
	ClMDTStringIntMap::iterator oIt = m_oFieldMap.find(ai_sName);
	if (oIt == m_oFieldMap.end()) {
		return M4_ERROR;
	}

	m4return_t iRes = m_oResultSet.GetValue(ai_iReg,(*oIt).second,ao_oRes);
	M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::GetField", "%s#GetValue" ) ;

	return M4_SUCCESS;
  //## end ClMDTTableLoader::GetField%1082967066.body
}

m4return_t ClMDTTableLoader::Dump (void)
{
  //## begin ClMDTTableLoader::Dump%1083080791.body preserve=yes

	// Nos quedamos con la posición del fichero de excluded para restaurar si no hay datos
	streampos				lStart = -1 ;
	streampos				lEnd = -1 ;
	streamoff				l = -1;
	ostream					*poStream = NULL ;
	M4XMLStreamGenerator	*poSecondStream = NULL ;
	
	poSecondStream = m_poOutput->GetSecondStream() ;

	if( poSecondStream != NULL )
	{
		poStream = poSecondStream->GetStream() ;

		if( poStream != NULL )
		{
			lStart = poStream->tellp() ;
		}
	}


	// table
	m4return_t iRes = m_poOutput->MakeElement("table", M4MDT_DATAXML_ALL);
	M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "table.start", "%s" ) ;

	iRes = m_poOutput->AddElementAttribute("name",m_poContext->GetType().c_str(), M4MDT_DATAXML_ALL);
	M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "table:name", "%s" ) ;

	string sUpdateFunction = m_poContext->GenerateUpdateFunction();
	if (sUpdateFunction.size()) {
		iRes = m_poOutput->AddElementAttribute("update-functions",sUpdateFunction.c_str(), M4MDT_DATAXML_FIRST);
		M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "table:name", "%s" ) ;
	}

	// void
	m_poOutput->EndElement(M4MDT_DATAXML_ALL);

	// register
	m4int32_t iNReg = m_oResultSet.GetRowNumber();

	// properties checks
	string sVisibilityKey("visible");
	string sMultilineKey("multiline");
	string sContentTypeKey("content-type");
	string sValueKey("value");
	string sFalseKey("false");
	string sTrueKey("true");

	// PK state
	ClMDTStringSet oPKSet;
	iRes = GetRegisterPKs(oPKSet);
	M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "register.pks", "%s" ) ;

	for (m4int32_t iReg=0; iReg<iNReg; iReg++) {

		// sets the data flag
		m_poContext->SetHasData(M4_TRUE);

		iRes = m_poOutput->MakeElement("register", M4MDT_DATAXML_ALL);
		M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "register.start", "%s" ) ;

		// void
		m_poOutput->EndElement(M4MDT_DATAXML_ALL);

		// get calculated fields definitions
		ClMDTStringSet &oCalculatedFieldsSet = m_poContext->GetCalculatedFieldsSet();
		ClMDTStringSet::iterator oItCalculated = oCalculatedFieldsSet.begin();

		// write fields
		m4int32_t iSelectFields = m_oFieldsVector.size();
		m4int32_t iNFields = iSelectFields + oCalculatedFieldsSet.size();


		// Se calcula la pk de cada registro
		string sPk ;
		sPk.reserve( 512 );

		ClMDTStringSet::iterator itPks = oPKSet.begin() ;

		while( itPks != oPKSet.end() )
		{
			string sName = itPks->c_str();
			
			ClMDTValue sValue;
			//Bug 0165917: hace falta ejecutar las transformaciones de valor antes de generar la PK
			string sValueTemp;
			if (m_poContext->CheckFieldProp(sName,sValueKey,sValueTemp) == M4_FALSE) {
				iRes = GetField(sName,iReg,sValue);
				M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_READ_VALUE, sName.c_str(), "%s" ) ;
			}
			else {

				// processes the value
				iRes = m_poContext->ProcValue(sValueTemp,0,sValue);
				M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::Dump", "%s#ProcValue" ) ;
			}

			sPk.append( sName.c_str() ) ;
			sPk.append( "=" ) ;

			if( sValue.IsNull() == M4_FALSE )
			{
				sPk.append( sValue.c_str() ) ;
			}
			sPk.append( ";" ) ;

			itPks++;
		}

		ClMDTStringSet oSet;

		for (m4int32_t iField=0; iField<iNFields; iField++) {
			// gets the field name
			string sName;
			DOMNode *poNode = NULL;
			if (iField < iSelectFields) {
				// gets from select nodes
				poNode = m_oFieldsVector[iField];
				sName ;
				iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, poNode, sName ) ;
				// error
			}
			else {
				// gets from calculated fields
				sName = *oItCalculated;
				++oItCalculated;
			}

			m4bool_t bIsVisible = M4_TRUE;
			m4bool_t bIsMultiline = M4_FALSE;
		
			// checks repetition
			if (oSet.find(sName) != oSet.end()) {
				continue;
			}
			oSet.insert(sName);


			// checks visibility
			bIsVisible = m_poContext->CheckFieldProp(sName,sVisibilityKey,sFalseKey) == M4_FALSE;

			if (poNode != NULL && bIsVisible) {
				// checks special types
				ClCharHolder pcInternalField ;
				iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChInternalField, poNode, pcInternalField ) ;
				// error

				m4int32_t iIF = atoi(pcInternalField);
				if (iIF == M4CL_INTERNAL_TYPE_T_DATE || 
					iIF == M4CL_INTERNAL_TYPE_SECURITY_USER || 
					iIF == M4CL_INTERNAL_TYPE_APP_ROLE || 
					iIF == M4CL_INTERNAL_TYPE_UNUSED_ITEM ) {
					bIsVisible = M4_FALSE;
				}
			}


			m4int32_t iPKPos = 0;
			if (bIsVisible)
			{
				m4uint8_t	iMode = M4MDT_DATAXML_NONE;
				string		sContentTypeValue;
				m4bool_t	bContentType = M4_FALSE;

				if (poNode != NULL)
				{
					// Si es pk va a los dos
					ClCharHolder pcPkPosition ;
					iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChPkPosition, poNode, pcPkPosition ) ;
					// error

					iPKPos = atoi( pcPkPosition ) ;
				}

				if( iPKPos != 0 )
				{
					iMode = M4MDT_DATAXML_ALL ;
				}
				else
				{
					m4bool_t bIsComparable = M4_TRUE;
					m4bool_t bIsTransferable = M4_TRUE;
					string sTableName = m_poContext->GetType();

					// get content-type
					if (m_poContext->CheckFieldProp(sName, sContentTypeKey, sContentTypeValue) == M4_TRUE)
					{
						bContentType = M4_TRUE;	
						if(strcmpi("presentation-package", sContentTypeValue.c_str()) == 0 ||
						   strcmpi("field-file", sContentTypeValue.c_str()) == 0)
						{
							// If content-type = "presentation-package", fields are always excluded.
							iMode |= M4MDT_DATAXML_SECOND;
							
							// sets the data flag
							m_poContext->SetHasExcludedData(M4_TRUE);
						}
						else
						{
							// Other content-types.
							iMode |= M4MDT_DATAXML_FIRST ;
						}
					}
					else
					{
						iRes = m_poService->GetIDictionary()->GetFieldConfiguration(sTableName.c_str(), sName.c_str(), bIsComparable, bIsTransferable);
						
						if( bIsComparable == M4_TRUE )
						{
							// Si es comparable va al primero
							iMode |= M4MDT_DATAXML_FIRST ;
						}
						else if( bIsTransferable == M4_TRUE )
						{
							// Si no es comparable y es transferible va al segundo
							iMode |= M4MDT_DATAXML_SECOND ;
							
							// sets the data flag
							m_poContext->SetHasExcludedData(M4_TRUE);
						}
					}
				}


				// write the tag
				iRes = m_poOutput->MakeElement("field", iMode);
				M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field", "%s" ) ;

				iRes = m_poOutput->AddElementAttribute("name",sName.c_str(), iMode);
				M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:name", "%s" ) ;

				iRes = m_poOutput->AddEscapedElementAttribute("pk",sPk.c_str(), sPk.size(), iMode);
				M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:pk", "%s" ) ;

				// checks the is-printable state
				if (oPKSet.find(sName) != oPKSet.end()) {
					iRes = m_poOutput->AddElementAttribute("is-printable","true", M4MDT_DATAXML_FIRST);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:is-printable", "%s" ) ;
				}

				ClMDTValue sValue;
				bIsMultiline = m_poContext->CheckFieldProp(sName,sMultilineKey,sTrueKey) == M4_TRUE;

				m4bool_t bIsExternal = M4_FALSE ;
				// process content-type
				// if (m_poContext->CheckFieldProp(sName,sContentTypeKey,sContentTypeValue))
				if (bContentType == M4_TRUE)
				{
					iRes = m_poOutput->AddElementAttribute("content-type",sContentTypeValue.c_str(), iMode);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:content-type", "%s" ) ;

					if( strcmpi( "external", sContentTypeValue.c_str() ) == 0 )
					{
						bIsExternal = M4_TRUE ;
					}
				}

				// gets the field value
				// checks for value by param
				string sValueTemp;
				if (m_poContext->CheckFieldProp(sName,sValueKey,sValueTemp) == M4_FALSE) {
					iRes = GetField(sName,iReg,sValue);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_READ_VALUE, sName.c_str(), "%s" ) ;
				}
				else {

					// processes the value
					iRes = m_poContext->ProcValue(sValueTemp,0,sValue);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::Dump", "%s#ProcValue" ) ;
				}

				if( bIsExternal == M4_TRUE && sValue.IsNull() == M4_FALSE )
				{
					// Si es un fichero externo válido se calcula su checksum y se quita el path
					m4uint32_t	iCheckSum = 0 ;
					m4pcchar_t	pccFile = NULL ;
					m4char_t	acCheckSum[ 8 + 1 ] ;

					iCheckSum = M4MDTCalculateCheckSum( sValue.c_str() ) ;
					sprintf( acCheckSum, "%x", iCheckSum ) ;

					iRes = m_poOutput->AddElementAttribute( "checksum", acCheckSum, M4MDT_DATAXML_FIRST ) ;
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:checksum", "%s" ) ;

					// Se obtiene el nombre sin path para el xml
					pccFile = sValue.c_str() ;
					pccFile = strrchr( pccFile, M4_BACK_SLASH[ 0 ] );

					if( pccFile != NULL )
					{
						sValue.Set( pccFile + 1 ) ;
					}
				}

				// checks di facto multiline fields
				if (bIsMultiline == M4_FALSE) {
					bIsMultiline = CheckIsMultiline(sValue);
				}

				if (bIsMultiline) {

					// checks null value
					if (sValue.IsNull()) {
						iRes = m_poOutput->AddElementAttribute("is-null","true", iMode);
					}

					iRes = m_poOutput->AddElementAttribute("multiline","true", iMode);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:multiline", "%s" ) ;

					// closes previous element
					m_poOutput->EndElement(iMode);

					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:value", "%s" ) ;

					iRes = m_poOutput->AddElementValue(M4MD_XML_CDATA_START, iMode);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:value", "%s" ) ;

					iRes = m_poOutput->AddElementValue(M4MDT_XML_CDATA_PREFIX, iMode);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:value", "%s" ) ;

					if (sValue.IsNull() == M4_FALSE) {
						iRes = m_poOutput->AddElementValue(sValue.c_str(), sValue.size(), iMode);
						M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:value", "%s" ) ;
					}

					iRes = m_poOutput->AddElementValue(M4MDT_XML_CDATA_SUFIX, iMode);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:value", "%s" ) ;

					iRes = m_poOutput->AddElementValue(M4MD_XML_CDATA_END, iMode);
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:value", "%s" ) ;
				}
				else {
					// checks null value
					if (sValue.IsNull()) {
						iRes = m_poOutput->AddElementAttribute("is-null","true", iMode);
					}
					else {
						iRes = m_poOutput->AddEscapedElementAttribute("value",sValue.c_str(), sValue.size(), iMode);
					}
					M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "field:value", "%s" ) ;
				}

				// void
				if (bIsMultiline) {
					m_poOutput->CloseElement("field", iMode);
				}
				else {
					m_poOutput->CloseSingleClosedElement(iMode);
				}
			}
		}


		iRes = m_poOutput->CloseElement("register", M4MDT_DATAXML_ALL);
		M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "register.end", "%s" ) ;
	}

	iRes = m_poOutput->CloseElement("table", M4MDT_DATAXML_ALL);
	M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_TR_WRITE_TAG, "table.end", "%s" ) ;


	// Restauramos la posición
	if( m_poContext->GetHasExcludedData() == M4_FALSE )
	{
		if( poStream != NULL )
		{
			lEnd = poStream->tellp() ;

			// Tenemos que escribir blancos, sino se queda basura
			poStream->seekp( lStart ) ;

			for (l = lStart; l < lEnd; l++)
			{
				poStream->put( ' ' ) ;
			}

			poStream->seekp( lStart ) ;
		}
	}


	return M4_SUCCESS;
  //## end ClMDTTableLoader::Dump%1083080791.body
}

m4return_t ClMDTTableLoader::GetValue (m4int32_t ai_iReg, string& ai_sName, ClMDTValue& ao_vRes)
{
  //## begin ClMDTTableLoader::GetValue%1084353854.body preserve=yes
	
	m4pcchar_t pccId = ai_sName.c_str();
	if (*pccId != '@') {
		ao_vRes = ai_sName;
		return M4_SUCCESS;
	}

	string sFieldName = pccId+1;

	return GetField(sFieldName,ai_iReg,ao_vRes);
  //## end ClMDTTableLoader::GetValue%1084353854.body
}

m4return_t ClMDTTableLoader::SetValue (m4int32_t ai_iReg, string& ai_sName, ClMDTValue& ai_vValue)
{
  //## begin ClMDTTableLoader::SetValue%1084353855.body preserve=yes

	m4pcchar_t pccId = ai_sName.c_str();
	if (*pccId != '@') {
		return M4_SUCCESS;
	}

	string sFieldName = pccId+1;

	// gets the alias map
	ClMDTStringIntMap::iterator oIt = m_oFieldMap.find(sFieldName);
	if (oIt == m_oFieldMap.end()) {
		return M4_ERROR;
	}

	m4return_t iRes = m_oResultSet.SetValue(ai_iReg,(*oIt).second,ai_vValue);
	M4MDT_RETURN_ERROR1( iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::SetValue", "%s#GetValue" ) ;

	return M4_SUCCESS;
  //## end ClMDTTableLoader::SetValue%1084353855.body
}

m4bool_t ClMDTTableLoader::CheckIsMultiline (string& ai_sValue)
{
  //## begin ClMDTTableLoader::CheckIsMultiline%1084447570.body preserve=yes
	// gets a char version
	m4pcchar_t pcValue = ai_sValue.c_str();
	m4int32_t iSize = ai_sValue.size();
	while (iSize--) {
		if (*pcValue++ == '\n') {
			return M4_TRUE;
		}
	}
	return M4_FALSE;
  //## end ClMDTTableLoader::CheckIsMultiline%1084447570.body
}

m4return_t ClMDTTableLoader::GetRegisterPKs (ClMDTStringSet& ao_oPKSet)
{
  //## begin ClMDTTableLoader::GetRegisterPKs%1084956753.body preserve=yes

	string sIsPrintable("is-printable");
	string sFalseKey("false");

	// reset output
	ao_oPKSet.clear();

	// excluded arguments
	ClMDTStringSet oSet;

	// scans all dictionary PK's and remove explicit pk's passed as argument
	m4int32_t iNFields = m_oFieldsVector.size();
	
	for (m4int32_t i=0; i<iNFields; i++) {
		DOMNode *poNode = m_oFieldsVector[i];
		
		// is a pk?
		ClCharHolder pcPKPosition ;
		m4return_t iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChPkPosition, poNode, pcPKPosition ) ;
		// error

		if (atoi(pcPKPosition) == 0) {
			continue;
		}

		string sName ;
		iRes = M4MDT_DOMGetAttribute( IMDTService::sm_sXMLChName, poNode, sName ) ;
		// error

		if( m_poContext->CheckFieldProp( sName, sIsPrintable, sFalseKey ) == M4_TRUE )
		{
			continue;
		}

		// controls repetition
		if (ao_oPKSet.find(sName) != ao_oPKSet.end()) {
			continue;
		}
		else {
			ao_oPKSet.insert(sName);
		}

		// checks for existen argument
		string sPK;
		if (m_poContext->GetPK(sName,sPK) == M4_SUCCESS) {
			oSet.insert(sName);
		}	
	}
	
	// exclude argument pk's if not all are arguments
	if (oSet.size() != ao_oPKSet.size()) {
		ClMDTStringSet:: iterator oIt = oSet.begin();
		while (oIt != oSet.end()) {
			// removes the pk from the output set
			ao_oPKSet.erase(*oIt);

			// next
			++oIt;
		}
	}

	return M4_SUCCESS;
  //## end ClMDTTableLoader::GetRegisterPKs%1084956753.body
}

m4return_t ClMDTTableLoader::Sort ()
{
  //## begin ClMDTTableLoader::Sort%1089021408.body preserve=yes

	// checks number of registers
	if (m_oResultSet.GetRowNumber() < 2) {
		return M4_SUCCESS;
	}

	// generates the vector of sorted columns
	m4int32_t iNPKs = m_oPKPos.size();
	m4int32_t *viOrder = new m4int32_t[iNPKs + 1];

	// populates the vector
	m4int32_t i;
	for (i=0; i<iNPKs; ++i) {

		viOrder[i] = m_oPKPos[i];
	}

	// marks the end of vector
	viOrder[i] = -1;

	// invoques the sort function
	m4return_t iRes = m_oResultSet.Sort(M4MDTGenericCompareFields,viOrder);

	delete [] viOrder;

	M4MDT_RETURN_ERROR2(  iRes != M4_SUCCESS, M4_ERROR, M4MDTERROR_INTERNAL_ERROR, "ClMDTTableLoader::Sort", "m_oResultSet.Sort", "%s#%s") ;

	return M4_SUCCESS;
  //## end ClMDTTableLoader::Sort%1089021408.body
}

// Additional Declarations
  //## begin ClMDTTableLoader%4085256000B0.declarations preserve=yes
  //## end ClMDTTableLoader%4085256000B0.declarations

//## begin module%407D0C8F037A.epilog preserve=yes
//## end module%407D0C8F037A.epilog
