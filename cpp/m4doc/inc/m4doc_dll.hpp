
#ifndef M4DOC_DLL_HPP
#define M4DOC_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4DOC
			#define M4_DECL_M4DOC __declspec(dllexport)
	#else
			#define M4_DECL_M4DOC __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4DOC
#endif
#endif

