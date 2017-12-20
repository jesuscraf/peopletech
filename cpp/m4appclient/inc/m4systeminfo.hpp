//	=============================================================================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:               m4systeminfo.hpp
//	 Project:            m4oeinit
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================

#ifndef __M4SYSTEMINFO_HPP__
#define __M4SYSTEMINFO_HPP__

#include "m4appclient_dll.hpp"

// m4types
#include <m4types.hpp>
// stl 
#include "m4stl.hpp"

//=================================================================================
//
// ClSystemInfo
//
// Clase que obtiene información del sistema (sistema operativo, navegador, etc).
//
//=================================================================================
// Newer system metrics values.
#ifndef SM_TABLETPC
#define SM_TABLETPC 86
#endif
#ifndef SM_MEDIACENTER
#define SM_MEDIACENTER 87
#endif
#ifndef SM_STARTER
#define SM_STARTER 88
#endif
#ifndef SM_SERVERR2
#define SM_SERVERR2 89
#endif


// Newer product types.
#ifndef PRODUCT_ULTIMATE
#define PRODUCT_ULTIMATE                            0x00000001
#endif
#ifndef PRODUCT_HOME_BASIC
#define PRODUCT_HOME_BASIC                          0x00000002
#endif
#ifndef PRODUCT_HOME_PREMIUM
#define PRODUCT_HOME_PREMIUM                        0x00000003
#endif
#ifndef PRODUCT_ENTERPRISE
#define PRODUCT_ENTERPRISE                          0x00000004
#endif
#ifndef PRODUCT_HOME_BASIC_N
#define PRODUCT_HOME_BASIC_N                        0x00000005
#endif
#ifndef PRODUCT_BUSINESS
#define PRODUCT_BUSINESS                            0x00000006
#endif
#ifndef PRODUCT_STANDARD_SERVER
#define PRODUCT_STANDARD_SERVER                     0x00000007
#endif
#ifndef PRODUCT_DATACENTER_SERVER
#define PRODUCT_DATACENTER_SERVER                   0x00000008
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER
#define PRODUCT_SMALLBUSINESS_SERVER                0x00000009
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER
#define PRODUCT_ENTERPRISE_SERVER                   0x0000000A
#endif
#ifndef PRODUCT_STARTER
#define PRODUCT_STARTER                             0x0000000B
#endif
#ifndef PRODUCT_DATACENTER_SERVER_CORE
#define PRODUCT_DATACENTER_SERVER_CORE              0x0000000C
#endif
#ifndef PRODUCT_STANDARD_SERVER_CORE
#define PRODUCT_STANDARD_SERVER_CORE                0x0000000D
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_CORE
#define PRODUCT_ENTERPRISE_SERVER_CORE              0x0000000E
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_IA64
#define PRODUCT_ENTERPRISE_SERVER_IA64              0x0000000F
#endif
#ifndef PRODUCT_BUSINESS_N
#define PRODUCT_BUSINESS_N                          0x00000010
#endif
#ifndef PRODUCT_WEB_SERVER
#define PRODUCT_WEB_SERVER                          0x00000011
#endif
#ifndef PRODUCT_CLUSTER_SERVER
#define PRODUCT_CLUSTER_SERVER                      0x00000012
#endif
#ifndef PRODUCT_HOME_SERVER
#define PRODUCT_HOME_SERVER                         0x00000013
#endif
#ifndef PRODUCT_STORAGE_EXPRESS_SERVER
#define PRODUCT_STORAGE_EXPRESS_SERVER              0x00000014
#endif
#ifndef PRODUCT_STORAGE_STANDARD_SERVER
#define PRODUCT_STORAGE_STANDARD_SERVER             0x00000015
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER
#define PRODUCT_STORAGE_WORKGROUP_SERVER            0x00000016
#endif
#ifndef PRODUCT_STORAGE_ENTERPRISE_SERVER
#define PRODUCT_STORAGE_ENTERPRISE_SERVER           0x00000017
#endif
#ifndef PRODUCT_SERVER_FOR_SMALLBUSINESS
#define PRODUCT_SERVER_FOR_SMALLBUSINESS            0x00000018
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_PREMIUM
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM        0x00000019
#endif
#ifndef PRODUCT_HOME_PREMIUM_N
#define PRODUCT_HOME_PREMIUM_N                      0x0000001A
#endif
#ifndef PRODUCT_ENTERPRISE_N
#define PRODUCT_ENTERPRISE_N                        0x0000001B
#endif
#ifndef PRODUCT_ULTIMATE_N
#define PRODUCT_ULTIMATE_N                          0x0000001C
#endif
#ifndef PRODUCT_WEB_SERVER_CORE
#define PRODUCT_WEB_SERVER_CORE                     0x0000001D
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT
#define PRODUCT_MEDIUMBUSINESS_SERVER_MANAGEMENT    0x0000001E
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY
#define PRODUCT_MEDIUMBUSINESS_SERVER_SECURITY      0x0000001F
#endif
#ifndef PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING
#define PRODUCT_MEDIUMBUSINESS_SERVER_MESSAGING     0x00000020
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_PRIME
#define PRODUCT_SMALLBUSINESS_SERVER_PRIME          0x00000021
#endif
#ifndef PRODUCT_HOME_PREMIUM_SERVER
#define PRODUCT_HOME_PREMIUM_SERVER                 0x00000022
#endif
#ifndef PRODUCT_SERVER_FOR_SMALLBUSINESS_V
#define PRODUCT_SERVER_FOR_SMALLBUSINESS_V          0x00000023
#endif
#ifndef PRODUCT_STANDARD_SERVER_V
#define PRODUCT_STANDARD_SERVER_V                   0x00000024
#endif
#ifndef PRODUCT_DATACENTER_SERVER_V
#define PRODUCT_DATACENTER_SERVER_V                 0x00000025
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_V
#define PRODUCT_ENTERPRISE_SERVER_V                 0x00000026
#endif
#ifndef PRODUCT_DATACENTER_SERVER_CORE_V
#define PRODUCT_DATACENTER_SERVER_CORE_V            0x00000027
#endif
#ifndef PRODUCT_STANDARD_SERVER_CORE_V
#define PRODUCT_STANDARD_SERVER_CORE_V              0x00000028
#endif
#ifndef PRODUCT_ENTERPRISE_SERVER_CORE_V
#define PRODUCT_ENTERPRISE_SERVER_CORE_V            0x00000029
#endif
#ifndef PRODUCT_HYPERV
#define PRODUCT_HYPERV                              0x0000002A
#endif
#ifndef PRODUCT_STORAGE_EXPRESS_SERVER_CORE
#define PRODUCT_STORAGE_EXPRESS_SERVER_CORE         0x0000002B
#endif
#ifndef PRODUCT_STORAGE_STANDARD_SERVER_CORE
#define PRODUCT_STORAGE_STANDARD_SERVER_CORE        0x0000002C
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_SERVER_CORE
#define PRODUCT_STORAGE_WORKGROUP_SERVER_CORE       0x0000002D
#endif
#ifndef PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE
#define PRODUCT_STORAGE_ENTERPRISE_SERVER_CORE      0x0000002E
#endif
#ifndef PRODUCT_PROFESSIONAL
#define PRODUCT_PROFESSIONAL                        0x00000030
#endif
#ifndef PRODUCT_PROFESSIONAL_N
#define PRODUCT_PROFESSIONAL_N                      0x00000031
#endif
#ifndef PRODUCT_SB_SOLUTION_SERVER
#define PRODUCT_SB_SOLUTION_SERVER                  0x00000032
#endif
#ifndef PRODUCT_SERVER_FOR_SB_SOLUTIONS
#define PRODUCT_SERVER_FOR_SB_SOLUTIONS             0x00000033
#endif
#ifndef PRODUCT_STANDARD_SERVER_SOLUTIONS
#define PRODUCT_STANDARD_SERVER_SOLUTIONS           0x00000034
#endif
#ifndef PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE
#define PRODUCT_STANDARD_SERVER_SOLUTIONS_CORE      0x00000035
#endif
#ifndef PRODUCT_SB_SOLUTION_SERVER_EM
#define PRODUCT_SB_SOLUTION_SERVER_EM               0x00000036
#endif
#ifndef PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM
#define PRODUCT_SERVER_FOR_SB_SOLUTIONS_EM          0x00000037
#endif
#ifndef PRODUCT_SOLUTION_EMBEDDEDSERVER
#define PRODUCT_SOLUTION_EMBEDDEDSERVER             0x00000038
#endif
#ifndef PRODUCT_SOLUTION_EMBEDDEDSERVER_CORE
#define PRODUCT_SOLUTION_EMBEDDEDSERVER_CORE        0x00000039
#endif
#ifndef PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE
#define PRODUCT_SMALLBUSINESS_SERVER_PREMIUM_CORE   0x0000003F
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT
#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMT       0x0000003B
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL
#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL       0x0000003C
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC
#define PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC    0x0000003D
#endif
#ifndef PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC
#define PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC    0x0000003E
#endif
#ifndef PRODUCT_CLUSTER_SERVER_V
#define PRODUCT_CLUSTER_SERVER_V                    0x00000040
#endif
#ifndef PRODUCT_EMBEDDED
#define PRODUCT_EMBEDDED                            0x00000041
#endif
#ifndef PRODUCT_STARTER_E
#define PRODUCT_STARTER_E                           0x00000042
#endif
#ifndef PRODUCT_HOME_BASIC_E
#define PRODUCT_HOME_BASIC_E                        0x00000043
#endif
#ifndef PRODUCT_HOME_PREMIUM_E
#define PRODUCT_HOME_PREMIUM_E                      0x00000044
#endif
#ifndef PRODUCT_PROFESSIONAL_E
#define PRODUCT_PROFESSIONAL_E                      0x00000045
#endif
#ifndef PRODUCT_ENTERPRISE_E
#define PRODUCT_ENTERPRISE_E                        0x00000046
#endif
#ifndef PRODUCT_ULTIMATE_E
#define PRODUCT_ULTIMATE_E                          0x00000047
#endif
#ifndef PRODUCT_ENTERPRISE_EVALUATION
#define PRODUCT_ENTERPRISE_EVALUATION               0x00000048
#endif
#ifndef PRODUCT_PRERELEASE
#define PRODUCT_PRERELEASE                          0x0000004A
#endif
#ifndef PRODUCT_MULTIPOINT_STANDARD_SERVER
#define PRODUCT_MULTIPOINT_STANDARD_SERVER          0x0000004C
#endif
#ifndef PRODUCT_MULTIPOINT_PREMIUM_SERVER
#define PRODUCT_MULTIPOINT_PREMIUM_SERVER           0x0000004D
#endif
#ifndef PRODUCT_STANDARD_EVALUATION_SERVER
#define PRODUCT_STANDARD_EVALUATION_SERVER          0x0000004F
#endif
#ifndef PRODUCT_DATACENTER_EVALUATION_SERVER
#define PRODUCT_DATACENTER_EVALUATION_SERVER        0x00000050
#endif
#ifndef PRODUCT_PRERELEASE_ARM
#define PRODUCT_PRERELEASE_ARM                      0x00000051
#endif
#ifndef PRODUCT_PRERELEASE_N
#define PRODUCT_PRERELEASE_N                        0x00000052
#endif
#ifndef PRODUCT_ENTERPRISE_N_EVALUATION
#define PRODUCT_ENTERPRISE_N_EVALUATION             0x00000054
#endif
#ifndef PRODUCT_EMBEDDED_AUTOMOTIVE
#define PRODUCT_EMBEDDED_AUTOMOTIVE					0x00000055
#endif
#ifndef PRODUCT_EMBEDDED_INDUSTRY_A
#define PRODUCT_EMBEDDED_INDUSTRY_A					0x00000056
#endif
#ifndef PRODUCT_THINPC								
#define PRODUCT_THINPC								0x00000057
#endif
#ifndef PRODUCT_EMBEDDED_A
#define PRODUCT_EMBEDDED_A							0x00000058
#endif
#ifndef PRODUCT_EMBEDDED_INDUSTRY
#define PRODUCT_EMBEDDED_INDUSTRY					0x00000059
#endif
#ifndef PRODUCT_EMBEDDED_E
#define PRODUCT_EMBEDDED_E                          0x0000005A
#endif
#ifndef PRODUCT_EMBEDDED_INDUSTRY_E
#define PRODUCT_EMBEDDED_INDUSTRY_E                 0x0000005B
#endif
#ifndef PRODUCT_EMBEDDED_INDUSTRY_A_E
#define PRODUCT_EMBEDDED_INDUSTRY_A_E               0x0000005C
#endif
#ifndef PRODUCT_STORAGE_WORKGROUP_EVALUATION_SERVER
#define PRODUCT_STORAGE_STANDARD_EVALUATION_SERVER  0x00000060
#endif
#ifndef PRODUCT_CORE_ARM                            
#define PRODUCT_CORE_ARM                            0x00000061
#endif
#ifndef PRODUCT_CORE_N                              
#define PRODUCT_CORE_N                              0x00000062
#endif
#ifndef PRODUCT_CORE_COUNTRYSPECIFIC               
#define PRODUCT_CORE_COUNTRYSPECIFIC                0x00000063
#endif
#ifndef PRODUCT_CORE_SINGLELANGUAGE                 
#define PRODUCT_CORE_SINGLELANGUAGE                 0x00000064
#endif
#ifndef PRODUCT_CORE                                
#define PRODUCT_CORE                                0x00000065
#endif
#ifndef PRODUCT_PROFESSIONAL_WMC                    
#define PRODUCT_PROFESSIONAL_WMC                    0x00000067
#endif

// Newer suite types.
#ifndef VER_SUITE_WH_SERVER
#define VER_SUITE_WH_SERVER                         0x00008000
#endif
#ifndef VER_SUITE_COMPUTE_SERVER
#define VER_SUITE_COMPUTE_SERVER					0x00004000
#endif
#ifndef VER_SUITE_STORAGE_SERVER
#define VER_SUITE_STORAGE_SERVER					0x00002000
#endif
#ifndef VER_SUITE_BLADE
#define VER_SUITE_BLADE								0x00000400
#endif
#ifndef VER_SUITE_PERSONAL
#define VER_SUITE_PERSONAL							0x00000200
#endif


class M4_DECL_M4APPCLIENT ClSystemInfo 
{
  public:

	ClSystemInfo();
	~ClSystemInfo();
	  
	m4return_t GetOperatingSystem(string & ao_zOperatingSystem);
	m4return_t GetProcessorInfo(string & ao_sProcessorInfo);
	m4return_t GetMemory(m4uint64_t & ao_iMemory);
	m4return_t GetUserLanguage(string & ao_sLanguage);
	m4return_t GetNETFramework(string & ao_sNETFramework);
	m4return_t GetDiskSpaceInfo(m4pchar_t ai_szDriveName, m4uint64_t & ao_iTotalBytes, m4uint64_t & ao_iFreeBytes);
	m4return_t GetScreenResolution(m4uint32_t & ao_iWidth, m4uint32_t & ao_iHeight);
	m4return_t GetColorDepth(m4uint32_t & ao_iColorDepth);
	m4return_t Serialize(string & ao_sSystemInfo);
};


#endif
