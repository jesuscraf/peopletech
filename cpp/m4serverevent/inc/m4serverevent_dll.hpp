
#ifndef M4SERVEREVENT_DLL_HPP
#define M4SERVEREVENT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SERVEREVENT
			#define M4_DECL_M4SERVEREVENT __declspec(dllexport)
	#else
			#define M4_DECL_M4SERVEREVENT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SERVEREVENT
#endif
#endif

