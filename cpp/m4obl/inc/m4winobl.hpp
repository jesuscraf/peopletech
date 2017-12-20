
#ifndef __M4WINOBL_HPP__
#define __M4WINOBL_HPP__

#if  defined( _WINDOWS ) && !defined( _WIN32 ) 
        // Si estmos en windows de 16bits lo que hacemos es identificarlo con _WIN16
        #ifndef _WIN16
            #define _WIN16
        #endif
#endif


#ifdef  _WINDOWS
    #include <windows.h>
    #if defined( _WIN32 )
        #include "m432win.hpp"	// Si estamos en windows NT incluimos su include
    #else
        #include "m416win.hpp"
    #endif
#endif

#ifdef  _UNIX
#include "m4uxwinobl.hpp"
#endif


#endif

