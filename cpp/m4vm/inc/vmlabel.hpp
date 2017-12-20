//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           vmlabel.hpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    Declaración de la clase label. Esta clase es por si en un futuro en lugar
//    de usa etiqueta=cahr 255 cambia la estructura
////
//==============================================================================



#ifndef _CLVMLABEL_H
#define _CLVMLABEL_H

#include "vmtypes.hpp"
#include "vmdefine.hpp"

class ClVMStatusLabel{

public:
	m4return_t SetLabel(m4pcchar_t ai_pszLabel);
	m4return_t GetLabel(m4pchar_t ao_pszLabel, m4uint32_t ai_iSize);
protected:
	char m_acLabel[M4CL_MAX_STRING];
};

#endif

