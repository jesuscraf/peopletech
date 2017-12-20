
#ifndef M4CONFIGURATION_DLL_HPP
#define M4CONFIGURATION_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CONFIGURATION
			#define M4_DECL_M4CONFIGURATION __declspec(dllexport)
	#else
			#define M4_DECL_M4CONFIGURATION __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CONFIGURATION
#endif
#endif

