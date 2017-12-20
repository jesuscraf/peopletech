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

#include "license.hpp"
#include "version.hpp"
#include "error.hpp"

#include "licenseproperties.hpp"

//Para calcular la fecha de ahora
#include "m4date.hpp"

//Para obtener el identificador unico en la generacion de la licencia.
#include "m4uuid.hpp"

//Para inicializar el sistema que obtiene el nombre de la maquina.
//Para obtener el nombre de la maquina.
#include "tsap.hpp"
#include "m4win2ux.hpp"

//Para la encriptacion del resumen.
#include <encrypt.hpp>
#include <m4cryptc.hpp>
#include "m4cryptcobj.hpp"

//Para las claves de encriptacion.
#include "key.hpp"

#ifndef _WINDOWS
	#include <netinet/in.h>
#endif

#define CHECK_COPYANDREALLOCMEMORY(X) {if (X == M4_ERROR) { delete [] pSerialized; return M4_ERROR; }}
//----------------------------------------------
//Clase ClSecurizedModule
//----------------------------------------------
ClSecurizedModule::ClSecurizedModule( void )
	: m_sIdModule(M4MODKEY_EMPTY), m_sDescription(M4MODKEY_EMPTY)
{
	m_pVcSecurizedObjects = new VcClSecurizedObject;
}

ClSecurizedModule::ClSecurizedModule( m4pcchar_t ai_pIdModule, m4pcchar_t ai_pDescription )
{
	if (ai_pIdModule != NULL) {
		m_sIdModule = ai_pIdModule;
	}
	else {
		m_sIdModule = M4MODKEY_EMPTY;
	}
	if (ai_pDescription != NULL) {
		m_sDescription = ai_pDescription;
	}
	else {
		m_sDescription = M4MODKEY_EMPTY;
	}

	m_pVcSecurizedObjects = new VcClSecurizedObject;
}

ClSecurizedModule::ClSecurizedModule( const string & ai_sIdModule, const string & ai_sDescription )
{
	m_sIdModule = ai_sIdModule;
	m_sDescription = ai_sDescription;

	m_pVcSecurizedObjects = new VcClSecurizedObject;
}

ClSecurizedModule::~ClSecurizedModule( )
{
	if (m_pVcSecurizedObjects != NULL) {
		for (itVcClSecurizedObject it = m_pVcSecurizedObjects->begin(); it != m_pVcSecurizedObjects->end(); it++) {			
			ClSecurizedObject * pSecurizedObject = (*it);			
			if (pSecurizedObject != NULL) {
				delete pSecurizedObject;
			}
		}	
		delete m_pVcSecurizedObjects;
		m_pVcSecurizedObjects = NULL;
	}
}

m4bool_t ClSecurizedModule::IsEqualById ( const ClSecurizedModule & ai_oCopy ) const
{
	//Vamos a comparar por expresion regular.
	
	//--------------------------------------------------------------
	if (ClLicense::IsEqualByRegularExpression(m_sIdModule, ai_oCopy.m_sIdModule) == M4_TRUE) {
		return M4_TRUE;
	}
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	//Si no ha cumplido en un sentido, vamos a ver en el otro sentido.
	if (ClLicense::IsEqualByRegularExpression(ai_oCopy.m_sIdModule, m_sIdModule) == M4_TRUE) {
	}
	//--------------------------------------------------------------

	return M4_FALSE;
}

const string & ClSecurizedModule::GetIdModule( void ) const
{
	return m_sIdModule;
}

const string & ClSecurizedModule::GetDescription( void ) const
{
	return m_sDescription;
}

m4return_t ClSecurizedModule::AddSecurizedObject( ClSecurizedObject * ai_pClSecurizedObject )
{
	if ((m_pVcSecurizedObjects != NULL) && (ai_pClSecurizedObject != NULL)){
		m_pVcSecurizedObjects->push_back(ai_pClSecurizedObject);
		return M4_SUCCESS;
	}
	return M4_ERROR;	
}

m4int32_t ClSecurizedModule::GetNumSecurizedObjects ( void ) const
{
	if (m_pVcSecurizedObjects != NULL) {
		return m_pVcSecurizedObjects->size();
	}
	return 0;
}

const ClSecurizedObject * ClSecurizedModule::GetSecurizedObjectByIndex ( m4int32_t ai_iIndex ) const
{
	if ((ai_iIndex >= 0) && (ai_iIndex < GetNumSecurizedObjects())) {
		return (*m_pVcSecurizedObjects)[ai_iIndex];
	}
	return NULL;
}

m4bool_t ClSecurizedModule::IsSecurizedObject( const ClSecurizedObject & ai_oClSecurizedObject )
{
	//Buscamos un determinado el objeto De esta forma podremos saber a qué módulo pertenece.
	if (m_pVcSecurizedObjects != NULL) {
		for (itVcClSecurizedObject it = m_pVcSecurizedObjects->begin(); it != m_pVcSecurizedObjects->end(); it++) {			
			const ClSecurizedObject * pSecurizedObject = (*it);			
			if (pSecurizedObject != NULL) {				
				if (pSecurizedObject->IsEqualById(ai_oClSecurizedObject) == M4_TRUE) {
					return M4_TRUE;
				}
			}
		}	
	}
	return M4_FALSE;
}

void ClSecurizedModule::DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}

	string sTab(M4MODKEY_EMPTY);	
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		sTab.append("\t");
	}

	fprintf(ai_pFile, "%s *** Begin - ClSecurizedModule ***\n", sTab.c_str());
	fprintf(ai_pFile, "%s\tm_sIdModule: %s\n", sTab.c_str(), m_sIdModule.c_str());
	if (m_pVcSecurizedObjects != NULL) {
		for (itVcClSecurizedObject it = m_pVcSecurizedObjects->begin(); it != m_pVcSecurizedObjects->end(); it++) {			
			ClSecurizedObject * pSecurizedObject = (*it);
			pSecurizedObject->DumpInfo(ai_pFile, ai_iLevel + 1);			
		}	
	}
	fprintf(ai_pFile, "%s *** End - ClSecurizedModule ***\n", sTab.c_str());

	fflush(ai_pFile);
}

//----------------------------------------------
//FIN Clase ClSecurizedObject
//----------------------------------------------

//----------------------------------------------
//Clase ClSecurizedObject
//----------------------------------------------
ClSecurizedObject::ClSecurizedObject( void )
	: m_sIdNodeStructure(M4MODKEY_EMPTY), m_sIdItem(M4MODKEY_EMPTY)
{
	m_pModule = NULL;
}

ClSecurizedObject::ClSecurizedObject( ClSecurizedModule * ai_pModule, m4pcchar_t ai_pIdNodeStructure, m4pcchar_t ai_pIdItem )
{
	if (ai_pIdNodeStructure != NULL) {
		m_sIdNodeStructure = ai_pIdNodeStructure;
	}
	else {
		m_sIdNodeStructure = M4MODKEY_EMPTY;
	}

	if (ai_pIdItem != NULL) {
		m_sIdItem = ai_pIdItem;
	}
	else {
		m_sIdItem = M4MODKEY_EMPTY;
	}

	m_pModule = ai_pModule;
}

ClSecurizedObject::ClSecurizedObject( ClSecurizedModule * ai_pModule, const string & ai_sIdNodeStructure, const string & ai_sIdItem )
{	
	m_sIdNodeStructure = ai_sIdNodeStructure;
	m_sIdItem = ai_sIdItem;	
	m_pModule = ai_pModule;
}

ClSecurizedObject::ClSecurizedObject( const ClSecurizedObject & ai_oCopy )
{
	m_sIdNodeStructure = ai_oCopy.m_sIdNodeStructure;
	m_sIdItem = ai_oCopy.m_sIdItem;
	m_pModule = ai_oCopy.m_pModule;
}

ClSecurizedObject::~ClSecurizedObject( )
{
	//No elimina el modulo al que pertenece, solo desligamos el objeto al modulo!.
	m_pModule = NULL;
}

ClSecurizedObject & ClSecurizedObject::operator = ( const ClSecurizedObject & ai_oCopy )
{
	m_sIdNodeStructure = ai_oCopy.m_sIdNodeStructure;
	m_sIdItem = ai_oCopy.m_sIdItem;
	m_pModule = ai_oCopy.m_pModule;
	return *this;
}

m4bool_t operator < ( const ClSecurizedObject & ai_oLeft, const ClSecurizedObject & ai_oRight )
{
	if (ai_oLeft.m_sIdNodeStructure < ai_oRight.m_sIdNodeStructure){
		return M4_TRUE;
	}
	if (ai_oLeft.m_sIdNodeStructure != ai_oRight.m_sIdNodeStructure){
		return M4_FALSE;
	}
	if (ai_oLeft.m_sIdItem < ai_oRight.m_sIdItem){
		return M4_TRUE;
	}
	if (ai_oLeft.m_sIdItem != ai_oRight.m_sIdItem){
		return M4_FALSE;
	}
	return (M4_FALSE);
}

m4bool_t operator == ( const ClSecurizedObject & ai_oLeft, const ClSecurizedObject & ai_oRight )
{
	if (ai_oLeft.m_sIdNodeStructure != ai_oRight.m_sIdNodeStructure){
		return M4_FALSE;
	}
	if (ai_oLeft.m_sIdItem != ai_oRight.m_sIdItem){
		return M4_FALSE;
	}
	return (M4_TRUE);
}

m4bool_t ClSecurizedObject::IsEqualById ( const ClSecurizedObject & ai_oCopy ) const
{
	//Vamos a comparar por expresion regular.
	
	//--------------------------------------------------------------
	if (ClLicense::IsEqualByRegularExpression(m_sIdNodeStructure, ai_oCopy.m_sIdNodeStructure) == M4_TRUE) {
		//La estructura de nodo cumple la expresion regular.
		//Veamos si la cumple el item.
		if (ClLicense::IsEqualByRegularExpression(m_sIdItem, ai_oCopy.m_sIdItem) == M4_TRUE) {
			return M4_TRUE;
		}
		if (ClLicense::IsEqualByRegularExpression(ai_oCopy.m_sIdItem, m_sIdItem) == M4_TRUE) {
			return M4_TRUE;
		}
	}
	//--------------------------------------------------------------

	//--------------------------------------------------------------
	//Si no ha cumplido en un sentido, vamos a ver en el otro sentido.
	if (ClLicense::IsEqualByRegularExpression(ai_oCopy.m_sIdNodeStructure, m_sIdNodeStructure) == M4_TRUE) {
		//Veamos si la cumple el item.
		if (ClLicense::IsEqualByRegularExpression(ai_oCopy.m_sIdItem, m_sIdItem) == M4_TRUE) {
			return M4_TRUE;
		}
		if (ClLicense::IsEqualByRegularExpression(m_sIdItem, ai_oCopy.m_sIdItem) == M4_TRUE) {
			return M4_TRUE;
		}
	}
	//--------------------------------------------------------------

	return M4_FALSE;
}

string ClSecurizedObject::GetIdSecurizedObject( void ) const
{
	//Esta funcion retorna una cadena de forma que pueda identificar
	//al objeto segurizable.

	string sIdSecurizedObject(m_sIdNodeStructure);
	sIdSecurizedObject.append("#");
	sIdSecurizedObject.append(m_sIdItem);

	return sIdSecurizedObject;
}

const string & ClSecurizedObject::GetIdNodeStructure( void ) const
{
	return m_sIdNodeStructure;
}

const string & ClSecurizedObject::GetIdItem( void ) const
{
	return m_sIdItem;
}

const ClSecurizedModule * ClSecurizedObject::GetModule( void ) const
{
	return m_pModule;
}

void ClSecurizedObject::DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}

	string sTab(M4MODKEY_EMPTY);
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		sTab.append("\t");
	}

	fprintf(ai_pFile, "%s *** Begin - ClSecurizedObject ***\n", sTab.c_str());
	
	if (m_pModule != NULL) {
		fprintf(ai_pFile, "%s\tm_pModule: %s\n", sTab.c_str(), m_pModule->GetIdModule().c_str());
	}
	fprintf(ai_pFile, "%s\tm_sIdNodeStructure: %s\n", sTab.c_str(), m_sIdNodeStructure.c_str());
	fprintf(ai_pFile, "%s\tm_sIdItem: %s\n", sTab.c_str(), m_sIdItem.c_str());

	fprintf(ai_pFile, "%s *** End - ClSecurizedObject ***\n", sTab.c_str());

	fflush(ai_pFile);
}

//----------------------------------------------
//FIN Clase ClSecurizedObject
//----------------------------------------------

//----------------------------------------------
//Clase ClGenerationInfo
//----------------------------------------------
ClGenerationInfo::ClGenerationInfo( void )
	:m_sIdLicense(M4MODKEY_EMPTY), m_sGenerationVersion(ClVersion::GetFileVersionLicense()), m_sHostName(M4MODKEY_EMPTY), m_sGenerationDate(M4MODKEY_EMPTY)
{
	m_pClLicense = NULL;
}

ClGenerationInfo::ClGenerationInfo( ClLicense * ai_pClLicense )
	:m_sIdLicense(M4MODKEY_EMPTY), m_sGenerationVersion(ClVersion::GetFileVersionLicense()), m_sHostName(M4MODKEY_EMPTY), m_sGenerationDate(M4MODKEY_EMPTY)
{
	m_pClLicense = ai_pClLicense;
}

ClGenerationInfo::ClGenerationInfo( ClLicense * ai_pClLicense, const string & ai_sIdLicense, const string & ai_sGenerationVersion, const string & ai_sHostName, const string & ai_sGenerationDate )
{
	m_pClLicense = ai_pClLicense;

	//Importante!:
	//Cuando leemos el fichero de informacion de licencia NO hay seccion de generacion, por lo tanto es necesario 
	//crear una. Los valores estarán vacios.
	//Cuando generamos la licencia siempre se llama a los métodos de calcular!.
	
	//--------------------------------------------------------------------
	//Si está vacio lo calculo.	
	if (strcmpi(ai_sGenerationVersion.c_str(), M4MODKEY_EMPTY) == 0) {
		m_sGenerationVersion = ClVersion::GetFileVersionLicense();
	}
	else {
		m_sGenerationVersion = ai_sGenerationVersion;
	}
	
	//Si está vacio lo calculo.	
	if (strcmpi(ai_sIdLicense.c_str(), M4MODKEY_EMPTY) == 0) {
		m_sIdLicense = CalculateIdLicense();
	}
	else {
		m_sIdLicense = ai_sIdLicense;
	}
	
	//Si está vacio lo calculo.	
	if (strcmpi(ai_sHostName.c_str(), M4MODKEY_EMPTY) == 0) {
		m_sHostName = CalculateHostName();
	}
	else {
		m_sHostName = ai_sHostName;
	}

	//Si está vacia calculo la fecha.	
	if (strcmpi(ai_sGenerationDate.c_str(), M4MODKEY_EMPTY) == 0) {
		m_sGenerationDate = CalculateGenerationDate();
	}
	else {
		m_sGenerationDate = ai_sGenerationDate;
	}
	//--------------------------------------------------------------------
}

const string ClGenerationInfo::CalculateIdLicense( void )
{
	string sIdLicense(M4MODKEY_EMPTY);

	//Al generar la licencia calculamos un uid unico. Es el tamaño + el nulo.
	m4uint32_t iIdLicenseSize = M4CL_VM_SIZE_UUID + 1;
	m4char_t * pIdLicense = new m4char_t [iIdLicenseSize];
	if (pIdLicense == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4MODKEY_EMPTY;
	}
	if (GetUUID(pIdLicense, iIdLicenseSize) == M4_ERROR) {
		delete [] pIdLicense;
		return M4MODKEY_EMPTY;
		
	}			

	sIdLicense.append(pIdLicense);

	//Eliminamos el buffer del id de la licencia.
	delete [] pIdLicense;

	return sIdLicense;
}

const string ClGenerationInfo::CalculateHostName( void )
{
	string sHostName(M4MODKEY_EMPTY);
	//Tamaño mas el nulo.
	m4char_t pHostName[M4MODKEY_SIZE_DEFAULT_STRING + 1];		
	
	SocketInit();

	//En el tamaño del buffer incluimos el nulo final.
	if ((M4_gethostname(pHostName, M4MODKEY_SIZE_DEFAULT_STRING + 1) == M4_ERROR) || (pHostName == NULL)) {
		strcpy(pHostName, M4MODKEY_EMPTY);
	}
	sHostName.append(pHostName);

	return sHostName;
}

const string ClGenerationInfo::CalculateGenerationDate( void )
{
	//La calculamos en un formato yyyy-mm-dd hh:mm:ss
	m4char_t pNow[M4_SECOND_END + 1];
	string sNow(M4MODKEY_EMPTY);

	ClActualDate (pNow, M4_SECOND_END + 1);
	sNow.append(pNow);

	return sNow;
}

m4bool_t ClGenerationInfo::ValidateGenerationInfo( void )
{
	//Veamos qué informacion es candidata a ser verificada...

	//---------------------------------------------------------------------------------
	//Valida si la version con que se genero la licencia es correcta todavía.
	if (ClVersion::ValidateFileVersionLicense(m_sGenerationVersion) == M4_TRUE) {		
		return M4_TRUE;
	}
	//---------------------------------------------------------------------------------

	//No validamos nada más.

	//Si no hemos encontrado una correcta nos vamos sin validar.
	return M4_FALSE;
}

ClLicense * ClGenerationInfo::GetpLicense( void ) const
{
	return m_pClLicense;
}

const string & ClGenerationInfo::GetIdLicense( void ) const
{
	return m_sIdLicense;
}

const string & ClGenerationInfo::GetGenerationVersion( void ) const
{
	return m_sGenerationVersion;
}

const string & ClGenerationInfo::GetGenerationDate( void ) const
{
	return m_sGenerationDate;
}

const string & ClGenerationInfo::GetHostName( void ) const
{
	return m_sHostName;
}

void ClGenerationInfo::DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}

	string sTab(M4MODKEY_EMPTY);
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		sTab.append("\t");
	}

	fprintf(ai_pFile, "%s *** Begin - ClGenerationInfo ***\n", sTab.c_str());

	fprintf(ai_pFile, "%s\tm_sIdLicense: %s\n",sTab.c_str(), m_sIdLicense.c_str());
	fprintf(ai_pFile, "%s\tm_sGenerationVersion: %s\n",sTab.c_str(), m_sGenerationVersion.c_str());
	fprintf(ai_pFile, "%s\tm_sHostName: %s\n",sTab.c_str(), m_sHostName.c_str());
	fprintf(ai_pFile, "%s\tm_sGenerationDate: %s\n", sTab.c_str(), m_sGenerationDate.c_str());
	fprintf(ai_pFile, "%s *** End - ClGenerationInfo ***\n", sTab.c_str());

	fflush(ai_pFile);
}
//----------------------------------------------
//FIN Clase ClGenerationInfo
//----------------------------------------------

//----------------------------------------------
//Clase ClCustomerInfo
//----------------------------------------------
ClCustomerInfo::ClCustomerInfo( void )
	: m_sIdCustomer(M4MODKEY_EMPTY), m_sCustomerName(M4MODKEY_EMPTY), m_sCustomerCountry(M4MODKEY_EMPTY), m_sCustomerGeodiv(M4MODKEY_EMPTY), m_sDescription(M4MODKEY_EMPTY)
{}

ClCustomerInfo::ClCustomerInfo( const string & ai_sIdCustomer, const string & ai_sCustomerName, const string & ai_sCustomerCountry, const string & ai_sCustomerGeodiv, const string & ai_sDescription )
{
	m_sIdCustomer = ai_sIdCustomer;
	m_sCustomerName = ai_sCustomerName;
	m_sCustomerCountry = ai_sCustomerCountry;
	m_sCustomerGeodiv = ai_sCustomerGeodiv;
	m_sDescription = ai_sDescription;
}

const string & ClCustomerInfo::GetIdCustomer( void ) const
{
	return m_sIdCustomer;
}

const string & ClCustomerInfo::GetCustomerName( void ) const
{
	return m_sCustomerName;
}

const string & ClCustomerInfo::GetCustomerCountry( void ) const
{
	return m_sCustomerCountry;
}

const string & ClCustomerInfo::GetCustomerGeodiv( void ) const
{
	return m_sCustomerGeodiv;
}

const string & ClCustomerInfo::GetDescription( void ) const
{
	return m_sDescription;
}

void ClCustomerInfo::DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}

	string sTab(M4MODKEY_EMPTY);
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		sTab.append("\t");
	}

	fprintf(ai_pFile, "%s *** Begin - ClCustomerInfo ***\n", sTab.c_str());

	fprintf(ai_pFile, "%s\tm_sIdCustomer: %s\n", sTab.c_str(), m_sIdCustomer.c_str());
	fprintf(ai_pFile, "%s\tm_sCustomerName: %s\n", sTab.c_str(), m_sCustomerName.c_str());
	fprintf(ai_pFile, "%s\tm_sCustomerCountry: %s\n", sTab.c_str(), m_sCustomerCountry.c_str());
	fprintf(ai_pFile, "%s\tm_sCustomerGeodiv: %s\n", sTab.c_str(), m_sCustomerGeodiv.c_str());
	fprintf(ai_pFile, "%s\tm_sDescription: %s\n", sTab.c_str(), m_sDescription.c_str());

	fprintf(ai_pFile, "%s *** End - ClCustomerInfo ***\n", sTab.c_str());

	fflush(ai_pFile);
}
//----------------------------------------------
//FIN Clase ClCustomerInfo
//----------------------------------------------

//----------------------------------------------
//Clase ClNpItem
//----------------------------------------------
ClNpItem::ClNpItem( void )
	:m_sDescription(M4MODKEY_EMPTY), m_sSelect(M4MODKEY_EMPTY), m_sAuthorized(M4MODKEY_EMPTY), m_sEffectiveAuthorized(M4MODKEY_EMPTY), m_sLimit(M4MODKEY_EMPTY)
{
	m_sValue = "0" ;
}

ClNpItem::ClNpItem( const string & ai_sDescription, const string & ai_sSelect, const string & ai_sAuthorized, const string & ai_sEffectiveAuthorized, const string & ai_sLimit )
{
	m_sDescription = ai_sDescription;
	m_sSelect = ai_sSelect;
	m_sAuthorized = ai_sAuthorized;
	m_sEffectiveAuthorized = ai_sEffectiveAuthorized;
	m_sLimit = ai_sLimit;	
	m_sValue = "0" ;
}

const string & ClNpItem::GetDescription ( void) const
{
	return m_sDescription;
}

const string & ClNpItem::GetSelect ( void) const
{
	return m_sSelect;
}

const string & ClNpItem::GetAuthorized( void ) const
{
	return m_sAuthorized;
}

const string & ClNpItem::GetEffectiveAuthorized( void ) const
{
	return m_sEffectiveAuthorized;
}

const string & ClNpItem::GetLimit( void ) const
{
	return m_sLimit;
}

const string & ClNpItem::GetValue( void ) const
{
	return m_sValue;
}

void ClNpItem::SetValue( const string & ai_sValue )
{
	m_sValue = ai_sValue;
}

m4bool_t ClNpItem::ValidateNpItem( void ) const
{
	//Convertimos a numeros los valores y comparamos.
	m4int32_t iAuthorized = 0;
	m4int32_t iEffectiveAuthorized = 0;
	m4int32_t iLimit = 0;

	if (m_sAuthorized.size() > 0) {
		iAuthorized = atoi(m_sAuthorized.c_str());
	}
	if (m_sEffectiveAuthorized.size() > 0) {
		iEffectiveAuthorized = atoi(m_sEffectiveAuthorized.c_str());
	}
	if (m_sLimit.size() > 0) {
		iLimit = atoi(m_sLimit.c_str());
	}

	//Veamos los casos.
	if (iAuthorized > iEffectiveAuthorized) {
		return M4_FALSE;
	}
	if (iEffectiveAuthorized > iLimit) {
		return M4_FALSE;
	}

	return M4_TRUE;
}

void ClNpItem::DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}

	string sTab(M4MODKEY_EMPTY);
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		sTab.append("\t");
	}

	fprintf(ai_pFile, "%s *** Begin - ClNpItem ***\n", sTab.c_str());

	fprintf(ai_pFile, "%s\tm_sSelect: %s\n", sTab.c_str(), m_sSelect.c_str());
	fprintf(ai_pFile, "%s\tm_sAuthorized: %s\n", sTab.c_str(), m_sAuthorized.c_str());
	fprintf(ai_pFile, "%s\tm_sEffectiveAuthorized: %s\n", sTab.c_str(), m_sEffectiveAuthorized.c_str());
	fprintf(ai_pFile, "%s\tm_sLimit: %s\n", sTab.c_str(), m_sLimit.c_str());	

	fprintf(ai_pFile, "%s *** End - ClNpItem ***\n", sTab.c_str());

	fflush(ai_pFile);
}
//----------------------------------------------
//FIN Clase ClNpItem
//----------------------------------------------

//----------------------------------------------
//Clase ClNp
//----------------------------------------------
ClNp::ClNp( void )
{	
	m_pVcClNpItem = new VcClNpItem;
}

ClNp::~ClNp( )
{
	if (m_pVcClNpItem != NULL) {
		for (itVcClNpItem it = m_pVcClNpItem->begin(); it != m_pVcClNpItem->end(); it++)
		{
			delete (*it);
		}
		delete m_pVcClNpItem;
		m_pVcClNpItem = NULL;
	}
}

void ClNp::AddNpItem( ClNpItem * ai_pClNpItem )
{
	if (m_pVcClNpItem != NULL) {
		m_pVcClNpItem->push_back( ai_pClNpItem );
	}
}

m4int32_t ClNp::GetNumNpItem ( void ) const
{
	if (m_pVcClNpItem != NULL) {
		return m_pVcClNpItem->size();
	}
	return 0;
}

const ClNpItem * ClNp::GetNpItemByIndex ( m4int32_t ai_iIndex ) const
{
	if ((ai_iIndex >= 0) && (ai_iIndex < GetNumNpItem())) {
		return (*m_pVcClNpItem)[ai_iIndex];
	}
	return NULL;
}

m4bool_t ClNp::ValidateNp( void ) const
{
	if (m_pVcClNpItem != NULL) {
		for (itVcClNpItem it = m_pVcClNpItem->begin(); it != m_pVcClNpItem->end(); it++)
		{
			ClNpItem * pNpItem = (*it);
			if (pNpItem != NULL) {
				if (pNpItem->ValidateNpItem() == M4_FALSE) {
					//Si hay uno que no cumple nos vamos.
					return M4_FALSE;
				}
			}
		}
	}
	return M4_TRUE;
}

void ClNp::DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (m_pVcClNpItem != NULL) {
		for (itVcClNpItem it = m_pVcClNpItem->begin(); it != m_pVcClNpItem->end(); it++)
		{
			(*it)->DumpInfo(ai_pFile, ai_iLevel);
		}
	}
}

//----------------------------------------------
//FIN Clase ClNp
//----------------------------------------------

//----------------------------------------------
//Clase ClLicense
//----------------------------------------------

	//Escenario:

	//Listas de la licencia.
	//1.- Lista de objetos antiguos sobre los que el cliente no tiene permiso. Esto nos permitirá
	// poder abortar la ejecución de un item securizado antiguo que el cliente no debería poder utilizar.
	// En realidad se podría poner elementos nuevos que no tenemos permiso.

	//2.- Lista de items nuevos que el cliente ha adquirido. En M4 se encriptan estos items y
	// en el traspaso es donde se comprueba si deben desencriptarse. Si el item está en la lista
	// el cliente deberá tenerlo desencriptado.
	// También podrían haber items viejos con permiso!.

	//----
	//Importante:
	// 1.- Tenemos la lista de objetos securizables, cuando salen de M4 esos objetos se encriptan en el mdb que va a cliente.
	// 2.- Cuando se aplica en el cliente el mdb, si el item está en la lista de con permiso se desencriptan.
	// 3.- En runtime, cuando utilicemos un item comprobaremos si está en la lista de sin permiso, si está,
	// entonces daremos un error, independientemente si está encriptado o no (pe un objeto viejo securizable 
	// que ha sufrido actualización vendrá encriptado y ademas puede ser que el cliente no tenga permiso. Si el objeto
	// no ha sufrido cambio, en el mdb no viene pero debemos prohibir su utilizacion pq no tiene permiso el cliente.
	//----

	//En realidad se trata de lista de modulos. La relación modulo con los items del módulo
	//están en el xml de objetos.

	//El cliente tendrá en su repositorio tanto la lista de objetos-modulo como la de 
	//modulos adquiridos por el cliente.

ClLicense::ClLicense( void )
	:m_sLicenseVersion(M4MODKEY_EMPTY), m_sExpiration(M4MODKEY_EMPTY)
{	
	m_pGenerationInfo = NULL;
	m_pCustomerInfo = NULL;
	m_pNp = NULL;

	m_pVcSecurizedNotAllowed = new VcClSecurizedModule;
	m_pVcSecurizedAllowed = new VcClSecurizedModule;
	m_pmClSecurizedObjectCache = new MpClSecurizedObjectCache;
}

ClLicense::~ClLicense()
{
	if (m_pGenerationInfo != NULL) {
		delete m_pGenerationInfo;
		m_pGenerationInfo = NULL;
	}

	if (m_pCustomerInfo != NULL) {
		delete m_pCustomerInfo;
		m_pCustomerInfo = NULL;
	}

	if (m_pNp != NULL) {
		delete m_pNp;
		m_pNp = NULL;
	}

	if (m_pVcSecurizedNotAllowed != NULL) {
		for (itVcClSecurizedModule it = m_pVcSecurizedNotAllowed->begin(); it != m_pVcSecurizedNotAllowed->end(); it++) {
			delete *it;
		}
		delete m_pVcSecurizedNotAllowed;
		m_pVcSecurizedNotAllowed = NULL;
	}

	if (m_pVcSecurizedAllowed != NULL) {
		for (itVcClSecurizedModule it = m_pVcSecurizedAllowed->begin(); it != m_pVcSecurizedAllowed->end(); it++) {
			delete *it;
		}
		delete m_pVcSecurizedAllowed;
		m_pVcSecurizedAllowed = NULL;
	}

	if (m_pmClSecurizedObjectCache != NULL) {		
		m_pmClSecurizedObjectCache->clear();
		delete m_pmClSecurizedObjectCache;
	}
}

m4return_t ClLicense::_InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey )
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

void ClLicense::_SetLicenseVersion( const string & ai_sLicenseVersion)
{
	m_sLicenseVersion = ai_sLicenseVersion;
}

void ClLicense::_SetExpiration( const string & ai_sExpiration )
{
	m_sExpiration = ai_sExpiration;
}

void ClLicense::_SetGenerationInfo( ClGenerationInfo * ai_pGenerationInfo )
{
	m_pGenerationInfo = ai_pGenerationInfo;
}

void ClLicense::_SetCustomerInfo( ClCustomerInfo * ai_pCustomerInfo )
{
	m_pCustomerInfo = ai_pCustomerInfo;
}

void ClLicense::_SetNp( ClNp * ai_pNp )
{
	m_pNp = ai_pNp;
}

m4return_t ClLicense::_AddSecurizedModule( eSecurizedType_t ai_eSecurizedType, ClSecurizedModule * ai_pClSecurizedModule )
{
	switch (ai_eSecurizedType)
	{
	case SECURIZED_NOT_ALLOWED:
		if (m_pVcSecurizedNotAllowed == NULL) {
			return M4_ERROR;			
		}
		m_pVcSecurizedNotAllowed->push_back(ai_pClSecurizedModule);

		break;

	case SECURIZED_ALLOWED:
		if (m_pVcSecurizedAllowed == NULL) {
			return M4_ERROR;			
		}
		m_pVcSecurizedAllowed->push_back(ai_pClSecurizedModule);

		break;

	default:
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4bool_t ClLicense::_ValidateLicenseVersion( void ) const
{
	if (strcmpi(m_sLicenseVersion.c_str(), M4MODKEY_EMPTY) == 0) {
		//La version es necesaria.
		return M4_FALSE;
	}

	//---------------------------------------------------------------------------------
	//Versiones validas.
	if (strcmpi(m_sLicenseVersion.c_str(), M4MODKEY_LICENSEVERSION_VERSION_1) == 0) {		
		return M4_TRUE;
	}

	if (strcmpi(m_sLicenseVersion.c_str(), M4MODKEY_LICENSEVERSION_VERSION_2) == 0) {
		return M4_TRUE;;
	}
	//---------------------------------------------------------------------------------

	//Si no hemos encontrado una correcta nos vamos sin validar.
	return M4_FALSE;
}

m4bool_t ClLicense::_ValidateLicenseExpiration( void ) const
{
	//----------------------------------
	//Validamos que no haya expirado.
	if (strcmpi(m_sExpiration.c_str(), M4MODKEY_EMPTY) != 0) {
		//Si no esta vacia...
		m4date_t dDate = ClStringToDate(m_sExpiration.c_str());	

		//---------------------------------------------------------------------
		//Antes vamos a comprobar si efectivamente se trata de una fecha valida,
		//es decir, no me ponen el 35 de abril.
		//Convirtiendo otra vez a cadena y comparando.
		m4int32_t iRecalculteSize = m_sExpiration.size();
		m4pchar_t pRecalculateSize = new m4char_t [iRecalculteSize + 1];
		ClDateToString(dDate, pRecalculateSize, iRecalculteSize + 1);		
		if (strncmp(m_sExpiration.c_str(), pRecalculateSize, iRecalculteSize) != 0) {
			//No son iguales.
			delete [] pRecalculateSize;
			return M4_FALSE;
		}
		delete [] pRecalculateSize;

		//---------------------------------------------------------------------
		m4date_t dNow;
		ClActualDate (dNow);
		if (dDate > dNow) {
			return M4_TRUE;			
		}
		return M4_FALSE;
	}
	else {
		//No hemos puesto fecha de expiracion.
		return M4_TRUE;
	}
	//----------------------------------

	return M4_FALSE;	
}

const string & ClLicense::GetLicenseVersion( void ) const	
{
	return m_sLicenseVersion;
}

const string & ClLicense::GetExpiration( void ) const
{
	return m_sExpiration;
}

const ClGenerationInfo * ClLicense::GetGenerationInfo( void ) const
{
	return m_pGenerationInfo;
}

const ClCustomerInfo * ClLicense::GetCustomerInfo( void ) const
{
	return m_pCustomerInfo;
}

const ClNp * ClLicense::GetNp( void ) const
{
	return m_pNp;
}

m4pcchar_t ClLicense::GetIdLicense( void ) const
{
	if (m_pGenerationInfo != NULL) {
		if (m_pGenerationInfo->GetIdLicense().size() > 0) {
			return m_pGenerationInfo->GetIdLicense().c_str();
		}
	}
	return NULL;
}

m4bool_t ClLicense::IsEqualByRegularExpression ( const string & ai_oRegularExpression, const string & ai_oString )
{
	//Directamente comprobamos si son iguales.
	if (ai_oRegularExpression == ai_oString) {
		return M4_TRUE;
	}

	//Suponemos que el primer argumento es el que tiene la expresion regular.
	//Asterisco (*): N caracteres.
	//Interrogante (?): 1 caracter.
	m4int32_t iRegCurrent = 0 ;
	m4int32_t iStrCurrent = 0;
	m4int32_t iRegSize = ai_oRegularExpression.size();
	m4int32_t iStrSize = ai_oString.size();

	m4char_t cReg;
	m4bool_t bFound = M4_FALSE;

	//Mientras hay vida hay esperanza...
	while (1)
	{
		//Si hemos llegado al final de una cadena, tenemos que haber llegado tambien al final de la otra!.
		if (iRegCurrent >= iRegSize) {
			if (iStrCurrent < iStrSize) {
				return M4_FALSE;
			}
			else {
				return M4_TRUE;
			}			
		}

		cReg = ai_oRegularExpression[iRegCurrent];

		switch (cReg)
		{
		case '*':
			//Si encuentra un asterisco, nos situamos en el siguiente y buscamos el primer caracter que no sea asterisco.
			iRegCurrent = ai_oRegularExpression.find_first_not_of("*", iRegCurrent + 1);

			if (iRegCurrent == -1) {
				//Despues del asterisco no hay mas caracteres.
				//Esto significa que si hasta ahora iban coincidiendo... definitivamente coinciden pq cualquier cadena
				//cumple con asterisco.
				return M4_TRUE;
			}

			//Si estamos aqui es pq hay un caracter, veamos cual es.
			cReg = ai_oRegularExpression[iRegCurrent];

			//Ojo, no estamos soportando el caso de interrogantes despues de asterisco!.

			//Nos saltamos todos los caracteres de la segunda cadena hasta encontrar el caracter.			
			bFound = M4_FALSE;
			while (iStrCurrent < iStrSize)			
			{
				//Si el caracter es el que buscabamos entonces vamos bien.
				if (cReg == ai_oString[iStrCurrent]) {
					//Hemos encontrado el caracter que buscabamos. Salimos del while
					bFound = M4_TRUE;
					break;
				}
				iStrCurrent++;
			}

			if (bFound == M4_FALSE) {
				//No se encontro el caracter que buscabamos, por lo tanto nos vamos.
				return M4_FALSE;
			}

			//Se encontro el caracter, continuamos comprobando con el siguiente caracter.
			iRegCurrent ++;
			iStrCurrent++;

			continue;

		case '?':
			//Nos situamos en el siguiente caracter.			
			iRegCurrent++;			
			//Obligatoriamente en la cadena debe haber un caracter que nos lo podemos saltar.			
			iStrCurrent++;

			continue;

		default:
			//Cualquier otro caracter.
			if (ai_oRegularExpression[iRegCurrent] != ai_oString[iStrCurrent]) {
				return M4_FALSE;
			}
			iRegCurrent++;			
			iStrCurrent++;
		}
	}

	//Si estoy aqui es pq no son iguales!.
	return M4_FALSE;
}

m4bool_t ClLicense::ValidateLicense( void) const
{
	m4bool_t bValidateLicense = M4_FALSE;

	//----------------------------------
	//Validamos que no haya expirado.	
	if ((bValidateLicense = _ValidateLicenseExpiration()) == M4_FALSE) {
		return M4_FALSE;
	}
	//----------------------------------

	//----------------------------------
	//Validamos la version de la licencia.	
	if ((bValidateLicense = _ValidateLicenseVersion()) == M4_FALSE) {
		return M4_FALSE;
	}
	//----------------------------------

	//----------------------------------
	//Validamos miembros.
	if (m_pGenerationInfo != NULL) {
		if ((bValidateLicense = m_pGenerationInfo->ValidateGenerationInfo()) == M4_FALSE) {
			return M4_FALSE;
		}
	}
	//----------------------------------

	//----------------------------------
	//Validamos miembros.
	if (m_pNp != NULL) {
		if ((bValidateLicense = m_pNp->ValidateNp()) == M4_FALSE) {
			return M4_FALSE;
		}
	}
	//----------------------------------

	return bValidateLicense;
}

m4bool_t ClLicense::ValidateLicenseExpiration( void) const
{
	m4bool_t bValidateLicense = M4_FALSE;

	//----------------------------------
	//Validamos que no haya expirado.	
	if ((bValidateLicense = _ValidateLicenseExpiration()) == M4_FALSE) {
		return M4_FALSE;
	}
	//----------------------------------

	return bValidateLicense;
}

m4return_t ClLicense::CreatePropertiesByLicense( ClModKeyProperties * & ao_pClProperties ) const
{
	//Obtenemos el resumen de la licencia.
	ao_pClProperties = NULL;	

	m4pchar_t pSummaryEncrypted = NULL;
	m4int32_t iSizeSummaryEncrypted = 0;
	if (GetLicenseSummaryEncrypted(pSummaryEncrypted, iSizeSummaryEncrypted) == M4_ERROR) {
		return M4_ERROR;
	}

	//Si estamos aqui es pq hemos obtenido correctamente el resumen.
	ClModKeyProperties * pClProperties = new ClModKeyProperties();
	if (pClProperties->Init(pSummaryEncrypted, iSizeSummaryEncrypted) == M4_ERROR) {
		delete pClProperties;
		delete pSummaryEncrypted;
		return M4_ERROR;
	}

	//Ya podemos eliminarlo.
	delete [] pSummaryEncrypted;

	//Se ha inicializado correctamente.
	ao_pClProperties = pClProperties;
	return M4_SUCCESS;
}

m4return_t ClLicense::GetLicenseSummaryEncrypted( m4pchar_t & ao_pLicenseSummaryEncrypted, m4int32_t & ao_iSizeLicenseSummaryEncrypted ) const
{	
	//Importante:
	//Retorna un buffer donde se ha serializado y despues encriptado el resumen de licencia.
	//Es resposabilidad del invocante liberar el buffer!!!.

	//Debe estar vacia!.
	if (ao_pLicenseSummaryEncrypted != NULL) {
		return M4_ERROR;
	}

	ao_pLicenseSummaryEncrypted = NULL;
	ao_iSizeLicenseSummaryEncrypted = 0;

	//Damos un tamaño inicial y ademas esta vacio.
	m4int32_t iSerializedMaxSize = 1024;
	m4int32_t iSerializeSize = 0;	
	m4int32_t iSizeItem = 0;
	m4int32_t iSizeItemHostToNetwork = htonl(iSizeItem);
	m4char_t * pSerialized = new m4char_t [iSerializedMaxSize + 1];	
	if (pSerialized == NULL) {
		ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
		return M4_ERROR;
	}
	memset(pSerialized, NULL, iSerializedMaxSize + 1);

	//-------------------------------------------------
	//Vamos a construir el buffer con informacion de resumen de licencia.
	//TamañoEtiqueta1-Etiqueta1-TamañoValor1-Valor1-...-TamañoEtiquetaN-EtiquetaN-TamañoValorN-ValorN
	//-------------------------------------------------

	//-----------------------
	iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_DUMPED_TIME.size();
	iSizeItemHostToNetwork = htonl(iSizeItem);
	CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
	CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_DUMPED_TIME.c_str(), iSizeItem));	
	iSizeItem = M4_SECOND_END + 1;
	m4char_t pNow[M4_SECOND_END + 1];
	ClActualDate (pNow, iSizeItem + 1);	
	iSizeItemHostToNetwork = htonl(iSizeItem);
	CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
	CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, pNow, iSizeItem));
	//-----------------------

	if (m_pGenerationInfo != NULL) {
		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_ID.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_ID.c_str(), iSizeItem));
		iSizeItem = m_pGenerationInfo->GetIdLicense().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pGenerationInfo->GetIdLicense().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_VERSION.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_VERSION.c_str(), iSizeItem));
		iSizeItem = GetLicenseVersion().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, GetLicenseVersion().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_VERSION_GEN.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_VERSION_GEN.c_str(), iSizeItem));
		iSizeItem = m_pGenerationInfo->GetGenerationVersion().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pGenerationInfo->GetGenerationVersion().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_HOSTNAME.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_HOSTNAME.c_str(), iSizeItem));
		iSizeItem = m_pGenerationInfo->GetHostName().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pGenerationInfo->GetHostName().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_GENERATION_DATE.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_GENERATION_DATE.c_str(), iSizeItem));
		iSizeItem = m_pGenerationInfo->GetGenerationDate().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pGenerationInfo->GetGenerationDate().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_EXPIRATION.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_EXPIRATION.c_str(), iSizeItem));
		iSizeItem = GetExpiration().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, GetExpiration().c_str(), iSizeItem));
		//-----------------------------------------------
	}

	if (m_pCustomerInfo != NULL) {				
		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_ID.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_ID.c_str(), iSizeItem));
		iSizeItem = m_pCustomerInfo->GetIdCustomer().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pCustomerInfo->GetIdCustomer().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_NAME.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_NAME.c_str(), iSizeItem));
		iSizeItem = m_pCustomerInfo->GetCustomerName().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pCustomerInfo->GetCustomerName().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_COUNTRY.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_COUNTRY.c_str(), iSizeItem));
		iSizeItem = m_pCustomerInfo->GetCustomerCountry().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pCustomerInfo->GetCustomerCountry().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_GEODIV.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_GEODIV.c_str(), iSizeItem));
		iSizeItem = m_pCustomerInfo->GetCustomerGeodiv().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pCustomerInfo->GetCustomerGeodiv().c_str(), iSizeItem));
		//-----------------------------------------------

		//-----------------------------------------------
		iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_DESC.size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_DESC.c_str(), iSizeItem));
		iSizeItem = m_pCustomerInfo->GetDescription().size();
		iSizeItemHostToNetwork = htonl(iSizeItem);
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
		CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, m_pCustomerInfo->GetDescription().c_str(), iSizeItem));
		//-----------------------------------------------
	}

	// Se vuelcan también los NPs
	if( m_pNp != NULL )
	{
		m4int32_t	i = 0 ;
		m4int32_t	iLength = m_pNp->GetNumNpItem() ;
		m4char_t	acTemp[ 128 ] ;

		for( i = 0 ; i < iLength ; i++ )
		{
			const ClNpItem*	poNpItem = m_pNp->GetNpItemByIndex( i ) ;

			if( poNpItem != NULL )
			{
				sprintf( acTemp, "%03d", i + 1 ) ;

				//-----------------------------------------------
				iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_NP_DESC.size();
				iSizeItemHostToNetwork = htonl(iSizeItem + 3);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_NP_DESC.c_str(), iSizeItem));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, acTemp, 3));
				iSizeItem = poNpItem->GetDescription().size();
				iSizeItemHostToNetwork = htonl(iSizeItem);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, poNpItem->GetDescription().c_str(), iSizeItem));
				//-----------------------------------------------

				//-----------------------------------------------
				iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_NP_SELECT.size();
				iSizeItemHostToNetwork = htonl(iSizeItem + 3);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_NP_SELECT.c_str(), iSizeItem));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, acTemp, 3));
				iSizeItem = poNpItem->GetSelect().size();
				iSizeItemHostToNetwork = htonl(iSizeItem);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, poNpItem->GetSelect().c_str(), iSizeItem));
				//-----------------------------------------------

				//-----------------------------------------------
				iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_NP_VALUE.size();
				iSizeItemHostToNetwork = htonl(iSizeItem + 3);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_NP_VALUE.c_str(), iSizeItem));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, acTemp, 3));
				iSizeItem = poNpItem->GetValue().size();
				iSizeItemHostToNetwork = htonl(iSizeItem);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, poNpItem->GetValue().c_str(), iSizeItem));
				//-----------------------------------------------

				//-----------------------------------------------
				iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_NP_AUTHORIZED.size();
				iSizeItemHostToNetwork = htonl(iSizeItem + 3);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_NP_AUTHORIZED.c_str(), iSizeItem));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, acTemp, 3));
				iSizeItem = poNpItem->GetAuthorized().size();
				iSizeItemHostToNetwork = htonl(iSizeItem);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, poNpItem->GetAuthorized().c_str(), iSizeItem));
				//-----------------------------------------------

				//-----------------------------------------------
				iSizeItem = M4MODKEY_PROPERTY_ID_LICENSE_NP_LIMIT.size();
				iSizeItemHostToNetwork = htonl(iSizeItem + 3);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, M4MODKEY_PROPERTY_ID_LICENSE_NP_LIMIT.c_str(), iSizeItem));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, acTemp, 3));
				iSizeItem = poNpItem->GetLimit().size();
				iSizeItemHostToNetwork = htonl(iSizeItem);
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, (m4pchar_t) (&iSizeItemHostToNetwork), sizeof(iSizeItemHostToNetwork)));
				CHECK_COPYANDREALLOCMEMORY (CopyAndReallocMemory(pSerialized, iSerializedMaxSize, iSerializeSize, poNpItem->GetLimit().c_str(), iSizeItem));
				//-----------------------------------------------
			}
		}
	}


	ClEncryption oClEncryption;

	if (_InitEncrypter(&oClEncryption, M4MODKEY_ENCRYPT_KEY_SUMMARY) == M4_ERROR) {
		delete [] pSerialized;
		pSerialized = NULL;
		return M4_ERROR;
	}

	//Si estamos aqui es pq ya tenemos encriptador válido.

	//Encriptamos!.
	m4char_t * pLicenseSummary = NULL;	
	m4int_t iLicenseSummary = 0;
	if (oClEncryption.Encrypt( pSerialized, iSerializeSize, pLicenseSummary, iLicenseSummary ) == M4_ERROR) {
		delete [] pSerialized;
		pSerialized = NULL;
		return M4_ERROR;
	}
	//Ya no se utiliza.
	if (pSerialized != NULL) {
		delete [] pSerialized;
		pSerialized = NULL;
	}


	// Miriam.
	/*int iSerializeSize2 = 0;
	if (oClEncryption.Decrypt( pLicenseSummary, iLicenseSummary, pSerialized, iSerializeSize2 ) == M4_ERROR)
	{
		delete pSerialized;
		return M4_ERROR;
	}*/
	// Miriam

	//Copiamos en la cadena de salida.
	// ao_pLicenseSummaryEncrypted = pLicenseSummary;
	ao_pLicenseSummaryEncrypted = new m4char_t[iLicenseSummary + 1];
	memcpy(ao_pLicenseSummaryEncrypted, pLicenseSummary, iLicenseSummary + 1);
	ao_iSizeLicenseSummaryEncrypted = iLicenseSummary;

	delete pLicenseSummary;

	return M4_SUCCESS;
}

m4return_t ClLicense::CopyAndReallocMemory( m4pchar_t & ao_pSource, m4int32_t & ao_iMaxSizeSource, m4int32_t & ao_iSizeSource, const m4char_t * const ai_pBufferToCopy, m4int32_t ai_iSizeToCopy )
{
	//Si lo que vamos a copiar no cabe hay que realocar.
	m4pchar_t pTemp = NULL;
	if (ao_iMaxSizeSource < (ao_iSizeSource + ai_iSizeToCopy)) {
		ao_iMaxSizeSource = (ao_iSizeSource + ai_iSizeToCopy) * 2;
		pTemp = new m4char_t [ao_iMaxSizeSource + 1];
		if (pTemp == NULL) {
			ClErrorManager::DumpError( M4MODKEY_ERROR_INTERNAL_MEMORY );
			return M4_ERROR;
		}
		//Rellenamos de nulos.
		memset(pTemp, NULL, ao_iMaxSizeSource + 1);
		if (ao_pSource != NULL) {
			memcpy(pTemp, ao_pSource, ao_iSizeSource);			
			delete [] ao_pSource;			
		}
		ao_pSource = pTemp;
	}
	//Ya tenemos reallocado y suficiente tamaño para copiar.
	memcpy(ao_pSource + ao_iSizeSource, ai_pBufferToCopy, ai_iSizeToCopy);
	ao_iSizeSource += ai_iSizeToCopy;

	return M4_SUCCESS;
}

m4int32_t ClLicense::GetNumSecurizedModulesNotAllowed ( void ) const
{
	if (m_pVcSecurizedNotAllowed != NULL) {
		return m_pVcSecurizedNotAllowed->size();
	}
	return 0;
}

m4int32_t ClLicense::GetNumSecurizedModulesAllowed ( void ) const
{
	if (m_pVcSecurizedAllowed != NULL) {
		return m_pVcSecurizedAllowed->size();
	}
	return 0;
}

const ClSecurizedModule * ClLicense::GetSecurizedModuleNotAllowedByIndex ( m4int32_t ai_iIndex ) const
{
	if ((ai_iIndex >= 0) && (ai_iIndex < GetNumSecurizedModulesNotAllowed())) {
		return (*m_pVcSecurizedNotAllowed)[ai_iIndex];
	}
	return NULL;
}

const ClSecurizedModule * ClLicense::GetSecurizedModuleAllowedByIndex ( m4int32_t ai_iIndex ) const
{
	if ((ai_iIndex >= 0) && (ai_iIndex < GetNumSecurizedModulesAllowed())) {
		return (*m_pVcSecurizedAllowed)[ai_iIndex];
	}
	return NULL;
}

m4bool_t ClLicense::IsSecurizedModuleNotAllowed( const ClSecurizedModule & ai_oClSecurizedModule ) const
{
	return IsSecurizedModuleInList(SECURIZED_NOT_ALLOWED, ai_oClSecurizedModule);
}

m4bool_t ClLicense::IsSecurizedModuleAllowed( const ClSecurizedModule & ai_oClSecurizedModule ) const
{
	return IsSecurizedModuleInList(SECURIZED_ALLOWED, ai_oClSecurizedModule);
}

m4bool_t ClLicense::IsSecurizedModuleInList( eSecurizedType_t ai_eSecurizedType, const ClSecurizedModule & ai_oClSecurizedModule ) const
{
	//Comprobamos si el id de un modulo está en la lista que me indican.
	VcClSecurizedModule * pVcClSecurizedModule = NULL;

	switch (ai_eSecurizedType)
	{
	case SECURIZED_NOT_ALLOWED:
		if (m_pVcSecurizedNotAllowed == NULL) {
			return M4_FALSE;
		}
		pVcClSecurizedModule = m_pVcSecurizedNotAllowed;

		break;

	case SECURIZED_ALLOWED:
		if (m_pVcSecurizedAllowed == NULL) {
			return M4_FALSE;
		}
		pVcClSecurizedModule = m_pVcSecurizedAllowed;

		break;

	default:
		return M4_FALSE;
	}

	//El puntero temporal seguro que tiene valor.
	for (itVcClSecurizedModule it = pVcClSecurizedModule->begin(); it != pVcClSecurizedModule->end(); it++) {		
		const ClSecurizedModule * pClSecurizedModule = (*it);
		if (pClSecurizedModule != NULL) {
			if (pClSecurizedModule->IsEqualById(ai_oClSecurizedModule) == M4_TRUE) {
				return M4_TRUE;
			}
		}
	}

	return M4_FALSE;
}

m4int32_t ClLicense::GetNumSecurizedObjectsNotAllowed ( void ) const
{
	//Recorremos todos los modulos y contamos cuantos objetos hay.
	m4int32_t iResult = 0;

	m4int32_t iNumSecurizedModules = GetNumSecurizedModulesNotAllowed();
	for (m4int32_t i=0; i<iNumSecurizedModules; i++) {
		const ClSecurizedModule * pClSecurizedModule = GetSecurizedModuleNotAllowedByIndex(i);
		if (pClSecurizedModule != NULL) {
			iResult += pClSecurizedModule->GetNumSecurizedObjects();
		}
	}
	return iResult;
}

const ClSecurizedObject * ClLicense::GetSecurizedObjectNotAllowedByIndex ( m4int32_t ai_iIndex ) const
{
	m4int32_t iCurrent = 0;

	//Aplanamos.
	m4int32_t iNumSecurizedModules = GetNumSecurizedModulesNotAllowed();
	for (m4int32_t i=0; i<iNumSecurizedModules; i++) {
		const ClSecurizedModule * pClSecurizedModule = GetSecurizedModuleNotAllowedByIndex(i);
		if (pClSecurizedModule != NULL) {
			m4int32_t iNumSecurizedObjects = pClSecurizedModule->GetNumSecurizedObjects();			
			//Situamos el current al inicio de la ventana.
			iCurrent += iNumSecurizedObjects;
			//Si el indice que buscamos es menor es pq nos hemos pasado de la ventana.
			if (ai_iIndex < iCurrent) {
				//Restauramos current para situarnos en la ventana donde esta el indice
				//que buscamos.
				iCurrent -= iNumSecurizedObjects;
				//Ahora recorremos la ventana.
				for (m4int32_t j=0; j<iNumSecurizedObjects; j++) {					
					if (iCurrent == ai_iIndex) {
						//Encontrado.
						return pClSecurizedModule->GetSecurizedObjectByIndex(j);
					}
					//Movemos el current
					iCurrent ++;
				}
				//Si estamos aqui, mal rollo pq debería haberse encontrado en esta ventana.
				return NULL;
			}
		}
	}
	return NULL;
}

m4int32_t ClLicense::GetNumSecurizedObjectsAllowed ( void ) const
{
	//Recorremos todos los modulos y contamos cuantos objetos hay.
	m4int32_t iResult = 0;

	m4int32_t iNumSecurizedModules = GetNumSecurizedModulesAllowed();
	for (m4int32_t i=0; i<iNumSecurizedModules; i++) {
		const ClSecurizedModule * pClSecurizedModule = GetSecurizedModuleAllowedByIndex(i);
		if (pClSecurizedModule != NULL) {
			iResult += pClSecurizedModule->GetNumSecurizedObjects();
		}
	}
	return iResult;
}

const ClSecurizedObject * ClLicense::GetSecurizedObjectAllowedByIndex ( m4int32_t ai_iIndex ) const
{
	m4int32_t iCurrent = 0;

	//Aplanamos.
	m4int32_t iNumSecurizedModules = GetNumSecurizedModulesAllowed();
	for (m4int32_t i=0; i<iNumSecurizedModules; i++) {
		const ClSecurizedModule * pClSecurizedModule = GetSecurizedModuleAllowedByIndex(i);
		if (pClSecurizedModule != NULL) {
			m4int32_t iNumSecurizedObjects = pClSecurizedModule->GetNumSecurizedObjects();			
			//Situamos el current al inicio de la ventana.
			iCurrent += iNumSecurizedObjects;
			//Si el indice que buscamos es menor es pq nos hemos pasado de la ventana.
			if (ai_iIndex < iCurrent) {
				//Restauramos current para situarnos en la ventana donde esta el indice
				//que buscamos.
				iCurrent -= iNumSecurizedObjects;
				//Ahora recorremos la ventana.
				for (m4int32_t j=0; j<iNumSecurizedObjects; j++) {					
					if (iCurrent == ai_iIndex) {
						//Encontrado.
						return pClSecurizedModule->GetSecurizedObjectByIndex(j);
					}
					//Movemos el current
					iCurrent ++;
				}
				//Si estamos aqui, mal rollo pq debería haberse encontrado en esta ventana.
				return NULL;
			}
		}
	}
	return NULL;
}

m4bool_t ClLicense::IsSecurizedObjectNotAllowed( const ClSecurizedObject & ai_oClSecurizedObject ) const
{
	return IsSecurizedObjectInList(SECURIZED_NOT_ALLOWED, ai_oClSecurizedObject);
}

m4bool_t ClLicense::IsSecurizedObjectAllowed( const ClSecurizedObject & ai_oClSecurizedObject ) const
{
	return IsSecurizedObjectInList(SECURIZED_ALLOWED, ai_oClSecurizedObject);
}

m4bool_t ClLicense::IsSecurizedObjectInList( eSecurizedType_t ai_eSecurizedType, const ClSecurizedObject & ai_oClSecurizedObject ) const
{
	//Comprobamos si el id de un modulo está en la lista que me indican.
	VcClSecurizedModule * pVcClSecurizedModule = NULL;

	//--------------------------------------------------------------
	//Antes de nada, comprobamos si está en la cache!.
	itMpClSecurizedObjectCache itMpCache = m_pmClSecurizedObjectCache->find(ai_oClSecurizedObject);
	if (itMpCache != m_pmClSecurizedObjectCache->end()) {
		//Lo hemos encontrado en la cache, veamos a que lista está asociado.
		eSecurizedType_t sSecTp = (*itMpCache).second;		
		if (sSecTp == ai_eSecurizedType) {
			//Si coincide la lista es pq lo hemos encontrado en la lista que buscabamos.
			return M4_TRUE;
		}
		else {
			//No coincide en la lista, pero SI lo hemos encontrado en la cache.
			return M4_FALSE;
		}		
	}

	//----------------------------------
	//No esta en la cache, continuamos!.
	//Tendremos que insertarlo en la cache!!!
	//----------------------------------

	//--------------------------------------------------------------

	switch (ai_eSecurizedType)
	{
	case SECURIZED_NOT_ALLOWED:
		if (m_pVcSecurizedNotAllowed == NULL) {
			return M4_FALSE;
		}
		pVcClSecurizedModule = m_pVcSecurizedNotAllowed;

		break;

	case SECURIZED_ALLOWED:
		if (m_pVcSecurizedAllowed == NULL) {
			return M4_FALSE;
		}
		pVcClSecurizedModule = m_pVcSecurizedAllowed;

		break;

	default:
		return M4_FALSE;
	}

	//El puntero temporal seguro que tiene valor.
	for (itVcClSecurizedModule it = pVcClSecurizedModule->begin(); it != pVcClSecurizedModule->end(); it++) {		
		const ClSecurizedModule * pClSecurizedModule = (*it);
		if (pClSecurizedModule != NULL) {
			//Dado un modulo buscamos los objetos que estan dentro del modulo.
			m4int32_t iNumSecurizedObjects = pClSecurizedModule->GetNumSecurizedObjects();
			for (m4int32_t i=0; i<iNumSecurizedObjects; i++) {		
				const ClSecurizedObject * pClSecurizedObject = pClSecurizedModule->GetSecurizedObjectByIndex(i);
				if (pClSecurizedObject != NULL) {
					if (pClSecurizedObject->IsEqualById(ai_oClSecurizedObject) == M4_TRUE) {
						//Lo hemos encontrado en la lista que buscabamos, lo insertamos en la cache
						//asociandolo a la lista donde se ha encontrado.
						m_pmClSecurizedObjectCache->insert( MpClSecurizedObjectCache::value_type(ai_oClSecurizedObject, ai_eSecurizedType));
						return M4_TRUE;
					}
				}
			}
		}
	}

	return M4_FALSE;
}

void ClLicense::DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel )
{
	if (ai_pFile == NULL) {
		return;
	}

	string sTab(M4MODKEY_EMPTY);
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		sTab.append("\t");
	}

	fprintf(ai_pFile, "%s *** Begin - ClLicense ***\n", sTab.c_str());

	fprintf(ai_pFile, "%s\tm_sLicenseVersion: %s\n",sTab.c_str(), m_sLicenseVersion.c_str());
	fprintf(ai_pFile, "%s\tm_sExpiration: %s\n",sTab.c_str(), m_sExpiration.c_str());

	m4pchar_t pLicenseSummaryEncrypted = NULL;
	m4int32_t iLicenseSummaryEncrypted = 0;
	if (GetLicenseSummaryEncrypted(pLicenseSummaryEncrypted, iLicenseSummaryEncrypted) == M4_SUCCESS) {		
		fprintf(ai_pFile, "%s\tLicenseSummary encrypted Now: %s\n", sTab.c_str(), pLicenseSummaryEncrypted);
		delete [] pLicenseSummaryEncrypted;
	}

	if (m_pGenerationInfo != NULL) {
		m_pGenerationInfo->DumpInfo(ai_pFile, ai_iLevel + 1);
	}
	if (m_pCustomerInfo != NULL) {
		m_pCustomerInfo->DumpInfo(ai_pFile, ai_iLevel + 1);
	}
	if (m_pNp != NULL) {
		m_pNp->DumpInfo(ai_pFile, ai_iLevel + 1);
	}

	if (m_pVcSecurizedNotAllowed != NULL) {
		fprintf(ai_pFile, "%s * Begin securized modules without permission *\n", sTab.c_str());
		for (itVcClSecurizedModule it = m_pVcSecurizedNotAllowed->begin(); it != m_pVcSecurizedNotAllowed->end(); it++) {
			ClSecurizedModule  * pSecurizedModule = (*it);
			if (pSecurizedModule != NULL) {
				pSecurizedModule->DumpInfo(ai_pFile, ai_iLevel + 1);
			}
		}
		fprintf(ai_pFile, "%s * End securized modules without permission *\n", sTab.c_str());
	}

	if (m_pVcSecurizedAllowed != NULL) {
		fprintf(ai_pFile, "%s * Begin securized modules with permission *\n", sTab.c_str());
		for (itVcClSecurizedModule it = m_pVcSecurizedAllowed->begin(); it != m_pVcSecurizedAllowed->end(); it++) {
			ClSecurizedModule  * pSecurizedModule = (*it);
			if (pSecurizedModule != NULL) {
				pSecurizedModule->DumpInfo(ai_pFile, ai_iLevel + 1);
			}
		}
		fprintf(ai_pFile, "%s * End securized modules with permission *\n", sTab.c_str());
	}

	fprintf(ai_pFile, "%s *** End - ClLicense ***\n", sTab.c_str());

	fflush(ai_pFile);
}

//----------------------------------------------
//FIN Clase ClLicense
//----------------------------------------------
