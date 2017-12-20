
#ifndef M4CONFIGTREE_DLL_HPP
#define M4CONFIGTREE_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4CONFIGTREE
			#define M4_DECL_M4CONFIGTREE __declspec(dllexport)
	#else
			#define M4_DECL_M4CONFIGTREE __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4CONFIGTREE
#endif
#endif

