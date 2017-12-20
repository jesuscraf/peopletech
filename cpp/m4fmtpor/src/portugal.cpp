//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             portugal.cpp
// Project:			 M4fmtpor.CPP
// Author:           Meta Software M.S. , S.A
// Date:			 09/07/2002 
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//
//==============================================================================

#include "portugal.h"
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////

static const char* s_xllones[]=
{        
   " mil",
   "milhão",
   "bilião",
   "trillón",
   "cuatrillón",
   "quintillón"
};

static const char* s_centenas[]=
{                 
   "duzent",
   "trezent",
   "quatrocent",
   "quinhent",
   "seiscent",
   "setecent",
   "oitocent",
   "novecent"
};


static const char* s_decenas1[]=
{ 
   "dez",
   "onze",
   "doze",
   "treze",
   "catorze",
   "quinze",
   "dezesseis",
   "dezessete",
   "dezoito",
   "dezenove"   
};

static const char* s_decenas2[]=
{    
   "deza",
   "vinte",
   "trinta",
   "quarenta",
   "cinquenta",
   "sessenta",
   "setenta",
   "oitenta",
   "noventa"
   
};

static const char* s_unidades1[]=
{     
   "dois",
   "três",
   "quatro",
   "cinco",
   "seis",
   "sete",
   "oito",
   "nove",
   "zero"
};

static const char* s_meses[]=
{
	"Janeiro",
    "Fevereiro",
    "Março",
    "Abril",
    "Maio",
    "Junho",
    "Julho",
    "Agosto",
    "Setembro",
    "Outubro",
    "Novembro",
    "Dezembro"
};

static const char* s_dias[]=
{
    "Segunda",
    "Terça",
    "Quarta",
    "Quinta",
    "Sexta",
	"Sabado",
    "Domingo"
};

///////////////////////////////////////////////////////////////////////

const StPlugIn POR = {
    &POR_NumberToText,
	&POR_DecimalToText,
	&POR_YearToText,
	&POR_GetMonth,  
	&POR_GetDay
};

void AttachNewLanguage(const StPlugIn** ai_stInfo, char* ai_pcLanguage)
{
	*ai_stInfo = &POR;
	strcpy(ai_pcLanguage, "PO");
	ai_pcLanguage[2]='\0';

};

////////////////////////////////////////////////////////////////////////////////////

short POR_NumberToText(ClCadena *a_pCad, ClCadena *a_pNum, bool bGender, bool bIsDate)
{
	bool bHay; 

	if(!a_pNum->EsNula()){   
		char buf[256];   
		unsigned long anterior=0;
		bHay=false;
		a_pNum->Posiciona(0, INICIO);
		unsigned short cifras=a_pNum->LonReal();
		if(cifras>18){                  
			unsigned short l=cifras-18;
			a_pNum->LeeCadena(buf, l);
			a_pCad->EscribeCadena(buf, l);
			cifras=18; 
			buf[l]='\0';
			anterior=atoi(buf);     
			if(anterior){
				a_pCad->EscribeChar(' ');
				bHay=true;
				a_pCad->EscribeCadena(s_xllones[3], strlen(s_xllones[3]));
			};
		};
         
		unsigned short nmill=cifras/6;
		unsigned short resto=cifras-nmill*6;    
		unsigned short k;
		if(resto){
			nmill++;
			k=resto;
		} else {
			k=6;
		};
		char millones[]="000000";
		a_pNum->LeeCadena(millones+6-k, k);

		for(;;){   
			/*if(nmill==1){ 
				unsigned long numero=POR_CenDecUni(a_pCad,millones,&bHay, bGender);
				if(numero){
					a_pCad->EscribeCadena(s_xllones[0]);
				};
				anterior=numero*1000+POR_CenDecUni(a_pCad,millones+3,&bHay, bGender);
			}*/
			//nmill--;
			//if(nmill){ 
			unsigned long numero=POR_CenDecUni(a_pCad,millones,&bHay, bGender);
			if(numero){
				a_pCad->EscribeCadena(s_xllones[0]);
			}
				
			m4uint32_t numero2=POR_CenDecUni(a_pCad, millones+3, &bHay, bGender);

			if(numero2==1){
				// se ha quedado el uno sin escribir      
				a_pCad->EscribeCadena("um");
					
			}
			anterior=numero*1000+numero2;
			nmill--;
			if(nmill){ 
				if(bHay){
					a_pCad->EscribeChar(' ');
				} else {
					bHay=true;
				}	                     
				if(anterior){
					if(anterior==1){
						a_pCad->EscribeCadena(s_xllones[nmill], strlen(s_xllones[nmill]));
					} else {
						// en plural :      
						a_pCad->EscribeCadena(s_xllones[nmill], strlen(s_xllones[nmill])-2);
						a_pCad->EscribeCadena("ões", 3);
					}
					
				}
				a_pNum->LeeCadena(millones, 6);            
			} else {       
				if(!bHay){
					if(!anterior){
						a_pCad->EscribeCadena(s_unidades1[8],4);
					}
				}
				break;              
			}
		}
	}
	return 0;	
}


unsigned long POR_CenDecUni(ClCadena *a_pCad, char* a_cendecuni, bool* pbHay, bool bGender,unsigned long tamgrup) 
{
	unsigned long numero=0L;
	char Cen, Dec, Uni;
   
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
		if(*pbHay) {
			//flag para indicar si hay unidades de miles o nº superior
			a_pCad->EscribeChar(' ');      
		} else {
			*pbHay=true;
		}
		
		numero=Cen*100;   
		
		if(Cen==1){                                                     
			if(Dec | Uni){
				a_pCad->EscribeCadena("cento");            
			}                            
			else{
				a_pCad->EscribeCadena("cem");            
				return numero;
			}   
		}
		else{
			if(Cen<10){     
				a_pCad->EscribeCadena(s_centenas[Cen-2]);
				a_pCad->EscribeCadena( "os");            
			}
			else
				;// error
		}      
		
	
	}
   
	if(Dec>0){          
		if(*pbHay){	  
			if ((tamgrup != 2) || (Cen>0)){
				a_pCad->EscribeCadena(" e ");                     
			}	  
		} else {
			*pbHay=true;
        };
		numero+=Dec*10;      
                  
		if(Dec==1){                    
			a_pCad->EscribeCadena(s_decenas1[Uni]);            
			return numero;            
		} else {
			if(Dec<10){            							
				a_pCad->EscribeCadena(s_decenas2[Dec-1]);
				if (Uni==0) {
					return numero;
				};
			};
		};
	};
   
	if(Uni>0){
		
		numero+=Uni;    		     
		
		if(*pbHay){
			if ((tamgrup != 1) && (tamgrup != 2) || (Dec>0) || (Cen>0)){
				a_pCad->EscribeCadena(" e ");                     
			}
		}
		*pbHay=true;
		if(Uni==1){                                                      
			if(numero!=1){
				a_pCad->EscribeCadena("um");            
			}
		}
		else a_pCad->EscribeCadena(s_unidades1[Uni-2]);                  
		
	}
   
   return numero;
}


const char* POR_GetMonth(unsigned short ai_iMes)
{		
	return s_meses[ai_iMes-1];
}

const char* POR_GetDay(unsigned short ai_iDia)
{
	return s_dias[ai_iDia-1];
}

short POR_YearToText(ClCadena *a_pCad, ClCadena *a_pNum, bool bGender) 
{
	POR_NumberToText(a_pCad, a_pNum, bGender, 1); 
		
	return 1;
}


short POR_DecimalToText(ClCadena *a_pCad, ClCadena *a_pNum, unsigned long ai_iDecGrup, bool bGender)
{ 
	bool bHay;
	if(!a_pNum->EsNula()){   
		bHay=true;
		a_pNum->Posiciona(0, INICIO);
		unsigned long anterior=0;
		unsigned long cifras=a_pNum->LonReal();
		unsigned long grupos=cifras/ai_iDecGrup;   //No llegaria hasta aqui si ai_iDecGrup <1 o ai_iDecGrup >3
		unsigned long resto=cifras-grupos*ai_iDecGrup;    
		unsigned long tamgrup=ai_iDecGrup;       //Se debe inicializar
		char *s= new char [ai_iDecGrup];

		if(resto) { //Vemos resto en ultimo grupo
			grupos++;
		};
		for(;;){   
			if(grupos==1)  //grupos indica el grupo actual
			{
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
			char* p= s;
			unsigned long i= tamgrup;
   
			while(*p=='0')
			{
				if(bHay){
   	  				a_pCad->EscribeCadena(" ", 1);
				};
   				a_pCad->EscribeCadena(s_unidades1[8], 4);
				bHay=true;   
				if(!(--i)){
      				break;
				};
   				p++;
			};
			if(i){    //habremos acabado de recorrer el grupo?
				bHay = true;
			};
			a_pCad->EscribeChar(' ');
			anterior=POR_CenDecUni(a_pCad, s, &bHay, bGender, tamgrup);  //Grupo debe escribir primero los ceros
			if(anterior==1){
				// se ha quedado el uno sin escribir      
   				a_pCad->EscribeCadena(" um");
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

