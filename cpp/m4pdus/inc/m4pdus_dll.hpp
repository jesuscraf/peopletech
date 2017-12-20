
#ifndef M4PDUS_DLL_HPP
#define M4PDUS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4PDUS
			#define M4_DECL_M4PDUS __declspec(dllexport)
	#else
			#define M4_DECL_M4PDUS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4PDUS
#endif
#endif

