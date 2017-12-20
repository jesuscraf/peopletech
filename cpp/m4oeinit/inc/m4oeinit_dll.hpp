
#ifndef M4OEINIT_DLL_HPP
#define M4OEINIT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4OEINIT
			#define M4_DECL_M4OEINIT __declspec(dllexport)
	#else
			#define M4_DECL_M4OEINIT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4OEINIT
#endif
#endif

