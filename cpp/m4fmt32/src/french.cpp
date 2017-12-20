//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             FRENCH.CPP
// Project:          M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:             21/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
// Propietary:       Manuel Lazcano P�rez
// Modifications:
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include "fmt.h"
#include "fmtdf.cpp"

#include "french.hpp"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

static const m4char_t* s_xllones[]=
{
	"mille",
	"million",
	"milliard",
	"billion",
	"mille"
};


static const m4char_t* s_decenas1[]=
{
	"dix",
	"onze",
	"douze",
	"treize",
	"quatorze",
	"quinze",
	"seize",
	"dix-sept",
	"dix-huit",
	"dix-neuf"
};


static const m4char_t* s_decenas2[]=
{
	"vingt",
	"trente",
	"quarante",
	"cinquante",
	"soixante",
	"soixante",
	"quatre-vingt",
	"quatre-vingt",
	" cent"
};


static const m4char_t* s_unidades1[]=
{
	"un",
	"deux",
	"trois",
	"quatre",
	"cinq",
	"six",
	"sept",
	"huit",
	"neuf",
	"z�ro"
};

static const m4char_t* s_meses[]=
{
	"Janvier",
	"F�vrier",
	"Mars",
	"Avril",
	"Mai",
	"Juin",
	"Juillet",
	"Ao�t",
	"Septembre",
	"Octobre",
	"Novembre",
	"D�cembre"
};

static const m4char_t* s_dias[]=
{	
	"Lundi",
	"Mardi",
	"Mercredi",
	"Jeudi",
	"Vendredi",
	"Samedi",
	"Dimanche" 
};


///////////////////////////////////////////////////////////////////////

const StPlugIn FRA = {
    &FRA_NumberToText,
	&FRA_DecimalToText,
	&FRA_YearToText,
	&FRA_GetMonth,  
	&FRA_GetDay
};

void AttachFRALanguage(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage)
{
	*ai_stInfo = &FRA;
	strcpy(ai_pcLanguage, "FR");
	ai_pcLanguage[2]='\0';

};

////////////////////////////////////////////////////////////////////////////////////

m4int16_t FRA_NumberToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender, m4bool_t bIsDate)
{
	m4bool_t bHay;

	if(!a_pNum->EsNula()){   
   
		m4char_t buf[256];   
		m4uint32_t anterior=0;
		bHay=M4_FALSE;
		a_pNum->Posiciona(0, INICIO);
		m4uint16_t cifras=a_pNum->LonReal();
   
		if(cifras>18){                  
			m4uint16_t l=cifras-18;
			a_pNum->LeeCadena(buf, l);
			a_pCad->EscribeCadena(buf, l);
			cifras=18; 
			buf[l]='\0';
			anterior=atoi(buf);     
      
			if(anterior){
				a_pCad->EscribeChar(' ');
				bHay=M4_TRUE;     
				a_pCad->EscribeCadena(s_xllones[2],strlen(s_xllones[2])); //EscribeXllones
			};
		};
         
		m4uint16_t nmill=cifras/3;
		m4uint16_t resto=cifras-nmill*3;    
		m4uint16_t k;
		if(resto){
			nmill++;
			k=resto;
		} else {
			k=3;
		};
		m4char_t millones[]="000000";
		a_pNum->LeeCadena(millones+3-k, k);
   
		for(;;){   
			if(nmill==1){
				anterior=FRA_CenDecUni(a_pCad, millones, &bHay)*1000+FRA_CenDecUni(a_pCad, millones+3, &bHay);  
			};
			nmill--;
			if(nmill){ 
				anterior=FRA_CenDecUni(a_pCad, millones, &bHay)*1000+FRA_CenDecUni(a_pCad, millones+3, &bHay);
				if(bHay){
					a_pCad->EscribeChar(' ');
				} else {
					bHay=M4_TRUE;
                };
				if(anterior || nmill==3){
					a_pCad->EscribeCadena(s_xllones[nmill-1],strlen(s_xllones[nmill-1]));
				};
				a_pNum->LeeCadena(millones, 3);            
			} else {      
				if(!bHay){
					if(!anterior){
						a_pCad->EscribeCadena(s_unidades1[9],4);
					};
				};
				break;              
			};
		};
	};
	return 0;	
}


m4uint32_t FRA_CenDecUni(ClCadena *a_pCad, m4char_t* a_cendecuni, m4bool_t* pbHay, m4uint32_t tamgrup) 
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
   
	if(Cen>0){      
		if(*pbHay){
			//flag para indicar si hay unidades de miles o n� superior
			a_pCad->EscribeChar(' ');      
		} else {
			*pbHay=M4_TRUE;
        };
		numero=Cen*100;   
            
		if(Cen<10){  
			if(Cen!=1){
				a_pCad->EscribeCadena(s_unidades1[Cen-1]);
				a_pCad->EscribeCadena(s_decenas2[8]);
				a_pCad->EscribeCadena("s ");
			} else {
				a_pCad->EscribeCadena(s_decenas2[8]);
				a_pCad->EscribeCadena(" ");
			};
			if(Dec | Uni){            				
			} else {
				return numero;
			};
		};
	};
   
	if(Dec>0){          
		*pbHay=M4_TRUE;
		numero+=Dec*10;      
   		if(Dec==1){                    
			a_pCad->EscribeCadena(s_decenas1[Uni]);            
			return numero;            
		} else {
			if(Dec<10){            							
				a_pCad->EscribeCadena(s_decenas2[Dec-2]);
				if(Dec==8 && Uni==0) a_pCad->EscribeCadena("s");
				if ((Dec==7 || Dec==9) && Uni==0){ 
					a_pCad->EscribeCadena("-dix");
					return numero;			                           
				};
			};
		};
	};
   
	if(Uni>0){                                    		
		numero+=Uni;           		     
		if(Dec){
			a_pCad->EscribeChar('-');
        };
		*pbHay=M4_TRUE;
		if(Uni==1 && Dec>1 && Dec<8){
			a_pCad->EscribeCadena("et-");
		};
		if(Dec==7 || Dec==9){
			a_pCad->EscribeCadena(s_decenas1[Uni]);
		} else {
			a_pCad->EscribeCadena(s_unidades1[Uni-1]);
		};
	};
 
   return numero;
}

const m4char_t* FRA_GetMonth(m4uint16_t ai_iMes)
{		
	return s_meses[ai_iMes-1];
}

const m4char_t* FRA_GetDay(m4uint16_t ai_iDia)
{
	return s_dias[ai_iDia-1];
}

m4int16_t FRA_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender) 
{
	FRA_NumberToText(a_pCad, a_pNum, bGender, M4_TRUE); 
	return 1;
}


m4int16_t FRA_DecimalToText(ClCadena *a_pCad, ClCadena *a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender)
{ 
	m4bool_t bHay;
	if(!a_pNum->EsNula()){   
   
		bHay=M4_FALSE;
		a_pNum->Posiciona(0, INICIO);
		m4uint32_t anterior=0;
		m4uint32_t cifras=a_pNum->LonReal();
		m4uint32_t grupos=cifras/ai_iDecGrup;   //No llegaria hasta aqui si ai_iDecGrup <1 o ai_iDecGrup >3
		m4uint32_t resto=cifras-grupos*ai_iDecGrup;    
		m4uint32_t tamgrup=ai_iDecGrup;       //Se debe inicializar
		m4char_t *s= new m4char_t [ai_iDecGrup];
		if(resto) { //Vemos resto en ultimo grupo
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
			}
			//leemos grupo actual
			a_pNum->LeeCadena(s, tamgrup);
			//escribimos numeros
			m4char_t* p=s;
			m4uint32_t i= tamgrup;
   
			while(*p=='0') {
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
			if(i) {   //habremos acabado de recorrer el grupo?
				bHay = M4_FALSE;
			};
			a_pCad->EscribeChar(' ');

			anterior=FRA_CenDecUni(a_pCad, s, &bHay, tamgrup);  //Grupo debe escribir primero los ceros
			//otro grupo
			grupos--;
			if(!grupos){ //No hay mas grupos, hay que acabar esto
				break;
			};
		};//Fin bucle infinito   
	};
 return 0;
}

