#include "property.h"

#ifndef __NODPROP_HPP__
#define __NODPROP_HPP__

//ojo que el código está en chanprop.cpp

class M4_DECL_M4DM ClAutoLoadProperty_Node : public ClProperty
{
public:
	ClAutoLoadProperty_Node ();
	m4return_t Get(m4VariantType &ao_var);
	m4return_t Set(const m4VariantType &ai_var);

	m4return_t Get(m4uint8_t &ao_var);
	m4return_t Set(m4uint8_t ai_var);

private:	
    m4uint8_t m_iautoload;
};

#endif //__NODPROP_HPP__
