
#ifndef M4EXEFACTORY_DLL_HPP
#define M4EXEFACTORY_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4EXEFACTORY
			#define M4_DECL_M4EXEFACTORY __declspec(dllexport)
	#else
			#define M4_DECL_M4EXEFACTORY __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4EXEFACTORY
#endif
#endif

