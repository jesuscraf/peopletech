
#ifndef M4MEMINFO_DLL_HPP
#define M4MEMINFO_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MEMINFO
			#define M4_DECL_M4MEMINFO __declspec(dllexport)
	#else
			#define M4_DECL_M4MEMINFO __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MEMINFO
#endif
#endif

