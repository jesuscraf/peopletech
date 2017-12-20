// TraductTools.cpp: implementation of the CTraductTools class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4prop.h"
#include "cchildcoldef.h"
#include "cpropcoldef.h"
#include "traducttools.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTraductTools::CTraductTools()
{

}

CTraductTools::~CTraductTools()
{

}

HRESULT CTraductTools::ResetTradIdentifiers(ICPropBagDef *pPBD)
{

	if (pPBD)
	{
		return RecPBD(pPBD);
	}
	
	return E_POINTER;

}

HRESULT CTraductTools::RecPBD(ICPropBagDef *pPBD)
{
	ICPropColDef * pPCD = NULL;
	
	HRESULT hr = pPBD->get_PropColDef(&pPCD);

	if (hr == S_OK)
	{
		CCPropColDef * pCPCD = static_cast<CCPropColDef*>(pPCD);
		
		pCPCD->ResetTradIdentifiers();
		pPCD->Release();
	}

	// Ahora toca iterar por los elementos hijos
	ICChildColDef * pCCD = NULL;
	hr = pPBD->get_ChildColDef(&pCCD);

	if (hr == S_OK)
	{
		ICPropBagDef * pChildPBD = NULL;
		M4PROP_POSITION * pNode = ((CCChildColDef*)pCCD)->GetHeadNode();

		while(pNode)
		{
			pChildPBD = ((CCChildColDef*)pCCD)->GetNext(&pNode);
			RecPBD(pChildPBD);
		}

		pCCD->Release();
	}
	return S_OK;

}
