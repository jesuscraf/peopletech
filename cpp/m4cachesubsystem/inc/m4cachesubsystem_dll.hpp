
#ifndef M4CACHESUBSYSTEM_DLL_HPP
#define M4CACHESUBSYSTEM_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CACHESUBSYSTEM
			#define M4_DECL_M4CACHESUBSYSTEM __declspec(dllexport)
	#else
			#define M4_DECL_M4CACHESUBSYSTEM __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CACHESUBSYSTEM
#endif
#endif

