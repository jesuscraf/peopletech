
#ifndef M4EVENTHANDLER_DLL_HPP
#define M4EVENTHANDLER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4EVENTHANDLER
			#define M4_DECL_M4EVENTHANDLER __declspec(dllexport)
	#else
			#define M4_DECL_M4EVENTHANDLER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4EVENTHANDLER
#endif
#endif

