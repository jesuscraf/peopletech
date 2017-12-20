
#ifndef M4MDFAC_DLL_HPP
#define M4MDFAC_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MDFAC
			#define M4_DECL_M4MDFAC __declspec(dllexport)
	#else
			#define M4_DECL_M4MDFAC __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MDFAC
#endif
#endif

