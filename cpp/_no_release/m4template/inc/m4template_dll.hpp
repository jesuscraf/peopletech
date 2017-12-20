
#ifndef M4TEMPLATE_DLL_HPP
#define M4TEMPLATE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4TEMPLATE
			#define M4_DECL_M4TEMPLATE __declspec(dllexport)
	#else
			#define M4_DECL_M4TEMPLATE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4TEMPLATE
#endif
#endif

