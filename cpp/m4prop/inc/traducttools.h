// TraductTools.h: interface for the CTraductTools class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADUCTTOOLS_H__EC23C8F3_45FD_11D4_BE05_00C04F71652D__INCLUDED_)
#define AFX_TRADUCTTOOLS_H__EC23C8F3_45FD_11D4_BE05_00C04F71652D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTraductTools  
{
public:
	HRESULT ResetTradIdentifiers(ICPropBagDef * pPBD);
	CTraductTools();
	virtual ~CTraductTools();

private:
	HRESULT RecPBD(ICPropBagDef * pPBD);
};

#endif // !defined(AFX_TRADUCTTOOLS_H__EC23C8F3_45FD_11D4_BE05_00C04F71652D__INCLUDED_)
