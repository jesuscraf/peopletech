
#ifndef M4COMPRESS_DLL_HPP
#define M4COMPRESS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4COMPRESS
			#define M4_DECL_M4COMPRESS __declspec(dllexport)
	#else
			#define M4_DECL_M4COMPRESS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4COMPRESS
#endif
#endif

