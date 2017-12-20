

#include <m4types.hpp>
#include "m4stl.hpp"

typedef map < m4pchar_t , m4uint32_t , less <m4pchar_t > > mServersMap ;

typedef mServersMap::iterator iServersIterator ;


class ClServers	
{
public:

	ClServers	() ;

	~ClServers	() ;

	m4uint32_t nServers ( ) ;

	m4return_t GetNextServer ( m4pchar_t & ao_szServerHost , m4uint32_t & ao_iPort ) ;

	m4return_t GetFirstServer ( m4pchar_t & ao_szServerHost , m4uint32_t & ao_iPort ) ;

	m4return_t AddServer ( const m4pchar_t ai_szServerHost , const m4uint32_t ai_iPort ) ;

private :

	m4return_t CheckIterator () ;

	mServersMap m_oServersMap ;

	iServersIterator m_iIterator ;

} ;

