//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4clflag.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:7/17/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    Control de flags. Un flag-un bit.
//
//
//==============================================================================

#ifndef __M4CLFLAG__H__
#define __M4CLFLAG__H__

#include "mit.h"

/*
  Esta clase permite almacenar un flag por bit. Para asignar los flags, nos creamos
enums utilizando la enum BITS, teniendo cuidado de no repetir ninguno. 
  La clase es muy rápida, pues tiene todos metodos inline, y admás operan con lógica de  
bits tipo OR, AND y NOT. 
  Todos los métodos pueden operar a la vez con varios flags: simplemente unir estos con el
operador OR de bits (el caracter |).
  Una forma de crear Flags es derivando clases, con lo que los enum son privados a ella:

class ClFlagFont : public M4ClFlag {
MIT_PUBLIC:
	enum {BOLD=BIT0, ITALIC=BIT1, UNDERLINE=BIT2} ;
} FlagFont  ;

FlagFont.On(ClFlagFont::BOLD|ClFlagFont::ITALIC) ;
 ....

  O bien, no nos creamos clases, pero los enum son globales. Esta es mejor, porque no 
crea más clases, pero hay que hacer que los enum no coincidan con ningún otro.

enum {FONT_BOLD=M4ClFlag::BIT0, FONT_ITALIC=M4ClFlag::BIT1, FONT_UNDERLINE=M4ClFlag::BIT2} ;
M4ClFlag FlagFont ;
FlagFont.On(FONT_BOLD|FONT_ITALIC) ;

*/

class M4_DECL_M4MIENV ClMIT_Flag  
{

	//ACCESO LIMITADO A INSPECTOR

MIT_PUBLIC:
	typedef m4uint32_t Value_t ;

MIT_PRIVATE:
	Value_t m_Value ;

MIT_PUBLIC:
	enum BITS 
	{ 
		BIT0 =0x00000001, BIT1 =0x00000002, BIT2 =0x00000004, BIT3 =0x00000008,
		BIT4 =0x00000010, BIT5 =0x00000020, BIT6 =0x00000040, BIT7 =0x00000080,
		BIT8 =0x00000100, BIT9 =0x00000200, BIT10=0x00000400, BIT11=0x00000800,
		BIT12=0x00001000, BIT13=0x00002000, BIT14=0x00004000, BIT15=0x00008000,

		BIT16=0x00010000, BIT17=0x00020000, BIT18=0x00040000, BIT19=0x00080000,
		BIT20=0x00100000, BIT21=0x00200000, BIT22=0x00400000, BIT23=0x00800000,
		BIT24=0x01000000, BIT25=0x02000000, BIT26=0x04000000, BIT27=0x08000000,
		BIT28=0x10000000, BIT29=0x20000000, BIT30=0x40000000, BIT31=0x80000000,

		NONE_BITS=0x00000000, ALL_BITS=0xFFFFFFFF
	} ;
	
	ClMIT_Flag() { Reset(NONE_BITS) ; }
	ClMIT_Flag(Value_t ai_iValue) { Reset(ai_iValue) ; }
	~ClMIT_Flag() {}
	virtual size_t GetSizeof() const { return sizeof(ClMIT_Flag) ; }

	void Reset(Value_t ai_iValue=NONE_BITS) { m_Value=ai_iValue; }
	Value_t Value() const { return m_Value; }

	m4bool_t True(Value_t ai_iMask) const { return TrueAND(ai_iMask) ; }
	m4bool_t False(Value_t ai_iMask) const {return FalseAND(ai_iMask) ; }
	m4bool_t TrueAND(Value_t ai_iMask) const {return M4_BOOL((m_Value & ai_iMask)== ai_iMask) ; }
	m4bool_t TrueOR(Value_t ai_iMask) const {return M4_BOOL((m_Value & ai_iMask)!= NONE_BITS) ; }
	m4bool_t FalseAND(Value_t ai_iMask) const {return M4_BOOL(!TrueOR(ai_iMask)); }
	m4bool_t FalseOR(Value_t ai_iMask) const {return M4_BOOL(!TrueAND(ai_iMask)); }

	void On(Value_t ai_iMask) { m_Value=( (m_Value & (~ai_iMask))|ai_iMask ) ; }
	void Off(Value_t ai_iMask) { m_Value=( (m_Value & (~ai_iMask)) ) ; }

	void Switch(Value_t ai_iMask, m4bool_t ai_bSw) { ai_bSw ? On(ai_iMask) : Off(ai_iMask) ; }

};

#endif
