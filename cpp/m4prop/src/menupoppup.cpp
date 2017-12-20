// MenuPopPup.cpp : Implementation of CMenuPopPup
#include "stdafx.h"
#include "m4prop.h"
#include "menupoppup.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuPopPup

STDMETHODIMP CMenuPopPup::AddNewMenuItem(BSTR* strCaption,BSTR* strKey,VARIANT_BOOL * bEnabled,VARIANT_BOOL * bChecked,VARIANT_BOOL * bVisible,VARIANT_BOOL * bBold,VARIANT_BOOL * bRet)
{
    /*Dim tmpMenuItem As CMenuItem
    Set tmpMenuItem = New CMenuItem
    
    tmpMenuItem.Caption = strCaption
    tmpMenuItem.Key = strKey
    tmpMenuItem.Bold = bBold
    tmpMenuItem.Checked = bChecked
    tmpMenuItem.Enabled = bEnabled
    tmpMenuItem.Visible = bVisible
    On Error Resume Next
        Err.Clear
        If strKey <> "" Then
            m_colMenu.Add tmpMenuItem, strKey
        Else
            m_colMenu.Add tmpMenuItem
        End If
        If Err.Number = 0 Then
            AddNewMenuItem = True
        Else
            AddNewMenuItem = False
            Err.Clear
        End If
    On Error GoTo 0*/

	IMenuItem * pMenuItem = NULL;
	HRESULT hr = S_OK;

	hr = CoCreateInstance(CLSID_MenuItem, NULL, CLSCTX_ALL, IID_IMenuItem, (void**)&pMenuItem);

	if (hr == S_OK)
	{
		pMenuItem->put_Caption(*strCaption);
		pMenuItem->put_Key(*strKey);
		pMenuItem->put_Bold(*bBold);
		pMenuItem->put_Checked(*bChecked);
		pMenuItem->put_Enabled(*bEnabled);
		pMenuItem->put_Visible(*bVisible);

		if (::SysStringLen(*strKey) == 0)
		{
			// No tenemos Key
			CComVariant v;
			v.vt = VT_EMPTY;
			hr = Add(pMenuItem, v);
		}
		else
		{
			// Tenemos Key
			hr = Add(pMenuItem, CComVariant(*strKey));
		}
		if (hr == S_OK)
		{
			*bRet = -1; // TRUE
		}
		else
		{
			*bRet = 0; // False
		}
		RELEASE(pMenuItem);
	}

	return S_OK;
}

STDMETHODIMP CMenuPopPup::MenuEventClick(BSTR * strKey)
{
	// TODO: Add your implementation code here
	Fire_MenuClick(strKey);
	return S_OK;
}

STDMETHODIMP CMenuPopPup::Reset()
{
	// TODO: Add your implementation code here
	RemoveAll();
	return S_OK;
}
