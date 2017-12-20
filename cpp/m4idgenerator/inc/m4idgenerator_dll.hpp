
#ifndef M4IDGENERATOR_DLL_HPP
#define M4IDGENERATOR_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4IDGENERATOR
			#define M4_DECL_M4IDGENERATOR __declspec(dllexport)
	#else
			#define M4_DECL_M4IDGENERATOR __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4IDGENERATOR
#endif
#endif

