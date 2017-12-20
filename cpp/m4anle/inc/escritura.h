// Escritura.h: interface for the CEscritura class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ESCRITURA_H__BAA7B503_A92E_11D2_BCAE_00A0248A69E8__INCLUDED_)
#define AFX_ESCRITURA_H__BAA7B503_A92E_11D2_BCAE_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "m4stl.hpp"
#include "messages.h"


//##ModelId=3973073A0188
class CEscritura  
{
public:
	//##ModelId=3973073A0250
	short Apertura(CString pfichero);
	//##ModelId=3973073A0247
	short Inicial();
	//##ModelId=3973073A0246
	void Cierrfich();
	//##ModelId=3973073A021E
	void Escribe(int linea, CString texto, int codigo, CString objeto, CString padre, CString propiedad, CString propval);
	//##ModelId=3973073A0200
	void WriteToFile(int linea, CString texto, CString objeto, CString padre, CString propiedad, CString propval);
	//##ModelId=3973073A01F7
	long Compabierto() { return m_NumError; };
	//##ModelId=3973073A01ED
	CEscritura(CString pfichero,CPilaErrores *pila, CMessages * pMessages);
	//##ModelId=3973073A01E4
	virtual ~CEscritura();
	//##ModelId=3973073A01E2
	void Enable(BOOL enable);
	//##ModelId=3973073A01D8
	long GenFileFromPila();
private:
	FILE* m_pFile;
	//##ModelId=3973073A01B0
	CString m_NombreFich;
	//##ModelId=3973073A01A7
	long m_NumError;
	//##ModelId=3973073A01A6
	BOOL m_enable; //Habilitado o deshabilitado
	//##ModelId=3973073A019E
	CPilaErrores *m_pila; //Pila para guardar los errores
	//##ModelId=3973073A0189
	BOOL m_firsterr;

	CMessages * m_pMessages; // Referencia al traductor de mensajes

};

#endif // !defined(AFX_ESCRITURA_H__BAA7B503_A92E_11D2_BCAE_00A0248A69E8__INCLUDED_)
