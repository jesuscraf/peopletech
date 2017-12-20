#ifndef _USERNOTIFIER_HPP
#define _USERNOTIFIER_HPP

#include "m4types.hpp"
#include "m4stl.hpp"
#include "sizer.hpp"


template <class T>
class ClUsingNotify{

public:
	// Notificaciones.
	m4return_t	NotifyUsing(T *);
	m4return_t	NotifyFreeing(T *);

	m4return_t  GetSize(ClSize &ao_size);


protected:
	set <T *, less<T* > >						m_sUsed;
	typedef M4_TYPE_ITERATOR set <T*, less<T* > >::iterator		ituser_t;

};

template <class T>
m4return_t ClUsingNotify<T>::NotifyUsing(T *ai_pT)
{
	if ((m_sUsed.insert(ai_pT)).second!=M4_TRUE){
		return M4_WARNING;
	}
	else {
		return M4_SUCCESS;
	}

	return M4_ERROR;
}

template <class T>
m4return_t ClUsingNotify<T>::NotifyFreeing(T *ai_pT)
{
	M4_TYPE_ITERATOR set<T *, less<T* > >::iterator it;
	it = m_sUsed.find(ai_pT);

	if (it==m_sUsed.end()){
		return M4_WARNING;
	}
	else {
		m_sUsed.erase(*it);
		return M4_SUCCESS;
	}

	return M4_ERROR;
}

template <class T>
m4return_t ClUsingNotify<T>::GetSize(ClSize &ao_size)
{
	ao_size+=m_sUsed.size()*sizeof(T *);
	ao_size+=sizeof(m_sUsed);

	return M4_SUCCESS;
}


#endif

