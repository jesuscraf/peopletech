
#ifndef M4CRYPTWRAP_DLL_HPP
#define M4CRYPTWRAP_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CRYPTWRAP
			#define M4_DECL_M4CRYPTWRAP __declspec(dllexport)
	#else
			#define M4_DECL_M4CRYPTWRAP __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CRYPTWRAP
#endif
#endif

