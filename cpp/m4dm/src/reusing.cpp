#include "reusing.hpp"
#include "sizer.hpp"

#define M4DM_LAST_UNIQUE_KEY    0xffffffff

ClReusingBlockUKey::ClReusingBlockUKey(void)
{
	Reset ();
}

void ClReusingBlockUKey::Reset (void)
{
    m4uint32_t i;

    for (i = 0; i < M4_UKCHACHE_SIZE; i++)
    {
        m_UKCache[i] = M4DM_LAST_UNIQUE_KEY;
    }
    m_iLastUsed = 0;
	m_iFreeCursor = M4DM_LAST_UNIQUE_KEY;
}

void ClReusingBlockUKey::Reuse(m4uint32_t ai_iUniqueKey)
{
/*
    m4uint32_t i = 0;

	//buscamos una entrada libre
    while ((m_UKCache[i] != M4DM_LAST_UNIQUE_KEY)&&(i < M4_UKCHACHE_SIZE))
        i ++;
*/
    if ((m_iFreeCursor + 1) < M4_UKCHACHE_SIZE)
	{
		m_iFreeCursor++;
        m_UKCache[m_iFreeCursor] = ai_iUniqueKey;	//nueva entrada
	}
}

m4return_t ClReusingBlockUKey::Get(m4uint32_t & ao_iUniqueKey)
{
//    m4int32_t i = M4_UKCHACHE_SIZE - 1;
/*
    //buscamos una entrada ocupada
	while ((m_UKCache[i] == M4DM_LAST_UNIQUE_KEY)&&(i >= 0))
        i --;
*/
    if (m_iFreeCursor != M4DM_LAST_UNIQUE_KEY)
    {
		//encontrado
        ao_iUniqueKey = m_UKCache[m_iFreeCursor];
        m_UKCache[m_iFreeCursor] = M4DM_LAST_UNIQUE_KEY;
		m_iFreeCursor--;
        return M4_SUCCESS;
    }

    else if (m_iLastUsed != M4DM_LAST_UNIQUE_KEY)
    {
        ao_iUniqueKey = m_iLastUsed ++;
        return M4_SUCCESS;
    }
    else {
		//error muy gordo, nos hemos quedado sin numeros
        return M4_ERROR;
	}
}
m4return_t 
ClReusingBlockUKey::GetSize(ClSize &ao_size)
{
    ao_size+=sizeof(ClReusingBlockUKey);
	return M4_SUCCESS;
}
