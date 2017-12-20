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

short ClCadena::LonReal(void)
{                           
	unsigned long iError=0;
   
	if(m_pDirIni!=NULL){
		char * p=m_pDirIni;
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


short ClCadena::Asigna(char * buf)
{   
	short i;
	
	i= (buf==NULL)? 0 : strlen(buf);
   
	return Asigna(buf, i);
}
 
short ClCadena::Asigna(char * buf, unsigned long l)
{     
	short iError;
   
	iError=0;
   
	End();           
   
	m_Estado &= ~ESTADO_CREADA;   
	m_iPos=0;      
	m_pDirIni=m_pDir=buf;
	m_iLonBuffer=l;                  
   
	return iError;
}


short ClCadena::CopiaDe(ClCadena *a_pCad)
{   
	return Crea(a_pCad->m_pDirIni, a_pCad->m_iLonBuffer);
}

short ClCadena::Crea(const char * buf)
{     
	return Crea(buf, (buf==NULL)? 0 : strlen(buf));
}

short ClCadena::Crea(unsigned long l)
{
	return Crea(NULL, l);
}
 
short ClCadena::Crea(const char * buf, unsigned long l)
{          
	short iError;   
   
	iError=0;
   
	End();           
	Init();
   
	m_Estado |= ESTADO_CREADA;
   
	if(l!=0){
		m_pDirIni=m_pDir=new char[l+1];
		m_pDirIni[l]='\0';
		if(m_pDir!=NULL){           
			m_iLonBuffer=l;               
			if(buf){
	            // Copia la cadena pasada
				char * p=m_pDirIni;
				do{
					*(p++)=*(buf++);
					l--;
				}while(l);  
			} else {
				*m_pDirIni='\0';         
			};
		} else {
         iError=ERROR_NOMEMORY;
		};
	};
   
	return iError;
}
 

void ClCadena::Posiciona(short l, char dir)
{           
	short k;
   
	switch(dir){
		case INICIO:
			k=l;         
			break;
         
		case ACTUAL:
			k=((short)m_iPos)+l;               
			break;

		case FINAL:
			k=((short)m_iLonBuffer)-l-1;
			break;         
	};
   
	if(k>=0){
		if(((unsigned long)k)>=m_iLonBuffer){
			k=m_iLonBuffer;
		};
	} else {
		k=0;
	};
      
	m_iPos=(unsigned long)k;
	m_pDir=m_pDirIni+m_iPos;      
}

short ClCadena::LeeCadena(char * buf, unsigned long l)
{     
	unsigned long t=l;
   
	while(m_iPos<m_iLonBuffer && l){
		*(buf++)=*(m_pDir++);   
		m_iPos++;
		l--;
	};
   
	return t-l;
}

short ClCadena::EscribeCadena(const char * buf) 
{
	return EscribeCadena(buf,strlen(buf)) ;
}

short ClCadena::EscribeCadena(const char * buf, unsigned long l)
{
	unsigned long t=l;
   
	while(m_iPos<m_iLonBuffer && l){
		*(m_pDir++)=*(buf++);   
		m_iPos++;
		l-=1;
	};
   
	return t-l;
}

short ClCadena::EscribeCadena(ClCadena *a_pCad, unsigned long l)
{                                               
	return EscribeCadena(a_pCad->m_pDir, l);   
}


char ClCadena::LeeChar(void)
{     
	char c;
   
	if(m_iPos<m_iLonBuffer){
		c=*(m_pDir++);
		m_iPos++;
	} else {
		c='\0';
	};
      
	return c;   
}

char ClCadena::LeeCharPaTras(void)
{     
	char c;
   
	if(m_iPos>=0){
		c=*(m_pDir--);
		m_iPos--;
	} else {
		c='\0';
	};
      
	return c;   
}


char ClCadena::LeeCharNA(void)
{     
	char c;
	char * p=m_pDir;
	unsigned long i=m_iPos;
   
	if(i<m_iLonBuffer){
		c=*(p++);
		i++;
	} else {
		c='\0';
	};
      
	return c;   
}


void ClCadena::EscribeChar(char c)
{
	if(m_iPos<m_iLonBuffer){
		*(m_pDir++)=c;
		m_iPos++;
	};
}

short ClCadena::ComparaConNA(ClCadena *a_pCad)
{                        
	short iError;   
	short im=a_pCad->m_iPos;
   
	iError=ComparaCon(a_pCad);
   
	if(iError){
		// no coincide:
		a_pCad->Posiciona(im, INICIO);
	};
      
	return iError;   
}

short ClCadena::ComparaCon(ClCadena *a_pCad)
{     
	short iError;                                   
	short l=LonReal();      
              
	if(l){                 
		if(l==1){
			iError=(a_pCad->LeeChar()==*m_pDirIni)? 0 : -1;
		} else {   
			char * buf=new char[l];
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

short ClCadena::Mapeo(const char * a_pTabla)
{                    
	unsigned long iError=0;  
	unsigned long i=0;

	if(m_pDirIni!=NULL){
		char * p=m_pDirIni;
		
		while(iError<m_iLonBuffer){
			unsigned char c=*p;  // el unsigned es IMPORTANTISIMO
         
			if(c!='\0'){    
				if(i<m_iLonBuffer){
					unsigned char q=a_pTabla[c];
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
      
	return (short)iError;
}

///////////////////////////////////////////////////////////////////////////////

