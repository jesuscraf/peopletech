
#ifndef M4PRINTPDF_DLL_HPP
#define M4PRINTPDF_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4PRINTPDF
			#define M4_DECL_M4PRINTPDF __declspec(dllexport)
	#else
			#define M4_DECL_M4PRINTPDF __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4PRINTPDF
#endif
#endif

