// Runtime.cpp: implementation of the CRuntime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4anle.h"
#include "runtime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRuntime::CRuntime()
{
	// Se incializan las variables miembro.
	m_pPBObjects=NULL;
	m_pPBPropertysgroup=NULL;
}

CRuntime::~CRuntime()
{
	// Se incializan las variables miembro.
	m_pPBObjects=NULL;
	m_pPBPropertysgroup=NULL;
}

// Se carga las variables miembro con los valores.
void CRuntime::Cargar(ICPropBag * pval1,ICPropBag * pval2)
{
	// Variable miembro de Objetos.
	m_pPBObjects = pval1;
	// Variable miembro de Grupo de Propiedades.
	m_pPBPropertysgroup = pval2;
}


// Nos indica si para un objeto y propiedad determinada si es
// o no un Método de Runtime
bool CRuntime::Runtimemethod(CString objname, CString propname)
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
	long numobj=1;
	long retorn=0;
	short devul=0;
	bool resul=FALSE;
	CString tipo;

	BSTR bClassname=NULL;
	BSTR bAlias=NULL;

	if (m_pPBObjects)
	{
		HRESULT hr = m_pPBObjects->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos obteniendo los punteros a las diferentes 
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

					// Buscamos el objeto a traves del alias.
					if (Alias==objname)
					{						
						retorn=numobj;
						// Una vez encontrado llamamos a la función
						// para que nos localice si dicha propiedad es
						// o no un método de runtime.
						resul = Encmethod(retorn, propname, 1);
					}
					else
						// Vamos incrementado una variable para indicarnos
						// por que objeto vamos de la Property Bag.
						++numobj;

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

	// Devuelve si dicha propiedad es o no un método de runtime.
	return resul;
}

// Nos indica si para un objeto y propiedad determinada si es
// o no una Propiedad de Runtime
bool CRuntime::Runtimeproperty(CString objname, CString propname)
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
	long numobj=1;
	long retorn=0;
	short devul=0;
	bool resul=FALSE;
	CString tipo;

	BSTR bClassname=NULL;
	BSTR bAlias=NULL;

	if (m_pPBObjects)
	{
		HRESULT hr = m_pPBObjects->get_ChildCol((ICChildCol**)&pCC);
		if (hr == S_OK)
		{
			// Aquí vamos obteniendo los punteros a las diferentes 
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

					// Buscamos el objeto a traves del alias.
					if (Alias==objname)
					{						
						retorn=numobj;
						// Una vez encontrado llamamos a la función
						// para que nos localice si dicha propiedad es
						// o no una propiedad de runtime.
						resul = Encmethod(retorn, propname, 2);
					}
					else
						// Vamos incrementado una variable para indicarnos
						// por que objeto vamos de la Property Bag.
						++numobj;

					::VariantClear(&vIn);
					pEnum->Next(1,&vIn,&num);

				}
				pEnum->Release();
			}
			}
			pCC->Release();
		}
	}
	// Liberamos la variables.
	SysFreeString(bClassname);
	SysFreeString(bAlias);

	// Devuelve si dicha propiedad es o no una propiedad de runtime.
	return resul;

}

// Función para localizar si un método es de runtime. Tenemos que tomar primero 
// la ChildCol y localizar aquellas PB que sean de Classname Property y aquellas
// otras cuya Classname sea Aggregates. Para aquellas que sean Property tengo que
// comprobar que el alias que tiene coincide con el nombre de propiedad que nos
// han pasado. Para aquellas que son Aggregates tengo que localizar dentro de 
// PropertyGroup aquel que coincida su alias con el valor de la propiedad Propertygroup
// del Aggregates.

bool CRuntime::Encmethod(long obj, CString propname, short tipo)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICPropBag *pPBAux2 = NULL;
	ICChildCol *pCC = NULL;
	ICChildCol *pCC2 = NULL;
	ICChildCol *pCC3 = NULL;
	ICPropCol *pPropCol = NULL;
	ICSingleProp *pSP = NULL;

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
	long retorn=0;
	bool aux=false;
	short devul=0;


	if ( obj == -1)
		return FALSE;
	else
	{
	BSTR bClassname=NULL;
	BSTR bAlias=NULL;
	BSTR nombre=NULL;

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
				v.lVal = obj;
				hr = pCC->get_Item(v,&pPBAux);
				::VariantClear(&v);
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
								while (((vIn.vt==VT_DISPATCH)&&(num==1))&&(retorn==0))
								{
									pPBAux2=(ICPropBag*)vIn.pdispVal;

									hr = pPBAux2->get_ClassName(&bClassname);
									hr = pPBAux2->get_Alias(&bAlias);

									M4BSTRToCString(bClassname, clase);
									M4BSTRToCString(bAlias, Alias);

									if (clase=="Property")
									{
										if (Alias==propname)
										{
											// Rellena la estructura a devolver
											// y devuelve un 1
											hr =pPBAux2->get_PropCol(&pPropCol);
											if (hr==S_OK)
											{
												hr = pPropCol->get_Count(&nCount);
												if ((hr==S_OK) && (nCount>0))
												{
													// Aquí vendría comprobar si el método o
													// Propiedad es de Runtime.
													aux=Isruntime(&pPropCol,tipo);
													retorn=1;
												}
											}
											pPropCol->Release();
										}
									}
									else
									{

									if (clase=="Aggregates")									
									{
										hr =pPBAux2->get_PropCol(&pPropCol);
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
												// Llamamos a la función para localizar la propiedad
												// dentro del Grupo de porpiedades indicadas por el
												// Aggregates.
												aux=Isaggreagates(propname, nomalias,tipo);
												if (aux)
													retorn=1;
											}
										}
										pPropCol->Release();
									}
									}
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
	// Liberamos la variables.
	SysFreeString(bClassname);
	SysFreeString(bAlias);
	SysFreeString(nombre);
	// Retornamos el valor encontrado.
	return aux;
	}
}
// Con esta función localizamos si la propiedad es un método o
// propieda de Runtime. Atraves de tipo sabemos si es un método
// o una propiedad: tipo=1 es método y tipo=2 es una propiedad.
bool CRuntime::Isruntime(ICPropCol * * pval, short tipo)
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
	bool rsbol=FALSE;

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

			pSP->get_Name(&nombre);
			M4BSTRToCString(nombre, vernom);
			
//*************************
			if ((vernom=="Runtimemethod") && (tipo==1))
			{
				::VariantInit(&vOt);
				pSP->get_Value(&vOt);

				esbolean = vOt.bVal;
				if (esbolean)
					rsbol=TRUE;
				else
					rsbol=FALSE;				
			
				::VariantClear(&vOt);
			}
			if ((vernom=="Runtimeproperty") && (tipo==2))
			{
				::VariantInit(&vOt);
				pSP->get_Value(&vOt);

				esbolean = vOt.bVal;
				if (esbolean)
					rsbol=TRUE;
				else
					rsbol=FALSE;				
			
				::VariantClear(&vOt);
			}
//**********************
			
			::VariantClear(&vIn);
			pEnum->Next(1,&vIn,&num);

		}
		pEnum->Release();

	}
	
	}
	}
	// Liberamos la varible.
	SysFreeString(nombre);
	// Devolvemos el valor encontrado.
	return rsbol;
}

// Con esta función localizamos dentro de la PB de Grupo
// de Propiedades si para el grupo anteriormente localizado
// dicha propiedad se encuentra o no en el grupo. Para lo cual
// tenemos que volver a llamar a otra función y realizar el mismo
// proceso que con un objeto.
bool CRuntime::Isaggreagates(CString propname, CString nomgrupo, short tipo)
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
	long numobj=1;
	short retorn=0;
	bool aux=false;

	BSTR bClassname=NULL;
	BSTR bAlias=NULL;

	if (m_pPBPropertysgroup)
	{
		HRESULT hr = m_pPBPropertysgroup->get_ChildCol(&pCC);
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

					hr = pPBAux->get_ClassName(&bClassname);
					hr = pPBAux->get_Alias(&bAlias);

					M4BSTRToCString(bClassname, clase);
					M4BSTRToCString(bAlias, Alias);

					if (Alias==nomgrupo)
					{
						// Aquí tengo la property donde del nombre pasado
						aux=Isencprop(pPBAux, propname, tipo);
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
	SysFreeString(bClassname);
	SysFreeString(bAlias);
	//return retorn;
	return aux;
}

// Aqui se hace el proceso de encontrar la propiedad pero también 
// tenemos que tener encuentra que dentro de un grupo de propiedades
// puede haber otro grupo por lo cual estamos realizando una función
// recursiva que llama otra vez a Isaggreagates hasta que encontremos
// la propiedad especificada.
bool CRuntime::Isencprop(ICPropBag * pval, CString propname, short tipo)
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
	bool aux=false;

	BSTR bClassname=NULL;
	BSTR bAlias=NULL;

	if (pval)
	{
		HRESULT hr = pval->get_ChildCol(&pCC);
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

						hr = pPBAux->get_ClassName(&bClassname);
						hr = pPBAux->get_Alias(&bAlias);

						M4BSTRToCString(bClassname, clase);
						M4BSTRToCString(bAlias, Alias);

						if (Alias==propname)
						{
							hr =pPBAux->get_PropCol(&pPropCol);
							if (hr==S_OK)
							{
								hr = pPropCol->get_Count(&nCount);
								if ((hr==S_OK) && (nCount>0))
								{
									// Aquí vendría el rellenar la estructura
									aux = Isruntime(&pPropCol, tipo);
									retorn=1;
								}
							}
							pPropCol->Release();
						}
///*******
						if (clase=="Aggregates")									
						{
							hr =pPBAux->get_PropCol(&pPropCol);
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
									aux = Isaggreagates(propname, nomalias, tipo);
									if (aux)
										retorn=1;
								}
							}
							pPropCol->Release();
						}

///******
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
	// Liberamos las variables.
	SysFreeString(bClassname);
	SysFreeString(bAlias);
	// Devolvemos el valor encontrado.
	return aux;
}


// Devuelve si el nombre de objeto que le pasamos
// esta declarado como Objeto dentro de la Property
// Bag de Objetos.
BOOL CRuntime::IsClassnameDefined(CString nombreobj)
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
	long retorn=0;
	short devul=0;
	CString tipo;

	BSTR bClassname=NULL;
	BSTR bAlias=NULL;

	if (m_pPBObjects)
	{
		HRESULT hr = m_pPBObjects->get_ChildCol(&pCC);
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
					if (Alias==nombreobj)
					{						
						retorn = 1;
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
	return retorn;
}
