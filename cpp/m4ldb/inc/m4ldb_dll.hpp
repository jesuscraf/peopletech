
#ifndef M4LDB_DLL_HPP
#define M4LDB_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4LDB
			#define M4_DECL_M4LDB __declspec(dllexport)
	#else
			#define M4_DECL_M4LDB __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4LDB
#endif
#endif

