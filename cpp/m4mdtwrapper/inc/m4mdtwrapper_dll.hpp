
#ifndef M4MDTWRAPPER_DLL_HPP
#define M4MDTWRAPPER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MDTWRAPPER
			#define M4_DECL_M4MDTWRAPPER __declspec(dllexport)
	#else
			#define M4_DECL_M4MDTWRAPPER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MDTWRAPPER
#endif
#endif

