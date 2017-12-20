
#ifndef M4PROP_DLL_HPP
#define M4PROP_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4PROP
			#define M4_DECL_M4PROP __declspec(dllexport)
	#else
			#define M4_DECL_M4PROP __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4PROP
#endif
#endif

