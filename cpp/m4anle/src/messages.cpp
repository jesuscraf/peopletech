// Messages.cpp: implementation of the CMessages class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4anle.h"
#include "messages.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessages::CMessages()
{

}

CMessages::~CMessages()
{

}

void CMessages::Cargar(ICPropBag * pval)
{
	m_pPBMessages = pval;
}

CString CMessages::TextMessage (CString Code)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;
	ICPropCol *pPC = NULL;
	ICPropCol *pPropCol = NULL;
	ICSingleProp *pSP=NULL;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT va;
	VARIANT vValue;
	
	

	CString clase;
	CString Alias;
	long retorn=0;
	CString resul;
	CString tipo;
	CString nomalias;

	BSTR bClassname=NULL;
	BSTR bAlias=NULL;


	resul = "";

	if (Code.Left(1)!="m")
		Code = CString("m") + Code;

	if (m_pPBMessages)
	{
		HRESULT hr = m_pPBMessages->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);

			if (nCount>0)
			{
				hr=pCC->get__NewEnum(&pUnk);
				hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
				pUnk->Release();

				if (hr == S_OK)
				{
				
					::VariantInit(&vIn);
					pEnum->Reset();
					pEnum->Next(1,&vIn,&num);
					while (((vIn.vt==VT_DISPATCH)&&(num==1))&&(retorn==0))
					{

						pPBAux=(ICPropBag*)vIn.pdispVal;
						hr = pPBAux->get_ClassName(&bClassname);
						hr = pPBAux->get_Alias(&bAlias);

						M4BSTRToCString(bClassname, clase);
						M4BSTRToCString(bAlias, Alias);

					// Comprobamos que el Alias coincide con el nombre
					// que nos han pasado
						if (Alias==Code)
						{						
							retorn = 1;
						// Aqui tomo el valor de la propiedad Text.
							hr =pPBAux->get_PropCol((ICPropCol**)&pPropCol);
							if (hr==S_OK)
							{
								hr = pPropCol->get_Count(&nCount);
								if ((hr==S_OK) && (nCount>0))
								{
							// Aquí busco donde se encuentra la maldita propiedad,
							// pues tengo el grupo de propiedades.
												
									::VariantInit(&va);
									va.vt = VT_I4;
									va.lVal = 1;
									hr = pPropCol->get_Item(va,&pSP);
									::VariantClear(&va);
									::VariantInit(&va);
									::VariantInit(&vValue);
									hr=pSP->get_Value(&vValue);
									pSP->Release();
									M4BSTRToCString(vValue.bstrVal, nomalias);
									::VariantClear(&va);
									::VariantClear(&vValue);

							// Compruebo que existe la propiedad en el 
							// grupo de propiedades. Aux = -1 cuando
							// no hemos encontrado dicha propiedad en
							// el grupo especificado
									resul = nomalias;
												
									pPropCol->Release();
						
								}
							}
						}

						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);

					}
					pEnum->Release();
				}
			}
			pCC->Release();
		}
	}
	// Liberamos las variables.
	SysFreeString(bClassname);
	SysFreeString(bAlias);

	// Retornamos si dicho objeto se encuentra o no dentro
	// de la PB de Objetos.
	return resul;
	
}