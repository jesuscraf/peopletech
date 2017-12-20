// $Id: ClMD5Digest.cpp,v 1.1 1997/12/02 18:44:18 pjlobo Exp pjlobo $

#include <openssl/pem.h>     // SSLeay
#include <openssl/evp.h>     // SSLeay

#define _m4crypt_interno
#include "m4cryptcobj.hpp"

ClMDContext* ClMD5Digest::DigestStart()
{
  ClMDContext* ctx = new ClMDContext();

  EVP_DigestInit(ctx->ctx, EVP_md5());
  return ctx;
}

void ClMD5Digest::Digest(ClMDContext* ctx, void* src, int slen)
{
  EVP_DigestUpdate(ctx->ctx, (unsigned char *) src, (unsigned int) slen);
}

void ClMD5Digest::DigestFinish(ClMDContext* ctx, void* dst, int* dlen)
{
  EVP_DigestFinal(ctx->ctx, (unsigned char *) dst, (unsigned int *) dlen);
  delete ctx;
}

int ClMD5Digest::DigestSize()
{
  return EVP_md5()->md_size;
}
