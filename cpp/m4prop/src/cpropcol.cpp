// CPropCol.cpp : Implementation of CCPropCol
#include "stdafx.h"
#include "m4prop.h"
#include "cpropcol.h"
#include "csingleprop.h"

//##. Begin Adri y Juanma.
//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
//	de propiedades y las sustituimos por un código
#include "namepropmap.h"

extern CNamePropMap g_NamePropMap;
// End 

/////////////////////////////////////////////////////////////////////////////
// CCPropCol


STDMETHODIMP CCPropCol::get_Length(long * pVal)
{
	ICSingleProp * sp;
	CMinNode * pNode;
	long nLength;

	pNode = m_pNodeHead;

	nLength = 0;
	while (pNode != NULL)
	{
		long tmpVal;
		sp = pNode->pData;

		if( sp != NULL )
		{
			sp->get_Length(&tmpVal);
			nLength += tmpVal;
		}
		pNode = pNode->pNext;
	}
	
	*pVal = nLength;

	return S_OK;
}

STDMETHODIMP CCPropCol::WriteASN(unsigned char** charIn)
{
	CMinNode* pNode = m_pNodeHead;

	while( pNode != NULL )
	{
		CCSingleProp* sp = (CCSingleProp*)pNode->pData;

		if( sp != NULL )
		{
			sp->WriteASN(charIn);
		}
		pNode = pNode->pNext;
	}

	return S_OK;
}

STDMETHODIMP CCPropCol::WriteXML(ClLongString &roXML)
{
	CMinNode* pNode = m_pNodeHead;

	while( pNode != NULL )
	{
		CCSingleProp* sp = (CCSingleProp*)pNode->pData;

		if( sp != NULL )
		{
			sp->WriteXML(roXML);
		}
		pNode = pNode->pNext;
	}

	return S_OK;
}

void CCPropCol::PrePersistProccess()
{
	CCSingleProp	*sp;
	CMinNode		*pNode;
	CMyBSTR			bstrTemp1;
	CMyBSTR			bstrTemp2;
	pNode = m_pNodeHead;

	while (pNode != NULL)
	{
		sp = (CCSingleProp*)pNode->pData;
		// Nos pasamos al siguiente antes de la posible eliminación
		pNode = pNode->pNext;
		//##. Begin Adri y Juanma.
		//	Optimización de ECI. Eliminamos todas las cadenas que almacenan nombre
		//	de propiedades y las sustituimos por un código
		//bstrTemp1 = sp->m_bstrName;
		BSTR bstrName = g_NamePropMap.GetBSTRFromCode (sp->m_iNameCode);
		bstrTemp1 =  bstrName;
		//End
		bstrTemp2 = "FEKey";
		if (bstrTemp1 == bstrTemp2)
		{
			VARIANT Var;
			::VariantInit(&Var);
			Var.vt = VT_BSTR;
			//##. Begin Adri y Juanma.
				//Var.bstrVal = ::SysAllocString(sp->m_bstrName);
			Var.bstrVal = ::SysAllocString(bstrName);
			//End

			Remove(Var);
			::VariantClear(&Var);
		}
		bstrTemp2 = "User";
		if (bstrTemp1 == bstrTemp2)
		{
			// Hay que eliminar esta Key
						// Hay que eliminar esta Key

			VARIANT Var;
			::VariantInit(&Var);
			Var.vt = VT_BSTR;
			//##. Begin Adri y Juanma.
				//Var.bstrVal = ::SysAllocString(sp->m_bstrName);
			Var.bstrVal = ::SysAllocString(bstrName);
			//End

			Remove(Var);
			::VariantClear(&Var);
		}
	}

}
