
BOOL M4InsertNode (CM4Node *a_pNode)
{
	long lIndex = (long) floor ((double) a_pNode->M4GetNodeID () / (double) M4TREE_SIZE_LIST_NODE);
	if (m_pList[lIndex] == NULL)
		m_pList[lIndex] = new PCM4Node[M4TREE_SIZE_LIST_NODE];
	if (m_pList[lIndex] == NULL)
		return FALSE;
	long lNode = a_pNode->M4GetNodeID () - (lIndex * M4TREE_SIZE_LIST_NODE);
	m_pList[lIndex][lNode] = a_pNode;
	return TRUE;
}
	
CM4Node *M4FindNode (long a_lNodeID)
{
	long lIndex = (long) floor ((double) a_lNodeID / (double) M4TREE_SIZE_LIST_NODE);
	long lNode = a_lNodeID - (lIndex * M4TREE_SIZE_LIST_NODE);
	return m_pList[lIndex][lNode];
}

BOOL M4DeleteNode (CM4Node *a_pNode)
{
	long lIndex = (long) floor ((double) a_pNode->M4GetNodeID () / (double) M4TREE_SIZE_LIST_NODE);
	long lNode = a_pNode->M4GetNodeID () - (lIndex * M4TREE_SIZE_LIST_NODE);
	m_pList[lIndex][lNode] = NULL;
	return TRUE;
}

BOOL M4DeleteNode (long a_lNodeID)
{
	long lIndex = (long) floor ((double) a_lNodeID / (double) M4TREE_SIZE_LIST_NODE);
	long lNode = a_lNodeID - (lIndex * M4TREE_SIZE_LIST_NODE);
	m_pList[lIndex][lNode] = NULL;
	return TRUE;
}

