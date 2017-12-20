/*
+-------------------------------------------------------------------------+
| COPYRIGHT © 1996-1999 Meta4 Spain, S.A.                                 |
| All rights reserved.                                                    |
+-------------------------------------------------------------------------+
| Module           : [M4UUID] <Meta4 UUID Generator>                      |
| File             : <multipla.hpp>                                       |
| Project          : Meta4Mind®©                                          |
| Author           : Meta4 Spain, S.A.                                    |
| Date             : 08/Jun/1999                                          |
| Language         : C++                                                  |
| Operating System : All                                                  |
| Design Document  :                                                      |
| Description      :                                                      |
|                                                                         |
+-------------------------------------------------------------------------+
| History                                                                 |
|                                                                         |
|     <Ver>  <Programer>     <Date>                                       |
|        1    J.G.L.          08/Jun/1999                                 |                                                                         |
+-------------------------------------------------------------------------+
*/

#ifndef _MULTIPLA__HPP_
#define _MULTIPLA__HPP_

/*
+-------------------------------------------------------------------------+
| External dependencies                                                   |
+-------------------------------------------------------------------------+
*/

#if defined( _WINDOWS ) || defined ( WINDOWS ) || defined( _WIN16 ) || defined( _M4WINX86_ )

//--- Begin Windows dependencies 

	#include <windows.h>
	#include "m4win.hpp"
    #include "m4time.hpp"


//--- End Windows dependencies 

#else

//--- Begin Unix dependencies 

	#ifndef bool	
		#define bool m4bool_t
	#endif

	#ifdef BOOL
		#undef BOOL
	#endif


	#ifdef _UNIX
		#include <unistd.h>
	#endif

//--- End Unix dependencies 

#endif

#endif
