
#ifndef M4IPCS_DLL_HPP
#define M4IPCS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4IPCS
			#define M4_DECL_M4IPCS __declspec(dllexport)
	#else
			#define M4_DECL_M4IPCS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4IPCS
#endif
#endif

