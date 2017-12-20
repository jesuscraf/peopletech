
#ifndef M4OBL_DLL_HPP
#define M4OBL_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4OBL
			#define M4_DECL_M4OBL __declspec(dllexport)
	#else
			#define M4_DECL_M4OBL __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4OBL
#endif
#endif

