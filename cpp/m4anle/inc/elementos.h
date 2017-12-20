// Elementos.h: interface for the CElementos class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ELEMENTOS_H__2DDDA173_A606_11D2_BCAC_00A0248A69E8__INCLUDED_)
#define AFX_ELEMENTOS_H__2DDDA173_A606_11D2_BCAC_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "lectura.h"
#include "pila.h"
#include "compobjects.h"
#include "comptype.h"
#include "pilaobligatorios.h"
#include "runtime.h"
#include "messages.h"
#include "escritura.h"

//##ModelId=3973073F01AD
class CElementos  
{
public:
	// Analiza el fichero que le pasamos
	//##ModelId=397307400028
	long Compilar(const char *pfichero, CPilaErrores *pilaerror, BOOL genlog, const char *pfilelog = NULL);
	// Verifica que el alias de un objeto es correcto sintácticamente
	//##ModelId=397307400000
	short CompAlias(CString palab, char &ult, CString &texterror);
	// Apila un objeto
	//##ModelId=3973073F03AB
	short Apilar(int li_obj, CString alias, int linea, CString nombobj, short allprop, short allobj);
	// Apila un error
	//##ModelId=3973073F03A1
	short ApilarError();
	// Crea una estructura de la pila
	//##ModelId=3973073F0397
	void CreaEstr(pila*& estruc);	
	//##ModelId=3973073F0352
	CElementos(long max_error, short errorlevel, ICPropBag * pPBTypesrec, ICPropBag * pPBPropertiesgrouprec, ICPropBag * pPBObjectsgrouprec, ICPropBag * pPBObjectsrec, ICPropBag * pPBMessagesrec);
	//##ModelId=3973073F0347
	virtual ~CElementos();
private:
	//##ModelId=3973073F033D
	void AnalyzeRunTime(CEscritura *error);
	//##ModelId=3973073F0329
	long ShowNumErrors(CEscritura *error, BOOL writeFich);
	//##ModelId=3973073F0315
	void ProcessPresentation(CEscritura *error, CPilaErrores *pilaerror);
private:
	//##ModelId=3973073F02BB
	CString MsgTexto (CString cadena, CString objeto, CString objeto_padre, int num_linea, CString propiedad, CString value_propiedad, CString codigo_error);
	//##ModelId=3973073F02B1
	short m_Errorlevel;		// Nivel de error analizar.
	//##ModelId=3973073F029D
	CString m_NombreFich;	// Nombre del fichero	
	//##ModelId=3973073F0293
	int m_padre;			// Nodo padre	
	//##ModelId=3973073F027F
	int m_pila;				// Nivel de la pila.	
	//##ModelId=3973073F0275
	CString m_ObjPath;		// Path del objeto
	//##ModelId=3973073F026B
	CString m_AliasPath;	// Path de alias
	//##ModelId=3973073F024F
	CLectura m_leer;		// Variable para leer del fichero
	//##ModelId=3973073F023B
	Cpila m_pilar;			// Para apilar los objetos
	//##ModelId=3973073F0225
	int m_tope;				// Tope de errores
	//##ModelId=3973073F021B
	CCompObjects m_Cpobjects;
	//##ModelId=3973073F01FF
	CCompType m_CComType;
	//##ModelId=3973073F01EB
	CPilaObligatorios m_PilaOblig;
	//##ModelId=3973073F01D7
	CRuntime m_Crun;
	//##ModelId=3973073F01C3
	CMessages m_CMessages;
};

#endif // !defined(AFX_ELEMENTOS_H__2DDDA173_A606_11D2_BCAC_00A0248A69E8__INCLUDED_)
