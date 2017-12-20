// AnalyzeProperty.cpp: implementation of the CAnalyzeProperty class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4define.hpp"
#include "m4anle.h"
#include "analyzeproperty.h"
#include "type.h"
#include "expressionproperty.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnalyzeProperty::CAnalyzeProperty()
{

}

CAnalyzeProperty::~CAnalyzeProperty()
{
	// Destruimos todas las referencias creadas
	// anteriormente para los Tipos, Objetos y
	// grupo de propiedades.

	if(m_pPBTypes)
		m_pPBTypes->Release();

	if(m_pPBPropertiesgroup)
		m_pPBPropertiesgroup->Release();

	if(m_pPBObjects)
		m_pPBObjects->Release();

}
// Realizamos la carga de las Property Bag de Objetos
// Tipos y Grupo de Propiedades
void CAnalyzeProperty::CargarPB(ICPropBag * pPBTypesrec, ICPropBag * pPBPropertiesgrouprec, ICPropBag * pPBObjectsrec)
{
	// m_pPBTypes es la variable miembro para
	// la property bag de tipos.
	m_pPBTypes = pPBTypesrec;
	// Se registra dicho entorno
	m_pPBTypes->AddRef();

	// m_pPBPropertiesgroup es la variable miembro
	// para la property bag de Grupo de propiedades.
	m_pPBPropertiesgroup = pPBPropertiesgrouprec;
	// Se registra dicho entorno
	m_pPBPropertiesgroup->AddRef();

	// m_pPBObjects es la variable miembro para
	// la property bag de Objetos
	m_pPBObjects = pPBObjectsrec;
	// Se registra dicho entorno
	m_pPBObjects->AddRef();

	// Inicializamos las variables de Objeto
	// Propiedad y Valor de la Propiedad.
	m_NomObjeto="";
	m_NomProperty="";
	m_ValueProp="";
}
// Esta función analiza si para el objeto existe la propiedad
// y para el valor dado para la propiedad es correcta.
short CAnalyzeProperty::AnalyzeProp(BSTR Object, BSTR Property, BSTR Value)
{
	USES_CONVERSION;

	long NumObj=0;

	// Hacemos una conversión a CString
	M4BSTRToCString(Object, m_NomObjeto);
	M4BSTRToCString(Property, m_NomProperty);
	M4BSTRToCString(Value, m_ValueProp);

	// Encontramos el objeto en la PB de Objetos
	NumObj = IsObject();

	// Verificamos si el objeto ha sido encontrado
	if (NumObj)
		// Vemos si es correcta la propieda y su valor.
	{
		return IsCorrect(NumObj);

	}
	else
		// En el caso de que el objeto no haya sido encontrado
		// en el metalenguaje,
		return 3;
}

// Función para comprobar la existencia del Objeto.
long CAnalyzeProperty::IsObject()
{
	USES_CONVERSION;
	// PB auxiliar que utilizamos en el tratamiento de datos
	ICPropBag *pPBAux = NULL;
	// ChildCol auxiliar que utilizamos para recorrer todos los objetos
	ICChildCol *pCC = NULL;
	

	// Lo utilizamos para recorrer el interfaz
	unsigned long num;


	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	
	// Variable para determina si es un Object
	CString clase;
	// Varialble que toma el Classname del Objeto
	CString Alias;
	// Variable que determina si hemos encontrado el objeto.
	short retorn=0;
	// Para determinar el número de objeto especifico.
	long numobj = 1;
	// Para comprobar la existencia de PB hijas dentro de
	// la PB de Objetos
	long nCount;

	// Obtenemos el Classname de la PB hija.
	BSTR bstrClassname = NULL;
	// Obtenemos el Alisas de la PB hija.
	BSTR bstrAlias = NULL;


	// Comprobamos que es valida la Property Bag de Objetos.
	if (m_pPBObjects)
	{
		// Recogemos la ChildCol de la Property Bag de Objetos.
		HRESULT hr = m_pPBObjects->get_ChildCol(&pCC);
		// Se compueba que la hemos recogido correctamente.
		if (hr == S_OK)
		{
			// Comprobamos cuantos objetos existe en la Colección de PB de la
			// PB de Objetos
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
					// Recorremos todas la PB hijas hasta encontrar la que
					// se corresponde al Objeto que nos acaba de pasar.
					while (((vIn.vt==VT_DISPATCH)&&(num==1))&&(retorn==0))
					{

						pPBAux=(ICPropBag*)vIn.pdispVal;
						// Obtenemos el Classname y el Alias de la PB actual.
						hr = pPBAux->get_ClassName(&bstrClassname);
						hr = pPBAux->get_Alias(&bstrAlias);

						// Conversión a CString del ClassName y el Alias.
						M4BSTRToCString(bstrClassname, clase);
						M4BSTRToCString(bstrAlias, Alias);
						::SysFreeString(bstrClassname);
						bstrClassname = NULL;
						::SysFreeString(bstrAlias);
						bstrAlias = NULL;

						// Comprobamos que el Alias coincide con el nombre
						// que nos han pasado
						if (Alias==m_NomObjeto)
						{						
							// Hemos encontrado el objeto, por lo cual paramos
							// la busqueda del mismo.
							retorn = 1;
						}
						else
							// No hemos encontrado el objeto, por lo que incrementamos
							// el contador del objetos.
							++numobj;

						// Liberamos la variable auxiliar y nos colocamos en la siguiente
						// PB.
						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);

					}
					// Liberamos la variable enumerador.
					pEnum->Release();
				}
			}
			// Liberamos la ChildCol de la Property Bag de Objetos.
			pCC->Release();
		}
	}

	// Retornamos si dicho objeto se encuentra o no dentro
	// de la PB de Objetos.
	if (retorn)
		return numobj;
	else
	// En el caso de encontrar el objeto en la Property Bag
		return 0;
}


// Comprobamos que para el objeto dado y la propiedad especificada
// se encuentra en el Objeto y además el valor dado cumple las 
// Constraint dadas.
short CAnalyzeProperty::IsCorrect(long Id_Object)
{
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICPropBag *pPBAux2 = NULL;
	ICPropBag *pPBAux3 = NULL;
	ICChildCol *pCC = NULL;
	ICChildCol *pCC2 = NULL;
	ICChildCol *pCC3 = NULL;
	ICPropCol *pPropCol = NULL;
	ICSingleProp *pSP=NULL;

	unsigned long num;


	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	VARIANT	vIn;
	VARIANT	va;
	VARIANT	vValue;

	CString clase;
	CString Alias;
	CString nomalias;	

	short retorn=0;
	short aux=0;
	short aux2=0;


	BSTR bstrClassname = NULL;
	BSTR bstrAlias = NULL;

	if (m_pPBObjects)
	{

		HRESULT hr = m_pPBObjects->get_ChildCol(&pCC);
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
				v.lVal = Id_Object;
				hr = pCC->get_Item(v,&pPBAux);
				::VariantClear(&v);
				// Liberamos la ChildCol de la Property Bag del Objeto elegido.
				pCC->Release();				

				if (hr==S_OK)
				{
					hr=pPBAux->get_ChildCol(&pCC2);
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

									hr = pPBAux2->get_ClassName(&bstrClassname);
									hr = pPBAux2->get_Alias(&bstrAlias);

									M4BSTRToCString(bstrClassname, clase);
									M4BSTRToCString(bstrAlias, Alias);

									::SysFreeString(bstrClassname);
									bstrClassname = NULL;

									::SysFreeString(bstrAlias);
									bstrAlias = NULL;

									if (clase=="Property")									
									{
										// Si el Alias coincide con el nombre de propiedad 
										// encontrada. Ya podemos salirnos.
										if(Alias==m_NomProperty)
										{
											retorn = 1;

											hr =pPBAux2->get_PropCol(&pPropCol);
											if (hr==S_OK)
											{
												hr = pPropCol->get_Count(&nCount);
												if ((hr==S_OK) && (nCount>0))
												{						
													// Realizo la comprobación de los
													// Constraint de Propiedad
													aux=SeeValue(&pPropCol);
												}
											}
											// Aqui tenemos que coger la property bag para el Constraint
											pPropCol->Release();
											if (aux)
											{
												hr=pPBAux2->get_ChildCol(&pCC3);
												if (hr==S_OK)
												{
													hr =pCC3->get_Count(&nCount);
													if ((hr==S_OK) && (nCount>0))
													{
														VARIANT v;
														::VariantInit(&v);
														v.vt = VT_I4;
														v.lVal = 1;
														hr = pCC3->get_Item(v,&pPBAux3);
														::VariantClear(&v);														
														if (hr==S_OK)
														{
															aux2 = SeeValueConstraint(pPBAux3);
															if (aux2>=0)
																aux = aux2;
	
														}														
													}
													pCC3->Release();
												}
											}
										}
									}
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

												// Compruebo que existe la propiedad en el 
												// grupo de propiedades. Aux = -1 cuando
												// no hemos encontrado dicha propiedad en
												// el grupo especificado
												
												aux=SeeAggregates(nomalias);

												if (aux>=0)
													retorn = 1;
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
						pCC2->Release();
					}					
					pPBAux->Release();
				}				
			}
		}
	}
	if (retorn)
		// Hemos encontrado la propiedad y hemos verificado su valor.
		return aux;
	else
		// Cuando no hemos encontrado la propiedad para el objeto especificado.
		return 4;	
}

// Función para comprobar el constraint de tipo para una propiedad
// determina
short CAnalyzeProperty::SeeValue(ICPropCol  ** pval)
{
	// Posibles valores de retorno
	// 0 cuando hay un error de nivel 0
	// 1 cuando ha ido bien.
	// 2 cuando se ha encontrado algún constraint
	// de nivel 1 o 2 y no hemos podido verificarlo.
	
	USES_CONVERSION;

	ICPropCol *pPC = *pval;
	ICSingleProp *pSP = NULL;
	CType Tipo;
	unsigned long num;
	CString vernom;
	VARIANT vIn;
	VARIANT vOt;

	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum = NULL;
	
	long nProp;
	short aux=0;
	short retorn=0;
	CString cadena;	
	Str_type estr_tipo;
	Str_Check estr_check;

	BSTR bstrNombre = NULL;

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
			while (((vIn.vt==VT_DISPATCH)&&(num==1)) && (retorn==0))
			{
				pSP=(ICSingleProp*)vIn.pdispVal;

				pSP->get_Name(&bstrNombre);
				M4BSTRToCString(bstrNombre, vernom);
				::SysFreeString(bstrNombre);
				bstrNombre = NULL;

				if (vernom=="Type")
				{	
					retorn = 1;
					::VariantInit(&vOt);
					pSP->get_Value(&vOt);

					M4BSTRToCString(vOt.bstrVal, cadena);

					Tipo.CargarPB(m_pPBTypes);

					if (Tipo.GetType(cadena, &estr_tipo))
					{						
						estr_check.Loopinit=estr_tipo.Loopinit;
						estr_check.Loopend=estr_tipo.Loopend;
						estr_check.Expression=estr_tipo.Expression;
						estr_check.Code=estr_tipo.Code;

						aux = m_Expression.VerifyProperty(m_NomProperty, m_ValueProp, m_NomObjeto, estr_check);
					}
			
					::VariantClear(&vOt);
				}

				::VariantClear(&vIn);
				pEnum->Next(1,&vIn,&num);

			}
			pEnum->Release();
		}
	}
	return aux;
}

// Función para detectar un Aggregates especifico para un grupo de
// propiedades.
short CAnalyzeProperty::SeeAggregates(CString NomAggreg)
{
	// Posibles valores de retorno
	// -1 cuando no hemos encontrado la propiedad en el
	// grupo especificado
	// otro resultado cuando la hemos encontrado y verificado
	// su valor.
	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;

	unsigned long num;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;

	CString clase;
	CString Alias;
	short retorn=0;
	short aux=0;

	if (m_pPBPropertiesgroup)
	{
		HRESULT hr = m_pPBPropertiesgroup->get_ChildCol(&pCC);
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
						::SysFreeString(bstrClassName);
						::SysFreeString(bstrAlias);
						bstrClassName = NULL;
						bstrAlias = NULL;

						if (Alias==NomAggreg)
						{
							// Aquí tengo la property donde del nombre pasado
							retorn = 1;
							aux = SeeValueAggreg(pPBAux);						
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

	if (retorn)
		if (aux>=0)
			return retorn;
		else
			return -1;
	else
		return -1;

}

// Buscamos por el aggregate determinado si existe la propiedad determinada
// incluso por los subconjuntos.
short CAnalyzeProperty::SeeValueAggreg ( ICPropBag * pval)
{

	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICPropBag *pPBAux3 = NULL;
	ICChildCol *pCC = NULL;
	ICChildCol *pCC3 = NULL;
	ICPropCol *pPropCol;
	ICSingleProp *pSP=NULL;

	unsigned long num;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT va;
	VARIANT vValue;
	
	CString clase;
	CString Alias;
	CString nomalias;
	short aux=0;
	short aux2=0;
	short retorn=0;

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


						if (clase=="Property")
						{
							if (Alias==m_NomProperty)
							{
								retorn = 1;

								hr =pPBAux->get_PropCol(&pPropCol);
								if (hr==S_OK)
								{
									hr = pPropCol->get_Count(&nCount);
									if ((hr==S_OK) && (nCount>0))
									{						
										// Realizo la comprobación de los
										// Constraint de Propiedad
										aux=SeeValue(&pPropCol);
									}
								}
								pPropCol->Release();
								if (aux)
								{
									hr=pPBAux->get_ChildCol(&pCC3);
									if (hr==S_OK)
									{
										hr =pCC3->get_Count(&nCount);
										if ((hr==S_OK) && (nCount>0))
										{
											VARIANT v;
											::VariantInit(&v);
											v.vt = VT_I4;
											v.lVal = 1;
											hr = pCC3->get_Item(v,&pPBAux3);
											::VariantClear(&v);														
											if (hr==S_OK)
											{
												// Aqui tengo la Constraint
												aux2 = SeeValueConstraint(pPBAux3);
												if (aux2>=0)
													aux = aux2;
												pPBAux3->Release();
											}														
											pCC3->Release();
										}										
									}
								}
								// Aqui tengo que ver el constraint del Aggregates
								if (aux)
								{
									aux2 = SeeConstraintAggreg(pval);
									if (aux2>=0)
										aux=aux2;
								}
							}
						}
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
									// Compruebo que existe la propiedad en el 
									// grupo de propiedades. Aux = -1 cuando
									// no hemos encontrado dicha propiedad en
									// el grupo especificado
											
									aux=SeeAggregates(nomalias);

									if (aux>=0)
										retorn = 1;
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
			pCC->Release();
		}
	}
	if (retorn)
		// Hemos encontrado la propiedad y hemos verificado su valor.
		return aux;
	else
		// Cuando no hemos encontrado la propiedad para el objeto especificado.
		return -1;
}

// Comprobamos para el constraint de un Aggregates determinado si se
// cumple para el conjunto de propiedades.
short CAnalyzeProperty::SeeConstraintAggreg ( ICPropBag * pval)
{

	USES_CONVERSION;
	ICPropBag *pPBAux = NULL;
	ICChildCol *pCC = NULL;

	unsigned long num;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;	

	CString clase;
	CString Alias;
	short aux=0;
	short retorn=0;

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


						if (clase=="Constraint")
						{
							retorn = 1;

							aux = SeeValueConstraint(pPBAux);
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
	if (retorn)
		// Hemos encontrado la propiedad y hemos verificado su valor.
		return aux;
	else
		// Cuando no hemos encontrado la propiedad para el objeto especificado.
		return -1;
}

// Rellena los valores del constraint para luego poder analizarlos.
short CAnalyzeProperty::SeeValueConstraint(ICPropBag * pval)
{
	USES_CONVERSION;
	ICSingleProp *pSP=NULL;
	ICPropCol *pPC = NULL;

	unsigned long num;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum;
	VARIANT	vIn;
	VARIANT vOt;
	
	CString clase;
	CString Alias;
	CString nomalias;
	CString vernom;
	CString cadena;
	short aux=0;
	short aux2=0;
	short retorn=0;
	long nCount=0;
	Str_Check estr_check;



	estr_check.Loopinit="";
	estr_check.Loopend="";
	estr_check.Expression="";
	estr_check.Code="";
	

	if (pval)
	{
		HRESULT hr =pval->get_PropCol(&pPC);
		if (hr==S_OK)
		{
			hr = pPC->get_Count(&nCount);
			if ((hr==S_OK) && (nCount>0))
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
						::SysFreeString(bstrTemp);
						bstrTemp = NULL;

						if (vernom=="Loopinit")
						{
							::VariantInit(&vOt);
							pSP->get_Value(&vOt);

							M4BSTRToCString(vOt.bstrVal, cadena);
							estr_check.Loopinit=cadena;
							::VariantClear(&vOt);
						}
						if (vernom=="Loopend")
						{
							::VariantInit(&vOt);
							pSP->get_Value(&vOt);

							M4BSTRToCString(vOt.bstrVal, cadena);
							estr_check.Loopend=cadena;
							::VariantClear(&vOt);
						}
						if (vernom=="Expression")
						{
							retorn = 1;
							::VariantInit(&vOt);
							pSP->get_Value(&vOt);

							M4BSTRToCString(vOt.bstrVal, cadena);
							estr_check.Expression=cadena;
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
									estr_check.Code=code;
								break;
								case VT_BSTR:
									M4BSTRToCString(vOt.bstrVal, estr_check.Code);
									break;
							}			
							::VariantClear(&vOt);
						}

						::VariantClear(&vIn);
						pEnum->Next(1,&vIn,&num);

					}
					pEnum->Release();
					if (retorn)
					{
						aux = m_Expression.VerifyProperty(m_NomProperty, m_ValueProp, m_NomObjeto, estr_check);
					}
				}
			}
			pPC->Release();
		}
	}
		
	if (retorn)
		return aux;
	else
		return 1;
}
