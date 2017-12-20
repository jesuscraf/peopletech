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

//Orden de dependencias:
//m4cryptclib, m4tsapdll, m4win2ux, m4xmlutil, m4uuidlib, m4modkey, m4oeinit

#ifndef	_M4MODKEY_LICENSE_HPP_
#define _M4MODKEY_LICENSE_HPP_

#include "m4stl.hpp"
#include "globdec.hpp"

enum eSecurizedType_t {
	SECURIZED_UNDEFINED = 0,
	SECURIZED_NOT_ALLOWED,
	SECURIZED_ALLOWED	
};

//-------------------------------------------------------
class ClSecurizedModule;

//Vamos a tener vector los modulos que pueden estar cifrados.
typedef vector<ClSecurizedModule *>				VcClSecurizedModule;
typedef VcClSecurizedModule *					PVcClSecurizedModule;
typedef VcClSecurizedModule::iterator			itVcClSecurizedModule;
//-------------------------------------------------------

//-------------------------------------------------------
class ClSecurizedObject;

//Vamos a tener vector los items a cifrar.
typedef vector<ClSecurizedObject *>				VcClSecurizedObject;
typedef VcClSecurizedObject *					PVcClSecurizedObject;
typedef VcClSecurizedObject::iterator			itVcClSecurizedObject;
typedef VcClSecurizedObject::reverse_iterator	itReverseVcClSecurizedObject;

typedef set<ClSecurizedObject, less <ClSecurizedObject> >		SClSecurizedObject;
typedef SClSecurizedObject *									PSClSecurizedObject;
typedef SClSecurizedObject::iterator							itSClSecurizedObject;

typedef map<ClSecurizedObject,eSecurizedType_t,less<ClSecurizedObject> >	MpClSecurizedObjectCache;
typedef MpClSecurizedObjectCache *											PMpClSecurizedObjectCache;
typedef MpClSecurizedObjectCache::iterator									itMpClSecurizedObjectCache;
//-------------------------------------------------------

//-------------------------------------------------------
//Tendremos una lista de items Nps.
class ClNpItem;

typedef vector<ClNpItem *>		VcClNpItem;
typedef VcClNpItem *			PVcClNpItem;
typedef VcClNpItem::iterator	itVcClNpItem;
//-------------------------------------------------------

//-------------------------------------------------------
//Tendremos una lista de licencias.
class ClLicense;

typedef vector<ClLicense *>		VcClLicense;
typedef VcClLicense *			PVcClLicense;
typedef VcClLicense::iterator	itVcClLicense;
//-------------------------------------------------------

//----------------------------------------------
//Clase ClSecurizedModule
//----------------------------------------------
//Cómo identificar un item que se va a encriptar.
class M4_DECL_M4MODKEY ClSecurizedModule {

private:
	string m_sIdModule;
	string m_sDescription;
	VcClSecurizedObject * m_pVcSecurizedObjects;

public:
	ClSecurizedModule( void );
	ClSecurizedModule( m4pcchar_t ai_pIdModule, m4pcchar_t ai_pDescription );
	ClSecurizedModule( const string & ai_sIdModule, const string & ai_sDescription );
	~ClSecurizedModule( );

	m4bool_t IsEqualById ( const ClSecurizedModule & ai_oCopy ) const;

	const string & GetIdModule( void ) const;
	const string & GetDescription( void ) const;

	m4return_t AddSecurizedObject( ClSecurizedObject * ai_pClSecurizedObject );

	m4int32_t GetNumSecurizedObjects ( void ) const;
	const ClSecurizedObject * GetSecurizedObjectByIndex ( m4int32_t ai_iIndex ) const;
	m4bool_t IsSecurizedObject( const ClSecurizedObject & ai_oClSecurizedObject );

	void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClSecurizedModule
//----------------------------------------------

//----------------------------------------------
//Clase ClSecurizedObject
//----------------------------------------------
//Cómo identificar un item que se va a encriptar.
class M4_DECL_M4MODKEY ClSecurizedObject {

private:
	ClSecurizedModule * m_pModule;
	string m_sIdNodeStructure;
	string m_sIdItem;
	
public:
	ClSecurizedObject( void );
	ClSecurizedObject( ClSecurizedModule * ai_pModule, m4pcchar_t ai_pIdNodeStructure, m4pcchar_t ai_pIdItem );
	ClSecurizedObject( ClSecurizedModule * ai_pModule, const string & ai_sIdNodeStructure, const string & ai_sIdItem );
	~ClSecurizedObject( );

	//Constructor copia.
	ClSecurizedObject( const ClSecurizedObject & ai_oCopy );

	//Operador asignacion.
	ClSecurizedObject & operator = ( const ClSecurizedObject & ai_oCopy );

	//Operadores <, ==
	friend m4bool_t operator < (const ClSecurizedObject & ai_oLeft, const ClSecurizedObject & ai_oRight);

	friend m4bool_t operator == (const ClSecurizedObject & ai_oLeft, const ClSecurizedObject & ai_oRight);

	m4bool_t IsEqualById ( const ClSecurizedObject & ai_oCopy ) const;
	
	string GetIdSecurizedObject( void ) const;

	const string & GetIdNodeStructure( void ) const;
	const string & GetIdItem( void ) const;
	const ClSecurizedModule * GetModule( void ) const;

	void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClSecurizedObject
//----------------------------------------------

//----------------------------------------------
//Clase ClGenerationInfo
//----------------------------------------------
class M4_DECL_M4MODKEY ClGenerationInfo {

private:
	ClLicense * m_pClLicense;

	string m_sIdLicense;
	string m_sGenerationVersion;
	string m_sHostName;
	string m_sGenerationDate;

public:
	ClGenerationInfo( void );	
	ClGenerationInfo( ClLicense * ai_pClLicense );
	ClGenerationInfo( ClLicense * ai_pClLicense, const string & ai_sIdLicense, const string & ai_sGenerationVersion, const string & ai_sHostName, const string & ai_sGenerationDate );

	m4bool_t ValidateGenerationInfo( void );

	ClLicense * GetpLicense( void ) const;	
	const string & GetIdLicense( void ) const;
	const string & GetGenerationVersion( void ) const;	
	const string & GetHostName( void ) const;
	const string & GetGenerationDate( void ) const;

	static const string CalculateIdLicense( void );	
	static const string CalculateHostName( void );
	static const string CalculateGenerationDate( void );

	void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClGenerationInfo
//----------------------------------------------

//----------------------------------------------
//Clase ClCustomerInfo
//----------------------------------------------
class M4_DECL_M4MODKEY ClCustomerInfo {

private:
	string m_sIdCustomer;
	string m_sCustomerName;
	string m_sCustomerCountry;
	string m_sCustomerGeodiv;
	string m_sDescription;

public:
	ClCustomerInfo( void );
	ClCustomerInfo( const string & ai_sIdCustomer, const string & ai_sCustomerName, const string & ai_sCustomerCountry, const string & ai_sCustomerGeodiv, const string & ai_sDescription );

	const string & GetIdCustomer( void ) const;
	const string & GetCustomerName( void ) const;
	const string & GetCustomerCountry( void ) const;
	const string & GetCustomerGeodiv( void ) const;
	const string & GetDescription( void ) const;

	void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClCustomerInfo
//----------------------------------------------

//----------------------------------------------
//Clase ClNpItem
//----------------------------------------------
class M4_DECL_M4MODKEY ClNpItem {

private:
	string m_sDescription;
	string m_sSelect;
	string m_sAuthorized;
	string m_sEffectiveAuthorized;
	string m_sLimit;	
	string m_sValue;	

public:
	ClNpItem( void );
	ClNpItem( const string & ai_sDescription, const string & ai_sSelect, const string & ai_sAuthorized, const string & ai_sEffectiveAuthorized, const string & ai_sLimit );

	const string & GetDescription ( void) const;
	const string & GetSelect ( void) const;
	const string & GetAuthorized( void ) const;
	const string & GetEffectiveAuthorized( void ) const;
	const string & GetLimit( void ) const;	
	const string & GetValue( void ) const;	
	void SetValue( const string & ai_sValue );

	m4bool_t ValidateNpItem( void ) const;

	void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClNpItem
//----------------------------------------------

//----------------------------------------------
//Clase ClNp
//----------------------------------------------
class M4_DECL_M4MODKEY ClNp {

private:
	VcClNpItem * m_pVcClNpItem;

public:
	ClNp( void );
	~ClNp( );

	void AddNpItem( ClNpItem * ai_pClNpItem );

	m4int32_t GetNumNpItem ( void ) const;
	const ClNpItem * GetNpItemByIndex ( m4int32_t ai_iIndex ) const;
	m4bool_t ValidateNp( void ) const;

	void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClNp
//----------------------------------------------

//----------------------------------------------
//Clase ClLicense
//----------------------------------------------
class ClXmlAnalyzer;
class ClEncryption;
class ClModKeyProperties;
class M4_DECL_M4MODKEY ClLicense {

	//Hacemos amigos para que puedan hacer set.
	friend class ClXmlAnalyzerLicense;

private:
	static m4return_t _InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey );

private:
	string m_sLicenseVersion;
	string m_sExpiration;

	ClGenerationInfo * m_pGenerationInfo;
	ClCustomerInfo * m_pCustomerInfo;
	ClNp * m_pNp;

	VcClSecurizedModule * m_pVcSecurizedNotAllowed;
	VcClSecurizedModule * m_pVcSecurizedAllowed;

	MpClSecurizedObjectCache * m_pmClSecurizedObjectCache;	

protected:
	void _SetLicenseVersion( const string & ai_sLicenseVersion);
	void _SetExpiration( const string & ai_sExpiration);
	void _SetGenerationInfo( ClGenerationInfo * ai_pGenerationInfo );
	void _SetCustomerInfo( ClCustomerInfo * ai_pCustomerInfo );
	void _SetNp( ClNp * ai_pNp );

	m4return_t _AddSecurizedModule( eSecurizedType_t ai_eSecurizedType, ClSecurizedModule * ai_pClSecurizedModule );

	m4bool_t _ValidateLicenseVersion( void ) const;
	m4bool_t _ValidateLicenseExpiration( void ) const;	

public:
	ClLicense( void );
	~ClLicense();

	const string & GetLicenseVersion( void ) const;	
	const string & GetExpiration( void ) const;
	const ClGenerationInfo * GetGenerationInfo( void ) const;
	const ClCustomerInfo * GetCustomerInfo( void ) const;
	const ClNp * GetNp( void ) const;

	m4pcchar_t GetIdLicense( void ) const;

	static m4bool_t IsEqualByRegularExpression ( const string & ai_oRegularExpression, const string & ai_oString );

	m4bool_t ValidateLicense( void) const;
	m4bool_t ValidateLicenseExpiration( void ) const;
	
	m4return_t CreatePropertiesByLicense( ClModKeyProperties * & ao_pClProperties ) const;
	m4return_t GetLicenseSummaryEncrypted( m4pchar_t & ao_pLicenseSummaryEncrypted, m4int32_t & ao_iSizeLicenseSummaryEncrypted ) const;
	static m4return_t CopyAndReallocMemory( m4pchar_t & ao_pSource, m4int32_t & ao_iMaxSizeSource, m4int32_t & ao_iSizeSource, const m4char_t * const ai_pBufferToCopy, m4int32_t ai_iSizeToCopy );

	//-- POR MODULOS --
	//Podemos preguntar por los modulos o si un determinado modulo esta en listas.
	m4int32_t GetNumSecurizedModulesNotAllowed ( void ) const;
	m4int32_t GetNumSecurizedModulesAllowed ( void ) const;
	const ClSecurizedModule * GetSecurizedModuleNotAllowedByIndex ( m4int32_t ai_iIndex ) const;
	const ClSecurizedModule * GetSecurizedModuleAllowedByIndex ( m4int32_t ai_iIndex ) const;	

	m4bool_t IsSecurizedModuleNotAllowed( const ClSecurizedModule & ai_oClSecurizedModule ) const;
	m4bool_t IsSecurizedModuleAllowed( const ClSecurizedModule & ai_oClSecurizedModule ) const;	
	m4bool_t IsSecurizedModuleInList( eSecurizedType_t ai_eSecurizedType, const ClSecurizedModule & ai_oClSecurizedModule ) const;

	//-- POR OBJETOS --
	//Podemos preguntar por los objetos o si un determinado objeto esta en listas
	m4int32_t GetNumSecurizedObjectsNotAllowed ( void ) const;
	const ClSecurizedObject * GetSecurizedObjectNotAllowedByIndex ( m4int32_t ai_iIndex ) const;
	m4int32_t GetNumSecurizedObjectsAllowed ( void ) const;
	const ClSecurizedObject * GetSecurizedObjectAllowedByIndex ( m4int32_t ai_iIndex ) const;

	m4bool_t IsSecurizedObjectNotAllowed( const ClSecurizedObject & ai_oClSecurizedObject ) const;
	m4bool_t IsSecurizedObjectAllowed( const ClSecurizedObject & ai_oClSecurizedObject ) const;	
	m4bool_t IsSecurizedObjectInList( eSecurizedType_t ai_eSecurizedType, const ClSecurizedObject & ai_oClSecurizedObject ) const;

	void DumpInfo( FILE * ai_pFile, m4int32_t ai_iLevel );
};
//----------------------------------------------
//FIN Clase ClLicense
//----------------------------------------------

#endif


