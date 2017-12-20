// LocalID.cpp : Implementation of CM4LCIDApp and DLL registration.

#include "stdafx.h"
#include "M4LCID.h"
#include "LocalID.h"

static unsigned long SystemLCID;
static bool Created = false;

/////////////////////////////////////////////////////////////////////////////
//
CLocalID::CLocalID() 
{
	if (Created == false)
	{
		SystemLCID = GetThreadLocale();	
		Created = true;
	}
	
	instance = NULL;
}

STDMETHODIMP CLocalID::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ILocalID,
	};

	for (int i=0;i<sizeof(arr)/sizeof(arr[0]);i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CLocalID::SetLanguage(long LCID)
{
	
	if (IsWinNT())
	{
		SetThreadLocale(LCID);
	}	
	SystemLCID = LCID;
	return S_OK;
}


STDMETHODIMP CLocalID::GetCurrentDateFormat(BSTR *dateFormat)
{
	
	USES_CONVERSION;

	IM4RegKey	*m_reg = NULL;
	HRESULT		 hr = S_OK;
	char		*sDate = new char[ 200 + 1 ];

	GetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SSHORTDATE,sDate, 200);

	*dateFormat = A2BSTR(sDate);
	delete []sDate;

	return S_OK;
}

STDMETHODIMP CLocalID::SetCurrentDate(BSTR date)
{
	
	USES_CONVERSION;

	char *m_date = new char [200];
	m_date = OLE2A(date);

	//if (IsWinNT())
	//	SetLocaleInfo(SystemLCID,LOCALE_IDATE,m_date);
	//else
		SetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_IDATE,m_date);

	return S_OK;
}


STDMETHODIMP CLocalID::GetCurrentDate(BSTR *date)
{
	
	USES_CONVERSION;
	char *sdate = new char[100];
	int  longDate = 100, res = 0;
	long error;
	BSTR myFormat = NULL;

	GetCurrentDateFormat(&myFormat);

	//if (IsWinNT())
	//	res = GetDateFormat(SystemLCID,0,NULL,OLE2A(myFormat),sdate,longDate);
	//else
		res = GetDateFormat(LOCALE_USER_DEFAULT,0,NULL,OLE2A(myFormat),sdate,longDate);
	
	SysFreeString(myFormat);
	
	if (res == 0)
	{
		error = GetLastError();
		*date = ::SysAllocString(A2OLE(""));
	}
	else
		*date = A2BSTR(sdate);
	
	delete []sdate;
	return S_OK;
}

STDMETHODIMP CLocalID::SetCurrentDateFormat(BSTR dateFormat)
{
	
	USES_CONVERSION;

	IM4RegKey	*m_reg = NULL;
	HRESULT		 hr;
	
	hr = CoCreateInstance(CLSID_M4RegKey, NULL, CLSCTX_ALL, IID_IM4RegKey, (void**)&m_reg);
	if (hr != S_OK)
		return S_OK;
	
	BSTR		 m_mainKey = ::SysAllocString(L"HKEY_CURRENT_USER");
	BSTR	     m_subKey = ::SysAllocString(L"Control Panel\\International");
	BSTR	     m_in = ::SysAllocString(L"sShortDate");
	VARIANT	     vIn;

	::VariantInit(&vIn);

	m_reg->put_MainKey (m_mainKey);
	m_reg->put_SubKey (m_subKey);
	m_reg->OpenKey();

	vIn.vt = VT_BSTR;
	vIn.bstrVal = SysAllocString(dateFormat);
	
	// Rellenamos la información en el registro.
	hr = m_reg->put_Value(m_in,vIn);
	
	VariantClear(&vIn);

	::SysFreeString(m_mainKey);
	::SysFreeString(m_subKey);
	::SysFreeString(m_in);
	
	m_reg->Release();
	
	// Asignamos las propiedades del Sistema
	//if (IsWinNT())
	//	SetLocaleInfo(SystemLCID,LOCALE_SSHORTDATE,OLE2A(dateFormat));
	//else
		SetLocaleInfo(LOCALE_USER_DEFAULT,LOCALE_SSHORTDATE,OLE2A(dateFormat));

	// Notificamos a las ventanas que algo ha cambiado en el sistema
	SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, NULL, NULL);

	return S_OK;
}


STDMETHODIMP CLocalID::GetLanguage(long *LCID)
{
	(*LCID) = SystemLCID;
	
	return S_OK;
}

STDMETHODIMP CLocalID::LoadResourceString(long IdRes, long hInst, BSTR *resource)
{
	USES_CONVERSION;

	HRSRC siteResource;
	long m_hInst;
	int section = 0;
	int index = 0;

	section = (int)(IdRes / 16) + 1;
	index = IdRes % 16;

	if (hInst == 0)
		m_hInst = (long)instance;
	else m_hInst = hInst;







	/*if ((long)GetThreadLocale() == SystemLCID)
	//estamos en NT
	{
	//long default1 = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
	//long default2 = MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT);
	//char *pp=new char[2000];
	//LoadString((HINSTANCE)m_hInst,IdRes,pp,2000);
	//siteResource = FindResourceEx((HINSTANCE)m_hInst,MAKEINTRESOURCE(section), RT_STRING, default1);
	siteResource = FindResourceEx((HINSTANCE)m_hInst, RT_STRING,MAKEINTRESOURCE(section), GetThreadLocale());
	if (GetLastError() != 0)
	return S_FALSE;

	}
	else*/
	siteResource = FindResourceEx((HINSTANCE)m_hInst, RT_STRING, MAKEINTRESOURCE(section), (unsigned short)SystemLCID);

	if (siteResource != NULL)
	{
		HGLOBAL resour = LoadResource((HINSTANCE)m_hInst, siteResource);
		//resource es un array de WIDE CHAR 
		//acceder a la componente index

		unsigned char *FinalRes;
		int lenRes = FindPosition((char *)resour, index, &FinalRes);

		(*resource) = ::SysAllocStringLen((const OLECHAR *)FinalRes, lenRes);
		delete FinalRes;
	}

	return S_OK;
}


int CLocalID::FindPosition (char *strRes, int index, unsigned char **FinalRes)
{
	//hago la reserva de mem de FinalRes
	
	int i, lenRes;

	for (i=0;i<index;i++)
	{
		//lennRes = (unsigned char)(*strRes);
		//if ((unsigned char)*(strRes+1) > 0)
		lenRes = ((unsigned char)*(strRes+1))<<8 | (unsigned char)(*strRes);
		strRes += (lenRes + 1) * 2;
	}

	///lenRes = (unsigned char)(*strRes);
	//if ((unsigned char)*(strRes+1) > 0)
	lenRes = ((unsigned char)*(strRes+1))<<8 | (unsigned char)(*strRes);

	*FinalRes = new unsigned char [lenRes*2];
	memcpy(*FinalRes, strRes+2, lenRes*2);

	return lenRes;

}

bool CLocalID::IsWinNT()
{
	OSVERSIONINFO osvi;

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	
	GetVersionEx(&osvi);

	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		return true;
	else
		return false;


/*	
	unsigned long ini = 0;
	bool result = true;

	ini = GetThreadLocale();
	SetThreadLocale(ini+1);

	if (GetThreadLocale() != (ini+1))
		//no ha cambiado el thread
		result = false;

	//dejo todo como al principio. Si es Win95 esto ni caso
	SetThreadLocale(ini);
	return result;
*/

}


STDMETHODIMP CLocalID::get_hInstance(long* pVal)
{
	(*pVal) = (long)instance;
	return S_OK;
}

STDMETHODIMP CLocalID::put_hInstance(long pVal)
{
	instance = (HINSTANCE)pVal;
	return S_OK;
}

STDMETHODIMP CLocalID::GetLocalInfo(M4LocalType emlt, BSTR * bstrOut)
{
	USES_CONVERSION;
	char *sDate = (char *)malloc(200);

	//if (IsWinNT())
	//	GetLocaleInfo(SystemLCID,emlt,sDate, 199);
	//else
		GetLocaleInfo(LOCALE_USER_DEFAULT,emlt,sDate, 199);
	
	*bstrOut = A2BSTR(sDate);

	delete []sDate;

	return S_OK;
}
