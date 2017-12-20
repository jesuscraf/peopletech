
#ifndef M4MEMDBG_DLL_HPP
#define M4MEMDBG_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MEMDBG
			#define M4_DECL_M4MEMDBG __declspec(dllexport)
	#else
			#define M4_DECL_M4MEMDBG __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MEMDBG
#endif
#endif

