
#ifndef M4SERVENV_DLL_HPP
#define M4SERVENV_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SERVENV
			#define M4_DECL_M4SERVENV __declspec(dllexport)
	#else
			#define M4_DECL_M4SERVENV __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SERVENV
#endif
#endif

