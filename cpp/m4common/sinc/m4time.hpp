
#ifndef __M4TIME_HPP__
#define __M4TIME_HPP__


#ifdef _WINDOWS
	#include <time.h>

	#ifdef _WIN32
		#include <windows.h>
		#include <winbase.h>
		// se añade esta cabecera para el GetTickCount
	#endif

	#include ".\windows\m4witime.hpp"
#endif

#ifdef _SOLARIS
#include "./solaris/m4sotime.hpp"
#endif

#ifdef _HP
#include "./hp/m4hptime.hpp"
#endif

#ifdef _AIX
#include "./aix/m4aitime.hpp"
#endif


#endif

