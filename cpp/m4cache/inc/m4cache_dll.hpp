
#ifndef M4CACHE_DLL_HPP
#define M4CACHE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CACHE
			#define M4_DECL_M4CACHE __declspec(dllexport)
	#else
			#define M4_DECL_M4CACHE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CACHE
#endif
#endif

