
#ifndef M4IPUTILS_DLL_HPP
#define M4IPUTILS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4IPUTILS
			#define M4_DECL_M4IPUTILS __declspec(dllexport)
	#else
			#define M4_DECL_M4IPUTILS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4IPUTILS
#endif
#endif

