// $Id: Cl3DESCryptography.cpp,v 1.1 1997/12/02 18:44:17 pjlobo Exp pjlobo $

#include <openssl/pem.h>     // SSLeay
#include <openssl/evp.h>     // SSLeay
#include <openssl/des.h>

#define _m4crypt_interno
#include "m4cryptcobj.hpp"

Cl3DESCryptography::Cl3DESCryptography(ClKey* k) : ClSymCryptography(k)
{
  // La clave son 3 claves DES juntas

  des_cblock* dcb;

  dcb = (des_cblock*) k->Key();
  des_set_odd_parity(dcb);
  ++dcb;
  des_set_odd_parity(dcb);
  ++dcb;
  des_set_odd_parity(dcb);
}

ClCryptContext* Cl3DESCryptography::EncryptStart()
{
  ClCryptContext* ctx = new ClCryptContext();
  key->ResetIV();
  EVP_EncryptInit(ctx->ctx, EVP_des_ede3_cbc(), (unsigned char*) key->Key(), key->IV());
  return ctx;
}

void Cl3DESCryptography::Encrypt(ClCryptContext* ctx, void* src, int slen,
				void* dst, int* dlen)
{
  EVP_EncryptUpdate(ctx->ctx, (unsigned char*) dst, dlen,
		    (unsigned char*) src, slen);
}

void Cl3DESCryptography::EncryptFinish(ClCryptContext* ctx,
				      void* dst, int* dlen)
{
  EVP_EncryptFinal(ctx->ctx, (unsigned char*) dst, dlen);
  delete ctx;
}

ClCryptContext* Cl3DESCryptography::DecryptStart()
{
  ClCryptContext* ctx = new ClCryptContext();
  key->ResetIV();
  EVP_DecryptInit(ctx->ctx, EVP_des_ede3_cbc(), (unsigned char*) key->Key(), key->IV());
  return ctx;
}

void Cl3DESCryptography::Decrypt(ClCryptContext* ctx, void* src, int slen,
				void* dst, int* dlen)
{
  EVP_DecryptUpdate(ctx->ctx, (unsigned char*) dst, dlen,
		    (unsigned char*) src, slen);
}

void Cl3DESCryptography::DecryptFinish(ClCryptContext* ctx,
				      void* dst, int* dlen)
{
  EVP_DecryptFinal(ctx->ctx, (unsigned char*) dst, dlen);
  delete ctx;
}

int Cl3DESCryptography::KeyLen()
{
  return EVP_des_ede3_cbc()->key_len;
}

int Cl3DESCryptography::IVLen()
{
  return EVP_des_ede3_cbc()->iv_len;
}

int Cl3DESCryptography::BlockSize()
{
  return EVP_des_ede3_cbc()->block_size;
}


int	Cl3DESCryptography::MaxLenOfEnCrypted( int ai_iSize )
{
	// EVP_EncryptUpdate	size + block_size - 1
	// EVP_EncryptFinal		block_size
	// Nullteminado			+1 
	return( ai_iSize + 2 * EVP_des_ede3_cbc()->block_size ) ;
}

int	Cl3DESCryptography::MaxLenOfDeCrypted( int ai_iSize )
{
	// EVP_DecryptUpdate	size + block_size
	// EVP_DecryptFinal		block_size
	// Nullteminado			+1 
	return( ai_iSize + 2 * EVP_des_ede3_cbc()->block_size + 1 ) ;
}

