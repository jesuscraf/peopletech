
#ifndef M4DM_DLL_HPP
#define M4DM_DLL_HPP

#ifdef M4_USING_DLL

	#ifdef M4_O_M4DM
			#define M4_DECL_M4DM __declspec(dllexport)
	#else
			#define M4_DECL_M4DM __declspec(dllimport)
	#endif

#else
			#define M4_DECL_M4DM
#endif

#endif

