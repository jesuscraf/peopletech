#ifndef _LDB_DEF_HPP_
#define _LDB_DEF_HPP_

#include "m4stl.hpp"
#include "m4dbtypes.hpp"
//typedef const char m4cint8_t;

typedef m4pcchar_t m4id_t;

#define M4LDB_DEBUG_LEVEL_0		0
#define M4LDB_DEBUG_LEVEL_1		1
#define M4LDB_DEBUG_LEVEL_2		2
#define M4LDB_DEBUG_LEVEL_3		3


// Indice nulo.
m4cint8_t M4LDB_NULL_INDEX							= -1;

m4cint8_t M4LDB_SQL_NO_DATA_FOUND					= 100;

const m4uint16_t	M4LDB_SIZE_LONG					= 4096;
const m4uint8_t		M4LDB_SIZE_DEFAULT_STRING		= 255;
const m4uint16_t	M4LDB_SIZE_BLOB					= M4_MAX_PATH + M4LDB_SIZE_DEFAULT_STRING;

// Cadena vacía. !! 
// Ojo con el Template donde se usa: template <class T> m4pchar_t M4LDB_PSTR(T *X)
// en Sun CC 4.2 no se resuelve el simbolo y una solucion era cablearlo a ""
// parece que el problema es por definir la constante como array de char en vez de char*
m4cint8_t M4LDB_EMPTY						[] = "";
//m4cint8_t *M4LDB_EMPTY						= "";


// Constantes SQL.
m4cint8_t M4LDB_SQL_NULL_DATA				= -1;
m4cint8_t M4LDB_SQL_NTS						= -3;

#define M4LDB_SQL_LEN_DATA_AT_EXEC(length) (-(length)-100)

// Valor nulo.
m4cint8_t M4LDB_NULL_VALUE					= 0;

// Tamaño de los datos.
const m4uint8_t M4LDB_SIZE_DOUBLE				= 8;
const m4uint8_t M4LDB_SIZE_DATE					= 10;
const m4uint8_t M4LDB_SIZE_TIMESTAMP			= 19;

// Tipos de sentencias.
enum eStmType_t
{
	M4LDB_DELETE = 1,
	M4LDB_INSERT,
	M4LDB_UPDATE,
	M4LDB_SELECT,
	M4LDB_DUPLICATE
};

// Niveles de aislamiento.
enum eIsolationLevel_t
{
	M4LDB_IL_UNDEFINED = 0,
	M4LDB_IL_READ_UNCOMMITED,
	M4LDB_IL_READ_COMMITED,
	M4LDB_IL_REPEATABLE_READ,
	M4LDB_IL_SERIALIZABLE
};

// Códigos de transacción.
enum eTransType_t
{
	M4LDB_BEGIN_TRANS = 1,
	M4LDB_COMMIT,
	M4LDB_ROLLBACK
};

template <class T>
m4double_t M4LDB_DOUBLE(T *X)
{
	m4double_t *pTemp; 
	return (m4double_t) ( (pTemp = (m4double_t *)X) ? (*pTemp) : M4LDB_NULL_VALUE);
}

template <class T>
m4bool_t M4LDB_BOOL(T *X)
{
	m4double_t *pTemp;
	return (m4bool_t) ( (pTemp = (m4double_t *)X) ? ((*pTemp) ? M4_TRUE : M4_FALSE) : M4_FALSE);
}


template <class T>
m4int32_t M4LDB_NUMBER(T *X)
{
	m4double_t *pTemp; 
	return (m4int32_t) ( (pTemp = (m4double_t *)X) ? (*pTemp) : M4LDB_NULL_VALUE);
}

enum eConnectionType_t {
	M4LDB_CONN_TYPE_NORMAL						= 0,	
	M4LDB_CONN_TYPE_DYNAMIC_2X					= 1,
	M4LDB_CONN_TYPE_DYNAMIC						= 2
};

struct	stConexionProperties{
	string				m_stConnStr;
	eDDBBType_t			m_eDBW;
	eIsolationLevel_t	m_eILevel;
	m4int16_t			m_numMaxCursor;
	m4bool_t			m_bCursorOpened;
	string				m_stConExec;
	m4bool_t			m_bIsCritical;
	eConnectionType_t	m_eConnectionType;
	
	stConexionProperties(void){
		m_eDBW = M4DBW_ORACLE_DDBB;
		m_eILevel = M4LDB_IL_UNDEFINED;
		m_numMaxCursor= 240;
		m_bCursorOpened = M4_FALSE;
		m_bIsCritical = M4_FALSE;
		m_eConnectionType = M4LDB_CONN_TYPE_NORMAL;
	}
};

#endif

