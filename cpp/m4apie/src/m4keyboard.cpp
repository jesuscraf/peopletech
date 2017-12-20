// M4Keyboard.cpp : Implementation of CM4Keyboard
#include "stdafx.h"
#include "m4apie.h"
#include "m4keyboard.h"

/////////////////////////////////////////////////////////////////////////////
// CM4Keyboard


//##ModelId=396B32940259
STDMETHODIMP CM4Keyboard::KeyStatus(M4KeyEnum nKey, VARIANT_BOOL * pRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	short nKeyState = GetKeyState(nKey);

	*pRet = nKeyState & 0x8000;

	return S_OK;
}
