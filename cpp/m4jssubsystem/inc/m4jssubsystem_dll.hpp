
#ifndef M4JSSUBSYSTEM_DLL_HPP
#define M4JSSUBSYSTEM_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4JSSUBSYSTEM
			#define M4_DECL_M4JSSUBSYSTEM __declspec(dllexport)
	#else
			#define M4_DECL_M4JSSUBSYSTEM __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4JSSUBSYSTEM
#endif
#endif

