//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CADENA.CPP
// Project:			 M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:			 18/02/98
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

///////////////////////////////////////////////////////////////////////////////                                                                                             ///////////////////////////////////////////////////////////////////////////////
// INCLUDES        
          
#include "cadena.h"         
#include <stdlib.h>

#define ERROR_NOMEMORY            -1000

///////////////////////////////////////////////////////////////////////////////                                                                                             ///////////////////////////////////////////////////////////////////////////////
 
ClCadena::ClCadena()
{
	Init();
}

ClCadena::~ClCadena()
{
	End();
	Init(); // por si acaso
}

void ClCadena::End(void)
{             
	if(m_Estado & ESTADO_CREADA){
		if(m_pDirIni){
			delete []m_pDirIni;
		};
	};
}

m4int16_t ClCadena::LonReal(void)
{                           
	m4uint32_t iError=0;
   
	if(m_pDirIni!=NULL){
		m4char_t * p=m_pDirIni;
		while(iError<m_iLonBuffer){
			if(*p=='\0'){
	            break;
			} else {
				p++;
				iError++;
			};
		};
	};

	return (int)iError;
}

void ClCadena::Init(void)
{
	m_Estado=0;
	m_iPos=0;
	m_iLonBuffer=0;
      
	m_pDirIni=NULL;
	m_pDir=NULL;   
}


m4int16_t ClCadena::Asigna(m4char_t * buf)
{   
	m4int16_t i;
	
	i= (buf==NULL)? 0 : strlen(buf);
   
	return Asigna(buf, i);
}
 
m4int16_t ClCadena::Asigna(m4char_t * buf, m4uint32_t l)
{     
	m4int16_t iError;
   
	iError=0;
   
	End();           
   
	m_Estado &= ~ESTADO_CREADA;   
	m_iPos=0;      
	m_pDirIni=m_pDir=buf;
	m_iLonBuffer=l;                  
   
	return iError;
}


m4int16_t ClCadena::CopiaDe(ClCadena *a_pCad)
{   
	return Crea(a_pCad->m_pDirIni, a_pCad->m_iLonBuffer);
}

m4int16_t ClCadena::Crea(const m4char_t * buf)
{     
	return Crea(buf, (buf==NULL)? 0 : strlen(buf));
}

m4int16_t ClCadena::Crea(m4uint32_t l)
{
	return Crea(NULL, l);
}
 
m4int16_t ClCadena::Crea(const m4char_t * buf, m4uint32_t l)
{          
	m4int16_t iError = 0;   
   
	End();           
	Init();
   
	m_Estado |= ESTADO_CREADA;
   
	if(l){

		m_pDirIni=m_pDir=new m4char_t[l+1];
		
		if(m_pDir!=NULL){           

			if (buf) {
				memcpy(m_pDir,buf,l);
				m_pDir[l] = 0;
			}
			else {
				memset(m_pDir,0,l+1);
			}

			m_iLonBuffer=l;               

		} 
		else {
         iError=ERROR_NOMEMORY;
		};
	};
   
	return iError;
}
 

void ClCadena::Posiciona(m4int16_t l, m4char_t dir)
{           
	m4int16_t k;
   
	switch(dir){
		case INICIO:
			k=l;         
			break;
         
		case ACTUAL:
			k=((m4int16_t)m_iPos)+l;               
			break;

		case FINAL:
			k=((m4int16_t)m_iLonBuffer)-l-1;
			break;         
	};
   
	if(k>=0){
		if(((m4uint32_t)k)>=m_iLonBuffer){
			k=m_iLonBuffer;
		};
	} else {
		k=0;
	};
      
	m_iPos=(m4uint32_t)k;
	m_pDir=m_pDirIni+m_iPos;      
}

m4int16_t ClCadena::LeeCadena(m4char_t * buf, m4uint32_t l)
{     
	m4uint32_t t=l;
   
	while(m_iPos<m_iLonBuffer && l){
		*(buf++)=*(m_pDir++);   
		m_iPos++;
		l--;
	};
   
	return t-l;
}

m4int16_t ClCadena::EscribeCadena(const m4char_t * buf) 
{
	return EscribeCadena(buf,strlen(buf)) ;
}

m4int16_t ClCadena::EscribeCadena(const m4char_t * buf, m4uint32_t l)
{
	m4uint32_t t=l;
   
	while(m_iPos<m_iLonBuffer && l){
		*(m_pDir++)=*(buf++);   
		m_iPos++;
		l-=1;
	};
   
	return t-l;
}

m4int16_t ClCadena::EscribeCadena(ClCadena *a_pCad, m4uint32_t l)
{                                               
	return EscribeCadena(a_pCad->m_pDir, l);   
}


m4char_t ClCadena::LeeChar(void)
{     
	m4char_t c;
   
	if(m_iPos<m_iLonBuffer){
		c=*(m_pDir++);
		m_iPos++;
	} else {
		c='\0';
	};
      
	return c;   
}

m4char_t ClCadena::LeeCharPaTras(void)
{     
	m4char_t c;
   
	if(m_iPos>=0){
		c=*(m_pDir--);
		m_iPos--;
	} else {
		c='\0';
	};
      
	return c;   
}


m4char_t ClCadena::LeeCharNA(void)
{     
	m4char_t c;
	m4char_t * p=m_pDir;
	m4uint32_t i=m_iPos;
   
	if(i<m_iLonBuffer){
		c=*(p++);
		i++;
	} else {
		c='\0';
	};
      
	return c;   
}


void ClCadena::EscribeChar(m4char_t c)
{
	if(m_iPos<m_iLonBuffer){
		*(m_pDir++)=c;
		m_iPos++;
	};
}

m4int16_t ClCadena::ComparaConNA(ClCadena *a_pCad)
{                        
	m4int16_t iError;   
	m4int16_t im=a_pCad->m_iPos;
   
	iError=ComparaCon(a_pCad);
   
	if(iError){
		// no coincide:
		a_pCad->Posiciona(im, INICIO);
	};
      
	return iError;   
}

m4int16_t ClCadena::ComparaCon(ClCadena *a_pCad)
{     
	m4int16_t iError;                                   
	m4int16_t l=LonReal();      
              
	if(l){                 
		if(l==1){
			iError=(a_pCad->LeeChar()==*m_pDirIni)? 0 : -1;
		} else {   
			m4char_t * buf=new char[l];
			if(buf){            
				a_pCad->LeeCadena(buf, l);         
				iError=strncmp(buf, m_pDirIni, l);            
				delete buf;
			} else {
				iError=0;      
			};
		};
	} else {
		iError=-1;  // mal   
	};
   
	return iError;
}

m4int16_t ClCadena::Mapeo(const m4char_t * a_pTabla)
{                    
	m4uint32_t iError=0;  
	m4uint32_t i=0;

	if(m_pDirIni!=NULL){
		m4char_t * p=m_pDirIni;
		
		while(iError<m_iLonBuffer){
			m4uchar_t c=*p;  // el unsigned es IMPORTANTISIMO
         
			if(c!='\0'){    
				if(i<m_iLonBuffer){
					m4uchar_t q=a_pTabla[c];
					if(q!=' '){                                                      
						*p=q;                              
					};    
					p++;  
					iError++;
					i++;
					continue;
				};       
			};
			break;
		};
	};
      
	return (m4int16_t)iError;
}

///////////////////////////////////////////////////////////////////////////////

