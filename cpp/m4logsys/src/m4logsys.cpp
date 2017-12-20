//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         M4LogSys
// File:           m4logsys.cpp
// Project:        MVC
// Author:         Meta Software M.S. , S.A
// Date:           19 jan 99
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Error Format
////
//==============================================================================

#include "m4types.hpp" 
#include "m4stl.hpp" 
#include "m4var.hpp"		//M4Variant
#include "medvarin.hpp"		//M4Variant

#include "m4logsys.hpp"
#include "m4lang.hpp"




///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////                   /////////////////////////
/////////////////////////// Private Functions /////////////////////////
///////////////////////////                   /////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


enum ParseFormatParameter_option {
	parse_all,		// Parse all the string
	parse_date		// Parse only searching for a date in the begining of the string
};

// Parse a parameter using the locale settings
void ParseFormatParameter(
	m4VariantType& aio_varParameter,
	ParseFormatParameter_option ai_option,
	m4language_t ai_uiLang);

inline void ParseFormatDate(
	const m4char_t* ai_year, 
	const m4char_t* ai_month, 
	const m4char_t* ai_day, 
	const m4char_t* ai_hour, 
	const m4char_t* ai_minute, 
	const m4char_t* ai_second, 
	const m4char_t* ai_pszFormat,
	string& aio_strOutput)
{
	m4bool_t bLeadingZeros;
	const m4char_t* pszParse;
	const m4char_t* pszDoParse;
	const m4char_t* pszCopy;

	pszParse=ai_pszFormat;
	while (M4_TRUE) {
		
		pszDoParse=strchr(pszParse,'%'); 
		if (pszDoParse==NULL) {	// No more % chars
			aio_strOutput += pszParse;
			break;
		}

		aio_strOutput.append(pszParse, pszDoParse-pszParse);
		pszParse=pszDoParse;

		pszDoParse++; // Skip % char

		if (*pszDoParse=='%') { // Want a %.
			aio_strOutput+= '%';
			pszParse=pszDoParse+1; // Skip second %.
			continue;
		}

		// If the next token is a #, remove leading zeros.
		if (*pszDoParse=='#') {
			pszDoParse++;
			bLeadingZeros=M4_TRUE;
		} else {
			bLeadingZeros=M4_FALSE;
		}

		switch(*pszDoParse) {
		case 'd':
			pszCopy=ai_day;
			break;
		case 'm':
			pszCopy=ai_month;
			break;
		case 'Y':
			pszCopy=ai_year;
			break;
		case 'y':
			pszCopy=ai_year+2;
			break;
		case 'H':
			pszCopy=ai_hour;
			break;
		case 'M':
			pszCopy=ai_minute;
			break;
		case 'S':
			pszCopy=ai_second;
			break;

		default:	// Unknown option
			pszCopy=NULL;	// Invalid option
			aio_strOutput.append(pszParse, pszDoParse-pszParse);
			break;
		}

		if (pszCopy) {	// If it is a valid identifier.
			if (bLeadingZeros==M4_TRUE)
				while (pszCopy[0]=='0' && pszCopy[1]!='\0') // While it is a 0 (but in the last position)
					pszCopy++;
			aio_strOutput += pszCopy;
			pszDoParse++; // Skip option
		}

		pszParse=pszDoParse;
	}
}


inline m4bool_t LogSysIsDigit(char a) 
{	return ( a>='0' && a<='9' ) ? M4_TRUE : M4_FALSE;};




///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////                   /////////////////////////
///////////////////////////   API Functions   /////////////////////////
///////////////////////////                   /////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////




/*************************************************************************

NAME: FormatErrorCode
	
*************************************************************************/
m4return_t FormatErrorCode (
	m4uint32_t ai_ulCode,
	m4pchar_t ai_pszParams,
	m4uchar_t ai_byIdParamPos,
	m4pchar_t ao_pszOutString,
	m4uint32_t ai_uBufferSize,
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang
	)
{

	m4pchar_t pszParams;
	if (ai_pszParams)
		pszParams=ai_pszParams;
	else
		pszParams="";

	m4char_t szErrorString[LOGSYS_MAX_ERROR_STRING_LEN];

	if (ao_uBufferFilled)
		*ao_uBufferFilled = 0;

	if ( LogsysGetCodeString(ai_ulCode,szErrorString,LOGSYS_MAX_ERROR_STRING_LEN, NULL, ai_uiLang) == M4_ERROR) 
		return M4_ERROR;
	
	return FormatErrorString(szErrorString,
									pszParams,
									ai_byIdParamPos,
									ao_pszOutString,
									ai_uBufferSize,
									ao_uBufferFilled,
									ai_uiLang);
}



/*************************************************************************

NAME: FormatErrorString

*************************************************************************/
m4return_t FormatErrorString (
	m4pchar_t ai_pszErrorString,
	m4pchar_t ai_pszParams,			
	m4uchar_t ai_byIdParamPos,		
	m4pchar_t ao_pszOutString,		
	m4uint32_t ai_uBufferSize,		
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang)
{

	m4pchar_t pszErrorString;
	m4pchar_t pszOut;
	if (ai_pszErrorString)
		pszErrorString=ai_pszErrorString;
	else
		pszErrorString="";

	m4pchar_t pszParams;
	if (ai_pszParams)
		pszParams=ai_pszParams;
	else
		pszParams="";


	m4VariantType vDeserializedParam[LOGSYS_MAX_PARAM_LEN];
	m4uint32_t uError;
	m4uint32_t uExtra;
	m4uint32_t uNumParams;
	m4uint32_t cOutputFilled;

	m4uint32_t cOutputPos=0;
	m4uint32_t uPresentError=LOGSYS_INVALID_M4UINT32;
	m4return_t res=M4_SUCCESS;

// Initialization ---------------------

	if (ao_uBufferFilled) 
		*ao_uBufferFilled = 0;

	if (ao_pszOutString==NULL) 
		ai_uBufferSize = LOGSYS_INFINITE_BUFFER_LEN;
	else {
		if (ai_uBufferSize==0)
			return M4_ERROR;
	}

	while (pszParams!=NULL) {
		res=DeserializeParams(&pszParams, ai_byIdParamPos, vDeserializedParam, 
								LOGSYS_MAX_PARAM_LEN, &uNumParams,
								NULL, &uError, &uExtra, LOGSYS_PARSE_ERROR);

		if (res!=M4_SUCCESS) {
			// Mal rollo... podemos haber parado de parsear a la mitad porque tenemos 
			// una longitud máxima de parametros LOGSYS_MAX_PARAM_LEN
			// así que el bucle este no mola...
			return M4_ERROR;
		}

		if (uExtra!=LOGSYS_INVALID_M4UINT32) // Reach extra parameters
			break;

		// If we want to change the error code
		if (uError!=LOGSYS_INVALID_M4UINT32)
			uPresentError=uError;

		if (cOutputPos>0) {	// Concat strings with new line.
			if (ao_pszOutString!=NULL) {
				if (cOutputPos<ai_uBufferSize+1) {
					ao_pszOutString[cOutputPos]='\n';	
				} else {
					if (ao_uBufferFilled) 
						*ao_uBufferFilled = cOutputPos+1;

					if (ao_pszOutString) 
						ao_pszOutString[cOutputPos] = '\0';
					return M4_WARNING;
				}
			}
			cOutputPos++;
		}

		if (ao_pszOutString==NULL)
			pszOut=NULL;
		else
			pszOut=ao_pszOutString+cOutputPos;

		if (uPresentError!=LOGSYS_INVALID_M4UINT32)
			res=FormatErrorCodeByParams (uPresentError,vDeserializedParam,
													uNumParams,
													ai_byIdParamPos,
													pszOut,
													ai_uBufferSize-cOutputPos,
													&cOutputFilled,
													ai_uiLang);
		else
			res=FormatErrorStringByParams (pszErrorString,vDeserializedParam,
													uNumParams,
													ai_byIdParamPos,
													pszOut,
													ai_uBufferSize-cOutputPos,
													&cOutputFilled,
													ai_uiLang);

		if (res==M4_ERROR) {
			if (ao_uBufferFilled) 
				*ao_uBufferFilled = 0;
			return M4_ERROR;
		}

		cOutputPos+=cOutputFilled-1;

		M4_ASSERT(cOutputPos<ai_uBufferSize);

		if (res==M4_WARNING) // If no room
			break;
	}

	if (ao_uBufferFilled) 
		*ao_uBufferFilled = cOutputPos+1;

	if (ao_pszOutString) 
		ao_pszOutString[cOutputPos] = '\0';

	return res;
}



/*************************************************************************

NAME: FormatErrorCodeByParams
	
*************************************************************************/
m4return_t FormatErrorCodeByParams (
	m4uint32_t ai_ulCode,			
	m4VariantType *ai_pvParams,
	m4uint32_t ai_uParams,
	m4uchar_t ai_byIdParamPos,		
	m4pchar_t ao_pszOutString,		
	m4uint32_t ai_uBufferSize,		
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang)
{

	m4char_t szErrorString[LOGSYS_MAX_ERROR_STRING_LEN];

	if (ao_uBufferFilled)
		*ao_uBufferFilled = 0;

	if ( LogsysGetCodeString(ai_ulCode,szErrorString,LOGSYS_MAX_ERROR_STRING_LEN, NULL, ai_uiLang) == M4_ERROR) 
		return M4_ERROR;
	
	return FormatErrorStringByParams(szErrorString,
												ai_pvParams,
												ai_uParams,
												ai_byIdParamPos,
												ao_pszOutString,
												ai_uBufferSize,
												ao_uBufferFilled,
												ai_uiLang);
}



/*************************************************************************

NAME: FormatErrorStringByParams

*************************************************************************/
m4return_t FormatErrorStringByParams (
	m4pchar_t ai_pszErrorString,  
	m4VariantType *ai_pvParams,
	m4uint32_t ai_uParams,
	m4uchar_t ai_byIdParamPos,		
	m4pchar_t ao_pszOutString,		
	m4uint32_t ai_uBufferSize,		
	m4puint32_t ao_uBufferFilled,
	m4language_t ai_uiLang)	
{

	m4pchar_t pszErrorString;
	
	if (ai_pszErrorString)
		pszErrorString=ai_pszErrorString;
	else
		pszErrorString="";


	// m4bool_t bIsParamUsed[LOGSYS_MAX_PARAM_LEN];
	enum FormatErrorStringByParams_state { 
		NO_PARSE , BEGIN_PARSE, PARSE_NUM_PARAM, PARSE_TYPE_PARAM,EXIT_PARSE, NEW_LINE };

	m4return_t res;
	m4uint32_t uPresentParam;
	ClVariantConversor varConv;
	m4VariantType varType;
	m4char_t chParse;
	//m4uint32_t cCont;
	m4uint32_t cOutputPos=0;
	m4uint32_t cInputPos=0;
	m4uint32_t cBeginParsePos=0;
	FormatErrorStringByParams_state state=NO_PARSE;
	m4pchar_t pCopy=NULL;
	m4uint32_t uCopy=0;
	m4pchar_t pszPosBegin, pszPosEnd, pszPosFirstEnd;
	m4uchar_t cPos;

// Initialization ---------------------

	if (ao_uBufferFilled) 
		*ao_uBufferFilled = 0;

	if (ao_pszOutString==NULL) 
		ai_uBufferSize = LOGSYS_INFINITE_BUFFER_LEN;
	else {
		if (ai_uBufferSize==0)
			return M4_ERROR;
	}

	res = M4_SUCCESS;

	if (ai_uParams>LOGSYS_MAX_PARAM_LEN) 
		ai_uParams=LOGSYS_MAX_PARAM_LEN;
	
	//for (cCont=0; cCont<ai_uParams; cCont++)
	//	bIsParamUsed[cCont]=M4_FALSE;
		
// Parse ------------------------------

	while (!(state==EXIT_PARSE)) {
		chParse=pszErrorString[cInputPos];

		switch(state) {
		case NO_PARSE:
			if (chParse=='%') {
				state=BEGIN_PARSE;
				uCopy=cInputPos-cBeginParsePos;
				pCopy = pszErrorString+cBeginParsePos;
				cBeginParsePos=cInputPos;
			} else if (chParse=='\\' && pszErrorString[cInputPos+1]=='n') {
				state=NEW_LINE;
				pszErrorString[cInputPos]='\n'; // Replace '\\' for '\n'.
				uCopy=cInputPos-cBeginParsePos+1;
				pCopy = pszErrorString+cBeginParsePos;
			} else if (chParse=='\0') {
				state=EXIT_PARSE;
				uCopy=cInputPos-cBeginParsePos;
				pCopy = pszErrorString+cBeginParsePos;
			}
			break;

		case NEW_LINE:
			pszErrorString[cInputPos-1]='\\'; // Replace '\n' for '\\'.
			cBeginParsePos=cInputPos+1; // Jump 'n'
			state=NO_PARSE;
			break;

		case BEGIN_PARSE:
			if (isdigit(chParse)) {
				state=PARSE_NUM_PARAM;
				uPresentParam=chParse-'0';

			} else if (chParse=='\0') { // Bad param number
				state=EXIT_PARSE;
				uCopy=cInputPos-cBeginParsePos;
				pCopy = pszErrorString+cBeginParsePos;
			} else if (!isspace(chParse)) {
				state=NO_PARSE;
			}
			break;

		case PARSE_NUM_PARAM:
			if (isdigit(chParse)) {
				uPresentParam=uPresentParam*10+(chParse-'0');

			} else if (chParse==':') {
				state=PARSE_TYPE_PARAM;

			} else if (chParse=='\0') { // Bad param number
				state=EXIT_PARSE;
				uCopy=cInputPos-cBeginParsePos;
				pCopy = pszErrorString+cBeginParsePos;
			} else if (!isspace(chParse)) {
				state=NO_PARSE;
			}
			break;

		case PARSE_TYPE_PARAM:
			if (chParse=='\0') { // Bad param number
				state=EXIT_PARSE;
				uCopy=cInputPos-cBeginParsePos;
				pCopy = pszErrorString+cBeginParsePos;
			} else if (chParse=='s' || chParse=='n' || chParse=='d' || chParse=='f') {
				if (uPresentParam<ai_uParams) {
					varType=ai_pvParams[uPresentParam];
					switch(chParse) {
					case 's':
					case 'n':		// Parse && for id or name
						varConv.ToCharString(varType);
						pszPosBegin=varType.Data.PointerChar;
						pszPosEnd=pszPosBegin+strlen(pszPosBegin);
						pszPosFirstEnd=pszPosEnd;
						pCopy=pszPosBegin;

						cPos=0;
						while(cPos<=ai_byIdParamPos) {
							pCopy=strchr(pCopy,'&');
							if (pCopy==NULL) {
								break;
							}
							pCopy++;
							if (*pCopy=='&') {
								if (cPos==ai_byIdParamPos) {
									pszPosEnd=pCopy-1;
									break;
								}
								if (cPos==0)
									pszPosFirstEnd=pCopy-1;
								pCopy++;
								if (cPos==ai_byIdParamPos-1)
									pszPosBegin=pCopy;
								cPos++;
							}
						}

						if (cPos<ai_byIdParamPos) {	// pos not found
							pszPosBegin=varType.Data.PointerChar;
							pszPosEnd=pszPosFirstEnd;
						}

						pCopy=pszPosBegin;
						uCopy=(m4uint32_t)(pszPosEnd-pszPosBegin);
						
						break;

					case 'd':
						varConv.ToCharString(varType);
						ParseFormatParameter(varType, parse_date, ai_uiLang);
						pCopy=varType.Data.PointerChar;
						uCopy = strlen(pCopy);
						break;

					case 'f':
						varConv.ToCharString(varType);
						ParseFormatParameter(varType, parse_all, ai_uiLang);
						pCopy=varType.Data.PointerChar;
						uCopy = strlen(pCopy);
					break;

					}
					//bIsParamUsed[uPresentParam]=M4_TRUE;
				} 

				cBeginParsePos=cInputPos+1; // Finish param process
				state=NO_PARSE;

			} else if (!isspace(chParse)) {
				state=NO_PARSE;
			}
			break;


		default:
			M4_ASSERT(0);	// Error condition: Invalid state

		}

		if (uCopy) { // If need to copy
			if (!(ai_uBufferSize>cOutputPos+uCopy)) { // No room
				uCopy=ai_uBufferSize-cOutputPos-1; // To keep (ai_uBufferSize > cOutputPos)
				res = M4_WARNING;			
				state=EXIT_PARSE;
			}

			if (ao_pszOutString)
			{
				memcpy(ao_pszOutString+cOutputPos,pCopy,uCopy);

				/* Bug 0102154
				Se cambian los 3 por '#' por si se habían cambiado previamente
				*/
				m4uint32_t	i = 0 ;
				m4uint32_t	iStart = 0 ;
				m4uint32_t	iEnd = 0 ;

				iStart = cOutputPos ;
				iEnd = iStart + uCopy ;

				for( i = iStart ; i < iEnd ; i++ )
				{
					if( ao_pszOutString[ i ] == 3 )
					{
						ao_pszOutString[ i ] = '#' ;
					}
				}
			}

			cOutputPos+=uCopy;

			pCopy=NULL;
			uCopy=0;
		}

		cInputPos++; // Next token
		M4_ASSERT(ai_uBufferSize > cOutputPos); // Always true
	}

/* ---> Remove this compatibility 
	// For backward compatibility, dump unused arguments
	for (cCont=0; cCont<ai_uParams; cCont++)
		if (bIsParamUsed[cCont]==M4_FALSE) {

			varType=ai_pvParams[cCont];
			varConv.ToCharString(varType);
			pCopy=varType.Data.PointerChar;
			uCopy = strlen(pCopy);

			if (uCopy==0) // If param is empty.
				continue;

			if (ai_uBufferSize>cOutputPos+1+uCopy) {
				if (ao_pszOutString) {	// Write overrun param
					ao_pszOutString[cOutputPos]='\n';
					memcpy(ao_pszOutString+cOutputPos+1, pCopy, uCopy);
				}
				cOutputPos+=uCopy+1;
			} else
				break; // Not enough memory
		}
*/

	if (ao_uBufferFilled) 
		*ao_uBufferFilled = cOutputPos+1;

	if (ao_pszOutString) 
		ao_pszOutString[cOutputPos] = '\0';

	return res;
}



/*************************************************************************

NAME: SerializeParams

*************************************************************************/
m4return_t SerializeParams(	
	m4VariantType *ai_pvParams,	
	m4uint32_t ai_uNumParams,		
	m4uint32_t ai_uSource,	
	m4pchar_t ao_pszOutString,
	m4uint32_t ai_uBufferSize,
	m4puint32_t ao_puBufferFilled)	
{

	m4uint32_t cCont;
	m4uint32_t cOutputPos;
	m4uint32_t uParamLen;
	m4pchar_t pszParam;
	ClVariantConversor varConv;
	m4VariantType varType;

	char szSource[30]; // To save *S_* for the source
//	char szTempSource[30]; // To save code for the source

// Initialization ---------------------

	if (ao_puBufferFilled) 
		*ao_puBufferFilled = 0;
	
	if (ao_pszOutString==NULL) 
		ai_uBufferSize = LOGSYS_INFINITE_BUFFER_LEN;
	else {
		if (ai_uBufferSize==0)
			return M4_ERROR;
		ao_pszOutString[0] = '\0';

		if (ao_puBufferFilled) 
			*ao_puBufferFilled = 1;

		if (LOGSYS_PARAM_SEPARATOR_END_LEN > ai_uBufferSize)  
			return M4_WARNING;
		ai_uBufferSize -= LOGSYS_PARAM_SEPARATOR_END_LEN;
	}

// Begining separator -----------------

	if (LOGSYS_PARAM_SEPARATOR_BEGIN_LEN > ai_uBufferSize) 
		return M4_WARNING;

	if (ao_pszOutString) // Output first separator
		memcpy(ao_pszOutString, 
					LOGSYS_PARAM_SEPARATOR_BEGIN, 
					LOGSYS_PARAM_SEPARATOR_BEGIN_LEN);

	cOutputPos = LOGSYS_PARAM_SEPARATOR_BEGIN_LEN; // Skip first separator

// BackupSource -----------------------

	//itoa(ai_uSource, szTempSource, 10);
	//sprintf(szSource,"*S%s*",szTempSource);
	sprintf(szSource,"*S%d*",(int)ai_uSource);

	uParamLen = strlen(szSource);

	if (cOutputPos+uParamLen>ai_uBufferSize) {
		memcpy(ao_pszOutString+cOutputPos, 
					LOGSYS_PARAM_SEPARATOR_END, 
					LOGSYS_PARAM_SEPARATOR_END_LEN);
		if (ao_puBufferFilled)
			*ao_puBufferFilled = cOutputPos+LOGSYS_PARAM_SEPARATOR_END_LEN;
		return M4_WARNING;
	}

	if (ao_pszOutString) // Output backupsource
		memcpy(ao_pszOutString+cOutputPos, szSource, uParamLen);

	cOutputPos += uParamLen; // Skip backupsource

// Params -----------------------------

	for (cCont=0; cCont<ai_uNumParams; cCont++) {

		switch (ai_pvParams[cCont].Type) {
			case M4CL_CPP_TYPE_STRING_VAR:
				pszParam = ai_pvParams[cCont].Data.PointerChar;
				break;
									   
			case M4CL_CPP_TYPE_NUMBER:
			case M4CL_CPP_TYPE_DATE:	
				varType = ai_pvParams[cCont];
				varConv.ToCharString(varType);
				pszParam = varType.Data.PointerChar;
				break;
											      
			case M4CL_CPP_TYPE_NULL:
				pszParam = LOGSYS_NULL_STRING;
				break; 
	
			default: // Severe error condition (invalid argument)
				if (ao_pszOutString) 
					ao_pszOutString[0] = '\0';
				if (ao_puBufferFilled)
					*ao_puBufferFilled = 0;
				return M4_ERROR;		
				break;
		}

		uParamLen = strlen(pszParam);

		if (cOutputPos+uParamLen+LOGSYS_PARAM_SEPARATOR_INTER_LEN>ai_uBufferSize) {
			memcpy(ao_pszOutString+cOutputPos, 
						LOGSYS_PARAM_SEPARATOR_END, 
						LOGSYS_PARAM_SEPARATOR_END_LEN);
			if (ao_puBufferFilled)
				*ao_puBufferFilled = cOutputPos+LOGSYS_PARAM_SEPARATOR_END_LEN;
			return M4_WARNING;
		}

		if (ao_pszOutString) { // Output separator
			memcpy(ao_pszOutString+cOutputPos, LOGSYS_PARAM_SEPARATOR_INTER, LOGSYS_PARAM_SEPARATOR_INTER_LEN);
			memcpy(ao_pszOutString+cOutputPos+LOGSYS_PARAM_SEPARATOR_INTER_LEN, pszParam, uParamLen);

			/* Bug 0102154
			Se cambian los '#' por 3 porque sino se confunde con el separador
			A la hora de visualizar o volcar habrá que deshacer el cambio
			*/
			m4uint32_t	i = 0 ;
			m4uint32_t	iStart = 0 ;
			m4uint32_t	iEnd = 0 ;

			iStart = cOutputPos + LOGSYS_PARAM_SEPARATOR_INTER_LEN ;
			iEnd = iStart + uParamLen ;

			for( i = iStart ; i < iEnd ; i++ )
			{
				if( ao_pszOutString[ i ] == '#' )
				{
					ao_pszOutString[ i ] = 3 ;
				}
			}
		}

		cOutputPos += uParamLen+LOGSYS_PARAM_SEPARATOR_INTER_LEN; // Skip param and separator
	}


// Ending separator -------------------

	if (ao_pszOutString) 
		memcpy(ao_pszOutString+cOutputPos, 
					LOGSYS_PARAM_SEPARATOR_END, 
					LOGSYS_PARAM_SEPARATOR_END_LEN);

	if (ao_puBufferFilled)
		*ao_puBufferFilled = cOutputPos+LOGSYS_PARAM_SEPARATOR_END_LEN;

	return M4_SUCCESS;
}



/*************************************************************************

NAME: DeserializeParams

*************************************************************************/
m4return_t DeserializeParams(
	m4pchar_t *aio_ppszInString,		
	m4uchar_t ai_byIdParamPos,		
	m4VariantType *ao_pvParams,	
	m4uint32_t ai_uNumParams,		
	m4puint32_t ao_puParamsFilled,
	m4puint32_t ao_puSource,
	m4puint32_t ao_puError,
	m4puint32_t ao_puExtra,
	m4uchar_t ai_byParseOption)
{

	enum DeserializeParams_state { NO_PARSE , BEGIN_PARSE, NEXT_PARSE, BEGIN_OPT };

	m4uint32_t cArg=0;
	m4char_t chParse;

	m4pchar_t pszBeginParse=NULL;
	m4pchar_t pszInString=*aio_ppszInString;
	DeserializeParams_state state=NO_PARSE;

	m4bool_t bOpt=M4_FALSE;
	m4bool_t bStillParsing=M4_TRUE;
	m4bool_t bNextOpt=M4_FALSE;
	m4uint32_t uOpt;
	m4uint32_t uParamPos=0;

	if (ai_byParseOption!=LOGSYS_PARSE_ERROR &&
			ai_byParseOption!=LOGSYS_PARSE_EXTRA &&
			ai_byParseOption!=LOGSYS_PARSE_NEVER )
		ai_byParseOption=LOGSYS_PARSE_ALWAYS;

	if (ao_puParamsFilled)
		*ao_puParamsFilled=0;

	if (ao_puSource)
		*ao_puSource=LOGSYS_INVALID_M4UINT32;

	if (ao_puError)
		*ao_puError=LOGSYS_INVALID_M4UINT32;

	if (ao_puExtra)
		*ao_puExtra=LOGSYS_INVALID_M4UINT32;

	if (ao_pvParams==NULL)
		ai_uNumParams=LOGSYS_INFINITE_BUFFER_LEN;

	if (ai_byParseOption==LOGSYS_PARSE_EXTRA) 
		bStillParsing=M4_TRUE;	// Just parse if it is an extra

	if (ai_byParseOption==LOGSYS_PARSE_NEVER) 
		bStillParsing=M4_FALSE;	// Never parse


	// If "" is the argument
	if (pszInString==NULL || *pszInString=='\0') {	
		if (ao_puParamsFilled)
			*ao_puParamsFilled=0;

		*aio_ppszInString=NULL;
		return M4_SUCCESS;
	}
	

	// For backward compatibility, if param doesn't start with #
	//  it is considered a single param.
	if (*pszInString!='#') {	
		if (ai_uNumParams==0) 
			return M4_WARNING;

		if (ao_puParamsFilled)
			*ao_puParamsFilled=1;

		*aio_ppszInString=NULL;

		if (ao_pvParams)
			ao_pvParams[0].SetString(pszInString);

		return M4_SUCCESS;
	}


	// Parse
	while (((chParse=*pszInString) != NULL) &&
				(chParse!='|' || pszInString[1]!='|')  ) {
		switch(state) {
		case NO_PARSE:
			M4_ASSERT(pszBeginParse==NULL);
			switch(chParse) {	
			case '#':
				state=BEGIN_PARSE;
				break;
	
			default:
				break;
			}
			break;

		case BEGIN_PARSE:
			M4_ASSERT(pszBeginParse==NULL);
			switch(chParse) {	
			case '#':	// New empty param: Go to next_pase state
				state=NEXT_PARSE;
				pszBeginParse = pszInString;
				--pszInString; // Don't want to advance
				break;
	
			case '*':	// Extra param
				if (uParamPos==0 && cArg==0 && bOpt==M4_FALSE) {
					bNextOpt=M4_TRUE;
					bOpt=M4_TRUE;
					state=BEGIN_OPT;
				} else {	// Not really an opt param
					state=NEXT_PARSE;
					pszBeginParse = pszInString;
					break;
				}
				break;

			default:
				state=NEXT_PARSE;
				pszBeginParse = pszInString;
				break;
			}

			if (state==NEXT_PARSE && bStillParsing==M4_FALSE) { // If we don't really want to parse (optimization)
				pszInString=strstr(pszInString-1,"||");
				if (pszInString==NULL)
					*aio_ppszInString=NULL;
				else	
					*aio_ppszInString=pszInString+2; // Skip ||
				
				if (ao_puParamsFilled)
					*ao_puParamsFilled = 0;
				return M4_SUCCESS;
			}

			break;

		case NEXT_PARSE:
			M4_ASSERT(pszBeginParse!=NULL);
			switch(chParse) {	
			case '#':	// New param
				// Just write if we are in the correct position or in the first position
				if (ao_pvParams!=NULL && 
					(uParamPos==0 || uParamPos==ai_byIdParamPos)) {

					if (ai_uNumParams==cArg) {
						if (ao_puParamsFilled)
							*ao_puParamsFilled = cArg;
						// JMT
						// Debemos salir con un estado consistente.
						// Para poder continuar parseando a partir de aqui
						//if ( *(pszBeginParse-1) != '#' )
						return M4_ERROR;	// Maximo numero de parametros
						//*aio_ppszInString=(pszBeginParse-1); // The Next point to parse, is this argument, in the #
						//return M4_WARNING;
						//return M4_SUCCESS;
					} else {
						*pszInString = '\0';
						ao_pvParams[cArg].SetString(pszBeginParse);
						*pszInString = '#';
					}
				}

				state=BEGIN_PARSE;
				cArg++;
				pszBeginParse=NULL;
				uParamPos=0;
				break;
	
			case '&':	// && means param with several positions
				if (*(pszInString-1)=='&' ) {

					// We get first param position because maybe the position doesn't exist
					// or we are in the right position
					if (ao_pvParams!=NULL && 
						(uParamPos==0 || uParamPos==ai_byIdParamPos)) {
						if (ai_uNumParams==cArg) {
							if (ao_puParamsFilled)
								*ao_puParamsFilled = cArg;
							//if ( *pszBeginParse != '#' ) {
							return M4_ERROR;
							//}
							// JMT
							// Debemos salir con un estado consistente.
							// Para poder continuar parseando a partir de aqui
							//*aio_ppszInString=pszBeginParse; // The Next point to parse
							//return M4_WARNING;
							//return M4_SUCCESS;
						} else {
							*(pszInString-1) = '\0';
							ao_pvParams[cArg].SetString(pszBeginParse);
							*(pszInString-1) = '&';
						}
					}
		
					state=BEGIN_PARSE;
					uParamPos++;	// Next position
					pszBeginParse=NULL;
				}
				break;

			default:
				break;
			}
			break;

		case BEGIN_OPT:
			M4_ASSERT(pszBeginParse==NULL);
			switch(chParse) {	
			case '#':	// Finish opt.
				state=BEGIN_PARSE;
				break;

			case '*':	// Next opt.
				bNextOpt=M4_TRUE;
				break;

			case 'S':
			case 'N':
			case 'E':
				if (bNextOpt!=M4_TRUE)
					break;

				bNextOpt=M4_FALSE;
				uOpt=0;
				for ( ; isdigit(pszInString[1]); pszInString++)
					uOpt = (pszInString[1]-'0') + uOpt*10;

				switch(chParse) {	
				case 'S':
					if (ao_puSource)
						*ao_puSource=uOpt;
					break;
				case 'N':
					if (ao_puError)
						*ao_puError=uOpt;
					break;
				case 'E':
					if (ao_puExtra)
						*ao_puExtra=uOpt;
					if (ai_byParseOption==LOGSYS_PARSE_EXTRA) 
						bStillParsing=M4_TRUE;
					if (ai_byParseOption==LOGSYS_PARSE_ERROR) 
						bStillParsing=M4_FALSE;
					break;
				default:
					M4_ASSERT(0);
				}

				break;

			default:
				break;
			}
			break;

		default:
			M4_ASSERT(0);	// Error condition: Invalid state
		}
		pszInString++; // Next token
	}

	if (ao_puParamsFilled)
		*ao_puParamsFilled = cArg;

	if (*pszInString=='\0')	// No more ||
		*aio_ppszInString=NULL;
	else	
		*aio_ppszInString=pszInString+2; // Skip ||

	return M4_SUCCESS;	
}


/*************************************************************************

NAME: FormatErrorExtraArray

*************************************************************************/
m4return_t FormatErrorExtraArray (
	m4pchar_t ai_pszParams,			
	pairFormatErrorExtra_t *ao_pExtra,
	m4uint32_t ai_uNumExtras,		
	m4puint32_t ao_puExtrasFilled)
{

	m4pchar_t pszParams, pszBeginParams;
	if (ai_pszParams)
		pszParams=ai_pszParams;
	else
		pszParams="";
	pszBeginParams = pszParams;

	m4uint32_t uExtra;
	m4pchar_t pszExtra;
	m4uint32_t cCont;

	m4uint32_t cExtras=0;
	m4return_t ret = M4_SUCCESS;

	vector < pairFormatErrorExtra_t > fmtVector;
	pairFormatErrorExtra_t fmtPair;

	if (ao_puExtrasFilled)
		*ao_puExtrasFilled=0;

	if (ao_pExtra==NULL)
		ai_uNumExtras = LOGSYS_INFINITE_BUFFER_LEN;

	while (pszParams!=NULL) {
		pszExtra=pszParams;
		DeserializeParams(&pszParams, 0, NULL, 0,NULL,
								NULL, NULL, &uExtra, LOGSYS_PARSE_NEVER);
		
		if (uExtra!=LOGSYS_INVALID_M4UINT32) {
			if (ai_uNumExtras==cExtras) {	// No room
				ret=M4_WARNING;
				break;
			}

			fmtPair.uExtraType = uExtra;
			fmtPair.uExtraHandle = (m4uint32_t)(pszExtra-pszBeginParams);
			fmtVector.push_back(fmtPair);
			cExtras++;
		}
	}

	if (ao_pExtra) {
		sort (fmtVector.begin(), fmtVector.end());
		for (cCont=0; cCont<cExtras; cCont++)  
			ao_pExtra[cCont] = fmtVector[cCont];
	}

	if (ao_puExtrasFilled)
		*ao_puExtrasFilled=cExtras;

	return ret;
}



/*************************************************************************

NAME: FormatErrorExtraParams

*************************************************************************/
m4return_t FormatErrorExtraParams (
	m4pchar_t ai_pszParams,			
	m4uint32_t ai_uExtraHandle,
	m4uchar_t ai_byIdParamPos,		
	m4VariantType *ao_pvParams,	
	m4uint32_t ai_uNumParams,		
	m4puint32_t ao_puParamsFilled)
{
	m4pchar_t pszParams;
	if (ai_pszParams)
		pszParams=ai_pszParams;
	else
		pszParams="";

	pszParams += ai_uExtraHandle;

	return DeserializeParams(&pszParams, ai_byIdParamPos, ao_pvParams, 
										ai_uNumParams, ao_puParamsFilled,
										NULL,NULL,NULL, LOGSYS_PARSE_EXTRA);
}



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////                   /////////////////////////
/////////////////////////// Private Functions /////////////////////////
///////////////////////////                   /////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////



/*************************************************************************

NAME: ParseFormatParameter

*************************************************************************/
void ParseFormatParameter(
	m4VariantType& aio_varParameter,
	ParseFormatParameter_option ai_option,
	m4language_t ai_uiLang)
{
	m4bool_t bTimeStamp;
	string strOutput;
	const m4char_t *pszFormat = "";

	char year[5], month[3], day[3];
	char hour[3], minute[3], second[3];

	strcpy(year,"0000");
	strcpy(month,"00");
	strcpy(day,"00");
	strcpy(hour,"00");
	strcpy(minute,"00");
	strcpy(second,"00");


	// If not a string, just return
	if (aio_varParameter.Type==M4CL_CPP_TYPE_STRING_VAR) {
		const m4char_t* pszBeginParse=aio_varParameter.Data.PointerChar;
		const m4char_t* pszEndParse=pszBeginParse + strlen(pszBeginParse);
		const m4char_t* pszParse;
		const m4char_t* pszDoParse;

		for (pszParse=pszBeginParse;pszParse!=pszEndParse;pszParse++) {
			pszDoParse=pszParse; 

			// Look for a date, so we search a number before the last char is reached
			while (LogSysIsDigit(*pszDoParse)==M4_FALSE && pszDoParse!=pszEndParse-1) 
				pszDoParse++;

			// We don't want to parse all, and we didn't find anything in the first parsing,
			// So we don't modify the parameter
			if (ai_option!=parse_all && pszDoParse!=pszBeginParse)
				return;

			strOutput.append(pszParse, pszDoParse-pszParse);	// Copy skipped chars
			pszParse=pszDoParse; // Skip copy chars

			// We have a number so we see if it is a year (we also see if it is the end of the parameter)
			bTimeStamp=M4_FALSE;
			if ( LogSysIsDigit(pszDoParse[0])==M4_FALSE || LogSysIsDigit(pszDoParse[1])==M4_FALSE || 
              LogSysIsDigit(pszDoParse[2])==M4_FALSE || LogSysIsDigit(pszDoParse[3])==M4_FALSE ||
              pszDoParse[4]!='-') {
				strOutput.append(pszParse,1);
				continue;
			}
			memcpy(year, pszDoParse, 4);
			pszDoParse+=5; // Skip year and -

			// We have a number so we see if it is a month
			if ( LogSysIsDigit(pszDoParse[0])==M4_FALSE || LogSysIsDigit(pszDoParse[1])==M4_FALSE || 
              pszDoParse[2]!='-') {
				strOutput.append(pszParse,1);
				continue;
			}
			memcpy(month, pszDoParse, 2);
			pszDoParse+=3; // Skip month and -

			// We have a number so we see if it is a day
			if ( LogSysIsDigit(pszDoParse[0])==M4_FALSE || LogSysIsDigit(pszDoParse[1])==M4_FALSE ) {
				strOutput.append(pszParse,1);
				continue;
			}
			memcpy(day, pszDoParse, 2);
			pszDoParse+=2; // Skip day

			// At least we have a date (year, month, day), try to search timestamp		

			// See if we have the hours
			if ( pszDoParse[0]==' ' &&
              LogSysIsDigit(pszDoParse[1])==M4_TRUE && LogSysIsDigit(pszDoParse[2])==M4_TRUE ) {
				memcpy(hour, pszDoParse+1, 2);
				pszDoParse+=3; // Skip hours
				bTimeStamp=M4_TRUE;				

				// See if we have the minutes
				if ( pszDoParse[0]==':' &&
                 LogSysIsDigit(pszDoParse[1])==M4_TRUE && LogSysIsDigit(pszDoParse[2])==M4_TRUE ) {
					memcpy(minute, pszDoParse+1, 2);
					pszDoParse+=3; // Skip minutes

					// See if we have the seconds
					if ( pszDoParse[0]==':' &&
			           LogSysIsDigit(pszDoParse[1])==M4_TRUE && LogSysIsDigit(pszDoParse[2])==M4_TRUE ) {
						memcpy(second, pszDoParse+1, 2);
						pszDoParse+=3; // Skip seconds
					} else {
						memcpy(second,"00",2);
					}
				} else {
					memcpy(minute,"00",2);
					memcpy(second,"00",2);
				}
			}

			/* Bug 0090314
			Se toman los formatos del lenguaje de regional settings si existe
			*/
			if (bTimeStamp==M4_FALSE)
			{
				pszFormat=M4ClLangInfo::Instance()->GetSettingsErrorDateFormat();
			}
			else
			{
				pszFormat=M4ClLangInfo::Instance()->GetSettingsErrorTimeFormat();
			}

			if( *pszFormat == '\0' )
			{
				/* Bug 0090314
				Ahora hay que pasar el lenguaje para coger el formateo correcto.
				Si se pasa se utiliza el que se ha pasado y si no se ha conseguido
				se hace como antes
				*/
				if( ai_uiLang != LOGSYS_INVALID_M4UINT32 )
				{
					if (bTimeStamp==M4_FALSE)
					{
						pszFormat=M4ClLangInfo::Instance()->GetErrorDateFormat(ai_uiLang);
					}
					else
					{
						pszFormat=M4ClLangInfo::Instance()->GetErrorTimeFormat(ai_uiLang);
					}
				}
				else
				{
					pszFormat = "" ;
				}
			}

			if( *pszFormat == '\0' )
			{
				if (bTimeStamp==M4_FALSE)
				{
					pszFormat=M4ClLangInfo::Instance()->GetCurrentErrorDateFormat();
				}
				else
				{
					pszFormat=M4ClLangInfo::Instance()->GetCurrentErrorTimeFormat();
				}
			}

			ParseFormatDate(year, month, day, hour, minute, second, pszFormat, strOutput);
			pszParse=pszDoParse-1; // Skip date

			// We don't want to parse all
			if (ai_option!=parse_all) // Just want to parse this
				break;

		}		

		aio_varParameter.SetString((char*)strOutput.c_str());
	}


}



