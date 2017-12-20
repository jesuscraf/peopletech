//==============================================================================
//
// (c) Copyright  1991-1998 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo> 
// File:             m4loadll.hpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:6/28/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
//## begin module.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module.cm

//## begin module.cp preserve=no
//## end module.cp

//## Module: m4loadll; Package specification
//## Subsystem: <Top Level>
//## Source file: X:\m4rel\m4source\3x_work\core\communication\working\maite\m4loadll\inc\m4loadll.hpp

#ifndef m4loadll_h
#define m4loadll_h 1

//## begin module.additionalIncludes preserve=no
//## end module.additionalIncludes

//## begin module.includes preserve=yes
#include "m4loadll_dll.hpp"
#include "m4types.hpp"
#include "m4win.hpp"

#ifdef _UNIX
#include <dlfcn.h>
#endif
//## end module.includes

//## begin module.declarations preserve=no
//## end module.declarations

//## begin module.additionalDeclarations preserve=yes
#ifdef _UNIX
typedef void*	m4typefunreturn;
typedef void*	handle;
#else
typedef FARPROC		m4typefunreturn;
typedef HINSTANCE	handle;
#endif
//## end module.additionalDeclarations


//## Class: M4ClLoaDll
//	Clase que permite cargar una librería dinámica desde
//	dentro de una librería dinámica.
//## Category: <Top Level>
//## Subsystem: <Top Level>
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4LOADLL M4ClLoaDll
{
  //## begin M4ClLoaDll.initialDeclarations preserve=yes
  //## end M4ClLoaDll.initialDeclarations

  public:
	  const m4bool_t get_m_bIsOpenDll (void) const
	  {
		  return m_bIsOpenDll;
	  }

    //## Constructors (specified)
      //## Operation: M4ClLoaDll%884371568
      //	Constructor  de la clase , no hace nada realmente
      M4ClLoaDll (void);

    //## Destructor (specified)
      //## Operation: ~M4ClLoaDll%884371569
      //	En el destuctor  se cierra la  librería si previamente
      //	no habíamos hecho un clos.
      ~M4ClLoaDll (void);


    //## Other Operations (specified)
      //## Operation: Open%884371570
      //	Esta función carga la librería dinámica  cuyo nombre le
      //	pasamos como parámetro.
      m4return_t Open (m4pcchar_t ai_pccNameDll);

      //## Operation: Close%884371571
      //	Función que cierra la librería dinámica .
      m4return_t Close (void);

      //## Operation: GetFunc%884371572
      //	Esta función obtiene un puntero a la función que le
      //	indicamos en el parámetro de entrada y que pertenece a
      //	la librería dinámica.
      m4return_t  GetFunc (m4pcchar_t ai_pccNameFunc, m4typefunreturn   &ao_AddressFunc);

    // Additional Public Declarations
      //## begin M4ClLoaDll.public preserve=yes
      //## end M4ClLoaDll.public

  private:
    //## Get and Set Operations for Class Attributes (generated)

      //## Attribute: m_bIsOpenDll
      void set_m_bIsOpenDll (m4bool_t value)
	  {
		  m_bIsOpenDll = value;
	  }

      //## Attribute: m_hDllHandle
	  const handle get_m_hDllHandle (void) const
	  {
		  return m_hDllHandle;
	  }

      void set_m_hDllHandle (handle value)
	  {
		  m_hDllHandle = value;
	  }

    // Additional Private Declarations
      //## begin M4ClLoaDll.private preserve=yes
      //## end M4ClLoaDll.private

  private:  //## implementation
    // Data Members for Class Attributes

      //## begin M4ClLoaDll::m_bIsOpenDll.attr preserve=no  private: m4bool_t {U} 
      m4bool_t m_bIsOpenDll;
      //## end M4ClLoaDll::m_bIsOpenDll.attr

      //## begin M4ClLoaDll::m_hDllHandle.attr preserve=no  private: handle {U} 
      handle m_hDllHandle;
      //## end M4ClLoaDll::m_hDllHandle.attr

    // Additional Implementation Declarations
      //## begin M4ClLoaDll.implementation preserve=yes
      //## end M4ClLoaDll.implementation
};

//## begin M4ClLoaDll.postscript preserve=yes
//## end M4ClLoaDll.postscript


//## begin module.epilog preserve=yes
//## end module.epilog
#endif
