//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             accessSeq.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the CAccess class.
//
//
//==============================================================================


#include "asndef.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAccessSeq::CAccessSeq()
{
	channel = NULL;
	node = NULL;
	item = NULL;
	grant = 31; // valor por defecto
	grantsByUser = M4_FALSE;
	lengthChannel = 0;
	lengthNode = 0;
	lengthItem = 0;
}

CAccessSeq::~CAccessSeq()
{
	if (channel!=NULL)
		delete []channel;
	if (node!=NULL)
		delete []node;
	if (item!=NULL)
		delete []item;
	lengthChannel = 0;
	lengthNode = 0;
	lengthItem = 0;
}

/*m4return_t CAccessSeq::cmpAccess(CAccessSeq *acc)
{
	
	if (acc!=NULL)
	{
		
		if ((acc->channel!=NULL)&&(channel!=NULL))
			if ((lengthChannel==acc->lengthChannel) && 
				(memcmp(channel,acc->channel,lengthChannel) == 0))
				return M4_SUCCESS;
			else return M4_ERROR;

		else
			return M4_ERROR;
	}
	else return M4_ERROR;

		if ((acc->node!=NULL)&&(node!=NULL))
		{
			if ((lengthNode==acc->lengthNode) && 
				(memcmp(node,acc->node,lengthNode) == 0))
				cond_node = true;
			else cond_node = false;
		}
		else cond_node = ((acc->node==NULL)&&(node==NULL));

		if ((acc->item!=NULL)&&(item!=NULL))
		{
			if ((lengthItem==acc->lengthItem) && 
				(memcmp(item,acc->item,lengthItem) == 0))
				cond_item = true;
			else cond_item = false;
		}
		else cond_item = ((acc->item==NULL)&&(item==NULL));
	}

	if (cond_channel && cond_node && cond_item)
		return M4_SUCCESS;
	else return M4_ERROR;
	

}
*/
