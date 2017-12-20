
void M4SetNodeID (long a_lNodeID)
{
	m_lNodeID = a_lNodeID;
}

long M4GetNodeID (void)
{
	return m_lNodeID;
}

void M4SetParent (CM4Node *a_pParent)
{
	m_pParent = a_pParent;
}

CM4Node *M4GetParent (void)
{
	return m_pParent;
}

void M4SetPrevSibling (CM4Node *a_pPrevSibling)
{
	m_pPrevSibling = a_pPrevSibling;
}

CM4Node *M4GetPrevSibling (void)
{
	return m_pPrevSibling;
}

void M4SetNextSibling (CM4Node *a_pNextSibling)
{
	m_pNextSibling = a_pNextSibling;
}

CM4Node *M4GetNextSibling (void)
{
	return m_pNextSibling;
}

void M4SetFirstChild (CM4Node *a_pFirstChild)
{
	m_pFirstChild = a_pFirstChild;
}

CM4Node *M4GetFirstChild (void)
{
	return m_pFirstChild;
}

void M4SetLastChild (CM4Node *a_pLastChild)
{
	m_pLastChild = a_pLastChild;
}

CM4Node *M4GetLastChild (void)
{
	return m_pLastChild;
}

void M4SetExpand (BOOL a_bExpand)
{
	m_bExpand = a_bExpand;
}

BOOL M4GetExpand (void)
{
	return m_bExpand;
}

void M4SetValid (BOOL a_bValid)
{
	m_bValid = a_bValid;
}

BOOL M4GetValid (void)
{
	return m_bValid;
}

void M4SetLevel (long a_lLevel)
{
	m_lLevel = a_lLevel;
}

long M4GetLevel (void)
{
	return m_lLevel;
}

void M4SetSize (long a_lWidth, long a_lHeight)
{
	m_lWidth = a_lWidth;
	m_lHeight = a_lHeight;
}

void M4GetSize (long &a_lWidth, long &a_lHeight)
{
	a_lWidth = m_lWidth;
	a_lHeight = m_lHeight;
}

void M4SetRect (long a_lLeft, long a_lTop, long a_lRight, long a_lBottom)
{
	m_lLeft = a_lLeft;
	m_lTop = a_lTop;
	m_lRight = a_lRight;
	m_lBottom = a_lBottom;
}
	
void M4GetRect (long &a_lLeft, long &a_lTop, long &a_lRight, long &a_lBottom)
{
	a_lLeft = m_lLeft;
	a_lTop = m_lTop;
	a_lRight = m_lRight;
	a_lBottom = m_lBottom;
}
	
void M4SetRectChilds (long a_lLeft, long a_lTop, long a_lRight, long a_lBottom)
{
	m_lChildLeft = a_lLeft;
	m_lChildTop = a_lTop;
	m_lChildRight = a_lRight;
	m_lChildBottom = a_lBottom;
}

void M4GetRectChilds (long &a_lLeft, long &a_lTop, long &a_lRight, long &a_lBottom)
{
	a_lLeft = m_lChildLeft;
	a_lTop = m_lChildTop;
	a_lRight = m_lChildRight;
	a_lBottom = m_lChildBottom;
}

void M4GetRectTotal (long &a_lLeft, long &a_lTop, long &a_lRight, long &a_lBottom)
{
	a_lLeft = min (m_lLeft, m_lChildLeft);
	a_lTop = min (m_lTop, m_lChildTop);
	a_lRight = max (m_lRight, m_lChildRight);
	a_lBottom = max (m_lBottom, m_lChildBottom);
}

void M4SetNumberChildren (long a_lNumberChildren)
{
	m_lNumberChildren = a_lNumberChildren;
}

long M4GetNumberChildren (void)
{
	return m_lNumberChildren;
}

CM4Style *M4GetLocalProperty (void)
{
	return m_pLocalProperty;
}

