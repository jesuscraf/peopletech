// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "definitions_i.c"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <stdafx.h>
#include "m4unicode.hpp"


void	M4BSTRToCString( const BSTR& ai_oInput, CString& ao_roOutput )
{
	char*	pcString = M4VBToCpp( ai_oInput ) ;
	ao_roOutput = pcString ;
	delete pcString ;
}


// Esta parte la metemos aquí por comodidad a la hora de encontrar definiciones
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__


// Declaramos los ClassId que vamos a usar en éste proyecto
const CLSID CLSID_CPropBag = {0x13313659,0x2584,0x11D2,{0x96,0x9E,0x00,0xC0,0x4F,0xC2,0x44,0x4B}};
const CLSID CLSID_ConvertPropBag = {0x1331365D,0x2584,0x11D2,{0x96,0x9E,0x00,0xC0,0x4F,0xC2,0x44,0x4B}};
const CLSID CLSID_CPropBagDef = {0x2D3708E9,0x64D8,0x11D2,{0xBC,0x7F,0x00,0xC0,0x4F,0x71,0x65,0x2D}};
const CLSID CLSID_DataProperties = {0xB4981954,0x0D0C,0x11D3,{0xBC,0xAA,0x00,0xC0,0x4F,0x71,0x65,0x2D}};
const CLSID CLSID_TraductProp = {0xD3A42354,0x0DC6,0x11D3,{0xBC,0xAB,0x00,0xC0,0x4F,0x71,0x65,0x2D}};
const CLSID CLSID_M4FontHolder = {0x06854C14,0x088F,0x11D3,{0xBC,0x9D,0x00,0xC0,0x4F,0x71,0x65,0x2D}};

#ifdef __cplusplus
}
#endif
