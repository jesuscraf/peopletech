// CompType.cpp: implementation of the CCompType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "operador.h"
#include "arbol.h"
#include "evaluacion.h"
#include "expresion.h"
#include "m4anle.h"
#include "comptype.h"
#include "utils.h"
#include "macros.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CAMBIOS PARA VERSION 3


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompType::CCompType()
{
	m_pPBTypes = NULL;
}

CCompType::~CCompType()
{

}

void CCompType::cargapb(ICPropBag *pbag)
{
	m_pPBTypes = pbag;
}


// Comprueba si valor es un dato valido para tipo
short CCompType::comptype(LPCSTR ctipo, LPCSTR cvalor, info_error *pserror)
{
	ICPropBag *pbag=NULL;
	ICPropBag *pPBAux=NULL;
	ICChildCol *pchild1=NULL;
	ICChildCol *pchild2=NULL;
	ICSingleProp *psingle=NULL;
	ICPropCol *pprop=NULL;
	short ret=FALSE;
	long n = 0;
	CString alias;
	CString name;
	CComVariant condicion;
	CComVariant error;
	CComVariant tipobase;
	CComVariant ind;
	char *exp=NULL;
	char *tipo = NULL,*valor = NULL;

	bool findColor = false;
	IUnknown *pUnk = NULL;
	IEnumVARIANT *pEnum1 = NULL,*pEnum2 = NULL;
	VARIANT vIn1,vIn2;
	unsigned long num;
	unsigned long num2;


	USES_CONVERSION;

	tipo = (char *)malloc(strlen(ctipo) + 1);
	if(!tipo)
		return FALSE;
	valor = (char *)malloc(strlen(cvalor) +1);
	if(!valor)
		return FALSE;
	strcpy(tipo,ctipo);
	strcpy(valor,cvalor);


	if(!m_pPBTypes)
		return FALSE;

	if(FAILED(m_pPBTypes->get_ChildCol((ICChildCol**)&pchild1)))
		return FALSE;

	if(FAILED(pchild1->get_Count(&n)))
		return FALSE;
	
	if(n <= 0) 
		return FALSE;

	if(FAILED(pchild1->get__NewEnum(&pUnk)))
		return FALSE;

	if(FAILED(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum1)))
		return FALSE;

	pUnk->Release();

	::VariantInit(&vIn1);
	pEnum1->Reset();
	pEnum1->Next(1,&vIn1,&num);
	while((vIn1.vt == VT_DISPATCH) && (num == 1))
		{
		pPBAux = (ICPropBag*)vIn1.pdispVal;


		BSTR bstrTemp = NULL;
		if(FAILED(pPBAux->get_Alias(&bstrTemp)))
			break;

		M4BSTRToCString(bstrTemp, alias);
		::SysFreeString(bstrTemp);
		bstrTemp = NULL;

		if(alias == tipo)
			{
			
			// modificaciones de Gemma
			// cableo que te cagas !!!

			//if (strcmp(alias,"Color") == 0)
			//	findColor = true;
			//
			
			//Se busca el tipo base y se verifica que es correcto
			if(FAILED(pPBAux->get_PropCol((ICPropCol**)&pprop)))
				break;

			if(FAILED(pprop->get_Count(&n)))
				break;

			if(n<=0)
				break;

			ind = 1;
			if(FAILED(pprop->get_Item(ind, &psingle)))
				break;

			//AVISO(OJO_SOLO_COMENTADO_PARA_PODER_HACER_PRUEBAS)
			/*if(FAILED(psingle->get_Name(&aux)))
				break;

			M4BSTRToCString(aux, name);
			if(name != "BaseType")
				break;*/

			if(FAILED(psingle->get_Value(&tipobase)))
				break;

			pprop->Release();pprop = NULL;
			psingle->Release();psingle = NULL;

			M4BSTRToCString(tipobase.bstrVal, name);

			if(comptipobase((LPCSTR)name,valor,pserror)<=0)
				break;

			//Busqueda de la expresión
			if(FAILED(pPBAux->get_ChildCol((ICChildCol**)&pchild2)))
				break;

			if(FAILED(pchild2->get_Count(&n)))
				break;

			if(n == 0)
				{
				ret = TRUE;//Es un tipo base
				break;
				}

			ind = 1;
			if(FAILED(pchild2->get_Item(ind, &pbag)))
				break;

			if(FAILED(pbag->get_PropCol((ICPropCol**)&pprop)))
				break;

			if(FAILED(pprop->get_Count(&n)))
				break;

			if(n<=0)
				break;

			if(FAILED(pprop->get__NewEnum(&pUnk)))
				break;

			if(FAILED(pUnk->QueryInterface(IID_IEnumVARIANT, (void**)&pEnum2)))
				break;

			pUnk->Release();

			::VariantInit(&vIn2);
			pEnum2->Reset();
			pEnum2->Next(1,&vIn2,&num2);
			psingle = NULL;
			while((vIn2.vt == VT_DISPATCH) && (num2 == 1))
				{
				//if(psingle) 
				//	psingle->Release();
				psingle = (ICSingleProp*)vIn2.pdispVal;

				
				if(FAILED(psingle->get_Name(&bstrTemp)))
					break;

				M4BSTRToCString(bstrTemp, name);
				::SysFreeString(bstrTemp);
				bstrTemp = NULL;
				if(name == "Expression")
					{
					CExpresion expresion;

					if(FAILED(psingle->get_Value(&condicion)))
						break;
					
					M4BSTRToCString(condicion.bstrVal, name);
					exp = (char *)malloc(strlen((LPCSTR)name) +1);
					if(!exp)
						break;
					strcpy(exp,(LPCSTR)name);

					CleanChr(valor,34);//Quitar a valor las comillas
					expresion.Define(valor, tipo, exp);
					ret = (short)expresion.Evalua();
					LIBERA(exp);
					}
				else
				if(name == "Type")
					{
					psingle->get_Value(&error);
					M4BSTRToCString(error.bstrVal, pserror->Type);
					}
				else
				if(name == "Message")
					{
					psingle->get_Value(&error);
					M4BSTRToCString(error.bstrVal, pserror->Message);
					}
				else
				if(name == "Code")
					{
					char code[20];
					psingle->get_Value(&error);
					switch(error.vt)
						{
						case VT_I2:
						case VT_I4:
							itoa(error.iVal, code, 10);
							pserror->Code = code;
							break;
						case VT_BSTR:
							M4BSTRToCString(error.bstrVal, pserror->Code);
							break;
						}
					}


				::VariantClear(&vIn2);
				pEnum2->Next(1,&vIn2,&num2);
				}

			pEnum2->Release();
			pchild2->Release();pchild2=NULL;
			pbag->Release();pbag=NULL;
			pprop->Release();pprop=NULL;
			break;
			}

		::VariantClear(&vIn1);
		pEnum1->Next(1,&vIn1,&num);
		}

	//No se ha encontrado el tipo. Se comprueba si se trata de un tipo base
	if((vIn1.vt != VT_DISPATCH) || (num != 1))
		ret = comptipobase(tipo, valor, pserror);

	if(pchild1)	pchild1->Release();
	if(pchild2)	pchild2->Release();
	if(pbag) pbag->Release();
	if(pprop) pprop->Release();
	pEnum1->Release();
	LIBERA(tipo);
	LIBERA(valor);

	/*if (findColor)
		return 1;
	else*/
		return ret;

//#endif
}

// Método para determinar si el tipo base es correcto
short CCompType::comptipobase(LPCSTR tipo, LPCSTR valor, info_error *pserror)
{
CString palab;
CString pmayus;
CString csizqu = "";
int li_inte=0;
long li_lon=0;
char csner[20];

	palab=valor;

	pmayus=palab;

	pmayus.MakeUpper();


	if(!strcmp(tipo,"String"))
		{// String
		if (!compstring(palab))
			{
			pserror->Type = "ERROR";
			pserror->Code = "29";
			pserror->Message = "le falta el comienzo o el final de las comillas.";
			return FALSE;//Le falta el comienzo o el final de las comillas
			}
		else
			return TRUE;
		}
	else
	if(!strcmp(tipo,"ListString"))
		{// ListString
		if (!compstring(palab))
			{
			pserror->Type = "ERROR";
			pserror->Code = "29";
			pserror->Message = "le falta el comienzo o el final de las comillas.";
			return FALSE;//Le falta el comienzo o el final de las comillas
			}
		else
			if (complist(palab))
				return TRUE;
			else
				{
				pserror->Type = "ERROR";
				pserror->Code = "30";
				pserror->Message = "no es una lista de cadenas.";
				return FALSE;//No es una lista de cadenas
				}
		}
	else
	if(!strcmp(tipo,"Bool"))
		{// Boolean
		if ((pmayus=="TRUE") || (pmayus=="FALSE") || (pmayus=='0') || (pmayus=='1'))
			return TRUE;
		else
			{
			pserror->Type = "ERROR";
			pserror->Code = "31";
			pserror->Message = "no tiene un valor booleano.";
			return FALSE;//No tiene un valor booleano
			}
		}
	else
	if(!strcmp(tipo,"Integer"))
		{// Integer
		csizqu = palab.Left(1);

		if (csizqu=='-')
			palab = palab.Mid(1,palab.GetLength());

		li_inte = atoi(palab);
		itoa(li_inte,csner,10);

		if (((!li_inte) && (palab !="0")) || (li_inte<0) || (palab!=csner))
			{
			pserror->Type = "ERROR";
			pserror->Code = "32";
			pserror->Message = "no es un númerico entero.";
			return FALSE;//No es un númerico entero.";
			}
		else
			{
			if (li_inte > 32767)
				{
				pserror->Type = "ERROR";
				pserror->Code = "33";
				pserror->Message = "es mayor que un númerico entero.";
				return FALSE;//Es mayor que un númerico entero.";
				}
			else
				return TRUE;
			}
		}
	else
	if(!strcmp(tipo,"Long"))
		{// Long
		csizqu = palab.Left(1);

		if (csizqu=='-')
			palab = palab.Mid(1,palab.GetLength());

		li_lon = atol(palab);
		ltoa(li_lon,csner,10);
		
/*		if ((!li_lon) && (palab !="0") || (li_lon<0) || (palab!=csner))
			{
			pserror->Type = "ERROR";
			pserror->Code = "34";
			pserror->Message = "no es un númerico entero largo.";
			return FALSE;//No es un númerico entero largo
			}
		else
			{*/
			if (li_lon > LONG_MAX)
				{
				pserror->Type = "ERROR";
				pserror->Code = "35";
				pserror->Message = "es mayor que un númerico entero largo.";
				return FALSE;//Es mayor que un númerico entero largo.";
				}
			else
				return TRUE;
		//	}
		}
	else
	if(!strcmp(tipo,"ListInteger"))
		return complistint(palab);
	else
		return -1;//No se trata de un tipo base

}


// Método  para determinar si es un string
short CCompType::compstring(CString palab)
{
	CString csizqu="";
	CString csdere="";

	palab.TrimRight();
	csizqu = palab.Left(1);
	csdere = palab.Right(1);
	
	if ((csizqu=='"') && (csdere=='"'))
		return 1;
	else
		return 0;
}


// Método  para determinar si es un Liststring
short CCompType::complist(CString palab)
{
	CString csaux="";
	int posinc=0;
	int posfin=0;
	int posaux=0;
	int term =1;
	int encon=1;
	
	posfin=((palab.GetLength())-2);
	csaux = palab.Mid(1,posfin);
	posinc= csaux.Find(',');
	if (posinc==-1)
		return 1;
	else
	{
		posaux = (csaux.GetLength()) - 1;
		if ((posinc==0) || (posinc==posaux))
			return 0;
		else
		{
			do
			{
				posfin = csaux.GetLength();
				++posinc;
				csaux= csaux.Mid(posinc,posfin);
				posinc= csaux.Find(',');
				if (posinc==-1)
					term=0;
				else
				{					
					posaux = (csaux.GetLength()) - 1;
					if ((posinc==0) || (posinc==posaux))
					{
						encon=0;
						term=0;
					}
				}

			} while (term);			

			return (short)encon;
		}

	}
}


// Metodo para determinar si es un list string
short CCompType::complistint(CString palab)
{
size_t sz,i;
char *aux = NULL;

	sz = strlen((LPCSTR)palab);

	aux = (char *)malloc(sz+1);
	CleanSp(aux, (LPCSTR)palab);

	for(i=0; i<sz; i++)
		{
		if(aux[i] != ',')
			if((aux[i]>'9') || (aux[i]<'0'))
				return FALSE;
		else
			if(aux[i+1] == ',')
				return FALSE;
		}

	return TRUE;
}

