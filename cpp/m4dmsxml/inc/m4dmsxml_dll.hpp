
#ifndef M4DMSXML_DLL_HPP
#define M4DMSXML_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4DMSXML
			#define M4_DECL_M4DMSXML __declspec(dllexport)
	#else
			#define M4_DECL_M4DMSXML __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4DMSXML
#endif
#endif

