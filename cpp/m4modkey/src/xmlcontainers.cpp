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

#include "xmlcontainers.hpp"

//----------------------------------------------
//Clase ClAttributeValue
//----------------------------------------------
ClAttributeValue::ClAttributeValue ( void )
	: m_sAttribute(M4MODKEY_EMPTY), m_sValue(M4MODKEY_EMPTY)
{}

ClAttributeValue::ClAttributeValue (string ai_sAttribute, m4pcchar_t ai_sValue )
	: m_sAttribute(ai_sAttribute), m_sValue(ai_sValue)
{}

ClAttributeValue::ClAttributeValue (string ai_sAttribute, string ai_sValue )
	: m_sAttribute(ai_sAttribute), m_sValue(ai_sValue)
{}

const string * ClAttributeValue::GetAttribute( void ) const
{
	return &m_sAttribute;
}

const string * ClAttributeValue::GetValue( void ) const
{
	return &m_sValue;
}

//----------------------------------------------
//FIN Clase ClAttributeValue
//----------------------------------------------

//----------------------------------------------
//Clase ClXmlTagContainer
//----------------------------------------------
ClXmlTagContainer::ClXmlTagContainer( void )
	:m_sTagId (M4MODKEY_EMPTY)
{
	m_pVcAttributeValue = NULL;
	m_pXmlTagContainerParent = NULL;
	m_pVcXmlTagContainerChild = NULL;
}

ClXmlTagContainer::ClXmlTagContainer( string ai_sTagId, ClXmlTagContainer * ai_pXmlTagContainerParent )
	:m_sTagId (ai_sTagId)
{
	m_pVcAttributeValue = new VcClAttributeValue;
	m_pVcXmlTagContainerChild = new VcClXmlTagContainer;
	m_pXmlTagContainerParent = ai_pXmlTagContainerParent;

	//Si se indica un padre, tendremos que insertarnos como hijo en la lista de mi padre.
	if (m_pXmlTagContainerParent != NULL) {
		m_pXmlTagContainerParent->AddChild(this);
	}
}

ClXmlTagContainer::~ClXmlTagContainer( )
{
	if (m_pVcAttributeValue != NULL) {
		for (itVcClAttributeValue it = m_pVcAttributeValue->begin(); it != m_pVcAttributeValue->end(); it++)
		{
			delete (*it);
		}
		delete m_pVcAttributeValue;
		m_pVcAttributeValue = NULL;
	}

	//Destruimos sus hijos.
	if (m_pVcXmlTagContainerChild != NULL) {
		for (itVcClXmlTagContainer it = m_pVcXmlTagContainerChild->begin(); it != m_pVcXmlTagContainerChild->end(); it++)
		{
			delete (*it);
		}
		delete m_pVcXmlTagContainerChild;
		m_pVcXmlTagContainerChild = NULL;
	}
}

m4return_t ClXmlTagContainer::AddAttributeValue( ClAttributeValue * ai_pAttributeValue )
{
	if (m_pVcAttributeValue != NULL) {
		m_pVcAttributeValue->push_back(ai_pAttributeValue);
		return M4_SUCCESS;
	}
	return M4_ERROR;
}

const ClAttributeValue * ClXmlTagContainer::GetAttributeValue( const string & ai_sAttributeToFind, m4bool_t ai_bSearchUpWard )
{
	//El que invoca tiene que declararar un const ClAttributeValue * p3;
	//De esta forma el puntero no es constante pero el objeto apuntado sí!.
	//El invocante recibe un puntero y solo puede consultar!.

	//Tenemos la posibilidad de buscar hacia arriba, los nodos ascendentes.

	if (m_pVcAttributeValue != NULL) {
		for (itVcClAttributeValue it = m_pVcAttributeValue->begin(); it != m_pVcAttributeValue->end(); it++ )
		{
			ClAttributeValue * pAttributeValue = *it;
			if ((pAttributeValue != NULL) 
				&& (pAttributeValue->GetAttribute() != NULL)
				&& (*(pAttributeValue->GetAttribute()) == ai_sAttributeToFind)) {
				//Lo hemos encontrado.
				return pAttributeValue;
			}
		}

		//Si es necesario buscamos en los padres.
		if ((ai_bSearchUpWard == M4_TRUE) && (m_pXmlTagContainerParent != NULL)) {
			return m_pXmlTagContainerParent->GetAttributeValue( ai_sAttributeToFind, ai_bSearchUpWard );
		}
	}
	return NULL;
}

void ClXmlTagContainer::AddChild( ClXmlTagContainer * ai_pClXmlTagContainer )
{
	if (m_pVcXmlTagContainerChild != NULL) {
		m_pVcXmlTagContainerChild->push_back(ai_pClXmlTagContainer);
	}
}
//----------------------------------------------
//FIN Clase ClXmlTagContainer
//----------------------------------------------
