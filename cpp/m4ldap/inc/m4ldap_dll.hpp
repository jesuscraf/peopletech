
#ifndef M4LDAP_DLL_HPP
#define M4LDAP_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4LDAP
			#define M4_DECL_M4LDAP __declspec(dllexport)
	#else
			#define M4_DECL_M4LDAP __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4LDAP
#endif
#endif

