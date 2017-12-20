#ifndef __TABLASIMBOLOS_HPP__
#define __TABLASIMBOLOS_HPP__

#include <stdio.h>

#include "cltypes.hpp"
#include "m4var.hpp"

//Objeto: Variable Local
struct TVL{
	
	int orden;				//Num.de Variable Local
	m4VariantType valor;	//Valor de la variable local
	int stringSize;			//Tamano que ocupara formateada

	TVL *next;				//Next de la Lista

	//Constructor
	TVL(int nOrden, m4VariantType nValor, int nStringSize){
		orden = nOrden;
		valor = nValor;		
		stringSize = nStringSize;
        next = NULL ;
	}

	//Constructor de Copia
	TVL(const TVL& ai_oTVL){
		orden = ai_oTVL.orden;
		valor = ai_oTVL.valor;		
		stringSize = ai_oTVL.stringSize;
        next = NULL ;	//Desvinculado
	}

	~TVL() {
		Clean();
	}

	//Limpiado
    void Clean(void) {}
};

//Objeto: Tabla de Variables Locales
struct LinkedTVL{
	TVL *head;			//Cabeza de la Lista de Objectos de Variable Local
	int contextSize;	//Tamano de los Objetos formateados

	//Constructor por defecto
	LinkedTVL(){
		head = NULL;
		contextSize = 0;
	}

	//Destructor
    ~LinkedTVL(){
        Clean() ;
	}

	void addNew(int nKey,m4VariantType nValor, int strSize);	//Anadir nuevo Objeto
	void addNew(TVL &nElem, int strSize);						//Anadir nuevo Objeto a partir de otro
	int searchKey(int key, m4VariantType &valor);				//Buscar Objeto
	int setValue(int key, m4VariantType valor, int strSize);	//Modificar o Añadir Objecto
    void Clean( void );											//Borrar la Tabla de Objetos
};

//Objeto: Item
struct _TO{
	char *canal;			//Nombre del Canal
	char *nodo;				//Nombre del Nodo
	int  registro;			//No.de Registro
	char *item;				//Nombre del Item
	int  tramo;				//No.de Tramo
	int atributo;			//No.de Atributo
	m4VariantType valor;	//Valor del Item
	int numRefArgs;			//Numero de Argumentos por referencia
	m4VariantType *refArgs;	//Vector de Argumentos del Item
	int stringSize;			//Tamaño del Item formateado

	_TO *next;				//Siguiente elemento en la lista


	//################## Metodos ##################

	m4return_t AssignVectorVariants(m4VariantType* &aio_vVariantDest, const m4VariantType *ai_vVariantSource, int ai_iNumVar)
	{
		//Borrar previo
		//if (aio_vVariantDest) {
		//	delete []aio_vVariantDest;
		//	aio_vVariantDest=NULL;
		//}
		//Check entradas
		aio_vVariantDest = NULL;
		if (ai_iNumVar<0 || !ai_vVariantSource)
			return M4_ERROR;
		if (ai_iNumVar==0)
			return M4_SUCCESS;
		//Nuevo vector
		aio_vVariantDest = new m4VariantType[ai_iNumVar];
		if (!aio_vVariantDest)
			return M4_ERROR;
		//Copia del vector
		for (int i=0; i<ai_iNumVar; i++) {
			aio_vVariantDest[i]=ai_vVariantSource[i];
		}
		return M4_SUCCESS;
	}


	//Asignar una Cadena Origen a un miembro del Objeto. Si Origen=NULL, Destino=""
	m4return_t AssignString(char* &aio_pszDest, const char *ai_pszSource) 
	{
		//Borrado previo
		//if (aio_pszDest)
		//	delete []aio_pszDest;
		aio_pszDest = NULL;
		if (!ai_pszSource) {
			//Default = ""
			aio_pszDest=new char[1];
			*aio_pszDest='\0';
		} else {
			//Copia de la Cadena
			int iLen = strlen(ai_pszSource);
			aio_pszDest = new char[iLen+1];
			if (!aio_pszDest)
				return M4_ERROR;
			memcpy(aio_pszDest, ai_pszSource, iLen+1);
		}
		return M4_SUCCESS;
	}


	//## Constructor por Defecto
	_TO(){
		AssignString(canal, NULL);
		AssignString(nodo,	NULL);
		registro = 0;
		AssignString(item,	NULL);
		tramo = 0;
		atributo = -1;
		valor.Type = M4CL_CPP_TYPE_NULL;
		valor.Data.DoubleData = 0.0;
		refArgs = NULL;
		numRefArgs = 0;
		next = NULL;
		stringSize = 0;
	}
	
	//## Constructor con parametros
	_TO(char *nCanal, char* nNodo, int nRegistro, char *nItem, int nTramo, int nAtributo, m4VariantType nValor, int nNumRefArgs, m4VariantType *nRefArgs, int nStringSize){
		AssignString(canal, nCanal);
		AssignString(nodo,	nNodo);
		registro = nRegistro;
		AssignString(item,	nItem);
		tramo = nTramo;
		atributo = nAtributo;
		valor = nValor;
		numRefArgs = nNumRefArgs;
		AssignVectorVariants(refArgs, nRefArgs, nNumRefArgs);	//refArgs = nRefArgs;	//Copiar el no. nNumRefArgs
		stringSize = nStringSize;
		next = NULL;
	}

	//## Constructor de Copia
	_TO(const _TO& ai_oTO){
		if (canal)
			delete []canal;
		AssignString(canal, ai_oTO.canal);
		if (nodo)
			delete []nodo;
		AssignString(nodo,	ai_oTO.nodo);
		registro = ai_oTO.registro;
		if (item)
			delete []item;
		AssignString(item,	ai_oTO.item);
		tramo = ai_oTO.tramo;
		atributo = ai_oTO.atributo;
		valor = ai_oTO.valor;
		numRefArgs = ai_oTO.numRefArgs;
		if (refArgs)
			delete []refArgs;
		AssignVectorVariants(refArgs, ai_oTO.refArgs, ai_oTO.numRefArgs);
		next = NULL;	//Desvinculado
		stringSize = ai_oTO.stringSize;
	}

	//Destructor
	~_TO(){
		Clean();
	}
    
	//Limpiar memoria reservada por el Objeto
	void Clean( void )
	{
		//Limpiamos memoria del objeto
		if (canal)
			delete [] canal;
		if (nodo)
			delete [] nodo;
		if (item)
			delete [] item;
		if (refArgs)
			delete [] refArgs;
		canal	=NULL;
		nodo	=NULL;
		item	=NULL;
		refArgs	=NULL;
	}
};


//Objeto: Tabla de Objetos
struct LinkedTO{

	_TO *head;			//Cabeza de la Lista de Objectos de Variable Local
	int contextSize;	//Tamano de los Objetos formateados

	//Constructor por defecto
	LinkedTO(){
		head = NULL;
		contextSize = 0;
	}

	//Destructor
    ~LinkedTO(){
        Clean() ;
	}

	void addNew(char *nCanal, char* nNodo, int nRegistro, char *nItem, int nTramo,
		        int nAtributo, m4VariantType nValor, int strSize, int numRefArgs, 
				m4VariantType *refArgs);	//Añadir elemento
    void addNew(_TO &nElem, int strSize);	//Añadir elemento a partir de otro elemento
	int searchKey(char *keyCanal, char *keyNodo, int keyRegistro, char *keyItem, 
		          int keyTramo, int keyAtributo, m4VariantType &valor, 
				  int &numRefArgs, m4VariantType* &refArgs);	//Buscar elemento
	int setValue(char *keyCanal, char *keyNodo, int keyRegistro, char *keyItem, 
		         int keyTramo, int keyAtributo, m4VariantType valor, int strSize,
				 int numRefArgs, m4VariantType *refArgs);		//Modificar o Añadir elemento


    void Clean( void ) ;	//Borrar la Tabla de Objetos
};

#endif	//__TABLASIMBOLOS_HPP__

