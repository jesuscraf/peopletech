
#ifndef M4OBJCACHEDIRECTORY_DLL_HPP
#define M4OBJCACHEDIRECTORY_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4OBJCACHEDIRECTORY
			#define M4_DECL_M4OBJCACHEDIRECTORY __declspec(dllexport)
	#else
			#define M4_DECL_M4OBJCACHEDIRECTORY __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4OBJCACHEDIRECTORY
#endif
#endif

