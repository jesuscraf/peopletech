
CM4Node *M4GetRootNode (void)
{
	return m_pRoot;
}

CM4Node *M4GetCurrentNode (void)
{
	return m_pCurrentNode;
}

CM4Node *M4GetNextNode (void)
{
	return m_pNextNode;
}

void M4SetCurrentNode (CM4Node *a_pCurrentNode)
{
	m_pCurrentNode = a_pCurrentNode;
}

long M4GetLevelCount (void)
{
	return m_lLevelCount;
}

long M4GetNodeCount (void)
{
	return m_lNodeCount;
}

CM4Node *M4FindNode (long a_lNodeID)
{
	if (a_lNodeID == M4TREE_CURRENT_NODE)
		return m_pCurrentNode;
	else
		return m_pListNode->M4FindNode (a_lNodeID);
}