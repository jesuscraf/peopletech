// $Id: m4crypt.hpp,v 1.1 1997/12/02 19:08:31 pjlobo Exp pjlobo $

#ifndef _m4cryptcobj_hpp
#define _m4cryptcobj_hpp

#ifndef _m4crypt_interno
class EVP_CIPHER_CTX;
class DH;
class BIGNUM;
class RSA;
class EVP_MD_CTX;
#endif

//
// Clases de criptografía simétrica
//
#include <m4types.hpp>


class ClKey {

private:

  m4char_t key [48]; // Suficiente para claves de 384 bits (3IDEA, p.e.)
  m4uint32_t keylen;
  unsigned char iv[16];  // Todos los algoritmos de bloques usan bloques
                         // de 64 bits, pero por si acaso se deja el
                         // doble de espacio
  int ivlen;

public:

  ClKey(m4pcchar_t k, int kl);
  ~ClKey();

  m4pcchar_t Key() const { return key; };
  int Keylen() {return keylen; } ;
  unsigned char* IV() { return iv; };
  void SetIV(unsigned char* iv, int ivl);
  void ResetIV();

};


class  ClCryptContext {

  friend class ClSymCryptography;
  friend class ClNullSymCryptography;
  friend class ClDESCryptography;
  friend class ClRC4Cryptography;
  friend class Cl3DESCryptography;
  friend class ClIDEACryptography;

private:

  EVP_CIPHER_CTX* ctx;

public:

  ClCryptContext();
  ~ClCryptContext();

};



class  ClSymCryptography {

protected:

  ClKey* key;

public:

  ClSymCryptography(ClKey* k) { key = k; };

  virtual ClCryptContext* EncryptStart() = 0;
  virtual void Encrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen) = 0;
  virtual void EncryptFinish(ClCryptContext* ctx, void* dst, int* dlen) = 0;

  virtual ClCryptContext* DecryptStart() = 0;
  virtual void Decrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen) = 0;
  virtual void DecryptFinish(ClCryptContext* ctx, void* dst, int* dlen) = 0;

  virtual int KeyLen() = 0;
  virtual int IVLen() = 0;
  virtual int BlockSize() = 0;

  static const unsigned int DES_CBC;
  static const unsigned int DES3_CBC;
  static const unsigned int IDEA_CBC;
  static const unsigned int RC4;

};

/*
class M4_DECL_CRYPT ClNullSymCryptography : public ClSymCryptography {

public:

  ClNullSymCryptography(ClKey* k);

  ClCryptContext* EncryptStart();
  void Encrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void EncryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  ClCryptContext* DecryptStart();
  void Decrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void DecryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  int KeyLen();
  int IVLen();
  int BlockSize();

};

class M4_DECL_CRYPT ClDESCryptography : public ClSymCryptography {

public:

  ClDESCryptography(ClKey* k);

  ClCryptContext* EncryptStart();
  void Encrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void EncryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  ClCryptContext* DecryptStart();
  void Decrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void DecryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  int KeyLen();
  int IVLen();
  int BlockSize();

};
*/
class  Cl3DESCryptography : public ClSymCryptography {

public:

  Cl3DESCryptography(ClKey* k);

  ClCryptContext* EncryptStart();
  void Encrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void EncryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  ClCryptContext* DecryptStart();
  void Decrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void DecryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  int KeyLen();
  int IVLen();
  int BlockSize();

  static	int	MaxLenOfEnCrypted( int ai_iSize ) ;
  static	int	MaxLenOfDeCrypted( int ai_iSize ) ;

};
/*
class M4_DECL_CRYPT ClRC4Cryptography : public ClSymCryptography {

public:

  ClRC4Cryptography(ClKey* k);

  ClCryptContext* EncryptStart();
  void Encrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void EncryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  ClCryptContext* DecryptStart();
  void Decrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void DecryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  int KeyLen();
  int IVLen();
  int BlockSize();

};

class M4_DECL_CRYPT ClIDEACryptography : public ClSymCryptography {

public:

  ClIDEACryptography(ClKey* k);

  ClCryptContext* EncryptStart();
  void Encrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void EncryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  ClCryptContext* DecryptStart();
  void Decrypt(ClCryptContext* ctx, void* src, int slen,
		       void* dst, int* dlen);
  void DecryptFinish(ClCryptContext* ctx, void* dst, int* dlen);

  int KeyLen();
  int IVLen();
  int BlockSize();

};

//
// Clases para el intercambio de claves simétricas mediante el
// algoritmo Diffie-Hellman
//

class M4_DECL_CRYPT ClDHKey {

public:

  // Constructor para la parte que lleva la iniciativa en la negociacion
  ClDHKey(int keylen = 128);
  // Constructor para la parte que no lleva la iniciativa
  ClDHKey(const void* p, int lp);

  ~ClDHKey();

  // La longitud de los parametros que se devuelven es la longitud en
  // bytes de la clave mas 8
  void GetInitialParameters(void* p, int* lp);
  void GetPublicInt(void* pi, int* lpi);
  void SetRemoteInt(const void* ri, int lri);

  ClKey* GetKey();

private:

  DH* dhLocal;
  BIGNUM* bnRemote;
};

//
// Clases de hash
//
*/
class  ClMDContext {

  friend class ClAsymCryptography;
  friend class ClNullAsymCryptography;
  friend class ClRSACryptography;

  friend class ClDigest;
  friend class ClNullDigest;
  friend class ClMD5Digest;
  friend class ClSHA1Digest;

protected:

  EVP_MD_CTX* ctx;

public:

  ClMDContext();
  ~ClMDContext();

};

class  ClDigest {

public:

  virtual ClMDContext* DigestStart() = 0;
  virtual void Digest(ClMDContext* ctx, void* src, int slen) = 0;
  virtual void DigestFinish(ClMDContext* ctx, void* dst, int* dlen) = 0;

  virtual int DigestSize() = 0;

};
/*
class M4_DECL_CRYPT ClNullDigest : public ClDigest {

public:

  ClMDContext* DigestStart();
  void Digest(ClMDContext* ctx, void* src, int slen);
  void DigestFinish(ClMDContext* ctx, void* dst, int* dlen);

  int DigestSize();

};
*/
class  ClMD5Digest : public ClDigest {

public:

  ClMDContext* DigestStart();
  void Digest(ClMDContext* ctx, void* src, int slen);
  void DigestFinish(ClMDContext* ctx, void* dst, int* dlen);

  int DigestSize();

};
/*
class M4_DECL_CRYPT ClSHA1Digest : public ClDigest {

public:

  ClMDContext* DigestStart();
  void Digest(ClMDContext* ctx, void* src, int slen);
  void DigestFinish(ClMDContext* ctx, void* dst, int* dlen);

  int DigestSize();

};

//
// Clases de criptografía de clave pública
//

class M4_DECL_CRYPT ClAsymCryptography {

public:

  virtual void PublicEncrypt(void* src, int slen, void* dst, int* dlen) = 0;
  virtual void PublicDecrypt(void* src, int slen, void* dst, int* dlen) = 0;
  virtual void PrivateEncrypt(void* src, int slen, void* dst, int* dlen) = 0;
  virtual void PrivateDecrypt(void* src, int slen, void* dst, int* dlen) = 0;

  virtual ClMDContext* SignStart(ClDigest* dg);
  virtual void Sign(ClDigest* dg, ClMDContext* ctx, void* src, int slen);
  virtual void SignFinish(ClDigest* dg, ClMDContext* ctx,
			  void* dst, int* dlen) = 0;

  virtual ClMDContext* VerifyStart(ClDigest* dg);
  virtual void Verify(ClDigest* dg, ClMDContext* ctx, void* src, int slen);
  virtual int VerifyFinish(ClDigest* dg, ClMDContext* ctx,
			   void* signature, int slen) = 0;

};

class M4_DECL_CRYPT ClRSACryptography : public ClAsymCryptography {

private:

  RSA* key;

public:

  ClRSACryptography(int l = 1024);  // Este constructor genera una nueva clave
  ClRSACryptography(unsigned char* public_key, int pub_len,
		    unsigned char* private_key = NULL, int priv_len = 0);
  ~ClRSACryptography();

  void PublicEncrypt(void* src, int slen, void* dst, int* dlen);
  void PublicDecrypt(void* src, int slen, void* dst, int* dlen);
  void PrivateEncrypt(void* src, int slen, void* dst, int* dlen);
  void PrivateDecrypt(void* src, int slen, void* dst, int* dlen);

  void SignFinish(ClDigest* dg, ClMDContext* ctx, void* dst, int* dlen);
  // Este metodo devuelve 0 si falla la comprobacion de firma y 1 si la
  // firma es correcta
  int VerifyFinish(ClDigest* dg, ClMDContext* ctx,
		   void* signature, int slen);

  void ExtractPublicKey(unsigned char* dst, int* dlen);
  void ExtractPrivateKey(unsigned char* dst, int* dlen);

};



class M4_DECL_CRYPT ClNullAsymCryptography : public ClAsymCryptography {

private:

  RSA* key;

public:

  ClNullAsymCryptography(int len = 1024);
  ClNullAsymCryptography(unsigned char* public_key, int pub_len,
			 unsigned char* private_key = NULL, int priv_len = 0);
  ~ClNullAsymCryptography();

  void PublicEncrypt(void* src, int slen, void* dst, int* dlen);
  void PublicDecrypt(void* src, int slen, void* dst, int* dlen);
  void PrivateEncrypt(void* src, int slen, void* dst, int* dlen);
  void PrivateDecrypt(void* src, int slen, void* dst, int* dlen);

  void SignFinish(ClDigest* dg, ClMDContext* ctx, void* dst, int* dlen);
  // Este metodo devuelve 0 si falla la comprobacion de firma y 1 si la
  // firma es correcta
  int VerifyFinish(ClDigest* dg, ClMDContext* ctx,
		   void* signature, int slen);

  void ExtractPublicKey(unsigned char* dst, int* dlen);
  void ExtractPrivateKey(unsigned char* dst, int* dlen);

};
*/
#endif // _m4crypt_hpp
