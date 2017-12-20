//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#include "licenseproperties.hpp"
#include "encrypt.hpp"
#include "key.hpp"
#include "error.hpp"

#include "m4date.hpp"

#include <m4cryptc.hpp>
#include <m4unicode.hpp>

#ifndef _WINDOWS
	#include <netinet/in.h>
#endif

//Traducciones.
const string M4MODKEY_PROPERTY_TRANS_LICENSE_DUMPED_TIME		("Dumped Time");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_ID					("ID License");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_VERSION			("License Version");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_VERSION_GEN		("Generation Version");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_HOSTNAME			("HostName");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_GENERATION_DATE	("Generation Date");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_EXPIRATION			("Expiration");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_ID		("ID Customer");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_NAME		("Customer Name");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_COUNTRY	("Customer Country");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_GEODIV	("Customer GeoDiv");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_DESC		("Customer Description");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_NP					("NP");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_NP_DESC			("Description");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_NP_SELECT			("Select");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_NP_VALUE			("Value");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_NP_AUTHORIZED		("Authorized");
const string M4MODKEY_PROPERTY_TRANS_LICENSE_NP_LIMIT			("Limit");


//------------------------------------------------
// Clase ClPropertyValue.
//------------------------------------------------
ClPropertyValue::ClPropertyValue( const string & ai_sPropertyValue )
{
	m_sPropertyValue = ai_sPropertyValue;
}

ClPropertyValue::ClPropertyValue( const ClPropertyValue & ai_oCopy )
{	
	m_sPropertyValue = ai_oCopy.m_sPropertyValue;
}

ClPropertyValue & ClPropertyValue::operator = ( const ClPropertyValue & ai_oCopy )
{	
	m_sPropertyValue = ai_oCopy.m_sPropertyValue;
	return *this;
}

m4bool_t operator < (const ClPropertyValue & ai_oLeft, const ClPropertyValue & ai_oRight)
{
	if (ai_oLeft.m_sPropertyValue < ai_oRight.m_sPropertyValue){
		return M4_TRUE;
	}
	if (ai_oLeft.m_sPropertyValue != ai_oRight.m_sPropertyValue){
		return M4_FALSE;
	}
	return (M4_FALSE);
}

m4bool_t operator == (const ClPropertyValue & ai_oLeft, const ClPropertyValue & ai_oRight)
{
	if (ai_oLeft.m_sPropertyValue != ai_oRight.m_sPropertyValue){
		return M4_FALSE;
	}
	return (M4_TRUE);
}

m4pcchar_t ClPropertyValue::GetPropertyValue( void )
{
	return m_sPropertyValue.c_str();
}
//------------------------------------------------
// FIN Clase ClPropertyValue.
//------------------------------------------------

//------------------------------------------------
// Clase ClModKeyProperties.
//------------------------------------------------
ClModKeyProperties::ClModKeyProperties( void )
{
	//----------------------------------------------
	//Ahora inicializamos el mapa donde tenemos las traducciones de las propiedades.
	_FillPropertyIdTraslation();
	//----------------------------------------------
}

ClModKeyProperties::~ClModKeyProperties( )
{
	//Eliminamos el mapa.
	for (itMpClPropertyItem it = m_oMpClPropertyId.begin(); it != m_oMpClPropertyId.end(); it++) {
		ClPropertyValue * pClPropertyValue = (*it).second;
		delete pClPropertyValue;
	}
	m_oMpClPropertyId.clear();

	//Eliminamos el mapa.
	for (itMpClPropertyItem it2 = m_oMpPropertyTrans.begin(); it2 != m_oMpPropertyTrans.end(); it2++) {
		ClPropertyValue * pClPropertyValue = (*it2).second;
		delete pClPropertyValue;
	}
	m_oMpPropertyTrans.clear();
}

m4return_t ClModKeyProperties::_InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey )
{
	if (ai_pClEncryption == NULL) {
		return M4_ERROR;
	}

	//------------- IMPORTANTE --------------------------
	//Tenemos que seguir el protocolo.
	m4date_t dNow;
	ClActualDate (dNow);
	m4int32_t iSizeKeyA = sizeof(dNow) + strlen(ai_pKey);
	m4char_t * pKeyA = new m4char_t[iSizeKeyA + 1];
	if (pKeyA == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memcpy(pKeyA, &dNow, sizeof(dNow));
	memcpy(pKeyA + sizeof(dNow), ai_pKey, strlen(ai_pKey));
	pKeyA[iSizeKeyA] = NULL;
	
	//----------------------------------------------------
	//Aplicamos la encriptación de la ClaveA.
	//----------------------------------------------------
	m4pchar_t pBufferEncryptedWithKeyA = pKeyA;
	m4int32_t iSizeEncryptedWithKeyA = iSizeKeyA;

	m4int32_t iLeft = 0;
	m4int32_t iRight = (iSizeEncryptedWithKeyA - 1);	
	while (iLeft <= iRight)
	{
		if ((iLeft % 2) == 0) {
			//Es caracter par.			
			m4char_t cTemp = *(pBufferEncryptedWithKeyA + iRight);
			*(pBufferEncryptedWithKeyA + iRight) = *(pBufferEncryptedWithKeyA + iLeft);
			*(pBufferEncryptedWithKeyA + iLeft) = cTemp;
		}
		iLeft++;
		iRight--;
	}

	//Restauramos para dar otra vuelta.
	pBufferEncryptedWithKeyA = pKeyA;

	for (m4int32_t i=0; i<iSizeEncryptedWithKeyA; i++)
	{		
		*pBufferEncryptedWithKeyA = ((*pBufferEncryptedWithKeyA) + M4MODKEY_ENCRYPT_KEYA);
		pBufferEncryptedWithKeyA++;
	}
	//----------------------------------------------------
	//FIN Aplicamos la encriptación de la ClaveA.
	//----------------------------------------------------

	//Emviamos la clave a utilizar!. Esperamos una fecha.
	m4char_t * pSignature = new m4char_t[sizeof(dNow) + 1];
	if (ai_pClEncryption->SendKeyB( pKeyA, iSizeKeyA, pSignature, sizeof(dNow) ) == M4_ERROR) {
		delete [] pKeyA;
		delete [] pSignature;
		ClErrorManager::DumpError( M4MODKEY_ERROR_SEND_KEY );
		return M4_ERROR;
	}

	//Verificamos que la fecha es correcta.
	if (memcmp(&dNow, pSignature, sizeof(dNow)) != 0) {
		delete [] pKeyA;
		delete [] pSignature;
		ClErrorManager::DumpError( M4MODKEY_ERROR_VERIFYING_KEY );
		return M4_ERROR;
	}

	//Si estamos aqui es pq el protocolo se ha validado. Desmontamos la paraeta.
	delete [] pKeyA;
	delete [] pSignature;
	pKeyA = NULL;
	pSignature = NULL;
	//------------- FIN IMPORTANTE --------------------------

	return M4_SUCCESS;
}

m4return_t ClModKeyProperties::Init( m4pcchar_t ai_pLicenseSummaryEncrypted, m4int32_t ai_SizeLicenseSummaryEncrypted )
{
	//Tendremos que desencriptar el resumen e ir metiendo las propiedades.
	//Para cada propiedad se encripta: #Etiqueta1#Valor1#Etiqueta2#Valor2#

	//---- Desencriptacion ------
	//Llamamos directamente a la desencriptacion pq conocemos la clave con que
	//se encripta.

	// Si el sumario está dentro de lo que se pasa se busca
	m4pchar_t	pcWatermarkStart = NULL ;
	m4pchar_t	pcWatermarkEnd = NULL ;

	pcWatermarkStart = (m4pchar_t) strstr( ai_pLicenseSummaryEncrypted, "Watermark. " ) ;

	if( pcWatermarkStart != NULL )
	{
		pcWatermarkStart += strlen( "Watermark. " ) ;
		ai_SizeLicenseSummaryEncrypted -= ( pcWatermarkStart - ai_pLicenseSummaryEncrypted ) ;
		ai_pLicenseSummaryEncrypted = pcWatermarkStart ;

		pcWatermarkEnd = strchr( pcWatermarkStart, '\r' ) ;

		if( pcWatermarkEnd == NULL )
		{
			pcWatermarkEnd = strchr( pcWatermarkStart, '\n' ) ;
		}

		if( pcWatermarkEnd != NULL )
		{
			*pcWatermarkEnd = '\0' ;
			ai_SizeLicenseSummaryEncrypted = pcWatermarkEnd - pcWatermarkStart ;
		}
	}

	//Desencriptamos.
	ClEncryption oClEncryption;

	if (_InitEncrypter(&oClEncryption, M4MODKEY_ENCRYPT_KEY_SUMMARY) == M4_ERROR) {
		return M4_ERROR;
	}

	//Si estamos aqui es pq ya tenemos encriptador válido.

	//Desencriptamos!.
	m4char_t * pLicenseSummary = NULL;	
	m4int_t iLicenseSummary = 0;
	if (oClEncryption.Decrypt( ai_pLicenseSummaryEncrypted, ai_SizeLicenseSummaryEncrypted, pLicenseSummary, iLicenseSummary ) == M4_ERROR) {
		return M4_ERROR;
	}

	//Si no ha habido error, tenemos que deserializar el buffer.
	//TamañoEtiqueta1-Etiqueta1-TamañoValor1-Valor1-...-TamañoEtiquetaN-EtiquetaN-TamañoValorN-ValorN

	m4int32_t iCurrent = 0;
	m4int32_t iSizeLabelNetworkToHost = 0;
	m4int32_t iSizeLabel = ntohl(iSizeLabelNetworkToHost);
	m4pchar_t pLabel = NULL;
	m4int32_t iSizeValueNetworkToHost = 0;	
	m4int32_t iSizeValue = ntohl(iSizeValueNetworkToHost);
	m4pchar_t pValue = NULL;	
	while (iCurrent < iLicenseSummary)
	{
		//Extraemos la etiqueta.
		memcpy(&iSizeLabelNetworkToHost, pLicenseSummary + iCurrent, sizeof(iSizeLabelNetworkToHost));
		iSizeLabel = ntohl(iSizeLabelNetworkToHost);
		iCurrent += sizeof(m4int32_t);
		pLabel = new m4char_t [iSizeLabel + 1];
		if (pLabel == NULL) {
			delete [] pLicenseSummary;
			ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
			return M4_ERROR;
		}
		memset(pLabel, NULL, iSizeLabel + 1);
		memcpy(pLabel, pLicenseSummary + iCurrent, iSizeLabel);
		iCurrent += iSizeLabel;

		//Extraemos el valor.
		memcpy(&iSizeValueNetworkToHost, pLicenseSummary + iCurrent, sizeof(iSizeValueNetworkToHost));
		iSizeValue = ntohl(iSizeValueNetworkToHost);
		iCurrent += sizeof(m4int32_t);
		pValue = new m4char_t [iSizeValue + 1];
		if (pValue == NULL) {
			delete [] pLicenseSummary;
			delete [] pLabel;
			ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
			return M4_ERROR;
		}
		memset(pValue, NULL, iSizeValue + 1);
		memcpy(pValue, pLicenseSummary + iCurrent, iSizeValue);
		iCurrent += iSizeValue;

		//Ya tenemos el tamaño y valor de la etiqueta.
		string sId(pLabel);
		string sValue(pValue);
		ClPropertyValue * pClPropertyValue = new ClPropertyValue(sValue);
		m_oMpClPropertyId.insert( MpClPropertyItem::value_type(sId, pClPropertyValue));

		delete [] pLabel;
		pLabel = NULL;
		delete [] pValue;
		pValue = NULL;
	}

	delete [] pLicenseSummary;
	//---- FIN Encriptacion ------

	return M4_SUCCESS;
}

void ClModKeyProperties::_FillPropertyIdTraslation( void )
{
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_DUMPED_TIME, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_DUMPED_TIME)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_ID, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_ID)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_VERSION, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_VERSION)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_VERSION_GEN, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_VERSION_GEN)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_HOSTNAME, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_HOSTNAME)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_GENERATION_DATE, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_GENERATION_DATE)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_EXPIRATION, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_EXPIRATION)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_ID, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_ID)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_NAME, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_NAME)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_COUNTRY, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_COUNTRY)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_GEODIV, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_GEODIV)));
	m_oMpPropertyTrans.insert( MpClPropertyItem::value_type(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_DESC, new ClPropertyValue(M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_DESC)));
}

m4pcchar_t ClModKeyProperties::GetPropertyTransById( m4pcchar_t ai_pPropertyId )
{
	//Buscamos en el mapa y devolvemos la etiqueta.
	string sPropertyId(ai_pPropertyId);
	itMpClPropertyItem it = m_oMpPropertyTrans.find(sPropertyId);
	if (it != m_oMpPropertyTrans.end()) {
		//Si lo encontramos.
		ClPropertyValue * pClPropertyValue = (*it).second;
		if (pClPropertyValue != NULL) {
			return pClPropertyValue->GetPropertyValue();
		}
	}
	return NULL;
}

m4pcchar_t ClModKeyProperties::GetPropertyValueById( m4pcchar_t ai_pPropertyId )
{
	//Buscamos en el mapa y devolvemos el valor.
	string sPropertyId(ai_pPropertyId);
	itMpClPropertyItem it = m_oMpClPropertyId.find(sPropertyId);
	if (it != m_oMpClPropertyId.end()) {
		//Si lo encontramos.
		ClPropertyValue * pClPropertyValue = (*it).second;
		if (pClPropertyValue != NULL) {
			return pClPropertyValue->GetPropertyValue();
		}
	}
	return NULL;
}

m4return_t ClModKeyProperties::DumpSummary( m4pchar_t ai_pFileName )
{
	//Si no indicamos nombre lo volcamos a salida estandar.

	FILE * pFileOut;
	m4bool_t bStdOut = M4_FALSE;

	if (ai_pFileName != NULL) {
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		pFileOut = M4Fopen(ai_pFileName, M4UniWrite, eEncoding);
		if (pFileOut == 0) {
			return M4_ERROR;
		}
	}
	else {
		pFileOut= stdout;
		bStdOut = M4_TRUE;
	}
	
	//Volcamos la informacion de las propiedades en el fichero de salida.	
	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_DUMPED_TIME.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_DUMPED_TIME.c_str()));
	
	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_ID.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_ID.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_VERSION.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_VERSION.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_VERSION_GEN.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_VERSION_GEN.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_HOSTNAME.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_HOSTNAME.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_GENERATION_DATE.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_GENERATION_DATE.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_EXPIRATION.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_EXPIRATION.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_ID.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_ID.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_NAME.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_NAME.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_COUNTRY.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_COUNTRY.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_GEODIV.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_GEODIV.c_str()));

	fprintf(pFileOut, "%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_CUSTOMER_DESC.c_str());
	fprintf(pFileOut, "%s\n", GetPropertyValueById(M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_DESC.c_str()));

	// Se vuelcan también los NPs
	m4uint32_t	i = 0 ;
	m4pcchar_t	pccValue = NULL ;
	m4char_t	acTemp[ 128 ] ;
	string		sProperty ;

	while( 1 )
	{
		sprintf( acTemp, "%03d", i + 1 ) ;

		sProperty = M4MODKEY_PROPERTY_ID_LICENSE_NP_DESC + acTemp ;
		pccValue = GetPropertyValueById( sProperty.c_str() ) ;

		if( pccValue == NULL )
		{
			break ;
		}

		fprintf(pFileOut, "\n%s %s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_NP.c_str(), acTemp);
		fprintf(pFileOut, "%s\n", pccValue);

		sProperty = M4MODKEY_PROPERTY_ID_LICENSE_NP_SELECT + acTemp ;
		pccValue = GetPropertyValueById( sProperty.c_str() ) ;

		if( pccValue == NULL )
		{
			break ;
		}

		fprintf(pFileOut, "\t%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_NP_SELECT.c_str());
		fprintf(pFileOut, "%s\n", pccValue);

		sProperty = M4MODKEY_PROPERTY_ID_LICENSE_NP_VALUE + acTemp ;
		pccValue = GetPropertyValueById( sProperty.c_str() ) ;

		if( pccValue == NULL )
		{
			break ;
		}

		fprintf(pFileOut, "\t%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_NP_VALUE.c_str());
		fprintf(pFileOut, "%s\n", pccValue);

		sProperty = M4MODKEY_PROPERTY_ID_LICENSE_NP_AUTHORIZED + acTemp ;
		pccValue = GetPropertyValueById( sProperty.c_str() ) ;

		if( pccValue == NULL )
		{
			break ;
		}

		fprintf(pFileOut, "\t%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_NP_AUTHORIZED.c_str());
		fprintf(pFileOut, "%s\n", pccValue);

		sProperty = M4MODKEY_PROPERTY_ID_LICENSE_NP_LIMIT + acTemp ;
		pccValue = GetPropertyValueById( sProperty.c_str() ) ;

		if( pccValue == NULL )
		{
			break ;
		}

		fprintf(pFileOut, "\t%s:\t", M4MODKEY_PROPERTY_TRANS_LICENSE_NP_LIMIT.c_str());
		fprintf(pFileOut, "%s\n", pccValue);

		i++ ;
	}

	fflush(pFileOut);

	if (bStdOut != M4_TRUE) {
		fclose(pFileOut);
	}

	return M4_SUCCESS;
}

//------------------------------------------------
// FIN Clase ClModKeyProperties.
//------------------------------------------------

