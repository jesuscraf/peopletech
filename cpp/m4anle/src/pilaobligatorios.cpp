// PilaObligatorios.cpp: implementation of the CPilaObligatorios class.
//
//////////////////////////////////////////////////////////////////////

//FICHERO MODIFICADO PARA VERSION 3

#include "stdafx.h"
#include "m4anle.h"
#include "runtime.h"
#include "objeto.h"
#include "pilaobligatorios.h"
#include "macros.h"
#include "utils.h"
#include "globdef.h"
#include "operador.h"
#include "arbol.h"
#include "evaluacion.h"
#include "expresion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CAMBIOS PARA VERSION 3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMetodo::CMetodo(CString nombre, CString metodo, int padre, CString expresion)
{
	m_nombre = nombre;
	m_metodo = metodo;
	m_padre = padre;
	m_expresion = expresion;
}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPilaObligatorios::CPilaObligatorios()
{
	m_objraiz =	NULL;
	m_runtime = NULL;
}

CPilaObligatorios::~CPilaObligatorios()
{
CObject *pobj;

	if(m_objraiz)
		delete m_objraiz;

	while(!m_listmetodos.IsEmpty())
		{
		pobj = m_listmetodos.GetHead();
		m_listmetodos.RemoveHead();
		delete pobj;
		}

}

void CPilaObligatorios::Cargar(CRuntime *runtime)
{
	m_runtime = runtime;
}


short CPilaObligatorios::NewObjeto(CString objeto, CString alias, int identificador,int padre)
{
CObjeto *pnew = new CObjeto(objeto, alias, identificador, m_runtime);
CObjeto *pobj = NULL;

	if(!m_objraiz)
		{
		m_objraiz = pnew;
		return TRUE;
		}
	else
		{
		pobj = m_objraiz->FindObjeto(padre);
		if(!pobj)
			return FALSE;

		pnew->m_padre = pobj;
		return pobj->AddChild(pnew);
		}
}


short CPilaObligatorios::NewPropiedad(int objeto, CString propiedad, CString valor, CString *expresion, BOOL IsMetodo)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return FALSE;

	if(IsMetodo)
		{// Se trata de un metodo
		CMetodo *pmetodo = new CMetodo(propiedad, valor, objeto, *expresion);
		if(pmetodo)
			m_listmetodos.AddHead(pmetodo);
		}

	pobj = m_objraiz->FindObjeto(objeto);
	if(!pobj)
		return FALSE;

	if(IsMetodo)
		return pobj->AddPropiedad(propiedad, valor, NULL);
	else
		return pobj->AddPropiedad(propiedad, valor, expresion);
}

void CPilaObligatorios::AddObjObligatorio(int objpadre, OBJ_OBLIGATORIO *obligatorio)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return;

	pobj = m_objraiz->FindObjeto(objpadre);
	if(!pobj)
		return;

	pobj->AddObjObligatorio(obligatorio);
}

void CPilaObligatorios::AddPropObligatoria(int objpadre, CString propiedad)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return;

	pobj = m_objraiz->FindObjeto(objpadre);
	if(!pobj)
		return;

	pobj->AddPropObligatoria(propiedad);
}

void CPilaObligatorios::GetObjObligatorio(int objeto, int num, CString &classname)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return;

	pobj = m_objraiz->FindObjeto(objeto);
	if(!pobj)
		return;

	pobj->GetObjObligatorio(num,classname);
}


void CPilaObligatorios::GetPropObligatoria(int objeto, int num, CString &propiedad)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return;

	pobj = m_objraiz->FindObjeto(objeto);
	if(!pobj)
		return;

	pobj->GetPropObligatoria(num, propiedad);
}

short CPilaObligatorios::CheckObjObligatorios(int objeto)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return FALSE;

	pobj = m_objraiz->FindObjeto(objeto);
	if(!pobj)
		return FALSE;

	return (short)pobj->GetNumObjObligatorios();
}

short CPilaObligatorios::CheckObjMaxObligatorios(int objeto)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return FALSE;

	pobj = m_objraiz->FindObjeto(objeto);
	if(!pobj)
		return FALSE;

	return (short)pobj->GetNumMaxObjObligatorios();
}


short CPilaObligatorios::CheckPropObligatorias(int objeto)
{
CObjeto *pobj = NULL;

	if(!m_objraiz)
		return FALSE;

	pobj = m_objraiz->FindObjeto(objeto);
	if(!pobj)
		return FALSE;

	return (short)pobj->GetNumPropObligatorias();
}


int CPilaObligatorios::GetNumMetodos()
{
	return (int) m_listmetodos.GetCount();
}

short CPilaObligatorios::CheckMetodo(int nummetodo, CHK_METODO *info)
{
POSITION pos;
CMetodo *metodo;
int n;
CObjeto *pobj = NULL;
BOOL res;
CHK_CONSTRAINT fail;

	n = (int) m_listmetodos.GetCount();

	if((nummetodo > n) || (nummetodo <= 0))
		return FALSE;
	
	n = 1;
	for(pos = m_listmetodos.GetTailPosition(); ((n<=nummetodo) && (pos != NULL));)
		{
		metodo = (CMetodo *)m_listmetodos.GetPrev(pos);
		n++;
		}

	info->objeto = metodo->m_padre;
	info->propiedad = metodo->m_nombre;
	info->valor = metodo->m_metodo;
	info->failpath = FALSE;
	info->failruntimeproperty = FALSE;
	info->failruntimemethod = FALSE;
	fail.failpath = FALSE;
	fail.failruntimeproperty = FALSE;
	fail.failruntimemethod = FALSE;

	pobj = m_objraiz->FindObjeto(metodo->m_padre);
	if(!pobj)
		return FALSE;
	else
		{
		res = pobj->CheckConstraint(metodo->m_nombre, metodo->m_metodo, &(metodo->m_expresion), &fail);
		info->failpath = fail.failpath;
		info->failruntimeproperty = fail.failruntimeproperty;
		info->failruntimemethod = fail.failruntimemethod;
		return (short)res;
		}
}

