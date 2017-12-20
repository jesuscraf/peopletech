
#ifndef M4CACERT_DLL_HPP
#define M4CACERT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CACERT
			#define M4_DECL_M4CACERT __declspec(dllexport)
	#else
			#define M4_DECL_M4CACERT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CACERT
#endif
#endif

