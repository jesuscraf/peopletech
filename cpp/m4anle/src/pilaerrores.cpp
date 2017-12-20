// PilaErrores.cpp: implementation of the CPilaErrores class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4anle.h"
#include "error.h"
#include "dinarray.h"
#include "pilaerrores.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//CAMBIOS PARA VERSION 3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPilaErrores::CPilaErrores()
{
}

CPilaErrores::~CPilaErrores()
{
	Limpia();
}

void CPilaErrores::AddError(int linea, CString texto, int codigo, CString objeto, CString padre, CString propiedad, CString propval)
{
CError *err=NULL;
CError *aux=NULL;
POSITION pos,pos2;

	err = new CError(linea, texto, codigo, objeto, padre, propiedad, propval);
	if(!err) return;

	if(m_array.IsEmpty())
		{
		m_array.AddHead(err);
		return;
		}

	pos2 = m_array.GetHeadPosition();
	for( pos = pos2; pos != NULL; )
		{
		aux = (CError*) m_array.GetNext(pos);
		if(aux->m_linea > linea)
			break;
		pos2 = pos;
		}

	if(pos2 != NULL)
		m_array.InsertBefore(pos2, err);
	else
		m_array.AddTail(err);
}

void CPilaErrores::Limpia()
{
CObject *pobj;

	while(!m_array.IsEmpty())
		{
		pobj = m_array.GetHead();
		m_array.RemoveHead();
		delete pobj;
		}
}

int CPilaErrores::GetError(int n, CError *error)
{
CError *err=NULL;
POSITION pos;

	if((n > m_array.GetCount()) || (n<=0))
		return  -1;

	pos = m_array.FindIndex(n-1);
	if(!pos)
		return -1;

	err = (CError*)m_array.GetAt(pos);
	if(!err)
		return -1;

	error->m_linea = err->m_linea;
	error->m_texto = err->m_texto;
	error->m_codigo = err->m_codigo;
	error->m_objeto = err->m_objeto;
	error->m_padre = err->m_padre;
	error->m_propiedad = err->m_propiedad;
	error->m_propval = err->m_propval;

	return 0;
}

int CPilaErrores::GetCount()
{
	return (int) m_array.GetCount();
}

