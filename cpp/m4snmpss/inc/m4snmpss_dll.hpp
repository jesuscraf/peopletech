
#ifndef M4SNMPSS_DLL_HPP
#define M4SNMPSS_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4SNMPSS
			#define M4_DECL_M4SNMPSS __declspec(dllexport)
	#else
			#define M4_DECL_M4SNMPSS __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4SNMPSS
#endif
#endif

