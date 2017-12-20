
#ifndef M4SERVERBASE_DLL_HPP
#define M4SERVERBASE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SERVERBASE
			#define M4_DECL_M4SERVERBASE __declspec(dllexport)
	#else
			#define M4_DECL_M4SERVERBASE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SERVERBASE
#endif
#endif

