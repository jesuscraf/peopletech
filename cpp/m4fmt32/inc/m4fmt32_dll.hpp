
#ifndef M4FMT32_DLL_HPP
#define M4FMT32_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4FMT32
			#define M4_DECL_M4FMT32 __declspec(dllexport)
	#else
			#define M4_DECL_M4FMT32 __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4FMT32
#endif
#endif

