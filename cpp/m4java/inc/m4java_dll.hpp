
#ifndef M4JAVA_DLL_HPP
#define M4JAVA_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4JAVA
			#define M4_DECL_M4JAVA __declspec(dllexport)
#else
			#define M4_DECL_M4JAVA __declspec(dllimport)
#endif
#else
			#define M4_DECL_M4JAVA
#endif
#endif

