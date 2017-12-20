//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             m4criptc.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:15/01/1999
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================



#include <m4win.hpp>
#include <m4cryptctasa.hpp>
#include <m4cryptcobj.hpp>
#include <stdio.h>

#define	M4CH_DUMMY		"\xC\x2\xA\x1\x9\xC\xF\x6\x1\xD\x1\x1\x4\xC\x2\x8\x2\x9\xA\x3A\x7\x1\xB\xC\xD\x4\x7\x6\x2\x1\x8\xF\xD\xA\x3\x8\x8"
#define MAX_KEY_LEN		256

m4return_t BuildKey ( m4pchar_t & ao_szKey , m4int_t & ao_iLenOut ) ;
m4return_t ASCIItoHEX ( m4pcchar_t ai_szASCII , m4int_t ai_iLen , m4pchar_t ao_szHEX , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) ;
m4return_t HEXtoASCII ( m4pcchar_t ai_szHEX , m4int_t ai_iLen , m4pchar_t ao_szASCII , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) ;


m4return_t EncryptOneWay ( m4pcchar_t src , m4int_t slen , m4pchar_t dst , m4int_t lenbufferout , m4int_t &dlen )
{
	ClMD5Digest oMD5Digest ;
	int dlen2 = 0;
	m4pchar_t aux ;
	m4uint32_t iAuxLen ;


	iAuxLen= (slen +1 + 8 ) *2 ;

	aux = new m4char_t [ iAuxLen] ;

	memcpy ( aux , src , slen +1 ) ;

	  ClMDContext* poContext = oMD5Digest.DigestStart();
  oMD5Digest.Digest(poContext, aux, slen);
  oMD5Digest.DigestFinish(poContext, aux ,  &dlen2);

  ASCIItoHEX ( aux , dlen2 , dst , lenbufferout , dlen ) ;
  
  delete [] aux ;

  return M4_SUCCESS ;
}


m4return_t SynCrypt ( m4pcchar_t ai_szText , m4int_t ai_iLenText , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szCrypt , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut , m4bool_t ai_bApplyInternalKey )
{
	m4pchar_t			aux ;
	static m4char_t		auxkey [10240];
	static m4char_t		szPreviousKey  [10240] = "" ;
	m4pchar_t			keyok;
	static int			dlenauxkey = 0;
	int					dlenkeyok , dlen ;
	m4return_t			iRet = M4_SUCCESS ;
	m4int_t			iprimkeylen = 0 ;
	m4pchar_t			szKeyprim ;

	
//	auxkey = new m4char_t [ai_iLenBufferOut];
	


	if ( ai_bApplyInternalKey )
	{
		if ( strcmp ( szPreviousKey , ai_szKey ) )
		{
			strcpy ( szPreviousKey , ai_szKey ) ;

			if ( M4_ERROR == BuildKey ( szKeyprim , iprimkeylen ) )
			{
				return M4_ERROR ;
			}

			if ( M4_ERROR == SynDeCrypt ( ai_szKey , ai_iLenKey , szKeyprim , iprimkeylen , auxkey , ai_iLenBufferOut, dlenauxkey , M4_FALSE ) )
			{
				return M4_ERROR ;
			}

		}
		keyok = auxkey ;
		dlenkeyok = dlenauxkey ;
	}
	else
	{
		keyok = (char*)ai_szKey  ;
		dlenkeyok = ai_iLenKey  ;
	}

	aux = new m4char_t [ai_iLenBufferOut];

	iRet = SymCryptASCII  ( ai_szText , ai_iLenText , keyok , dlenkeyok  , (m4pchar_t) aux , ai_iLenBufferOut , dlen ) ;

	if ( M4_SUCCESS == iRet )
	{
		if ( M4_ERROR == ASCIItoHEX ( (m4pchar_t) aux , dlen , ao_szCrypt , ai_iLenBufferOut , ao_iLenOut  ) )
		{
			iRet = M4_ERROR ;
		}
	}

	delete [] aux ;
	
  return iRet ;
}


m4return_t SynDeCrypt ( m4pcchar_t ai_szCrypt , m4int_t ai_iLenCrypt , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szText , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ,  m4bool_t ai_bApplyInternalKey )
{
	static m4char_t		auxkey [10240];
	static m4char_t		szPreviousKey [10240] = "" ;
	m4pchar_t			aux = NULL , keyok = NULL;
	int					auxlen, dlenkeyok ;
	static int			dlen = 0;
	m4return_t			iRet ;
	m4int_t				iprimkeylen ;
	m4pchar_t			szKeyprim ;

	
//	auxkey = new m4char_t [ai_iLenBufferOut];
	


	if ( ai_bApplyInternalKey )
	{
		if ( strcmp ( szPreviousKey , ai_szKey ) )
		{
			strcpy ( szPreviousKey , ai_szKey ) ;

			if ( M4_ERROR == BuildKey ( szKeyprim , iprimkeylen ) )
			{
				return M4_ERROR ;
			}

			if ( M4_ERROR == SynDeCrypt ( ai_szKey , ai_iLenKey , szKeyprim , iprimkeylen , auxkey , ai_iLenBufferOut, dlen , M4_FALSE ) )			
			{
				return M4_ERROR ;
			}
		}

		keyok = auxkey ;
		dlenkeyok = dlen ;
	}
	else
	{
		keyok = (char*)ai_szKey  ;
		dlenkeyok = ai_iLenKey  ;
	}

	auxlen = ( ai_iLenCrypt /2 )+2 ;

	aux = (m4pchar_t ) new m4uchar_t [auxlen];

	iRet = HEXtoASCII ( ai_szCrypt , ai_iLenCrypt , aux, auxlen , ai_iLenCrypt ) ;

	if ( M4_SUCCESS == iRet )
	{
		iRet = SymDeCryptASCII ( aux, ai_iLenCrypt , keyok, dlenkeyok  , ao_szText, ai_iLenBufferOut , ao_iLenOut ) ;
	}

	delete [] aux ;

	return iRet ;
}


m4return_t SymCryptASCII ( m4pcchar_t ai_szText , m4int_t ai_iLenText , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szCrypt , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) 
{
	ClKey					* poKey ;									  
	Cl3DESCryptography		* poCrypt;
	ClCryptContext			* poContext ;
	m4return_t				iRet = M4_SUCCESS ;



	poKey = new ClKey (  ai_szKey , ai_iLenKey ) ;									  

	poCrypt = new Cl3DESCryptography ( poKey ) ;

	poContext = poCrypt -> EncryptStart();

	poCrypt -> Encrypt(poContext , (void *)ai_szText , ai_iLenText , ao_szCrypt , &ao_iLenOut );

	ai_iLenText = ao_iLenOut ;

	poCrypt -> EncryptFinish(poContext, ao_szCrypt+ ai_iLenText , &ao_iLenOut );

	ao_iLenOut+= ai_iLenText ;

	delete poKey ;

	delete poCrypt ;

	if (! ao_iLenOut )
		iRet = M4_ERROR ;

	ao_szCrypt[ao_iLenOut] = 0 ;

	return iRet ;
}


m4return_t SymDeCryptASCII ( m4pcchar_t ai_szCrypt , m4int_t ai_iLenCrypt , m4pcchar_t ai_szKey , m4int_t ai_iLenKey , m4pchar_t ao_szText , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut ) 
{
	ClKey					* poKey ;									  
	Cl3DESCryptography		* poCrypt;
	ClCryptContext			* poContext ;
	m4return_t				iRet = M4_SUCCESS ;




	poKey = new ClKey ( ai_szKey , ai_iLenKey ) ;

	poCrypt = new Cl3DESCryptography ( poKey ) ;

	poContext = poCrypt -> DecryptStart();

	poCrypt -> Decrypt(poContext, (void *)ai_szCrypt , ai_iLenCrypt , (m4pchar_t ) ao_szText, &ao_iLenOut);

	ai_iLenCrypt = ao_iLenOut ;

	poCrypt -> DecryptFinish(poContext, ( m4pchar_t ) ( ao_szText + ai_iLenCrypt ), &ao_iLenOut);

	ao_iLenOut+= ai_iLenCrypt ;

	ao_szText [ ao_iLenOut ] = 0 ;

	if (! ao_iLenOut )
		iRet = M4_ERROR ;

	delete poKey ;
	delete poCrypt ;

	ao_szText [ao_iLenOut] =0 ;

	return iRet ;
}



m4return_t ASCIItoHEX ( m4pcchar_t ai_szASCII , m4int_t ai_iLen , m4pchar_t ao_szHEX , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut )
{
	m4int_t iCont ;
	m4char_t aux [10];
	m4uint8_t car ;

	if ( ai_iLenBufferOut < ( ( ai_iLen *2 )  + 1 ) )
	{
		return M4_ERROR ;
	}

	for ( iCont =0 ; iCont < ai_iLen ; iCont ++ )
	{
		car = ai_szASCII [iCont] ;

		sprintf ( aux , "%2xc" , car );

		if ( car < 16 )
			aux[0] = '0' ;

		memcpy ( ao_szHEX + (iCont * 2) , aux , 2 ) ;
	}

	ao_szHEX [ iCont * 2 ] = 0 ;

	ao_iLenOut = iCont *2 ;

	return M4_SUCCESS;
}



m4return_t HEXtoASCII ( m4pcchar_t ai_szHEX , m4int_t ai_iLen , m4pchar_t ao_szASCII , m4int_t ai_iLenBufferOut , m4int_t & ao_iLenOut )
{
	m4int32_t iCont ;
	m4char_t aux [5] = "0x";
	m4uchar_t car ;
	m4int_t icar ;


	if ( ai_iLenBufferOut < ( ai_iLen /2 ) +1 )
		return M4_ERROR ;

	ao_iLenOut = 0;

	for ( iCont = 0 ; iCont < ai_iLen ; iCont +=2 , ao_iLenOut ++ )
	{
		strncpy ( aux  , ai_szHEX + iCont , 2 ) ;

		aux[2] = 0;

		sscanf ( aux , "%x" , &icar ) ;

		car = icar ;

		ao_szASCII [ ao_iLenOut ] = car ;
	}

	ao_szASCII [ ao_iLenOut ] = 0;

	return M4_SUCCESS ;
}

m4return_t BuildKey ( m4pchar_t & ao_szKey , m4int_t & ao_iLenOut ) 
{
	m4int32_t ilen ;
	static m4int_t iLenOut  = 0 ;
	static m4pchar_t szKey ;
	static m4bool_t bKeyMade = M4_FALSE ;



	if ( ! bKeyMade )
	{


		iLenOut = strlen ( M4CH_DUMMY ) ;

		ao_iLenOut = iLenOut ;

		szKey = new m4char_t [ ao_iLenOut + 1] ;

		strcpy ( szKey , M4CH_DUMMY ) ;

		for (ilen = 0 ; ilen <= ao_iLenOut ; ilen ++ )
		{
			szKey [ ilen ] =  ( szKey [ ilen ] * 10 ) + 30 ;
		}

		szKey [ ao_iLenOut ] = 0 ;

		bKeyMade = M4_TRUE ;
	}

	ao_szKey = szKey ;

	ao_iLenOut = iLenOut ;


	return M4_SUCCESS;
}
