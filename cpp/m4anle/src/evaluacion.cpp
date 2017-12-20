// Implementación de la Clase CEvaluacion

#include "windows.h"
#include "globdef.h"
#include "operador.h"
#include "arbol.h"
#include "evaluacion.h"
#include "macros.h"
#include "utils.h"

/*-----------------------------------------------------------------
Constructor
-----------------------------------------------------------------*/
CEvaluacion::CEvaluacion()
{
	m_expresion = NULL;
	m_definida = FALSE;
}

CEvaluacion::~CEvaluacion()
{

	LIBERA(m_expresion);
}


/*-----------------------------------------------------------------
Define la expresión que ha de ser evaluada
-----------------------------------------------------------------*/
int CEvaluacion::Define(char *valor, char *exp)
{
size_t sz;
BOOL salir = FALSE;
char *aux = NULL;

	sz = strlen(exp);
	aux = (char*)malloc(sz+1);
	if(!aux)
		return -1; //Error de memoria

	//Se eliminan los espacion es blanco
	CleanSp(aux, exp);
	//Se sustituye Myvalue por el valor correspondiente
	ChgStr(&m_expresion, aux, VALOR, valor);
	LIBERA(aux);

	//Busqueda de operadores para rellenar arbol
	if(Busca_Operador(m_expresion)<0)
		return -1;

	m_definida = TRUE;
	return 0;
}


/*-----------------------------------------------------------------
Busca los operadores y operandos de la expresión y monta el arbol
-----------------------------------------------------------------*/
int CEvaluacion::Busca_Operador(char *str, CNodo *parent/*=NULL*/)
{
COperador *operador = NULL;
CNodo	*nodo = NULL;
char *pstr = NULL;
char *par = NULL;
size_t sz;
int find;

	// Comienza la busqueda de operadores en la cadena de entrada

	if((str[0] == 34) && (str[strlen(str)-1] == 34) && parent)
		{//La cadena comienza con " y termina por " => es un operando tipo String
		operador = new COperando(str);
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
	
		return 0;
		}

	/***BUSQUEDA DEL OPERADOR "IN"***/
	pstr = strstr(str, G_OPERADOR[ISIN]);
	if(pstr)//Se encuentra el operador In
		{
		operador = new COIn;
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
		// Busqueda del primer parametro
		sz = pstr-str;
		par = (char *)malloc(sz+1);
		if(!par)
			return -1;
		memcpy(par, str, sz);
		par[sz] = 0;
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}
		//Busqueda del segundo parametro
		LIBERA(par);
		sz = strlen(pstr) - (strlen(G_OPERADOR[ISIN])-1) -1;
		par = (char *)malloc(sz+1);
		strcpy(par, pstr + (strlen(G_OPERADOR[ISIN])-1) +1);
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}

		LIBERA(par)
		return 0;
		}

	/***BUSQUEDA DEL OPERADOR ">="***/
	pstr = FindStr3(str, G_OPERADOR[MAYOR_IGUAL]);
	if(pstr)//Se encuentra el operador >=
		{
		operador = new COMayorIgual;
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
		// Busqueda del primer parametro
		sz = pstr-str;
		par = (char *)malloc(sz+1);
		if(!par)
			return -1;
		memcpy(par, str, sz);
		par[sz] = 0;
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}
		//Busqueda del segundo parametro
		LIBERA(par);
		sz = strlen(pstr) - strlen(G_OPERADOR[MAYOR_IGUAL]);
		par = (char *)malloc(sz+1);
		strcpy(par, pstr + strlen(G_OPERADOR[MAYOR_IGUAL]));
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}

		LIBERA(par)
		return 0;
		}

	/***BUSQUEDA DEL OPERADOR "<="***/
	pstr = FindStr3(str, G_OPERADOR[MENOR_IGUAL]);
	if(pstr)//Se encuentra el operador <=
		{
		operador = new COMenorIgual;
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
		// Busqueda del primer parametro
		sz = pstr-str;
		par = (char *)malloc(sz+1);
		if(!par)
			return -1;
		memcpy(par, str, sz);
		par[sz] = 0;
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}
		//Busqueda del segundo parametro
		LIBERA(par);
		sz = strlen(pstr) - strlen(G_OPERADOR[MENOR_IGUAL]);
		par = (char *)malloc(sz+1);
		strcpy(par, pstr + strlen(G_OPERADOR[MENOR_IGUAL]));
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}

		LIBERA(par)
		return 0;
		}


	/***BUSQUEDA DEL OPERADOR "="***/
	pstr = FindStr3(str, G_OPERADOR[IGUAL]);
	if(pstr)//Se encuentra el operador =
		{
		operador = new COIgual;
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
		// Busqueda del primer parametro
		sz = pstr-str;
		par = (char *)malloc(sz+1);
		if(!par)
			return -1;
		memcpy(par, str, sz);
		par[sz] = 0;
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}
		//Busqueda del segundo parametro
		LIBERA(par);
		sz = strlen(pstr) - strlen(G_OPERADOR[IGUAL]);
		par = (char *)malloc(sz+1);
		strcpy(par, pstr + strlen(G_OPERADOR[IGUAL]));
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}

		LIBERA(par)
		return 0;
		}
	
	/***BUSQUEDA DEL OPERADOR "<>"***/
	pstr = FindStr3(str, G_OPERADOR[DISTINTO]);
	if(pstr)//Se encuentra el operador <>
		{
		operador = new CODistinto;
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
		// Busqueda del primer parametro
		sz = pstr-str;
		par = (char *)malloc(sz+1);
		if(!par)
			return -1;
		memcpy(par, str, sz);
		par[sz] = 0;
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}
		//Busqueda del segundo parametro
		LIBERA(par);
		sz = strlen(pstr) - strlen(G_OPERADOR[DISTINTO]);
		par = (char *)malloc(sz+1);
		strcpy(par, pstr + strlen(G_OPERADOR[DISTINTO]));
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}

		LIBERA(par)
		return 0;
		}
	
	/***BUSQUEDA DEL OPERADOR "<"***/
	pstr = FindStr3(str, G_OPERADOR[MENOR]);
	if(pstr)//Se encuentra el operador <
		{
		operador = new COMenor;
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
		// Busqueda del primer parametro
		sz = pstr-str;
		par = (char *)malloc(sz+1);
		if(!par)
			return -1;
		memcpy(par, str, sz);
		par[sz] = 0;
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}
		//Busqueda del segundo parametro
		LIBERA(par);
		sz = strlen(pstr) - strlen(G_OPERADOR[MENOR]);
		par = (char *)malloc(sz+1);
		strcpy(par, pstr + strlen(G_OPERADOR[MENOR]));
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}

		LIBERA(par)
		return 0;
		}
	
	/***BUSQUEDA DEL OPERADOR ">"***/
	pstr = FindStr3(str, G_OPERADOR[MAYOR]);
	if(pstr)//Se encuentra el operador >
		{
		operador = new COMayor;
		nodo = new CNodo;
		nodo->m_operador = operador;
		m_arbol.Inserta(parent, nodo);
		// Busqueda del primer parametro
		sz = pstr-str;
		par = (char *)malloc(sz+1);
		if(!par)
			return -1;
		memcpy(par, str, sz);
		par[sz] = 0;
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}
		//Busqueda del segundo parametro
		LIBERA(par);
		sz = strlen(pstr) - strlen(G_OPERADOR[MAYOR]);
		par = (char *)malloc(sz+1);
		strcpy(par, pstr + strlen(G_OPERADOR[MAYOR]));
		if(Busca_Operador(par, nodo)<0)
			{
			LIBERA(par);
			return -1;
			}

		LIBERA(par)
		return 0;
		}

	/***BUSQUEDA DEL RESTO DE OPERADORES"***/
	find = GetFirstOp(str,&pstr);
	switch(find)
		{
		case LEN://Se encuentra el operador Len
			{
			operador = new COLen;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primer parametro
			sz = strlen(pstr) - (strlen(G_OPERADOR[LEN])-1) - 1;
			par = (char *)malloc(sz+1);
			strcpy(par, pstr + (strlen(G_OPERADOR[LEN])-1) + 1);
			pstr = strrchr(par,0x29);
			if(!pstr)
				{
				LIBERA(par);
				return -1;
				}
			pstr[0] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}

			LIBERA(par);
			return 0;
			break;
			}
		case POS://Se encuentra el operador Pos
			{
			char *aux = NULL;
			operador = new COPos;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primer parametro
			pstr += (strlen(G_OPERADOR[POS])-1) + 1;
			aux = FindChr3(pstr, 44);
			if(!aux)
				return -1;
			sz = aux - pstr;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, pstr, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			//Busqueda del segundo parametro
			LIBERA(par);
			sz = strlen(aux+1) - 1;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, aux+1, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			
			LIBERA(par);
			return 0;
			break;
			}
		case UPPER://Se encuentra el operador Upper
			{
			operador = new COUpper;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primer parametro
			sz = strlen(pstr) - (strlen(G_OPERADOR[UPPER])-1) - 1;
			par = (char *)malloc(sz+1);
			strcpy(par, pstr + (strlen(G_OPERADOR[UPPER])-1) + 1);
			pstr = strrchr(par,0x29);
			if(!pstr)
				{
				LIBERA(par);
				return -1;
				}
			pstr[0] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}

			LIBERA(par);
			return 0;
			break;
			}
		case LOWER://Se encuentra el operador Lower
			{
			operador = new COLower;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primer parametro
			sz = strlen(pstr) - (strlen(G_OPERADOR[LOWER])-1) - 1;
			par = (char *)malloc(sz+1);
			strcpy(par, pstr + (strlen(G_OPERADOR[LOWER])-1) + 1);
			pstr = strrchr(par,0x29);
			if(!pstr)
				{
				LIBERA(par);
				return -1;
				}
			pstr[0] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}

			LIBERA(par);
			return 0;
			break;
			}
		case FUPPER://Se encuentra el operador Fupper
			{
			operador = new COFupper;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primer parametro
			sz = strlen(pstr) - (strlen(G_OPERADOR[FUPPER])-1) - 1;
			par = (char *)malloc(sz+1);
			strcpy(par, pstr + strlen(G_OPERADOR[FUPPER]) + 1 -1);
			pstr = strrchr(par,0x29);
			if(!pstr)
				{
				LIBERA(par);
				return -1;
				}
			pstr[0] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}

			LIBERA(par);
			return 0;
			break;
			}		
		case LEFT://Se encuentra el operador Left
			{
			char *aux = NULL;
			operador = new COLeft;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primer parametro
			pstr += (strlen(G_OPERADOR[LEFT])-1) + 1;
			aux = FindChr3(pstr, 44);
			if(!aux)
				return -1;
			sz = aux - pstr;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, pstr, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			//Busqueda del segundo parametro
			LIBERA(par);
			sz = strlen(aux+1) - 1;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, aux+1, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			
			LIBERA(par);
			return 0;
			break;
			}
		case RIGHT://Se encuentra el operador Right
			{
			char *aux = NULL;
			operador = new CORight;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primer parametro
			pstr += (strlen(G_OPERADOR[RIGHT])-1) + 1;
			aux = FindChr3(pstr, 44);
			if(!aux)
				return -1;
			sz = aux - pstr;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, pstr, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			//Busqueda del segundo parametro
			LIBERA(par);
			sz = strlen(aux+1) - 1;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, aux+1, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			
			LIBERA(par);
			return 0;
			break;
			}
		case MID://Se encuentra el operador Mid
			{
			char *aux = NULL;
			int n;
			operador = new COMid;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primero y segundo parametro
			pstr += (strlen(G_OPERADOR[MID])-1) + 1;
			for(n=0; n<2; n++)
				{
				aux = FindChr3(pstr, 44);
				if(!aux)
					return -1;
				sz = aux - pstr;
				par = (char *)malloc(sz+1);
				if(!par)
					return -1;
				memcpy(par, pstr, sz);
				par[sz] = 0;
				if(Busca_Operador(par, nodo)<0)
					{
					LIBERA(par);
					return -1;
					}
			
				LIBERA(par);
				pstr = aux+1;
				}
			//Busqueda del tercer parametro
			sz = strlen(aux+1) - 1;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, aux+1, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			
			LIBERA(par);
			return 0;
			break;
			}
		case TRADUCT://Se encuentra el operador TradictString
			{
			char *aux = NULL;
			int n;
			operador = new COTraductString;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda del primero,segundo y tercer parametro
			pstr += (strlen(G_OPERADOR[TRADUCT])-1) + 1;
			for(n=0; n<3; n++)
				{
				aux = FindChr3(pstr, 44);
				if(!aux)
					return -1;
				sz = aux - pstr;
				par = (char *)malloc(sz+1);
				if(!par)
					return -1;
				memcpy(par, pstr, sz);
				par[sz] = 0;
				if(Busca_Operador(par, nodo)<0)
					{
					LIBERA(par);
					return -1;
					}
			
				LIBERA(par);
				pstr = aux+1;
				}
			//Busqueda del cuarto parametro
			sz = strlen(aux+1) - 1;
			par = (char *)malloc(sz+1);
			if(!par)
				return -1;
			memcpy(par, aux+1, sz);
			par[sz] = 0;
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(par);
				return -1;
				}
			
			LIBERA(par);
			return 0;
			break;
			}
		case GSTRFT://Se encuentra el operador GetStringFromTo
			{
			char *aux = NULL;
			char seps[] = ",";
			char *pstr2 = NULL,*pstr3 = NULL;
			operador = new COGetStringFromTo;
			nodo = new CNodo;
			nodo->m_operador = operador;
			m_arbol.Inserta(parent, nodo);
			//Busqueda de operandos
			pstr += (strlen(G_OPERADOR[GSTRFT])-1);
			pstr2 = strchr(pstr, 40);
			if(!pstr2)
				return -1;
			pstr3 = FindChr2(pstr2+1, 41);
			if(!pstr3)
				return -1;
			sz = pstr3 - pstr2;
			aux = (char *)malloc(sz);
			if(!aux)
				return -1;
			memcpy(aux, pstr2+1, sz-1);
			aux[sz-1] = 0;
			//1.
			par = strtok(aux, seps);
			if(!par)
				{
				LIBERA(aux);
				return -1;
				}
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(aux);
				return -1;
				}
			//2.
			par = strtok(NULL, seps);
			if(!par)
				{
				LIBERA(aux);
				return -1;
				}
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(aux);
				return -1;
				}
			//3.
			par = strtok(NULL, seps);
			if(!par)
				{
				LIBERA(aux);
				return -1;
				}
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(aux);
				return -1;
				}
			//4.
			par = strtok(NULL, seps);
			if(!par)
				{
				LIBERA(aux);
				return -1;
				}
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(aux);
				return -1;
				}
			//5.
			par = strtok(NULL, seps);
			if(!par)
				{
				LIBERA(aux);
				return -1;
				}
			if(Busca_Operador(par, nodo)<0)
				{
				LIBERA(aux);
				return -1;
				}

			LIBERA(aux);
			return 0;
			break;
			}

		default:
			break;
		}

	//No se encuentra ninun operador => es un operando
	operador = new COperando(str);
	nodo = new CNodo;
	nodo->m_operador = operador;
	m_arbol.Inserta(parent, nodo);

	return 0;
}

/*-----------------------------------------------------------------
Resuelve la expresión e indica si esta es correcta o no
-----------------------------------------------------------------*/
BOOL CEvaluacion::Resuelve(void)
{
	if(!m_definida)
		return FALSE;
	else
		return m_arbol.Resuelve();
}


/*-----------------------------------------------------------------
Busqueda del primer operador presenta en la expresión
-----------------------------------------------------------------*/
int CEvaluacion::GetFirstOp(char *str, char **pstr)
{
int i;
int ret = -1;
char *paux;

	*pstr = str+strlen(str);
	for(i=MENOR; i<=ISIN; i++)
		{
		paux = strstr(str, G_OPERADOR[i]);
		if((paux) && (paux<*pstr))
			{
			*pstr = paux;
			ret = i;
			}
		}

	return ret;
}

