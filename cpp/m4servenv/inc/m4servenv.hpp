
#include <m4types.hpp>
#include <navigatorf.hpp>
#include <m4servenv_dll.hpp>

#ifndef m4servenv
#define m4servenv

M4_DECL_M4SERVENV m4return_t GetEnviron (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ao_szValue, m4uint32_t ai_iBufflen) ;
M4_DECL_M4SERVENV m4return_t SetEnviron (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4pchar_t ai_szValue , m4bool_t ai_bPersistable ) ;
M4_DECL_M4SERVENV m4return_t GetEnvironDouble (m4pchar_t ai_szPath, m4pchar_t ai_szProperty, m4double_t & ao_dPropertyValue) ;
M4_DECL_M4SERVENV void SetNavigator (ClNavigatorFirewall *ai_poNavigator) ;


#endif


