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

#ifndef	_M4MODKEY_XMLCONTAINERS_HPP_
#define _M4MODKEY_XMLCONTAINERS_HPP_

#include "m4stl.hpp"
#include "globdec.hpp"
#include "xmltaglabel.hpp"


//----------------------------------------------
//Clase ClAttributeValue
//----------------------------------------------
class ClAttributeValue;

//Vamos a tener vector los items a cifrar.
typedef vector<ClAttributeValue *>			VcClAttributeValue;
typedef VcClAttributeValue *				PVcClAttributeValue;
typedef VcClAttributeValue::iterator		itVcClAttributeValue;

class ClAttributeValue {
	string m_sAttribute;
	string m_sValue;
public:
	ClAttributeValue ( void );
	ClAttributeValue (string ai_sAttribute, string ai_sValue );
	ClAttributeValue (string ai_sAttribute, m4pcchar_t ai_sValue );

	const string * GetAttribute( void ) const;
	const string * GetValue( void ) const;
};
//----------------------------------------------
//FIN Clase ClAttributeValue
//----------------------------------------------

//----------------------------------------------
//Clase ClXmlTagContainer
//----------------------------------------------
class ClXmlTagContainer;

//Vamos a tener vector los items a cifrar.
typedef vector<ClXmlTagContainer *>			VcClXmlTagContainer;
typedef VcClXmlTagContainer *				PVcClXmlTagContainer;
typedef VcClXmlTagContainer::iterator		itVcClXmlTagContainer;

//Se trata de una clase abstracta de la que heredaran todas las clases que
//contendran elementos que se hayan parseado del xml.
class ClXmlTagContainer {
	//Tendremos el identificador del tag y una lista de atributo-valor
	string m_sTagId;
	PVcClAttributeValue m_pVcAttributeValue;

	//Un tag tiene un puntero a su tag padre.
	ClXmlTagContainer * m_pXmlTagContainerParent;

	//Un tag tambien tiene un lista de todos sus tag hijo!.
	PVcClXmlTagContainer	m_pVcXmlTagContainerChild;

public:
	ClXmlTagContainer( void );
	ClXmlTagContainer( string ai_sTagId, ClXmlTagContainer * ai_pXmlTagContainerParent );
	virtual ~ClXmlTagContainer( );

	virtual m4return_t AddAttributeValue( ClAttributeValue * ai_pAttributeValue );
	const ClAttributeValue * GetAttributeValue( const string & ai_sAttributeToFind, m4bool_t ai_bSearchUpWard );

	void AddChild( ClXmlTagContainer * ai_pClXmlTagContainer );
};
//----------------------------------------------
//FIN Clase ClXmlTagContainer
//----------------------------------------------

#endif


