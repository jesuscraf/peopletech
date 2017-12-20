// Implementación de la Clase CExpresion

#include "windows.h"
#include "globdef.h"
#include "operador.h"
#include "arbol.h"
#include "evaluacion.h"
#include "expresion.h"
#include "macros.h"
#include "utils.h"

/*-----------------------------------------------------------------
Constructor
-----------------------------------------------------------------*/
CExpresion::CExpresion()
{
	m_num_evaluaciones = 0;
	m_num_operadores = 0;
	m_expresion = NULL;
	m_definida = FALSE;
}

CExpresion::~CExpresion()
{
int i;

	for(i=0; i<m_num_evaluaciones; i++)
		delete m_pevaluaciones[i];

	LIBERA(m_expresion);
}



/*-----------------------------------------------------------------
Define la expresión que ha de ser evaluada
-----------------------------------------------------------------*/
int CExpresion::Define(char *valor, char *tipo, char *exp)
{
size_t sz;
int n,op;
BOOL salir = FALSE;
char *txt=0;
BOOL exp_simple = FALSE;

	Trace("****EXPRESION: ",exp);

	sz = strlen(exp);
	m_expresion = (char*)malloc(sz+1);
	if(!m_expresion)
		return -1; //Error de memoria

	
	if(CheckExpresion(exp))
		return -1; //Expresión mal definida

	strcpy(m_expresion, exp);

	n = 1;
	while(!salir)
		{
		if(!Busca_Evaluacion(&txt, n))
			{
			salir = TRUE;
			LIBERA(txt);
			break;
			}
		else
			{
			m_pevaluaciones[m_num_evaluaciones] = new CEvaluacion;
			m_num_evaluaciones++;
			//Se conprueba se la evaluacion coincide con la expresión => se trata de una expresión simple o una expresión compuesta dentro de un (...)
			if(!strcmp(exp, txt))
				exp_simple = TRUE;
			FormateaExpresion(txt);
			//Se comprueba si es expresión simple o expresión dentro de (...)
			if((!exp_simple) || (strcmp(exp,txt)))
				exp_simple = FALSE;
			if((FindStr2(txt," AND")) || (FindStr2(txt,")AND")) || (FindStr2(txt," OR")) || (FindStr2(txt,")OR")))
				{//Si dentro de una de las evaluaciones se encuentra un AND o un OR se trata como una nueva expresión
				if(exp_simple)
					{//Si es una expresión simple no puede contener AND y OR
					LIBERA(txt);
					return -1;
					}
				CExpresion *newexp = new CExpresion;
				if(newexp->Define(valor,tipo,txt)<0)
					{
					LIBERA(txt);
					delete newexp;
					return -1;
					}
				if(newexp->Evalua())
					strcpy(txt,"TRUE");
				else
					strcpy(txt,"FALSE");
				delete newexp;
				}
			Trace("****EVALUACION: ", txt);
			if(m_pevaluaciones[m_num_evaluaciones-1]->Define(valor, txt)<0)
				{
				LIBERA(txt);
				return -1;
				}
			LIBERA(txt);
			}

		if((op = Busca_Operador(n)) >= 0)
			{
			m_operadores[m_num_operadores] = op;
			m_num_operadores++;
			}

		n++;
		}

	m_definida = TRUE;
	return 0;
}



/*-----------------------------------------------------------------
Evalua la expresión e indica si esta es correcta o no
-----------------------------------------------------------------*/
BOOL CExpresion::Evalua(void)
{
BOOL op1,op2;
int i;

	if((!m_num_evaluaciones) || (!m_definida))
		return FALSE;

	op1 = m_pevaluaciones[0]->Resuelve();

	for(i=1; i<m_num_evaluaciones; i++)
		{
		op2 = m_pevaluaciones[i]->Resuelve();
		op1 = Opera(op1, m_operadores[i-1], op2);
		}

	return op1;
}


/*-----------------------------------------------------------------
Busca una cadena a evaluar dentro de la expresión
-----------------------------------------------------------------*/
int CExpresion::Busca_Evaluacion(char **eva, int n)
{
int i, sz, offset;
char *pini = NULL;
char *p1 = NULL,*p2 = NULL,*pfin = NULL;
char *aux = NULL;

	pini = m_expresion;
	offset = 0;
	for(i=1;i<=n;i++)
		{
		pini += offset;
		p1 = FindStr(pini, G_OPERADOR[AND]);
		while((p1) && (*(p1-1) != 32) && (*(p1-1) != 41))
			p1 = FindStr(p1+1, G_OPERADOR[AND]);
		p2 = FindStr(pini, G_OPERADOR[OR]);
		while((p2) && (*(p2-1) != 32) && (*(p2-1) != 41))
			p2 = FindStr(p2+1, G_OPERADOR[AND]);
		if((!p1) && (!p2))
			{
			pfin = pini + strlen(pini);
			i++;
			break;
			}
		else
		if((p1) && ((p1 < p2) || (!p2)))
			pfin = p1 - 1;
		else
			pfin = p2 - 1;

		aux = strchr(pfin + 1, ' ');
		if(!aux)
			{
			pfin = pini + strlen(pini);
			i++;
			break;
			}
		else
			offset = (int)(aux - pini + 1);
		}

	if(i<=n)
		return 0;

	
	sz = (int)(pfin - pini);
	*eva = 0;
	*eva = (char*)malloc(sz+1);
	if(!(*eva))
		return 0;
	memcpy(*eva, pini, sz);
	(*eva)[sz]=0;

	return sz;
}


/*-----------------------------------------------------------------
Busca un operador lógico dentro de la expresión
-----------------------------------------------------------------*/
int CExpresion::Busca_Operador(int n)
{
int i;
char *pstr = NULL;
char *pop1 = NULL, *pop2 = NULL, *pop = NULL;

	pstr = m_expresion;
	for(i=1; i<=n; i++)
		{
		pop1 = FindStr(pstr, G_OPERADOR[AND]);
		pop2 = FindStr(pstr, G_OPERADOR[OR]);
		if((pop1) && ((pop1 < pop2) || (!pop2)))
			pop = pop1;
		else
			pop = pop2;

		if(pop)
			pstr = pop + 1;
		}
		
	if(!pop)
		return -1;

	if(pop == pop1)
		return AND;
	else
		return OR;
}

/*-----------------------------------------------------------------
Realiza una operación lógica entre dos operandos
-----------------------------------------------------------------*/
BOOL CExpresion::Opera(BOOL operador1, int op_logico, BOOL operador2)
{
BOOL ret;

	switch(op_logico)
		{
		case AND:
			ret = (operador1 && operador2);
			break;
		case OR:
			ret = (operador1 || operador2);
			break;
		default:
			ret = FALSE;
			break;
		}

	return ret;
}


/*--------------------------------------------------------------------
Pasa de una expresión tipo " (expresion) " a otra del tipo "expresion"
--------------------------------------------------------------------*/
void CExpresion::FormateaExpresion(char *exp)
{
char *aux = NULL;
size_t sz;
size_t ini,fin;
BOOL parentesis = FALSE;

	sz = strlen(exp);
	aux = (char *)malloc(sz+1);
	strcpy(aux, exp);

	ini = 0;
	while(aux[ini] == ' ')
		ini++;

	if(aux[ini] == 40)
		{
		ini++;
		parentesis = TRUE;
		}

	fin = strlen(aux) - 1;
	while(aux[fin] == ' ')
		fin--;

	if((aux[fin] == 41) && (parentesis))
		fin--;

	aux[fin+1] = 0;
	strcpy(exp, aux+ini);

	LIBERA(aux);
}


BOOL CExpresion::CheckExpresion(char *expresion)
{
size_t sz;
size_t i;
size_t n,m;

	//Si la expresión contiene mayor número de '(' que de ')'
	// o contiene un número de '"' impar, la expresión esta mal definida
	//Trace("Expresion:",expresion);

	sz = strlen(expresion);

	n = 0;
	m = 0;
	for(i = 0; i<sz; i++)
		{
		switch(expresion[i])
			{
			case 40:
				n++;
				break;
			case 41:
				n--;
				break;
			case 34:
				m++;
				break;
			}
		}

	if(m == 0)	m = 2;
	if((n != 0) || (m % 2))
		return TRUE;
	else
		return FALSE;
}
