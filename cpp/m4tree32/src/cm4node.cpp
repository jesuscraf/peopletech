
#include "cm4node.hpp"


CM4Node::CM4Node (void)
{
	m_lNodeID = 1L;
	m_pParent = NULL;
	m_pPrevSibling = NULL;
	m_pNextSibling = NULL;
	m_pFirstChild = NULL;
	m_pLastChild = NULL;
	m_bExpand = TRUE;
	m_bValid = FALSE;
	m_lLevel = 1L;
	m_lWidth = 0L;
	m_lHeight = 0L;
	m_lNumberChildren = 0L;
	m_pLocalProperty = new CM4Style (M4TREE_SIZE_STYLE);
}

CM4Node::~CM4Node (void)
{
	M4DELETE (m_pLocalProperty);
}

