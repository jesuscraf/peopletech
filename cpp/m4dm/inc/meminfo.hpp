
#ifndef _MEMINFO_HPP_
#define _MEMINFO_HPP_
#include "m4dm_dll.hpp"
#include "m4types.hpp"
#include "m4stl.hpp"

class ClSize;
const m4uint8_t	MAX_CHECK_POINTS							=32;


#if defined(_WINDOWS) && defined(_DEBUG)
	#include "windows.h"
	#include <crtdbg.h>
	typedef _CrtMemState *				pCrtMem;
#endif //defined(_WINDOWS) && defined(_DEBUG)

class ClAccess;
class ClNode;
class ClRecordSet;
class _ClRegister_Base;
class ClItem;
class ClBlockReference;
class ClValue;

//Informacion de Registro
struct M4_DECL_M4DM stRgMemInfo_t
{
	m4uint32_t m_nDataSize;		//Tamaño ocupado por los datos
	m4uint32_t m_nTotalSize;	//Tamaño Total = Datos+Estructuras internas.
	m4uint32_t m_nNulls;		//Numero de Nulos

	//Constructores
	stRgMemInfo_t() { Reset(); }
	stRgMemInfo_t(const stRgMemInfo_t &ai_stRhs) { 
		m_nDataSize		= ai_stRhs.m_nDataSize; 
		m_nTotalSize	= ai_stRhs.m_nTotalSize; 
		m_nNulls		= ai_stRhs.m_nNulls; 
	}
	void Reset() { m_nDataSize=m_nTotalSize=m_nNulls=0; }
	
	//Sobrecarga de operadores
	stRgMemInfo_t &operator+=(const stRgMemInfo_t &ai_stRhs) { 
		m_nDataSize		+= ai_stRhs.m_nDataSize; 
		m_nTotalSize	+= ai_stRhs.m_nTotalSize; 
		m_nNulls		+= ai_stRhs.m_nNulls; 
		return *this;
	}
	stRgMemInfo_t &operator-=(const stRgMemInfo_t &ai_stRhs) { 
		m_nDataSize		-= ai_stRhs.m_nDataSize; 
		m_nTotalSize	-= ai_stRhs.m_nTotalSize; 
		m_nNulls		-= ai_stRhs.m_nNulls; 
		return *this;
	}
};

//Informacion de RecordSet
struct M4_DECL_M4DM stRSMemInfo_t : public stRgMemInfo_t
{
	m4uint32_t m_nRegisters;	//Numero de Registros

	//Constructores
	stRSMemInfo_t() { Reset(); }
	stRSMemInfo_t(const stRSMemInfo_t &ai_stRhs) { m_nRegisters	= ai_stRhs.m_nRegisters; }
	void Reset() { m_nRegisters=0; }
	
	//Sobrecarga de operadores
	stRSMemInfo_t &operator+=(const stRSMemInfo_t &ai_stRhs) {
		this->stRgMemInfo_t::operator+=(ai_stRhs);
		m_nRegisters		+= ai_stRhs.m_nRegisters; 
		return *this;
	}
	stRSMemInfo_t &operator-=(const stRSMemInfo_t &ai_stRhs) {
		this->stRgMemInfo_t::operator-=(ai_stRhs);
		m_nRegisters		-= ai_stRhs.m_nRegisters; 
		return *this;
	}
};

//Informacion de Nodo
struct M4_DECL_M4DM stNodeMemInfo_t : public stRSMemInfo_t
{
	m4uint32_t m_nBlocks;		//Numero de Bloques
	m4uint32_t m_nProxyBlocks;	//Numero de Bloques Proxy

	//Constructores
	stNodeMemInfo_t() { Reset(); }
	stNodeMemInfo_t(const stNodeMemInfo_t &ai_stRhs) { m_nRegisters	= ai_stRhs.m_nRegisters; }
	void Reset() { m_nBlocks=0;m_nProxyBlocks=0;}
	
	//Sobrecarga de operadores
	stNodeMemInfo_t &operator+=(const stNodeMemInfo_t &ai_stRhs) {
		this->stRSMemInfo_t::operator+=(ai_stRhs);
		m_nRegisters		+= ai_stRhs.m_nRegisters; 
		return *this;
	}
	stNodeMemInfo_t &operator-=(const stNodeMemInfo_t &ai_stRhs) {
		this->stRSMemInfo_t::operator-=(ai_stRhs);
		m_nRegisters		-= ai_stRhs.m_nRegisters; 
		return *this;
	}
};

//Informacion de Canal
struct M4_DECL_M4DM stChannelMemInfo_t : public stNodeMemInfo_t
{
	m4uint32_t m_nNodes;	//Numero de Nodos

	//Constructores
	stChannelMemInfo_t() { Reset(); }
	stChannelMemInfo_t(const stChannelMemInfo_t &ai_stRhs) { m_nRegisters = ai_stRhs.m_nRegisters; }
	void Reset() { m_nNodes=0; }

	//Sobrecarga de operadores
	stChannelMemInfo_t &operator+=(const stChannelMemInfo_t &ai_stRhs) {
		this->stNodeMemInfo_t::operator+=(ai_stRhs);
		m_nRegisters		+= ai_stRhs.m_nRegisters; 
		return *this;
	}
	stChannelMemInfo_t &operator-=(const stChannelMemInfo_t &ai_stRhs) {
		this->stNodeMemInfo_t::operator-=(ai_stRhs);
		m_nRegisters		-= ai_stRhs.m_nRegisters; 
		return *this;
	}

};


//Sobrecarga de operadores globales

//stRgMemInfo_t
inline stRgMemInfo_t M4_DECL_M4DM operator+ (const stRgMemInfo_t &ai_stLhs, const stRgMemInfo_t &ai_stRhs)
{ 
	stRgMemInfo_t oTemp(ai_stLhs);
	return oTemp+=ai_stRhs;
}
inline stRgMemInfo_t M4_DECL_M4DM operator- (const stRgMemInfo_t &ai_stLhs, const stRgMemInfo_t &ai_stRhs)
{ 
	stRgMemInfo_t oTemp(ai_stLhs);
	return oTemp-=ai_stRhs;
}
inline ostream M4_DECL_M4DM &operator<< (ostream &ai_ofs, const stRgMemInfo_t &ai_rs)
{
	ai_ofs << "Data  Size: "	<< ai_rs.m_nDataSize	<< endl;
	ai_ofs << "Total Size: "	<< ai_rs.m_nTotalSize	<< endl;
	ai_ofs << "No. Nulls: "		<< ai_rs.m_nNulls		<< endl;
	return ai_ofs;
}

//stRSMemInfo_t
inline stRSMemInfo_t M4_DECL_M4DM operator+ (const stRSMemInfo_t &ai_stLhs, const stRSMemInfo_t &ai_stRhs)
{ 
	stRSMemInfo_t oTemp(ai_stLhs);
	return oTemp+=ai_stRhs;
}
inline stRSMemInfo_t M4_DECL_M4DM operator- (const stRSMemInfo_t &ai_stLhs, const stRSMemInfo_t &ai_stRhs)
{ 
	stRSMemInfo_t oTemp(ai_stLhs);
	return oTemp-=ai_stRhs;
}
inline ostream M4_DECL_M4DM &operator<< (ostream &ai_ofs, const stRSMemInfo_t &ai_rs)
{
	ai_ofs << ((const stRgMemInfo_t&) ai_rs) ;
	ai_ofs << "No. Registers: "	<< ai_rs.m_nRegisters	<< endl;
	return ai_ofs;
}

//stNodeMemInfo_t 
inline stNodeMemInfo_t M4_DECL_M4DM operator+ (const stNodeMemInfo_t &ai_stLhs, const stNodeMemInfo_t &ai_stRhs)
{ 
	stNodeMemInfo_t oTemp(ai_stLhs);
	return oTemp+=ai_stRhs;
}
inline stNodeMemInfo_t M4_DECL_M4DM operator- (const stNodeMemInfo_t &ai_stLhs, const stNodeMemInfo_t &ai_stRhs)
{ 
	stNodeMemInfo_t oTemp(ai_stLhs);
	return oTemp-=ai_stRhs;
}
inline ostream M4_DECL_M4DM &operator<< (ostream &ai_ofs, const stNodeMemInfo_t &ai_rs)
{
	ai_ofs << ((const stRSMemInfo_t&) ai_rs) ;
	ai_ofs << "No. Blocks: "	<< ai_rs.m_nBlocks	<< endl;
	ai_ofs << "No. Empty Blocks: "	<< ai_rs.m_nProxyBlocks	<< endl;
	return ai_ofs;
}

//stChannelMemInfo_t 
inline stChannelMemInfo_t M4_DECL_M4DM operator+ (const stChannelMemInfo_t &ai_stLhs, const stChannelMemInfo_t &ai_stRhs)
{ 
	stChannelMemInfo_t oTemp(ai_stLhs);
	return oTemp+=ai_stRhs;
}
inline stChannelMemInfo_t M4_DECL_M4DM operator- (const stChannelMemInfo_t &ai_stLhs, const stChannelMemInfo_t &ai_stRhs)
{ 
	stChannelMemInfo_t oTemp(ai_stLhs);
	return oTemp-=ai_stRhs;
}
inline ostream M4_DECL_M4DM &operator<< (ostream &ai_ofs, const stChannelMemInfo_t &ai_rs)
{
	ai_ofs << ((const stNodeMemInfo_t&) ai_rs) ;
	ai_ofs << "No. Nodes: "		<< ai_rs.m_nNodes	<< endl;
	return ai_ofs;
}



//Estadisticas del Uso de Memoria a distintos niveles: Canal, Nodo, RecordSet, Registro
class M4_DECL_M4DM ClChannelMemInfo
{
public:
	ClChannelMemInfo(void);
	void Reset();
	m4return_t GetMemInfo(ClAccess *,stChannelMemInfo_t &);
	m4return_t GetMemInfo(ClNode *,stNodeMemInfo_t &);
	m4return_t GetMemInfo(ClRecordSet *,stRSMemInfo_t &);
	m4return_t GetMemInfo(ClBlockReference &);				//publica ???
	m4return_t GetMemInfo(_ClRegister_Base *,stRgMemInfo_t &);

protected:
	m4return_t _GetNodeMemInfo(ClNode *);
	m4return_t _GetRSMemInfo(ClRecordSet *);
	m4return_t _GetRegisterMemInfo(_ClRegister_Base *);

	m4return_t _GetIIDataSize(ClItem &);
	m4return_t _GetValueItDataSize(ClItem &);

	void _UpdateMemInfo(stChannelMemInfo_t &);
	void _UpdateMemInfo(stNodeMemInfo_t &);
	void _UpdateMemInfo(stRSMemInfo_t &);
	void _UpdateMemInfo(stRgMemInfo_t &);

	m4uint32_t m_nNodes;
	m4uint32_t m_nBlocks;
	m4uint32_t m_nProxyBlocks;

	m4uint32_t m_nRegisters;
	m4uint32_t m_nNulls;

	m4uint32_t m_nDataSize;
	m4uint32_t m_nTotalSize;

};


#if defined _WINDOWS&& defined _DEBUG

class M4_DECL_M4DM CRT{
public:

	CRT()
	{
		hF=0;
		m_nStates = 0;
	}
	~CRT()
	{
		if (hF){
			CloseHandle((HANDLE) hF);
			hF=0;
		}
	}

	m4int16_t 	SetCrtMemCheckPoint();
	m4return_t	SetCrtDumpFile(m4pchar_t ai_path);
	m4uint32_t  GetAllocMem(m4int16_t check1,m4int16_t check2=-1);
	m4uint32_t  DumpCrtMem(m4int16_t check=0);

private:
	m4int16_t			GetNumStates();
	_CrtMemState		vMemState[MAX_CHECK_POINTS];
	_HFILE				hF;
	m4int16_t			m_nStates;

};
#endif //defined _WINDOWS&& defined _DEBUG

#endif
