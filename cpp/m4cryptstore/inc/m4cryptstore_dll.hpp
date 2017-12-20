
#ifndef M4CRYPTSTORE_DLL_HPP
#define M4CRYPTSTORE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CRYPTSTORE
			#define M4_DECL_M4CRYPTSTORE __declspec(dllexport)
	#else
			#define M4_DECL_M4CRYPTSTORE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CRYPTSTORE
#endif
#endif

