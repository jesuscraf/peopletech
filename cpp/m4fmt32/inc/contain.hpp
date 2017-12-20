//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CONTAIN.HPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98 - 12/06/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
//
// Definition:
//
//    This module defines the real containers of the strings and 
//	  formats in the project
//
//==============================================================================

#ifndef __CONTAIN_HPP
#define __CONTAIN_HPP

#define M4CT_INCARRAY                   8
#define M4CT_MAXNUMBEROFELEMENTS    32768
#define M4CT_LOG2ELEMENTSBYPAGE         3
#define M4CT_ELEMENTSBYPAGE             (1<<M4CT_LOG2ELEMENTSBYPAGE)

#define M4CT_MAXNUMBEROFPAGES (M4CT_MAXNUMBEROFELEMENTS/M4CT_ELEMENTSBYPAGE)

#define M4CT_DIMCACHE  32

#include "m4types.hpp"
#include "plugin.hpp"
#include "clm4fmt.hpp"

////////////////////////////////////////////////////////////////////////////////

class FMT_ClElement;
class FMT_ClPage;
class FMT_ClContainer;

extern FMT_ClElement		*g_M4CTGetElement(m4uint32_t a_uHandle);
extern FMT_ClContainer	*g_GetAutoContainer(void);

/////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------
//   Clase FMT_ClElement
//-------------------------------------------------------------------------------

class FMT_ClElement
{

private:

	static m4int16_t m_iCountElement ; 

public:                                     
                
			FMT_ClElement();
	virtual ~FMT_ClElement();

	m4uint32_t GetHandle(void){ return m_uHandle; }; //posicion
	m4uint32_t GetError(void){ return m_uError; };
	m4uint32_t Insert(FMT_ClContainer **a_ppContainer);
	m4uint32_t AutoInsert(void);
	m4uint32_t AutoInsertIn(m4uint32_t a_iHandle);
	void       Free(FMT_ClContainer **a_ppContainer);
	m4bool_t   AutoFree(void);
         
protected:
   
	StFMT_TaskEnv*	 m_poTaskEnv;	
	m4uint32_t       m_uHandle;
	m4uint32_t       m_uError;
   
friend class FMT_ClPage;
friend class FMT_ClContainer;

};

//--------------------------------------------------------------------------------
// Clase FMT_ClPage
//--------------------------------------------------------------------------------

class FMT_ClPage
{

public:
   
	FMT_ClPage(FMT_ClContainer *, m4uint32_t a_uPosition, m4uint32_t a_uFirstHandle);
	~FMT_ClPage();
	
	//Sobrecarga del operador elemento
	FMT_ClElement* operator [](m4uint32_t a_uIndex)	{ return GetElement(a_uIndex); }

private:

	//Miembros
	static m4char_t	  m_iDir;
	static m4uint32_t m_N;
	m4uint32_t		  m_uCache;
	FMT_ClElement*    m_ppElements[M4CT_ELEMENTSBYPAGE];
	FMT_ClContainer*  m_pContainer;
	m4uint32_t        m_uCount;
	m4uint32_t        m_uPosition;
	m4uint32_t        m_uFirstHandle;

	//Metodos
	m4uint32_t		  InsertElementIn(FMT_ClElement  *a_pElement, m4uint32_t a_uPos);
	m4uint32_t        InsertElement(FMT_ClElement  *a_pElement);
	FMT_ClElement*    GetElement(m4uint32_t a_uHandle);
	m4uint32_t        FreeElement(m4uint32_t a_uHandle);
	m4bool_t          ForEachElement(m4bool_t (FMT_ClElement::*a_pFunction)(void  *, void  *), void  *a_pArg, void  *a_pReturn);
	void              AutoDestroy(void);
	m4uint32_t        GetFreePosition(void);

friend class FMT_ClContainer;

};

//----------------------------------------------------------------------------------
//  Class FMT_ClContainer
//----------------------------------------------------------------------------------

class FMT_ClContainer
{

public: 
             FMT_ClContainer();
	virtual ~FMT_ClContainer();

	//Metodos publicos
	m4uint32_t     InsertElement(FMT_ClElement  *a_pElement);
	m4uint32_t     InsertElementIn(FMT_ClElement  *a_pElement, m4uint32_t a_uHandle);
	FMT_ClElement* GetElement(m4uint32_t a_uHandle);
	void           FreeElement(m4uint32_t a_uHandle);
	void           ForEachElement(m4bool_t(FMT_ClElement::*a_pFunction)(void *, void *), void *a_pArg, void *a_pReturn);
	void           AutoDestroy(void);
	unsigned long  GetCount(void) { return m_ulCount; };

private:

	//Miembros
   StFMT_TaskEnv*	m_poTaskEnv;
   m4uint32_t		m_puFreeSpace[M4CT_DIMCACHE];
   m4uint32_t		m_uIndexCache;
   m4uint32_t       m_uPagesCount;
   m4uint32_t       m_uMaxPage;            
   unsigned long	m_ulCount;
   m4uint32_t       m_uCacheCount;
   FMT_ClPage**     m_ppPage;

   //Metodos privados
   FMT_ClPage* NewPage(m4uint32_t a_uPosition, m4uint32_t a_uFirstHandle);
   m4uint32_t  CreatePage(void);
   m4uint32_t  EnlargeArray(m4uint32_t a_uIncArray);
   void        FreePage(m4uint32_t a_uPosition);
   m4bool_t    InsertInCache(m4uint32_t n);
   void        FreeFromCache(m4uint32_t n);

friend class FMT_ClPage;

};


//-----------------------------------------------------------------------------
//  Clase FMT_ClElementAuto
//-----------------------------------------------------------------------------

class FMT_ClElementAuto : public FMT_ClElement
{  

public:
             FMT_ClElementAuto();
	virtual ~FMT_ClElementAuto();
};

#endif

///////////////////////////////////////////////////////////////////////////////

