// Error.h: interface for the CError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERROR_H__D3BE7463_D0B2_11D2_8BE1_00105A6690CF__INCLUDED_)
#define AFX_ERROR_H__D3BE7463_D0B2_11D2_8BE1_00105A6690CF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//##ModelId=3973073903CB
class CError : public CObject  
{
public:
	//##ModelId=3973073A0016
	CError(int linea, CString texto, int codigo, CString objeto, CString padre, CString propiedad, CString propval);
	//##ModelId=3973073A0024
	CError();
	//##ModelId=3973073A000D
	virtual ~CError();

//Datos miembro
	//##ModelId=3973073A000C
	int m_linea;
	//##ModelId=3973073A000B
	CString m_texto;
	//##ModelId=3973073A0002
	int m_codigo;
	//##ModelId=3973073A0001
	CString m_objeto;
	//##ModelId=3973073903E0
	CString m_padre;
	//##ModelId=3973073903DF
	CString m_propiedad;
	//##ModelId=3973073903D6
	CString m_propval;
};

#endif // !defined(AFX_ERROR_H__D3BE7463_D0B2_11D2_8BE1_00105A6690CF__INCLUDED_)
