
#include "cm4tree.hpp"
#include "stdio.h"
#include "resource.h"


CM4Tree::CM4Tree (void)
{
	m_pRoot = NULL;
	m_lIDCount = 0L;
	m_lNodeCount = 0L;
	m_lLevelCount = 0L;
	m_pCurrentNode = NULL;
	m_pNextNode = NULL;
	m_pMoveNode = NULL;
	m_pListNode = new CM4ListNode ();
}

CM4Tree::~CM4Tree (void)
{
	if (m_pRoot)
		M4DeleteSubTree (m_pRoot);
	M4DELETE(m_pListNode);
}

long CM4Tree::M4InsertNode (long a_lParentID)
{
	if (m_pRoot==NULL)
	{
		CM4Node *pNewNode = new CM4Node ();
		if (pNewNode == NULL)
			return 0L;
		m_lIDCount = 1L;
		m_lNodeCount = 1L;
		m_lLevelCount = 1L;
		m_pRoot = pNewNode;
		m_pRoot->M4SetNodeID (m_lIDCount);
		m_pCurrentNode = pNewNode;
		m_pListNode->M4InsertNode (m_pRoot);
		return m_lIDCount;
	}

	CM4Node *pParentNode;
	if (a_lParentID == M4TREE_CURRENT_NODE)
		pParentNode = m_pCurrentNode;
	else
		pParentNode = m_pListNode->M4FindNode (a_lParentID);

	if (pParentNode == NULL)
		return 0L;

	CM4Node *pNewNode = new CM4Node ();
	if (pNewNode == NULL)
		return 0L;
	m_lIDCount++;
	m_lNodeCount++;
	pNewNode->M4SetNodeID (m_lIDCount);
	pNewNode->M4SetParent (pParentNode);
	pNewNode->M4SetLevel (pParentNode->M4GetLevel ()+1);
		
	if (m_lLevelCount < pNewNode->M4GetLevel ())
		m_lLevelCount = pNewNode->M4GetLevel ();
	pParentNode->M4SetNumberChildren (pParentNode->M4GetNumberChildren ()+1);
	if (pParentNode->M4GetFirstChild ())
	{
		(pParentNode->M4GetLastChild ())->M4SetNextSibling (pNewNode);	// El nuevo nodo es siguiente hermano al último hijo del padre.
		pNewNode->M4SetPrevSibling (pParentNode->M4GetLastChild ());	// El último hijo del padre es el nodo siguiente al nuevo nodo.
	}
	else
		pParentNode->M4SetFirstChild (pNewNode);	// El nuevo nodo es el primer hijo del padre.
	
	pParentNode->M4SetLastChild (pNewNode);	// El nuevo nodo es el nuevo último hijo del padre.
	m_pCurrentNode = pNewNode;
	m_pListNode->M4InsertNode (pNewNode);
	return m_lIDCount;
}

BOOL CM4Tree::M4DeleteNode (long a_lNodeID)
{
	CM4Node *pNode;
	if (a_lNodeID == M4TREE_CURRENT_NODE)
		pNode = m_pCurrentNode;
	else
		pNode = m_pListNode->M4FindNode (a_lNodeID);

	if (pNode==NULL)
		return FALSE;

	if (pNode == m_pRoot)
		return M4DeleteSubTree (m_pRoot);
	
	m_pCurrentNode = pNode->M4GetParent ();

	if (pNode->M4GetParent ()->M4GetFirstChild () == pNode)
		pNode->M4GetParent ()->M4SetFirstChild (pNode->M4GetNextSibling ());

	if (pNode->M4GetParent ()->M4GetLastChild () == pNode)
		pNode->M4GetParent ()->M4SetLastChild (pNode->M4GetPrevSibling ());
	
	if (pNode->M4GetPrevSibling ())
		pNode->M4GetPrevSibling ()->M4SetNextSibling (pNode->M4GetNextSibling ());

	if (pNode->M4GetNextSibling ())
		pNode->M4GetNextSibling ()->M4SetPrevSibling (pNode->M4GetPrevSibling ());

	pNode->M4GetParent ()->M4SetNumberChildren (pNode->M4GetParent ()->M4GetNumberChildren ()-1);
	pNode->M4SetNextSibling (NULL);
	pNode->M4SetPrevSibling (NULL);
	pNode->M4SetParent (NULL);

	return M4DeleteSubTree (pNode);
}

BOOL CM4Tree::M4DeleteSubTree (CM4Node *a_pNode)
{
	if (a_pNode==NULL)
		return FALSE;
	
	if (m_pRoot==a_pNode)
		m_pRoot = NULL;

	CM4Node *pNode = a_pNode;
	CM4Node *pChild, *pSibling, *pParent, *pParent2;
	while (pNode)
	{
		pChild = pNode->M4GetFirstChild ();

		if (pChild)
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
			{
				m_pListNode->M4DeleteNode (pNode);
				delete pNode;
				pNode = pSibling;
			}
			else
			{
				pParent = pNode->M4GetParent ();
				m_pListNode->M4DeleteNode (pNode);
				delete pNode;
				pNode = NULL;
				if (pParent == NULL)
					return TRUE;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					pParent2 = pParent->M4GetParent ();
					m_pListNode->M4DeleteNode (pParent);
					delete pParent;
					pParent = pParent2;
					if (pParent == NULL)
						return TRUE;
				}
			}
		}
	}
	return TRUE;
}

BOOL CM4Tree::M4SetRectChildNode (CM4Node *a_pParent)
{
	if (a_pParent==NULL)
		return FALSE;

	CM4Node *pNode = a_pParent->M4GetFirstChild();
	if (pNode==NULL)
		return FALSE;

	long lLeft, lTop, lRight, lBottom;
	long lParentLeft, lParentTop, lParentRight, lParentBottom;
	pNode->M4GetRect (lLeft, lTop, lRight, lBottom);
	a_pParent->M4SetRectChilds (lLeft, lTop, lRight, lBottom);

	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{
		pNode->M4GetRect (lLeft, lTop, lRight, lBottom);
		a_pParent->M4GetRectChilds (lParentLeft, lParentTop, lParentRight, lParentBottom);
		if (lLeft < lParentLeft)
			lParentLeft = lLeft;
		if (lRight > lParentRight)
			lParentRight = lRight;
		if (lTop < lParentTop)
			lParentTop = lTop;
		if (lBottom > lParentBottom)
			lParentBottom = lBottom;
		a_pParent->M4SetRectChilds (lParentLeft, lParentTop, lParentRight, lParentBottom);

		pChild = pNode->M4GetFirstChild ();

		if ((pChild) && (pNode->M4GetExpand () == TRUE))
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if ((pParent == NULL) || (pParent == a_pParent))
					return TRUE;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if ((pParent == NULL) || (pParent == a_pParent))
							return TRUE;
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CM4Tree::M4ChangeLevelNode (CM4Node *a_pNode)
{
	CM4Node *pNewNode = new CM4Node;
	if (pNewNode == NULL)
		return FALSE;

	pNewNode->M4SetNodeID (0);
	pNewNode->M4SetLevel (a_pNode->M4GetLevel ());
	pNewNode->M4SetExpand(TRUE);

	pNewNode->M4SetParent (a_pNode->M4GetParent ());
	if (a_pNode->M4GetParent ()->M4GetFirstChild () == a_pNode)
		a_pNode->M4GetParent ()->M4SetFirstChild (pNewNode);
	if (a_pNode->M4GetParent ()->M4GetLastChild () == a_pNode)
		a_pNode->M4GetParent ()->M4SetLastChild (pNewNode);

	if (a_pNode->M4GetPrevSibling ())
	{
		pNewNode->M4SetPrevSibling (a_pNode->M4GetPrevSibling ());
		a_pNode->M4GetPrevSibling ()->M4SetNextSibling (pNewNode);
	}

	if (a_pNode->M4GetNextSibling ())
	{
		pNewNode->M4SetNextSibling (a_pNode->M4GetNextSibling ());
		a_pNode->M4GetNextSibling ()->M4SetPrevSibling (pNewNode);
	}

	pNewNode->M4SetFirstChild (a_pNode);
	pNewNode->M4SetLastChild (a_pNode);
		
	a_pNode->M4SetParent (pNewNode);
	a_pNode->M4SetPrevSibling (NULL);
	a_pNode->M4SetNextSibling (NULL);

	CM4Node *pNode = a_pNode;
	
	if (pNode->M4GetFirstChild () == NULL)
	{
		pNode->M4SetLevel (pNode->M4GetLevel ()+1);
		if (m_lLevelCount < pNewNode->M4GetLevel ())
			m_lLevelCount = pNewNode->M4GetLevel ();
		return TRUE;
	}
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{
		pNode->M4SetLevel (pNode->M4GetLevel ()+1);
		if (m_lLevelCount < pNewNode->M4GetLevel ())
			m_lLevelCount = pNewNode->M4GetLevel ();

		pChild = pNode->M4GetFirstChild ();

		if (pChild)
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if ((pParent == NULL) || (pParent == a_pNode))
					return TRUE;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if ((pParent == NULL) || (pParent == a_pNode))
							return TRUE;
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL CM4Tree::M4CutNode (long a_lNodeID)
{
	if (m_pMoveNode)
		M4DeleteSubTree (m_pMoveNode);

	if (a_lNodeID == M4TREE_CURRENT_NODE)
		m_pMoveNode = m_pCurrentNode;
	else
		m_pMoveNode = m_pListNode->M4FindNode (a_lNodeID);

	if (m_pMoveNode == NULL)
		return FALSE;

	if (m_pMoveNode == m_pRoot)
	{
		m_pMoveNode = NULL;
		return M4DeleteSubTree (m_pRoot);
	}

	if (m_pMoveNode->M4GetParent ()->M4GetFirstChild () == m_pMoveNode)
		m_pMoveNode->M4GetParent ()->M4SetFirstChild (m_pMoveNode->M4GetNextSibling ());

	if (m_pMoveNode->M4GetParent ()->M4GetLastChild () == m_pMoveNode)
		m_pMoveNode->M4GetParent ()->M4SetLastChild (m_pMoveNode->M4GetPrevSibling ());
	
	if (m_pMoveNode->M4GetPrevSibling ())
		m_pMoveNode->M4GetPrevSibling ()->M4SetNextSibling (m_pMoveNode->M4GetNextSibling ());

	if (m_pMoveNode->M4GetNextSibling ())
		m_pMoveNode->M4GetNextSibling ()->M4SetPrevSibling (m_pMoveNode->M4GetPrevSibling ());

	m_pCurrentNode = m_pMoveNode->M4GetParent ();

	m_pMoveNode->M4GetParent ()->M4SetNumberChildren (m_pMoveNode->M4GetParent ()->M4GetNumberChildren ()-1);
	m_pMoveNode->M4SetNextSibling (NULL);
	m_pMoveNode->M4SetPrevSibling (NULL);
	m_pMoveNode->M4SetParent (NULL);
	return TRUE;
}
	
BOOL CM4Tree::M4PasteNode (long a_lParentID)
{
	if (m_pMoveNode == NULL)
		return FALSE;
		
	CM4Node *pParentNode;
	if (a_lParentID == M4TREE_CURRENT_NODE)
		pParentNode = m_pCurrentNode;
	else
		pParentNode = m_pListNode->M4FindNode (a_lParentID);

	m_pMoveNode->M4SetParent (pParentNode);

	pParentNode->M4SetNumberChildren (pParentNode->M4GetNumberChildren ()+1);
	if (pParentNode->M4GetFirstChild ())
	{
		(pParentNode->M4GetLastChild ())->M4SetNextSibling (m_pMoveNode);
		m_pMoveNode->M4SetPrevSibling (pParentNode->M4GetLastChild ());
	}
	else
		pParentNode->M4SetFirstChild (m_pMoveNode);
	
	long lDifLevel = m_pMoveNode->M4GetLevel ()-pParentNode->M4GetLevel ()-1;
	pParentNode->M4SetLastChild (m_pMoveNode);
	m_pCurrentNode = m_pMoveNode;
	m_pMoveNode = NULL;

	CM4Node *pNode = m_pCurrentNode;
	
	if (pNode->M4GetFirstChild () == NULL)
	{
		pNode->M4SetLevel (pNode->M4GetLevel ()-lDifLevel);
		if (m_lLevelCount < pNode->M4GetLevel ())
			m_lLevelCount = pNode->M4GetLevel ();
		return TRUE;
	}
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{
		pNode->M4SetLevel (pNode->M4GetLevel ()-lDifLevel);
		if (m_lLevelCount < pNode->M4GetLevel ())
			m_lLevelCount = pNode->M4GetLevel ();

		pChild = pNode->M4GetFirstChild ();

		if (pChild)
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if ((pParent == NULL) || (pParent == m_pCurrentNode))
					return TRUE;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if ((pParent == NULL) || (pParent == m_pCurrentNode))
							return TRUE;
					}
				}
			}
		}
	}
	return TRUE;
}
	
BOOL CM4Tree::M4ExpandLevel (long a_lLevel)
{
	CM4Node *pNode = m_pRoot;
	CM4Node *pChild, *pSibling, *pParent;
	while (pNode)
	{
		if (a_lLevel == 0L)
				pNode->M4SetExpand (TRUE);
		else
		if (pNode->M4GetLevel ()<a_lLevel) 
			pNode->M4SetExpand (TRUE);
		else
			pNode->M4SetExpand (FALSE);

		pChild = pNode->M4GetFirstChild ();

		if (pChild)
			pNode = pChild;
		else
		{
			pSibling = pNode->M4GetNextSibling ();
			if (pSibling)
				pNode = pSibling;
			else
			{
				pParent = pNode->M4GetParent ();
				if (pParent == NULL)
					return TRUE;
				BOOL bExitWhile = FALSE;
				while (bExitWhile == FALSE)
				{
					pSibling = pParent->M4GetNextSibling ();
					if (pSibling)
					{
						pNode = pSibling;
						bExitWhile = TRUE;
					}
					else
					{
						pParent = pParent->M4GetParent ();
						if (pParent == NULL)
							return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}

void CM4Tree::M4SetNextNode (CM4Node *a_pNode)
{
	if (a_pNode->M4GetFirstChild ())
	{
		m_pNextNode = a_pNode->M4GetFirstChild ();
		return;
	}
	if (a_pNode->M4GetNextSibling ())
	{
		m_pNextNode = a_pNode->M4GetNextSibling ();
		return;
	}
	CM4Node *pSibling, *pParent;
	pParent = a_pNode->M4GetParent ();
	while (pParent)
	{
		pSibling = pParent->M4GetNextSibling ();
		if (pSibling)
		{
			m_pNextNode = pSibling;
			return;
		}
		pParent = pParent->M4GetParent ();
	}
	m_pNextNode = NULL;
}
