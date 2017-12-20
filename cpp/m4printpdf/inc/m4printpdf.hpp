//## begin module.cm preserve=no
//## end module.cm

//## begin module.cp preserve=no
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:              m4printpdf.hpp
//	 Project:           M4DMSXML
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=========================================================
//	=====================

#ifndef printpdf_h
#define printpdf_h 1

#include "m4printpdf_dll.hpp"
#include "m4var.hpp"
#include "m4types.hpp"


// Default number of concurrent copies.
#define M4PDF_MIN_CONCURRENT_PRINTING_DOC			2
#define M4PDF_ACROBAT_RUNNING_MAX_NUM				600
#define M4PDF_ACROBAT_RUNNING_WAIT_TIME				100 // milliseconds
#define M4PDF_ACROBAT_MAX_VERSION_SUPPORTED			9

#ifdef _DEBUG
#	define	PDF_DEBUG(ai_statement) ai_statement
#else
#	define	PDF_DEBUG(ai_statement)
#endif



extern "C" M4_DECL_M4PRINTPDF m4return_t M4Download					(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4PRINTPDF m4return_t PrintPDF					(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4PRINTPDF m4return_t PrintPDFWithoutConcurrency	(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
extern "C" M4_DECL_M4PRINTPDF m4return_t GetAcrobatVersion			(m4VariantType *ai_pvArg, m4uint32_t ai_iLongArg, m4VariantType &ai_pvReturn, m4pvoid_t ai_pvContext);
#endif
