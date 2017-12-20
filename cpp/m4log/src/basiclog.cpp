//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              M4Log
// File:                basiclog.cpp
// Project:             mind3.x      
// Author:              Meta Software M.S. , S.A
// Date:                10-May-1999
// Language:            C++
// Operating System:    WINDOWS, UNIX
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//    Class implementation that is a wrapper for parsing parameter
//			in the error messages.
//
//
//==============================================================================


#include "basiclog.hpp"

#include "syncro.hpp"
#include "cldates.hpp"


static const m4int32_t	M4_BASICLOG_BUFFER_LEN = (1024*1-1);	// Long. Max of the buffer = 1 Kb
static const m4char_t	M4_BASICLOG_STORAGE[]	= "#*S1*#"	;	// String format for logmsg.ini messages
static const size_t		M4_BASICLOG_STORAGE_LEN=strlen(M4_BASICLOG_STORAGE);	// Len of storage string

static const m4char_t	M4_BASICLOG_CONCAT_STRING[] = "&&";	// SubString to concat id and name
static const m4char_t	M4_BASICLOG_CONCAT_PARAM[] = "#";	// SubString to concat arguments

static const m4char_t	M4_BASICLOG_NEW_ERROR[] = "||#*S1*N%d*";
static const m4char_t	M4_BASICLOG_NEW_EXTRA[] = "||#*S1*E%d*";


// Truco: en Debug se chequea el tipo, pero en release no (nos ahorramos la llamada a la funcion en las trazas de log)
#ifdef _DEBUG
m4return_t DumpBasicLog(m4dumplog_t ai_eDumpState)
{
	return ( (ai_eDumpState == DUMPLOG_OK) ? M4_SUCCESS : M4_ERROR) ;
}
#endif // _DEBUG


// Constructor and virtual destructor

M4ClBasicLog::M4ClBasicLog (m4bool_t ai_bEnableLocks /* =M4_TRUE */)
{	
	m_iSize = M4_BASICLOG_BUFFER_LEN+1;	// Initial buffer size
	if (ai_bEnableLocks==M4_TRUE)
		m_poMutex = new ClMutex(M4_TRUE);
	else
		m_poMutex = NULL;
	m_pcBuffer = new m4char_t[m_iSize];
    
	m_bLocked = M4_FALSE;
}

M4ClBasicLog::~M4ClBasicLog ()
{
	if (m_pcBuffer) {
		delete [] m_pcBuffer;
		m_pcBuffer = NULL;
		m_iSize = 0;
	}
	if (m_poMutex) {
		if (m_bLocked)
			m_poMutex->Unlock();
		delete m_poMutex;
		m_poMutex = NULL;
	}
}


// Initialize and Reset functions

void M4ClBasicLog::Init ()
{
	m_pcCadFormato[0] = '\0';
	strcpy(m_pcBuffer, M4_BASICLOG_STORAGE);
	m_iPos = M4_BASICLOG_STORAGE_LEN;

	m_bConcat = M4_FALSE;
	m_bConcatOn = M4_FALSE;
	m_bIsFirst = M4_TRUE;
	m_bDumpActive = M4_TRUE;
}


void M4ClBasicLog::Reset ()
{
	m_bLocked = M4_FALSE;
	m_iType = ERRORLOG;
	m_iNumError = 0;
	
	//Si el buffer se realoco por encima de lo normal, reajustamos para que baje la memoria
	if (m_iSize > (M4_BASICLOG_BUFFER_LEN+1) ) {
		delete [] m_pcBuffer;
		m_pcBuffer = NULL;
		m_iSize = M4_BASICLOG_BUFFER_LEN+1;	//Inicialmente Tamaño de buffer prefijado
		while (m_pcBuffer==NULL) {	//Seria "la pera" un bucle infinito aqui !!!
			m_pcBuffer = new m4char_t[m_iSize];
		}
	}
	Init();
}



// Function Lock - Unlock for thread-safe

void M4ClBasicLog::Lock()
{
	if (m_poMutex)
		m_poMutex->Lock();
	if (!m_bLocked)
		m_bLocked = M4_TRUE;
	Init();
}


void M4ClBasicLog::Unlock()
{
	Reset();
	if (m_bLocked)
		m_bLocked = M4_FALSE;
	if (m_poMutex)
		m_poMutex->Unlock();
}




// Buffer reallocation
void M4ClBasicLog::ReallocToConcat(m4uint32_t ai_iSize)
{
	// Verify length
	m4uint32_t iSizeNeed = (m4uint32_t)m_iPos + ai_iSize;
	if ( m_iSize <= (iSizeNeed + 5) ) {	// 5 extra bytes
		m4pchar_t pcTemp = new m4char_t[iSizeNeed+5];
		if (pcTemp) {
			// Realloc the buffer
			strcpy(pcTemp, m_pcBuffer);
			delete [] m_pcBuffer;
			m_pcBuffer = pcTemp;
			m_iSize = iSizeNeed+5;
		}
	}
}


// Prepara la salida del argumento, indicando el espacio necesario
void M4ClBasicLog::PrepareOutput(
	m4pcchar_t ai_pcSubCadFormato, 
	m4pchar_t &ao_pcBuffer, 
	m4uint32_t iSize)
{	
	m_pcCadFormato[0] = '\0';
	if (m_bConcat || m_bConcatOn) {
		if (!m_bIsFirst) {	// First argument, don't go back
			--m_iPos;
			if (m_bConcatAnd)	// "&&"
				strcpy(m_pcCadFormato, M4_BASICLOG_CONCAT_STRING);
			else
				*m_pcCadFormato = '\0';
		};
		m_bConcat = M4_FALSE;
	} else {
		*m_pcCadFormato = '\0';
	}
	m_bIsFirst = M4_FALSE;
	m_bConcatAnd = M4_FALSE;
	strcat(m_pcCadFormato, ai_pcSubCadFormato);
	strcat(m_pcCadFormato, M4_BASICLOG_CONCAT_PARAM);

	ReallocToConcat(iSize);

	ao_pcBuffer = m_pcBuffer + m_iPos;
}


// Sobrecarga de operador <<  para tipos basicos

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4double_t ai_d) 
{
	/*
    os  << setiosflags(ios::fixed)
		<< setw (10)
		<< setprecision (6)
		<<ai_d;
		*/
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput("%10.8f", pcBuffer, 50);	//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_d);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4uint32_t ai_ui)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput("%d", pcBuffer, 30);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_ui);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4int32_t ai_i)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput("%d", pcBuffer, 30);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_i);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4int_t ai_i)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput("%d", pcBuffer, 30);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_i);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}


M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog, const m4int64_t ai_i)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput(M4_PRIdPTR, pcBuffer, 30);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_i);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4pcchar_t ai_s)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		if (!ai_s) {
			aio_oLog.PrepareOutput("%s", pcBuffer, 1);		//bytes como maximo que ocupara el tipo formateado
			sprintf(pcBuffer, aio_oLog.m_pcCadFormato, " ");	//espacio por defecto
		}else {
			aio_oLog.PrepareOutput("%s", pcBuffer, (m4uint32_t)strlen(ai_s));//bytes como maximo que ocupara el tipo formateado
			sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_s);

			/* Bug 0102154
			Se cambian los '#' por 3 porque sino se confunde con el separador
			A la hora de visualizar o volcar habrá que deshacer el cambio
			Se tiene como excepción la información adicional que puede tener #, y empieza por ||#*E
			*/
			m4uint32_t	i = 0 ;
			size_t		iEnd = 0 ;

			iEnd = strlen( ai_s ) ;

			if( iEnd < 5 || memcmp( ai_s, "||#*E", 5 ) != 0 )
			{
				for( i = 0 ; i < iEnd ; i++ )
				{
					if( pcBuffer[ i ] == '#' )
					{
						pcBuffer[ i ] = 3 ;
					}
				}
			}

		}
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4char_t ai_c)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput("%c", pcBuffer, 1);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_c);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4bool_t ai_b)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput("%s", pcBuffer, 1);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, (ai_b? "1":"0"));
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}

M4ClBasicLog &operator << (M4ClBasicLog& aio_oLog,const m4float_t ai_f)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput("%10.8f", pcBuffer, 30);
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_f);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}


// ****************************
// Indirect manipulators
// ****************************

// Funcion-Manipulador con 2 args (indirecta)
M4ClBasicLog &BeginBasicLog(
	M4ClBasicLog& aio_oLog, 
	m4int32_t ai_iNumError, 
	m4int16_t ai_iTipoError)
{	
	aio_oLog.Lock();
	aio_oLog.SetNumError	(ai_iNumError);
	aio_oLog.SetTypeError	(ai_iTipoError);
	aio_oLog.SetDumpState();
	return aio_oLog;
}

// Funcion-Manipulador con 1 arg (fecha) (indirecta)
M4ClBasicLog& DateBasicLogf(
	M4ClBasicLog& aio_oLog, 
	m4date_t ai_iDate)
{
	if (aio_oLog.IsDumpActive()) {
		int			iYear, iMonth, iDay;
		m4int32_t	iHour, iMinute, iSecond;
		m4pchar_t pcBuffer;

		//convertimos a año,mes,dia, Formateamos, y metemos fecha
		ClDateToNumbers (ai_iDate, iYear, iMonth, iDay, iHour, iMinute, iSecond );

		if (iHour==0 && iMinute==0 && iSecond==0) {
			//Salida: aa-mm-dd hh:mm:ss
			aio_oLog.PrepareOutput("%04d-%02d-%02d", pcBuffer, 40);
			sprintf(pcBuffer, aio_oLog.m_pcCadFormato, iYear, iMonth, iDay);
		} else {
			//Salida: aa-mm-dd
			aio_oLog.PrepareOutput("%04d-%02d-%02d %02d:%02d:%02d", pcBuffer, 40);
			sprintf(pcBuffer, aio_oLog.m_pcCadFormato, iYear, iMonth, iDay, iHour, iMinute, iSecond);
		}
		
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}


// Manipulador de 1 args. (nuevo error)  (llamda in-directa)
M4ClBasicLog& NewErrorBasicLogf	(M4ClBasicLog& aio_oLog, m4uint32_t ai_uCode)
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput(M4_BASICLOG_NEW_ERROR, pcBuffer, 30);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_uCode);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}


// Manipulador de 1 args. (nuevo extra) (llamda in-directa)
M4ClBasicLog& NewExtraBasicLogf	(M4ClBasicLog& aio_oLog, m4uint32_t ai_uExtra)	
{
	if (aio_oLog.IsDumpActive()) {
		m4pchar_t pcBuffer;
		aio_oLog.PrepareOutput(M4_BASICLOG_NEW_EXTRA, pcBuffer, 30);		//bytes como maximo que ocupara el tipo formateado
		sprintf(pcBuffer, aio_oLog.m_pcCadFormato, ai_uExtra);
		aio_oLog.m_iPos += strlen(pcBuffer);
	}
	return aio_oLog;
}


// ****************************
// Direct manipulators
// ****************************

// Funcion-Manipulador con 0 args (directa) : Finalizar error
m4dumplog_t EndBasicLog(M4ClBasicLog& aio_oLog)
{
	aio_oLog.flush();
	aio_oLog.Unlock();	
	return DUMPLOG_OK;
}


// Funcion que vuelca Info de "*** File: namefile(line) ***" en DEBUG
// Volcamos INFO como Error EXTENDIDO: E103
string BasicLogLineInfoDebug(const char *ai_pszFileName, int ai_iLine)
{
	string stTemp;
	char pcBuffer[16];
	
	//Formateamos al estilo que espera la Log:
	stTemp.append("*** File: ");
	stTemp.append(ai_pszFileName);
	stTemp.append("(");
	sprintf(pcBuffer, "%d", ai_iLine);
	stTemp.append(pcBuffer);
	stTemp.append(") ***");

	return stTemp;
}

