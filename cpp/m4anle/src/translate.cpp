// Translate.cpp: implementation of the Translate class.
//
//////////////////////////////////////////////////////////////////////

#include "translate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Translate::Translate()
{
	// Inicialmente se deja en lenguaje no inicializado y sin desplazamiento
	// en la búsqueda de recursos
	m_lLCID = 0;
	m_lResourceOffset = 0;
}

Translate::~Translate()
{

}

void Translate::SetLanguage(long lLCID)
{
	// Por el momento sólo se soporta Español e Inglés
	if (lLCID == 3082) // Español
	{
		m_lLCID = 3082; // Guardamos el LCID Español
		m_lResourceOffset = 2000; // Establecemos el Offset de recursos
	}
	else // Inglés
	{
		m_lLCID = 1033; // Guardamos el LCID Inglés (US)
		m_lResourceOffset = 1000;
	}
}

long Translate::GetLanguage()
{
	return m_lLCID;
}

HRSRC Translate::M4FindResource(HMODULE hModule, long idResource, LPCTSTR lpType)
{
	return FindResource(hModule, MAKEINTRESOURCE(idResource + m_lResourceOffset), lpType);
}
