// clsLineInfo.h: interface for the clsLineInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLSLINEINFO_H__345C0827_A1BD_4C1F_BB0A_EC7389F61063__INCLUDED_)
#define AFX_CLSLINEINFO_H__345C0827_A1BD_4C1F_BB0A_EC7389F61063__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class clsLineInfo  
{
public:
	clsLineInfo();
	virtual ~clsLineInfo();

	inline int get_LineStatus()
	{
		return m_lLineStatus;
	}

	inline void put_LineStatus(int lValue)
	{
		m_bUndefined = false;
		m_lLineStatus = lValue;
	}

	inline short get_SelectionMark()
	{
		return m_iSelectionMark;
	}

	inline void put_SelectionMark(short lValue)
	{
		m_iSelectionMark = lValue;
	}

	inline short get_BreakPointMark()
	{
		return m_iBreakPointMark;
	}

	inline void put_BreakPointMark(short lValue)
	{
		m_iBreakPointMark = lValue;
	}

	inline bool get_bUndefined()
	{
		return m_bUndefined;
	}

protected:
	bool m_bUndefined;
	// Cada parser guardará información específica
	int m_lLineStatus; // Indica el estado de la línea (si se está en comentario multilinea, ...)
	short m_iSelectionMark; // Marca de seleccion para la línea
	short m_iBreakPointMark; // Marca de breakpoint para la línea
};

#endif // !defined(AFX_CLSLINEINFO_H__345C0827_A1BD_4C1F_BB0A_EC7389F61063__INCLUDED_)
