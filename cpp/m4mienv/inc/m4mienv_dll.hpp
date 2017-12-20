
#ifndef M4MIENV_DLL_HPP
#define M4MIENV_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MIENV
			#define M4_DECL_M4MIENV __declspec(dllexport)
	#else
			#define M4_DECL_M4MIENV __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MIENV
#endif
#endif

