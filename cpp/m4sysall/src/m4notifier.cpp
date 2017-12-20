//## begin module%3912C58502D0.cm preserve=no
//## end module%3912C58502D0.cm

//## begin module%3912C58502D0.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%3912C58502D0.cp

//## Module: m4notifier%3912C58502D0; Package body
//## Subsystem: m4sysall::src%3912C5DD0060
//## Source file: d:\factor\m4sysall\src\m4notifier.cpp

//## begin module%3912C58502D0.additionalIncludes preserve=no
//## end module%3912C58502D0.additionalIncludes

//## begin module%3912C58502D0.includes preserve=yes
#include "m4notifier.hpp"
#include "m4event.hpp"	// Para que me lo exporte.
//## end module%3912C58502D0.includes


class M4_DECL_M4SYSALL ClEvent;

//## begin module%3912C58502D0.declarations preserve=no
//## end module%3912C58502D0.declarations

//## begin module%3912C58502D0.additionalDeclarations preserve=yes
//## end module%3912C58502D0.additionalDeclarations


// Class ClNotifier 



//## Other Operations (implementation)
void ClNotifier::OnKill (ClEvent& ai_pEvent)
{
  //## begin ClNotifier::OnKill%341468323.body preserve=yes
	NotifiableSet CopySet = m_setNotifiable;
	
	itNotifiableSet it = CopySet.begin();
	itNotifiableSet itend = CopySet.end();

	while (it!=itend){
		(*it)->OnKill(ai_pEvent);
		it++;
	}
  //## end ClNotifier::OnKill%341468323.body
}

m4return_t ClNotifier::Attach (ClNotifiable* ai_pNewClient)
{
  //## begin ClNotifier::Attach%-226474902.body preserve=yes
	
	if (ai_pNewClient){
		// Me da igual lo que devuelva. Si lo ha insertado bien, y si no es que ya estaba.
		m_setNotifiable.insert(ai_pNewClient);
	}

	return M4_SUCCESS;
  //## end ClNotifier::Attach%-226474902.body
}

m4return_t ClNotifier::Dettach (ClNotifiable* ai_pOldClient)
{
  //## begin ClNotifier::Dettach%-1208465467.body preserve=yes
	if (ai_pOldClient){
		// Me da igual lo que devuelva. Si lo ha borrado bien, y si no es que no estaba.
		m_setNotifiable.erase(ai_pOldClient);
	}
	return M4_SUCCESS;
  //## end ClNotifier::Dettach%-1208465467.body
}

void ClNotifier::Signal (ClEvent& ai_pEvent)
{
  //## begin ClNotifier::Signal%-489603831.body preserve=yes

	// Me hago copia del set por si se me atachan o desatachan durante la notificacion.
	NotifiableSet CopySet = m_setNotifiable;
	
	itNotifiableSet it = CopySet.begin();
	itNotifiableSet itend = CopySet.end();

	m4bool_t bErase = M4_TRUE;

	while (it!=itend){
		bErase = (*it)->Notify (ai_pEvent);
		if (bErase){
			// Lo borro del original.
			m_setNotifiable.erase(*it);
		}
		it++;
	}
  //## end ClNotifier::Signal%-489603831.body
}

// Additional Declarations
  //## begin ClNotifier%3912C584013C.declarations preserve=yes
  //## end ClNotifier%3912C584013C.declarations

//## begin module%3912C58502D0.epilog preserve=yes
//## end module%3912C58502D0.epilog
