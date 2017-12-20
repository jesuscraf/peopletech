
#ifndef M4MODKEY_DLL_HPP
#define M4MODKEY_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4MODKEY
			#define M4_DECL_M4MODKEY __declspec(dllexport)
	#else
			#define M4_DECL_M4MODKEY __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4MODKEY
#endif
#endif

