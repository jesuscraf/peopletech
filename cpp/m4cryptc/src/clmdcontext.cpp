// $Id: ClMDContext.cpp,v 1.1 1997/12/02 18:44:18 pjlobo Exp pjlobo $

#include <openssl/pem.h>     // SSLeay
#include <openssl/evp.h>     // SSLeay

#define _m4crypt_interno
#include "m4cryptcobj.hpp"

ClMDContext::ClMDContext()
{
  ctx = new EVP_MD_CTX;
}

ClMDContext::~ClMDContext()
{
  delete ctx;
}
