
#ifndef M4DBWRAP_DLL_HPP
#define M4DBWRAP_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4DBWRAP
			#define M4_DECL_M4DBWRAP __declspec(dllexport)
	#else
			#define M4_DECL_M4DBWRAP __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4DBWRAP
#endif
#endif

