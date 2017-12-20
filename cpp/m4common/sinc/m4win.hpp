
#ifndef __M4WIN_HPP__
#define __M4WIN_HPP__

#include "m4types.hpp"

#if defined WIN32 && !defined _WIN32
     #define _WIN32
#endif

#ifdef _WIN32
	#include <windows.h>
	#include ".\windows\m432win.hpp"
#elif defined _UNIX
	#include "./unix/m4uxwin.hpp"
#else
	error  hay que definir una plataforma
#endif


#endif

