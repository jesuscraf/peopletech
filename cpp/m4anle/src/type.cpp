// Type.cpp: implementation of the CType class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4anle.h"
#include "Type.h"
#include "globdef.h"
#include "macros.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define NUM_TIPOS_BASE	6
char G_TIPOS_BASE[6][30] =
	{
	"Integer", 
	"String", 
	"Bool", 
	"Long", 
	"ListString",
	"ListInteger"
	};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CType::CType()
{
	m_pPBTypes = NULL;
}

CType::~CType()
{

}

void CType::CargarPB(ICPropBag * pPBTypesrec)
{
	m_pPBTypes = pPBTypesrec;
}


//--------------------------------------------------------------------------------------
//Busca en la PB de tipos la información referente al tipo indicado en el parametro Type
//--------------------------------------------------------------------------------------
short CType::GetType(CString Type, Str_type * Datas)
{

	ICPropBag *pbag=NULL;
	ICPropBag *pPBAux=NULL;
	ICChildCol *pchild1=NULL;
	ICChildCol *pchild2=NULL;
	ICSingleProp *psingle=NULL;
	ICPropCol *pprop=NULL;
	short ret=FALSE;
	long n = 0;
	BSTR aux=NULL;
	CString alias;
	CString name;
	CComVariant condicion;
	CComVariant code;
	CComVariant loop;
	CComVariant error;
	CComVariant tipobase;
	CComVariant ind;
	char *exp=NULL;
	char *tipo = NULL;

	IUnknown *pUnk;
	IEnumVARIANT *pEnum1,*pEnum2;
	VARIANT vIn1,vIn2;
	unsigned long num;
	unsigned long num2;


	USES_CONVERSION;

	//Inicialización de la estructura
	Datas->BaseType = "";
	Datas->Loopinit = "1";
	Datas->Loopend = "1";
	Datas->Code = "";
	Datas->Expression = "";
	Datas->Errorlevel = 0;

	//Se comprueba si es un tipo base
	if(IsBaseType(Type))
		{//Si es un tipo base se genera el constraint correspondiente
		GenConstraint(Type, Datas);
		return TRUE;
		}

	tipo = (char *)malloc(strlen(Type) + 1);
	if(!tipo)
		return FALSE;
	strcpy(tipo,Type);


	if(!m_pPBTypes)
		return FALSE;

	if(FAILED(m_pPBTypes->get_ChildCol(&pchild1)))
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


		if(FAILED(pPBAux->get_Alias(&aux)))
			break;

		M4BSTRToCString(aux, alias);

		//Se encuentra el tipo
		if(alias == tipo)
			{
			//Se busca el tipo base
			if(FAILED(pPBAux->get_PropCol(&pprop)))
				break;

			if(FAILED(pprop->get_Count(&n)))
				break;

			if(n<=0)
				break;

			ind = 1;
			if(FAILED(pprop->get_Item(ind, &psingle)))
				break;

			/*if(FAILED(psingle->get_Name(&aux)))
				break;

			M4BSTRToCString(aux, name);
			if(name != "BaseType")
				break;*/

			if(FAILED(psingle->get_Value(&tipobase)))
				break;

			pprop->Release();pprop = NULL;
			psingle->Release();psingle = NULL;

			//Tipo base encontrado
			M4BSTRToCString(tipobase.bstrVal, name);
			Datas->BaseType = name;


			//Busqueda de parametros del constraint de tipo
			if(FAILED(pPBAux->get_ChildCol(&pchild2)))
				break;

			if(FAILED(pchild2->get_Count(&n)))
				break;

			//No tiene constraint
			if(n == 0)
				{//Es un tipo base
				GenConstraint(name, Datas);
				ret = TRUE;
				break;
				}

			ind = 1;
			if(FAILED(pchild2->get_Item(ind, &pbag)))
				break;

			if(FAILED(pbag->get_PropCol(&pprop)))
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
			ret = TRUE;
			while((vIn2.vt == VT_DISPATCH) && (num2 == 1))
				{
				//if(psingle) 
				//	psingle->Release();
				psingle = (ICSingleProp*)vIn2.pdispVal;

				if(FAILED(psingle->get_Name(&aux)))
					break;

				M4BSTRToCString(aux, name);
				//Se encuentra la expresión
				if(name == "Expression")
					{
					if(FAILED(psingle->get_Value(&condicion)))
						break;
					
					M4BSTRToCString(condicion.bstrVal, name);
					Datas->Expression = name;
					}
				else
				//Se encuentra el Loopinit
				if(name == "Loopinit")
					{
					psingle->get_Value(&loop);
					M4BSTRToCString(loop.bstrVal, Datas->Loopinit);
					}
				else
				if(name == "Loopend")
					{
					psingle->get_Value(&loop);
					M4BSTRToCString(loop.bstrVal, Datas->Loopend);
					}
				else
				if(name == "Errorlevel")
					{
					psingle->get_Value(&error);
					Datas->Errorlevel = error.iVal;
					}
				else
				if(name == "Code")
					{
					char cod[20];
					psingle->get_Value(&code);
					switch(code.vt)
						{
						case VT_I2:
						case VT_I4:
							itoa(code.iVal, cod, 10);
							Datas->Code = cod;
							break;
						case VT_BSTR:
							M4BSTRToCString(code.bstrVal, Datas->Code);
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

	//No se ha encontrado el tipo.
	if((vIn1.vt != VT_DISPATCH) || (num != 1))
		ret = FALSE;

	if(pchild1)	pchild1->Release();
	if(pchild2)	pchild2->Release();
	if(pbag) pbag->Release();
	if(pprop) pprop->Release();
	pEnum1->Release();
	LIBERA(tipo);
	SysFreeString(aux);

	return ret;

}


//-------------------------------------
//Comprueba si el tipo es una tipo base
//-------------------------------------
BOOL CType::IsBaseType(CString Type)
{
int n;

	for(n=0; n<NUM_TIPOS_BASE; n++)
		{
		if(!strcmp(Type, G_TIPOS_BASE[n]))
			return TRUE;
		}

	return FALSE;
}

//-----------------------------------------------
//Genera un constraint para el tipo base indicado
//-----------------------------------------------
int CType::GenConstraint(CString Type, Str_type *Datas)
{
int n;

	for(n=0; n<NUM_TIPOS_BASE; n++)
		{
		if(!strcmp(Type, G_TIPOS_BASE[n]))
			break;
		}

	if(n >= NUM_TIPOS_BASE)
		return -1;//No es un tipo base

	Datas->BaseType = G_TIPOS_BASE[n];
	Datas->Loopinit = "1";
	Datas->Loopend = "1";
	Datas->Code = "";

	return 0;
}
