#ifndef _MOVEUP_FDS
#define _MOVEUP_FDS

#include "syncro.hpp"

#define	T_ARRAY_FDS	1024 * 3
#define	DELTA_FDS	255

class ClMoveUpFds
{
private:
	static	ClMutex		m_Mutex;
	static	m4bool_t	m_pbFds[T_ARRAY_FDS];
	static	m4bool_t	m_bFirst;
	static	void		Initialize();
public:
	static	m4return_t	MoveUp(int &ao_iFd);
	static	m4return_t	Clean(int ai_iFd);
};

#endif

