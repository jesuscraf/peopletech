// $Id: ClKey.cpp,v 1.1 1997/12/02 18:44:18 pjlobo Exp pjlobo $

#include <string.h>

#include <openssl/pem.h>     // SSLeay
#include <openssl/evp.h>     // SSLeay

#define _m4crypt_interno
#include "m4cryptcobj.hpp"

// Este constructor usa los datos que se le pasan directamente
// como clave
ClKey::ClKey(m4pcchar_t k, int kl)
{
	memset(key , 0 , 48);
	memcpy(key, k, kl);
  key[kl] = 0 ;
  keylen = kl;
}

ClKey::~ClKey()
{
  // El destructor sobreescribe la clave (por seguridad)
	memset (key, 0, 48) ;
}

void ClKey::SetIV(unsigned char* iv2, int ivl2)
{
  memcpy(iv, iv2, ivl2);
  ivlen = ivl2;
}

void ClKey::ResetIV()
{
  memset(iv, 0, sizeof(iv));
}
