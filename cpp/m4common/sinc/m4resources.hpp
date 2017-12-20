
#include "m4version.hpp"

#define __V__(x)			#x
#define __S__(x)			__V__(x)

#ifdef _AIX
	#define	M4_RESOURCE_BUILD_PLATFORM		"AIX ripley61 1 6 00C49E8E4C00"
	#ifdef _DEBUG
		#define	M4_RESOURCE_BUILD_INFORMATION	"aix debug"
	#else
		#define	M4_RESOURCE_BUILD_INFORMATION	"aix release"
	#endif
#elif defined _SOLARIS
	#define	M4_RESOURCE_BUILD_PLATFORM		"SunOS marte 5.10 Generic_147147-26 sun4u sparc SUNW,Sun-Fire-280R"
	#ifdef _DEBUG
		#define	M4_RESOURCE_BUILD_INFORMATION	"solaris debug"
	#else
		#define	M4_RESOURCE_BUILD_INFORMATION	"solaris release"
	#endif
#elif defined _WIN64
	#ifdef _DEBUG
		#define	M4_RESOURCE_BUILD_INFORMATION	"win64 debug"
	#else
		#define	M4_RESOURCE_BUILD_INFORMATION	"win64 release"
	#endif
#else
	#ifdef _DEBUG
		#define	M4_RESOURCE_BUILD_INFORMATION	"win32 debug"
	#else
		#define	M4_RESOURCE_BUILD_INFORMATION	"win32 release"
	#endif
#endif

#define	M4_RESOURCE_PRODUCT_NAME		"PeopleNet"
#define	M4_RESOURCE_PRODUCT_VERSION		"8.1sp4"
#define	M4_RESOURCE_COMPANY_NAME		"Meta4 Spain, S.A."
#define	M4_RESOURCE_LEGAL_TRADEMARKS	"Meta4(r), Meta4Mind(r), Meta4 PeopleNet(r), Meta4 KnowNet(r), Meta4 e-mind(r), Meta4 t.innova(r), PeopleNet Ksystem(r) are trademarks of Meta4 Spain, S.A."
#define	M4_RESOURCE_LEGAL_COPYRIGHT		"(c)2017"

