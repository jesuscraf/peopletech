// VersionInfo file. Automatic generation. Do not edit.

#include "m4resources.hpp"

// M4_VAR struct
typedef struct {
   char *szkey;
   char *value;
   int array_size;
} M4_VAR;

// version information in Microsoft like format
static M4_VAR m4versionInfo[] = {
#ifdef DRIVER_ODBC
   {"FileDescription", "m4dbwrap-odbc " M4_RESOURCE_BUILD_INFORMATION, 10},
#else
  #ifdef DRIVER_OCI12G
   {"FileDescription", "m4dbwrap-oci12g " M4_RESOURCE_BUILD_INFORMATION, 10},
  #else
   {"FileDescription", "m4dbwrap-oci11g " M4_RESOURCE_BUILD_INFORMATION, 10},
  #endif
#endif
   {"FileVersion", __S__(M4_RESOURCE_VERSION_ONE) "." __S__(M4_RESOURCE_VERSION_TWO) "." __S__(M4_RESOURCE_VERSION_THREE), 10},
   {"InternalName", "m4dbwrap", 10},
   {"BuildInformation", M4_RESOURCE_BUILD_INFORMATION, 10},
   {"ProductName", M4_RESOURCE_PRODUCT_NAME, 10},
   {"ProductVersion", M4_RESOURCE_PRODUCT_VERSION, 10},
   {"CompanyName", M4_RESOURCE_COMPANY_NAME, 10},
   {"LegalTrademarks", M4_RESOURCE_LEGAL_TRADEMARKS, 10},
   {"LegalCopyright", M4_RESOURCE_LEGAL_COPYRIGHT, 10},
   {"Build Platform", M4_RESOURCE_BUILD_PLATFORM, 10}
};

// pointer to M4_VAR array
void *m4versionInfo_pointer = (void *)&m4versionInfo;

// version information in sccsid format
#ifdef DRIVER_ODBC
static char *sccsid_m4dbwrap_0 = "@(#) FileDescription : m4dbwrap-odbc " M4_RESOURCE_BUILD_INFORMATION;
#else
  #ifdef DRIVER_OCI12G
static char *sccsid_m4dbwrap_0 = "@(#) FileDescription : m4dbwrap-oci12g " M4_RESOURCE_BUILD_INFORMATION;
  #else
static char *sccsid_m4dbwrap_0 = "@(#) FileDescription : m4dbwrap-oci11g " M4_RESOURCE_BUILD_INFORMATION;
  #endif
#endif
static char *sccsid_m4dbwrap_1 = "@(#) FileVersion : " __S__(M4_RESOURCE_VERSION_ONE) "." __S__(M4_RESOURCE_VERSION_TWO) "." __S__(M4_RESOURCE_VERSION_THREE);
static char *sccsid_m4dbwrap_2 = "@(#) InternalName : m4dbwrap";
static char *sccsid_m4dbwrap_3 = "@(#) BuildInformation : " M4_RESOURCE_BUILD_INFORMATION;
static char *sccsid_m4dbwrap_4 = "@(#) ProductName : " M4_RESOURCE_PRODUCT_NAME;
static char *sccsid_m4dbwrap_5 = "@(#) ProductVersion : " M4_RESOURCE_PRODUCT_VERSION;
static char *sccsid_m4dbwrap_6 = "@(#) CompanyName : " M4_RESOURCE_COMPANY_NAME;
static char *sccsid_m4dbwrap_7 = "@(#) LegalTrademarks : " M4_RESOURCE_LEGAL_TRADEMARKS;
static char *sccsid_m4dbwrap_8 = "@(#) LegalCopyright : " M4_RESOURCE_LEGAL_COPYRIGHT;
static char *sccsid_m4dbwrap_9 = "@(#) Build Platform : " M4_RESOURCE_BUILD_PLATFORM;

