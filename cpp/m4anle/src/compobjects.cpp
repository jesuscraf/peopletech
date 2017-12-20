// CompObjects.cpp: implementation of the CCompObjects class.
//
//////////////////////////////////////////////////////////////////////
 
#include "stdafx.h"
#include "runtime.h"
#include "m4anle.h"
#include "compobjects.h"
#include "pilaobligatorios.h"
#include "utils.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompObjects::CCompObjects()
{
	m_pPBObjects=NULL;
	m_pPBObjectsgroup=NULL;
	m_pPBPropertysgroup=NULL;
}

CCompObjects::~CCompObjects()
{

}

long CCompObjects::BuscObj(CString nomobj,short& allpro, short& allobj,short& ignore)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;

	unsigned long num;


	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	VARIANT	vIn;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	long numobj=1;
	long retorn=0;
	short devul=0;
	CString tipo;


	if (m_pPBObjects)
	{
		HRESULT hr = m_pPBObjects->get_ChildCol((ICChildCol**)&pCC);
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
					
					BSTR bstrClassName = NULL;
					BSTR bstrAlias = NULL;
					hr = pPBAux->get_ClassName(&bstrClassName);
					hr = pPBAux->get_Alias(&bstrAlias);

					M4BSTRToCString(bstrClassName, clase);
					M4BSTRToCString(bstrAlias, Alias);

					::SysFreeString(bstrClassName);
					::SysFreeString(bstrAlias);
					bstrClassName = NULL;
					bstrAlias = NULL;

					if (Alias==nomobj)
					{						
						tipo="Allobjects";
						devul = Allobjecto(pPBAux,tipo);
						if (devul<0)
							allobj = 1;
						devul=0;
						tipo="Ignore";
						devul = Allobjecto(pPBAux,tipo);
						if (devul<0)
							ignore = 1;
						devul=0;
						tipo="Allproperties";
						devul = Allobjecto(pPBAux,tipo);
						if (devul<0)
							allpro = 1;
						retorn=numobj;
					}
					else
						++numobj;

					// Una vez obtenida la clase
					// trabajamos con las propiedades
					// y objetos que tiene contenidos

					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();
			}
			}
			pCC->Release();
		}
	}

	return retorn;
}

void CCompObjects::CargaPb(ICPropBag * pval1,ICPropBag * pval2,ICPropBag * pval3)
{
	m_pPBObjects = pval1;
	m_pPBObjectsgroup = pval2;
	m_pPBPropertysgroup = pval3;

}

long CCompObjects::BusCont(long nobj, CString nomobj)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICPropBag *pPBAux2 = NULL;
	ICChildCol *pCC = NULL;
	ICChildCol *pCC2 = NULL;
	ICPropCol *pPropCol;

	unsigned long num;


	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	VARIANT	vIn;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	long retorn=0;
	short aux=0;
	short devul=0;
	ICSingleProp *pSP = NULL;
	//ICDefProp *pSP=NULL;
	CString vernom;
	CString tipo;


	if ( nobj == -1)
		return 1;
	else
	{


	if (m_pPBObjects)
	{

	HRESULT hr = m_pPBObjects->get_ChildCol((ICChildCol**)&pCC);
	if (hr == S_OK)
	{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);


		if ((hr==S_OK) && (nCount>0))
		{
				// Aquí obtenemos el puntero a la PB hija que queremos,
				// y hemos pasado como argumento

				VARIANT v;
				::VariantInit(&v);
				v.vt = VT_I4;
				v.lVal = nobj;
				hr = pCC->get_Item(v,(ICPropBag**)&pPBAux);
				::VariantClear(&v);
				pCC->Release();

				tipo="Allobjects";

				devul = Allobjecto(pPBAux,tipo);
				if (devul<0)
					retorn = -1;

				if (hr==S_OK)
				{
					hr=pPBAux->get_ChildCol((ICChildCol**)&pCC2);
					if (hr==S_OK)
					{
						hr =pCC2->get_Count(&nCount);
						if ((hr==S_OK) && (nCount>0))
						{
							hr=pCC2->get__NewEnum(&pUnk);
							hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
							pUnk->Release();
							if (hr == S_OK)
							{				
								::VariantInit(&vIn);
								pEnum->Reset();
								pEnum->Next(1,&vIn,&num);
								while (((vIn.vt==VT_DISPATCH)&&(num==1))&&(retorn==0))
								{
									pPBAux2=(ICPropBag*)vIn.pdispVal;

									BSTR bstrTemp = NULL;
									hr = pPBAux2->get_ClassName(&bstrTemp);
									M4BSTRToCString(bstrTemp, clase);
									RELEASEBSTR(bstrTemp);

									if (clase=="Contains")									
									{
										hr =pPBAux2->get_PropCol((ICPropCol**)&pPropCol);
										if (hr==S_OK)
										{
											hr = pPropCol->get_Count(&nCount);
											if ((hr==S_OK) && (nCount>0))
											{						
												aux=EncObj(&pPropCol, nomobj);
												if (aux>0)
													retorn=1;
											}
										}
										pPropCol->Release();
									}
									::VariantClear(&vIn);
									pEnum->Next(1,&vIn,&num);
								}
								::VariantClear(&vIn);
								pEnum->Release();
							}
						}
					}
					pCC2->Release();
				}
				
			}
		}
	}
	return retorn;
	}
}


short CCompObjects::IntroduOblig(long nobj, CPilaObligatorios * PilaOblig, int id_obj)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICPropBag *pPBAux2 = NULL;
	ICChildCol *pCC = NULL;
	ICChildCol *pCC2 = NULL;
	ICPropCol *pPropCol;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT vValue;
	CString nomalias;
	ICPropCol *pPC = NULL;

	short retorn=1;
	short aux=0;
	ICSingleProp *pSP=NULL;
	CString vernom;


	if (m_pPBObjects)
	{

		HRESULT hr = m_pPBObjects->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);
			if ((hr==S_OK) && (nCount>0))
			{
				// Aquí obtenemos el puntero a la PB hija que queremos,
				// y hemos pasado como argumento
				hr = pCC->get_Item(CComVariant(nobj),(ICPropBag**)&pPBAux);
				pCC->Release();
				if (hr==S_OK)
				{
					hr=pPBAux->get_ChildCol((ICChildCol**)&pCC2);
					if (hr==S_OK)
					{
						hr =pCC2->get_Count(&nCount);
						if ((hr==S_OK) && (nCount>0))
						{
							hr=pCC2->get__NewEnum(&pUnk);
							hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
							pUnk->Release();
							if (hr == S_OK)
							{				
								::VariantInit(&vIn);
								pEnum->Reset();
								pEnum->Next(1,&vIn,&num);
								while (((vIn.vt==VT_DISPATCH)&&(num==1))&&(retorn==1))
								{
									pPBAux2=(ICPropBag*)vIn.pdispVal;

									BSTR bstrClassName = NULL;
									hr = pPBAux2->get_ClassName(&bstrClassName);
									
									if (wcscmp(bstrClassName , L"Property") == 0)
									{
										hr =pPBAux2->get_PropCol((ICPropCol**)&pPropCol);
										if (hr==S_OK)
										{
											hr = pPropCol->get_Count(&nCount);
											if ((hr==S_OK) && (nCount>0))
												{
												// Aquí vendría el rellenar la estructura
													if ( EncPropObl(&pPropCol) )
													{
														BSTR bstrAlias = NULL;
														pPBAux2->get_Alias(&bstrAlias);
														CString Alias = bstrAlias;
														PilaOblig->AddPropObligatoria(id_obj,Alias);
														RELEASEBSTR(bstrAlias);
													}
												}
										}
										pPropCol->Release();
									}


									if (wcscmp(bstrClassName , L"Contains") == 0)
									{
										hr =pPBAux2->get_PropCol((ICPropCol**)&pPropCol);
										if (hr==S_OK)
										{
											hr = pPropCol->get_Count(&nCount);
											if ((hr==S_OK) && (nCount>0))
											{					
											// Llama a una función recursiva con el nombre.
												GuarObjOblig(&pPropCol, PilaOblig, id_obj);
											}
										}
										pPropCol->Release();
									}

									if (wcscmp(bstrClassName , L"Aggregates") == 0)
									{
										hr =pPBAux2->get_PropCol((ICPropCol**)&pPropCol);
										if (hr==S_OK)
										{
											hr = pPropCol->get_Count(&nCount);
											if ((hr==S_OK) && (nCount>0))
											{
												// Aquí busco donde se encuentra la maldita propiedad,
												// pues tengo el grupo de propiedades.
												pPropCol->get_Item(CComVariant((long)1),(ICSingleProp**)&pSP);
												if (pSP)
												{
													::VariantInit(&vValue);
													pSP->get_Value(&vValue);
													RELEASE(pSP);
													M4BSTRToCString(vValue.bstrVal, nomalias);
													::VariantClear(&vValue);
													GuarProblig(nomalias, PilaOblig, id_obj);
												}
											}
										}
										RELEASE(pPropCol);
									}
									RELEASEBSTR(bstrClassName);
									::VariantClear(&vIn);
									pEnum->Next(1,&vIn,&num);
								}
								::VariantClear(&vIn);
								pEnum->Release();
							}
						}
					}
					pCC2->Release();
				}
				
			}
		}
	}

	return retorn;

}

short CCompObjects::EncObj(ICPropCol  ** pval, CString nomobj)
{
	USES_CONVERSION;

	ICPropCol *pPC = *pval;
	ICSingleProp *pSP=NULL;
	unsigned long num;
	CString vernom;
	VARIANT vIn;
	VARIANT vOt;

	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	
	long nProp;
	CString cadena;	
	short termina=0;
	short aux=0;

	HRESULT hr = pPC->get_Count(&nProp);
	
	if ((hr==S_OK) && (nProp>0))
	{

    hr=pPC->get__NewEnum(&pUnk);
	hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
	pUnk->Release();

	if (hr == S_OK)
	{
		
		::VariantInit(&vIn);
		pEnum->Reset();
		pEnum->Next(1,&vIn,&num);
		while (((vIn.vt==VT_DISPATCH)&&(num==1)) && (termina==0))
		{
			pSP=(ICSingleProp*)vIn.pdispVal;

			BSTR bstrTemp = NULL;
			pSP->get_Name(&bstrTemp);

			if (wcscmp(bstrTemp, L"Object") == 0)
			{
				::VariantInit(&vOt);
				pSP->get_Value(&vOt);

				M4BSTRToCString(vOt.bstrVal, cadena);

				if (nomobj==cadena)
				{
					termina=1;
				}
			
				::VariantClear(&vOt);
			}
			if (wcscmp(bstrTemp, L"Objectgroup") == 0)
			{

				::VariantInit(&vOt);
				pSP->get_Value(&vOt);

				M4BSTRToCString(vOt.bstrVal, cadena);

				termina=EncGroupObj(nomobj,cadena);

				::VariantClear(&vOt);
			}
			RELEASEBSTR(bstrTemp);
			::VariantClear(&vIn);
			pEnum->Next(1,&vIn,&num);

		}
		pEnum->Release();

	}
	}
	return termina;

}

short CCompObjects::EncGroupObj(CString nomobj, CString nomgroup)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildColDef *pCC = NULL;

	unsigned long num;


	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	VARIANT	vIn;
	
	ICPropColDef *pPC = NULL;

	CString clase;
	CString Alias;
	long numobj=1;
	short retorn=0;
	short aux=0;

	if (m_pPBObjectsgroup)
	{
		HRESULT hr = m_pPBObjectsgroup->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);


			if ((nCount>0) && (hr==S_OK))
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

					BSTR bstrTemp = NULL;
					hr = pPBAux->get_ClassName(&bstrTemp);
					M4BSTRToCString(bstrTemp, clase);
					RELEASEBSTR(bstrTemp);
					hr = pPBAux->get_Alias(&bstrTemp);
					M4BSTRToCString(bstrTemp, Alias);
					RELEASEBSTR(bstrTemp);

					if (Alias==nomgroup)
					{
						aux=EncContainsGroup(pPBAux, nomobj);
						if (aux>0)
							retorn=1;
					}
					else
						++numobj;

					// Una vez obtenida la clase
					// trabajamos con las propiedades
					// y objetos que tiene contenidos

					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();
			}
			}
			pCC->Release();
		}
	}
	return retorn;
}

short CCompObjects::EncContainsGroup(ICPropBag * pval, CString nomobj)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;
	ICPropCol *pPropCol;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	short retorn=0;
	short aux=0;

	if (pval)
	{
		HRESULT hr = pval->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);

			if ((hr==S_OK) && (nCount>0))
			{
				// Aquí obtenemos el puntero a la PB hija que queremos,
				// y hemos pasado como argumento

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

						BSTR bstrTemp = NULL;
						hr = pPBAux->get_ClassName(&bstrTemp);

						if (wcscmp(bstrTemp, L"Contains") == 0)
						{
							hr =pPBAux->get_PropCol((ICPropCol**)&pPropCol);
							if (hr==S_OK)
							{
								hr = pPropCol->get_Count(&nCount);
								if ((hr==S_OK) && (nCount>0))
								{
									aux=EncObj(&pPropCol, nomobj);
									if (aux>0)
										retorn=1;
								}
							}
							pPropCol->Release();
						}
						RELEASEBSTR(bstrTemp);
						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);
					}
					::VariantClear(&vIn);
					pEnum->Release();
				}

			}
			pCC->Release();
		}
	}
	return retorn;
}

long CCompObjects::EncProp(long nobj, CString nompro, tpprop& estr_prop)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICPropBag *pPBAux2 = NULL;
	ICChildCol *pCC = NULL;
	ICChildCol *pCC2 = NULL;
	ICChildCol *pCC3 = NULL;
	ICPropCol *pPropCol;
	ICSingleProp *pSP;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT vValue;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	CString nomalias;
	long retorn=0;
	short aux=0;
	short devul=0;

	CString tipo;

	if ( nobj == -1)
		return -1;
	else
	{

		if (m_pPBObjects)
		{
			HRESULT hr = m_pPBObjects->get_ChildCol((ICChildCol**)&pCC);
			if (hr == S_OK)
			{
				// Aquí vamos objetniendo los punteros a las diferentes 
				// Property bag hijas
				long nCount;
				hr = pCC->get_Count(&nCount);
				if ((hr==S_OK) && (nCount>0))
				{
					// Aquí obtenemos el puntero a la PB hija que queremos,
					// y hemos pasado como argumento

					hr = pCC->get_Item(CComVariant(nobj),(ICPropBag**)&pPBAux);
					pCC->Release();

					tipo="Allproperties";

					devul = Allobjecto(pPBAux,tipo);
					if (devul<0)
						retorn = -1;

					if (hr==S_OK)
					{
						hr=pPBAux->get_ChildCol((ICChildCol**)&pCC2);
						if (hr==S_OK)
						{
							hr =pCC2->get_Count(&nCount);
							if ((hr==S_OK) && (nCount>0))
							{
								hr=pCC2->get__NewEnum(&pUnk);
								hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
								pUnk->Release();

								if (hr == S_OK)
								{
					
									::VariantInit(&vIn);
									pEnum->Reset();
									pEnum->Next(1,&vIn,&num);
									while (((vIn.vt==VT_DISPATCH)&&(num==1))&&(retorn==0))
									{
										pPBAux2=(ICPropBag*)vIn.pdispVal;
										
										BSTR bstrClassName = NULL;
										BSTR bstrAlias = NULL;
										hr = pPBAux2->get_ClassName(&bstrClassName);
										hr = pPBAux2->get_Alias(&bstrAlias);

										if (wcscmp(bstrClassName, L"Property") == 0)
										{
											if (bstrAlias == nompro)
											{
												// Rellena la estructura a devolver
												// y devuelve un 1
												hr =pPBAux2->get_PropCol((ICPropCol**)&pPropCol);
												if (hr==S_OK)
												{
													hr = pPropCol->get_Count(&nCount);
													if ((hr==S_OK) && (nCount>0))
													{
														// Aquí vendría el rellenar la estructura
														aux=RellEstruc(&pPropCol, estr_prop);
														if (aux>0)
															retorn=1;
													}
												}
												pPropCol->Release();
												hr=pPBAux2->get_ChildCol((ICChildCol**)&pCC3);						
												if (hr==S_OK)
												{
													estr_prop.Constraint.Endvalidate=FALSE;
													estr_prop.Constraint.Expression="";												
													estr_prop.Constraint.Code="";
													estr_prop.Constraint.Message="";
													estr_prop.Constraint.Loopinit="1";
													estr_prop.Constraint.Loopend="1";
													estr_prop.Constraint.Errorlevel=0;
													aux=RellEstrucConstr(&pCC3, estr_prop);
												}
											}
										}
										else
										{

											if (wcscmp(bstrClassName, L"Aggregates") == 0)
											{
												hr =pPBAux2->get_PropCol((ICPropCol**)&pPropCol);
												if (hr==S_OK)
												{
													hr = pPropCol->get_Count(&nCount);
													if ((hr==S_OK) && (nCount>0))
													{					
														// Aquí busco donde se encuentra la maldita propiedad,
														// pues tengo el grupo de propiedades.
														hr = pPropCol->get_Item(CComVariant((long)1),(ICSingleProp**)&pSP);
														::VariantInit(&vValue);
														hr=pSP->get_Value(&vValue);
														pSP->Release();
														M4BSTRToCString(vValue.bstrVal, nomalias);
														::VariantClear(&vValue);
														estr_prop.Constraint.Endvalidate=FALSE;
														estr_prop.Constraint.Expression="";												
														estr_prop.Constraint.Code="";
														estr_prop.Constraint.Message="";
														estr_prop.Constraint.Loopinit="1";
														estr_prop.Constraint.Loopend="1";
														estr_prop.Constraint.Errorlevel=0;
														aux=EncAgregates(nompro, nomalias, estr_prop);
														if (aux>0)
															retorn=1;
													}
												}
												pPropCol->Release();
											}
										}
										RELEASEBSTR(bstrClassName);
										RELEASEBSTR(bstrAlias);
										::VariantClear(&vIn);
										pEnum->Next(1,&vIn,&num);
									}
									::VariantClear(&vIn);
									pEnum->Release();
								}
							}
						}
					}
					pCC2->Release();
				}
			}
		}
		return retorn;
	}
}

short CCompObjects::EncGroupPro(ICPropBag * pval, CString nompro, tpprop & estr_prop)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;
	ICPropCol *pPropCol;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT va;
	VARIANT vValue;
	ICSingleProp *pSP=NULL;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	CString nomalias;
	short retorn=0;
	short aux=0;

	if (pval)
	{
		HRESULT hr = pval->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);

			if ((hr==S_OK) && (nCount>0))
			{
				// Aquí obtenemos el puntero a la PB hija que queremos,
				// y hemos pasado como argumento

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

						BSTR bstrClassName = NULL;
						BSTR bstrAlias = NULL;
						hr = pPBAux->get_ClassName(&bstrClassName);
						hr = pPBAux->get_Alias(&bstrAlias);

						M4BSTRToCString(bstrClassName, clase);
						M4BSTRToCString(bstrAlias, Alias);

						RELEASEBSTR(bstrClassName);
						RELEASEBSTR(bstrAlias);

						if (Alias==nompro)
						{
							hr =pPBAux->get_PropCol((ICPropCol**)&pPropCol);
							if (hr==S_OK)
							{
								hr = pPropCol->get_Count(&nCount);
								if ((hr==S_OK) && (nCount>0))
								{
									// Aquí vendría el rellenar la estructura
									aux=RellEstruc(&pPropCol, estr_prop);
									if (aux>0)
										retorn=1;
								}
							}
							pPropCol->Release();
						}
						if (clase=="Aggregates")									
						{
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
									hr = pPropCol->get_Item(va,(ICSingleProp**)&pSP);
									::VariantClear(&va);
									::VariantInit(&va);
									::VariantInit(&vValue);
									hr=pSP->get_Value(&vValue);
									pSP->Release();
									M4BSTRToCString(vValue.bstrVal, nomalias);
									::VariantClear(&va);
									::VariantClear(&vValue);								
									aux=EncAgregates(nompro, nomalias, estr_prop);
									if (aux>0)
										retorn=1;
								}
							}
							pPropCol->Release();
						}

						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);
					}
					::VariantClear(&vIn);
					pEnum->Release();
				}

			}
			pCC->Release();
		}
	}
	return retorn;
}



short CCompObjects::EncAgregates(CString nompro, CString nomgroup, tpprop &estr_prop)
{

	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildColDef *pCC = NULL;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	
	ICPropColDef *pPC = NULL;

	CString clase;
	CString Alias;
	long numobj=1;
	short retorn=0;
	short aux=0;

	if (m_pPBPropertysgroup)
	{
		HRESULT hr = m_pPBPropertysgroup->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);


			if ((nCount>0) && (hr==S_OK))
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

					BSTR bstrClassName = NULL;
					BSTR bstrAlias = NULL;
					hr = pPBAux->get_ClassName(&bstrClassName);
					hr = pPBAux->get_Alias(&bstrAlias);

					M4BSTRToCString(bstrClassName, clase);
					M4BSTRToCString(bstrAlias, Alias);

					RELEASEBSTR(bstrClassName);
					RELEASEBSTR(bstrAlias);


					if (Alias==nomgroup)
					{
						// Aquí tengo la property donde del nombre pasado
						aux=EncGroupPro(pPBAux, nompro, estr_prop);
						if (aux>0)
							retorn=1;
					}
					else
						++numobj;

					// Una vez obtenida la clase
					// trabajamos con las propiedades
					// y objetos que tiene contenidos

					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();
			}
			}
			pCC->Release();
		}
	}
	return retorn;

}

short CCompObjects::RellEstruc(ICPropCol ** pval, tpprop & estr_prop)
{
	USES_CONVERSION;

	ICPropCol *pPC = *pval;
	ICSingleProp *pSP=NULL;
	unsigned long num;
	CString vernom;
	VARIANT vIn;
	VARIANT vOt;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	
	long nProp;
	CString cadena;	
	short aux=0;
	int esbolean;
	bool rsbol;

	estr_prop.Type="";
	estr_prop.Kind="FREE";
	estr_prop.Translated=FALSE;
	estr_prop.Helpstring="";
	estr_prop.Runtimeproperty=FALSE;
	estr_prop.Runtimemethod=FALSE;

	if (pPC)
	{

		HRESULT hr = pPC->get_Count(&nProp);
		
		if ((hr==S_OK) && (nProp>0))
		{

			hr=pPC->get__NewEnum(&pUnk);
			hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			pUnk->Release();

			if (hr == S_OK)
			{
				
				::VariantInit(&vIn);
				pEnum->Reset();
				pEnum->Next(1,&vIn,&num);
				while (((vIn.vt==VT_DISPATCH)&&(num==1)))
				{
					pSP=(ICSingleProp*)vIn.pdispVal;

					BSTR bstrTemp = NULL;
					pSP->get_Name(&bstrTemp);
					M4BSTRToCString(bstrTemp, vernom);
					RELEASEBSTR(bstrTemp);

					if (vernom=="Type")
					{
						
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						M4BSTRToCString(vOt.bstrVal, cadena);

						estr_prop.Type=cadena;
					
						::VariantClear(&vOt);
					}
					
					if (vernom=="Kind")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						M4BSTRToCString(vOt.bstrVal, cadena);

						estr_prop.Kind=cadena;
					
						::VariantClear(&vOt);
						
					}
					if (vernom=="Translated")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						esbolean = vOt.bVal;
						if (esbolean)
							rsbol=TRUE;
						else
							rsbol=FALSE;
						
						estr_prop.Translated = rsbol;
					
						::VariantClear(&vOt);
					}

					if (vernom=="Runtimemethod")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						esbolean = vOt.bVal;
						if (esbolean)
							rsbol=TRUE;
						else
							rsbol=FALSE;
						
						estr_prop.Runtimemethod = rsbol;
					
						::VariantClear(&vOt);
					}
					if (vernom=="Runtimeproperty")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						esbolean = vOt.bVal;
						if (esbolean)
							rsbol=TRUE;
						else
							rsbol=FALSE;
						
						estr_prop.Runtimeproperty = rsbol;
					
						::VariantClear(&vOt);
					}

					if (vernom=="Helpstring")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						M4BSTRToCString(vOt.bstrVal, cadena);

						estr_prop.Helpstring=cadena;
					
						::VariantClear(&vOt);
					}
					
					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();

			}
		
		}
	}
	
	return 1;
}

short CCompObjects::Allobjecto(ICPropBag * pval, CString tipo)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICSingleProp *pSP=NULL;
	ICPropCol *pPropCol = NULL;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT vOt;
	
	short retorn=0;
	long nCount;
	short termina=0;

	CString vernom;
	int esbolean=0;

	if (pval)
	{
		HRESULT hr = pval->get_PropCol((ICPropCol**)&pPropCol);
				
		if (hr == S_OK)
		{
			hr = pPropCol->get_Count(&nCount);
			if ((hr==S_OK) && (nCount>0))
			{
				hr=pPropCol->get__NewEnum(&pUnk);
				hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
				pUnk->Release();

				if (hr == S_OK)
				{
		
					::VariantInit(&vIn);
					pEnum->Reset();
					pEnum->Next(1,&vIn,&num);
					while (((vIn.vt==VT_DISPATCH)&&(num==1)) && (termina==0))
					{
						pSP=(ICSingleProp*)vIn.pdispVal;

						BSTR bstrTemp = NULL;
						pSP->get_Name(&bstrTemp);
						M4BSTRToCString(bstrTemp, vernom);
						RELEASEBSTR(bstrTemp);

						if (vernom==tipo)
						{
							termina=1;
							::VariantInit(&vOt);
							pSP->get_Value(&vOt);

							//if (vOt.vt==VT_BOOL)
							esbolean = vOt.bVal;
							if (esbolean)
								retorn=-1;				
			
							::VariantClear(&vOt);
						}
						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);
					}
					::VariantClear(&vIn);
					pEnum->Release();
				}
			}
			pPropCol->Release();
		}
	}

	return retorn;
}


short CCompObjects::EncPropObl ( ICPropCol * * pval)
{

	USES_CONVERSION;

	ICPropCol *pPC = *pval;
	ICSingleProp *pSP=NULL;
	unsigned long num;
	CString vernom;
	VARIANT vIn;
	VARIANT vOt;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	
	long nProp;
	CString cadena;	
	short aux=0;
	short retorn=0;
	short termina=0;

	if (pval)
	{

		HRESULT hr = pPC->get_Count(&nProp);
		
		if ((hr==S_OK) && (nProp>0))
		{

			hr=pPC->get__NewEnum(&pUnk);
			hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			pUnk->Release();

			if (hr == S_OK)
			{
				
				::VariantInit(&vIn);
				pEnum->Reset();
				pEnum->Next(1,&vIn,&num);
				while (((vIn.vt==VT_DISPATCH)&&(num==1)) && (termina==0))
				{
					pSP=(ICSingleProp*)vIn.pdispVal;

					BSTR bstrTemp = NULL;
					pSP->get_Name(&bstrTemp);
					M4BSTRToCString(bstrTemp, vernom);
					RELEASEBSTR(bstrTemp);
					
					if (vernom=="Kind")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						termina=1;
						M4BSTRToCString(vOt.bstrVal, cadena);

						if (cadena=="COMPULSORY")
							retorn=1;
					
						::VariantClear(&vOt);
						
					}

					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();

			}
		
		}
	}
	return retorn;
}

short CCompObjects::GuarObjOblig(ICPropCol **  pval, CPilaObligatorios * PilaOblig, int id_obj)
{
	USES_CONVERSION;

	ICPropCol *pPC = *pval;
	ICDefProp *pSP=NULL;
	unsigned long num;
	CString vernom;
	VARIANT vIn;
	VARIANT vOt;
	OBJ_OBLIGATORIO rell_estr;
	long minins=0;
	long maxins=0;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	
	long nProp;
	CString cadena;	
	short termina=0;
	short aux=0;

	rell_estr.nombre="";

	HRESULT hr = pPC->get_Count(&nProp);
	
	if ((hr==S_OK) && (nProp>0))
	{

		hr=pPC->get__NewEnum(&pUnk);
		hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
		pUnk->Release();

		if (hr == S_OK)
		{
			
			::VariantInit(&vIn);
			pEnum->Reset();
			pEnum->Next(1,&vIn,&num);
			while (((vIn.vt==VT_DISPATCH)&&(num==1)))
			{
				pSP=(ICDefProp*)vIn.pdispVal;

				BSTR bstrTemp = NULL;
				pSP->get_Name(&bstrTemp);
				M4BSTRToCString(bstrTemp, vernom);
				RELEASEBSTR(bstrTemp);

				if (vernom=="Object")
				{
					::VariantInit(&vOt);
					pSP->get_Value(&vOt);

					M4BSTRToCString(vOt.bstrVal, cadena);

					rell_estr.nombre=cadena;
					rell_estr.insmin=-1;
					rell_estr.insmax=-1;
				
					::VariantClear(&vOt);
				}

				if (vernom=="Mininstances")
				{
					::VariantInit(&vOt);
					pSP->get_Value(&vOt);

					if (vOt.vt == VT_I2)
						minins = vOt.iVal;
					else if (vOt.vt == VT_I4)
						minins = vOt.lVal;
					else
						minins = -1;


					rell_estr.insmin=minins;
					termina=1;
				
					::VariantClear(&vOt);
				}

				if (vernom=="Maxinstances")
				{
					::VariantInit(&vOt);
					pSP->get_Value(&vOt);

					if (vOt.vt == VT_I2)
						maxins = vOt.iVal;
					else if (vOt.vt == VT_I4)
						maxins = vOt.lVal;
					else
						maxins = -1;

					rell_estr.insmax=maxins;
									
					::VariantClear(&vOt);
				}

				if (vernom=="Objectgroup")
				{
					// Tengo que coger el nombre del 
					// grupo y pasarselo

					::VariantInit(&vOt);
					pSP->get_Value(&vOt);

					M4BSTRToCString(vOt.bstrVal, cadena);

					GroupObjOblig(cadena, PilaOblig, id_obj);

					::VariantClear(&vOt);
				}

				::VariantClear(&vIn);
				pEnum->Next(1,&vIn,&num);

			}
			pEnum->Release();
			
			if (termina)
			{
				PilaOblig->AddObjObligatorio(id_obj,&rell_estr);
			}


		}
	}
	
	return 1;
}

short CCompObjects::GroupObjOblig(CString nomgrup, CPilaObligatorios * PilaOblig, int id_obj)
{

	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	short retorn=0;
	short aux=0;

	if (m_pPBObjectsgroup)
	{
		HRESULT hr = m_pPBObjectsgroup->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);


			if ((nCount>0) && (hr==S_OK))
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

						BSTR bstrAlias = NULL;
						hr = pPBAux->get_Alias(&bstrAlias);

						M4BSTRToCString(bstrAlias, Alias);
						RELEASEBSTR(bstrAlias);

						if (Alias==nomgrup)
						{
							// Aquí tengo la property donde del nombre pasado
							RecorrerGroupObj(pPBAux, PilaOblig, id_obj);
							retorn=1;
						}

						// Una vez obtenida la clase
						// trabajamos con las propiedades
						// y objetos que tiene contenidos

						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);

					}
					pEnum->Release();
				}
			}
			pCC->Release();
		}
	}
	return retorn;
}

short CCompObjects::RecorrerGroupObj(ICPropBag * pval, CPilaObligatorios * PilaOblig, int id_obj)
{

	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;
	ICPropCol *pPropCol;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	short retorn=0;
	short aux=0;

	if (pval)
	{
		HRESULT hr = pval->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);

			if ((hr==S_OK) && (nCount>0))
			{
				// Aquí obtenemos el puntero a la PB hija que queremos,
				// y hemos pasado como argumento

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

						BSTR bstrClassName = NULL;
						hr = pPBAux->get_ClassName(&bstrClassName);

						if (wcscmp(bstrClassName, L"Contains") == 0)
						{
							hr =pPBAux->get_PropCol((ICPropCol**)&pPropCol);
							if (hr==S_OK)
							{
								hr = pPropCol->get_Count(&nCount);
								if ((hr==S_OK) && (nCount>0))
								{
									GuarObjOblig(&pPropCol, PilaOblig, id_obj);
								}
							}
							pPropCol->Release();
						}
						RELEASEBSTR(bstrClassName);
						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);
					}
					::VariantClear(&vIn);
					pEnum->Release();
				}

			}
			pCC->Release();
		}
	}
	return retorn;
}

short CCompObjects::GuarProblig(CString nomgroup, CPilaObligatorios * PilaOblig, int id_obj)
{

	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	short retorn=0;
	short aux=0;

	if (m_pPBPropertysgroup)
	{
		HRESULT hr = m_pPBPropertysgroup->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);
			if ((nCount>0) && (hr==S_OK))
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

					BSTR bstrTemp = NULL;
					hr = pPBAux->get_Alias(&bstrTemp);
					M4BSTRToCString(bstrTemp, Alias);
					RELEASEBSTR(bstrTemp);

					if (Alias==nomgroup)
					{
						// Aquí tengo la property donde del nombre pasado
						GroupProblig(pPBAux, PilaOblig, id_obj);
						retorn=1;
					}

					// Una vez obtenida la clase
					// trabajamos con las propiedades
					// y objetos que tiene contenidos

					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();
			}
			}
			pCC->Release();
		}
	}
	return retorn;

}

short CCompObjects::GroupProblig(ICPropBag * pval, CPilaObligatorios * PilaOblig, int id_obj)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;
	ICPropCol *pPropCol = NULL;
	ICSingleProp *pSP=NULL;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT va;
	VARIANT vValue;
	
	ICPropCol *pPC = NULL;

	CString clase;
	CString Alias;
	CString nomalias;
	short aux=0;

	BSTR bClassname=NULL;
	BSTR bAlias=NULL;

	if (pval)
	{
		HRESULT hr = pval->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos objetniendo los punteros a las diferentes 
			// Property bag hijas
			long nCount;
			hr = pCC->get_Count(&nCount);

			if ((hr==S_OK) && (nCount>0))
			{
				// Aquí obtenemos el puntero a la PB hija que queremos,
				// y hemos pasado como argumento

				hr=pCC->get__NewEnum(&pUnk);
				hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
				pUnk->Release();

				if (hr == S_OK)
				{
					::VariantInit(&vIn);
					pEnum->Reset();
					pEnum->Next(1,&vIn,&num);
					while (((vIn.vt==VT_DISPATCH)&&(num==1)))
					{
						pPBAux=(ICPropBag*)vIn.pdispVal;

						hr = pPBAux->get_ClassName(&bClassname);
						hr = pPBAux->get_Alias(&bAlias);

						M4BSTRToCString(bClassname, clase);
						M4BSTRToCString(bAlias, Alias);

						if (clase=="Property")
						{
							hr =pPBAux->get_PropCol((ICPropCol**)&pPropCol);
							if (hr==S_OK)
							{
								hr = pPropCol->get_Count(&nCount);
								if ((hr==S_OK) && (nCount>0))
								{
									// Aquí vendría el rellenar la estructura
										if ( EncPropObl(&pPropCol) )
										{
											PilaOblig->AddPropObligatoria(id_obj,Alias);
										}
								}
							}
							pPropCol->Release();
						}
						if (clase=="Aggregates")
						{
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
												
									GuarProblig(nomalias, PilaOblig, id_obj);
								}
							}
							pPropCol->Release();
						}


						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);
					}
					::VariantClear(&vIn);
					pEnum->Release();
				}

			}
			pCC->Release();
		}
	}
	SysFreeString(bClassname);
	SysFreeString(bAlias);
	return 1;
}

short CCompObjects::RellEstrucConstr(ICChildCol ** pval, tpprop & estr_pro)
{
	USES_CONVERSION;
	ICChildCol *pPC = *pval;
	ICPropBag *pPBAux = NULL;
	ICPropCol *pPropCol;
	ICSingleProp *pSP=NULL;

	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	
	CString clase;
	CString Alias;
	short retorn=0;
	short aux=0;
	long nCount;
	tpcons estr_tpcons;

	estr_tpcons.Endvalidate= FALSE;
	estr_tpcons.Expression="";
	estr_tpcons.Code="";
	estr_tpcons.Errorlevel=0;
	estr_tpcons.Loopinit="1";
	estr_tpcons.Loopend="1";
	estr_tpcons.Message="";
	
			
	BSTR bClassname=NULL;
	BSTR bAlias=NULL;


	if (pPC)
	{			
			HRESULT hr = pPC->get_Count(&nCount);
			if ((hr==S_OK) && (nCount>0))
			{
				// Aquí obtenemos el puntero a la PB hija que queremos,
				// y hemos pasado como argumento

				hr=pPC->get__NewEnum(&pUnk);
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

						if (clase=="Constraint")
						{
							hr =pPBAux->get_PropCol((ICPropCol**)&pPropCol);
							if (hr==S_OK)
							{
								hr = pPropCol->get_Count(&nCount);
								if ((hr==S_OK) && (nCount>0))
								{
									// Aquí vendría el rellenar la estructura
									RellConstraint(&pPropCol, estr_tpcons);	
									estr_pro.Constraint = estr_tpcons;
								}
							}
							pPropCol->Release();
						}
						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);
					}
					::VariantClear(&vIn);
					pEnum->Release();
				}
			}
		}
	SysFreeString(bClassname);
	SysFreeString(bAlias);
	return retorn;
}

short CCompObjects::RellConstraint(ICPropCol * * pval, tpcons & estr_tpcons)
{
	USES_CONVERSION;

	ICPropCol *pPC = *pval;
	ICSingleProp *pSP=NULL;
	unsigned long num;
	CString vernom;
	VARIANT vIn;
	VARIANT vOt;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	
	long nProp;
	CString cadena;	
	BSTR nombre=NULL;
	short aux=0;
	int esbolean;
	BOOL rsbol;

	estr_tpcons.Endvalidate= FALSE;
	estr_tpcons.Expression="";
	estr_tpcons.Code="";
	estr_tpcons.Errorlevel=0;
	estr_tpcons.Loopinit="1";
	estr_tpcons.Loopend="1";
	estr_tpcons.Message="";

	if (pval)
	{

		HRESULT hr = pPC->get_Count(&nProp);
		
		if ((hr==S_OK) && (nProp>0))
		{

			hr=pPC->get__NewEnum(&pUnk);
			hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			pUnk->Release();

			if (hr == S_OK)
			{
				
				::VariantInit(&vIn);
				pEnum->Reset();
				pEnum->Next(1,&vIn,&num);
				while (((vIn.vt==VT_DISPATCH)&&(num==1)))
				{
					pSP=(ICSingleProp*)vIn.pdispVal;

					pSP->get_Name(&nombre);
					M4BSTRToCString(nombre, vernom);

					if (vernom=="Loopinit")
					{				
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						M4BSTRToCString(vOt.bstrVal, cadena);

						estr_tpcons.Loopinit=cadena;
					
						::VariantClear(&vOt);				
					}
		//**************
					if (vernom=="Loopend")
					{				
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						M4BSTRToCString(vOt.bstrVal, cadena);

						estr_tpcons.Loopend=cadena;
					
						::VariantClear(&vOt);				
					}
		//**************
					if (vernom=="Errorlevel")
					{				
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						estr_tpcons.Errorlevel=vOt.iVal;
					
						::VariantClear(&vOt);				
					}
		//**************
					if (vernom=="Finalevaluation")
					{				

						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						esbolean = vOt.bVal;
						if (esbolean)
							rsbol=TRUE;
						else
							rsbol=FALSE;
						
						estr_tpcons.Endvalidate = rsbol;
					
						::VariantClear(&vOt);
					}
		//******************			
					if (vernom=="Expression")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						M4BSTRToCString(vOt.bstrVal, cadena);

						estr_tpcons.Expression=cadena;
					
						::VariantClear(&vOt);
						
					}
					if (vernom=="Code")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						char code[20];
						switch(vOt.vt)
						{
							case VT_I2:
							case VT_I4:
								itoa(vOt.iVal, code, 10);
								estr_tpcons.Code = code;
								break;
							case VT_BSTR:
								M4BSTRToCString(vOt.bstrVal, estr_tpcons.Code);
								break;
						}
						::VariantClear(&vOt);
					}
					
					if (vernom=="Message")
					{
						::VariantInit(&vOt);
						pSP->get_Value(&vOt);

						M4BSTRToCString(vOt.bstrVal, cadena);

						estr_tpcons.Message=cadena;
					
						::VariantClear(&vOt);
					}
					
					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();

			}
		}	
	}
	SysFreeString(nombre);
	return 1;
}