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


#ifndef	_M4MODKEY_LICENSEPROPERTIES_HPP_
#define _M4MODKEY_LICENSEPROPERTIES_HPP_

#include "m4stl.hpp"
#include "globdec.hpp"


//Id.
const string M4MODKEY_PROPERTY_ID_LICENSE_DUMPED_TIME			("LICENSE_DUMPED_TIME");
const string M4MODKEY_PROPERTY_ID_LICENSE_ID					("LICENSE_ID");
const string M4MODKEY_PROPERTY_ID_LICENSE_VERSION				("LICENSE_VERSION");
const string M4MODKEY_PROPERTY_ID_LICENSE_VERSION_GEN			("LICENSE_VERSION_GEN");
const string M4MODKEY_PROPERTY_ID_LICENSE_HOSTNAME				("LICENSE_HOSTNAME");
const string M4MODKEY_PROPERTY_ID_LICENSE_GENERATION_DATE		("LICENSE_GENERATION_DATE");
const string M4MODKEY_PROPERTY_ID_LICENSE_EXPIRATION			("LICENSE_EXPIRATION");
const string M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_ID			("LICENSE_CUSTOMER_ID");
const string M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_NAME			("LICENSE_CUSTOMER_NAME");
const string M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_COUNTRY		("LICENSE_CUSTOMER_COUNTRY");
const string M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_GEODIV		("LICENSE_CUSTOMER_GEODIV");
const string M4MODKEY_PROPERTY_ID_LICENSE_CUSTOMER_DESC			("LICENSE_CUSTOMER_DESC");
const string M4MODKEY_PROPERTY_ID_LICENSE_NP_DESC				("LICENSE_NP_DESC_");
const string M4MODKEY_PROPERTY_ID_LICENSE_NP_SELECT				("LICENSE_NP_SELECT_");
const string M4MODKEY_PROPERTY_ID_LICENSE_NP_VALUE				("LICENSE_NP_VALUE_");
const string M4MODKEY_PROPERTY_ID_LICENSE_NP_AUTHORIZED			("LICENSE_NP_AUTHORIZED_");
const string M4MODKEY_PROPERTY_ID_LICENSE_NP_LIMIT				("LICENSE_NP_LIMIT_");


class ClPropertyValue;

//En el mapa tendremos el identificador y el valor.
typedef map<string,ClPropertyValue *,less<string> >		MpClPropertyItem;
typedef MpClPropertyItem *								PMpClPropertyItem;
typedef MpClPropertyItem::iterator						itMpClPropertyItem;

//------------------------------------------------
// Clase ClPropertyValue.
//------------------------------------------------
class ClPropertyValue {	

private:
	string m_sPropertyValue;

public:
	ClPropertyValue( const string & ai_sPropertyValue );

	//Constructor copia.
	ClPropertyValue( const ClPropertyValue & ai_oCopy );

	//Operador asignacion.
	ClPropertyValue & operator = ( const ClPropertyValue & ai_oCopy );

	//Operadores <, ==
	friend m4bool_t operator < (const ClPropertyValue & ai_oLeft, const ClPropertyValue & ai_oRight);

	friend m4bool_t operator == (const ClPropertyValue & ai_oLeft, const ClPropertyValue & ai_oRight);

	m4pcchar_t GetPropertyValue( void );
};
//------------------------------------------------
// FIN Clase ClPropertyValue.
//------------------------------------------------

//------------------------------------------------
// Clase ClModKeyProperties.
//------------------------------------------------
class ClEncryption;
class M4_DECL_M4MODKEY ClModKeyProperties  {

private:
	static m4return_t _InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey );

private:
	MpClPropertyItem m_oMpPropertyTrans;
	MpClPropertyItem m_oMpClPropertyId;

	void _FillPropertyIdTraslation( void );
	
public:
	ClModKeyProperties( void );
	~ClModKeyProperties( );

	m4return_t Init( m4pcchar_t ai_pLicenseSummaryEncrypted, m4int32_t ai_SizeLicenseSummaryEncrypted );
	m4pcchar_t GetPropertyTransById( m4pcchar_t ai_pPropertyId );
	m4pcchar_t GetPropertyValueById( m4pcchar_t ai_pPropertyId );	
	m4return_t DumpSummary( m4pchar_t ai_pFileName );
};
//------------------------------------------------
// FIN Clase ClModKeyProperties.
//------------------------------------------------

#endif


