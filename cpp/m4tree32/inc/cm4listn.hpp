#ifndef CM4LISTNODE_HPP
#define CM4LISTNODE_HPP

#include "m4detree.hpp"
#include "cm4node.hpp"
#include "math.h"

class CM4ListNode
{
private:
	PPCM4Node *m_pList;

public:
	CM4ListNode (void);
	~CM4ListNode (void);

#include "cm4listn.inl"

};

#endif
