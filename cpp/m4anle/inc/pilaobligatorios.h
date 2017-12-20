// PilaObligatorios.h: interface for the CPilaObligatorios class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PILAOBLIGATORIOS_H__1AD9B283_D2DF_11D2_8BED_00105A6690CF__INCLUDED_)
#define AFX_PILAOBLIGATORIOS_H__1AD9B283_D2DF_11D2_8BED_00105A6690CF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "objeto.h"

//CAMBIOS PARA VERSION 3

typedef struct __chk_metodo__
{
	int objeto;
	CString propiedad;
	CString valor;
	BOOL failpath;
	BOOL failruntimeproperty;
	BOOL failruntimemethod;
}CHK_METODO;

//##ModelId=397307400303
class CMetodo : public CObject
{
public:
	//##ModelId=397307400321
	CMetodo(CString nombre, CString metodo, int padre, CString expresion);

	//##ModelId=397307400318
	CString m_nombre;
	//##ModelId=397307400317
	CString m_metodo;
	//##ModelId=39730740030F
	int m_padre;
	//##ModelId=39730740030E
	CString m_expresion;
};


//##ModelId=39730742034C
class CPilaObligatorios  
{
public:
	//##ModelId=397307430056
	CPilaObligatorios();
	//##ModelId=397307430054
	virtual ~CPilaObligatorios();

	//##ModelId=39730743004A
	void Cargar(CRuntime *runtime);
	//##ModelId=397307430036
	short NewObjeto(CString objeto,CString alias,int identificador,int padre);
	//##ModelId=397307430018
	short NewPropiedad(int objeto, CString propiedad, CString valor, CString *expresion, BOOL IsMetodo);
	//##ModelId=397307430005
	void AddObjObligatorio(int objpadre, OBJ_OBLIGATORIO *obligatorio);
	//##ModelId=3973074203E2
	void AddPropObligatoria(int objpadre, CString propiedad);
	//##ModelId=3973074203CE
	void GetObjObligatorio(int objeto,int num, CString &classname);
	//##ModelId=3973074203BA
	void GetPropObligatoria(int objeto,int num, CString &propiedad);
	//##ModelId=3973074203B0
	short CheckObjObligatorios(int objeto);
	//##ModelId=3973074203A7
	short CheckObjMaxObligatorios(int objeto);
	//##ModelId=39730742039D
	short CheckPropObligatorias(int objeto);
	//##ModelId=39730742039C
	int GetNumMetodos();
	//##ModelId=397307420392
	short CheckMetodo(int nummetodo, CHK_METODO *info);

private:
	//##ModelId=397307420380
	CObjeto *m_objraiz;
	//##ModelId=39730742036C
	CObList m_listmetodos;
	//##ModelId=397307420362
	CRuntime *m_runtime;
};

#endif // !defined(AFX_PILAOBLIGATORIOS_H__1AD9B283_D2DF_11D2_8BED_00105A6690CF__INCLUDED_)
