//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             reportenv.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/31/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
//
//
//==============================================================================

#ifndef __REPORTENV_H__
#define __REPORTENV_H__

//=============================================================================StENG_ReportEnv

// Entorno de ejecución de report, objetos que una instancia de report precisa. 
// Se aislan en una estructura para poderlos pasar a otras clases que precisan de estos datos,
//	sin tener que pasarles el puntero al objeto ClENG_Report en sí (pues en estecaso se podría 
//	manipular el objeto ilegalmente). En este contexto, siempre se pasan los objetos para
//	ser utilizados, nunca para crearlos ni destruirlos.
// La existencia de esta estructura solo obedece a la necesidad de externalizar unos 
//	miembros de ClENG_Report, ocultando otros. Y el hecho de que todos sen punteros en vez de
//	objetos es simplemente para evitar arboles de inclusion grandes en el proyecto.

class ClFONT_M_DBFont ;
class ClENG_DBFormat ;
class ClENG_OutDevice ;
class ClMIT_Chn ;
class ClENG_ReportArgs ;
class ClENG_FieldsManager ;

struct StENG_ReportEnv 
{
	ClFONT_M_DBFont		*m_poDBFont ;
	ClENG_DBFormat		*m_poDBFormat ;
	ClENG_OutDevice		*m_poDvc ;		//NULL si no se pudo inicializar, el resto siempre creado.
	ClMIT_Chn			*m_poChnData ;
	ClENG_ReportArgs	*m_poExeParams ;
	ClENG_FieldsManager *m_poFieldManager ;	

	typedef map<ClMIT_Str,ClMIT_Str, less<ClMIT_Str> > TrLitMap_t;

	TrLitMap_t			*m_poTrLit ;
	
	StENG_ReportEnv() ;	//No construye m_poDvc, es virtual. El resto lo construye.
	~StENG_ReportEnv() ;//Destruye todo.
} ;

#endif

