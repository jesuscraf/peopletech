#include <unistd.h>

#include "moveupfds.hpp"
#include "m4trace.hpp"
#include "blocksynchronization.hpp"


ClMutex		ClMoveUpFds::m_Mutex(M4_TRUE);
m4bool_t	ClMoveUpFds::m_bFirst = M4_TRUE;
m4bool_t	ClMoveUpFds::m_pbFds[T_ARRAY_FDS];

void	ClMoveUpFds::Initialize()
{
	int	i;

	for (i = 0; i < T_ARRAY_FDS; i++)
		m_pbFds[i] = M4_FALSE;

}

m4return_t ClMoveUpFds::Clean(int	ai_iFd)
{
	// fix bug 0095080
	ClMutBlock oBlock (&m_Mutex);

	if (m_bFirst)
	{
		m4Trace(fprintf(stderr, "Error en ClMoveUpFds::Clean Clase no inicializada\n"));

		return M4_ERROR;
	}

	if (ai_iFd - DELTA_FDS < 0 || ai_iFd - DELTA_FDS > T_ARRAY_FDS)
	{
		m4Trace(fprintf(stderr, "Error en ClMoveUpFds::Clean vale %d fuera de rango\n", ai_iFd));

		return M4_ERROR;
	}
	else
		m_pbFds[ai_iFd - DELTA_FDS] = M4_FALSE;

	return M4_SUCCESS;
}


m4return_t ClMoveUpFds::MoveUp(int &ao_iFd)
{
	int	i;

	ClMutBlock oBlock (&m_Mutex);

	if (m_bFirst)
	{
		Initialize();
		m_bFirst = M4_FALSE;
	}

	for (i = 0; i < T_ARRAY_FDS; i++)
		if (m_pbFds[i] == M4_FALSE)
		{
			if (dup2(ao_iFd, i + DELTA_FDS) != -1)
			{
				close(ao_iFd);
				m4Trace(fprintf(stderr, "He cerrado %d y lo he puesto en %d\n", ao_iFd, i+512));
				ao_iFd = i + DELTA_FDS;
				m_pbFds[i] = M4_TRUE;
				break;
			}
			else
			{
				m4Trace(fprintf(stderr, "Error en dup2 con socket %d puesto en %d\n", ao_iFd, i + DELTA_FDS));
				close(ao_iFd);

				return M4_ERROR;
			}
		}

	if (i >= T_ARRAY_FDS)
	{
		m4Trace(fprintf(stderr, "Tabla llena en dup2 con socket %d\n", ao_iFd));
		close(ao_iFd);

		return M4_ERROR;
	}

	return M4_SUCCESS;
}
