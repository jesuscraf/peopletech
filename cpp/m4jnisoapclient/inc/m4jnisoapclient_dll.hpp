
#ifndef M4JNISOAPCLIENT_DLL_HPP
#define M4JNISOAPCLIENT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4JNISOAPCLIENT
			#define M4_DECL_M4JNISOAPCLIENT __declspec(dllexport)
	#else
			#define M4_DECL_M4JNISOAPCLIENT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4JNISOAPCLIENT
#endif
#endif

