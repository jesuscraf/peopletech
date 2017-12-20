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
	// en la b�squeda de recursos
	m_lLCID = 0;
	m_lResourceOffset = 0;
}

Translate::~Translate()
{

}

void Translate::SetLanguage(long lLCID)
{
	// Por el momento s�lo se soporta Espa�ol e Ingl�s
	if (lLCID == 3082) // Espa�ol
	{
		m_lLCID = 3082; // Guardamos el LCID Espa�ol
		m_lResourceOffset = 2000; // Establecemos el Offset de recursos
	}
	else // Ingl�s
	{
		m_lLCID = 1033; // Guardamos el LCID Ingl�s (US)
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
