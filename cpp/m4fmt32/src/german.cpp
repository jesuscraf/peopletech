//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             GERMAN.CPP
// Project:          M4FMT32.DLL
// Author:           Meta Software M.S. , S.A
// Date:             21/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:  
//
// Propietary:       Manuel Lazcano Pérez
// Modifications:
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================


//===== INCLUDES =============

#include "fmt.h"
#include "fmtdf.cpp"

#include "german.hpp"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////

static const m4char_t* s_xllones[]=
{
	"tausend",
	"million",
	"billion",
	"milliarde"
};

static const m4char_t* s_decenas1[]=
{
	"zehn",
	"elf",
	"zwölf",
	"dreizehn",
	"vierzehn",
	"fünfzehn",
	"sechszehn",
	"siebzehn",
	"achtzehn",
	"neunzehn"
};

static const m4char_t* s_decenas2[]=
{
	"zwanzig",
	"dreissig",
	"vierzig",
	"fünfzig",
	"sechszig",
	"siebzig",
	"achtzig",
	"neunzig",
	"hundert"
};

static const m4char_t* s_unidades1[]=
{
	"ein",
	"zwei",
	"drei",
	"vier",
	"fünf",
	"sechs",
	"sieben",
	"acht",
	"neun",
	"null"
};

static const m4char_t* s_meses[]=
{
	"Januar",
    "Februar",
    "März",
    "April",
    "Mai",
    "Juni",
    "Juli",
    "August",
    "September",
    "Oktober",
    "November",
    "Dezember"
};

static const m4char_t* s_dias[]=
{
	"Montag",
    "Dienstag",
    "Mittwoch",
    "Donnerstag",
    "Freitag",
    "Samtag",
    "Sonntag" 
};

///////////////////////////////////////////////////////////////////////

const StPlugIn GER = {
    &GER_NumberToText,
	&GER_DecimalToText,
	&GER_YearToText,
	&GER_GetMonth,  
	&GER_GetDay
};

void AttachGERLanguage(const StPlugIn** ai_stInfo, m4char_t* ai_pcLanguage)
{
	*ai_stInfo = &GER;
	strcpy(ai_pcLanguage, "GE");
	ai_pcLanguage[2]='\0';

};

////////////////////////////////////////////////////////////////////////

m4int16_t GER_NumberToText(ClCadena *a_pCad,ClCadena *a_pNum, m4bool_t bGender, m4bool_t bIsDate)
{
	m4bool_t bHay;

	if(!a_pNum->EsNula()){		
		m4char_t buf[254];
		m4uint32_t anterior=0;
		bHay=M4_FALSE;
		a_pNum->Posiciona(0,INICIO);
		m4uint16_t cifras=a_pNum->LonReal();
		if(cifras>18){
			m4uint16_t l=cifras-18;
			a_pNum->LeeCadena(buf,l);
			a_pCad->EscribeCadena(buf,l);
			cifras=18;
			buf[l]='\0';
			anterior=atoi(buf);
			if(anterior){
				bHay=M4_TRUE;
				if(anterior==1){   //EscribeXllones
					a_pCad->EscribeCadena(s_xllones[3],strlen(s_xllones[3]));
					a_pCad->EscribeChar(' ');
				} else {
					a_pCad->EscribeCadena(s_xllones[3],strlen(s_xllones[3]));
					a_pCad->EscribeCadena("en",2);
				};
				a_pCad->EscribeChar(' ');
			};
		};
		m4uint16_t nmill=cifras/6;
		m4uint16_t resto=cifras-nmill*6;
		m4uint16_t k;
 		if(resto){
			nmill++;
			k=resto;
		} else {
			k=6;
		};
		m4char_t millones[]="000000";
		a_pNum->LeeCadena(millones+6-k,k);
		for(;;){
			if(nmill==1){	
				m4uint32_t numero=GER_CenDecUni(a_pCad,millones, &bHay);
				if(numero){
					a_pCad->EscribeCadena(s_xllones[0]);
				};
				anterior=numero*1000+GER_CenDecUni(a_pCad,millones+3, &bHay);
			};
			nmill--;
			if(nmill){
				m4uint32_t numero=GER_CenDecUni(a_pCad,millones, &bHay);
				if(numero){
					a_pCad->EscribeCadena(s_xllones[0]);
				};
				anterior=numero*1000+GER_CenDecUni(a_pCad,millones+3, &bHay);
				bHay=M4_TRUE;
				if(anterior){
					if(anterior==1){ //EscribeXllones
						a_pCad->EscribeCadena(s_xllones[nmill],strlen(s_xllones[nmill]));
						a_pCad->EscribeChar(' ');
					} else {
						a_pCad->EscribeCadena(s_xllones[nmill],strlen(s_xllones[nmill]));
						a_pCad->EscribeCadena("en",2);
					};
					a_pCad->EscribeChar(' ');
				};
				a_pNum->LeeCadena(millones,6);
			} else {
				if(!bHay) {
					if(!anterior) {
						a_pCad->EscribeCadena(s_unidades1[9],4);
					};
				};
				break;
			};
		};
	};
  return 0;
}

m4uint32_t GER_CenDecUni(ClCadena* a_pCad, m4char_t* a_cendecuni, m4bool_t* pbHay, m4uint32_t tamgrup)
{
	m4uint32_t numero=0L;
	m4char_t Cen,Dec,Uni;

	switch(tamgrup){
		default:  
			//Se agrupan de 3 en 3 por defecto
		case 3:
			Cen=*a_cendecuni-'0';
			Dec=a_cendecuni[1]-'0';
			Uni=a_cendecuni[2]-'0';
			break;
		case 2:
			Cen=0;
			Dec=*a_cendecuni-'0';
			Uni=a_cendecuni[1]-'0';
			break;
		case 1:
			Cen=0;
			Dec=0;
			Uni=*a_cendecuni-'0';
			break;
	};

	if(Cen>0){
		if(*pbHay) {
			//flag para indicar si hay unidades de miles o nº superior
			a_pCad->EscribeChar(' ');
		} else {
			*pbHay=M4_TRUE;
		};
		numero=Cen*100;

		if(Cen<10){
			a_pCad->EscribeCadena(s_unidades1[Cen-1]);
			a_pCad->EscribeCadena(s_decenas2[8]);
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
				if(Uni){
					a_pCad->EscribeCadena(s_unidades1[Uni-1]);
					a_pCad->EscribeCadena("und");
				};
				a_pCad->EscribeCadena(s_decenas2[Dec-2]);
				if(Uni==0) {
					return numero;
				};
			};
		};
	};

	if(Dec==0){
		if(Uni>0){
			numero+=Uni;
			if(!*pbHay){
				*pbHay=M4_TRUE;
			};
			a_pCad->EscribeCadena(s_unidades1[Uni-1]);
		};
	};

  return numero;
}


const m4char_t* GER_GetMonth(m4uint16_t ai_iMes)
{
	return s_meses[ai_iMes-1];
}

const m4char_t* GER_GetDay(m4uint16_t ai_iDia)
{
	return s_dias[ai_iDia-1];
}

m4int16_t GER_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, m4bool_t bGender) 
{
	m4bool_t bHay;
	m4uint16_t cifras=a_pNum->LonReal();
	if (cifras>4)
		return 0;
	if (cifras<4) {
		if (cifras==3){
			m4char_t s[]="000";
			a_pNum->LeeCadena(s+2,1);		
			bHay=M4_FALSE;
			m4uint32_t numero=GER_CenDecUni(a_pCad, s, &bHay);
			a_pCad->EscribeChar(' ');
			a_pNum->LeeCadena(s+1,2);
			bHay=M4_FALSE;
			numero=GER_CenDecUni(a_pCad, s, &bHay);
		}
	}
	else
	{
		//romper en grupos de dos
		m4char_t s[]="000";
		
		a_pNum->LeeCadena(s+1,2);		
		bHay=M4_FALSE;
		m4uint32_t numero=GER_CenDecUni(a_pCad, s, &bHay);	
		a_pCad->EscribeCadena(s_decenas2[8]);
		a_pCad->EscribeCadena(" ");		
		a_pNum->LeeCadena(s+1,2);
		bHay=M4_FALSE;
		numero=GER_CenDecUni(a_pCad, s, &bHay);

	}
	
	return 1;
}


m4int16_t GER_DecimalToText(ClCadena* a_pCad,ClCadena* a_pNum, m4uint32_t ai_iDecGrup, m4bool_t bGender)
{
	m4bool_t bHay;

	if(!a_pNum->EsNula()){
		
		bHay=M4_FALSE;
		a_pNum->Posiciona(0,INICIO);
		m4uint32_t anterior=0;
		m4uint32_t cifras=a_pNum->LonReal();
		m4uint32_t grupos=cifras/ai_iDecGrup;  
		   //No llegaria aqui si D.iDecGrup<1 || D.iDecGrup>3
		m4uint32_t resto=cifras-grupos*ai_iDecGrup;
		m4uint32_t tamgrup=ai_iDecGrup;    //Se debe inicializar
		m4char_t* s=new m4char_t [ai_iDecGrup];

		if(resto){ //Comprobamos resto en el ultimo grupo
			grupos++;
		};
		for(;;){
			if(grupos==1){
				if(resto>0 && resto<ai_iDecGrup) {
					tamgrup=resto;
				} else {
					tamgrup=ai_iDecGrup;
				};
			};

			//Leemos grupo actual
			a_pNum->LeeCadena(s,tamgrup);

			//escribimos los numeros
			m4char_t*  p=s;
			m4uint32_t i=tamgrup;

			while(*p=='0'){
				if(bHay){
					a_pCad->EscribeCadena(" ",1);
				};
				a_pCad->EscribeCadena(s_unidades1[9],4);
				bHay=M4_TRUE;
				if(!(--i)){
					break;
				};
				p++;
			};
			if(i){     //Ver si hemos acabado de recorrer el grupo
				bHay=M4_FALSE;
			};
			a_pCad->EscribeChar(' ');
			anterior=GER_CenDecUni(a_pCad, s, &bHay, tamgrup);
			//Grupo debe escribir primero los ceros

			//otro grupo
			grupos--;

			if(!grupos){
				break;
			};
		}; //del bucle for infinito
	};
  return 0;
}

