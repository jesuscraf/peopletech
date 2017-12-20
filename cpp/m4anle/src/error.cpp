// Error.cpp: implementation of the CError class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4anle.h"
#include "error.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CError::CError(int linea, CString texto, int codigo, CString objeto, CString padre, CString propiedad, CString propval)
{
	m_linea = linea;
	m_texto = texto;
	m_codigo = codigo;
	m_objeto = objeto;
	m_padre = padre;
	m_propiedad = propiedad;
	m_propval = propval;
}

CError::CError()
{
}

CError::~CError()
{

}
