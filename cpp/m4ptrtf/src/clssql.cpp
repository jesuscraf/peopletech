// clsSQL.cpp: implementation of the clsSQL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "clssql.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*******************************************************************************/
/* ESTE MODULO SE DEBERIA SINCRONIZAR CON LAS MEJORAS REALIZADAS EN clsLN4.cpp */
/*******************************************************************************/

CString cKeyWordsSQL[] = { "ADD", "ALL", "ALTER", "AND", "ANY", "ASC", "BETWEEN",
						   "BY", "CALL", "CHECK", "COLUMN", "CONSTRAINT", "COUNT", "CREATE",
						   "DELETE", "DESC", "DISTINCT", "DROP", "EXIST", "FOR", "FROM",
						   "FUNCTION", "GRANT", "GROUP", "HAVING", "IN", "INDEX", "INNER",
						   "INSERT", "INTO", "JOIN", "KEY", "LEFT", "LIKE", "MODIFY",
						   "NOT", "ON", "OPTION", "OR", "ORDER", "OUTER", "PRIMARY",
						   "PROCEDURE", "PUBLIC", "REFERENCES", "REVOKE", "RIGHT", "SELECT", "SET",
						   "SOME", "STATISTICS", "SYNONYM", "TABLE", "TO", "TRIGGER", "UNION",
						   "UNIQUE", "UPDATE", "VIEW", "WHERE", "WITH" };
int  iNumKeyWordsSQL = 61;

CString cFunctionsSQL[] = { "ABS", "AVG", "COS", "EXP", "LOWER", "MAX", "MIN",
							"ROUND", "SQRT", "SUM", "TAN", "TRUNC", "UPPER" };

int iNumFunctionsSQL = 13;

#define StringChar '\''

#define CommentChar '\0'


inline bool SQL_NeedCharEscape (TCHAR aChar)
{
	return (aChar == '\n' || aChar == '\t' || aChar =='{' || aChar =='}');
}

inline bool SQL_IsDigit (TCHAR value)
{
	return (value >= '0' && value <='9');
}

inline bool SQL_IsOperator (TCHAR value)
{
	return (value == '=' || value == '+' || value == '-' || value == '*' || value == '/');
}

inline int SQL_SearchKeyWordAux(CString & candidate, CString * tableKey, int iniPos, int finPos)
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
			return SQL_SearchKeyWordAux(candidate, tableKey, iniPos, medPos - 1);
		case 0:
			return medPos;
		default: // 1
			return SQL_SearchKeyWordAux(candidate, tableKey, medPos + 1, finPos);
		}
	}
}

inline int SQL_SearchKeyWord (CString & candidate, CString * tableKey, int iNumKeys)
{
/*	for (int iCount=0; (iCount< iNumKeys) && (candidate > tableKey[iCount]); iCount++);
	return ((iCount < iNumKeys) && (candidate == tableKey[iCount]) ? iCount : -1);*/
	return SQL_SearchKeyWordAux (candidate, tableKey, 0, iNumKeys -1);
}

inline int SQL_SearchEndWord (CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		//'"' Pruebas
		if (aChar == '"' || aChar == StringChar || aChar == '\r' || aChar == '\n' || aChar ==' ' || aChar =='\t' || aChar =='(' || aChar ==')' || aChar == ',' || aChar == ':' || SQL_IsOperator(aChar))
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

inline int SQL_SearchEndLine (CString & text, int iIniPos, int iLastPos)
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

inline int SQL_SearchChar (CString & text, int iIniPos, int iLastPos, TCHAR charSearch)
{
	int iActualPos = iIniPos;

	for (; (iActualPos < iLastPos) && (charSearch != text[iActualPos]); iActualPos++);
	return (iActualPos == iLastPos ? -1 : iActualPos);
}

inline int SQL_SearchNumber(CString & text, int iIniPos, int iLastPos)
{
	int iActualPos = iIniPos;
	TCHAR aChar = '\0';
	bool bPointUsed = false;

	while (iActualPos < iLastPos)
	{
		aChar = text[iActualPos];
		if (SQL_IsDigit(aChar))
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

clsSQL::clsSQL()
{
	int i;

	m_eType = M4PT_SQL;

	m_cKeyWords = new CString[iNumKeyWordsSQL];
	for (i=0; i < iNumKeyWordsSQL; i++)
	{
		m_cKeyWords[i] = cKeyWordsSQL[i];
		m_cKeyWords[i].MakeUpper();
	}
	m_iNumKeyWords = iNumKeyWordsSQL;

	m_cFunctions = new CString[iNumFunctionsSQL];
	for (i=0; i < iNumFunctionsSQL; i++)
	{
		m_cFunctions[i] = cFunctionsSQL[i];
		m_cFunctions[i].MakeUpper();
	}
	m_iNumFunctions = iNumFunctionsSQL;
}

clsSQL::~clsSQL()
{
	delete [] m_cKeyWords;
}

CString clsSQL::GetColorTable()
{
	return "\\red0\\green0\\blue128;\\red128\\green0\\blue0;\\red128\\green128\\blue128;\\red0\\green128\\blue0;";
}

void clsSQL::Parse(clsTextInfo * oTextInfo, long lStartLine, CString &sWorkText, VARIANT_BOOL * bPostProcess, long * lEndLine)
{
	int iActualPos = 0;
	int iAuxPos;
	int iFinal;
	bool bChangePending = false;
	int iParameters = 0;
	TCHAR aChar;
	CString csAux;

	iFinal = sWorkText.GetLength();
	// Se parsea el texto
	for (; iActualPos < iFinal; iActualPos++)
	{
		aChar = sWorkText[iActualPos];

		if (SQL_NeedCharEscape(aChar) || aChar =='\r')
		{
			if (aChar == '\n') iParameters = 0;
			// Se sustituira al final (Es mucho más eficiente. Mejoras del 40% en tiempo)
			continue;
		}
		else if (aChar == CommentChar)
		{
			iAuxPos = SQL_SearchEndLine(sWorkText, iActualPos + 1, iFinal);
			if (iAuxPos >=0)
			{
				sWorkText.Insert(iActualPos, "\\cf4 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10;
				iFinal = sWorkText.GetLength();
			}
		}
		else if (aChar == StringChar)
		{
			iAuxPos = SQL_SearchChar (sWorkText, iActualPos+1, iFinal, StringChar);
			if (iAuxPos >=0)
			{
				/*bChangePending = true;
				sWorkText.Insert(iActualPos, "\\cf4 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10; //5;
				iFinal = sWorkText.GetLength();*/
				iActualPos = iAuxPos + 1;
			}
		}
		else if (aChar =='(')
		{
			iParameters++;
			sWorkText.Insert(iActualPos,"\\cf1 ");
			sWorkText.Insert(iActualPos + 6, "\\cf0 ");
			iActualPos += 10;
			iFinal +=10;
		}
		else if (aChar ==')')
		{
			if (iParameters > 0)
			{
				iParameters--;
				sWorkText.Insert(iActualPos,"\\cf1 ");
				sWorkText.Insert(iActualPos + 6, "\\cf0 ");
				iActualPos += 10;
				iFinal +=10;
			}
		}
		else if (aChar ==',')
		{
			if (iParameters > 0)
			{
				sWorkText.Insert(iActualPos,"\\cf1 ");
				sWorkText.Insert(iActualPos + 6, "\\cf0 ");
				iActualPos += 10;
				iFinal +=10;
			}
		}
		//else if
		else if (aChar == '=' || aChar == '+' || aChar == '*' || aChar == '/' || aChar == '<' || aChar == '>')
		{
			sWorkText.Insert(iActualPos,"\\cf2 ");
			sWorkText.Insert(iActualPos + 6, "\\cf0 ");
			iActualPos += 10;
			iFinal +=10;
		}
		else if (aChar =='@' || aChar =='#')
		{
			iAuxPos = SQL_SearchEndWord(sWorkText, iActualPos, iFinal);
			sWorkText.Insert(iActualPos, "\\cf3 ");
			sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
			iActualPos = iAuxPos + 10; //5;
			iFinal += 10;
		}
		else if ((aChar == '-') || SQL_IsDigit(aChar)) // Evaluar número
		{
			iAuxPos = SQL_SearchNumber(sWorkText, ((aChar == '-') ? iActualPos+1 : iActualPos), iFinal);
			if (iAuxPos >=0)
			{
				/*bChangePending = true;
				sWorkText.Insert(iActualPos, "\\cf2 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos = iAuxPos + 10; //5;
				iFinal = sWorkText.GetLength();*/
				iActualPos = iAuxPos;
			}
		}
		else if (aChar != ':' && aChar != ' ' && aChar != '\r')
		{
			iAuxPos = SQL_SearchEndWord(sWorkText, iActualPos, iFinal);
			csAux = sWorkText.Mid(iActualPos, iAuxPos - iActualPos + 1);
			if (csAux == "?")
			{
				sWorkText.Insert(iActualPos,"\\cf2 ");
				sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
				iActualPos += 10;
				iFinal +=10;
			}
			else
			{
				csAux.MakeUpper();
				if (SQL_SearchKeyWord(csAux,m_cKeyWords, m_iNumKeyWords) >=0)
				{
					sWorkText.Insert(iActualPos, "\\cf1 ");
					sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
					iActualPos = iAuxPos + 10;
					iFinal += 10;
				}
				else if (SQL_SearchKeyWord(csAux,m_cFunctions, m_iNumFunctions) >=0)
				{
					sWorkText.Insert(iActualPos, "\\cf1 ");
					sWorkText.Insert(iAuxPos + 6, "\\cf0 ");
					iActualPos = iAuxPos + 10;
					iFinal += 10;
				}
				else if (iAuxPos > iActualPos)
				{
					iActualPos = iAuxPos;
				}
			}
		}
	}
}

VARIANT_BOOL clsSQL::GetColorInfo(short Id, long *Color, long *DefaultColor, short *iKind, CString & sDesc)
{
	return FALSE;
}

VARIANT_BOOL clsSQL::SetColorInfo(short Id, long Color)
{
	return FALSE;
}
