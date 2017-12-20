
#ifndef M4EXECUTIVE_DLL_HPP
#define M4EXECUTIVE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4EXECUTIVE
			#define M4_DECL_M4EXECUTIVE __declspec(dllexport)
	#else
			#define M4_DECL_M4EXECUTIVE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4EXECUTIVE
#endif
#endif

