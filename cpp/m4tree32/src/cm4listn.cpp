
#include "cm4listn.hpp"

CM4ListNode::CM4ListNode (void)
{
	m_pList = new PPCM4Node[M4TREE_SIZE_LIST_NODE];
	if (m_pList == NULL)
		return;
	for (long i=0L; i<M4TREE_SIZE_LIST_NODE; i++)
		m_pList[i] = NULL;
}

CM4ListNode::~CM4ListNode (void)
{
	if (m_pList)
	{
		for (long i=0L; i<M4TREE_SIZE_LIST_NODE; i++)
		{		
			if (m_pList[i])
			{
				delete [] m_pList[i];
				m_pList [i] = NULL;
			}
		}
		M4DELETEV(m_pList);
		m_pList = NULL;
	}
}

