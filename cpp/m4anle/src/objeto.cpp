// Objeto.cpp: implementation of the CObjeto class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "m4anle.h"
#include "runtime.h"
#include "objeto.h"
#include "utils.h"
#include "macros.h"
#include "operador.h"
#include "arbol.h"
#include "evaluacion.h"
#include "expresion.h"
#include "globdef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//CAMBIOS PARA VERSION 3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjObligatorio::CObjObligatorio(CString nombre, long insmax, long insmin)
{
	m_nombre = nombre;
	m_insmax = insmax;
	m_insmin = insmin;
	m_insnum = 0;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPropiedad::CPropiedad(CString nombre, CString valor)
{
	m_nombre = nombre;
	m_valor = valor;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjeto::CObjeto(CString nombre, CString alias, int identificador, CRuntime *runtime)
{
	m_nombre = nombre;
	m_alias = alias;
	m_identificador = identificador;
	m_padre = NULL;
	m_runtime = runtime;
}


CObjeto::~CObjeto()
{
CObject *pobj;

	while(!m_listchild.IsEmpty())
		{
		pobj = m_listchild.GetHead();
		m_listchild.RemoveHead();
		delete pobj;
		}

	while(!m_objobligatorios.IsEmpty())
		{
		pobj = m_objobligatorios.GetHead();
		m_objobligatorios.RemoveHead();
		delete pobj;
		}

	while(!m_listprop.IsEmpty())
		{
		pobj = m_listprop.GetHead();
		m_listprop.RemoveHead();
		delete pobj;
		}
}


CObjeto * CObjeto::FindObjeto(int id)
{
POSITION pos;
CObjeto *ret=NULL;
CObjeto *child=NULL;

	if(m_identificador == id)
		return (CObjeto *)this;

	for(pos = m_listchild.GetHeadPosition(); pos != NULL;)
		{
		child = (CObjeto *)m_listchild.GetNext(pos);
		ret = child->FindObjeto(id);
		if(ret)
			return ret;
		}

	return NULL;
}


void CObjeto::AddObjObligatorio(OBJ_OBLIGATORIO *objeto)
{
CObjObligatorio *pnew = new CObjObligatorio(objeto->nombre, objeto->insmax, objeto->insmin);

	m_objobligatorios.AddHead(pnew);
}


void CObjeto::AddPropObligatoria(CString propiedad)
{
	m_propobligatorias.AddHead((LPCSTR)propiedad);
}

void CObjeto::GetObjObligatorio(int num,CString &objeto)
{
CObjObligatorio *pobj=NULL;
POSITION pos;

	if(num<1)
		return;

	pos = m_objobligatorios.FindIndex(num-1);
	if(!pos)
		return;

	pobj = (CObjObligatorio *)m_objobligatorios.GetAt(pos);
	if(!pobj)
		return;

	objeto = pobj->m_nombre;
}

void CObjeto::GetPropObligatoria(int num,CString &propiedad)
{
POSITION pos;

	if(num<1)
		return;

	pos = m_propobligatorias.FindIndex(num-1);
	if(!pos)
		return;

	propiedad = m_propobligatorias.GetAt(pos);
}

short CObjeto::AddChild(CObjeto *pobj)
{
POSITION pos;
CObjObligatorio *pobligatorio;
CObjeto *phermano;
BOOL error = FALSE;

	//1.- Comprobar si el objeto padre ya contiene algun objeto con el mismo alias
	for(pos = m_listchild.GetHeadPosition(); pos != NULL;)
		{
		phermano = (CObjeto *)m_listchild.GetNext(pos);
		if(phermano->m_alias == pobj->m_alias)
			{
			error = TRUE;//Alias repetido
			break;
			}
		}

	m_listchild.AddHead(pobj);

	//2.- Comprobar si el nuevo objeto supera el máximo número de instancias permitidas dentro del objeto padre
	for(pos = m_objobligatorios.GetHeadPosition(); pos != NULL;)
		{
		pobligatorio = (CObjObligatorio *)m_objobligatorios.GetNext(pos);
		if(pobligatorio->m_nombre == pobj->m_nombre)
			{
			pobligatorio->m_insnum++;
			if((pobligatorio->m_insnum > pobligatorio->m_insmax) && (pobligatorio->m_insmax > 0))
				{//Se ha superado el nº máximo de instancias -> 0. Si tambien se repite el alias -> -2
				if(error)
					return -2;//Superado nº máximo de instancias + alias repetido
				else
					return FALSE;//Superado nº máximo de instancias
				}
			break;
			}
		}

	if(error)
		return -1;//Alias repetido
	else
		return TRUE;//OK
}


CObjeto * CObjeto::FindChild(CString alias)
{
POSITION pos;
CObjeto *child;

	for(pos = m_listchild.GetHeadPosition(); pos != NULL;)
		{
		child = (CObjeto *)m_listchild.GetNext(pos);
		if(child->m_alias == alias)
			return child;
		}

	return NULL;
}


short CObjeto::AddPropiedad(CString propiedad, CString valor, CString *expresion)
{
POSITION pos1,pos2;
CPropiedad *prop = new CPropiedad(propiedad, valor);
CHK_CONSTRAINT info;

	m_listprop.AddHead(prop);

	for(pos1 = m_propobligatorias.GetHeadPosition(); pos1 != NULL;)
		{
		pos2 = pos1;
		if(m_propobligatorias.GetNext(pos1) == propiedad)
			{
			m_propobligatorias.RemoveAt(pos2);
			break;
			}
		}

	if(!expresion)
		return TRUE;

	if(expresion->IsEmpty())
		return TRUE;

	//Verificación del constrain
	return (short)CheckConstraint(propiedad, valor, expresion, &info);
}


BOOL CObjeto::CheckConstraint(CString propiedad, CString valor, CString *expresion, CHK_CONSTRAINT *info)
{
char *pstr1 = NULL;
char *pstr2 = NULL;
char *pstr3 = NULL;
char *aux1 = NULL;
char *aux2 = NULL;
char *aux3 = NULL;
size_t sz;
CString existe = "FALSE";
CString value="";
CExpresion constraint;
BOOL ret = FALSE;

#ifdef __NO_CONSTRAINT__
AVISO(OJO_NO_SE_EVALUAN_LOS_CONSTRAINT)
return TRUE;
#else

	Trace("**Expresion:", *expresion);

	//---------VERIFICACION DEL CONSTRAIN-----------
	//----------------------------------------------

	//Sustitución de MyValue() por el valor de la propiedad
	ChgStr(&aux2, (*expresion), VALOR, valor);
	Trace("**MyValue:", aux2);

	//Sustitución de MyName por el nombre de la propiedad
	ChgStr(&aux1, aux2, "MyName()", propiedad);
	LIBERA(aux2);
	Trace("**MyName:", aux1);

	//Sustitución de IsPresent por TRUE o FALSE segun corresponda
	pstr1 = strstr(aux1,"IsPresent");
	while(pstr1 >= aux1)
		{
		pstr2 = strchr(pstr1, 40);
		pstr3 = strchr(pstr1, 41);
		if((!pstr2) || (!pstr3))
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;
		
		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		if(IsPresent(aux2))
			existe = "TRUE";
		else
			existe = "FALSE";

		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, existe);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = strstr(aux1,"IsPresent");
		}
	Trace("**IsPresent:", aux1);

	//Sustirución de Value por el valor de la propiedad correspondiente
	pstr1 = strstr(aux1,"Value(");
	while((pstr1 >= aux1) && ((pstr1-1)[0] == 'y'))
		pstr1 = strstr(pstr1+1,"Value(");
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		pstr3 = strchr(pstr1, 41);
		if((!pstr2) || (!pstr3))
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;
		
		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		value = Value(aux2);

		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, value);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = strstr(aux1,"Value(");
		while((pstr1 >= aux1) && ((pstr1-1)[0] == 'y'))
			pstr1 = strstr(pstr1+1,"Value(");
		}
	Trace("**Value:", aux1);


	//Sustitución de GetStringFromTo por su valor correspondiente
	char seps[] = ",";
	char *token = NULL;
	char *parm1 = NULL,*parm2 = NULL,*parm4 = NULL;
	int parm3,parm5;
	char *result=NULL;

	pstr1 = FindLast(aux1, "GetStringFromTo");
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		if(!pstr2)
			break;
		pstr3 = FindChr2(pstr2+1, 41);
		if(!pstr3)
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;

		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		//Busqueda de los 5 parametros
		//1.
		parm1 = strtok(aux2, seps);
		if(!parm1)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		//2.
		parm2 = strtok(NULL, seps);
		if(!parm2)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		//3.
		token = strtok(NULL, seps);
		if(!token)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		parm3 = atoi(token);
		//4.
		parm4 = strtok(NULL, seps);
		if(!parm4)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		//5.
		token = strtok(NULL, seps);
		if(!token)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		parm5 = atoi(token);

		if((parm1[0] == 34) && (parm1[strlen(parm1) - 1]== 34))
			{
			parm1[strlen(parm1) - 1]=0;
			parm1 = parm1+1;
			}
		else
			if(parm1[0] == 32)
				parm1 = parm1+1;
		parm2[strlen(parm2) - 1]=0;
		parm4[strlen(parm4) - 1]=0;
		GetStringFromTo(&result, parm1, parm2+1, parm3, parm4+1, parm5);
		if(!result)
			{
			result = (char*)malloc(6);
			if(!result)
				{
				LIBERA(aux2);
				LIBERA(aux3);
				break;
				}
			strcpy(result,"ERROR");
			}
		
		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, result);
		LIBERA(result);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = FindLast(aux1,"GetStringFromTo");
		}
	Trace("**GetStringFromTo:", aux1);

	//Sustitución de GetListStringFromTo por su valor correspondiente
	pstr1 = FindLast(aux1, "GetListStringFromTo");
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		if(!pstr2)
			break;
		pstr3 = FindChr2(pstr2+1, 41);
		if(!pstr3)
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;

		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		//Busqueda de los 5 parametros
		//1.
		parm1 = strtok(aux2, seps);
		if(!parm1)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		//2.
		parm2 = strtok(NULL, seps);
		if(!parm2)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		//3.
		token = strtok(NULL, seps);
		if(!token)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		parm3 = atoi(token);
		//4.
		parm4 = strtok(NULL, seps);
		if(!parm4)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		//5.
		token = strtok(NULL, seps);
		if(!token)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		parm5 = atoi(token);

		if((parm1[0] == 34) && (parm1[strlen(parm1) - 1]== 34))
			{
			parm1[strlen(parm1) - 1]=0;
			parm1 = parm1+1;
			}
		else
			if(parm1[0] == 32)
				parm1 = parm1+1;
		parm2[strlen(parm2) - 1]=0;
		parm4[strlen(parm4) - 1]=0;
		GetStringFromTo(&result, parm1, parm2+1, parm3, parm4+1, parm5);
		if(!result)
			{
			result = (char*)malloc(6);
			if(!result)
				{
				LIBERA(aux2);
				LIBERA(aux3);
				break;
				}
			strcpy(result,"ERROR");
			}
		
		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, result);
		LIBERA(result);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = FindLast(aux1,"GetListStringFromTo");
		}
	Trace("**GetListStringFromTo:", aux1);
	

	//Sustitución de IsClassnameDefined por su valor correspondiente
	pstr1 = NULL;
	pstr1 = strstr(aux1, "IsClassnameDefined");
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		if(!pstr2)
			break;
		pstr3 = FindChr(pstr2+1, 41);
		if(!pstr3)
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;

		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;
	
		CleanChr(aux2,34);//Eliminar las ""
		if(m_runtime->IsClassnameDefined(aux2))
			existe = "TRUE";
		else
			existe = "FALSE";

		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, existe);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = strstr(aux1,"IsClassnameDefined");
		}
	Trace("**IsClassnameDefined:", aux1);

	//Sustitución de Classname por su valor correspondiente
	pstr1 = NULL;
	CObjeto *pobj2=NULL;
	pstr1 = strstr(aux1, "Classname");
	CString cname;
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		if(!pstr2)
			break;
		pstr3 = FindChr(pstr2+1, 41);
		if(!pstr3)
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;

		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		pobj2 = IsValidPath(aux2);
		if(!pobj2)
			{
			cname = "ERROR";
			info->failpath = TRUE;
			}
		else
			cname = pobj2->m_nombre;

		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, cname);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = strstr(aux1,"Classname");
		}
	Trace("**Classname:", aux1);

	//Sustitución de RuntimeMethod por su valor correspondiente
	CString objname, propname;
	pstr1 = strstr(aux1, "RuntimeMethod");
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		if(!pstr2)
			break;
		pstr3 = FindChr(pstr2+1, 41);
		if(!pstr3)
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;

		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		//Busqueda de los 2 parametros
		//1.
		parm1 = strtok(aux2, seps);
		if(!parm1)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		CleanSp(parm1,parm1);
		objname = parm1;
		//2.
		parm2 = strtok(NULL, seps);
		if(!parm2)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		CleanSp(parm2,parm2);
		propname = parm2;

		if(m_runtime->Runtimemethod(objname, propname))
			existe = "TRUE";
		else
			{
			existe = "FALSE";
			info->failruntimemethod = TRUE;
			}

		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, existe);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = strstr(aux1,"RuntimeMethod");
		}
	Trace("**RuntimeMethod:", aux1);
		

	//Sustitución de RuntimeProperty por su valor correspondiente
	pstr1 = strstr(aux1, "RuntimeProperty");
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		if(!pstr2)
			break;
		pstr3 = FindChr(pstr2+1, 41);
		if(!pstr3)
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;

		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		//Busqueda de los 2 parametros
		//1.
		parm1 = strtok(aux2, seps);
		if(!parm1)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		CleanSp(parm1,parm1);
		objname = parm1;
		//2.
		parm2 = strtok(NULL, seps);
		if(!parm2)
			{
			LIBERA(aux2);
			LIBERA(aux3);
			break;
			}
		CleanSp(parm2,parm2);
		propname = parm2;

		if(m_runtime->Runtimeproperty(objname, propname))
			existe = "TRUE";
		else
			{
			existe = "FALSE";
			info->failruntimeproperty = TRUE;
			}

		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, existe);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = strstr(aux1,"RuntimeProperty");
		}
	Trace("**RuntimeProperty:", aux1);


	//Sustitución de IsValidPath por su valor correspondiente
	pstr1 = NULL;
	pstr1 = strstr(aux1, "IsValidPath");
	while(pstr1 >= aux1) 
		{
		pstr2 = strchr(pstr1, 40);
		if(!pstr2)
			break;
		pstr3 = FindChr(pstr2+1, 41);
		if(!pstr3)
			break;

		sz = pstr3 - pstr2;
		aux2 = (char *)malloc(sz);
		if(!aux2)
			break;
		memcpy(aux2, pstr2+1, sz-1);
		aux2[sz-1] = 0;

		sz = pstr3 - pstr1;
		aux3 = (char *)malloc(sz+2);
		if(!aux3)
			break;
		memcpy(aux3,pstr1,sz+1);
		aux3[sz+1] = 0;

		if(!IsValidPath(aux2))
			{
			existe = "FALSE";
			info->failpath = TRUE;
			}
		else
			existe = "TRUE";

		LIBERA(aux2);
		sz = strlen(aux1) + 1;
		aux2 = (char*)malloc(sz);
		strcpy(aux2,aux1);
		LIBERA(aux1);
		ChgStr(&aux1, aux2, aux3, existe);
		LIBERA(aux2);
		LIBERA(aux3);
		pstr1 = strstr(aux1,"IsValidPath");
		}
	Trace("**IsValidPath:", aux1);

	//Se realiza la evaluación de la expresión
	//char *aux4=NULL;
	Trace("DEFINE:",aux1);
	/*ChrStr(&aux4, aux1, valor, VALOR);
	LIBERA(aux1);
	if(aux4)
		{
		constraint.Define(0,"",aux1);
		ret = constraint.Evalua();
		LIBERA(aux4);
		}
	else
		ret = FALSE;*/
	constraint.Define(0,"",aux1);
	ret = constraint.Evalua();
	LIBERA(aux1);

	return ret;

#endif
}


int CObjeto::GetNumObjObligatorios()
{
POSITION pos;
CObjObligatorio *pobligatorio;
int ret = 0;

	for(pos = m_objobligatorios.GetHeadPosition(); pos != NULL;)
		{
		pobligatorio = (CObjObligatorio *)m_objobligatorios.GetNext(pos);
		if(pobligatorio->m_insnum < pobligatorio->m_insmin)
			ret += (pobligatorio->m_insmin - pobligatorio->m_insnum);
		}

	return ret;
}

int CObjeto::GetNumMaxObjObligatorios()
{
POSITION pos;
CObjObligatorio *pobligatorio;
int ret = 0;

	for(pos = m_objobligatorios.GetHeadPosition(); pos != NULL;)
		{
		pobligatorio = (CObjObligatorio *)m_objobligatorios.GetNext(pos);
		if(pobligatorio->m_insnum > pobligatorio->m_insmax)
			ret += (pobligatorio->m_insnum - pobligatorio->m_insmax);
		}

	return ret;
}

int CObjeto::GetNumPropObligatorias()
{
	return (int) m_propobligatorias.GetCount();
}


BOOL CObjeto::IsPresent(LPCSTR propiedad)
{
BOOL existe = FALSE;
POSITION pos1;
CPropiedad *prop = NULL;

	for(pos1 = m_listprop.GetHeadPosition(); pos1 != NULL;)
		{
		prop = (CPropiedad*)m_listprop.GetNext(pos1);
		if(prop->m_nombre == propiedad)
			existe = TRUE;
		}

	return existe;
}

CString& CObjeto::Value(LPCSTR propiedad)
{
	POSITION pos1;
	CPropiedad *prop = NULL;
	CString  * ret = new CString;

	for(pos1 = m_listprop.GetHeadPosition(); pos1 != NULL;)
	{
		prop = (CPropiedad*)m_listprop.GetNext(pos1);
		if(prop->m_nombre == propiedad)
			*ret = prop->m_valor;
	}

	 return *ret;
}


CObjeto * CObjeto::GetRaiz()
{
CObjeto *pobj=NULL;
CObjeto *pant;

	if(!m_padre)
		return this;

	pobj = m_padre;
	while(pobj)
		{
		pant = pobj;
		pobj = pobj->m_padre;
		}

	return pant;
}


CObjeto* CObjeto::IsValidPath(LPCSTR path)
{
char *token = NULL;
char seps[] = "/";
size_t sz;
CObjeto *pobj=NULL;
CObjeto *raiz=NULL;
char *aux = NULL;
char *pstr=NULL;
char *propiedad=NULL;
char *objeto=NULL;


	sz = strlen((LPCSTR)path);
	aux = (char *)malloc(sz +1);
	if(!aux)
		return NULL;
	strcpy(aux,(LPCSTR)path);

	raiz = GetRaiz();

	token = strtok(aux, seps);
	if(!strcmp(token,".."))
		{
		pobj = m_padre;
		token = strtok( NULL, seps );
		while(!strcmp(token,".."))
			{
			pobj = pobj->m_padre;
			if(!pobj)
				break;
			token = strtok( NULL, seps );
			if(!token)
				break;
			}
		}
	else
		pobj = raiz;

	if(!pobj)
		{
		LIBERA(aux);
		return NULL;
		}

	if(pobj == raiz)
		{
		if(strcmp(token, pobj->m_alias))
			{
			LIBERA(aux);
			return NULL;
			}
		else
			token = strtok( NULL, seps );
		}

	while(token != NULL)
		{
		if(!strcmp(token,".."))
			pobj = pobj->m_padre;
		else
			pobj = pobj->FindChild(token);
		if(!pobj)
			break;
		token = strtok( NULL, seps );
		}

	LIBERA(aux);
	return pobj;
}
