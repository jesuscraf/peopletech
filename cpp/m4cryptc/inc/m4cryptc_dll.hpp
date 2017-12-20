
#ifndef M4CRYPTC_DLL_HPP
#define M4CRYPTC_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CRYPTC
			#define M4_DECL_M4CRYPTC __declspec(dllexport)
	#else
			#define M4_DECL_M4CRYPTC __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CRYPTC
#endif
#endif

