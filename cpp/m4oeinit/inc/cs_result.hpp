//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                CS_Result.hpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                19/06/98
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Definitions used in the Object used to return a result from a C/S transaction
//
// 
//==============================================================================

#ifndef _CS_RESULT_HPP_
#define _CS_RESULT_HPP_

#include "m4oeinit_dll.hpp"
#include "m4types.hpp"

class ClGenericIODriver;
class ClChannelManager;

// Error codes >>>>>>>>>>>>>>>>>>>>>>>>>>
#define M4_CSR_ERROR   -1
#define M4_CSR_SUCCESS  0
#define M4_CSR_WARNING  1

#define M4_CSR_ERROR_CHANNEL_VERSION 2
#define M4_CSR_ERROR_NO_CHANNEL      3
#define M4_CSR_ERROR_NO_EXECONTEXT   4
#define M4_CSR_ERROR_EXECUTE         5
#define M4_CSR_ERROR_DESERIALIZE     6
#define M4_CSR_ERROR_CONCURRENCY     7
#define M4_CSR_EXEC_INTERRUPT		 8
// Error codes <<<<<<<<<<<<<<<<<<<<<<<<<<

const m4uint16_t CS_RESULT_MAX_MESSAGES     = 500;
const m4uint16_t CS_RESULT_MAX_MESSAGE_SIZE = 255;

class M4_DECL_M4OEINIT ClClientServerResult
{
public:
	ClClientServerResult();
	
	void Reset(void);
	
	~ClClientServerResult(void);
	
	/**
	 * Fija el código de error. El código sólo se puede fijar una vez (una vez puesto no se puede cambiar).
	 * Con esto se pretende que si una capa inferior a puesto un código de error, no lo cambie una capa superior.
	 *
	 * @param ai_iResponse Código de error.
	 */
	void SetResponseCode(const m4return_t ai_iResponse);

	/**
	 * Retorna el código de error.
	 *
	 * @return m4return_t (código de error)
	 */
	m4return_t GetResponseCode(void) const;
	
	/**
	 * Para añadir mensajes que aportan información extra sobre el código de error.
	 *
	 * @param ai_pzMessage Mensaje que se desea añadir.
	 * @return m4return_t.
	 */
	m4return_t AddMess(const m4char_t* const ai_pszMessage);
	
	/**
	 * Devuelve el puntero al mensaje. Ojo, la cadena pertenece a ClClientServerResult.
	 *
	 * @param ai_iMessageIndex Índice del mensaje que queremos obtener.
	 * @return Puntero al mensaje.
	 */
	const m4char_t* const GetMess(const m4uint16_t ai_iMessageIndex);
	
	/**
	 * Indica el número de mensajes que hay.
	 *
	 * @return Número de mensajes.
	 */
	m4uint16_t NumberOfMessages(void) const;
	
	m4return_t Serialize(ClGenericIODriver& IOD, ClChannelManager *ai_poChannelManager, m4bool_t ai_bSendStatistics);
	m4return_t DeSerialize(ClGenericIODriver& IOD, ClChannelManager *ai_poChannelManager);
	
private:
	m4return_t m_iResponse;
	m4uint16_t m_iNumberOfMessages;
	m4char_t*  m_pszMessage[CS_RESULT_MAX_MESSAGES];
};

#endif
