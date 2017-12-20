// Copyright (C) 1991 - 1999 Rational Software Corporation

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_HOOK_DATA_396891EA035A_INCLUDED
#define _INC_HOOK_DATA_396891EA035A_INCLUDED

//##ModelId=396DF30E0249
struct HOOK_DATA 
{
public:
	//##ModelId=396DF30E0286
	HWND hWnd;

	//##ModelId=396DF30E027C
	LONG_PTR WndProc;

	//##ModelId=396DF30E0272
	long nRefCount;

	//##ModelId=396DF30E026A
	HOOK_DATA* pNext;

};

#endif /* _INC_HOOK_DATA_396891EA035A_INCLUDED */

