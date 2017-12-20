
#ifndef M4CSDLL_DLL_HPP
#define M4CSDLL_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CSDLL
			#define M4_DECL_M4CSDLL __declspec(dllexport)
	#else
			#define M4_DECL_M4CSDLL __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CSDLL
#endif
#endif

