
#ifndef M4APPCLIENT_DLL_HPP
#define M4APPCLIENT_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4APPCLIENT
			#define M4_DECL_M4APPCLIENT __declspec(dllexport)
	#else
			#define M4_DECL_M4APPCLIENT __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4APPCLIENT
#endif
#endif

