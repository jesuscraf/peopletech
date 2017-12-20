
#ifndef _LDB_UTILS_HPP_
#define _LDB_UTILS_HPP_

#include "defmain.hpp"
#include "m4stl.hpp"

const m4pcchar_t g_pccCacheFilePath=M4_BACK_SLASH "LDB" M4_BACK_SLASH "m4dict.chn";

struct StLogicConn
{
	ClLConn *m_pLConn;
	m4bool_t m_bStatus;

	StLogicConn (void)
	{
		m_pLConn = 0;
		m_bStatus = M4_TRUE;
	}
};
typedef vector<struct StLogicConn>					VcLogicConn;
typedef VcLogicConn::iterator						itVcLogicConn;

struct stIdObject {
	m4id_t m_pIdObject;

	stIdObject (void) {
		m_pIdObject = NULL;
	}

	stIdObject (const struct stIdObject &ai_stCopy) {
		m_pIdObject = ai_stCopy.m_pIdObject;
	}

	friend m4bool_t operator < (const struct stIdObject &lhc, const struct stIdObject &rhc) {
		return ((strcmpi (lhc.m_pIdObject, rhc.m_pIdObject) < 0) ? M4_TRUE : M4_FALSE);
	}

	friend m4bool_t operator ==(const struct stIdObject &lhc, const struct stIdObject &rhc) {
		return ((!strcmpi (lhc.m_pIdObject, rhc.m_pIdObject)) ? M4_TRUE : M4_FALSE);
	}
};

typedef map<struct stIdObject, m4int32_t, less<struct stIdObject> >	MpObject;
typedef MpObject::iterator									itMpObject;

class ClLdbCacheContainer{
public:
	ClLdbCacheContainer(void){
		m_iCacheMode = M4CL_CACHE_NONE;
	}
	operator m4uint8_t () const  {
		return m_iCacheMode;
	}
	operator m4pchar_t() const  {
		return (m4pchar_t)m_stPath.c_str();
	}
	void operator = (m4uint8_t ai_CacheMode){
		m_iCacheMode = ai_CacheMode;
	}
	void operator = (m4pcchar_t	ai_Path){
		m_stPath = ai_Path;
		m_stPath.append(g_pccCacheFilePath);
	}


protected:
	string m_stPath;
	m4uint8_t	m_iCacheMode;
};

#endif

