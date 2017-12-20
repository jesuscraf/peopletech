//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                TablFunc.cpp
// Project:        Cálculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:   
//
//
// Definition: 
//
//    This module defines las constantes de los identificadores de objetos implementadores
//    de VM's
////
//==============================================================================

#ifndef __VM_OBJECT_DEFINE_HPP__
#define __VM_OBJECT_DEFINE_HPP__

//comun para todos las vm
#define M4CL_VM_OBJECT_THIS 0  //el mismo

//--------------
//objetos de vm0
//--------------
#define M4CL_VM0_N_OBJECTS 5

//1l objeto 0 es el this
#define M4CL_VM0_COMPATIBILITY_DX		 1  //el que implementa las funciones de compatibilidad con 2.x
#define M4CL_VM0_STRING_MANIP			 2  //el que implementa las funciones de cadenas
#define M4CL_VM0_ERROR_IO				 3  //el que implementa las funciones de cadenas
#define M4CL_VM0_GENERAL				 4  //el que implementa las funciones generales

//--------------
//objetos de vm1
//--------------
#define M4CL_VM1_N_OBJECTS 10

#define M4CL_VM1_PROPAGATOR				1  //el que implementa la propagación
#define M4CL_VM1_LDB					2  //ldb 
#define M4CL_VM1_INDIRECTION			3  //Indirecciones
#define M4CL_VM1_COMPILER_JIT			4  //Compiler y JIT
#define M4CL_VM1_SORT_FILTER			5  //funciones de filtrados y ordenaciones, búsquedas
#define M4CL_VM1_CMCR					6  //funciones del metacanal
#define M4CL_VM1_REGISTER				7  //funciones de registro
#define M4CL_VM1_LIB					8  //basic lib
#define M4CL_VM1_PAYROLL				9  //payroll lib (av & curr)


//--------------
//objetos de vm2
//--------------
#define M4CL_VM2_N_OBJECTS 1

#endif

