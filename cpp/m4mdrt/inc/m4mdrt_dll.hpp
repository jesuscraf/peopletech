
#ifndef M4MDRT_DLL_HPP
#define M4MDRT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MDRT
			#define M4_DECL_M4MDRT __declspec(dllexport)
	#else
			#define M4_DECL_M4MDRT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MDRT
#endif
#endif

