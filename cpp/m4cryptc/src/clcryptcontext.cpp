// $Id: ClCryptContext.cpp,v 1.1 1997/12/02 18:44:18 pjlobo Exp pjlobo $

#include <string.h>
#include <openssl/pem.h>     // SSLeay
#include <openssl/evp.h>     // SSLeay

#define _m4crypt_interno
#include "m4cryptcobj.hpp"

ClCryptContext::ClCryptContext()
{
  ctx = new EVP_CIPHER_CTX;
  memset ( ctx , 0 , sizeof ( EVP_CIPHER_CTX ) );
}

ClCryptContext::~ClCryptContext()
{
  delete ctx;
}
