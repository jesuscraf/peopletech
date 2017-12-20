
#include "stdafx.h"
#include "m4chwrap.h"
#include "wraprgit_blockwalker.h"
#include "wrapdef.h"
#include "wrapnode.h"
#include "ritbwalk.hpp"
#include "node.hpp"


/////////////////////////////////////////////////////////////////////////////
// CWrapRgIt_BlockWalker

CWrapRgIt_BlockWalker::CWrapRgIt_BlockWalker ()
{
    m_poRegister = new ClRegister_BlockWalker (0 /*de momento sin autoload*/);
}

CWrapRgIt_BlockWalker::~CWrapRgIt_BlockWalker ()
{
    delete m_poRegister;
}

STDMETHODIMP CWrapRgIt_BlockWalker::Attach (IWrapNode *ai_pNode, short* ao_RetVal)
{
	ENTER_METHOD(CWrapRgIt_BlockWalker::Attach);

    ClNode *pnode;
    ((CWrapNode*)ai_pNode)->Unwrap(&pnode);

    //el nodo:
    ((ClRegister_BlockWalker *)m_poRegister)->Attach (pnode->GetpNode_Data ());

    //el access, chapucilla para no recompilar:
    ((ClRegister_BlockWalker *)m_poRegister)->ClRegister::Attach (pnode->GetpAccess ());

	LEAVE_METHOD(CWrapRgIt_BlockWalker::Attach, S_OK);
}

STDMETHODIMP CWrapRgIt_BlockWalker::get_RegisterIterator(IWrapRgIt** pVal)
{
	ENTER_METHOD(CWrapRgIt_BlockWalker::get_RegisterIterator);

	// Just cast this and return a pointer to
	// the RgItFilter
	// Add one to the reference counts...
	// of the base class...
	*pVal = (IWrapRgIt*) this;

	TRACE("Casting CWrapRgIt_BlockWalker to CWrapRgIt");

	((IUnknown*)*pVal)->AddRef();

	LEAVE_METHOD(CWrapRgIt_BlockWalker::get_RegisterIterator, S_OK);
}
