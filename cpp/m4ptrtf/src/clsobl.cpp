// clsOBL.cpp: implementation of the clsOBL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "clsobl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*******************************************************************************/
/* ESTE MODULO SE DEBERIA SINCRONIZAR CON LAS MEJORAS REALIZADAS EN clsLN4.cpp */
/*******************************************************************************/
CString cKeyWordsOBL[] = { "BEGIN", "END", "VERSION" };
int iNumKeyWordsOBL = 3;

#define StringChar '"'

inline bool OBL_NeedCharEscape (TCHAR aChar)
{
	return (aChar == '\n' || aChar == '\t' || aChar =='{' || aChar =='}');
}

inline bool OBL_IsDigit (TCHAR value)
{
	return (value >= '0' && value <='9');
}

inline bool OBL_IsOperator (TCHAR value)
{
	return (value == '=' || value == '+' || value == '-' || value == '*' || value == '/');
}

inline int OBL_SearchKeyWordAux(CString & candidate, CString * tableKey, int iniPos, int finPos)
{
	int medPos;

	if (iniPos > finPos)
	{
		return -1;
	}
	else if (iniPos == finPos)
	{
		return (candidate == tableKey[iniPos] ? iniPos : -1);
	}
	else
	{
		medPos = (iniPos + finPos) / 2;
		switch (candidate.Compare(tableKey[medPos]))
		{
		case -1:
			return OBL_SearchKeyWordAux(candidate, tableKey, iniPos, medPos - 1);
		case 0:
			return medPos;
		default: // 1
			return OBL_SearchKeyWordAux(candidate, tableKey, medPos + 1, finPos);
		}
	}
}

inline int OBL_SearchKeyWord (CString & candidate, CString * tableKey, int iNumKeys)
{
/*	for (int iCount=0; (iCount< iNumKeys) && (candidate > tableKey[iCount]); iCount++);
	return ((iCount < iNumKeys) && (candidate == tableKey[iCount]) ? iCount : -1);*/
	return OBL_SearchKeyWordAux (candidate, tableKey, 0, iNumKeys -1);
}

inline int OBL_SearchEndWord (CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar = '\0';

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];

		if (aChar == StringChar || aChar == '\r' || aChar == '\n' || aChar ==' ' || aChar =='\t' || aChar =='=')
		{
			return (iActualPos - 1);
		}
		else
		{
			iActualPos++;
		}
	}
	return (iLastPos - 1);
}

inline int OBL_SearchEndLine (CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (aChar == '\n' || aChar == '\r')
		{
			return (iActualPos -1);
		}
		else
		{
			iActualPos++;
		}
	}
	return (iLastPos - 1);
}

inline int OBL_SearchChar (CString & text, int iIniPos, int iLastPos, TCHAR charSearch)
{
	int iActualPos = iIniPos;

	for (; (iActualPos < iLastPos) && (charSearch != text[iActualPos]); iActualPos++);
	return (iActualPos == iLastPos ? -1 : iActualPos);
}

inline int OBL_SearchCharOrEndLine (CString & text, int iIniPos, int iLastPos, TCHAR charSearch)
{
	int iActualPos = iIniPos;
	TCHAR aChar;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (aChar == '\n' || aChar == '\r')
		{
			return (iActualPos -1);
		}
		else if (aChar == charSearch)
		{
			return (iActualPos);
		}
		else
		{
			iActualPos++;
		}
	}
	return (iLastPos - 1);

}

inline int OBL_SearchNumber(CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar = '\0';
	bool bPointUsed = false;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (OBL_IsDigit(aChar))
		{
			iActualPos++;
			continue;
		}
		else if (aChar == '.')
		{
			if (bPointUsed)
			{
				return (-1);
			}
			else
			{
				iActualPos++;
				bPointUsed = true;
				continue;
			}
		}
		else if (aChar == '\r' || aChar == '\n' || aChar ==' ' || aChar =='\t' || aChar =='(' || aChar ==')' || aChar ==',')
		{
			return (iActualPos - 1);
		}
		else
		{
			return (-1);
		}
	}
	return (iLastPos - 1);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

clsOBL::clsOBL()
{
	int i;

	m_eType = M4PT_OBL;

	m_cKeyWords = new CString[iNumKeyWordsOBL];
	for (i=0; i < iNumKeyWordsOBL; i++)
	{
		m_cKeyWords[i] = cKeyWordsOBL[i];
		m_cKeyWords[i].MakeUpper();
	}
	m_iNumKeyWords = iNumKeyWordsOBL;
}

clsOBL::~clsOBL()
{
	delete [] m_cKeyWords;
}

CString clsOBL::GetColorTable()
{
	return "\\red0\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue128;\\red0\\green128\\blue0;";
}

void clsOBL::Parse(clsTextInfo * oTextInfo, long lStartLine, CString &sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine)
{
	int iActualPos = 0;
	int iAuxPos;
	int iFinal;
	bool bChangePending = false;
	TCHAR aChar;
	CString csAux;

	iFinal = sWorkText.GetLength();
	// Se parsea el texto
	for (; iActualPos < iFinal; iActualPos++)
	{
		aChar = sWorkText[iActualPos];

		if (OBL_NeedCharEscape(aChar) || aChar =='\r')
		{
			// Se sustituira al final (Es mucho más eficiente. Mejoras del 40% en tiempo)
			continue;
		}
		else if (aChar =='[')
		{
			iAuxPos = OBL_SearchCharOrEndLine(sWorkText, iActualPos, iFinal, ']');
			if (sWorkText[iAuxPos] ==']')
			{
				sWorkText.Insert(iActualPos, "\\cf1 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10;
				iFinal += 10;
			}
		}
		else if (aChar == '=') //Por ahora todo a la derecha del igual es un valor. Se marca de la misma forma
		{
			iAuxPos = OBL_SearchEndLine(sWorkText, iActualPos+1, iFinal);
			if (iAuxPos >=0)
			{
				sWorkText.Insert(iActualPos + 1, "\\cf2 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10; //5;
				iFinal += 10;
			}

		}
		else if (aChar == StringChar)
		{
			iAuxPos = OBL_SearchChar (sWorkText, iActualPos+1, iFinal, StringChar);
			if (iAuxPos >=0)
			{
				sWorkText.Insert(iActualPos, "\\cf2 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10; //5;
				iFinal += 10;
			}
		}
		else if ((aChar == '-') || OBL_IsDigit(aChar)) // Evaluar número
		{
			iAuxPos = OBL_SearchNumber(sWorkText, ((aChar == '-') ? iActualPos+1 : iActualPos), iFinal);
			if (iAuxPos >=0)
			{
				sWorkText.Insert(iActualPos, "\\cf2 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10; //5;
				iFinal += 10;
			}
		}
		else if (aChar != '=' && aChar != ' ' && aChar != '\r' && aChar != '"')
		{
			iAuxPos = OBL_SearchEndWord(sWorkText, iActualPos, iFinal);
			csAux = sWorkText.Mid(iActualPos, iAuxPos - iActualPos + 1);
			csAux.MakeUpper();
			if (csAux == "REM") //Es un comentario
			{
				iAuxPos = OBL_SearchEndLine(sWorkText, iActualPos, iFinal);
				sWorkText.Insert(iActualPos, "\\cf4 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10;
				iFinal = sWorkText.GetLength();
			}
			else if (OBL_SearchKeyWord(csAux,m_cKeyWords, m_iNumKeyWords) >=0)
			{
				sWorkText.Insert(iActualPos, "\\cf1 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10;
				iFinal = sWorkText.GetLength();
			}
			else if (iAuxPos > iActualPos)
			{
				iActualPos = iAuxPos;
			}
		}
	}
}

VARIANT_BOOL clsOBL::GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & sDesc)
{
	return FALSE;
}

VARIANT_BOOL clsOBL::SetColorInfo(short Id, long Color)
{
	return FALSE;
}
