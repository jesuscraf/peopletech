//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              serial 
// File:                exem4xdm.cpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                02-10-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    This module is the executor of OLTP in the server
//
//
//==============================================================================

// TODO:
/*
- métodos con argumentos
- generación de log
- datadef
- data
- traducciones
- filtros
- persistencia (serialización)
- contexto a nivel de ejecución
*/

#include "m4stl.hpp"
// VM
#include "m4define.hpp"
#include "m4types.hpp"
#include "dm.hpp"
#include "chan_knl.hpp"
#include "item.hpp"
//#include "cllconn.hpp"
#include "cldatstoiod.hpp"
#include "executor.hpp"
#include "csres.hpp"
#include "m4trace.hpp"
#include "exvariables.hpp"

#include "exechannel.hpp"
#include "m4thread.hpp"

#include "objdir.hpp"
#include "execntxt.hpp"
#include "cs_result.hpp"

#include "m4srvres.hpp"
#include "m4srvefcres.hpp"

#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <clexecutorstatemanager.hpp>

#include "srvm4xmlsax.hpp"



// ----------------------------------------------------------------------

// --------------------------------------------------------
// main XML routine
m4return_t ExecM4XML(ClCMInitParameters			&aio_oCMInitParameters,
					 SAXParser*					ai_poParser,
					 M4DataStorage*				ai_poInputDataStorage,
					 M4DataStorage*				ao_poOutputDataStorage,
					 ExecDumpInfo *				aio_poDumpInfo,
					 ClM4XMLInfo*				ai_poInfo)
{
	m4return_t iResult = M4_SUCCESS;;
#ifdef _DEBUG
	cerr << "M4XML EXECUTOR" << endl;
#else
	m4TraceLevel(2,cerr << "M4XML+" << endl);
#endif
	
	// Initial step
	if (aio_poDumpInfo != NULL)
	{
		aio_poDumpInfo->SetCurrentStage("Preparing to Execute");
	}

	// instances the parser
	SAXParser *poParser = ai_poParser;
	
	// checks the input lenght
	if (ai_poInputDataStorage->Blocks() < 1)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_INPUT_TRUNC );
#ifdef _DEBUG
		cerr << endl << "M4XML: No input data" << endl;
#endif
		return M4_ERROR;
	}
	
	// gets the buffer
	ClDataUnitInterface *poUnit = 0;
	if (ai_poInputDataStorage->GetNext(poUnit) != M4_SUCCESS)
	{
		DUMP_CHLOG_ERROR ( M4_ERR_XML_PDU_SCAN );
#ifdef _DEBUG
		cerr << endl << "M4XML: Can´t get data unit" << endl;
#endif
		return M4_ERROR;
	}	
	m4pchar_t poBuffer = poUnit->GetBuffer();
	m4uint32_t iBufferLen = poUnit->GetSize();

	// instances the input buffer
    MemBufInputSource oMemBuffer ((const XMLByte *) poBuffer, iBufferLen, "generic", false);

	// prepares the output stream
	ClM4XMLStream oOutput;
	oOutput.SetBuffer(ao_poOutputDataStorage);

	// instances the object handler
    M4XMLSAXHandlers  oHandler(aio_oCMInitParameters,oOutput,ai_poInfo);
	ClM4XMLErrorHandler oErrorHandler;
    poParser->setDocumentHandler(&oHandler);
    poParser->setErrorHandler(&oErrorHandler);

	// parses the buffer
 	ai_poInfo->SetFileInfo("StartParser");
    try
    {
		aio_oCMInitParameters.SetParseStatus(M4_TRUE);

		// checks global values
		oHandler.SetParsingPhase(M4XMLSAXHandlers::PARSING_PHASE_PRE);
        poParser->parse(oMemBuffer);

		// pre parses to check security and others
		oHandler.SetParsingPhase(M4XMLSAXHandlers::PARSING_PHASE_1);
        poParser->parse(oMemBuffer);

		// checks pre parsing status
		if (aio_oCMInitParameters.GetParseStatus())
		{
			// normal parse
			oHandler.SetParsingPhase(M4XMLSAXHandlers::PARSING_PHASE_2);
			poParser->parse(oMemBuffer);
		}
    }

    catch (const XMLException& toCatch)
    {
		DUMP_CHLOG_ERROR ( M4_ERR_XML_PARSE_ERROR );
#ifdef _DEBUG
        cerr    << "\nError during parsing memory: '" << endl
                << "Exception message is:  \n"
                << toCatch.getMessage() << "\n" << endl;
#endif
        iResult = M4_ERROR;
    }

	ai_poInfo->SetFileInfo("EndParser");

	// checks parser status
	if (oErrorHandler.IsOk() == M4_FALSE)
		iResult = M4_ERROR; // error reportado internamente
	if (oHandler.IsOk() == M4_FALSE)
		iResult = oHandler.GetError(); // error reportado internamente


#if _M4_DEBUG > 10
	// Archivos relacionados: "srvm4xmlsax.hpp", "srvm4xmlsax.cpp", "exem4xmlsax.cpp"
	M4_ASSERT(oHandler.bEndContextStart == M4_TRUE);
	M4_ASSERT(oHandler.bEndContextEnd == M4_TRUE);
#endif


#ifdef _DEBUG
	cerr << endl << "End of M4XML remote execution" << endl;
#else
	m4TraceLevel(2,cerr << "M4XML-" << endl);
#endif
	
	return iResult;
}
