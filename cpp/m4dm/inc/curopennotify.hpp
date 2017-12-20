
#ifndef _CUROPENNOTIFY_HPP
#define _CUROPENNOTIFY_HPP

#include "usernotifier.hpp"
#include "m4types.hpp"

// Por un bug de OS, hay que hacer :
class ClCursor;

class ClCursorOpened_Notifier	: public ClUsingNotify<ClCursor>{
public:
	m4return_t FreeOpenedCursors(void);
};




#endif

