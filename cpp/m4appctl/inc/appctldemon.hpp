#ifndef _APPCTLDEMON_H_
#define _APPCTLDEMON_H_

#include "m4thread.hpp"
#include "tsap.hpp"
#include "m4string.hpp"                


m4int8_t Daemon_main (m4pchar_t ai_pszConfigFilePath);
m4pchar_t GetAppCtlConfigPath() ;

#endif
