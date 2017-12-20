// stdafx.cpp : source file that includes just the standard includes
//  stdafx.pch will be the pre-compiled header
//  stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "definitions_i.c"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

// #include <atlimpl.cpp>
#include <stdafx.h>

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

#ifdef __cplusplus
}
#endif

