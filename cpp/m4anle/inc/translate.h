// Translate.h: interface for the Translate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSLATE_H__932159D8_2C5C_48FC_B05B_4A6CD9356FFA__INCLUDED_)
#define AFX_TRANSLATE_H__932159D8_2C5C_48FC_B05B_4A6CD9356FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class Translate  
{
public:
	Translate();
	virtual ~Translate();

	void	SetLanguage(long lLCID);
	long	GetLanguage();

	HRSRC M4FindResource(HMODULE hModule, long idResource, LPCTSTR lpType);


private:
	long			m_lLCID;	// Id de lenguaje para los errores / gestión de recursos
	long			m_lResourceOffset; // Offset de búsqueda de recursos

};

#endif // !defined(AFX_TRANSLATE_H__932159D8_2C5C_48FC_B05B_4A6CD9356FFA__INCLUDED_)
