
#ifndef M4INI_DLL_HPP
#define M4INI_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4INI
			#define M4_DECL_M4INI __declspec(dllexport)
	#else
			#define M4_DECL_M4INI __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4INI
#endif
#endif

