
#ifndef M4USERINTERFACE_DLL_HPP
#define M4USERINTERFACE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4USERINTERFACE
			#define M4_DECL_M4USERINTERFACE __declspec(dllexport)
	#else
			#define M4_DECL_M4USERINTERFACE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4USERINTERFACE
#endif
#endif

