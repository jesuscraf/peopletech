
#ifndef M4WIN2UX_DLL_HPP
#define M4WIN2UX_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4WIN2UX
			#define M4_DECL_M4WIN2UX __declspec(dllexport)
	#else
			#define M4_DECL_M4WIN2UX __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4WIN2UX
#endif
#endif

