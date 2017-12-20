
#ifndef CM4TREE_HPP
#define CM4TREE_HPP

#include "stdafx.h"
#include "m4detree.hpp"
#include "cm4style.hpp"
#include "cm4node.hpp"
#include "cm4listn.hpp"

class CM4Tree
{
private:
	CM4Node	   *m_pRoot;
	CM4Node	   *m_pCurrentNode;
	CM4Node	   *m_pNextNode;
	CM4Node	   *m_pMoveNode;
	CM4ListNode	*m_pListNode;
	long		m_lIDCount;
	long		m_lNodeCount;
	long		m_lLevelCount;

public:
				CM4Tree (void);
			   ~CM4Tree (void);
	long		M4InsertNode (long a_lParentID);
	BOOL		M4DeleteNode (long a_lNodeID);
	BOOL        M4CutNode (long a_lNodeID);
	BOOL        M4PasteNode (long a_lParentID);
	BOOL		M4DeleteSubTree (CM4Node *a_pNode);
	BOOL		M4SetRectChildNode (CM4Node *a_pParent);
	BOOL		M4ChangeLevelNode (CM4Node *a_pNode);
	BOOL		M4ExpandLevel (long a_lLevel);
	void		M4SetNextNode (CM4Node *a_pNode);

#include "cm4tree.inl"

};

#endif
