// Implementación de las clases que dan soporte a los operadores

#include "windows.h"
#include "m4define.hpp"
#include "globdef.h"
#include "operador.h"
#include "macros.h"
#include "utils.h"

/*-----------------------------------------------------------
Clase COperador
-----------------------------------------------------------*/

COperador::COperador()
{
}

COperador::~COperador()
{
}

void COperador::SetParam1(char *par)
{
	return;
}

void COperador::SetParam2(char *par)
{
	return;
}

void COperador::SetParam3(char *par)
{
	return;
}

void COperador::SetParam4(char *par)
{
	return;
}

void COperador::SetParam5(char *par)
{
	return;
}

/*-----------------------------------------------------------
Clase COLen
-----------------------------------------------------------*/

COLen::COLen()
{
	m_str = NULL;
}

COLen::~COLen()
{
	LIBERA(m_str);
}

void COLen::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);

}

int COLen::Opera(char **res)
{
long len,sz;
char aux[20];

	*res = NULL;

	len = (long) strlen(m_str);

	ltoa(len, aux, 10);
	sz = (long) strlen(aux);

	*res = (char*)malloc(sz+1);
	if(!res)
		return -1;

	strcpy(*res, aux);

	return sz;
}

/*-----------------------------------------------------------
Clase COPos
-----------------------------------------------------------*/
COPos::COPos()
{
	m_str = NULL;
	m_substr = NULL;
}

COPos::~COPos()
{
	LIBERA(m_str);
	LIBERA(m_substr);
}

void COPos::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_substr = (char *)malloc(sz+1);

	if(!m_substr)
		return;

	strcpy(m_substr, par);
	
}

void COPos::SetParam2(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

int COPos::Opera(char **res)
{
long n;
long sz;
char *pos = NULL; 	
char aux[20];

	*res = NULL;

	pos = strstr(m_str, m_substr);
	n = (long) (pos - m_str);

	ltoa(n, aux, 10);
	sz = (long) strlen(aux);

	*res = (char*)malloc(sz+1);
	if(!*res)
		return -1;

	strcpy(*res, aux);

	return sz;
}

/*-----------------------------------------------------------
Clase COUpper
-----------------------------------------------------------*/
COUpper::COUpper()
{
	m_str = NULL;
}

COUpper::~COUpper()
{
	LIBERA(m_str);
}

void COUpper::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

int COUpper::Opera(char **res)
{
int sz;

	*res = NULL;

	sz = (int) strlen(m_str);

	*res = (char*)malloc(sz+1);
	if(!res)
		return -1;

	strupr(m_str);
	strcpy(*res, m_str);

	return sz;
}

/*-----------------------------------------------------------
Clase COLower
-----------------------------------------------------------*/
COLower::COLower()
{
	m_str = NULL;
}

COLower::~COLower()
{
	LIBERA(m_str);
}

void COLower::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

int COLower::Opera(char **res)
{
int sz;

	*res = NULL;

	sz = (int) strlen(m_str);

	*res = (char*)malloc(sz+1);
	if(!res)
		return -1;

	strlwr(m_str);
	strcpy(*res, m_str);

	return sz;
}

/*-----------------------------------------------------------
Clase COFupper
-----------------------------------------------------------*/
COFupper::COFupper()
{
	m_str = NULL;
}

COFupper::~COFupper()
{
	LIBERA(m_str);
}

void COFupper::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

int COFupper::Opera(char **res)
{
int sz;

	*res = NULL;

	sz = (int) strlen(m_str);

	*res = (char*)malloc(sz+1);
	if(!res)
		return -1;

	strupr(m_str);
	strlwr(m_str+1);
	strcpy(*res, m_str);

	return sz;
}

/*-----------------------------------------------------------
Clase COLeft
-----------------------------------------------------------*/
COLeft::COLeft()
{
	m_str = NULL;
	m_pos = 0;
}

COLeft::~COLeft()
{
	LIBERA(m_str);
}

void COLeft::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

void COLeft::SetParam2(char *par)
{

	m_pos = atol(par);

}

int COLeft::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if((m_pos > (long)strlen(m_str)) || (m_pos < 1))
		return -1;

	(*res)[0] = m_str[m_pos-1];
	(*res)[1] = 0;

	return 1;
}

/*-----------------------------------------------------------
Clase CORight
-----------------------------------------------------------*/
CORight::CORight()
{
	m_str = NULL;
	m_pos = 0;
}

CORight::~CORight()
{
	LIBERA(m_str);
}

void CORight::SetParam1(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

void CORight::SetParam2(char *par)
{
	m_pos = atol(par);

}

int CORight::Opera(char **res)
{
long sz;

	*res = NULL;

	*res = (char*)malloc(2);
	if(!res)
		return -1;

	sz = (long) strlen(m_str);
	if(m_pos >= sz)
		return -1;

	(*res)[0] = m_str[sz - 1 - m_pos];
	(*res)[1] = 0;

	return 1;
}

/*-----------------------------------------------------------
Clase COMid
-----------------------------------------------------------*/
COMid::COMid()
{
	m_str = NULL;
	m_pos1 = 0;
	m_pos2 = 0;
}

COMid::~COMid()
{
	LIBERA(m_str);
}

void COMid::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

void COMid::SetParam2(char *par)
{

	m_pos1 = atol(par);

}

void COMid::SetParam3(char *par)
{
	m_pos2 = atol(par);

}

int COMid::Opera(char **res)
{
int sz;

	*res = NULL;

	if((m_pos1 > (long)strlen(m_str)) || (m_pos1 < 1))
		{
		*res = (char*)malloc(1);
		if(!*res)
			return -1;
		(*res)[0] = 0;
		return 0;
		}

	if(m_pos2 <= m_pos1)
		{
		*res = (char*)malloc(1);
		if(!*res)
			return -1;
		(*res)[0] = 0;
		return 0;
		}

	if(m_pos2 > (long)strlen(m_str))
		m_pos2 = (long)strlen(m_str);

	sz = (m_pos2 - m_pos1) + 1;

	*res = (char*)malloc(sz+1);
	if(!*res)
		return -1;

	memcpy(*res, m_str + m_pos1 - 1, sz);
	(*res)[sz] = 0;

	return sz;
}

/*-----------------------------------------------------------
Clase COTraductString
-----------------------------------------------------------*/
COTraductString::COTraductString()
{
	m_str = NULL;
	m_cad1 = NULL;
	m_cad2 = NULL;
	m_cad3 = NULL;
}

COTraductString::~COTraductString()
{
	LIBERA(m_str);
	LIBERA(m_cad1);
	LIBERA(m_cad2);
	LIBERA(m_cad3);
}

void COTraductString::SetParam1(char *par)
{
size_t sz;

	sz = strlen(par);
	m_str = (char *)malloc(sz+1);

	if(!m_str)
		return;

	strcpy(m_str, par);
	
}

void COTraductString::SetParam2(char *par)
{
size_t sz;

	sz = strlen(par);
	m_cad1 = (char *)malloc(sz+1);

	if(!m_cad1)
		return;

	strcpy(m_cad1, par);
	
}

void COTraductString::SetParam3(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_cad2 = (char *)malloc(sz+1);

	if(!m_cad2)
		return;

	strcpy(m_cad2, par);
	
}

void COTraductString::SetParam4(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_cad3 = (char *)malloc(sz+1);

	if(!m_cad3)
		return;

	strcpy(m_cad3, par);
	
}

int COTraductString::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(!strstr(m_str, DOBLE))
		(*res)[0] = '0';
	else
	if((strstr(m_str, m_cad1)) || (strstr(m_str, m_cad2)) || (strstr(m_str, m_cad3)))
		(*res)[0] = '2';
	else
		(*res)[0] = '1';

	(*res)[1] = 0;

	return 1;
}

/*-----------------------------------------------------------
Clase COIn
-----------------------------------------------------------*/
COIn::COIn()
{
	m_val = NULL;
	m_grupo = NULL;
}

COIn::~COIn()
{
	LIBERA(m_val);
	LIBERA(m_grupo);
}

void COIn::SetParam1(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_val = (char *)malloc(sz+1);

	if(!m_val)
		return;

	CleanSp(m_val, par);
	
}

void COIn::SetParam2(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_grupo = (char *)malloc(sz+1);

	if(!m_grupo)
		return;

	CleanSp(m_grupo, par);
	
}

int COIn::Opera(char **res)
{
size_t sz;
BOOL intervalo = FALSE;
char *pini = NULL, *pfin = NULL;
char *aux = NULL;
BOOL salir,ret;
long val,limsup, liminf;

	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(strlen(m_val) == 0)//Es una cadena ""
		{
		(*res)[0] = '0';
		(*res)[1] = 0;
		return 1;
		}

	//Si se encuentra el caracter '-' => se busca en un intervalo no en un grupo
	if((pini = strchr(m_grupo, '-')))
		{
		intervalo = TRUE; 
		// Se calculan los limites superior e inferior del intervalo
		sz = pini - m_grupo;
		aux = (char*)malloc(sz+1);
		if(!aux) return -1;
		memcpy(aux, m_grupo, sz);
		aux[sz] = 0;
		liminf = atol(aux);
		LIBERA(aux);

		sz = (m_grupo + strlen(m_grupo)) - (pini+1);
		aux = (char*)malloc(sz+1);
		if(!aux) return -1;
		memcpy(aux, (pini+1), sz);
		aux[sz] = 0;
		limsup = atol(aux);
		LIBERA(aux);
		}
	
	ret = TRUE;
	pini = m_val;
	salir = FALSE;
	while(!salir)
		{
		pfin = strchr(pini, ',');
		if(!pfin)
			{
			pfin = pini + strlen(pini);
			salir = TRUE;
			}
		
		sz = pfin-pini;
		aux = (char*)malloc(sz+1);
		if(!aux) 
			return -1;
		memcpy(aux, pini, sz);
		aux[sz] = 0;
		pini = pfin + 1;

		if(!intervalo) // Comparo con un grupo
			{
			if(!strstr(m_grupo, aux))
				{
				ret = FALSE;
				LIBERA(aux);
				break;
				}
			}
		else // Comparo con un intervalo
			{
			val = atol(aux);
			if((val < liminf) || (val > limsup))
				{
				ret = FALSE;
				LIBERA(aux);
				break;
				}
			}

		LIBERA(aux);
		}

	if(ret)
		(*res)[0] = '1';
	else
		(*res)[0] = '0';

	(*res)[1] = 0;

	return 1;
}

/*-----------------------------------------------------------
Clase COMayor
-----------------------------------------------------------*/
COMayor::COMayor()
{
	m_par1 = 0;
	m_par2 = 0;
}

COMayor::~COMayor()
{
}

void COMayor::SetParam1(char *par)
{
	m_par1 = atol(par);

}

void COMayor::SetParam2(char *par)
{
	m_par2 = atol(par);

}

int COMayor::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(m_par1 > m_par2)
		(*res)[0] = '1';
	else
		(*res)[0] = '0';

	(*res)[1]=0;

	return 1;
}

/*-----------------------------------------------------------
Clase COMenor
-----------------------------------------------------------*/
COMenor::COMenor()
{
	m_par1 = 0;
	m_par2 = 0;
}

COMenor::~COMenor()
{
}

void COMenor::SetParam1(char *par)
{
	m_par1 = atol(par);

}

void COMenor::SetParam2(char *par)
{
	m_par2 = atol(par);

}

int COMenor::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(m_par1 < m_par2)
		(*res)[0] = '1';
	else
		(*res)[0] = '0';

	(*res)[1]=0;

	return 1;
}

/*-----------------------------------------------------------
Clase COMayorIgual
-----------------------------------------------------------*/
COMayorIgual::COMayorIgual()
{
	m_par1 = 0;
	m_par2 = 0;
}

COMayorIgual::~COMayorIgual()
{
}

void COMayorIgual::SetParam1(char *par)
{
	m_par1 = atol(par);

}

void COMayorIgual::SetParam2(char *par)
{
	m_par2 = atol(par);

}

int COMayorIgual::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(m_par1 >= m_par2)
		(*res)[0] = '1';
	else
		(*res)[0] = '0';

	(*res)[1]=0;

	return 1;
}

/*-----------------------------------------------------------
Clase COMenorIgual
-----------------------------------------------------------*/
COMenorIgual::COMenorIgual()
{
	m_par1 = 0;
	m_par2 = 0;
}

COMenorIgual::~COMenorIgual()
{
}

void COMenorIgual::SetParam1(char *par)
{
	m_par1 = atol(par);

}

void COMenorIgual::SetParam2(char *par)
{
	m_par2 = atol(par);

}

int COMenorIgual::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(m_par1 <= m_par2)
		(*res)[0] = '1';
	else
		(*res)[0] = '0';

	(*res)[1]=0;

	return 1;
}

/*-----------------------------------------------------------
Clase COIgual
-----------------------------------------------------------*/
COIgual::COIgual()
{
	m_par1 = NULL;
	m_par2 = NULL;
}

COIgual::~COIgual()
{
	LIBERA(m_par1);
	LIBERA(m_par2);
}

void COIgual::SetParam1(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_par1 = (char *)malloc(sz+1);

	if(!m_par1)
		return;

	strcpy(m_par1, par);
	
}

void COIgual::SetParam2(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_par2 = (char *)malloc(sz+1);

	if(!m_par2)
		return;

	strcpy(m_par2, par);
	
}

int COIgual::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(!strcmp(m_par1, m_par2))
		(*res)[0] = '1';
	else
		(*res)[0] = '0';

	(*res)[1] = 0;

	return 1;
}

/*-----------------------------------------------------------
Clase CODistinto
-----------------------------------------------------------*/
CODistinto::CODistinto()
{
	m_par1 = NULL;
	m_par2 = NULL;
}

CODistinto::~CODistinto()
{
	LIBERA(m_par1);
	LIBERA(m_par2);
}

void CODistinto::SetParam1(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_par1 = (char *)malloc(sz+1);

	if(!m_par1)
		return;

	strcpy(m_par1, par);
	
}

void CODistinto::SetParam2(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_par2 = (char *)malloc(sz+1);

	if(!m_par2)
		return;

	strcpy(m_par2, par);
	
}

int CODistinto::Opera(char **res)
{
	*res = NULL;

	*res = (char*)malloc(2);
	if(!*res)
		return -1;

	if(!strcmp(m_par1, m_par2))
		(*res)[0] = '0';
	else
		(*res)[0] = '1';

	(*res)[1] = 0;

	return 1;
}


/*-----------------------------------------------------------
Clase COGetStringFromTo
-----------------------------------------------------------*/
COGetStringFromTo::COGetStringFromTo()
{
	m_par1 = NULL;
	m_par2 = NULL;
	m_par3 = 0;
	m_par4 = NULL;
	m_par5 = 0;
}

COGetStringFromTo::~COGetStringFromTo()
{
	LIBERA(m_par1);
	LIBERA(m_par2);
	LIBERA(m_par4);
}

void COGetStringFromTo::SetParam1(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_par1 = (char *)malloc(sz+1);

	if(!m_par1)
		return;

	strcpy(m_par1, par);
	
}

void COGetStringFromTo::SetParam2(char *par)
{
	size_t sz;

	sz = strlen(par);
	m_par2 = (char *)malloc(sz+1);

	if(!m_par2)
		return;

	strcpy(m_par2, par);
	
}

void COGetStringFromTo::SetParam3(char *par)
{
	m_par3 = atoi(par);
}

void COGetStringFromTo::SetParam4(char *par)
{
	size_t sz;
	
	sz = strlen(par);
	m_par4 = (char *)malloc(sz+1);

	if(!m_par4)
		return;

	strcpy(m_par4, par);
	
}

void COGetStringFromTo::SetParam5(char *par)
{
	m_par5 = atoi(par);
}

int COGetStringFromTo::Opera(char **res)
{
	*res = NULL;

	m_par1[strlen(m_par1) - 1]=0;
	m_par2[strlen(m_par2) - 1]=0;
	m_par4[strlen(m_par4) - 1]=0;
	
	GetStringFromTo(res, m_par1+1, m_par2+1, m_par3, m_par4+1, m_par5);

	if(!(*res))
		return -1;
	else
		return (int)(strlen(*res));

}


/*-----------------------------------------------------------
Clase COperando
-----------------------------------------------------------*/
COperando::COperando(char *par)
{
	size_t sz;

	m_operando = NULL;

	sz = strlen(par);
	m_operando = (char *)malloc(sz+1);

	if(!m_operando)
		return;

	strcpy(m_operando, par);
	CleanChr(m_operando,0x22);
}

COperando::~COperando()
{
	LIBERA(m_operando);
}

void COperando::SetParam1(char *par)
{
	size_t sz;

	LIBERA(m_operando);

	sz = strlen(par);
	m_operando = (char *)malloc(sz+1);

	if(!m_operando)
		return;

	strcpy(m_operando, par);
	CleanChr(m_operando,0x22);
	
}

int COperando::Opera(char **res)
{
int sz;
char *aux = NULL;

	*res = NULL;

	sz = (int) strlen(m_operando);
	aux = (char *)malloc(sz+1);

	if(!aux)
		return -1;

	strcpy(aux, m_operando);
	_strupr(aux);

	if(!strcmp(aux,VERDADERO))
		{
		*res = (char *)malloc(2);
		if(!*res)
			return -1;
		(*res)[0] = '1';
		(*res)[1] = 0;
		}
	else
	if(!strcmp(aux,FALSO))
		{
		*res = (char *)malloc(2);
		if(!*res)
			return -1;
		(*res)[0] = '0';
		(*res)[1] = 0;
		}
	else
		{
		*res = (char *)malloc(sz+1);
		if(!*res)
			return -1;
		strcpy(*res, m_operando);
		}

	LIBERA(aux);
	return sz;
}
