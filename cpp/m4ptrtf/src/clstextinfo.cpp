// clsTextInfo.cpp: implementation of the clsTextInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "clsTextInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define HEAPDELTA 500
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

clsTextInfo::clsTextInfo()
{
	m_lActiveLine = -1;
	m_lNumLinesInfo = 0;
	m_lNumLinesAllocated = 0;
	m_poLinesInfo = NULL;
}

clsTextInfo::~clsTextInfo()
{
	Clear();
}

void clsTextInfo::Clear()
{
	if (m_poLinesInfo)
	{
		for (int i = 0; i<m_lNumLinesInfo; delete m_poLinesInfo[i++]);
		delete [] m_poLinesInfo;
	}

	m_lActiveLine = -1;
	m_lNumLinesInfo = 0;
	m_lNumLinesAllocated = 0;
	m_poLinesInfo = NULL;
}

void clsTextInfo::TestLine(long lLine)
{
	clsLineInfo ** temp;
	long lLineStart;

	//Este método comprueba y reserva, si es necesario, la memoria para la línea indicada
	if (!m_poLinesInfo) // Hay que reservar el array
	{
		m_lNumLinesInfo = lLine + 1;
		m_lNumLinesAllocated = lLine + HEAPDELTA;
		m_poLinesInfo = new clsLineInfo * [m_lNumLinesAllocated];
		for (long i = 0; i<m_lNumLinesInfo; m_poLinesInfo[i++] = new clsLineInfo);
	}
	else if (lLine >= m_lNumLinesAllocated) // Hay que reajustar el array
	{
		lLineStart = m_lNumLinesInfo;
		m_lNumLinesInfo = lLine + 1;
		m_lNumLinesAllocated = lLine + HEAPDELTA;
		temp = m_poLinesInfo;
		// Creamos el array con el nuevo tamaño
		m_poLinesInfo = new clsLineInfo * [m_lNumLinesAllocated];
		// Copiamos contenido
		memcpy(m_poLinesInfo, temp, lLineStart * sizeof (clsLineInfo *));
		// Borramos el antiguo
		delete [] temp;
		// Inicializamos los nuevos elementos
		for (long i = lLineStart; i<m_lNumLinesInfo; m_poLinesInfo[i++] = new clsLineInfo);
	}
	else if (lLine >= m_lNumLinesInfo) // Se debe crear nuevas informaciones de linea
	{
		for (long i = m_lNumLinesInfo; i <= lLine; m_poLinesInfo[i++] = new clsLineInfo);
		m_lNumLinesInfo = lLine + 1;
	}
}

void clsTextInfo::AdjustStatus(long lStartLine, long lOldNumLines, long lNewNumLines)
{
	clsLineInfo ** temp;
	long lNumLines;
	long lPos;
	long lDiffLines;
	int iLinStatus;

	TestLine(lStartLine);
	lDiffLines = lNewNumLines - lOldNumLines;

	if (lDiffLines > 0) // Se insertan lineas
	{
		lNumLines = m_lNumLinesInfo + lDiffLines;
		if (lNumLines >= m_lNumLinesAllocated) // Hay que reajustar el array
		{
			m_lNumLinesAllocated = lNumLines + HEAPDELTA;
			temp = m_poLinesInfo;
			// Creamos el array con el nuevo tamaño
			m_poLinesInfo = new clsLineInfo * [m_lNumLinesAllocated];
			// Se ajusta la posición de corte
			lPos = lStartLine + lOldNumLines + 1;
			// Copiamos contenido anteriores a las lineas insertadas
			memcpy(m_poLinesInfo, temp, lPos * sizeof (clsLineInfo *));
			// Se toma el estado de la línea de corte para duplicarlo en las nuevas lineas
			iLinStatus = m_poLinesInfo[lPos-1]->get_LineStatus();
			// Inicializamos los nuevos elementos
			for (long i = lPos; i <= lStartLine + lNewNumLines; i++)
			{
				m_poLinesInfo[i] = new clsLineInfo;
				m_poLinesInfo[i]->put_LineStatus(iLinStatus);
			}
			// Copiamos contenido posterior a las lineas insertadas
			if (m_lNumLinesInfo > lStartLine + lOldNumLines)
			{
				memcpy(& m_poLinesInfo[lPos + lDiffLines], & temp[lPos] ,
				(m_lNumLinesInfo - lStartLine - lOldNumLines) * sizeof (clsLineInfo *));
			}
			//Borramos el antiguo
			delete [] temp;
		}
		else // No es necesario el reajuste
		{
			// Se ajusta la posición de corte
			lPos = lStartLine + lOldNumLines + 1;
			// Se toma el estado de la línea de corte para duplicarlo en las nuevas lineas
			iLinStatus = m_poLinesInfo[lPos-1]->get_LineStatus();
			// Se desplaza el contenido posterior a las lineas borradas
			memmove(& m_poLinesInfo[lPos + lDiffLines], & m_poLinesInfo[lPos],
				(m_lNumLinesInfo - lStartLine - lOldNumLines) * sizeof (clsLineInfo *));
			// Inicializamos los nuevos elementos
			for (long i = lPos; i <= lStartLine + lNewNumLines; i++)
			{
				m_poLinesInfo[i] = new clsLineInfo;
				m_poLinesInfo[i]->put_LineStatus(iLinStatus);
			}
		}

		m_lNumLinesInfo = lNumLines;
	}
	else if (lOldNumLines > lNewNumLines) // Se borran lineas
	{
		lNumLines = m_lNumLinesInfo + lDiffLines;
		// Se ajusta el punto de corte
		lPos = lStartLine + lOldNumLines + 1;
		// Se toma el estado de la línea de corte
		iLinStatus = m_poLinesInfo[lPos-1]->get_LineStatus();
		// Se desplaza el contenido posterior a las lineas borradas
		memmove(& m_poLinesInfo[lPos + lDiffLines], & m_poLinesInfo[lPos],
			(m_lNumLinesInfo - lStartLine - lOldNumLines) * sizeof (clsLineInfo *));
		if ((lPos + lDiffLines - 1) >= 0)
		{
			m_poLinesInfo[lPos + lDiffLines -1]->put_LineStatus(iLinStatus);
		}
		m_lNumLinesInfo = lNumLines;

		// Revisamos el valor de linea actual (ver si se ha eliminado)
		if ((lStartLine + lOldNumLines >= m_lActiveLine) &&
			(lStartLine + lNewNumLines < m_lActiveLine)) m_lActiveLine = -1;
	}
}

clsLineInfo * clsTextInfo::LineInfo(long lLine)
{
	TestLine(lLine);
	return m_poLinesInfo[lLine];
}

long clsTextInfo::NextSelectionLine(long Line)
{
	long lActual = (Line < 0 ? 0 : Line + 1);

	while ((lActual < m_lNumLinesInfo) && (m_poLinesInfo[lActual]->get_SelectionMark() == 0))
	{
		lActual++;
	}

	return (lActual >= m_lNumLinesInfo ? -1 : lActual);
}

long clsTextInfo::NextBreakPointLine(long Line)
{
	long lActual = (Line < 0 ? 0 : Line + 1);

	while ((lActual < m_lNumLinesInfo) && (m_poLinesInfo[lActual]->get_BreakPointMark() == 0))
	{
		lActual++;
	}

	return (lActual >= m_lNumLinesInfo ? -1 : lActual);
}
