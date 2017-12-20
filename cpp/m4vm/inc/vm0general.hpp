//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:                ExeFurep.h
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            21 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    Declaración de la clase que implementa las funciones generales de librería
////
//==============================================================================



#ifndef _ClVM0GENERAL_H
#define _ClVM0GENERAL_H

#include "vvm.hpp"
#include "vmtypes.hpp"
#include "vmdefine.hpp"
#include "m4clock.hpp"


class ClVM0General : public ClVVM{

	const m4VMState_t &m_oState;
public:

	ClVM0General (const m4VMState_t &ai_oState):m_oState(ai_oState){ 
	
	}

	m4return_t  Round           (void);
    m4return_t  CommonTime      (void);
    m4return_t  CommonTimestamp (void);
    m4return_t  DayOfTheWeek    (void);
    m4return_t  Months          (void);
    m4return_t  Years           (void);
    m4return_t  Max             (void);
    m4return_t  Min             (void);
    m4return_t  Abs             (void);
    m4return_t  Date30          (void);
    m4return_t  Date30Ex        (void);
    m4return_t  AddMonths       (void);
    m4return_t  AddYears        (void);
    m4return_t  Fraction        (void);
    m4return_t  Bound           (void);
    m4return_t  DaysOfYear      (void);
    m4return_t  DaysOfMonth     (void);
    m4return_t  Power           (void);
    m4return_t  Today           (void);
    m4return_t  TodayNow        (void);
    m4return_t  TodayGMT        (void);
    m4return_t  TodayNowGMT     (void);
    m4return_t  Percentage      (void);
    m4return_t  ToInt           (void);
    m4return_t  ToDouble        (void);
    m4return_t  ToDate          (void);
    m4return_t  ToString        (void);
    m4return_t  Truncate        (void);
    m4return_t  DateToNumbers   (void);
    m4return_t  NumbersToDate   (void);
    m4return_t  DateToAllNumbers(void);
    m4return_t  Div             (void);
	m4return_t  Log				(void);

    m4return_t  Sign(void);
    
    m4return_t  Convert				(void);
    m4return_t  DataConvert			(void);
    m4return_t  KeyConvert			(void);
    m4return_t  ConvertOneWay		(void);
    m4return_t  ConvertPassword		(void);
    m4return_t  ComparePassword		(void);
    m4return_t  Compare2FAPassword	(void);

    m4return_t  GetServerValue	(void);
    m4return_t  GetRegistryValue(void);

	//bitwise operations
	m4return_t  BitwiseOr		(void);
	m4return_t  BitwiseAnd		(void);
	m4return_t  BitwiseNot		(void);
	m4return_t  BitwiseXor		(void);
    
	//Variant Type
	m4return_t  GetVariantType	(void);

	//GetUniqueId
	m4return_t	GetUniqueID		(void);

	//random numbers
	m4return_t  Rnd(void);

	//diferencias/sumas de unidades a fechas
	m4return_t  DateAdd			(void);
	m4return_t  DateDiff		(void);

	//TickCount y sleep
	m4return_t  TickCount		(void);
	m4return_t  vmSleep			(void);

private:

	static	m4millisec_t	m_lBaseTickCount;

    m4return_t  _CalcCommonTime	(m4double_t &ao_res);
	m4return_t	_ComparePassword(m4bool_t ai_bIs2FA, m4pchar_t ai_pccFunctionName);

};

#endif
