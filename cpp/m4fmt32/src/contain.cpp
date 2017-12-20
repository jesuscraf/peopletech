//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CONTAIN.CPP
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
//    This module defines...
//
//
//==============================================================================

#define M4CT_SISTEMATICO2

#include <stdlib.h>
#include <string.h>
          
#include "fmt.h"
#include "fmtdf.cpp"
       
#include "contain.hpp"
#include "rctables.h"

#ifdef M4CT_SCREEN
	const HDC g_hdc;
	const HBRUSH g_hbr;
#endif

///////////////////////////////////////////////////////////////////////////////

m4uint32_t FMT_ClPage::m_N=0;

FMT_ClElement  *g_M4CTGetElement(m4uint32_t a_uHandle)
{
	ClMIT_TM *TM = GET_TM() ;
	StFMT_TaskEnv* poTaskEnv = TM_GET_TASK_ENV(TM) ;

	return (poTaskEnv->Container_g_pContainer)? poTaskEnv->Container_g_pContainer->GetElement(a_uHandle) : NULL;
}

FMT_ClContainer  *g_GetAutoContainer(void)
{
    ClMIT_TM *TM = GET_TM() ;
	StFMT_TaskEnv* poTaskEnv = TM_GET_TASK_ENV(TM) ;
 
    return poTaskEnv->Container_g_pContainer;
}

//------------------------------------------------------------------------------
// FMT_ClPage constructor
//------------------------------------------------------------------------------

FMT_ClPage::FMT_ClPage(FMT_ClContainer  *a_pContainer, m4uint32_t a_uPosition, m4uint32_t a_uFirstHandle)
{
	m_uCount=0;
	m_pContainer=a_pContainer;
	m_uPosition=a_uPosition;
	m_uFirstHandle=a_uFirstHandle;  
	m_uCache= m_pContainer->InsertInCache(a_uPosition)? 0 : 0xffff;

	memset(m_ppElements, 0, M4CT_ELEMENTSBYPAGE*sizeof(FMT_ClElement*));
}

//------------------------------------------------------------------------------
// FMT_ClPage destructor
//------------------------------------------------------------------------------

FMT_ClPage::~FMT_ClPage()
{
	if(m_pContainer){
		m_pContainer->FreeFromCache(m_uPosition);
		m_pContainer->FreePage(m_uPosition);      
	};
}

//------------------------------------------------------------------------------
// Page fuctions
//------------------------------------------------------------------------------

m4uint32_t FMT_ClPage::InsertElementIn(FMT_ClElement* a_pElement, m4uint32_t a_uPos)
{
	m4uint32_t handle = 0;
	FMT_ClElement** pp = m_ppElements + a_uPos;
	
	if(*pp==NULL){
		*pp=a_pElement;
		a_pElement->m_uHandle=handle=m_uFirstHandle+a_uPos;
		m_uCount++;   
		m_pContainer->m_ulCount++;

		#ifdef M4CT_SCREEN
			RECT r={a_uPos*4, m_uPosition*4, a_uPos*4+4, m_uPosition*4+4};
			FillRect(g_hdc, &r, g_hbr);
		#endif
            
		if(m_uCache==a_uPos){
			if(m_uCount<M4CT_ELEMENTSBYPAGE){
	            m_uCache=GetFreePosition();
			} else {
				m_pContainer->FreeFromCache(m_uPosition);
				m_uCache=0xffff;
			};
		};
	};

	return handle;
}

m4uint32_t FMT_ClPage::GetFreePosition(void)
{
	m4uint32_t n;
	
	#ifdef M4CT_SISTEMATICO1
		n=0;
		while(n<M4CT_ELEMENTSBYPAGE){
			if(m_ppElements[n]==NULL){
				break;
			};
		n++;
		};
	#else
   
		for(m4uint32_t i=M4CT_ELEMENTSBYPAGE; i; i--){
			n=m_N++;
			if(m_N>=M4CT_ELEMENTSBYPAGE){
				m_N=0;
			};
			if(m_ppElements[n]==NULL){
				break;
			};
		};

	#endif

	return n;
}

m4uint32_t FMT_ClPage::InsertElement(FMT_ClElement* a_pElement)
{
	m4uint32_t handle=0;

	if(m_uCount<M4CT_ELEMENTSBYPAGE){

		if(m_uCache!=0xffff){
			if(m_ppElements[m_uCache]==NULL){
				m_ppElements[m_uCache]=a_pElement;
				a_pElement->m_uHandle=handle=m_uFirstHandle+m_uCache;
				m_uCount++;
				m_pContainer->m_ulCount++;

				#ifdef M4CT_SCREEN
					RECT r={m_uCache*4, m_uPosition*4, m_uCache*4+4, m_uPosition*4+4};
					FillRect(g_hdc, &r, g_hbr);
				#endif
	
				if(m_uCount<M4CT_ELEMENTSBYPAGE){
					m_uCache=GetFreePosition();
				} else {
					m_pContainer->FreeFromCache(m_uPosition);
					m_uCache=0xffff;
				};
			};
		} else {
			m4uint32_t n=GetFreePosition();
			m_ppElements[n]=a_pElement;
			a_pElement->m_uHandle=handle=m_uFirstHandle+n;
			m_uCount++;
         
			m_pContainer->m_ulCount++;
         
			#ifdef M4CT_SCREEN
				RECT r={n*4, m_uPosition*4, n*4+4, m_uPosition*4+4};
				FillRect(g_hdc, &r, g_hbr);
			#endif
		};
	} else {
		// Error
	};  

	return handle;
}

//------------------------------------------------------------------------------
// This function gets the element of a page by its handle
//------------------------------------------------------------------------------

FMT_ClElement* FMT_ClPage::GetElement(m4uint32_t a_uHandle)
{
	m4uint32_t h=a_uHandle-m_uFirstHandle;

	return (h<M4CT_ELEMENTSBYPAGE)? m_ppElements[h] : NULL;
}

//------------------------------------------------------------------------------
// This function frees an element of a page by its handle
//------------------------------------------------------------------------------

m4uint32_t FMT_ClPage::FreeElement(m4uint32_t a_uHandle)
{                       
	m4uint32_t error;
	m4uint32_t h=a_uHandle-m_uFirstHandle;

	if(h<M4CT_ELEMENTSBYPAGE){
		m_ppElements[h]=NULL;
		#ifdef M4CT_SCREEN
			RECT r={h*4, m_uPosition*4, h*4+4, m_uPosition*4+4};
			FillRect(g_hdc, &r, GetStockObject(BLACK_BRUSH));
		#endif
		m_uCount--;              
		m_pContainer->m_ulCount--;

		if(!m_uCount){
			h=0xffff;
			delete this;
		} else {
			if(m_uCache==0xffff){
				if(m_pContainer->InsertInCache(m_uPosition)){
					m_uCache=h;
				};
			};
		};        
		error=0;
	} else {
		// Error
		error=1;
	};

	return error;
}

m4bool_t FMT_ClPage::ForEachElement(m4bool_t (FMT_ClElement::*a_pFunction)(void  *, void  *), void  *a_pArg, void  *a_pReturn)
{
	m4bool_t res=M4_TRUE;
	FMT_ClElement** ppe = m_ppElements;
	m4uint32_t c = m_uCount;
   
	while(c){
		FMT_ClElement  *pe=*ppe;
		if(pe){ 
			res=(pe->*a_pFunction)(a_pArg, a_pReturn);
			if(!res){  // puede que al volver el contenedor esté destruido (en ese caso m_iLon es cero)
				break;
			};
			c--;
		};
		ppe++;
	};

	return res;            
}

//------------------------------------------------------------------------------
// This function destroys an element of a page
//------------------------------------------------------------------------------

void FMT_ClPage::AutoDestroy(void)
{
	FMT_ClElement** ppe=m_ppElements;
	m4int16_t n = m_uCount;
	while(n){            // m_uCount es decrementado en FreeElement
		FMT_ClElement* pe=*ppe;
		if(pe){
			delete pe;
		}             ;

		ppe++;
		n--;
	};
}

//------------------------------------------------------------------------------
// Class FMT_ClContainer
//------------------------------------------------------------------------------

FMT_ClContainer::FMT_ClContainer()
{   
	ClMIT_TM *TM = GET_TM() ;
	m_poTaskEnv = TM_GET_TASK_ENV(TM) ;
	
	m_ulCount=0L;
	m_uCacheCount=0;
	m_uPagesCount=0;
	m_uMaxPage=0;
	m_ppPage=NULL;
	m_uIndexCache=0;
	
	memset(m_puFreeSpace, 0xff, sizeof(m_puFreeSpace));
}

FMT_ClContainer::~FMT_ClContainer()
{
	if(m_ppPage!=NULL){
		delete m_ppPage;
		m_ppPage=NULL;
	};
            
	m_uMaxPage=0;
	m_uPagesCount=0;
	m_poTaskEnv->Container_g_bContainerDestroyed=M4_TRUE;
}

//------------------------------------------------------------------------------
// Container Functions
//------------------------------------------------------------------------------

FMT_ClPage* FMT_ClContainer::NewPage(m4uint32_t a_uPage, m4uint32_t a_uFirstHandle)
{
	FMT_ClPage  *ppage=new FMT_ClPage(this, a_uPage, a_uFirstHandle);

	if(ppage!=NULL){
		m_ppPage[a_uPage]=ppage;
		m_uPagesCount++;      

		#ifdef M4CT_SCREEN
			RECT r={0, a_uPage*4, M4CT_ELEMENTSBYPAGE*4, a_uPage*4+4};
			FillRect(g_hdc, &r, GetStockObject(BLACK_BRUSH));
			m4char_t buffer[32];
			m4int16_t l=wsprintf(buffer, "NPages: %i   ", m_uPagesCount);
			TextOut(g_hdc, 200, 32, buffer, l);
		#endif
	} else {
		// Error
	};
   
	return ppage;
}

//------------------------------------------------------------------------------
// This function creates a new page
//------------------------------------------------------------------------------

m4uint32_t FMT_ClContainer::CreatePage(void)
{
	m4uint32_t n=0;
	m4uint32_t fh=1;

	for(;;){      
		if(n>=m_uMaxPage){
			// no hay sitio -> se incrementa el array         
			n=EnlargeArray(M4CT_INCARRAY);
			if(n==0xffff){
				break;
			};
		};
      
		if(m_ppPage[n]==NULL){
			FMT_ClPage  *ppage;
			ppage=NewPage(n, fh);
			if(ppage!=NULL){
				break;
			};
		};

		n++;
		fh+=M4CT_ELEMENTSBYPAGE;
	};

	return n;
}   


m4uint32_t FMT_ClContainer::EnlargeArray(m4uint32_t a_uIncArray)
{
	m4uint32_t n=0xffff;
	m4uint32_t nl=m_uMaxPage+a_uIncArray;

	if(nl>M4CT_MAXNUMBEROFPAGES){
		if(m_uMaxPage<M4CT_MAXNUMBEROFPAGES){
			nl=M4CT_MAXNUMBEROFPAGES;
		} else {
			// Error
			return 0xffff;
		};
	};

	FMT_ClPage  **pp=new FMT_ClPage  *[nl];
   
	if(pp!=NULL){
		memcpy(pp, m_ppPage, m_uMaxPage*sizeof(FMT_ClPage  *));
		memset(pp+m_uMaxPage, 0, a_uIncArray*sizeof(FMT_ClPage  *));

		if(m_ppPage!=NULL) {
			delete (void  **)m_ppPage;  // para que no llame a los destructores
		};

		m_ppPage=pp;      
		n=m_uMaxPage;
		m_uMaxPage=nl;
      
		#ifdef M4CT_SCREEN
			m4char_t buffer[32];
			m4int16_t l=wsprintf(buffer, "MaxPage: %i   ", m_uMaxPage);
			TextOut(g_hdc, 200, 16, buffer, l);
		#endif
	} else {
		// ERROR_NOMEMORY;
	};

	return n;
}

//------------------------------------------------------------------------------
// This function frees a page from memory
//------------------------------------------------------------------------------

void FMT_ClContainer::FreePage(m4uint32_t a_uPage)
{
	if(a_uPage<m_uMaxPage){
		m_ppPage[a_uPage]=NULL;      

		#ifdef M4CT_SCREEN
			RECT r={0, a_uPage*4, M4CT_ELEMENTSBYPAGE*4, a_uPage*4+4};
			FillRect(g_hdc, &r, GetStockObject(WHITE_BRUSH));
		#endif
	} else {
		// Error
	};

	m_uPagesCount--;

	#ifdef M4CT_SCREEN
		m4char_t buffer[32];
		m4int16_t l=wsprintf(buffer, "NPages: %i   ", m_uPagesCount);
		TextOut(g_hdc, 200, 32, buffer, l);
	#endif
   
	if(!m_uPagesCount){
		delete this;
	};
}

//------------------------------------------------------------------------------
// This function calculates...
//------------------------------------------------------------------------------

m4uint32_t FMT_ClContainer::InsertElement(FMT_ClElement* a_pElement)
{     
   m4int16_t handle=0;

   if(a_pElement!=NULL){
#ifdef M4CT_SCREEN
      m_poTaskEnv->Container_g_lSearchs++;
#endif
      m4uint32_t n;
      if(m_uCacheCount){
         for(m4uint32_t i=M4CT_DIMCACHE; i; i--){
            n=m_puFreeSpace[m_uIndexCache];
   
            m_uIndexCache++;
            if(m_uIndexCache>=M4CT_DIMCACHE){
               m_uIndexCache=0;
            }
   
            if(n!=0xffff){         
               FMT_ClPage  *ppage;
               ppage=m_ppPage[n];
               if(ppage!=NULL){
                  handle=ppage->InsertElement(a_pElement);
               }
   
               if(handle){                  
                  break;
               }
               else{
                  //MSGBOX("Error: h==0")
               }
            }
         }
      }

if(!handle){
#ifdef M4CT_SCREEN
      m_poTaskEnv->Container_g_lCacheFaults++;
#endif
      m4uint32_t uFirstNullPage=0xffff;
#ifdef M4CT_SISTEMATICO2
      // Metodo sistematico:
      n=0;      
      for(;;){
         if(n>=m_uMaxPage){
            // no hay paginas libres -> se crea una nueva
            if(uFirstNullPage==0xffff){
               n=EnlargeArray(M4CT_INCARRAY);               
               if(n==0xffff){
                  break;
               }
            }
            else{
               n=uFirstNullPage;
            }

            if(NewPage(n, n*M4CT_ELEMENTSBYPAGE+1)==NULL){
               break;
            }            
         }

         if(m_ppPage[n]==NULL){
            if(uFirstNullPage==0xffff)
               uFirstNullPage=n;
         }
         else{
            handle=m_ppPage[n]->InsertElement(a_pElement);
            if(handle){
               break;
            }
         }

         n++;
      }
#else
      m4uint32_t i=m_uMaxPage>>3;
      for(;;){
         if(i){
            n=((long)rand()*(long)m_uMaxPage)/RAND_MAX;
         }
         else{
            if(uFirstNullPage==0xffff){
               n=EnlargeArray(M4CT_INCARRAY);
               if(n==0xffff){
                  break;
               }
            }
            else{
               n=uFirstNullPage;
            }

            if(NewPage(n, n*M4CT_ELEMENTSBYPAGE+1)==NULL){
               break;
            }            
         }

         if(m_ppPage[n]==NULL){
            if(uFirstNullPage==0xffff)
               uFirstNullPage=n;
         }
         else{
            handle=m_ppPage[n]->InsertElement(a_pElement);
            if(handle){
               break;
            }
         }

         i--;
      }
#endif
}

   }
   else{
      // ERROR_NULLELEMENT;
   }     

   return handle;
}

//------------------------------------------------------------------------------
// This function calculates...
//------------------------------------------------------------------------------

m4uint32_t FMT_ClContainer::InsertElementIn(FMT_ClElement* a_pElement, m4uint32_t a_uHandle)
{
   m4uint32_t handle=0;

   if(a_uHandle){
      FMT_ClPage  *ppage=NULL;
      m4uint32_t i=(a_uHandle-1)>>M4CT_LOG2ELEMENTSBYPAGE;
      m4uint32_t r=i*M4CT_ELEMENTSBYPAGE+1;
      
      m4uint32_t n=0;
      if(i>=m_uMaxPage){
         n=EnlargeArray(((i-m_uMaxPage)/M4CT_INCARRAY+1)*M4CT_INCARRAY);                   
      }
      
      if(n==0xffff){
            // Error            
      }
      else{
         ppage=m_ppPage[i];
           
         if(ppage==NULL){            
            ppage=NewPage(i, r);
         }
     
         if(ppage!=NULL){
            handle=ppage->InsertElementIn(a_pElement, a_uHandle-r);            
         }
      }
   }

   return handle;
}

//------------------------------------------------------------------------------
// This function calculates...
//------------------------------------------------------------------------------

FMT_ClElement  *FMT_ClContainer::GetElement(m4uint32_t a_uHandle)
{
   FMT_ClElement* pElement=NULL;

   if(a_uHandle){      
      m4uint32_t i=(a_uHandle-1)>>M4CT_LOG2ELEMENTSBYPAGE;
      if(i<m_uMaxPage){
         FMT_ClPage  *ppage=m_ppPage[i];
         if(ppage){
            pElement=ppage->GetElement(a_uHandle);
         }
         else{
            // Error
         }
      }
      else{
         // Error
      }
   }
   else{
      // Error
   }

   return pElement;
}

//------------------------------------------------------------------------------
// This function calculates...
//------------------------------------------------------------------------------

void FMT_ClContainer::FreeElement(m4uint32_t a_uHandle)
{
   if(a_uHandle){      
      m4uint32_t page=(a_uHandle-1)>>M4CT_LOG2ELEMENTSBYPAGE;
      if(page<m_uMaxPage){
         FMT_ClPage  *ppage=m_ppPage[page];
         if(ppage){          
            m_poTaskEnv->Container_g_bContainerDestroyed=M4_FALSE;
            
            m4uint32_t error;                  
            error=ppage->FreeElement(a_uHandle);
            
            if(error){
               //
            }
         }

         else{
            // Error
         }
      }
      else{
         // Error
      }
   }
   else{
      // Error
   }   
}

//------------------------------------------------------------------------------
// This function applies the specified function to each element
//------------------------------------------------------------------------------

void FMT_ClContainer::ForEachElement(m4bool_t (FMT_ClElement::*a_pFunction)(void  *, void  *), void  *a_pArg, void  *a_pReturn)
{     
   FMT_ClPage  **pp=m_ppPage;
   m4uint32_t c;
       
   c=m_uPagesCount;    
   
   while(c){
      FMT_ClPage  *ppage=*pp;
      if(ppage){
         if(!ppage->ForEachElement(a_pFunction, a_pArg, a_pReturn))   // puede que al volver el contenedor esté destruido (en ese caso m_iLon es cero)
            break;
         c--;
      }

      pp++;
   }
}

void FMT_ClContainer::FreeFromCache(m4uint32_t n)
{
   m4uint32_t  *pfs=m_puFreeSpace;
   for(m4uint32_t i=M4CT_DIMCACHE; i; i--){
      if(*pfs==n){
         *pfs=0xffff;   // Se elimina
         m_uCacheCount--;
         break;
      }
      pfs++;
   }
}

m4bool_t FMT_ClContainer::InsertInCache(m4uint32_t n)
{
   m4bool_t res=M4_FALSE;

   if(m_uCacheCount<M4CT_DIMCACHE){
      for(m4uint32_t i=M4CT_DIMCACHE; i; i--){
         if(m_puFreeSpace[m_uIndexCache]==0xffff){
            m_puFreeSpace[m_uIndexCache]=n;
            m_uCacheCount++;
            res=M4_TRUE;
            break;
         }
         else{
            m_uIndexCache++;
            if(m_uIndexCache>=M4CT_DIMCACHE){
               m_uIndexCache=0;
            }
         }
      }
   }

   return res;
}

//------------------------------------------------------------------------------
// Destruye todas las paginas, las paginas destruyen todos los elementos
// y por fin se llama al destructor del FMT_ClContainer 
//------------------------------------------------------------------------------

void FMT_ClContainer::AutoDestroy(void)
{
   FMT_ClPage  **pp=m_ppPage;

   m4int16_t n = m_uPagesCount;
   while(n){       // m_uPagesCount es decrementado en FreePage
      FMT_ClPage  *ppage=*pp;
      if(ppage){
         ppage->AutoDestroy();
      }

      pp++;
	  n--;
   }
}


m4int16_t FMT_ClElement::m_iCountElement=0 ; 

//------------------------------------------------------------------------------
// FMT_ClElement constructor
//------------------------------------------------------------------------------

FMT_ClElement::FMT_ClElement()
{  
    ClMIT_TM *TM = GET_TM() ;
	m_poTaskEnv = TM_GET_TASK_ENV(TM) ;
    m_uError=0;
    m_uHandle=0;
    ++m_iCountElement;
}

//------------------------------------------------------------------------------
// FMT_ClElement destructor
//------------------------------------------------------------------------------

FMT_ClElement::~FMT_ClElement()
{
	--m_iCountElement;
}

//------------------------------------------------------------------------------
// This function inserts a new element in the specified container
//------------------------------------------------------------------------------
m4uint32_t FMT_ClElement::Insert(FMT_ClContainer  **a_ppContainer)
{
   FMT_ClContainer  *pc=*a_ppContainer;

   if(pc==NULL){      
      pc=new FMT_ClContainer();
      if(pc!=NULL){
         *a_ppContainer=pc;         
      }
   }   
   
   if(pc!=NULL)
      pc->InsertElement(this);

   return m_uHandle;
}

m4uint32_t FMT_ClElement::AutoInsert(void)
{                                  
   return Insert(&m_poTaskEnv->Container_g_pContainer);
}


m4uint32_t FMT_ClElement::AutoInsertIn(m4uint32_t a_iHandle)
{  
   if(m_poTaskEnv->Container_g_pContainer==NULL){      
      m_poTaskEnv->Container_g_pContainer=new FMT_ClContainer();
   }   

   return (m_poTaskEnv->Container_g_pContainer!=NULL)? m_poTaskEnv->Container_g_pContainer->InsertElementIn(this, a_iHandle) : 0;
}

void FMT_ClElement::Free(FMT_ClContainer  **a_ppContainer)
{     
   if(m_uHandle){
      FMT_ClContainer  *pc=*a_ppContainer;
      if(pc!=NULL){
         pc->FreeElement(m_uHandle);
         
         if(m_poTaskEnv->Container_g_bContainerDestroyed){
            *a_ppContainer=NULL;
         }
      }   
   }
}

m4bool_t FMT_ClElement::AutoFree(void)
{         
   Free(&m_poTaskEnv->Container_g_pContainer);
                         
   return (m_poTaskEnv->Container_g_pContainer==NULL)? M4_TRUE : M4_FALSE;
}


//------------------------------------------------------------------------------
// FMT_ClElementAuto constructor
//------------------------------------------------------------------------------

FMT_ClElementAuto::FMT_ClElementAuto() : FMT_ClElement()
{
   if(!AutoInsert())
      m_uError=1;
}

//------------------------------------------------------------------------------
// FMT_ClElementAuto Destructor
//------------------------------------------------------------------------------

FMT_ClElementAuto::~FMT_ClElementAuto()
{
   AutoFree();
}

