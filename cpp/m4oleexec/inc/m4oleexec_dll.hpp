
#ifndef M4OLEEXEC_DLL_HPP
#define M4OLEEXEC_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4OLEEXEC
			#define M4_DECL_M4OLEEXEC __declspec(dllexport)
	#else
			#define M4_DECL_M4OLEEXEC __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4OLEEXEC
#endif
#endif

