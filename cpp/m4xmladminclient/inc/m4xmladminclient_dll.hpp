
#ifndef M4XMLADMINCLIENT_DLL_HPP
#define M4XMLADMINCLIENT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4XMLADMINCLIENT
			#define M4_DECL_M4XMLADMINCLIENT __declspec(dllexport)
	#else
			#define M4_DECL_M4XMLADMINCLIENT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4XMLADMINCLIENT
#endif
#endif

