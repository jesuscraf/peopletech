
#ifndef M4VARIANT_DLL_HPP
#define M4VARIANT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4VARIANT
			#define M4_DECL_M4VARIANT __declspec(dllexport)
	#else
			#define M4_DECL_M4VARIANT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4VARIANT
#endif
#endif

