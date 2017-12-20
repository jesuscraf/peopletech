
#ifndef M4MODKEYWRAP_DLL_HPP
#define M4MODKEYWRAP_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MODKEYWRAP
			#define M4_DECL_M4MODKEYWRAP __declspec(dllexport)
	#else
			#define M4_DECL_M4MODKEYWRAP __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MODKEYWRAP
#endif
#endif

