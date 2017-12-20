//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              clc
// File:                vmtypes.hpp   
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                15-05-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module set the typedefs for all the VM
//
//==============================================================================

#ifndef __VMTYPES_HPP__
#define __VMTYPES_HPP__

#include "m4stl.hpp"

#include "vmdefine.hpp"
#include "cltypes.hpp"
#include "gram_tab.h"
#include "m4vmstate_t.hpp"

class ClVMachine0;
class ClBlock_Channel_Proxy;
class ClVMItemAttribute;
class ClCompiledMCR;

// *********Compiler

//OJO->Podemos usar este typedef porque la clase no tiene miembros virtuales y los punteros ocupar 4 bytes
//que es lo que toma cuando solo tiene la información de class x;
typedef m4return_t ((ClVMItemAttribute ::*pmfAttrib) (void)  );

//Partes de un item
typedef struct m4ItemComp_tag{
	m4char_t m_acItem[MAX_TOKEN];
	m4char_t m_acNode[M4CL_MAX_LEN_NODE+1];
	m4char_t m_acRule[M4CL_MAX_LEN_RULE+1];
	m4char_t m_acChannel[M4CL_MAX_LEN_CHANNEL+1];
	m4bool_t m_bTrame; //indica si tiene o no tramo, 
					   //pero no el tramo ya que es 
						//una expresion tan compleja como se desee
	m4bool_t m_bRow;	//indica si lleva fila o no
}m4ItemComp_t;

//Ojo, supongo que tenemos definido STD (en Windows está)
//#pragma warning(disable:4786)
#include "m4stl.hpp"
#include "m4string.hpp"

typedef map<M4ClString,m4int32_t,less<M4ClString> > MapaSimbolos;
typedef MapaSimbolos::iterator itMapaSimbolos;

typedef map<M4ClString,m4VariantType,less<M4ClString> > MapaSimbolosVariant;
typedef MapaSimbolosVariant::iterator itMapaSimbolosVariant;


//typedef stack<int> PilaInt; 
typedef stack<int, list<int> > PilaInt;
typedef list<int> ListaInt;

typedef map<string , ClCompiledMCR *, less <string> > 	MapMetaChannelforJIT;
typedef MapMetaChannelforJIT::iterator  itMapChannelJIT;

typedef vector< ClItemIndex >					VectorItemIndex;


#endif

