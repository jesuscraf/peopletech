
#ifndef M4JSUTILS_DLL_HPP
#define M4JSUTILS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4JSUTILS
			#define M4_DECL_M4JSUTILS __declspec(dllexport)
	#else
			#define M4_DECL_M4JSUTILS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4JSUTILS
#endif
#endif

