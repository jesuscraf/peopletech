// external calls for m4csdll library

#include "m4csdll_dll.hpp"


#define M4_CS_EVENT_SERVER_SHUTDOWN				1
#define M4_CS_EVENT_SERVER_RUPTURE_CONNECTION	2
#define M4_CS_EVENT_SESSION_TIMEOUT				3
#define M4_CS_EVENT_ADMIN_MESSAGE				4

long M4_DECL_M4CSDLL TimeoutDisplay(const char *ai_pcMessage, const long ai_iCurrent);

long M4_DECL_M4CSDLL ServerEventDisplay(const long ai_lServerEvent, const void * ai_pParam );

