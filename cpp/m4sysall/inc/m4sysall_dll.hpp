
#ifndef M4SYSALL_DLL_HPP
#define M4SYSALL_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SYSALL
			#define M4_DECL_M4SYSALL __declspec(dllexport)
	#else
			#define M4_DECL_M4SYSALL __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SYSALL
#endif
#endif

