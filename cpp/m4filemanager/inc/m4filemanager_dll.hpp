
#ifndef M4FILEMANAGER_DLL_HPP
#define M4FILEMANAGER_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4FILEMANAGER
			#define M4_DECL_M4FILEMANAGER __declspec(dllexport)
	#else
			#define M4_DECL_M4FILEMANAGER __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4FILEMANAGER
#endif
#endif

