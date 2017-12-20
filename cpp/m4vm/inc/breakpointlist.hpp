//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           VM 
// File:             breakpointlist.hpp
// Project:          m4vm
// Author:           Meta Software M.S. , S.A
// Date:             12/03/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Execution breakpoint
//
//
//==============================================================================
#ifndef __BREAKPOINTLIST_HPP__
#define __BREAKPOINTLIST_HPP__

#include "m4vm_dll.hpp"

#include "breakpoint.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"


typedef set<ClBreakpoint, less<ClBreakpoint> > BreakpointSet_t;
typedef BreakpointSet_t::iterator BreakpointSetIt_t;

class ClGenericIODriver;
/**
 * Defines the list of breakpoints
 */
class M4_DECL_M4VM ClBreakpointList {

public:

	ClBreakpointList (){};
	virtual ~ClBreakpointList (){}; //el set es miembro, no tenemos que hacer nada

	m4return_t	Add(const ClBreakpoint &ai_oBP);

	m4return_t		Remove	(const ClBreakpoint &ai_oBP);
	ClBreakpoint*	Get		(const ClBreakpoint &ai_oBP); //retorna null si no está y un puntero al objeto de la lista si existe uno igual que el de entrada.
	
	//ojo, estos que son por ID, solo tienen sentido para iterar, pero cachearse el ID 
	//no tiene sentido porque cuando se inserta/borra un elemento cambian los ID.
	m4return_t	Remove	(m4uint32_t ai_iId);
	m4bool_t	Get		(m4uint32_t ai_iId, ClBreakpoint &ao_oBP);

	m4return_t	RemoveAll(void);
	m4uint32_t	GetSize	(void);

	m4return_t	Serialize	(ClGenericIODriver& IOD);
	m4return_t	DeSerialize	(ClGenericIODriver& IOD);

protected:

	BreakpointSet_t m_oSet;

};

/*
Algunas notas sobre como se usa esta clase:

  En esta clase se meten todos los BP del stepper.
  Estos BP pueden meterse desde distintos sitios, y según el sitio desde el que se metan
  se tendrán que meter usando unas fechas o usando otras:
  * Si se meten desde el Front-end, es decir, los mete el usuario debe meter siempre las fechas
    reales de la regla
  * Si las mete el propio stepper o VM,ya metemos las reales de la regla (= que desde el front_end)

  A la hora de buscar un BP en la lista se usarán las de RT del tramo que se esté ejecutando,
  esto es las del state, y como para el orden (operador <) se usa ver si las dos fechas son menores
  no hay problema.
*/
#endif //__BREAKPOINTLIST_HPP__

