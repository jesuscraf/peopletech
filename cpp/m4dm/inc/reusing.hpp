#ifndef __REUSING__HPP__
#define __REUSING__HPP__

#include "m4types.hpp"

class ClSize;

#define M4_UKCHACHE_SIZE 8

class ClReusingBlockUKey
{
public:

    ClReusingBlockUKey(void);

	void Reset (void);

	void Reuse(m4uint32_t ai_iUniqueKey);

	m4return_t Get(m4uint32_t & ao_iUniqueKey);

	m4return_t GetSize(ClSize &ao_size);

protected:

    m4uint32_t m_UKCache[M4_UKCHACHE_SIZE];
    m4uint32_t m_iLastUsed;
	m4uint32_t m_iFreeCursor;
};

#endif //__REUSING__HPP__
