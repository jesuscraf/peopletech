
#ifndef CM4NODE_HPP
#define CM4NODE_HPP

#include "stdafx.h"
#include "cm4style.hpp"

class CM4Node
{
private:
	long		m_lNodeID;
	CM4Node    *m_pParent;
	CM4Node    *m_pPrevSibling;
	CM4Node    *m_pNextSibling;
	CM4Node    *m_pFirstChild;
	CM4Node    *m_pLastChild;
	BOOL		m_bExpand;
	BOOL		m_bValid;
	long		m_lLevel;
	long		m_lWidth;
	long		m_lHeight;
	long 		m_lLeft;
	long 		m_lTop;
	long		m_lRight;
	long		m_lBottom;
	long 		m_lChildLeft;
	long 		m_lChildTop;
	long		m_lChildRight;
	long		m_lChildBottom;
	long		m_lNumberChildren;
	CM4Style   *m_pLocalProperty;

public:

	CM4Node		(void);
	~CM4Node	(void);
	
#include "cm4node.inl"

};

typedef	CM4Node*	PCM4Node;
typedef	PCM4Node*	PPCM4Node;

#endif
