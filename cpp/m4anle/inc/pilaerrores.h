// PilaErrores.h: interface for the CPilaErrores class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PILAERRORES_H__D3BE7464_D0B2_11D2_8BE1_00105A6690CF__INCLUDED_)
#define AFX_PILAERRORES_H__D3BE7464_D0B2_11D2_8BE1_00105A6690CF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//##ModelId=3973073C0108
class CPilaErrores  
{
public:
	//##ModelId=3973073C015B
	CPilaErrores();
	//##ModelId=3973073C0159
	virtual ~CPilaErrores();

	//##ModelId=3973073C013A
	void AddError(int linea, CString texto, int codigo, CString objeto, CString padre, CString propiedad, CString propval);
	//##ModelId=3973073C0132
	void Limpia();
	//##ModelId=3973073C0127
	int GetError(int n, CError *error);
	//##ModelId=3973073C0126
	int GetCount();

//Datos miembro
private:
	//##ModelId=3973073C011E
	CObList m_array;
};

#endif // !defined(AFX_PILAERRORES_H__D3BE7464_D0B2_11D2_8BE1_00105A6690CF__INCLUDED_)
