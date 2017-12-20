//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             formattp.h
// Project:
// Author:           Meta Software M.S. , S.A
// Date:20/10/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//
//
//==============================================================================

#ifndef __FORMATTP__H__
#define __FORMATTP__H__

enum FORMAT_DATA_TYPE { FORMAT_STRING, FORMAT_DATE, FORMAT_DOUBLE, FORMAT_TIME_STAMP=8};

//ENUM para la declaracion de interes de ficheros
enum DECLARE_EXTERNAL_FILE { NOT_DECLARED, TEMPORARY, REFERENCE, COPY, REFERENCE_AND_COPY };

#endif

