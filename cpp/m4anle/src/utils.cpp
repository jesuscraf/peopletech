// Rutinas de apoyo utiles

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "utils.h"

//CAMBIOS PARA VERSION 3

// Elimina de una cadena los espacios en blanco
char * CleanSp(char *des, LPCSTR src)
{
size_t sz,i,n;

	
	sz = strlen(src);
	
	n = 0;
	for(i=0; i<sz; i++)
		{
		if(src[i] != ' ')
			{
			des[n] = src[i];
			n++;
			}
		}

	des[n] = 0;

	return des;
}

// Elimina de una cadena los caracteres chr
char * CleanChr(char *str, char chr)
{
size_t sz,i,n;
char *aux;
	
	sz = strlen(str);
	aux = (char *)malloc(sz+1);
	strcpy(aux,str);

	n = 0;
	for(i=0; i<sz; i++)
		{
		if(str[i] != chr)
			{
			aux[n] = str[i];
			n++;
			}
		}

	aux[n] = 0;

	strcpy(str,aux);
	LIBERA(aux);

	return str;
}

//Sustituye las subcadenas old que se encuentren en la cadena str por la cadena chg
char * ChgStr(char **des, LPCSTR str, LPCSTR old, LPCSTR chg)
{
BOOL salir = FALSE;
char *pstr;
char *aux1,*aux2;
size_t sz1,sz2;

	sz1 = strlen(str);
	*des = (char*)malloc(sz1+1);
	if(!*des)
		return 0;
	strcpy(*des, str);

	sz1 = strlen(old);

	while(!salir)
		{
		pstr = strstr(*des, old);
		if(!pstr)
			salir = TRUE;
		else
			{
			sz2 = strlen(*des) - (pstr-(*des)+sz1);
			aux1 = (char*)malloc(sz2+1);
			if(!aux1)
				return 0;
			strcpy(aux1, pstr+sz1);
			
			pstr[0] = 0;
			aux2 = (char*)malloc(strlen(*des)+1);
			if(!aux2)
				return 0;
			strcpy(aux2, *des);

			LIBERA(*des);
			*des = (char*)malloc(strlen(aux1)+strlen(chg)+strlen(aux2)+1);
			if(!des)
				return 0;
			strcpy(*des,aux2);
			strcat(*des,chg);
			strcat(*des,aux1);
			LIBERA(aux1);
			LIBERA(aux2);
			}
		}
			
	return *des;
}


//Busca dentro de la cadena str el primer caracter c que se encuentre fuera de un paréntesis
char * FindChr(char *str, char c)
{
size_t i,sz;
char *ret=NULL;
int n;

	sz = strlen(str);

	n = 0;
	for(i=0; i<sz; i++)
		{
		if((str[i] == c) && (!n))
			{
			ret = str + i;
			break;
			}
		else
		if(str[i] == 40)
			n++;
		else
		if(str[i] == 41)
			n--;
		}

	return ret;
}

//Busca dentro de la cadena str el primer caracter c que se encuentre fuera de " "
char * FindChr2(char *str, char c)
{
size_t i,sz;
char *ret=NULL;
int n;

	sz = strlen(str);

	n = 0;
	for(i=0; i<sz; i++)
		{
		if((str[i] == c) && ((n%2)==0))
			{
			ret = str + i;
			break;
			}
		else
		if(str[i] == 34)
			n++;
		}

	return ret;
}


//Busca dentro de la cadena str el primer caracter c que se encuentre fuera de un paréntesis y de un " "
char * FindChr3(char *str, char c)
{
size_t i,sz;
char *ret=NULL;
int n;
int m;

	sz = strlen(str);

	n = 0;
	m = 0;
	for(i=0; i<sz; i++)
		{
		if((str[i] == c) && (!n) && ((m%2)==0))
			{
			ret = str + i;
			break;
			}
		else
		if(str[i] == 40)
			n++;
		else
		if(str[i] == 41)
			n--;
		else
		if(str[i] == 34)
			m++;
		}

	return ret;
}
	
//Busca dentro de la cadena str la primera subcadena sub que se encuentre fuera de un paréntesis
char * FindStr(char *str, LPCSTR sub)
{
size_t i,sz;
char *ret=NULL;
int n,m;
size_t tam;

	sz = strlen(str);
	tam = strlen(sub);

	n = 0;
	m = 0;
	for(i=0; i<sz; i++)
		{
		if(str[i] == 40)
			n++;
		else
		if(str[i] == 41)
			n--;
		else
		if(!n)
			{
			if(str[i] == sub[m])
				{
				if(m == 0)
					ret = str + i;
				m++;
				if(m == tam)
					break;
				}
			else
				{
				ret = str + i;
				m = 0;
				}
			}
		}

	if(m < tam)
		ret = 0;

	return ret;

}

//Busca dentro de la cadena str la primera subcadena sub que se encuentre fuera de un " "
char * FindStr2(char *str, LPCSTR sub)
{
size_t i,sz;
char *ret=NULL;
int n,m;
size_t tam;

	sz = strlen(str);
	tam = strlen(sub);

	n = 0;
	m = 0;
	for(i=0; i<sz; i++)
		{
		if(str[i] == 34)
			n++;
		else
		if((n % 2) == 0)
			{
			if(str[i] == sub[m])
				{
				if(m == 0)
					ret = str + i;
				m++;
				if(m == tam)
					break;
				}
			else
				{
				ret = str + i;
				m = 0;
				}
			}
		}

	if(m < tam)
		ret = 0;

	return ret;

}

//Busca dentro de la cadena str la primera subcadena sub que se encuentre fuera de un paréntesis y de un " "
char * FindStr3(char *str, LPCSTR sub)
{
size_t i,sz;
char *ret=NULL;
int n,m;
size_t tam;
int sp;

	sz = strlen(str);
	tam = strlen(sub);

	n = 0;
	m = 0;
	sp = 0;
	for(i=0; i<sz; i++)
		{
		if(str[i] == 40)
			n++;
		else
		if(str[i] == 41)
			n--;
		else
		if(str[i] == 34)
			sp++;
		else
		if((!n) && ((sp % 2) == 0))
			{
			if(str[i] == sub[m])
				{
				if(m == 0)
					ret = str + i;
				m++;
				if(m == tam)
					break;
				}
			else
				{
				ret = str + i;
				m = 0;
				}
			}
		}

	if(m < tam)
		ret = 0;

	return ret;

}

/*Entrega la cadena que va desde la int1-enesima ocurrencia de substr1 hasta antes de la
int2-esima ocurrencia de de substr2*/
char *GetStringFromTo(char **des, LPSTR org, LPCSTR substr1, int int1, LPCSTR substr2, int int2)
{
char *ini=NULL;
char *fin=NULL;
size_t sz;
int n;

	*des = 0;	

	if(substr1[0] == 0)
		ini = org;
	if(substr2[0] == 0)
		fin = org + strlen(org);

	if(!ini)
		{
		if((n = StrCount(org, substr1)) == 0)
			return NULL;
		if((int1 > 0) && (int1 <= n))
			ini = FindStrPos(org, substr1, int1);
		else
		if((int1 < 0) && (int1 >= (-n)))
			ini = FindStrPos(org, substr1, n + int1 + 1);
		else
			return NULL;
		}

	if(!fin)
		{
		if((n = StrCount(org, substr2)) == 0)
			return NULL;
		if((int2 > 0) && (int2 <= n))
			fin = FindStrPos(org, substr2, int2);
		else
		if((int2 < 0) && (int2 >= (-n)))
			fin = FindStrPos(org, substr2, n + int2 + 1);
		else
			return NULL;
		}

	
	if((!ini) || (!fin))
		return NULL;

	ini = ini + strlen(substr1);
	fin = fin - 1;

	sz = (fin - ini) + 1;
	if(sz <= 0)
		return NULL;

	*des = (char *)malloc(sz+1);
	if(!*des)
		return NULL;
	memcpy(*des, ini, sz);
	(*des)[sz] = 0;

	return *des;
}


/*Entrega la lista de cadenas que va desde la int1-enesima ocurrencia de substr1 hasta antes de la
int2-esima ocurrencia de de substr2*/
char *GetListStringFromTo(char **des, LPSTR org, LPCSTR substr1, int int1, LPCSTR substr2, int int2)
{
char *aux=NULL;
char *ini = NULL;
char *fin = NULL;
size_t sz;

	GetStringFromTo(&aux, org, substr1, int1, substr2, int2);

	if(!aux)
		return NULL;

	ini = aux;
	fin = aux + strlen(aux) - 1;

	if(ini[0] == ',') ini++;
	if(fin[0] == ',') fin--;

	sz = (fin - ini) + 1;
	if(sz <= 0)
		return NULL;

	*des = (char *)malloc(sz+1);
	if(!*des)
		return NULL;
	memcpy(*des, ini, sz);
	*des[sz] = 0;

	LIBERA(aux);

	return *des;
}


//Devuelve el número de veces que se encuentra contenida la cadena substr en str
int StrCount(LPCSTR str, LPCSTR substr)
{
int veces = 0;
const char *p=NULL;

	p = strstr(str, substr);
	while(p)
		{
		veces++;
		p = strstr(p+1, substr);
		}

	return veces;
}


//Devuelve la dirección de la n-enesima ocurrencia de substr dentro de str
char *FindStrPos(LPCSTR str, LPCSTR substr, int n)
{
int i;
char *p = NULL;
char *p2;

	p = (char *)str;
	p2 = p;
	for(i = 0; i<n; i++)
		{
		p = strstr(p2, substr);
		p2 = p+1;
		if(!p)
			return NULL;
		}

	return p;
}

//Devuelve la dirección del a última ocurrencia de substr dentro de str
char *FindLast(LPCSTR str, LPCSTR substr)
{
const char *p = NULL;
const char *p1 = NULL;

	p = strstr(str, substr);

	while(p)
		{
		p1 = p;
		p = strstr(p+1, substr);
		}

	return (char*)p1;
}

void Trace(LPCSTR mensaje, LPCSTR dump)
{
#ifdef __GENERA_LOG__
FILE *pfile = NULL;

	// UNICODE FILE no se puede por las stl
	pfile = fopen("anle.log","a");
	if(!pfile)
		return;

	fprintf(pfile,"\n%s %s\n", mensaje, dump);

	fclose(pfile);
#endif
}
