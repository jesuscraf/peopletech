// Objeto.h: interface for the CObjeto class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJETO_H__1AD9B284_D2DF_11D2_8BED_00105A6690CF__INCLUDED_)
#define AFX_OBJETO_H__1AD9B284_D2DF_11D2_8BED_00105A6690CF__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//CAMBIOS PARA VERSION 3

typedef struct __obj_obligatorio__
	{
	CString nombre;
	long	insmax;
	long	insmin;
	}OBJ_OBLIGATORIO;

typedef struct __chk_constraint__
{
	BOOL failpath;
	BOOL failruntimeproperty;
	BOOL failruntimemethod;
}CHK_CONSTRAINT;

//##ModelId=397307450039
class CObjObligatorio : public CObject
{
public:
	//##ModelId=397307450059
	CObjObligatorio(CString nombre, long insmax, long insmin);

	//##ModelId=397307450058
	CString m_nombre;
	//##ModelId=397307450057
	long m_insmax; //nº de Instancias máximas
	//##ModelId=39730745004E
	long m_insmin; //nº de Instancias mínimas
	//##ModelId=39730745004D
	long m_insnum; //n1 de Instancias actual
};
	

//##ModelId=3973073D02FE
class CPropiedad : public CObject
{
public:
	//##ModelId=3973073D0313
	CPropiedad(CString nombre, CString valor);

	//##ModelId=3973073D0312
	CString m_nombre;
	//##ModelId=3973073D0309
	CString m_valor;
};

//##ModelId=3973073B0201
class CObjeto : public CObject
{
public:
	//##ModelId=3973073B03CF
	CObjeto(CString nombre, CString alias, int identificador, CRuntime *runtime);
	//##ModelId=3973073B03C4
	virtual ~CObjeto();

	//##ModelId=3973073B03BA
	CObjeto *FindObjeto(int id);
	//##ModelId=3973073B03B0
	CObjeto *FindChild(CString alias);
	//##ModelId=3973073B03A6
	void AddObjObligatorio(OBJ_OBLIGATORIO *objeto);
	//##ModelId=3973073B0392
	void AddPropObligatoria(CString propiedad);
	//##ModelId=3973073B037E
	void GetObjObligatorio(int num,CString &objeto);
	//##ModelId=3973073B036A
	void GetPropObligatoria(int num,CString &propiedad);
	//##ModelId=3973073B0356
	short AddChild(CObjeto *pobj);
	//##ModelId=3973073B0339
	short AddPropiedad(CString propiedad, CString valor, CString *expresion);
	//##ModelId=3973073B0338
	int GetNumObjObligatorios();
	//##ModelId=3973073B032F
	int GetNumMaxObjObligatorios();
	//##ModelId=3973073B032E
	int GetNumPropObligatorias();
	//##ModelId=3973073B0324
	BOOL IsPresent(LPCSTR propiedad);
	//##ModelId=3973073B0310
	CString& Value(LPCSTR propiedad);
	//##ModelId=3973073B0306
	CObjeto *GetRaiz();
	//##ModelId=3973073B02FB
	CObjeto *IsValidPath(LPCSTR path);
	//##ModelId=3973073B02C9
	BOOL CheckConstraint(CString propiedad, CString valor, CString *expresion, CHK_CONSTRAINT *info);

	//##ModelId=3973073B02BF
	CString m_nombre;
	//##ModelId=3973073B02AB
	CString m_alias;
	//##ModelId=3973073B02A1
	int m_identificador;
	//##ModelId=3973073B0285
	CObjeto *m_padre;
	//##ModelId=3973073B0271
	CRuntime *m_runtime;

private:
	//##ModelId=3973073B025D
	CObList m_listchild;
	//##ModelId=3973073B023F
	CObList m_listprop;
	//##ModelId=3973073B0235
	CObList m_objobligatorios;
	//##ModelId=3973073B0221
	CStringList m_propobligatorias;
};



#endif // !defined(AFX_OBJETO_H__1AD9B284_D2DF_11D2_8BED_00105A6690CF__INCLUDED_)
