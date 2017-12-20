// MyVariant.cpp: implementation of the CMyVariant class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4prop.h"
#include "myvariant.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyVariant::CMyVariant()
{
	VariantInit(&m_vData);
}

CMyVariant::~CMyVariant()
{
	VariantClear(&m_vData);
}

CMyVariant& CMyVariant::operator=( VARIANT str)
{
	VariantClear(&m_vData);
	VariantCopy(&m_vData, &str);

	return *this;
}

bool CMyVariant::operator==(const CMyVariant & pSrc)
{
	long lValue1;	
		
	if (&m_vData == (&(pSrc.m_vData))) {
		return true;
	}

	switch (m_vData.vt) {
		case VT_EMPTY:
			if (pSrc.m_vData.vt == VT_EMPTY)
			{
				return true;
			}
			else return false;
			break;
		case VT_NULL:
			if (pSrc.m_vData.vt == VT_NULL)
			{
				return true;
			}
			else return false;
			break;
		case VT_I2:
			lValue1 = (long)m_vData.iVal;
			switch (pSrc.m_vData.vt)
			{
				case VT_I2:
					return lValue1 == (long)pSrc.m_vData.iVal;
					break;
				case VT_I4:
					return lValue1 == pSrc.m_vData.lVal;
					break;
				case VT_UI1:
					return lValue1 == (long)pSrc.m_vData.bVal;
					break;
			}
			return false;
			break;
		case VT_I4:
			lValue1 = m_vData.lVal;
			switch (pSrc.m_vData.vt)
			{
				case VT_I2:
					return lValue1 == (long)pSrc.m_vData.iVal;
					break;
				case VT_I4:
					return lValue1 == pSrc.m_vData.lVal;
					break;
				case VT_UI1:
					return lValue1 == (long)pSrc.m_vData.bVal;
					break;
			}
			return false;
			break;
		case VT_UI1:
			lValue1 = (long)m_vData.bVal;
			switch (pSrc.m_vData.vt)
			{
				case VT_I2:
					return lValue1 == (long)pSrc.m_vData.iVal;
					break;
				case VT_I4:
					return lValue1 == pSrc.m_vData.lVal;
					break;
				case VT_UI1:
					return lValue1 == (long)pSrc.m_vData.bVal;
					break;
			}
			return false;
			break;
		case VT_BSTR:
			if (pSrc.m_vData.vt == VT_BSTR)
			{
				return (::SysStringByteLen(m_vData.bstrVal) == ::SysStringByteLen(pSrc.m_vData.bstrVal)) &&
					(memcmp(m_vData.bstrVal, pSrc.m_vData.bstrVal, ::SysStringByteLen(m_vData.bstrVal)) == 0);
			}
			else return false;
			break;
		case VT_BOOL:
			if (pSrc.m_vData.vt == VT_BOOL)
			{
				return (m_vData.boolVal == pSrc.m_vData.boolVal);
			}
			else return false;
			break;
		case VT_R4:
			if (pSrc.m_vData.vt == VT_R4)
			{
				return (m_vData.fltVal == pSrc.m_vData.fltVal);
			}
			else return false;
			break;
		case VT_R8:
			if (pSrc.m_vData.vt == VT_R8)
			{
				return (m_vData.dblVal == pSrc.m_vData.dblVal);
			}
			else return false;

			break;
		default:
			return false;
			break;
	}

	return false;
}

bool CMyVariant::operator!=(const CMyVariant & pSrc)
{
	return !(*this == pSrc);
}

void CMyVariant::Clear()
{
	VariantClear(&m_vData);
}

bool CMyVariant::EqualVARIANT (VARIANT var1,VARIANT var2)
{	

	long	lenVariant;

	switch (var1.vt) {
		case VT_EMPTY:
			if (var2.vt == VT_EMPTY)
			{
				return true;
			}
			else return false;
			break;
		case VT_NULL:
			if (var2.vt == VT_NULL)
			{
				return true;
			}
			else return false;
			break;
		case VT_I2:
			lenVariant = (long)var1.iVal;
			switch (var2.vt)
			{
				case VT_I2:
					return lenVariant == (long)var2.iVal;
					break;
				case VT_I4:
					return lenVariant == var2.lVal;
					break;
				case VT_UI1:
					return lenVariant == (long)var2.bVal;
					break;
			}
			return false;
			break;
		case VT_I4:
			lenVariant = var1.lVal;
			switch (var2.vt)
			{
				case VT_I2:
					return lenVariant == (long)var2.iVal;
					break;
				case VT_I4:
					return lenVariant == var2.lVal;
					break;
				case VT_UI1:
					return lenVariant == (long)var2.bVal;
					break;
			}
			return false;
			break;
		case VT_UI1:
			lenVariant = (long)var1.bVal;
			switch (var2.vt)
			{
				case VT_I2:
					return lenVariant == (long)var2.iVal;
					break;
				case VT_I4:
					return lenVariant == var2.lVal;
					break;
				case VT_UI1:
					return lenVariant == (long)var2.bVal;
					break;
			}
			return false;
			break;
		case VT_BSTR:
			if (var2.vt == VT_BSTR)
			{
				return (::SysStringByteLen(var1.bstrVal) == ::SysStringByteLen(var2.bstrVal)) &&
					(memcmp(var1.bstrVal, var2.bstrVal, ::SysStringByteLen(var1.bstrVal)) == 0);
			}
			else return false;

			break;
		case VT_BOOL:
			if (var2.vt == VT_BOOL)
			{
				return (var1.boolVal == var2.boolVal);
			}
			else return false;
			break;
		case VT_R4:
			if (var2.vt == VT_R4)
			{
				return (var1.fltVal == var2.fltVal);
			}
			else return false;
			break;

		case VT_R8:
			if (var2.vt == VT_R8)
			{
				return (var1.dblVal == var2.dblVal);
			}
			else return false;
			break;

		default:
			return false;
	}
}

