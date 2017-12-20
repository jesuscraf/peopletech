// Implementación de las clases CNodo y CArbol

#include <windows.h>
#include "operador.h"
#include "arbol.h"
#include "macros.h"

/*-----------------------------------------------------------
Clase CNodo
-----------------------------------------------------------*/
CNodo::CNodo()
{
	m_nodo1 = NULL;
	m_nodo2 = NULL;
	m_nodo3 = NULL;
	m_nodo4 = NULL;
	m_nodo5 = NULL;
	m_operador = NULL;
	m_resultado = NULL;
}

CNodo::~CNodo()
{
	if(m_nodo1)
		delete m_nodo1;
	if(m_nodo2)
		delete m_nodo2;
	if(m_nodo3)
		delete m_nodo3;
	if(m_nodo4)
		delete m_nodo4;
	if(m_nodo5)
		delete m_nodo5;
	if(m_operador)
		delete m_operador;

	LIBERA(m_resultado);
}

int CNodo::Inserta(CNodo *parent, CNodo *newnodo)
{
	if(parent == this)
		{
		if(!m_nodo1)
			m_nodo1 = newnodo;
		else
		if(!m_nodo2)
			m_nodo2 = newnodo;
		else
		if(!m_nodo3)
			m_nodo3 = newnodo;
		else
		if(!m_nodo4)
			m_nodo4 = newnodo;
		else
		if(!m_nodo5)
			m_nodo5 = newnodo;
		else
			return -1;

		return 0;
		}
	else
		{
		if(m_nodo1)
			{
			if(m_nodo1->Inserta(parent, newnodo) == 0)
				return 0;
			}
		if(m_nodo2)
			{
			if(m_nodo2->Inserta(parent, newnodo) == 0)
				return 0;
			}
		if(m_nodo3)
			{
			if(m_nodo3->Inserta(parent, newnodo) == 0)
				return 0;
			}
		if(m_nodo4)
			{
			if(m_nodo4->Inserta(parent, newnodo) == 0)
				return 0;
			}
		if(m_nodo5)
			{
			if(m_nodo5->Inserta(parent, newnodo) == 0)
				return 0;
			}
		return -1;
		}
}


int CNodo::Opera()
{
int res;

	if(!m_operador)
		return -1;

	if(m_nodo1)
		{
		if(m_nodo1->Opera() < 0)
			return -1;
		m_operador->SetParam1(m_nodo1->m_resultado);
		}
	if(m_nodo2)
		{
		if(m_nodo2->Opera() < 0)
			return -1;
		m_operador->SetParam2(m_nodo2->m_resultado);
		}
	if(m_nodo3)
		{
		if(m_nodo3->Opera() < 0)
			return -1;
		m_operador->SetParam3(m_nodo3->m_resultado);
		}
	if(m_nodo4)
		{
		if(m_nodo4->Opera() < 0)
			return -1;
		m_operador->SetParam4(m_nodo4->m_resultado);
		}
	if(m_nodo5)
		{
		if(m_nodo5->Opera() < 0)
			return -1;
		m_operador->SetParam5(m_nodo5->m_resultado);
		}

	res = m_operador->Opera(&m_resultado);
	if((!m_resultado) || (res<0))
		return -1;

	return 0;
}

/*-----------------------------------------------------------
Clase CArbol
-----------------------------------------------------------*/
CArbol::CArbol()
{
	m_raiz = NULL;
}

CArbol::~CArbol()
{
	if(m_raiz)
		delete m_raiz;
}

int CArbol::Inserta(CNodo *parent, CNodo *newnodo)
{
	if(!m_raiz)
		{
		m_raiz = newnodo;
		return 0;
		}

	if(!parent)
		return -1;
	else
		return m_raiz->Inserta(parent, newnodo);
}

BOOL CArbol::Resuelve(void)
{
BOOL res;

	if(!m_raiz)
		return FALSE;

	if(m_raiz->Opera() < 0)
		return FALSE;

	switch(m_raiz->m_resultado[0])
		{
		case '0':
			res = FALSE;
			break;
		case '1':
			res = TRUE;
			break;
		default:
			res = FALSE;
			break;
		}

	return res;
}


int CArbol::Limpia(void)
{
	if(m_raiz)
		{
		delete m_raiz;
		m_raiz = NULL;
		}

	return 0;
}

