//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             ListComponent.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the CListComp class.
//
//
//==============================================================================

#include "asndef.h"
#include "m4presres.hpp"
#include "presfac.hpp" 
#include "chlog.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListComponent::CListComponent()
{
	m_iNumComp = 0;
}

CListComponent::~CListComponent()
{
	m4uint32_t i=0;

	for (i=0; i<m_iNumComp; i++) {
		CComponent * pCComponent = (*(m_oVcComponent.begin() + i));
		if (pCComponent != NULL) {
			delete pCComponent;
		}		
	}
}

void CListComponent::Add( CComponent * comp )
{
	m_oVcComponent.push_back(comp);
	m_iNumComp++;
}

void CListComponent::Remove( m4int32_t i )
{
	// Elimina la componente i_esima de la lista.
	m_oVcComponent.erase(m_oVcComponent.begin()+i);
	m_iNumComp--;
}

CComponent* CListComponent::View( m4uint32_t i )
{
	// Devuelve la direccion de la componente i_esima de la lista.

	// Importante!! simepre que se quiere ver uan componente, se pasa 
	// un puntero y en este se devuelve la dirección dinde esta alocada 
	// la componente, con lo cualk no hay que eliminar este puntero desde 
	// fuera.

	return (*(m_oVcComponent.begin()+i));	
}

void CListComponent::SetGrant( m4uchar_t value )
{
	//Inserta la propiedad Grant en la lista.
	// Grant contiene el valor que se pasa como parametro.

	CComponent *grant = new CComponent();
	grant->SetTypeNameProp();
	grant->SetLengthNameProp(M4PRESENTATION_GRANTS_SIZE);
	grant->SetNameProp((m4char_t *)M4PRESENTATION_GRANTS, M4PRESENTATION_GRANTS_SIZE);
	grant->SetTypeProp(T_INT);
	grant->SetLengthProp(M4_FALSE,T_INT,value);
	grant->SetIntValueProp(value);
	
	//A fuego pq sabemos que está el classname y el alias.
	if (m_oVcComponent.size() >= 2) {
		m_oVcComponent.insert(m_oVcComponent.begin() + 2,grant);
	}
	else {
		m_oVcComponent.insert(m_oVcComponent.begin(),grant);
	}
	m_iNumComp++;
}

m4uint32_t CListComponent::GetNumComponent()
{
	// Devuelve el numero de componentes de la lista.
	return m_iNumComp;
}

m4return_t CListComponent::ReplaceComponentByPos ( m4uint32_t ai_iPosPropBag, CComponent * ai_pNewComponent )
{
	if ((ai_pNewComponent == NULL)
		|| (ai_iPosPropBag < 0)
		|| (ai_iPosPropBag >= m_oVcComponent.size())) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Tenemos que eliminar la entrada i-esima y añadir en la misma posicion el nuevo.
	//Obtenemos un iterador al elemento a eliminar del vector.
	itVcCComponent it = m_oVcComponent.begin() + ai_iPosPropBag;
	
	//Ojo pq en la insercion los iteradores quedarian invalidos, pero nosotros no utilizamos bucles con iteradores.
	//Insertamos antes de eliminar.
	m_oVcComponent.insert(it, ai_pNewComponent);

	it = m_oVcComponent.begin() + ai_iPosPropBag + 1;
	m_oVcComponent.erase(it);
	
	return M4_SUCCESS;
}

m4return_t CListComponent::InsertComponentInPosition( m4int32_t ai_iPosition, CComponent * ai_pNewComponent )
{
	//Insertamos justo en la posicion donde indicamos.
	m4int32_t iSize = m_oVcComponent.size();
	if ((ai_iPosition > iSize) || (ai_iPosition < 0)) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}
	itVcCComponent it = m_oVcComponent.begin() + ai_iPosition;

	m_oVcComponent.insert(it, ai_pNewComponent);
	m_iNumComp++;

	return M4_SUCCESS;
}

m4return_t CListComponent::InsertComponentInLastPosition( CComponent * ai_pNewComponent )
{
	//Si ai_iPosition es -1 significa que queremos moverlo al principio.
	m4int32_t iPosition = m_oVcComponent.size();
	return InsertComponentInPosition( iPosition, ai_pNewComponent);
}

m4return_t CListComponent::InsertComponentAfterPosition( m4int32_t ai_iPosition, CComponent * ai_pNewComponent )
{
	//Inserta un puntero DESPUES de la posicion que indica el parametro.
	//Las stl insertan before y nosotros queremos insertar after, por lo tanto tenemos que sumar uno.

	//Si ai_iPosition es -1 significa que queremos moverlo al principio.
	m4int32_t iPosition = ai_iPosition + 1;

	return InsertComponentInPosition( iPosition, ai_pNewComponent);
}

void CListComponent::RemoveInListAllComponent( VcCComponent & aio_vCComponent, m4bool_t ai_bDeleteElement )
{
	//Esta funcion es STATIC. Elimina y tambien puede liberar los elementos de una lista de componentes.

	for (itVcCComponent it = aio_vCComponent.begin(); it != aio_vCComponent.end(); it++) {
		CComponent * pCComponentMoveToEnd = (*it);
		if ((pCComponentMoveToEnd != NULL) && (ai_bDeleteElement == M4_TRUE)) {
			delete pCComponentMoveToEnd;
		}
	}
	aio_vCComponent.clear();
}

m4return_t CListComponent::SortListByPositionAfterBegin( VcCComponent & aio_vCComponent )
{
	//Nos pasan una lista pero vamos a recorrerla porque puede haber Positionafter = Begin que
	//haga que se reordene.
	m4return_t iRet = M4_SUCCESS;	
	
	itVcCComponent it;
	itRVcCComponent itR;
	VcCComponent vSorted;

	//--------------------------------------------------------------------------------------
	//Paso1:
	//Tratamos primero todo lo que no sean sequencias para que estén al principio.
	for (it = aio_vCComponent.begin(); it != aio_vCComponent.end(); it++) {
		CComponent * pCComponent = (*it);
		if (pCComponent != NULL) {
			if (pCComponent->GetTypeProp() != ASNSequence) {	
				vSorted.push_back(pCComponent);	
			}	
		}
	}
	//FIN Paso1.
	//--------------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------------
	//Paso2:
	//Tratamos las sequencias que tengan Positionafter = begin y las situamos a continuación.
	//Recorremos al revés para que la última componente que diga Begin es la que quede primero.
	for (itR = aio_vCComponent.rbegin(); itR != aio_vCComponent.rend(); itR++) {	
		CComponent * pCComponent = (*itR);
		if (pCComponent != NULL) {
			if (pCComponent->GetTypeProp() == ASNSequence) {	
				m4bool_t bHasPositionafterBegin = M4_FALSE;
				iRet = pCComponent->HasPositionAfterBegin( bHasPositionafterBegin );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				if (bHasPositionafterBegin == M4_TRUE) {
					vSorted.push_back(pCComponent);
				}
			}	
		}
	}
	//FIN Paso2.
	//--------------------------------------------------------------------------------------

	//Ya tenemos primero los atributos y despues las sequencias con Begin. Ahora reordenamos el resto.

	//--------------------------------------------------------------------------------------
	//Paso3:
	//Me queda ordenar las sequencias que no sean Begin y que tengan Positionafter.
	//Si no tienen position after se van añadiendo en el orden que vienen.
	for (it = aio_vCComponent.begin(); it != aio_vCComponent.end(); it++) {
		CComponent * pCComponent = (*it);
		if (pCComponent != NULL) {
			//Solo vale para sequencias!.
			if (pCComponent->GetTypeProp() == ASNSequence) {

				//Las componentes Positionafter Begin ya se han metido, por lo tanto no las tratamos.
				m4bool_t bHasPositionafterBegin = M4_FALSE;
				iRet = pCComponent->HasPositionAfterBegin( bHasPositionafterBegin );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				if (bHasPositionafterBegin == M4_TRUE) {
					continue;
				}

				//IMPORTANTE: Lo que si asumimos es que no hay un Positionafter que haga referencia a una componente no insertada!.
				//Podría haber habido referencia a una componente que tuviera Begin pero este caso no da problemas
				//porque las componentes con Begin ya se han metido!!.

				m4bool_t bHasPositionafter = M4_FALSE;
				m4int32_t iPositionFound = -1;
				iRet = pCComponent->FindPositionInVectorSearchingByIdSequence( &vSorted, bHasPositionafter, iPositionFound );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				//---------------------------------
				if (bHasPositionafter == M4_TRUE) {
					//La componente tiene Positionafter.
					if (iPositionFound != -1) {
						//La componente hija la ponemos ya en situación correcta.
						itVcCComponent it = vSorted.begin() + iPositionFound;
						vSorted.insert(it, pCComponent);
					}
					else {
						//Esta vez sí es un error, podríamos meterla al final pero vamos a dar un error.
						//-- vSorted.push_back(pViewChild);
						return M4_ERROR;
					}
				}
				else {
					//La componente NO tiene Positionafter. Se inserta y ya esta.
					vSorted.push_back(pCComponent);
				}
				//---------------------------------
			}
		}		
	}
	//FIN Paso3.
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//Paso4.
	//Traspaso de listas!.
	aio_vCComponent.clear();
	for (it = vSorted.begin(); it != vSorted.end(); it++) {
		CComponent * pCComponent = (*it);
		aio_vCComponent.push_back(pCComponent);
	}
	vSorted.clear();
	//--------------------------------------------------------------------------------------

	return iRet;
}


m4return_t CListComponent::MergeUnknownInList( VcCComponent & aio_vToFill, VcCComponent & aio_vToClear )
{
	m4return_t iRet = M4_SUCCESS;	

	//Nos pasan dos listas. La lista aio_vToClear debe quedar vacia, eso significara que habremos
	//encontrado donde poner cada elemento en la lista aio_vToFill.

	//Hay iterar hasta que queden elementos en aio_vToClear. Para evitar bloqueos, si hacemos una pasada
	//y no sabemos donde poner ningun elemento deberemos dar un error porque indica un interbloqueo.

	//--------------------------------------------------------------------------------------
	m4int32_t iPosition = 0;
	while (aio_vToClear.size() > iPosition)
	{		
		itVcCComponent it = aio_vToClear.begin() + iPosition;
		CComponent * pCComponent = (*it);
		if (pCComponent != NULL) {
			//Solo vale para sequencias!.
			if (pCComponent->GetTypeProp() == ASNSequence) 
			{
				//---------------------------------------------------
				//Comprobamos si tiene Positionafter con begin. No deberían haber componentes con begin porque 
				//el proceso de MergeUnknownInList se realiza despues de todo pero lo tendremos en cuenta.
				m4bool_t bHasPositionafterBegin = M4_FALSE;
				iRet = pCComponent->HasPositionAfterBegin( bHasPositionafterBegin );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				if (bHasPositionafterBegin == M4_TRUE) {
					//Movemos la componente entre las listas. Siempre se encuentra un hueco.
					m4int32_t iFirstSequence = CListComponent::FindFirstSequenceInVector(&aio_vToFill);
					itVcCComponent itToFill = aio_vToFill.begin() + iFirstSequence;
					aio_vToFill.insert(itToFill, pCComponent);
					aio_vToClear.erase(it);
					iPosition = 0;
					continue;
				}				
				//---------------------------------------------------

				//No tiene PositionafterBegin.

				//---------------------------------------------------
				//Comprobamos si la componente tiene Positionafter y tambien se obtendria la posicion a donde
				//tiene que ir en el vector aio_vToFill.
				m4bool_t bHasPositionafter = M4_FALSE;
				m4int32_t iPositionFound = -1;
				iRet = pCComponent->FindPositionInVectorSearchingByIdSequence( &aio_vToFill, bHasPositionafter, iPositionFound );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				//---------------------------------
				if (bHasPositionafter == M4_TRUE) {
					//La componente tiene Positionafter.
					if (iPositionFound != -1) {						
						itVcCComponent itToFill = aio_vToFill.begin() + iPositionFound;
						aio_vToFill.insert(itToFill, pCComponent);
						aio_vToClear.erase(it);
						iPosition = 0;
						continue;
					}
					else {
						//No sabemos donde meterlo, probamos en la siguiente.
						iPosition++;
						continue;
					}
				}
				else {
					//La componente NO tiene Positionafter. Este caso es un poco raro porque se supone en este vector 
					//hay componentes del Limbo que por su Positionafter no se saben donde meter.
					//Pero la metemos al final.
					aio_vToFill.push_back(pCComponent);
					aio_vToClear.erase(it);
					iPosition = 0;
					continue;
				}
				//---------------------------------
			}
			else {
				//Si es un atributo, me extraña pq se supone que en la lista solo hay componentes con Positionafter
				//que no supimos ubicar.
				return M4_ERROR;
			}
		}		
	}	
	//--------------------------------------------------------------------------------------

	if (aio_vToClear.size() > 0) {
		//Mal rollo pq no hemos podido pasar todo!. Vamos a montar el mensaje de error.

		itVcCComponent it = aio_vToClear.begin();
		CComponent * pCComponent = (*it);
		if (pCComponent != NULL) {			
			if (pCComponent->GetTypeProp() == ASNSequence) 
			{
				//---------------------------------------------------
				m4pchar_t pIdSequenceToMove = NULL;
				m4bool_t bHasPositionafter = M4_FALSE;
				m4int32_t iPositionFound = -1;
				iRet = pCComponent->GetIdSequenceMoveAttribute( bHasPositionafter, pIdSequenceToMove );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				//---------------------------------------------------

				//-------------------------------------------------------
				m4pchar_t pAliasMe = NULL;				
				m4pchar_t pClassNameMe = NULL;
				iRet = pCComponent->GetClassNameAndAlias( pClassNameMe, pAliasMe );
			
				if ((pAliasMe != NULL) 			
					&& (pIdSequenceToMove != NULL))
				{
					DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_MISSING_POSITIONAFTER, pAliasMe << pIdSequenceToMove ) ;
				}
				else {
					DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
				}
				//-------------------------------------------------------

				if (pClassNameMe != NULL) delete [] pClassNameMe;
				if (pAliasMe != NULL) delete [] pAliasMe;				
				if (pIdSequenceToMove != NULL) delete [] pIdSequenceToMove;
			}
		}

		return M4_ERROR;
	}

	return iRet;
}

void CListComponent::RemoveAllComponent( m4bool_t ai_bDeleteElement )
{
	//Destruimos y podemos liberar.
	if (ai_bDeleteElement == M4_TRUE) {
		for (itVcCComponent it=m_oVcComponent.begin(); it!=m_oVcComponent.end(); it++)
		{
			CComponent * pCComponent = (*it);
			if (pCComponent != NULL) {
				delete pCComponent;
			}
		}
	}
	m_oVcComponent.clear();
	m_iNumComp = 0;
}

void CListComponent::AddComponentFromComponentVector( VcCComponent * ai_pVcCComponent )
{
	//Añadimos las componentes que tiene el argumento.
	for (itVcCComponent it=ai_pVcCComponent->begin(); it!=ai_pVcCComponent->end(); it++) 
	{
		CComponent * pCComponent = (*it);
		Add( pCComponent );
	}
}

m4int32_t CListComponent::FindComponentInVector( VcCComponent * ai_pVcCComponent, const CComponent * ai_pCComponentToFind )
{
	m4int32_t iPos = 0;

	//Si lo encuentra retorna el iterador. Si no lo encuentra devuelve end.	
	for (itVcCComponent it=ai_pVcCComponent->begin(); it!=ai_pVcCComponent->end(); it++)
	{
		CComponent * pView = (*it);
		if (pView != NULL) {
			if (pView == ai_pCComponentToFind) {
				return iPos;
			}
		}
		iPos++;
	}
	//No lo hemos encontrado.
	return ai_pVcCComponent->size();
}

m4int32_t CListComponent::FindComponentInVectorByIdSequence( VcCComponent * ai_pVcCComponent, m4pcchar_t ai_pIdSequenceToMove )
{
	m4int32_t iPos = 0;

	//Buscamos en el vector un identificador. Retorna la posicion. Si no lo encuentra devuelve end.
	for (itVcCComponent it = ai_pVcCComponent->begin(); it != ai_pVcCComponent->end(); it++) 
	{
		CComponent * pView = (*it);
		if (pView != NULL) {
			//Solo tiene sentido buscar en las sequencias.
			if (pView->GetTypeProp() == ASNSequence) {
				//Como es una sequencia podemos preguntar por su alias.
				m4pchar_t pClassName = NULL;
				m4pchar_t pAlias = NULL;
				m4return_t iRet = pView->GetClassNameAndAlias( pClassName, pAlias );
				if (pClassName != NULL) delete pClassName;
				if ((iRet == M4_ERROR) || (pAlias == NULL)){					
					if (pAlias != NULL) delete pAlias;					
					return ai_pVcCComponent->size();						
				}
				//Hemos podido obtener el alias y vamos a compararlo.
				if (strcmp(pAlias, ai_pIdSequenceToMove) == 0) {
					//Coinciden.
					if (pAlias != NULL) delete pAlias;
					return iPos;
				}
				if (pAlias != NULL) delete pAlias;
			}
		}
		iPos++;
	}
	//No lo hemos encontrado.
	return ai_pVcCComponent->size();
}


m4int32_t CListComponent::FindFirstSequenceInVector( VcCComponent * ai_pVcCComponent )
{
	m4int32_t iPos = 0;

	//Si lo encuentra retorna la posicion. Si no lo encuentra devuelve end.	
	for (itVcCComponent it=ai_pVcCComponent->begin(); it!=ai_pVcCComponent->end(); it++)
	{
		CComponent * pView = (*it);
		if (pView != NULL) {
			if (pView->GetTypeProp() == ASNSequence) {
				return iPos;
			}
		}
		iPos++;
	}
	//No lo hemos encontrado.
	return ai_pVcCComponent->size();
}

