
#ifndef __AUTOLOAD_HPP__
#define __AUTOLOAD_HPP__
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "cldefine.hpp"

#include "property.h"


class ClAutoLoad_PlugIn;
class ClSize;


// ------------------------------------------------------------------
// class ClAutoLoadProperty
// ------------------------------------------------------------------

class M4_DECL_M4DM ClAutoLoadProperty : public ClProperty
{
public:
    ClAutoLoadProperty(void)
        : m_paupi(NULL)
    {
        m_ePropertyId = e_ClAutoLoadProperty;
    }

    void Attach (ClAutoLoad_PlugIn *ai_paupi)
    {
        M4_ASSERT (ai_paupi);
        m_paupi = ai_paupi;
    }

	virtual m4return_t Get(m4VariantType &ao_var) const;
	virtual m4return_t Set(const m4VariantType &ai_var);

	m4return_t Get(m4uint8_t &ao_var) const;
	m4return_t Set(m4uint8_t ai_var);
	
	virtual m4return_t GetSize(ClSize &ao_size) const;

    virtual m4return_t _Serialize(ClGenericIODriver& IOD);
    virtual m4return_t _DeSerialize(ClGenericIODriver& IOD);

private:
    ClAutoLoad_PlugIn* m_paupi;
};


// ------------------------------------------------------------------
// class ClAutoLoad_PlugIn
// ------------------------------------------------------------------

class M4_DECL_M4DM ClAutoLoad_PlugIn
{
public:
    ClAutoLoad_PlugIn(void)
        : m_iautoloadmode(M4CL_AUTOLOAD_OFF)
    {
        WantsAutoLoad.Attach (this);
    }

    m4uint8_t GetAutoLoadMode (void) const
    {
        return m_iautoloadmode;
    }

    virtual m4return_t SetAutoLoadMode (m4uint8_t ai_iautoloadmode)
    {
        m_iautoloadmode = ai_iautoloadmode;

        return M4_SUCCESS;
    }

	virtual m4return_t GetSize(ClSize &ao_size)
	{
		ao_size += sizeof (ClAutoLoad_PlugIn);
        ao_size -= sizeof (WantsAutoLoad);

		if (WantsAutoLoad.GetSize(ao_size) != M4_SUCCESS)
			return M4_ERROR;

		return M4_SUCCESS;
	}

    ClAutoLoadProperty WantsAutoLoad;

private:
    m4uint8_t m_iautoloadmode;
};


#endif //__AUTOLOAD_HPP__
