
#ifndef M4SUBSYSTEMS_DLL_HPP
#define M4SUBSYSTEMS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SUBSYSTEMS
			#define M4_DECL_M4SUBSYSTEMS __declspec(dllexport)
	#else
			#define M4_DECL_M4SUBSYSTEMS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SUBSYSTEMS
#endif
#endif

