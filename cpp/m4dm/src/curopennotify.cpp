#include "curopennotify.hpp"


m4return_t 
ClCursorOpened_Notifier::FreeOpenedCursors(void)
{
	ClCursor *pLast = 0;
	ituser_t	it;
	while (m_sUsed.empty()==0){
		it = m_sUsed.begin();
		if (pLast == (*it)){
			// No estamos liberando el cursor. porque estamos con el mismo Stmt.
			// Lo borramos y seguimos con los demas.
			// Se cerrara cuando se cierre la conexion
			m_sUsed.erase (*it);
			continue;
		}
		pLast = (*it);
//		M4ClosePartialLoad (pLast); FJSM EVENTOS....
		// OJO!!. A partir de aqui el iterador se queda invalido, porque el Close me lo va a borrar.
	}

	return M4_SUCCESS;
}
