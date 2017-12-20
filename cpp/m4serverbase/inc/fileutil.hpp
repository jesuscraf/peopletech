
#ifndef FILEUTIL_HPP
#define FILEUTIL_HPP


#include <m4types.hpp>
#include <m4serverbase_dll.hpp>



void M4_DECL_M4SERVERBASE SetFileDescriptorsLimit( m4uint32_t ai_iLimit ) ;

m4return_t M4_DECL_M4SERVERBASE GetSizeFile ( m4pchar_t ai_szFile , m4uint32_t & ao_iFileLen ) ;

#endif

