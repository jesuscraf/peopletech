//==============================================================================
//
// (c) Copyright  1991-2004 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4modkey
// File:                
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                08-11-2004
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//   
//	  
//
//==============================================================================

#ifndef	_M4MODKEY_ENCRYPT_HPP_
#define _M4MODKEY_ENCRYPT_HPP_

#include "globdec.hpp"

class ClSecurizedObject;

//----------------------------------------------
//Clase ClEncryption
//----------------------------------------------
class M4_DECL_M4MODKEY ClEncryption {

private:
	static m4return_t _InitEncrypter( ClEncryption * ai_pClEncryption, m4pcchar_t ai_pKey );

private:
	m4char_t * m_pMaskKeyB;
	m4int32_t m_iSizeMaskKeyB;

	//Desencripta/Encripta el buffer con clave A
	static void _DecryptWithKeyA( m4pchar_t ai_pBufferEncryptedWithKeyA, m4int32_t ai_iSizeEncryptedWithKeyA );

protected:

	static m4return_t _CalculateCheckSum( m4pcchar_t ai_pText , m4int_t ai_iLenText, m4pchar_t & ao_pBufferCheckSum, m4int_t & ao_iSizeCheckSum );
	m4bool_t _ExistsHeader( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted );

public:
	ClEncryption( void );
	~ClEncryption( void );

	m4return_t SendKeyB( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted, m4pchar_t ao_pSignature, m4int32_t ai_iSizeSignature );

	m4return_t Encrypt( m4pcchar_t ai_pText , m4int_t ai_iLenText, m4pchar_t & ao_pBufferEncrypted, m4int_t & ao_iSizeEncrypted );
	m4return_t Decrypt( m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted, m4pchar_t & ao_pText , m4int_t & ao_iLenText );

	m4return_t EncryptSecurizedObject( const ClSecurizedObject & oClSecurizedObject, m4pcchar_t ai_pText , m4int_t ai_iLenText, m4pchar_t & ao_pBufferEncrypted, m4int_t & ao_iSizeEncrypted );
	m4return_t DecryptSecurizedObject( const ClSecurizedObject & oClSecurizedObject, m4pcchar_t ai_pBufferEncrypted, m4int32_t ai_iSizeEncrypted, m4pchar_t & ao_pText , m4int_t & ao_iLenText );
	m4bool_t IsEncryptedSecurizedObject( const ClSecurizedObject & oClSecurizedObject, m4pcchar_t ai_pBuffer, m4int32_t ai_iSize );
};
//----------------------------------------------
//FIN Clase ClEncryption
//----------------------------------------------


#endif


