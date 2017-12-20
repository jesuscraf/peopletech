
#ifndef M4PRESENTATION_DLL_HPP
#define M4PRESENTATION_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4PRESENTATION
			#define M4_DECL_M4PRESENTATION __declspec(dllexport)
	#else
			#define M4_DECL_M4PRESENTATION __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4PRESENTATION
#endif
#endif

