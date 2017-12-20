//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             CASTELLA.CPP
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


///////////////////////////////////////////////////////////////////////////////

#include "fmt.h"
#include "fmtdf.cpp"

#include "castella.hpp"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////

#define FLAG_ACENTLESS				1
#define FLAG_IKIND                  2

///////////////////////////////////////////////////////////////////////////////

static const m4char_t* s_xllones[]=
{        
   " mil",
   "millón",
   "billón",
   "trillón"
};

static const m4char_t* s_centenas[]=
{                 
   "doscient",
   "trescient",
   "cuatrocient",
   "quinient",
   "seiscient",
   "setecient",
   "ochocient",
   "novecient",
   "cien"
};

static const m4char_t* s_decenas1[]=
{ 
   "diez",
   "once",
   "doce",
   "trece",
   "catorce",
   "quince"   
};

static const m4char_t* s_decenas2[]=
{ 
   "dieci",
   "veinti",
   "treinta",
   "cuarenta",
   "cincuenta",
   "sesenta",
   "setenta",
   "ochenta",
   "noventa"   
};

static const m4char_t* s_unidades1[]=
{     
   "uno",
   "dos",
   "tres",
   "cuatro",
   "cinco",
   "seis",
   "siete",
   "ocho",
   "nueve",
   "cero"
};

static const m4char_t* s_unidades2[]=
{     
   "uno",
   "dós",
   "trés",
   "cuatro",
   "cinco",
   "séis",
   "siete",
   "ocho",
   "nueve",
   "cero"
};

static const m4char_t* s_meses[]=
{
	"Enero",
    "Febrero",
    "Marzo",
    "Abril",
    "Mayo",
    "Junio",
    "Julio",
    "Agosto",
    "Septiembre",
    "Octubre",
    "Noviembre",
    "Diciembre"
};

static const m4char_t* s_dias[]=
{
    "Lunes",
    "Martes",
    "Mi\351rcoles",
    "Jueves",
    "Viernes",
    "S\341bado",
    "Domingo"
};
                                                                               
///////////////////////////////////////////////////////////////////////

const StPlugIn ESP = {
    &ESP_NumberToText,
	&ESP_DecimalToText,
	&ESP_YearToText,
	&ESP_GetMonth,  
	&ESP_GetDay
};

void AttachESPLanguage(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage)
{
	*ai_stInfo = &ESP;
	strcpy(ai_pcLanguage, "SP");
	ai_pcLanguage[2]='\0';
};

////////////////////////////////////////////////////////////////////////////////////

m4int16_t ESP_NumberToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender, m4bool_t bIsDate)
{ 
	m4bool_t bHay, bTio;

	if(!a_pNum->EsNula()){   
		char buf[256];
		m4uint32_t anterior=0;
		bHay=M4_FALSE;
		bTio=M4_TRUE;
		a_pNum->Posiciona(0, INICIO);
		m4uint32_t cifras=a_pNum->LonReal();
   
		if(cifras>18){                  
			m4uint32_t l=cifras-18;
			a_pNum->LeeCadena(buf, l);
			a_pCad->EscribeCadena(buf, l);
			cifras=18; 
			buf[l]='\0';
			anterior=atoi(buf);     
      
			if(anterior){
				a_pCad->EscribeChar(' ');
				bHay=M4_TRUE; 
				if(anterior==1){
					a_pCad->EscribeCadena(s_xllones[3], strlen(s_xllones[3]));
				} else {
					// en plural :      
					a_pCad->EscribeCadena(s_xllones[3], strlen(s_xllones[3])-2);
					a_pCad->EscribeCadena("ones", 4);
				};	
			};
		};
         
		m4uint32_t nmill=cifras/6;
		m4uint32_t resto=cifras-nmill*6;    
		m4uint32_t k;
		if(resto){
			nmill++;
			k=resto;
		} else {
			k=6;
		};
		char millones[]="000000";
		a_pNum->LeeCadena(millones+6-k, k);
   
		for(;;){   
			if(nmill==1){
				bTio=bGender;
			};  
			m4uint32_t numero=ESP_CenDecUni(a_pCad, millones, &bHay, &bTio, bIsDate);       
			if(numero){       
				const m4char_t* p;
				m4int16_t l;
				if(numero==1){   
					p=s_xllones[0]+1;
					l=3;
					bHay=M4_TRUE;   
				} else{
					p=s_xllones[0];
					l=4;      
				};
				if(bHay){
					a_pCad->EscribeCadena(p, l);
				};
			};  
			m4uint32_t numero2=ESP_CenDecUni(a_pCad, millones+3, &bHay, &bTio, bIsDate);
			if(numero2==1){
				// se ha quedado el uno sin escribir      
				if(bIsDate==M4_FALSE){
					a_pCad->EscribeCadena("una", bTio? 2 : 3);
				} else {
					a_pCad->EscribeCadena("uno", 3);
				};
			}
			anterior=numero*1000+numero2;
			nmill--;
			if(nmill){ 
				if(bHay){ 
					a_pCad->EscribeChar(' ');
				} else {
					bHay=M4_TRUE;
				};            
				if(anterior) {
					if(anterior==1){
						a_pCad->EscribeCadena(s_xllones[nmill], strlen(s_xllones[nmill]));
					} else {
						// en plural :      
						a_pCad->EscribeCadena(s_xllones[nmill], strlen(s_xllones[nmill])-2);
						a_pCad->EscribeCadena("ones", 4);
					};
					a_pNum->LeeCadena(millones, 6);
				};
			} else { 
				if(!bHay){
					if(!anterior){
					a_pCad->EscribeCadena(s_unidades1[9], 4);
					};
				};
				break;
			};
		};
	};
	return 0;
}


///////////////////////////////////////////////////////////////////////////////

const m4char_t* ESP_GetMonth(m4uint16_t ai_iMes)
{		
	return s_meses[ai_iMes-1];
}

const m4char_t* ESP_GetDay(m4uint16_t ai_iDia)
{
	return s_dias[ai_iDia-1];
}

m4int16_t ESP_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender) 
{
	// guardar el género actual y ponerlo a masc
	bGender=M4_TRUE;

	ESP_NumberToText(a_pCad, a_pNum, bGender, M4_TRUE); 
	return 1;
}

m4int16_t ESP_DecimalToText(ClCadena  *a_pCad, ClCadena  *a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender)
{ 
	m4bool_t bHay, bTio;

	if(!a_pNum->EsNula()){   
   
		bHay=M4_FALSE;
		bTio=bGender;  //No hay xllones que son siempre masculinos
		a_pNum->Posiciona(0, INICIO);
		m4uint32_t anterior=0;
		m4uint32_t cifras=a_pNum->LonReal();
		m4uint32_t grupos=cifras/ai_iDecGrup;   //No llegaria hasta aqui si ai_iDecGrup <1 o ai_iDecGrup >3
		m4uint32_t resto=cifras-grupos*ai_iDecGrup;    
		m4uint32_t tamgrup=ai_iDecGrup;       //Se debe inicializar
		char *s= new char [ai_iDecGrup];

		if(resto){  //Vemos resto en ultimo grupo
			grupos++;
		};
		for(;;){   
			if(grupos==1) { //grupos indica el grupo actual
				//Para el grupo incompleto
				if(resto>0 && resto<ai_iDecGrup){
					tamgrup=resto;
				} else {
					tamgrup=ai_iDecGrup;
				};
			};
			//leemos grupo actual
			a_pNum->LeeCadena(s, tamgrup);
     		//escribimos numeros
			m4char_t* p= s;
			m4uint32_t i= tamgrup;
   
			while(*p=='0'){
				if(bHay){
					a_pCad->EscribeCadena(" ", 1);
   				};
   				a_pCad->EscribeCadena(s_unidades1[9], 4);
				bHay=M4_TRUE;   
				if(!(--i)){
      				break;
				};
   				p++;
			};

			if(i) {    //habremos acabado de recorrer el grupo?
				anterior=ESP_CenDecUni(a_pCad, s, &bHay, &bTio, M4_FALSE, tamgrup);    //Grupo debe escribir primero los ceros
			};
			if(anterior==1){
				// se ha quedado el uno sin escribir      
   				if(bHay)
					a_pCad->EscribeCadena(bTio? " uno" : " una", 4);
				else
					a_pCad->EscribeCadena(bTio? " uno" : " una", 3);
			}
			//otro grupo
			grupos--;
     		if(!grupos){ //No hay mas grupos, hay que acabar esto
				break;
			};
		};//Fin bucle infinito   
	}; 
 return 0;
}


m4uint32_t ESP_CenDecUni(ClCadena  *a_pCad, m4char_t* a_cendecuni, m4bool_t* pbHay,
						 m4bool_t* pbTio,  m4bool_t bIsDate, m4uint32_t tamgrup)
{     
	m4uint32_t numero=0L;
	m4char_t Cen, Dec, Uni;
   
	switch(tamgrup)
	{    
   		default:  //Por defecto solo permite agrupar de 3 en 3
   		case 3:
   	  		Cen =   *a_cendecuni-'0';
   	  		Dec = a_cendecuni[1]-'0';
   			Uni = a_cendecuni[2]-'0';
   			break;
   		case 2:
   	  		Cen = 0;
   	  		Dec =   *a_cendecuni-'0';
   			Uni = a_cendecuni[1]-'0';
   			break;
   		case 1:
   	  		Cen = 0;
   	  		Dec = 0;
   			Uni = *a_cendecuni-'0';
   			break;
	};
   
	//Centenas
	if(Cen>0){      
		if(*pbHay){
			a_pCad->EscribeChar(' ');      
		} else {
         *pbHay=M4_TRUE;
		};
		numero=Cen*100;   
      
		if(Cen==1){      
			a_pCad->EscribeCadena(s_centenas[8], 4);
			if(Dec | Uni){
				a_pCad->EscribeCadena("to", 2);
			} else {
				return numero;
			};
		} else {
			if(Cen<10){     
				const m4char_t* p=s_centenas[Cen-2];            
				a_pCad->EscribeCadena(p, strlen(p));
				a_pCad->EscribeCadena( *pbTio? "os" : "as", 2);            
			} else {
				;// error
			};
		};  
	};
   
	//Decenas
	if(Dec>0){          
		if(*pbHay){
			a_pCad->EscribeChar(' ');      
		} else {
			*pbHay=M4_TRUE;
        };
		numero+=Dec*10;      
                  
		if(Dec==1){
			if(Uni<6){
				const m4char_t* p=s_decenas1[Uni];            
				a_pCad->EscribeCadena(p, strlen(p));            
				return numero;   
			};                
		} else {
			if(Dec==2){
				if(Uni==0){
				a_pCad->EscribeCadena("veinte", 6);
				return numero;   
				};
			};
		};
		const m4char_t* p=s_decenas2[Dec-1];                                                   
		a_pCad->EscribeCadena(p, strlen(p));
	};
   
	//Unidades
	if(Uni>0){                                    
		const char  **pp;      
		numero+=Uni;      
		if(Dec>0 && Dec<3){               
			pp=s_unidades2;
		} else {         
			pp=s_unidades1;       
			if(*pbHay){
				a_pCad->EscribeChar(' ');
            };
			if(Dec){
				a_pCad->EscribeCadena("y ", 2);                     
			};
		};
 		*pbHay=M4_TRUE;
                
		if(Uni==1){                                                      
			if(numero!=1){
				m4char_t* p;
				m4int16_t l;
				if(*pbTio){
					if(bIsDate==M4_FALSE){
						p=(Dec>2)? "un" : "ún";
						l=2;
					} else {
						p=(Dec>2)? "uno" : "uno";
						l=3;
					};
				} else {  
					p="una";
					l=3;
				};         
				a_pCad->EscribeCadena(p, l);            
			};
		} else {
			const m4char_t* p=pp[Uni-1];
			a_pCad->EscribeCadena(p, strlen(p));                
		};            
	};   
	return numero;
}
