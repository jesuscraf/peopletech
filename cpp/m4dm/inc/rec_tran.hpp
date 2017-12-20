#ifndef _REC_TRAN_HPP_
#define _REC_TRAN_HPP_

//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                rec_tran.hpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                28-07-1997
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
// 11/07/97
//==============================================================================


#include "vrb.hpp"


/**
 * Esta clase representa un vector con un solo registro. No se pueden añadir ni borrar registros en él.
 */
class ClRecordWithTransactions : public ClVectorRecordBase
{
public:
	/**
	 * Initialization routine. Debe hacerse una sola vez cuando se crea el objeto. No vale pare hacer un reset.
	 *
	 * @param ai_pSMData	Puntero al EMDStaticData. Esta estructura contiene metainformación sacada del metacanal.
	 * @return m4return_t.
	 */
	m4return_t Init(const EMDStaticData* const ai_pSMData);
};

#endif
