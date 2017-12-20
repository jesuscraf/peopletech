// pila.h: interface for the Cpila class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PILA_H__BA950A33_AA3A_11D2_BCAF_00A0248A69E8__INCLUDED_)
#define AFX_PILA_H__BA950A33_AA3A_11D2_BCAF_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "m4stl.hpp"
#include "dinarray.h"

// Esta es la estructura a utilizar en
// la pila de objetos.

typedef	struct str_pila {
		int id_end;
		int id_obj;
		int id_objpadre;
		CString alias_objeto;
		CString path_obj;
		CString nom_obj;
		int nlinea;
		short allproperties;
		short allobject;
} pila;


//##ModelId=39730746036F
class Cpila  
{
private:
	//##ModelId=397307470001
	void BorrEstr(DinArray& estruc);
	//##ModelId=397307460386
	DinArray m_DApila;
	
public:
	//##ModelId=3973074603E0
	void CargEstr(pila* estruc);
	//##ModelId=3973074603DE
	void DarEstr(int pospila);
	//##ModelId=3973074603D5
	int DarPadre(int posicion);
	//##ModelId=3973074603CC
	int DarObjeto(int posicion);
	//##ModelId=3973074603CA
	int DarLineas(int posicion);
	//##ModelId=3973074603C1
	CString DarNomObjeto(int posicion);
	//##ModelId=3973074603B8
	CString DarPathobj(int posicion);
	//##ModelId=3973074603B6
	CString DarPathalias(int posicion);
	//##ModelId=3973074603AD
	int BusEnd(int posicion);
	//##ModelId=3973074603A4
	void EncEnd(int pospila);
	//##ModelId=3973074603A2
	short DarAllPro(int posicion);
	//##ModelId=397307460399
	short DarAllObj(int posicion);
	//##ModelId=397307460398
	Cpila();
	//##ModelId=39730746038E
	virtual ~Cpila();

};

#endif // !defined(AFX_PILA_H__BA950A33_AA3A_11D2_BCAF_00A0248A69E8__INCLUDED_)
