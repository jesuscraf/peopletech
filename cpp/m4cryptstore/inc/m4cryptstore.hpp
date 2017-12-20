// $Id: m4cryptstore.hpp,v 1.1 1998/7/02 pablop $

#ifndef _m4cryptstore_hpp
#define _m4cryptstore_hpp

#include <m4types.hpp>
#include <string.h>
#include <m4cryptc.hpp>
#include <m4cryptcobj.hpp>
#include <clstorage.hpp>


#define CRYPT_PROP_VALLUE_MAX_LEN	2048

// Clase para parametrizar el algoritmo de criptografía utilizado en cada caso.
/*
enum SymCrypto { _NULLS, _DES, _3DES, _RC4, _IDEA };
enum AsymCrypto { _NULLA, _RSA };
enum Digest { _NULLD, _MD5, _SHA1 };

class M4_DECL_M4CRYPTSTORE ClCipherSuite {

public:
	ClCipherSuite( SymCrypto s = _DES, AsymCrypto a = _RSA, Digest d = _MD5 );
	SymCrypto getSym() { return sym; };
	m4return_t setSym ( SymCrypto s ) { sym = s; return M4_SUCCESS; };
	AsymCrypto getAsym() { return asym; };
	m4return_t setAsym ( AsymCrypto a ) { asym = a; return M4_SUCCESS; };
	Digest getDig() { return dig; };
	m4return_t setDig ( Digest d ) { dig = d; return M4_SUCCESS; };

private:
	SymCrypto sym;
	AsymCrypto asym;
	Digest dig;
};



// Clases para facilitar la gestion de datos.

class M4_DECL_M4CRYPTSTORE ClPing {

public:
	m4pchar_t getPingName() { return PingName; };
	m4return_t setPingName( m4pchar_t ai_name )
	{ 
		sprintf ( PingName , "%s", ai_name ); 
		return M4_SUCCESS; 
	};
	m4int16_t getlPing() { return lPing; };
	m4return_t setlPing( m4int16_t l ) { lPing = l; return M4_SUCCESS; };

private: 
	m4char_t PingName[1024];
	m4int16_t lPing;
};

*/


// Clase para la construccion del Storage cifrado.

class M4_DECL_M4CRYPTSTORE ClCryptStore {

public:

	ClCryptStore ( m4pcchar_t ai_szFile , m4pcchar_t ai_szKey , m4uint32_t ai_lKey ) 
		:m_okey ( ai_szKey , ai_lKey )
		{	m_storage = new ClFileStorageCrypt();
			strcpy( m_szStoreName, ai_szFile );
		};
	~ClCryptStore () { delete m_storage; };
	m4return_t CreateStore ( );
	m4return_t InsertData ( m4pchar_t ai_var, m4int_t ai_lvar,
				m4pchar_t ai_valor, m4int_t ai_lvalor );

	m4return_t ExtractData ( m4pchar_t ai_var, m4int_t ai_lvar,
				 m4pchar_t ao_valor, m4uint32_t ai_iBufferLen , m4int_t &ao_lvalor );

	m4return_t ChangePing ( ClKey key, ClKey newkey);
	m4return_t ConsistencyControl ( m4bool_t &estado);
	
private:
	ClKey m_okey ;
	ClStorage *m_storage;
	m4char_t m_szStoreName[1024];
	// Cada operacion sobre el storage es independiente (en cada una se
	// abre y cierra el recurso). Esta variable es para no tener que 
	// pasar el nombre del fichero continuamente si se quiere trabajar 
	// siempre con el mismo.
	// No debe haber problemas de scope con la variable de mismo nombre
	// en ClStorage.
	m4return_t CheckHeader ( m4bool_t &estado);
	m4return_t SeekData ( m4int_t ai_offset, ClStorage *ai_store,
				m4pchar_t ao_var, m4int_t &ao_lvar,
				m4pchar_t ao_valor, m4int_t &ao_lvalor,
				m4pchar_t ao_csum, m4int_t &ao_lcsum );

};



#endif // _m4cryptstore_hpp
