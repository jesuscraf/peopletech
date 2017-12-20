//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              All
// File:                m4typesext.hpp   
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
//    This module set the typedefs extensions for all the proyect
//
//
//==============================================================================


#ifndef _M4TYPESEXT_H_
#define _M4TYPESEXT_H_


//==============================================================================
// Extension en PRUEBA: "M4_BOOL"
//==============================================================================
// Esta definicion deberia ir al final de "m4types.hpp"
#ifdef _M4_NO_BOOL
#define M4_BOOL(x)	((x) ? M4_TRUE : M4_FALSE)
#else
#define M4_BOOL(x)	(x)
#endif

#ifdef _UNIX

//==============================================================================
// Extension en PRUEBA: "VARIANT de Windows para UNIX"
//==============================================================================
// VARIANT (no existe en UNIX, pero lo intentamos) :

//#ifndef _VARIANT_2_UNIX_	//OLD implementation
//#define _VARIANT_2_UNIX_
#ifdef _VARIANT_2_UNIX_

	// Necesitamos unos cuantos tipos antes: 

	typedef short		VARIANT_BOOL;
	typedef m4bool_t	_VARIANT_BOOL;
	typedef LONG		SCODE;

	typedef m4int64_t		LONGLONG;
	typedef short			SHORT;
	typedef double			DOUBLE;

	/* real definition that makes the C++ compiler happy */
	typedef union tagCY {
		struct {
	#ifdef _MAC
			long      Hi;
			long Lo;
	#else
			unsigned long Lo;
			long      Hi;
	#endif
		} _STRUCT_MAC;
		LONGLONG int64;
	} CY;


	/* VARIANT STRUCTURE
	 *
	 *  VARTYPE vt;
	 *  WORD wReserved1;
	 *  WORD wReserved2;
	 *  WORD wReserved3;
	 *  union {
	 *    LONG           VT_I4
	 *    BYTE           VT_UI1
	 *    SHORT          VT_I2
	 *    FLOAT          VT_R4
	 *    DOUBLE         VT_R8
	 *    VARIANT_BOOL   VT_BOOL
	 *    SCODE          VT_ERROR
	 *    CY             VT_CY
	 *    DATE           VT_DATE
	 *    BSTR           VT_BSTR
	 *    IUnknown *     VT_UNKNOWN
	 *    IDispatch *    VT_DISPATCH
	 *    SAFEARRAY *    VT_ARRAY
	 *    BYTE *         VT_BYREF|VT_UI1
	 *    SHORT *        VT_BYREF|VT_I2
	 *    LONG *         VT_BYREF|VT_I4
	 *    FLOAT *        VT_BYREF|VT_R4
	 *    DOUBLE *       VT_BYREF|VT_R8
	 *    VARIANT_BOOL * VT_BYREF|VT_BOOL
	 *    SCODE *        VT_BYREF|VT_ERROR
	 *    CY *           VT_BYREF|VT_CY
	 *    DATE *         VT_BYREF|VT_DATE
	 *    BSTR *         VT_BYREF|VT_BSTR
	 *    IUnknown **    VT_BYREF|VT_UNKNOWN
	 *    IDispatch **   VT_BYREF|VT_DISPATCH
	 *    SAFEARRAY **   VT_BYREF|VT_ARRAY
	 *    VARIANT *      VT_BYREF|VT_VARIANT
	 *    PVOID          VT_BYREF (Generic ByRef)
	 *    CHAR           VT_I1
	 *    USHORT         VT_UI2
	 *    ULONG          VT_UI4
	 *    INT            VT_INT
	 *    UINT           VT_UINT
	 *    DECIMAL *      VT_BYREF|VT_DECIMAL
	 *    CHAR *         VT_BYREF|VT_I1
	 *    USHORT *       VT_BYREF|VT_UI2
	 *    ULONG *        VT_BYREF|VT_UI4
	 *    INT *          VT_BYREF|VT_INT
	 *    UINT *         VT_BYREF|VT_UINT
	 *  }
	 */
	#define __VARIANT_NAME_1 n1
	#define __VARIANT_NAME_2 n2
	#define __VARIANT_NAME_3 n3
	#define __tagVARIANT
	typedef /* [wire_marshal] */ struct tagVARIANT VARIANT;

	// VARIANT - reducido para que trague.... pero no es utilizable!!!
	struct  tagVARIANT
		{
		union 
			{
			struct  __tagVARIANT
				{
				WORD wReserved1;
				WORD wReserved2;
				WORD wReserved3;
				union 
					{
					LONG lVal;
					BYTE bVal;
					SHORT iVal;
					FLOAT fltVal;
					DOUBLE dblVal;
					}		__VARIANT_NAME_3;
				}		__VARIANT_NAME_2;
			}	__VARIANT_NAME_1;
	};

	typedef VARIANT *	LPVARIANT;
	typedef VARIANT		VARIANTARG;
	typedef VARIANT *	LPVARIANTARG;


#else	// _VARIANT_2_UNIX_

	//Definicion Simple
	typedef int VARIANT;

#endif	// _VARIANT_2_UNIX_


#endif	// _UNIX



#endif

