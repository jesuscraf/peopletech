//## begin module%383E4DEC01F6.cm preserve=no
//## end module%383E4DEC01F6.cm

//## begin module%383E4DEC01F6.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
//	 Project:           M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//## end module%383E4DEC01F6.cp

//## Module: timeinterval%383E4DEC01F6; Package body
//## Subsystem: M4Date::src%3779DEFC01B7
//	d:\compon\m4date\src
//## Source file: D:\FuentesServidor\m4date\src\timeinterval.cpp

//## begin module%383E4DEC01F6.additionalIncludes preserve=no
//## end module%383E4DEC01F6.additionalIncludes

//## begin module%383E4DEC01F6.includes preserve=yes
//## end module%383E4DEC01F6.includes

// m4types
#include <m4types.hpp>
// timeinterval
#include <timeinterval.hpp>
//## begin module%383E4DEC01F6.declarations preserve=no
//## end module%383E4DEC01F6.declarations

//## begin module%383E4DEC01F6.additionalDeclarations preserve=yes
const m4uint32_t NumSecondsInDay=86400;
const m4uint32_t NumSecondsInHour=3600;
//## end module%383E4DEC01F6.additionalDeclarations


// Class ClTimeInterval 



//## Other Operations (implementation)
m4return_t ClTimeInterval::Sec2StrFormat (m4uint32_t ai_uiSec, M4ClString& ao_strFormat)
{
  //## begin ClTimeInterval::Sec2StrFormat%943607441.body preserve=yes
	m4char_t TempValue[25];
	m4uint32_t uiMinutos, uiHoras, uiSegundos, uiDias;
	uiSegundos = ai_uiSec;

	uiMinutos = uiSegundos / 60;
	uiSegundos = uiSegundos % 60;

	uiHoras = uiMinutos / 60;
	uiMinutos = uiMinutos % 60;

	uiDias = uiHoras / 24;
	uiHoras = uiHoras % 24;

	ao_strFormat="";
	if (uiDias) {
		sprintf(TempValue, "%d d ", uiDias);
		ao_strFormat = TempValue;
	}
	if (uiHoras) {
		sprintf(TempValue,"%d h ", uiHoras);
		ao_strFormat += TempValue;
	}
	if (uiMinutos) {
		sprintf(TempValue,"%d min ", uiMinutos);
		ao_strFormat += TempValue;
	}
	if (uiSegundos) {
		sprintf(TempValue,"%d sec", uiSegundos);
		ao_strFormat += TempValue;
	}
	if (ao_strFormat.empty())
		ao_strFormat = "0 sec";

	return M4_SUCCESS;
  //## end ClTimeInterval::Sec2StrFormat%943607441.body
}

m4return_t ClTimeInterval::StrFormat2Sec ( const M4ClString& ai_strFormat, m4uint32_t& ao_uiSec)
{
  //## begin ClTimeInterval::StrFormat2Sec%943607442.body preserve=yes
	m4uint32_t level=5; //nos indica si estamos leyendo dias,hora... para que se meta en orden
	m4return_t ret = M4_SUCCESS;
	m4char_t Numero[25]="";
	m4uint32_t indiceNumero=0;
	m4uint32_t uiSeconds=0;
	m4uint32_t uiLenCad = (m4uint32_t) ai_strFormat.size();

	if (uiLenCad == 0)
		return M4_ERROR;

	for (m4uint32_t i=0; i<uiLenCad && ret==M4_SUCCESS; i++) {
		switch (tolower(ai_strFormat[i])) {
			case ' ':
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				//Tenemos que leer un valor numérico.
				Numero[indiceNumero]=ai_strFormat[i];
				indiceNumero++;
				Numero[indiceNumero]='\0';
				//Comprobamos el límite, de forma que el número de dígitos
				//no debe ser mayor de 4. Para evitar que de se salga de rango
				//el número de segundos.
				if (indiceNumero>4) 
					ret = M4_ERROR;
				break;
			case 'd':
				//Comprobamos que tenemos que leer dias, el orden debe ser
				//dias, horas....
				if (level>=5 && Numero[0]!='\0') {
					//Hemos leido antes el valor de dias.
					uiSeconds+= (atol(Numero) * NumSecondsInDay);
					Numero[0]='\0';
					indiceNumero=0;
					level=4;
				}
				else
					ret=M4_ERROR;
				break;
			case 'h':
				//Comprobamos que tenemos que leer horas, el orden debe ser
				//dias, horas....
				if (level>=4 && Numero[0]!='\0') {
					//Hemos leido antes el valor de horas.
					uiSeconds+= (atol(Numero) * NumSecondsInHour);
					Numero[0]='\0';
					indiceNumero=0;
					level=3;
				}
				else
					ret=M4_ERROR;
				break;
			case 'm':
				//Comprobamos que tenemos que leer horas, el orden debe ser
				//dias, horas....
				if (level>=3 && Numero[0]!='\0') {
					//Comprobamos que luego viene una 'i' y una 'n' (min)
					if (i+2<uiLenCad) {
						if ((tolower(ai_strFormat[i+1]) == 'i') && 
							(tolower(ai_strFormat[i+2])=='n')) {
							//Hemos leido antes el valor de minutos.
							uiSeconds+= (atol(Numero) * 60);
							indiceNumero=0;
							Numero[0]='\0';
							level=2;
							i+=2;
						}
						else
							ret = M4_ERROR;
					}
					else
						ret = M4_ERROR;
				}
				else
					ret=M4_ERROR;
				break;
			case 's':
				//Comprobamos que tenemos que leer horas, el orden debe ser
				//dias, horas....
				if (level>=2 && Numero[0]!='\0') {
					//Comprobamos que luego viene una 'e' y una 'c' (sec)
					if (i+2<uiLenCad) {
						if ((tolower(ai_strFormat[i+1]) == 'e') && 
							(tolower(ai_strFormat[i+2])=='c')) {
							//Hemos leido antes el valor de segundos.
							uiSeconds+= atol(Numero);
							indiceNumero=0;
							Numero[0]='\0';
							level=1;
							i+=2;
						}
						else
							ret=M4_ERROR;
					}
					else
						ret=M4_ERROR;
				}
				else
					ret=M4_ERROR;
				break;
			default:
				ret = M4_ERROR;
		};
	}

	//Comprobamos que se ha pasado un número + un tipo (d, sec, ....)
	if (Numero[0]!='\0')
		return M4_ERROR;

	if (ret==M4_SUCCESS)
		ao_uiSec = uiSeconds;

	return ret;
  //## end ClTimeInterval::StrFormat2Sec%943607442.body
}

// Additional Declarations
  //## begin ClTimeInterval%383D4CE701B6.declarations preserve=yes
  //## end ClTimeInterval%383D4CE701B6.declarations

//## begin module%383E4DEC01F6.epilog preserve=yes
//## end module%383E4DEC01F6.epilog
