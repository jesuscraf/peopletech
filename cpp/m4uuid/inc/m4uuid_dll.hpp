
#ifndef M4UUID_DLL_HPP
#define M4UUID_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4UUID
			#define M4_DECL_M4UUID __declspec(dllexport)
	#else
			#define M4_DECL_M4UUID __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4UUID
#endif
#endif

