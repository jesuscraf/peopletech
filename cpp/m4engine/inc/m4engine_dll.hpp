
#ifndef M4ENGINE_DLL_HPP
#define M4ENGINE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4ENGINE
			#define M4_DECL_M4ENGINE __declspec(dllexport)
	#else
			#define M4_DECL_M4ENGINE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4ENGINE
#endif
#endif

