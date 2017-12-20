
#ifndef M4BUILDVERSION_DLL_HPP
#define M4BUILDVERSION_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4BUILDVERSION
			#define M4_DECL_M4BUILDVERSION __declspec(dllexport)
	#else
			#define M4_DECL_M4BUILDVERSION __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4BUILDVERSION
#endif
#endif
