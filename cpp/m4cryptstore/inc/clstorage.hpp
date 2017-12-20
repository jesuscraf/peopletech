// $Id: ClStorage.hpp,v 1.1 1998/7/13 pablop $

#ifndef _clstorage_hpp
#define _clstorage_hpp

#include <stdio.h>
#include <string.h>
#include "m4types.hpp"
#include "m4cryptstore_dll.hpp"



// Recubrimiento de clases para acceso a datos: ficheros, base de datos, ...

enum Mode { R, W, R_W, R_A };
enum origin_seek { _SEEK_CUR, _SEEK_END, _SEEK_SET };

class M4_DECL_M4CRYPTSTORE ClStorage {

public:
	ClStorage () { sprintf ( szStoreName , "%s", "" ); 
					szStoreDesc = NULL; 
					}
	virtual m4return_t Open( m4pchar_t ai_szName,
							 Mode ai_mode = R_W )
	{
		return M4_SUCCESS;
	}
	virtual m4return_t Close()
	{
		return M4_SUCCESS;
	}
	virtual m4return_t Read( m4int_t ai_sizeBuf, m4pchar_t ao_pBuf, 
							 m4int_t &ao_sizeBuf )
	{
		return M4_SUCCESS;
	}
	virtual m4return_t Write( m4pchar_t ai_pBuf , m4int_t ai_sizeBuf, 
							  m4int_t &ao_sizeBuf )
	{
		return M4_SUCCESS;
	}
	
	virtual	m4return_t Seek( m4int_t ai_offset, 
				 origin_seek ai_origin = _SEEK_CUR )
	{
		return M4_SUCCESS;
	}
	

protected:
	m4char_t szStoreName[1024];		
	m4pvoid_t szStoreDesc;

};


class M4_DECL_M4CRYPTSTORE ClFileStorageCrypt : public ClStorage {

public:
	m4return_t Open( m4pchar_t ai_szName, Mode ai_mode = R_W );
	m4return_t Close();
	m4return_t Read( m4int_t ai_sizeBuf, m4pchar_t ao_pBuf, 
					 m4int_t &ao_sizeBuf );
	m4return_t Write( m4pchar_t ai_pBuf , m4int_t ai_sizeBuf, 
					  m4int_t &ao_sizeBuf );
	m4return_t Seek( m4int_t ai_offset, origin_seek ai_origin = _SEEK_CUR );
	~ClFileStorageCrypt () { };
};



#endif // _clstorage_hpp
