// Lectura.h: interface for the CLectura class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LECTURA_H__7E939B13_A46F_11D2_BCAB_00A0248A69E8__INCLUDED_)
#define AFX_LECTURA_H__7E939B13_A46F_11D2_BCAB_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define  ESPACIO_BLANCO			1
#define  CR_NL					2
#define	 CR_NL_TAB_BLANCO		3
#define  TAB					4
#define  TAB_BLANCO				5
#define  EOFILE				   -1

#include "m4stl.hpp"
//##ModelId=3973073402F4
class CLectura  
{
public:
	//##ModelId=397307350041
	short Apertura(const char *pfichero);
	//##ModelId=397307350040
	CString Lpalabra();
	//##ModelId=39730735002E
	short Comprob(char caract, short tipo);
	//##ModelId=397307350024
	short Compr(char caract, short tipo);
	//##ModelId=397307350022
	CString Palabra(const char *pfichero);
	//##ModelId=397307350019
	char Lultimo() {return m_Caracter;};
	//##ModelId=397307350018
	short Finfich() {return (short)m_Termina;};
	//##ModelId=39730735000F
	void Termfich() {m_Termina=1;};
	//##ModelId=39730735000E
	int Numlinea() {return m_nLineas;};
	//##ModelId=397307350005
	void Cierrfich();
	//##ModelId=397307350004
	short Finalinea();
	//##ModelId=3973073403E4
	CLectura();
	//##ModelId=3973073403E2
	virtual ~CLectura();
	//##ModelId=3973073403DB
	short Leecaract();
private:
	FILE* m_pFile;
	//##ModelId=39730734034C
	int m_nCaract;
	//##ModelId=397307340344
	int m_nLineas;
	//##ModelId=397307340343
	char m_Caracter;
	//##ModelId=397307340342
	int m_Termina;
	//##ModelId=397307340338
	short m_NuevaLinea;

};

#endif // !defined(AFX_LECTURA_H__7E939B13_A46F_11D2_BCAB_00A0248A69E8__INCLUDED_)
