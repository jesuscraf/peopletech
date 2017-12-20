
#ifndef M4LDBSUBSY_DLL_HPP
#define M4LDBSUBSY_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4LDBSUBSY
			#define M4_DECL_M4LDBSUBSY __declspec(dllexport)
	#else
			#define M4_DECL_M4LDBSUBSY __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4LDBSUBSY
#endif
#endif

