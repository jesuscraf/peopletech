// veranle.cpp : Implementation of Cveranle
#include "stdafx.h"
#include "m4anle.h"
#include "error.h"
#include "dinarray.h"
#include "pilaerrores.h"
#include "veranle.h"
#include "runtime.h"
#include "elementos.h"
#include "lectura.h"
#include "escritura.h"
#include "pila.h"
#include "analyzeproperty.h"

#include <stdlib.h>
#include "m4stl.hpp"
#include <stdio.h>
#include <string.h>
#include "m4unicode.hpp"



#define RESOURCE_BUFFER_SIZE 8000

/////////////////////////////////////////////////////////////////////////////
// Cveranle


// Estas dos funcione get_Maxerror y put_Maxerror
// son para obtener y saber cual es el tope de errores
// que se puede cometer durante el analisis.

STDMETHODIMP Cveranle::get_Maxerror(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// m_max_error es una variable miembro donde se
	// encuentra el tope de errores

	*pVal = m_max_errores;

	return S_OK;
}

STDMETHODIMP Cveranle::put_Maxerror(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// m_max_error es una variable miembro donde se
	// encuentra el tope de errores

	m_max_errores = newVal;

	return S_OK;
}


Cveranle::Cveranle()
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	VariantInit(&m_vInitialize);
	
	// Inicializamos a cero el tema del multilenguaje
	m_nLangNumber = 0;
	HRESULT hr = 0;
	m_nMetalengResourceID = IDR_METALENG1;

	m_FicheroLog.Empty();	//Inicializa la cadena
	m_GenerarLog = TRUE;	//Por defecto se genera fichero .log
	m_max_errores = 0;		//Por defecto nº de errores ilimitado
	m_nerror = 0;			//Por defecto nº de errores es igual a 0
	m_Errorlevel = 0;		//Por defecto siempre analiza el nivel máximo.
	m_pPB = NULL;
	m_pPBTypes = NULL;
	m_pPBPropertiesgroup = NULL;
	m_pPBObjectsgroup = NULL;
	m_pPBObjects = NULL;
	m_pPBMessages = NULL;
	m_pTranslate = new Translate();
	
	// Inicializamos la PB de caché de objectos
	hr = CoCreateInstance(CLSID_CPropBag, NULL, CLSCTX_ALL, IID_ICPropBag, (void**)&m_pPBCache);
	if (hr != S_OK)
	{
		m_pPBCache = NULL;
	}

	put_ErrorLanguage(1033); // Por defecto establecemos lenguaje inglés para los errores

	m_pBuffer = (unsigned short*)malloc( (RESOURCE_BUFFER_SIZE+1) * sizeof(unsigned short) );
}

Cveranle::~Cveranle()
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	VariantClear(&m_vInitialize);
	
	// Primero limpiamos la pila de errores por
	// si quedaba algún error de anteriores análisis.

	if (m_pPB != NULL) m_pPB->Release();
	if (m_pPBMessages != NULL) m_pPBMessages->Release();
	if (m_pPBCache != NULL) m_pPBCache->Release();
		
	m_pilaerror.Limpia();

	// Liberamos los entornos antes registrados.
	if (m_pPBTypes != NULL) m_pPBTypes->Release();					 
	if (m_pPBPropertiesgroup != NULL) m_pPBPropertiesgroup->Release();
	if (m_pPBObjectsgroup != NULL) m_pPBObjectsgroup->Release();
	if (m_pPBObjects != NULL) m_pPBObjects->Release();
	
	if (m_pTranslate != NULL) delete m_pTranslate;

	m_pPB = NULL;
	m_pPBTypes = NULL;
	m_pPBPropertiesgroup = NULL;
	m_pPBObjectsgroup = NULL;
	m_pPBObjects = NULL;
	m_pPBMessages = NULL;
	m_pPBCache = NULL;
	m_pTranslate = NULL;
	free(m_pBuffer);
}


STDMETHODIMP Cveranle::Geterror(long error, VARIANT * estruct)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CError  mi_error;
	CComVariant aux[6];
	int resu=0;

	// Obtenermos dado un número de error determinado
	// los datos referentes a dicho error.

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	// Primero consultamos si dicho error se encuentra
	resu = m_pilaerror.GetError(error , &mi_error);

	if (resu>=0)
	{
		aux[0] =  mi_error.m_codigo;
		aux[1] =  mi_error.m_linea;
		aux[2] =  mi_error.m_objeto;
		aux[3] =  mi_error.m_padre;
		aux[4] =  mi_error.m_propiedad;
		aux[5] =  mi_error.m_propval;
		
		estruct[0] = aux[0];
		estruct[1] = aux[1];
		estruct[2] = aux[2];
		estruct[3] = aux[3];
		estruct[4] = aux[4];
		estruct[5] = aux[5];
	}

	return S_OK;
}

// Obtenemos el nombre del fichero log que se quiere
// generar.
STDMETHODIMP Cveranle::get_Filename(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	if (pVal)
	{
		::SysFreeString(*pVal);
		*pVal = M4CppToVB(m_FicheroLog);

	}
	else
		return E_INVALIDARG;

	return S_OK;
}

// Obtenemos el nombre del fichero log que se quiere
// generar.
STDMETHODIMP Cveranle::get_Msg(ICPropBag ** pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;


	*pVal = m_pPBMessages;

	return S_OK;
}

STDMETHODIMP Cveranle::put_Filename(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	// Nombre de la variable miembro donde tenemos el
	// nombre del fichero log a generar.
	M4BSTRToCString(newVal, m_FicheroLog);

	return S_OK;
}

// Función para saber si se genera o no el fichero
// log.
STDMETHODIMP Cveranle::get_GenerateFile(BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_GenerarLog;

	return S_OK;
}

STDMETHODIMP Cveranle::put_GenerateFile(BOOL newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Variable miembro para saber si se genera o no
	// el fichero log.
	m_GenerarLog = newVal;

	return S_OK;
}


// Función principal donde se realiza el analisis del fichero
// que pasamos como argumento.

bool Cveranle::IsIniMetaLenguage()
{
	if (m_pPB == NULL) return false;
		return true;
}

STDMETHODIMP Cveranle::Analyze(BSTR fichero)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;

	int resu=0;
	CString mira;

	// Inicializacion del número de errores a 0.
	m_nerror=0;


	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
	
	// Comprobamos que estan inicializadas las PB
   if((!m_pPB) || (!m_pPBTypes) || (!m_pPBPropertiesgroup) || (!m_pPBObjects))
		return S_OK;

	M4BSTRToCString(fichero, mira);

	// Limpiamos pila de errores
	m_pilaerror.Limpia();

 
	// Creamos una variable de tipo CElementos denominada prueb
	// y le pasamos el número de errores y la cuatro
	// property hijas.
	CElementos prueb(m_max_errores, m_Errorlevel, m_pPBTypes, m_pPBPropertiesgroup, m_pPBObjectsgroup, m_pPBObjects,m_pPBMessages);
	if(!m_FicheroLog.IsEmpty()) 
		//Se ha especificado un nombre para el fichero .log
		resu = prueb.Compilar(mira, &m_pilaerror, m_GenerarLog, m_FicheroLog);
	else 
		//No se ha especificado nombre para el .log
		resu = prueb.Compilar(mira, &m_pilaerror, m_GenerarLog);
	
	// Cargamos en la variable m_nerror el número de errores
	// encontrados tras el análisis.
	m_nerror=resu;

	return S_OK;
}

// Función para limpiar la pila de errores.

STDMETHODIMP Cveranle::Clear()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_pilaerror.Limpia();

	return S_OK;
}

STDMETHODIMP Cveranle::get_ListEventsFromClassname(BSTR classname, ICPropBag * * PB)
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (PB == NULL) return E_INVALIDARG;
	if (*PB)
	{
		(*PB)->Release();
		*PB = NULL;
	}

	HRESULT hr = S_OK;

	if (classname)
	{
		ICChildCol * pCC = NULL;
		hr = m_pPBObjects->get_ChildCol(&pCC);
		if (hr == S_OK)
		{
			hr = CoCreateInstance(CLSID_CPropBag, NULL, CLSCTX_INPROC_SERVER, IID_ICPropBag, (void**)PB);
			if (hr == S_OK)
			{
				long nCount,i;
				pCC->get_Count(&nCount);
				for (i = 1; i<nCount;i++)
				{
					ICPropBag * pPB2 = NULL;
					hr = pCC->get_Item(CComVariant(i), &pPB2);
					if (hr == S_OK)
					{
						BSTR bstrClass = NULL;
						pPB2->get_Alias(&bstrClass);
						// Ya tenemos el ClassName del primero
						VARIANT_BOOL bRet = -1;
						hr = IsContained(classname, bstrClass, &bRet);
						if ((hr == S_OK) && (bRet))
						{	
							// Comprobamos que sea Evento
							hr = IsEvent(bstrClass, &bRet);
							if (hr == S_OK)
							{
								if (bRet != 0)
								{
									long nResourceID = 0;
									nResourceID = GetResourceByClassName(bstrClass);
									(*PB)->AddProperty(bstrClass, CComVariant(nResourceID));
								}
							}
						}	
						pPB2->Release();
						pPB2 = NULL;
						::SysFreeString(bstrClass);
					}
				}
			}
			pCC->Release();
			pCC = NULL;
		}
	}
	return hr;
}

STDMETHODIMP Cveranle::get_PropBagDefFromClassname(BSTR classname, ICPropBagDef * * PBDef)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ICChildCol *pCC = NULL;
	ICPropBagDef * pPB = NULL;
	ICPropColDef *pPC = NULL;
	ICPropBag * pPBInit = NULL;
	
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
	
	if ((m_nLangNumber == 0) || (PBDef == NULL) || (m_pPBObjects == NULL))
	{
		return E_FAIL;
	}
	
	// Primero miramos si la tenemos cacheada
	HRESULT hr = GetFromCache(classname, PBDef);

	if (hr != S_OK)
	{
		// La cojemos y colocamos en la caché
		hr = m_pPBObjects->get_ChildCol(&pCC);
		if (hr == S_OK)
		{
			pCC->get_Item(CComVariant(classname), &pPBInit);
			pCC->Release();
		}
		
		if (pPBInit == NULL)
		{
			return E_INVALIDARG;
		}
		hr = CoCreateInstance(CLSID_CPropBagDef, NULL, CLSCTX_INPROC_SERVER, IID_ICPropBagDef, (void**)&pPB);
		
		if (hr == S_OK)
		{
			TransformPBDef(pPBInit, pPB);
		}
		// Ahora metemos la pPB en la caché
		IDispatch * pDisp = NULL;
		hr = pPB->QueryInterface(IID_IDispatch, (void**)&pDisp);
		if (hr == S_OK)
		{
			CComVariant vAux(pDisp);
			m_pPBCache->AddProperty(classname, vAux);
			pDisp->Release();
			// Lo último es recojerla de la caché
			hr = GetFromCache(classname, PBDef);
		}
		pPB->Release();
		pPBInit->Release();
	}

	return hr;
}

void Cveranle::TransformPBDef(ICPropBag * pbSource, ICPropBagDef * pbRes)
{
	ICChildCol *pChildCol = NULL;
	ICPropBag *pPropBagProperties = NULL;

	BSTR bstrClassName = NULL;
	HRESULT hr = pbSource->get_Alias(&bstrClassName);
	pbRes->put_ClassName(bstrClassName);
	::SysFreeString(bstrClassName);
	hr = pbSource->get_ChildCol(&pChildCol);
	 
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (hr == S_OK)
	{
		// Tenemos la colección de hijas de la pbDef
		long nLong, i;

		hr = pChildCol->get_Count(&nLong);
		for (i = 1; i<= nLong; i++)
		{
			hr = pChildCol->get_Item(CComVariant(i), &pPropBagProperties);
			if (hr == S_OK)
			{
				// Tenemos la primera Propiedad
				AddPropertyToPropBag(pbRes, pPropBagProperties);
				pPropBagProperties->Release();
			}

		}
		pChildCol->Release();
	}
}
void Cveranle::AddPropertiesOfGroup (BSTR nameGroup, ICPropBagDef *pbRes)
{
	
	ICChildCol *pChild = NULL, *pChild2 = NULL;
	ICPropBag *pPBAux = NULL, *pPropAux2 = NULL;
	ICDefProp    *pDefProp = NULL, *pNewDefP = NULL;
	ICPropCol *pPropColAux = NULL;

	unsigned long num, num1, num2;
	IUnknown *pUnk, *pUnk1, *pUnk2, *pUnk3;
	IEnumVARIANT *pEnum, *pEnum1, *pEnum2;
	VARIANT	vIn, vIn1, vIn2, m_objValue, valueDef, m_objValue1, m_objValue2;
	BSTR m_objClass = NULL;
	BSTR m_objName = NULL;
	BSTR m_objAlias = NULL;
	BSTR m_objDefAlias = NULL;
	long nCount, nCount1, nCount2;
	IDataProperties *dataProp1 = NULL, *dataProp2 = NULL;
	CString m_objAliasStr, m_objNameStr, m_objValueStr, inNameGroup, m_objClassStr;
	M4PropertyType m_type;
	bool insert;
	

	HRESULT hr = m_pPBPropertiesgroup->get_ChildCol(&pChild);
	if (hr == S_OK)
	{
		hr = pChild->get_Count(&nCount);

		if (nCount>0)
		{

			hr=pChild->get__NewEnum(&pUnk);
			hr=pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			pUnk->Release();

			if (hr == S_OK)
			{
				::VariantInit(&vIn);
				pEnum->Reset();
				pEnum->Next(1,&vIn,&num);

				while ((vIn.vt==VT_DISPATCH)&&(num==1))
				{//recorrer todas las PropertyGroup
						
					pPBAux=(ICPropBag*)vIn.pdispVal;
					hr = pPBAux->get_Alias(&m_objAlias);
					m_objAliasStr=m_objAlias;
					inNameGroup = nameGroup;
					if (m_objAliasStr == inNameGroup)
					{
						//por fin tenermos nuestra PropertyGroup
								
						hr = pPBAux->get_ChildCol(&pChild2);
						hr = pChild2->get_Count(&nCount1);

						if (nCount1>0)
						{
							hr=pChild2->get__NewEnum(&pUnk1);
							hr=pUnk1->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum1);
							pUnk1->Release();
							::VariantInit(&vIn1);
							hr = pEnum1->Reset();
							hr = pEnum1->Next(1,&vIn1,&num1);

							while ((vIn1.vt==VT_DISPATCH)&&(num1==1))
							{ //obtengo la PropBag

								
								pPropAux2=(ICPropBag*)vIn1.pdispVal;
								pPropAux2->get_PropCol(&pPropColAux);
								pPropAux2->get_Alias(&m_objDefAlias);
								pPropAux2->get_ClassName(&m_objClass);
								pPropColAux->get_Count(&nCount2);
								if (nCount2>0)
								{
									hr=pPropColAux->get__NewEnum(&pUnk2);
									hr=pUnk2->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum2);
									pUnk2->Release();
									::VariantInit(&vIn2);
									pEnum2->Reset();
									pEnum2->Next(1,&vIn2,&num2);

									while ((vIn2.vt==VT_DISPATCH)&&(num2==1))
									{//recorro la PropCol
										pDefProp=(ICDefProp*)vIn2.pdispVal;
										hr = pDefProp->get_Name(&m_objName);
										m_objNameStr=m_objName;
										::VariantInit(&m_objValue);
										hr = pDefProp->get_Value(&m_objValue);
								
										if (m_objValue.vt == VT_BSTR)
											m_objValueStr=m_objValue.bstrVal;
										
										if ((m_objNameStr=="Propertygroup") && (m_objValueStr=="Dataprop"))
										//soloo hay datProp (no cablear!!!)
										//incluir el objeto Dataprop
										{
											hr = CoCreateInstance(CLSID_DataProperties, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnk2);
											hr = pUnk2->QueryInterface(IID_IDataProperties, (void**)&dataProp1);

											if (hr == S_OK)
											{
												m_objValue1.pdispVal=dataProp1;
												m_objValue1.vt = VT_DISPATCH;
											}
												
											if (hr == S_OK)
											{
												hr = CoCreateInstance(CLSID_DataProperties, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pUnk3);
												hr = pUnk3->QueryInterface(IID_IDataProperties, (void**)&dataProp2);
										
												m_objValue2.pdispVal=dataProp2;
												m_objValue2.vt = VT_DISPATCH;
													
												BSTR nameAux = ::SysAllocString(L"DataProps");
												pbRes->AddProperty(nameAux, m_objValue1, m_objValue2,FE_DATAPROP);
												::SysFreeString(nameAux);
											}
											::VariantClear(&m_objValue1);
											::VariantClear(&m_objValue2);
											pUnk2->Release();
											pUnk3->Release();
											insert = false;
										}
										else
										{
											::VariantInit(&valueDef);
											if (m_objNameStr == "Default")
											{
												::VariantCopy(&valueDef,&m_objValue); 
											}
											else
											{
												if (m_objNameStr == "Type")
												{  
													if ((m_objValueStr == "Integer") || (m_objValueStr == "Long") || (m_objValueStr == "WorkdatesType") || (m_objValueStr == "GrantsType") || (m_objValueStr == "IdpresentationType") || (m_objValueStr == "TwovalueType") || (m_objValueStr == "FloatingType"))
													{
														m_type = FE_NUMBER;
													}
													else if (m_objValueStr == "Bool")
													{
														m_type = FE_BOOLEAN;
													}
													else if ((m_objValueStr == "String")  || (m_objValueStr == "ActionType") || (m_objValueStr == "AlignType") || (m_objValueStr == "IdinstanceType") || (m_objValueStr == "Action_exeType") || (m_objValueStr == "Action_copyType") || (m_objValueStr == "MvcactionType") || (m_objValueStr == "Action_helpType") || (m_objValueStr == "StringUpType") || (m_objValueStr == "ListstringUpType"))
													{
														m_type = FE_STRING;
													}
													else if (m_objValueStr == "Color")
													{
														m_type = FE_COLOR;
													}
												}
											}
										 }  

										  ::VariantClear(&vIn2);
										  pEnum2->Next(1,&vIn2,&num2);
									}

									if (insert)
									{
										if (m_type == FE_BOOLEAN)
										{
											VARIANT v;
											::VariantInit(&v);
											v.vt = VT_BOOL;
											if ((wcscmp(valueDef.bstrVal, L"True") == 0) || (wcscmp(valueDef.bstrVal, L"TRUE")== 0))
											{
												v.boolVal = -1; // True
											}
											else
											{
												v.boolVal = 0;  // False
											}
											pbRes->AddProperty(m_objDefAlias, v, v,m_type);
											::VariantClear(&v);
										}
										else
										{
											pbRes->AddProperty(m_objDefAlias, valueDef, valueDef,m_type);
										}
									}
									insert = true;
								}
								pEnum2->Release();

								::VariantClear(&m_objValue);
								::VariantClear(&vIn1);
								pEnum1->Next(1,&vIn1,&num1);
								::VariantClear(&m_objValue);

							} //end While

							::VariantClear(&valueDef);
							pEnum1->Release();
						}
					} //end if ==
					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				} //end While
			}
			
			pEnum->Release();

		}
		pChild->Release();

	}

	::SysFreeString(m_objDefAlias);
	::SysFreeString(m_objName);
	::SysFreeString(m_objAlias);

}

STDMETHODIMP Cveranle::get_ErrorCount(long * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// m_nerror es la variable miembro donde se encuentra
	// el número de errores déspues del análisis.

	*pVal= m_nerror;

	return S_OK;
}

STDMETHODIMP Cveranle::put_ErrorCount(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	
	m_nerror = newVal;
	return S_OK;
}

// Esta función comprueba para un objeto, y propiedad 
// y el valor de dicha propiedad si se cumple todos
// los constraint relacionados con la misma.

STDMETHODIMP Cveranle::get_AnalyzeProperty(BSTR Object, BSTR Property, BSTR Value, short * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CAnalyzeProperty VAnalyze;

	VAnalyze.CargarPB(m_pPBTypes, m_pPBPropertiesgroup, m_pPBObjects);

	*pVal = VAnalyze.AnalyzeProp(Object,Property,Value);

	return S_OK;
}


STDMETHODIMP Cveranle::put_Errorlevel(short newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	m_Errorlevel = newVal;

	return S_OK;
}

STDMETHODIMP Cveranle::get_CompileMode(BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP Cveranle::put_CompileMode(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	USES_CONVERSION;

	// TODO: Add your implementation code here
	//M4BSTRToCString(newVal, mira);

	return S_OK;
}

STDMETHODIMP Cveranle::get_ResultPB(ICPropBag * * PB, short * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP Cveranle::InitLang(long nLang, _Collection * colLang)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	VARIANT * pVar = new VARIANT;

	HRESULT hrres = S_OK;
	
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (colLang)
	{
		HRESULT hr = colLang->_NewEnum(&pUnk);
		if ((hr == S_OK) && (pUnk != NULL))
		{
			hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
			if ((hr == S_OK) && (pEnum != NULL))
			{
				// Inicializamos el Array con nLang elementos
				SAFEARRAYBOUND sab[1];
				sab[0].cElements = nLang;
				sab[0].lLbound = 1;
				SAFEARRAY * psa = SafeArrayCreate(VT_BSTR, 1, sab);
				VariantClear(&m_vInitialize);
				m_vInitialize.vt = VT_ARRAY | VT_BSTR;
				m_vInitialize.parray = psa;
				BSTR * pBstr = (BSTR*)psa->pvData;
								 
				pEnum->Reset();
				hrres = S_OK;
				for (int i = 0; i < nLang; i++)
				{
					VariantInit(pVar);
					hr = pEnum->Next(1, pVar, NULL);
					if((hr == S_OK) && (pVar->vt == VT_BSTR))
					{
						pBstr[i] = ::SysAllocString(pVar->bstrVal);
					}
					else
					{
						hrres = E_INVALIDARG;
					}
					::VariantClear(pVar);
				}
				RELEASE(pEnum);
			}
			RELEASE(pUnk);
		}
	}	
	if (hrres == S_OK) m_nLangNumber = nLang;
	delete(pVar);
	return hrres;

}

STDMETHODIMP Cveranle::get_IsControl(BSTR bstrClassName, VARIANT_BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_FALSE;
	*pVal = 0;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
	
	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			CComVariant v(bstrClassName);
			hr = pChildCol->get_Item(v, &pPBObject);
			if (hr == S_OK)
			{
				BSTR bstrTemp = ::SysAllocString(L"Control");
				hr = pPBObject->Property(bstrTemp, &CComVariant(0),&v);
				::SysFreeString(bstrTemp);
				if (hr == S_OK)
				{
					if (v.vt == VT_BOOL)
					{
						*pVal = v.boolVal;
					}
					else if (v.vt == VT_BSTR)
					{
						if ((wcscmp(v.bstrVal, L"True") == 0) || (wcscmp(v.bstrVal, L"TRUE") == 0))
						{
							*pVal = -1;
						}
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}

STDMETHODIMP Cveranle::get_IconString(BSTR bstrClassName, BSTR * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	
	if ((pVal == NULL) || (bstrClassName == NULL)) return E_INVALIDARG;

	HRESULT hr = S_FALSE;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
	
	if (*pVal)
	{
		::SysFreeString(*pVal);
	}
	*pVal = ::SysAllocString(L"ID_GENERIC_OBJECT_ICON");

	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			CComVariant v(bstrClassName);
			hr = pChildCol->get_Item(v, &pPBObject);
			if (hr == S_OK)
			{
				ICSingleProp * pSP = NULL;
				BSTR bstrTemp = ::SysAllocString(L"Icon");
				hr = pPBObject->Property(bstrTemp, &CComVariant(0), &v);
				::SysFreeString(bstrTemp);
				if (hr == S_OK)
				{
					if (v.vt == VT_BSTR)
					{
						::SysFreeString(*pVal);
						*pVal = ::SysAllocString(v.bstrVal);
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}

STDMETHODIMP Cveranle::get_HideFromInsert(BSTR bstrClassName, VARIANT_BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_FALSE;
	*pVal = 0;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			CComVariant v(bstrClassName);
			hr = pChildCol->get_Item(v, &pPBObject);
			if (hr == S_OK)
			{
				ICSingleProp * pSP = NULL;
				BSTR bstrTemp = ::SysAllocString(L"HideFromInsert");
				hr = pPBObject->Property(bstrTemp, &CComVariant(0),&v);
				::SysFreeString(bstrTemp);
				if (hr == S_OK)
				{
					if (v.vt == VT_BOOL)
					{
						*pVal = v.boolVal;
					}
					else if (v.vt == VT_BSTR)
					{
						if ((wcscmp(v.bstrVal, L"True") == 0) || (wcscmp(v.bstrVal, L"TRUE") == 0))
						{
							*pVal = -1;
						}
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}

STDMETHODIMP Cveranle::get_UniqueUID(BSTR bstrClassName, VARIANT_BOOL * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_FALSE;
	*pVal = 0;

	
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			CComVariant v(bstrClassName);
			hr = pChildCol->get_Item(v, &pPBObject);
			if (hr == S_OK)
			{
				ICSingleProp * pSP = NULL;
				BSTR bstrTemp = ::SysAllocString(L"Uniqueuid");
				hr = pPBObject->Property(bstrTemp, &CComVariant(0),&v);
				::SysFreeString(bstrTemp);
				if (hr == S_OK)
				{
					if (v.vt == VT_BOOL)
					{
						*pVal = v.boolVal;
					}
					else if (v.vt == VT_BSTR)
					{
						if ((wcscmp(v.bstrVal, L"True") == 0) || (wcscmp(v.bstrVal, L"TRUE") == 0))
						{
							*pVal = -1;
						}
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}

STDMETHODIMP Cveranle::MaxMinNumberOfObjects(BSTR bstrClassName, BSTR ClassNameChild, long * pValMax, long * pValMin)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_FALSE;
	*pValMin = 0;
	*pValMax = 0;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
	
	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pChildObject = NULL;
		ICPropBag * pChild = NULL;
		ICChildCol * pPBObject = NULL;
		BSTR ClassName;	

		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			CComVariant v(bstrClassName);
			hr = pChildCol->get_Item(v, &pChildObject);
			if (hr == S_OK)
			{
				//tengo el objeto y quiero sus objetos contenidos
				hr = pChildObject->get_ChildCol(&pPBObject);
				if (hr == S_OK)
				{	
					long m_count, i = 1;
					bool end = false;
					pPBObject->get_Count(&m_count);
					
					while ((!end) && (i <= m_count))
					{

						hr = pPBObject->get_Item(CComVariant(i), &pChild);

						ClassName = NULL;
						hr = pChild->get_ClassName(&ClassName);
						if (wcscmp(ClassName, L"Contains") == 0)
						{	
							// Tenemos un Contenedor
							// Buscamos si coincide con el que nos piden y si tiene MaxInstances
							ICPropCol * pPropCol = NULL;
							ICSingleProp    * pSP = NULL;
							BSTR           Name;
							CComVariant    varValue;

							hr = pChild->get_PropCol(&pPropCol);
							if (hr == S_OK)
							{
								long countProp = 0, j;
								Name = NULL;
								pPropCol->get_Count(&countProp);

								for (j = 1;j <= countProp;j++)
								{
									hr = pPropCol->get_Item(CComVariant(j), &pSP);
									pSP->get_Name(&Name);
									pSP->get_Value(&varValue);
									if ((wcscmp(Name, L"Object") == 0) && (varValue == CComVariant(ClassNameChild)))
									{
										end = true;
									}
									
									if (end)
									{
										if (wcscmp(Name, L"Maxinstances") == 0)
										{
											if (varValue.vt == VT_I4)
											{
												*pValMax = varValue.lVal;
											}
											else if (varValue.vt == VT_I2)
											{
												*pValMax = (long) varValue.iVal;
											}
										}

										if (wcscmp(Name, L"Mininstances") == 0)
										{
											if (varValue.vt == VT_I4)
											{
												*pValMin = varValue.lVal;
											}
											else if (varValue.vt == VT_I2)
											{
												*pValMin = (long) varValue.iVal;
											}
										}

									}
								}
								
								::SysFreeString(Name);
								pSP->Release();
							}
						
						}
						::SysFreeString(ClassName);
						i++;
					}

					pChild->Release();
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}



STDMETHODIMP Cveranle::get_InToolbar(BSTR bstrClassName, VARIANT_BOOL * pVal)
{
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_FALSE;
	*pVal = 0;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			CComVariant v(bstrClassName);
			hr = pChildCol->get_Item(v, &pPBObject);
			if (hr == S_OK)
			{
				BSTR bstrTemp = ::SysAllocString(L"Intoolbar");
				hr = pPBObject->Property(bstrTemp, &CComVariant(0), &v);
				::SysFreeString(bstrTemp);
				if (hr == S_OK)
				{
					if (v.vt == VT_BOOL)
					{
						*pVal = v.boolVal;
					}
					else if (v.vt == VT_BSTR)
					{
						if ((wcscmp(v.bstrVal, L"True") == 0) || (wcscmp(v.bstrVal, L"TRUE") == 0))
						{
							*pVal = -1;
						}
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}

BOOL Cveranle::IsInGroup(BSTR bstrGroupName, BSTR bstrClassName)
{
	BOOL	result = 0; // No encontrada
	
	ICPropBag * pPB = NULL;
	ICPropBag * pPB2 = NULL;
	ICChildCol * pChildCol = NULL;
	ICChildCol * pChildCol2 = NULL;
	ICPropCol * pPropCol = NULL;
	ICSingleProp * pSP = NULL;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBObjectsgroup)
	{
		HRESULT hr = m_pPBObjectsgroup->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{
			hr = pChildCol->get_Item(CComVariant(bstrGroupName), &pPB);
			if (hr == S_OK)
			{
				hr = pPB->get_ChildCol(&pChildCol2);
				if (hr == S_OK)
				{
					long nCount=0,i;	
					hr = pChildCol2->get_Count(&nCount);
					for (i = 1; i <= nCount; i++)
					{
						pChildCol2->get_Item(CComVariant(i), &pPB2);
						if (hr == S_OK)
						{
							// Comprobamos ClassName
							BSTR bstrTemp = NULL;
							pPB2->get_ClassName(&bstrTemp);
							if (wcscmp(bstrTemp, L"Contains") == 0)
							{
								// Tenemos objeto contenido
								if (TestObjectInPB(pPB2, bstrClassName))
								{
									pPB2->Release();	
									pChildCol2->Release();
									pPB->Release();
									pChildCol->Release();
									::SysFreeString(bstrTemp);
									return -1;
								}
							}
							::SysFreeString(bstrTemp);
							pPB2->Release();
						}
					}
					pChildCol2->Release();
				}
				pPB->Release();
			}
			pChildCol->Release();
		}
	}
	
	return 0;
}

STDMETHODIMP Cveranle::IsContained(BSTR bstrDestClassName, BSTR bstrContainedClassname, VARIANT_BOOL * pReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	HRESULT hr = S_FALSE;
	*pReturn = 0;
 
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (!bstrDestClassName || !bstrContainedClassname)
	{
		return E_INVALIDARG;
	}

	if (m_pPBObjects)
	{
		// Para gozo y disfrute del Juanma y esas estructuras no formalizadas
		if (wcscmp(bstrDestClassName, L"Contextblk") == 0)
		{
			*pReturn = -1;
			return S_OK;
		}
		
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			hr = pChildCol->get_Item(CComVariant(bstrDestClassName), &pPBObject);
			if (hr == S_OK)
			{
				// Buscamos dentro de las PB los Contains
				*pReturn = TestThisContainedObject(pPBObject, bstrContainedClassname);
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}

VARIANT_BOOL Cveranle::TestThisContainedObject(ICPropBag * pPB, BSTR bstrClassName)
{

	ICChildCol * pChildCol = NULL;
	ICPropBag * pPBChild = NULL;

	// Nos llega en pPBDef la CPropBagDef del Objecto.
	// Iteramos entre sus ChildColDef Hijas a ver si
	// Tiene una con ClassName Contains

	HRESULT hr = pPB->get_ChildCol(&pChildCol);
	if (hr == S_OK)
	{
		long nCount = 0, i;
		hr = pChildCol->get_Count(&nCount);
		for (i = 1; i <= nCount; i++)
		{
			
			hr = pChildCol->get_Item(CComVariant(i), &pPBChild);
			if (hr == S_OK)
			{
				// Comprobamos si su classname es Contains
				BSTR bstrClass = NULL;
				pPBChild->get_ClassName(&bstrClass);
				if (wcscmp(bstrClass, L"Contains") == 0)
				{
					if (TestObjectInPB(pPBChild, bstrClassName))
					{
						pPBChild->Release();
						pChildCol->Release();
						::SysFreeString(bstrClass);
						// Lo hemos encontrado
						return -1;
					}
				}
				::SysFreeString(bstrClass);
				pPBChild->Release();
			}
		}
		pChildCol->Release();
	}
	return 0;
}

BOOL Cveranle::TestObjectInPB(ICPropBag * pPB, BSTR bstrClassName)
{
	// Tenemos una que contiene
	// Sólo tiene una propiedad
	ICPropCol * pPC = NULL;

	HRESULT hr = pPB->get_PropCol(&pPC);
	if (hr == S_OK)
	{
		// Cojemos la primera propiedad
		ICSingleProp * pSP = NULL;
		hr = pPC->get_Item(CComVariant(1), &pSP);
		if (hr == S_OK)
		{
			CComVariant varValue;
			BSTR varName = NULL;
			pSP->get_Name(&varName);
			pSP->get_Value(&varValue);
			if (wcscmp(varName, L"Object") == 0)
			{
				// Nombre de Objeto
				if (varValue == CComVariant(bstrClassName))
				{
					pSP->Release();
					pPC->Release();
					::SysFreeString(varName);
					// Lo hemos encontrado
					return -1;
				}
			}
			if (wcscmp(varName, L"Objectgroup") == 0)
			{
				BSTR bstrGroup;
				if (varValue.vt == VT_BSTR)
				{
					bstrGroup = ::SysAllocString(varValue.bstrVal);
					if (IsInGroup(bstrGroup, bstrClassName))
					{
						pSP->Release();
						pPC->Release();
						::SysFreeString(bstrGroup);
						::SysFreeString(varName);
						// Lo hemos encontrado 
						return -1;
					}
					::SysFreeString(bstrGroup);
				}
			}
			::SysFreeString(varName);
			pSP->Release();
		}
		pPC->Release();
	}

	return 0;
}

void Cveranle::AddPropertyToPropBag(ICPropBagDef * pPBDest, ICPropBag * pPBProp)
{

	// Tenemos que hacer distinciones entre:
	// Propiedad ... La metemos sin preocuparnos de otra cosa
	// Aggregates ... Entramos a por ellos

	BSTR ClassName = NULL;
	HRESULT hr = pPBProp->get_ClassName(&ClassName);
	ICPropCol * pPropCol = NULL;
	
	
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (wcscmp(ClassName, L"Property") == 0)
	{	
		// Tenemos una propiedad
		// Buscamos por tipo
		hr = pPBProp->get_PropCol(&pPropCol);
		if (hr == S_OK)
		{
			BSTR bstrName = NULL;
			pPBProp->get_Alias(&bstrName);
			ProcessProperty(pPBDest, pPropCol, bstrName);
			::SysFreeString(bstrName);
			pPropCol->Release();
		}
	}
	else if(wcscmp(ClassName, L"Aggregates") == 0)
	{
		hr = pPBProp->get_PropCol(&pPropCol);
		if (hr == S_OK)
		{
			ICSingleProp *pAgg = NULL;
			hr = pPropCol->get_Item(CComVariant(1), &pAgg);
			if (hr == S_OK)
			{
				CComVariant vAggName;
				pAgg->get_Value(&vAggName);
				if (vAggName.vt == VT_BSTR)
				{
					ProcessAggregate(pPBDest, vAggName.bstrVal);
				}
				pAgg->Release();
			}
			pPropCol->Release();
		}
	}

	::SysFreeString(ClassName);
}

void Cveranle::ProcessProperty(ICPropBagDef * pPBDest, ICPropCol * pPropCol, BSTR bstrName)
{

	// Miramos si hay que añadir esta propiedad
	HRESULT hr = S_OK;
	ICSingleProp * pSP = NULL;
	
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	hr = pPropCol->get_Item(CComVariant(L"Type"), &pSP);
	if (hr == S_OK)
	{
		
		// Tenemos el Tipo
		CComVariant vType;
		hr = pSP->get_Value(&vType);

		// jcr Hay que liberar el objeto
		pSP->Release() ;
		pSP = NULL ;

		if (vType.vt == VT_BSTR)
		{
			BSTR bstrType = vType.bstrVal; // Sólo asignamos el puntero
			if (!(wcscmp(bstrType, L"NULL") == 0) || (wcscmp(bstrType, L"Null") == 0))
			{
				// El tipo es diferente de nulo
				// Podemos ir preparando una CProperty Para añadir
				CComVariant vValue;
				CComVariant vDefValue;
				M4PropertyType sType = FE_NOTDEFINED;

				if ((wcscmp(bstrType, L"Integer") ==0) || (wcscmp(bstrType, L"Long") ==0) || (wcscmp(bstrType, L"WorkdatesType") ==0) || (wcscmp(bstrType, L"GrantsType") ==0) || (wcscmp(bstrType, L"IdpresentationType") ==0) || (wcscmp(bstrType, L"TwovalueType") ==0) || (wcscmp(bstrType, L"FloatingType") ==0))
				{
					sType = FE_NUMBER;
				}
				else if ((wcscmp(bstrType, L"Bool") ==0))
				{
					sType = FE_BOOLEAN;
				}
				else if ((wcscmp(bstrType, L"String") ==0) || (wcscmp(bstrType, L"ActionType") ==0) || (wcscmp(bstrType, L"IdinstanceType") ==0) || (wcscmp(bstrType, L"Action_exeType") ==0) || (wcscmp(bstrType, L"Action_copyType") ==0) || (wcscmp(bstrType, L"MvcactionType") ==0) || (wcscmp(bstrType, L"Action_helpType") ==0) || (wcscmp(bstrType, L"StringUpType") ==0) || (wcscmp(bstrType, L"ListstringUpType") ==0) || (wcscmp(bstrType, L"ListString") ==0))
				{
					sType = FE_STRING;
				}
				else if ((wcscmp(bstrType, L"Color") ==0))
				{
					sType = FE_COLOR;
				}
				else if ((wcscmp(bstrType, L"TraductString") ==0))
				{
					sType = FE_TRADUCTPROP;
				}
				else if ((wcscmp(bstrType, L"Image") == 0))
				{
					sType = FE_IMAGE;
				}
				else if ((wcscmp(bstrType, L"AlignType") == 0))
				{
					sType = FE_ALIGNTYPE;
				}
				else if ((wcscmp(bstrType, L"LN4CodeArgs") == 0))
				{
					sType = FE_LN4_CODEARGS;
				}
				else if ((wcscmp(bstrType, L"Constructif") == 0))
				{
					sType = FE_CONSTRUCTIF;
				}
				else if ((wcscmp(bstrType, L"GrantsType") == 0))
				{
					sType = FE_GRANTSTYPE;
				}

				// Si seguimos sin tener tipo, remontamos a tipos
				if (sType == FE_NOTDEFINED)
				{
					ICPropBag * pPB = NULL;
					ICChildCol * pCC = NULL;
					if (m_pPBTypes->get_ChildCol(&pCC) == S_OK)
					{
						if (pCC->get_Item(CComVariant(bstrType), &pPB) == S_OK)
						{
							CComVariant vValue;
							pPB->Property(CComBSTR(L"Basetype"), &CComVariant(""), &vValue);
							if (vValue.vt == VT_BSTR && (wcscmp(vValue.bstrVal, L"") != 0))
							{
								// Comprobación de tipos básicos
								if ((wcsicmp(vValue.bstrVal, L"Integer") ==0) || (wcscmp(vValue.bstrVal, L"Long") ==0))
								{
									sType = FE_NUMBER;
								}
								else if (wcscmp(vValue.bstrVal, L"Bool") == 0)
								{
									sType = FE_BOOLEAN;
								}
								else if (wcscmp(vValue.bstrVal, L"String") == 0)
								{
									sType = FE_STRING;
								}
							}
							pPB->Release();
						}
						pCC->Release();
					}
				}

				// Ya tenemos el tipo, cojemos el valor por defecto
				ICSingleProp * pDefault = NULL;
				hr = pPropCol->get_Item(CComVariant(L"Default"), &pDefault);
				if (hr == S_OK)
				{
					pDefault->get_Value(&vValue);
					pDefault->get_Value(&vDefValue);
					pDefault->Release();
				}

				// Vamos a añadirla

				if (sType == FE_TRADUCTPROP)
				{
					ITraductProp * pTraduct = NULL;
					IDispatch * pDisp = NULL;
					// Creamos la TraductProp y la insertamos en su sitio
					hr = CoCreateInstance(CLSID_TraductProp, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void**)&pDisp);
					hr = pDisp->QueryInterface(IID_ITraductProp, (void**)&pTraduct);
					pTraduct->InitFromBSTRArray(m_vInitialize);
					pTraduct->put_Property(bstrName);
					vValue = pDisp;
					pTraduct->Release();
					pDisp->Release();
				}

				if (sType == FE_BOOLEAN)
				{
					if (vValue.vt != VT_BOOL)
					{
						if (vValue.vt == VT_BSTR)
						{
							if ((wcscmp(vValue.bstrVal, L"TRUE") == 0) || (wcscmp(vValue.bstrVal, L"True") == 0))
							{
								// Sólo si es True será True
								vValue = true;
								vDefValue = true;
							}
							else
							{
								// Colocamos un False
								vValue = false;
								vDefValue = false;
							}
						}
						else
						{
							// Colocamos un False
							vValue = false;
							vDefValue = false;
						}
					}
				}
			
				// Para Dentro
				pPBDest->AddProperty(bstrName, vValue, vDefValue, sType);	
			}
		}
	}

}

void Cveranle::ProcessAggregate(ICPropBagDef * pPBDest, BSTR bstrAggName)
{

	HRESULT hr;
	// Empezaremos buscando los conocidos
	// DataProps y Font

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (wcscmp(bstrAggName, L"Dataprop") == 0)
	{
		// Marchando una de objeto DataProps
		CComVariant vValue, vDefValue;

		IDispatch * pDisp = NULL;
		hr = CoCreateInstance(CLSID_DataProperties, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void**)&pDisp);
		if (hr == S_OK)
		{
			vValue = pDisp;
			//vDefValue = pDisp;
			pPBDest->AddProperty(CComBSTR(L"DataProps"), vValue, vDefValue, FE_DATAPROP);
			pDisp->Release();
		}
	}
	else if (wcscmp(bstrAggName, L"Fontgroup") == 0)
	{
		// Marchando un objeto M4FontHolder
		CComVariant vValue, vDefValue;

		IDispatch * pDisp = NULL;
		IDispatch * pDisp2 = NULL;
		hr = CoCreateInstance(CLSID_M4FontHolder, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void**)&pDisp);
		hr = CoCreateInstance(CLSID_M4FontHolder, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void**)&pDisp2);
		if (hr == S_OK)
		{
			vValue = pDisp;
			vDefValue = pDisp2;
			pPBDest->AddProperty(CComBSTR(L"Font"), vValue, vDefValue, FE_FONT);
			pDisp->Release();
			pDisp2->Release();
		}
	}
	else if (wcscmp(bstrAggName, L"Fontlinkgroup") == 0)
	{
		// Marchando un objeto M4FontHolder
		CComVariant vValue, vDefValue;

		IDispatch * pDisp = NULL;
		IDispatch * pDisp2 = NULL;
		IM4FontHolder * pM4FontDisp = NULL;
		IM4FontHolder * pM4FontDisp2 = NULL;
		hr = CoCreateInstance(CLSID_M4FontHolder, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void**)&pDisp);
		hr = CoCreateInstance(CLSID_M4FontHolder, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void**)&pDisp2);
		if (hr == S_OK)
		{
			vValue = pDisp;
			vDefValue = pDisp2;
			hr = pDisp->QueryInterface(IID_IM4FontHolder, (void**)&pM4FontDisp);
			hr = pDisp2->QueryInterface(IID_IM4FontHolder, (void**)&pM4FontDisp2);
			
			pM4FontDisp->put_FontBold(L"True");
			pM4FontDisp->put_FontUnderline(L"True");			
			pM4FontDisp2->put_FontBold(L"True");
			pM4FontDisp2->put_FontUnderline(L"True");			

			pPBDest->AddProperty(CComBSTR(L"Font"), vValue, vDefValue, FE_FONT);
			
			pDisp->Release();
			pDisp2->Release();
			pM4FontDisp->Release();
			pM4FontDisp2->Release();
		}
	}
	else
	{
		// Hay que buscarlo en los grupos de propiedades
		ICChildCol *pChildCol = NULL;
		hr = m_pPBPropertiesgroup->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{
			ICPropBag * pPBAgg = NULL;
			hr = pChildCol->get_Item(CComVariant(bstrAggName), &pPBAgg);
			if (hr == S_OK)
			{
				// Hay que iterar por sus PropertyBags
				long nLong, i;
				ICChildCol *pChildCol2 = NULL;
				hr = pPBAgg->get_ChildCol(&pChildCol2);
				if (hr == S_OK)
				{
					hr = pChildCol2->get_Count(&nLong);
					for (i = 1; i<= nLong; i++)
					{
						ICPropBag * pPBAgg2 = NULL;
						hr = pChildCol2->get_Item(CComVariant(i), &pPBAgg2);
						if (hr == S_OK)
						{
							AddPropertyToPropBag(pPBDest, pPBAgg2);								
							pPBAgg2->Release();
						}
					}
					pChildCol2->Release();					
				}
				pPBAgg->Release();
			}
			pChildCol->Release();
		}
	}
}

STDMETHODIMP Cveranle::GetContainedObjectsList(BSTR bstrClassName, ICPropBag * * ppRetVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_OK;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
	
	if (bstrClassName)
	{
		ICChildCol * pChildCol = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{
			hr = CoCreateInstance(CLSID_CPropBag, NULL, CLSCTX_INPROC_SERVER, IID_ICPropBag, (void**)ppRetVal);
			if (hr == S_OK)
			{
				long nCount,i;
				pChildCol->get_Count(&nCount);
				for (i = 1; i<=nCount;i++)
				{
					ICPropBag * pPB = NULL;
					hr = pChildCol->get_Item(CComVariant(i), &pPB);
					if (hr == S_OK)
					{
						BSTR bstrClass = NULL;
						pPB->get_Alias(&bstrClass);
						// Ya tenemos el ClassName del primero
						VARIANT_BOOL bRet = -1;
						hr = IsContained(bstrClassName, bstrClass, &bRet);
						if ((hr == S_OK) && (bRet))
						{	
							// Comprobamos que sea visualizable en esta Ventana
							hr = get_HideFromInsert(bstrClass, &bRet);
							if (hr == S_OK)
							{
								if (bRet == 0)
								{
									(*ppRetVal)->AddProperty(bstrClass, CComVariant(bstrClass));
								}
							}
						}	
						pPB->Release();
						::SysFreeString(bstrClass);
					}
				}
			}
			pChildCol->Release();
		}
	}
	return hr;
}

BOOL Cveranle::IsEvent(BSTR ClassName)
{

	BOOL nRet = 0; // False

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		HRESULT hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			hr = pChildCol->get_Item(CComVariant(ClassName), &pPBObject);
			if (hr == S_OK)
			{
				ICSingleProp * pSP = NULL;
				BSTR bstrTemp = ::SysAllocString(L"Event");
				CComVariant v;
				
				hr = pPBObject->Property(bstrTemp, &CComVariant(0), &v);
				::SysFreeString(bstrTemp);
				if (hr == S_OK)
				{
					if (v.vt == VT_BOOL)
					{
						nRet = v.boolVal;
					}
					else if (v.vt == VT_BSTR)
					{
						if ((wcscmp(v.bstrVal, L"True") == 0) || (wcscmp(v.bstrVal, L"TRUE") == 0))
						{
							nRet = -1;
						}
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}
	return nRet;

}

long Cveranle::GetResourceByClassName(BSTR ClassName)
{

	long nRet = 0;
	
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		HRESULT hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			hr = pChildCol->get_Item(CComVariant(ClassName), &pPBObject);
			if (hr == S_OK)
			{
				BSTR bstrTemp = ::SysAllocString(L"ResourceString");
				CComVariant vValue;
				hr = pPBObject->Property(bstrTemp, &CComVariant("NOT_DEFINED"), &vValue);
				::SysFreeString(bstrTemp);
				if (hr == S_OK)
				{
					if (vValue.vt == VT_I4)
					{
						nRet = vValue.lVal;
					}
					else if (vValue.vt == VT_I2)
					{
						nRet = vValue.iVal;
					}
					else if (vValue.vt == VT_UI2)
					{
						nRet = vValue.bVal;
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}
	return nRet;

}

STDMETHODIMP Cveranle::IsEvent(BSTR ClassName, VARIANT_BOOL * pResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	*pResult = (short)IsEvent(ClassName);
	return S_OK;
}

STDMETHODIMP Cveranle::get_ObjectPB(ICPropBag * * pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
		
	*pVal = m_pPBObjects;
	if (m_pPBObjects)
	{
		m_pPBObjects->AddRef();
	}

	return S_OK;
}

HRESULT Cveranle::GetFromCache(BSTR ClassName, ICPropBagDef * * pPB)
{
	ICPropBagDef * pPBInit;
	ICPropColDef * pPC;
	
	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBCache)
	{
		CComVariant vAux;
		HRESULT hr = m_pPBCache->Property(ClassName, &CComVariant(0), &vAux);
		if (hr == S_OK && vAux.vt == VT_DISPATCH)
		{
			hr = ((IDispatch*)(vAux.pdispVal))->QueryInterface(IID_ICPropBagDef, (void**)&pPBInit);
			if (hr == S_OK)
			{
				// Enganchamos con la definitiva
				if ((*pPB) == NULL)
				{
					hr = CoCreateInstance(CLSID_CPropBagDef, NULL, CLSCTX_INPROC_SERVER, IID_ICPropBagDef, (void**)pPB);
				}
				else
				{
					// Eliminamos sus contenidos de propiedades
					hr = (*pPB)->get_PropColDef(&pPC);
					if (hr == S_OK)
					{
						pPC->RemoveAll();
						pPC->Release();
					}
				}
				// Copiamos todas las propiedades de pPBInit a pPB
				CopyProperties(*pPB, pPBInit);
				pPBInit->Release();
				// Le informamos de qué objeto se trata
				(*pPB)->put_ClassName(ClassName);
				(*pPB)->put_Alias(ClassName);
				return S_OK;
			}
		}
	}
	return E_FAIL;
}

void Cveranle::CopyProperties(ICPropBagDef * pbDest, ICPropBagDef * pbSource)
{

	// Se copian a pelo
	ICDefProp * pDefProp = NULL;
	ICPropColDef * pPropColDef = NULL;

	if (pbSource && pbDest)
	{

		HRESULT hr = pbSource->get_PropColDef(&pPropColDef);
		if (hr == S_OK)
		{
			// Marchando una de enumerador
			IEnumVARIANT	* pEnum = NULL;
			IUnknown		* pUnk = NULL;
			hr = pPropColDef->get__NewEnum(&pUnk);
			if (hr == S_OK)
			{
				hr = pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum);
				if (hr == S_OK)
				{
					unsigned long num = 0;
					CComVariant vIn, vValue, vDefValue;
					ICDefProp * pDefNew = NULL;
					ICPropColDef * pPropColDef = NULL;
					M4PropertyType nType;
					BSTR bstrName = NULL;
					hr = pbDest->get_PropColDef(&pPropColDef);
					if (hr == S_OK)
					{
						pEnum->Reset();
						pEnum->Next(1,&vIn,&num);
						while ((vIn.vt==VT_DISPATCH)&&(num==1))
						{
							hr = ((IDispatch*)(vIn.pdispVal))->QueryInterface(IID_ICDefProp, (void**)&pDefProp);
							if (hr == S_OK)
							{	
								HRESULT hr2 = E_FAIL; 
								::SysFreeString(bstrName);
								bstrName = NULL;
								pDefProp->get_Name(&bstrName);
								pDefProp->get_Value(&vValue);
								pDefProp->get_DefValue(&vDefValue);
								pDefProp->get_Type(&nType);
								pDefProp->Release();
								// Hay que controlar que los Font, Traduct, y DataProps
								switch (nType) 
								{
									case FE_FONT:

										hr = CloneM4FontHolder(&vValue);
										hr2 = CloneM4FontHolder(&vDefValue);

										if ((hr == S_OK) && (hr2 == S_OK))
										{
											pbDest->AddProperty(bstrName, vValue, vDefValue, nType);
										}
										
										break;
									case FE_DATAPROP:
										// Solo se Clona el Value
										hr = CloneDataProps(&vValue);
										if (hr == S_OK)
											pbDest->AddProperty(bstrName, vValue, vDefValue, nType);
										break;
									case FE_TRADUCTPROP:
										// Sólo se Clona el Value
										hr = CloneTraductProp(&vValue);
										if (hr == S_OK)
											pbDest->AddProperty(bstrName, vValue, vDefValue, nType);
										break;
									
									default:
										// Tal Cual
										pbDest->AddProperty(bstrName, vValue, vDefValue, nType);
										break;
								}
							}
							vIn.Clear();
							vValue.Clear();
							vDefValue.Clear();
							pEnum->Next(1,&vIn,&num);
						}
						::SysFreeString(bstrName);
						pEnum->Release();
					}
					pPropColDef->Release();
				}
				pUnk->Release();
			}			
			pPropColDef->Release();
		}

	}

}

void Cveranle::CopyDefProp2PBD(ICPropBagDef * pbDest, ICDefProp * pSource)
{
	ICDefProp * pDestProp = NULL;
	CComVariant vValue;
	CComVariant vDefValue;
	M4PropertyType nType;
	
	if (pbDest && pSource)
	{
		BSTR bstrName = NULL;
		HRESULT hr = pSource->get_Name(&bstrName);
		if (hr == S_OK)
		{
			pSource->get_Value(&vValue);
			pSource->get_DefValue(&vDefValue);
			pSource->get_Type(&nType);
			pbDest->AddProperty(bstrName, vValue, vDefValue, nType);
		}
		::SysFreeString(bstrName);
	}
}

HRESULT Cveranle::CloneM4FontHolder(CComVariant * pComVariant)
{
	IM4FontHolder * pFontValue, * pFontValueTemp;
	IDispatch * pDisp;
	HRESULT hr = E_FAIL;

	if (pComVariant->vt == VT_DISPATCH)
	{
		hr = ((IDispatch*)(pComVariant->pdispVal))->QueryInterface(IID_IM4FontHolder, (void**)&pFontValue);
		if (hr == S_OK)
		{
			hr = pFontValue->Clone(&pFontValueTemp);
			if (hr == S_OK)
			{
				// Metemos la copia en el vValue
				if (pFontValueTemp->QueryInterface(IID_IDispatch, (void**)&pDisp) == S_OK)
				{
					(*pComVariant) = pDisp;
					pDisp->Release();
				}
				pFontValueTemp->Release();
			}
			pFontValue->Release();
		}
	}

	return hr;
}

HRESULT Cveranle::CloneDataProps(CComVariant * pComVariant)
{
	IDataProperties * pValue, * pValueTemp;
	IDispatch * pDisp;
	HRESULT hr = E_FAIL;

	if (pComVariant->vt == VT_DISPATCH)
	{
		hr = ((IDispatch*)(pComVariant->pdispVal))->QueryInterface(IID_IDataProperties, (void**)&pValue);
		if (hr == S_OK)
		{
			hr = pValue->Clone(&pValueTemp);
			if (hr == S_OK)
			{
				// Metemos la copia en el vValue
				if (pValueTemp->QueryInterface(IID_IDispatch, (void**)&pDisp) == S_OK)
				{
					(*pComVariant) = pDisp;
					pDisp->Release();
				}
				pValueTemp->Release();
			}
			pValue->Release();
		}
	}

	return hr;

}

HRESULT Cveranle::CloneTraductProp(CComVariant * pComVariant)
{
	ITraductProp * pValue = NULL, * pValueTemp = NULL;
	IDispatch * pDisp;
	HRESULT hr = E_FAIL;

	if (pComVariant->vt == VT_DISPATCH)
	{
		hr = ((IDispatch*)(pComVariant->pdispVal))->QueryInterface(IID_ITraductProp, (void**)&pValue);
		if (hr == S_OK)
		{
			hr = pValue->CloneClean(&pValueTemp);
			if (hr == S_OK)
			{
				// Metemos la copia en el vValue
				if (pValueTemp->QueryInterface(IID_IDispatch, (void**)&pDisp) == S_OK)
				{
					(*pComVariant) = pDisp;
					pDisp->Release();
				}
				pValueTemp->Release();
			}
			pValue->Release();
		}
	}

	return hr;

}

STDMETHODIMP Cveranle::InitLangFromArray(VARIANT vArray)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	
	if (vArray.vt == (VT_ARRAY | VT_BSTR))
	{
		BSTR * pBstrOrg = (BSTR*)vArray.parray->pvData;
		
		// Inicializamos el Array con nLang elementos
		SAFEARRAYBOUND sab[1];
		sab[0].cElements = vArray.parray->rgsabound[0].cElements;
		sab[0].lLbound = 1;
		SAFEARRAY * psa = SafeArrayCreate(VT_BSTR, 1, sab);
		VariantClear(&m_vInitialize);
		m_vInitialize.vt = VT_ARRAY | VT_BSTR;
		m_vInitialize.parray = psa;
		BSTR * pBstr = (BSTR*)psa->pvData;
		m_nLangNumber = vArray.parray->rgsabound[0].cElements;
		for (long i = 0; i < m_nLangNumber; i++)
		{
			pBstr[i] = ::SysAllocString(pBstrOrg[i]);
		}
		return S_OK;
	}
	return E_INVALIDARG;
}

STDMETHODIMP Cveranle::InitFromResource(long idResource)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	USES_CONVERSION;
		
	HRSRC siteResource = NULL; 
	IConvertPropBag *convertPB = NULL;
	SAFEARRAY *auxArray = NULL;
	SAFEARRAYBOUND resourceArray;
	void * pData = NULL;
	HRESULT hr = 0;

	ICChildCol *pCC = NULL;
	ICPropBag *pPBAux = NULL;

	unsigned long num = 0;

	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	VARIANT	vIn;
	
	ICPropCol *pPC = NULL;

	if (m_pPB != NULL)
		return E_FAIL;

	// Leemos de recursos el metalenguaje con el IdResource
	siteResource = FindResource(AfxGetInstanceHandle(),MAKEINTRESOURCE(idResource), "METALENG");
	HGLOBAL res = LoadResource(AfxGetInstanceHandle(),siteResource);
	resourceArray.cElements = SizeofResource(AfxGetInstanceHandle(),siteResource);
	resourceArray.lLbound = 0;
	hr = CoCreateInstance(CLSID_ConvertPropBag, NULL, CLSCTX_ALL, IID_IConvertPropBag, (void**)&convertPB);
	if (hr == S_FALSE)
		return hr;

	auxArray = SafeArrayCreate(VT_UI1,1,&resourceArray);
	hr = SafeArrayAccessData(auxArray, (void**)&pData);
	memcpy(pData, res, resourceArray.cElements);

	if (hr == S_OK)
	{
		hr = convertPB->Array2PB(&auxArray,&m_pPB);
		// jcr Hay que liberar el objeto
		convertPB->Release() ;
		convertPB = NULL ;
	}

	SafeArrayUnaccessData(auxArray);
	SafeArrayDestroy(auxArray);

	if (hr == S_OK)
	{
		// Tenemos la Property Bag
		BSTR ClassName = NULL;
					
		if (m_pPB)
		{
			HRESULT hr = m_pPB->get_ChildCol((ICChildCol**)&pCC);
			if (hr == S_OK)
			{
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
						while ((vIn.vt==VT_DISPATCH)&&(num==1))
						{
							pPBAux=(ICPropBag*)vIn.pdispVal;

							hr = pPBAux->get_ClassName(&ClassName);

							CString m_class;
							M4BSTRToCString(ClassName, m_class);

							::SysFreeString(ClassName);
							ClassName = NULL;
							
							if (strcmp(m_class,"Types") == 0)
							{
								// m_pPBTypes es la propertyBag de tipos.
								m_pPBTypes = pPBAux;
								m_pPBTypes->AddRef();

							}
							else if  (strcmp(m_class,"Propertygroups") == 0)
							{
								// m_pPBPropertiesgroup es la propertyBag de propiedades.
								m_pPBPropertiesgroup = pPBAux;
								m_pPBPropertiesgroup->AddRef();

							}
							else if  (strcmp(m_class,"Objectgroups") == 0)
							{
								// m_pPBObjectsgroup es la propertyBag de objetos.
								m_pPBObjectsgroup = pPBAux;
								m_pPBObjectsgroup->AddRef();

							}
							else if  (strcmp(m_class ,"Objects") == 0)
							{
								// m_pPBObjects es la propertyBag de Objetos
								m_pPBObjects = pPBAux;
								m_pPBObjects->AddRef();

							}else
							
							::VariantClear(&vIn);
							pEnum->Next(1,&vIn,&num);

						}
						// jcr Hay que liberar el objeto
						::VariantClear(&vIn);

						pEnum->Release();
					}
					pCC->Release();
				}
			}
		}
		::SysFreeString(ClassName);
	}

	return hr;
}

STDMETHODIMP Cveranle::get_MetalengResourceID(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_nMetalengResourceID;

	return S_OK;
}

STDMETHODIMP Cveranle::put_MetalengResourceID(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_nMetalengResourceID = newVal;
		
	return S_OK;
}

STDMETHODIMP Cveranle::GetStringFromResource(long nResourceID, BSTR *pVal)
{
	USES_CONVERSION;
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	
	memset(m_pBuffer, 0 , RESOURCE_BUFFER_SIZE * sizeof( unsigned short ));
	LoadStringW(::AfxGetInstanceHandle() , nResourceID, (LPWSTR) m_pBuffer, RESOURCE_BUFFER_SIZE);
	if (m_pBuffer[0])
	{
		*pVal = SysAllocString((LPWSTR) m_pBuffer);
	}

	return S_OK;
}

STDMETHODIMP Cveranle::GetPropertyGroups(ICPropBag **pRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
		
	*pRet = m_pPBPropertiesgroup;
	if (m_pPBPropertiesgroup)
	{
		m_pPBPropertiesgroup->AddRef();
	}

	return S_OK;
}

STDMETHODIMP Cveranle::GetObjectGroups(ICPropBag **pRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
		
	*pRet = m_pPBObjectsgroup;
	if (m_pPBObjectsgroup)
	{
		m_pPBObjectsgroup->AddRef();
	}

	return S_OK;
}

STDMETHODIMP Cveranle::GetTypesGroups(ICPropBag **pRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}
		
	*pRet = m_pPBTypes;
	if (m_pPBTypes)
	{
		m_pPBTypes->AddRef();
	}

	return S_OK;
}

STDMETHODIMP Cveranle::get_TemplateOnly(BSTR bstrClassName, VARIANT_BOOL *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRESULT hr = S_FALSE;
	*pVal = 0;

	if (!IsIniMetaLenguage())
	{
		InitFromResource(m_nMetalengResourceID);
	}

	if (m_pPBObjects)
	{
		ICChildCol * pChildCol = NULL;
		ICPropBag * pPBObject = NULL;
		hr = m_pPBObjects->get_ChildCol(&pChildCol);
		if (hr == S_OK)
		{	
			hr = pChildCol->get_Item(CComVariant(bstrClassName), &pPBObject);
			if (hr == S_OK)
			{
				CComVariant v;				
				hr = pPBObject->Property(CComBSTR(L"Templateonly"), &CComVariant(0), &v);
				if (hr == S_OK)
				{
					if (v.vt == VT_BOOL)
					{
						*pVal = v.boolVal;
					}
					else if (v.vt == VT_BSTR)
					{
						if (wcsicmp(v.bstrVal, L"true") == 0)
						{
							*pVal = -1;
						}
					}
				}
				pPBObject->Release();
			}
			pChildCol->Release();
		}
	}

	return S_OK;
}

STDMETHODIMP Cveranle::get_ErrorLanguage(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Devolvemos el LCID establecido
	if (pVal) *pVal = m_pTranslate->GetLanguage();

	return S_OK;
}

STDMETHODIMP Cveranle::put_ErrorLanguage(long pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	HRSRC siteResMsg = NULL; 
	IConvertPropBag *convertPB = NULL;
	SAFEARRAY *auxMsgArray = NULL;
	SAFEARRAYBOUND  ResMsgArray;
	void *pDataMsg = NULL;
	HRESULT hr = 0, hr1 = 0;
	long lOldLCID = m_pTranslate->GetLanguage();

	m_pTranslate->SetLanguage(pVal);

	// Sólo se (re)cargan los mensajes de error en el caso en el que realmente cambie
	// el lenguaje
	if (lOldLCID != m_pTranslate->GetLanguage())
	{
		if (m_pPBMessages != NULL)
		{
			// Si ya había sido inicializado se libera
			m_pPBMessages->Release();
			m_pPBMessages = NULL;
		}

		// Se recargan los mensajes de error
		hr = CoCreateInstance(CLSID_ConvertPropBag, NULL, CLSCTX_ALL, IID_IConvertPropBag, (void**)&convertPB);
		
		// Leemos los mensajes de error
		siteResMsg = m_pTranslate->M4FindResource(AfxGetInstanceHandle(), IDR_METALENGMSG, "METALENG");
		HGLOBAL resMsg = LoadResource(AfxGetInstanceHandle(),siteResMsg);
		ResMsgArray.cElements = SizeofResource(AfxGetInstanceHandle(),siteResMsg);
		ResMsgArray.lLbound = 0;

		auxMsgArray = SafeArrayCreate(VT_UI1,1,&ResMsgArray);
		hr1 = SafeArrayAccessData(auxMsgArray, (void**)&pDataMsg);
		memcpy(pDataMsg, resMsg, ResMsgArray.cElements);

		if ((hr == S_OK) && (hr1 == S_OK))
		{
			hr1 = convertPB->Array2PB(&auxMsgArray,&m_pPBMessages);
			convertPB->Release();
			convertPB = NULL;
		}
		
		SafeArrayUnaccessData(auxMsgArray);
		SafeArrayDestroy(auxMsgArray);
	}

	return S_OK;
}

