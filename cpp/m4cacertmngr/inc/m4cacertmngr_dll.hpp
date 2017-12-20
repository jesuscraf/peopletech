
#ifndef M4CACERTMNGR_DLL_HPP
#define M4CACERTMNGR_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CACERTMNGR
			#define M4_DECL_M4CACERTMNGR __declspec(dllexport)
	#else
			#define M4_DECL_M4CACERTMNGR __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CACERTMNGR
#endif
#endif

