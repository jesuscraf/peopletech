#ifndef __W_PRX_HPP__ 
#define __W_PRX_HPP__ 

#include "m4types.hpp"

class _AClItemProxy;
class ClConnectItemInfo;
class ClSize;

class M4_DECL_M4DM _withitemproxy
{
public:
    _withitemproxy ();

    _AClItemProxy *GetpItemProxy (void) const
    {
        return m_pClItemProxy;
    }

    virtual void SetpItemProxy (_AClItemProxy *ai_p)
    {
        m_pClItemProxy = ai_p;
    }

    ClConnectItemInfo *GetpConnectItemInfo (void) const;

    m4uint16_t GetPos (void) const;

    void SetEOF (void);

	m4return_t GetSize(ClSize &ao_size);

private:
    _AClItemProxy       *m_pClItemProxy;
};

#endif // __ITEM_PRX_HPP__ 
