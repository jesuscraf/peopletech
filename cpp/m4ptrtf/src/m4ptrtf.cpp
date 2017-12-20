// M4PTRTF.cpp : Implementation of CM4PTRTF
#include "stdafx.h"
#include "m4ptrtf32.h"
#include "m4ptrtf.h"
#include "clsln4.h"
#include "clsobl.h"
#include "clssql.h"

/////////////////////////////////////////////////////////////////////////////
// CM4PTRTF

CM4PTRTF::CM4PTRTF()
{
	m_Pattern =  new clsPattern;
	m_TextInfo = new clsTextInfo;
	put_LCID(0);
}

CM4PTRTF::~CM4PTRTF()
{
	if (m_Pattern) delete m_Pattern;
	if (m_TextInfo) delete m_TextInfo;
}

STDMETHODIMP CM4PTRTF::GenerateRTF(BSTR sPlainText, long lStartLine, VARIANT_BOOL * bPostProcess, long * lEndLine, BSTR *sRTFText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString sWorkText(sPlainText);

	// Se construye (si es necesario) la cabecera
	// que incluirá el listado de colores
	if (m_sRTFHeader.GetLength() == 0)
	{
		RegenerateHeader();
	}

	// Se sustituyen el caracter especial de RTF '\'
	// Se debe hacer ahora para evitar sustituir las secuencias de escape del RTF parseado
	// Se debe considerar en el Parseo que el \ siempre es doble
	sWorkText.Replace ("\\", "\\\\");
	// Se parsea el texto (búsqueda de token e incorporación de códigos RTF de colores
	m_Pattern->Parse(m_TextInfo, lStartLine, sWorkText, bPostProcess, lEndLine);

	// Se sustituyen los caracteres especiales de RTF
	sWorkText.Replace ("{", "\\{");
	sWorkText.Replace ("}", "\\}");
	sWorkText.Replace ("\t", "\\tab ");
	sWorkText.Replace ("\n", "\\par ");

	// Se añade la cabecera
	sWorkText.Insert (0, m_sRTFHeader);
	// y el final del RTF
	sWorkText += "}";

	// Se convierte a BSTR
	sWorkText.SetSysString (sRTFText);

	return S_OK;
}

void CM4PTRTF::RegenerateHeader()
{
	m_sRTFHeader = "{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033\n{\\colortbl ;";
	m_sRTFHeader += m_Pattern->GetColorTable();
	m_sRTFHeader += "}\n\\viewkind4 \\cf0 ";
}

void CM4PTRTF::ClearHeader()
{
	m_sRTFHeader = "";
}

STDMETHODIMP CM4PTRTF::get_ParseMode(short *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if (m_Pattern)
	{
		*pVal = m_Pattern->GetType();
	}
	else
	{
		*pVal = -1;
	}
	return S_OK;
}

STDMETHODIMP CM4PTRTF::put_ParseMode(short newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	long lOldLCID;

	ClearHeader();

	if (m_Pattern)
	{
		lOldLCID = m_Pattern->get_LCID();
		delete m_Pattern;
	}
	else
	{
		lOldLCID = 0;
	}

	switch (newVal)
	{
	case M4PT_LN4:
		m_Pattern = new clsLN4;
		break;
	case M4PT_SQL:
		m_Pattern = new clsSQL;
		break;
	case M4PT_OBL:
		m_Pattern = new clsOBL;
		break;
	default:
		m_Pattern = new clsPattern;
	}

	m_Pattern->put_LCID(lOldLCID);

	return S_OK;
}

STDMETHODIMP CM4PTRTF::get_ActiveLine(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_TextInfo->get_ActiveLine();
	return S_OK;
}

STDMETHODIMP CM4PTRTF::put_ActiveLine(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_TextInfo->put_ActiveLine(newVal);
	return S_OK;
}

STDMETHODIMP CM4PTRTF::get_SelectionMark(long Line, short *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_TextInfo->LineInfo(Line)->get_SelectionMark();

	return S_OK;
}

STDMETHODIMP CM4PTRTF::put_SelectionMark(long Line, short newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_TextInfo->LineInfo(Line)->put_SelectionMark(newVal);

	return S_OK;
}

STDMETHODIMP CM4PTRTF::NextSelectionLine(long Line, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_TextInfo->NextSelectionLine(Line);

	return S_OK;
}

STDMETHODIMP CM4PTRTF::get_BreakPointMark(long Line, short *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_TextInfo->LineInfo(Line)->get_BreakPointMark();

	return S_OK;
}

STDMETHODIMP CM4PTRTF::put_BreakPointMark(long Line, short newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_TextInfo->LineInfo(Line)->put_BreakPointMark(newVal);

	return S_OK;
}

STDMETHODIMP CM4PTRTF::NextBreakPointLine(long Line, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_TextInfo->NextBreakPointLine(Line);

	return S_OK;
}

STDMETHODIMP CM4PTRTF::ClearStatus()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_TextInfo->Clear();
	return S_OK;
}

STDMETHODIMP CM4PTRTF::AdjustTextStatus(long lStartLine, long lOldNumLines, long lNewNumLines)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_TextInfo->AdjustStatus(lStartLine, lOldNumLines, lNewNumLines);
	return S_OK;
}


STDMETHODIMP CM4PTRTF::GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, BSTR *Description, VARIANT_BOOL *bRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString sDesc;

	*bRet = m_Pattern->GetColorInfo(Id, Color, DefaultColor, iKind, sDesc);

	// Se convierte a BSTR
	sDesc.SetSysString(Description);

	return S_OK;
}

STDMETHODIMP CM4PTRTF::SetColorInfo(short Id, long Color, VARIANT_BOOL *bRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*bRet = m_Pattern->SetColorInfo(Id, Color);

	ClearHeader();

	return S_OK;
}

STDMETHODIMP CM4PTRTF::get_LCID(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = m_Pattern->get_LCID();

	return S_OK;
}

STDMETHODIMP CM4PTRTF::put_LCID(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	m_Pattern->put_LCID(newVal);

	return S_OK;
}
