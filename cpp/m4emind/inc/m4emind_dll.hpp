
#ifndef M4EMIND_DLL_HPP
#define M4EMIND_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4EMIND
			#define M4_DECL_M4EMIND __declspec(dllexport)
	#else
			#define M4_DECL_M4EMIND __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4EMIND
#endif
#endif

