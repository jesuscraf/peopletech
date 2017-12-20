
#ifndef M4JSRECOVER_DLL_HPP
#define M4JSRECOVER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4JSRECOVER
			#define M4_DECL_M4JSRECOVER __declspec(dllexport)
	#else
			#define M4_DECL_M4JSRECOVER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4JSRECOVER
#endif
#endif

