#ifndef ClHandle
#include "handles.hpp"
#endif

#ifndef __ITEMCONN_HPP__
#define __ITEMCONN_HPP__


class ClItem;
class ClNode_Data;


class _AClConnectorImplementation
{
protected:
    ClNode_Data    *m_pNodeChild;
    ClHandle        m_hItemChild;
    m4uint16_t      m_ichildnodepos;

    //_AClConnectorImplementation (/*ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos*/);

public:
    // utilizamos un pool, con una gestión un poco especial:
    void *operator new (size_t, void *ai_pbuffer);
    void operator delete (void *);

    virtual void SetChildValue (ClItem *ai_pitem) = 0;

    static m4return_t s_GetNewConnectorImplementation (_AClConnectorImplementation *&ao_p,
        m4uint8_t ai_ichildscope, m4uint8_t ai_ifatherscope, m4uint8_t ai_iconnectiontype,
        m4uint8_t ai_iconnectionrelation, _AClConnectorImplementation *ai_pdir);

    ClNode_Data *GetpNode_Data (void) const
    {
        return m_pNodeChild;
    }

    ClHandle GetHandle (void) const
    {
        return m_hItemChild;
    }

    m4uint16_t GetChildNodePos (void) const
    {
        return m_ichildnodepos;
    }

    void Init (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos)
    {
        m_pNodeChild = ai_pNodeChild;
        m_hItemChild = ai_hitemchild;
        m_ichildnodepos = ai_ichildnodepos;

        M4_ASSERT (m_pNodeChild);
        M4_ASSERT (m_hItemChild);
    }
	m4return_t GetSize(ClSize &ao_size);
};

// Para situaciones de error, en el futuro deberia desaparecer
class ClConnectorImplementation_Null_Any_Any : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_Free_Any_Block (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem)
    {
        return;
    }
};

class ClConnectorImplementation_Any_Any_Node : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_Any_Any_Node (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem);
};

class ClConnectorImplementation_BB_Block_Block : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_BB_Block_Block (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem);
};

class ClConnectorImplementation_RB_Block_Block : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_RB_Block_Block (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem);
};

class ClConnectorImplementation_Any_Any_Register : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_Any_Any_Register (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem);
};

class ClConnectorImplementation_Any_Register_Block : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_Any_Register_Block (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem);
};

class ClConnectorImplementation_Free_Any_Block : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_Free_Any_Block (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem);
};

class ClConnectorImplementation_Auto_Register_Register : public _AClConnectorImplementation
{
public:
    //ClConnectorImplementation_Free_Any_Block (ClNode_Data *ai_pNodeChild, ClHandle ai_hitemchild, m4uint16_t ai_ichildnodepos) : 
    //    _AClConnectorImplementation (ai_pNodeChild, ai_hitemchild, ai_ichildnodepos) {}

    void SetChildValue (ClItem *ai_pitem);
};

#endif // __ITEMCONN_HPP__
