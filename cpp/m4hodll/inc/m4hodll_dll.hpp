
#ifndef M4HODLL_DLL_HPP
#define M4HODLL_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4HODLL
			#define M4_DECL_M4HODLL __declspec(dllexport)
	#else
			#define M4_DECL_M4HODLL __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4HODLL
#endif
#endif

