
#ifndef M4OBJLOG_DLL_HPP
#define M4OBJLOG_DLL_HPP

#ifdef M4_USING_DLL
	#ifdef M4_O_M4OBJLOG
			#define M4_DECL_M4OBJLOG __declspec(dllexport)
	#else
			#define M4_DECL_M4OBJLOG __declspec(dllimport)
	#endif
#else
			#define M4_DECL_M4OBJLOG
#endif
#endif

