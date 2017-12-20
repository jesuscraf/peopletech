//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         Compiler
// File:           sttcommon.hpp
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:             21 may 97
// Language:        C++ 
// Operating System: WINDOWS 
// Design Document:  
//
//
// Definition:
// 
//    Comunes de estadísticas
////
//==============================================================================
// COMMON...


#ifndef _STTCOMMON_HPP
#define _STTCOMMON_HPP

#include "m4dm_dll.hpp"
#include "m4string.hpp"
#include "m4define.hpp"
#include "m4types.hpp"

//Items especiales
enum eSpecialItems_t
{ 
	M4_VM_STT_SPECIAL_ITEM_NULL = 0,
	M4_VM_STT_SPECIAL_ITEM_CREATE_MCR = 1,
	M4_VM_STT_SPECIAL_ITEM_CREATE_SCR = 2,
	M4_VM_STT_SPECIAL_ITEM_CREATE_PRESENTATION = 3,
	M4_VM_STT_SPECIAL_ITEM_PROXY_SPACE_CREATION = 4,
	M4_VM_STT_SPECIAL_ITEM_PROXY_SPACE_DESTRUCTION = 5,
	M4_VM_STT_SPECIAL_ITEM_METADATA_GET_UPDATE_LIST = 6,
	M4_VM_STT_SPECIAL_ITEM_CACHE_REMOVE_OBJECT = 7,
	M4_VM_STT_SPECIAL_ITEM_STT_SET_LABEL = 8,
	M4_VM_STT_SPECIAL_ITEM_STT_PERSIST = 9,
	M4_VM_STT_SPECIAL_ITEM_DESTROY_CHANNEL = 10,
	M4_VM_STT_SPECIAL_ITEM_CREATE_MAP = 11
};

//Items especiales
enum eCacheHit_t
{ 
	M4_VM_STT_CACHE_NO_CACHEABLE = 0,
	M4_VM_STT_CACHE_FOUND = 1,
	M4_VM_STT_CACHE_NOT_FOUND = 2
};

//DBFunction
enum eDBFunctionType_t
{ 
	M4_VM_STT_DBFUNCTION_OPEN = 1,
	M4_VM_STT_DBFUNCTION_CLOSE = 2,
	M4_VM_STT_DBFUNCTION_PREPARE = 3,
	M4_VM_STT_DBFUNCTION_EXECUTE = 4,
	M4_VM_STT_DBFUNCTION_FETCH = 5,
	M4_VM_STT_DBFUNCTION_BIND_PARAM = 6,
	M4_VM_STT_DBFUNCTION_BIND_RESULT = 7
};

// DBOperation
#define	M4_STT_LDB_DELETE			1
#define	M4_STT_LDB_INSERT			2
#define	M4_STT_LDB_UPDATE			3
#define	M4_STT_LDB_SELECT			4
#define	M4_STT_LDB_DUPLICATE		5


#ifdef _WINDOWS
//En Windows nos definimos nuestra propio clock con alta precision.
class M4_DECL_M4DM ClSttClock{ 
public:
   static LARGE_INTEGER InitTimer()
   {
	   LARGE_INTEGER iFreqAux;
	   //if (!m_iClockFrequency.QuadPart)
	   {
		    BOOL b = QueryPerformanceFrequency(&iFreqAux);

			m_iClockFrequency = iFreqAux;

			//if (m_iClockFrequency.QuadPart)
			{
				//Vemos lo que tarda el propio clock
				m4double_t dStart, dEnd;
				dStart = GetMileSeconds();
				for (int i=0; i<2000;i++)
					GetMileSeconds();
				dEnd = GetMileSeconds();
				m_dClockSelfTime = (dEnd - dStart) / 2000;
			}
	   }
	   return iFreqAux;
   }

   ClSttClock()
   {
      Start();
   }
   
   ~ClSttClock()
   {
   }
   
   void Reset(void)
   {
       m_dTimeMileSeconds = 0 ;
   }
   
   void Start(void)
   {
	   m_dTimeMileSeconds = GetMileSeconds();
   }
   
   double GetDifference(void) const
   {
       return GetMileSeconds()-m_dTimeMileSeconds;
   }
   
   friend  double operator - (const ClSttClock &r2, const ClSttClock &r1)
   {
      return( r2.m_dTimeMileSeconds - r1.m_dTimeMileSeconds );
   }

   static double GetMileSeconds(void)
   {
	   double dMileSeconds;
	   if (m_iClockFrequency.QuadPart)
	   {
		    LARGE_INTEGER iAux;
		    BOOL b;
			b = QueryPerformanceCounter(&iAux);
			dMileSeconds = (m4double_t)iAux.QuadPart;
			dMileSeconds *= 1000;
			dMileSeconds /= m_iClockFrequency.QuadPart;
	   }
	   else
	   {
		   dMileSeconds = GetTickCount();
	   }

	   return dMileSeconds;
   }

   static m4double_t m_dClockSelfTime;

private:
   m4double_t m_dTimeMileSeconds;  
   static LARGE_INTEGER m_iClockFrequency;
};
#else
//En UNIX utilizamos el clm4clock
typedef ClM4Clock ClSttClock;
#endif


#endif


