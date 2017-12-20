#include "tablasimbolos.hpp"


//Anadir nuevo Objeto
void LinkedTVL::addNew(int nKey, m4VariantType nValor, int strSize)
{
	TVL *nEntrada = new TVL(nKey, nValor, strSize);		//Creamos el Objeto
	TVL *runner;

	M4_ASSERT(nEntrada != NULL) ;

	// Tipica Insercion al Final de una lista Enlazada.

	// Localizamos el Final de la Lista Enlazada: Podriamos tener un puntero al final
	// y ahorrarnos esto.
	runner = head;
	if (runner != NULL){	
		//Lista no vacia
		while (runner->next != NULL){
			runner = runner->next;
		}
		runner->next = nEntrada;
	} else {	
		//Lista Vacia
		head = nEntrada;
	}
	nEntrada->next = NULL;

	contextSize += strSize;
}


//Anadir nuevo Objeto a partir de otro
void LinkedTVL::addNew(TVL &nElem, int strSize)
{
	TVL *runner;
	// Tipica Insercion al Final de una lista Enlazada.
	runner = head;
	if (head != NULL){
		//Lista No Vacia
		while (runner->next != NULL){
			runner = runner->next;
		}
		runner->next = &nElem;	//Asignamos el Objeto
	} else {
		//Lista Vacia
		head = &nElem;			//Asignamos el Objeto
	}
	nElem.next = NULL;

	contextSize += strSize;
}

//Buscar Objeto
int LinkedTVL::searchKey(int key, m4VariantType &valor)
{
	TVL *runner;
	//Busqueda en una Lista enlazada de una clave
	runner = head;
	while (runner != NULL){
		if (runner->orden == key){
			valor = runner->valor;
			return 1;	// Encontrado
		}
		runner = runner->next;
	}
	return 0;	// No Encontrado
}

//Modificar o Añadir Objecto
int LinkedTVL::setValue(int key, m4VariantType valor, int strSize)
{
	TVL *runner;
	//Busqueda en una Lista enlazada de una clave
	runner = head;
	while (runner != NULL){
		if (runner->orden == key){
			contextSize -= runner->stringSize;
			//actualizacion del tamaño del elemento de la lista enlazada
			runner->stringSize = strSize;
			contextSize += strSize;
			//actualizacion del valor del elemento de la lista enlazada
			runner->valor = valor;
			return 1;	// Modificacion (encontrado)
		}
		runner = runner->next;
	}
	addNew(key, valor, strSize);
	return 0;	// Nuevo valor (no encontrado)
}

//Borrar la Tabla de Objetos
void LinkedTVL::Clean( void )
{
	//Recorremos la lista borrando los nodos hacia adelante
	for (TVL *pTemp=head; pTemp!=NULL; pTemp=head) {
		head=pTemp->next;
		delete pTemp;
	}
	head = NULL;
    contextSize = 0;
}


//Añadir elemento
void LinkedTO::addNew(char *nCanal, char* nNodo, int nRegistro, char *nItem, int nTramo, int nAtributo, m4VariantType nValor, int strSize, int numRefArgs, m4VariantType *refArgs)
{
	_TO *nEntrada = new _TO(nCanal, nNodo, nRegistro, nItem, nTramo, nAtributo, nValor, numRefArgs, refArgs, strSize);
	_TO *runner;

	M4_ASSERT(nEntrada!=NULL) ;

	// Insercion al Final de la Lista Enlazada 
	runner = head;

	if (runner != NULL){
		//Lista no vacia
		while (runner->next != NULL){
			runner = runner->next;
		}
		runner->next = nEntrada;
	} else {
		//Lista vacia
		head = nEntrada;
	}
	nEntrada->next = NULL;

	contextSize += strSize;
}

//Añadir elemento a partir de otro elemento
void LinkedTO::addNew(_TO &nElem, int strSize)
{
	_TO *runner;

	// Insercion al Final de la Lista Enlazada 
	runner = head;
	if (runner != NULL){
		while (runner->next != NULL){
			runner = runner->next;
		}
		runner->next = &nElem;		//Asginamos el elemento
	} else {
		head = &nElem;				//Asginamos el elemento
	}
	nElem.next = NULL;

	contextSize += strSize;

}

//Buscar elemento
int LinkedTO::searchKey(char *keyCanal, char *keyNodo, int keyRegistro, char *keyItem, 
		                  int keyTramo, int keyAtributo, m4VariantType &valor,
						  int &numRefArgs, m4VariantType* &refArgs)
{
	_TO *runner;

	// Busqueda secuencial por clave en una Lista Enlazada 
	runner = head;
	while (runner != NULL){
		if ( (strcmp(keyCanal, runner->canal) == 0) && 
			 (strcmp(keyNodo , runner->nodo ) == 0) && 
			 (keyRegistro == runner->registro)      && 
			 (strcmp(keyItem,  runner->item ) == 0) && 
			 (keyTramo    == runner->tramo  )		&& 
			 (keyAtributo == runner->atributo) ) {
				valor = runner->valor;
				numRefArgs = runner->numRefArgs;
				refArgs = runner->refArgs;
				return 1;	// Encontrado
			}
		runner = runner->next;
	}
	return 0;	// No Encontrado
}

//Modificar o Añadir elemento
int LinkedTO::setValue(char *keyCanal, char *keyNodo, int keyRegistro, char *keyItem, 
		                 int keyTramo, int keyAtributo, m4VariantType valor, int strSize,
						 int numRefArgs, m4VariantType *refArgs)
{
	_TO *runner;

	// Busqueda secuencial por clave en una Lista Enlazada 
	runner = head;
	while (runner != NULL){
		if ( (strcmp(keyCanal, runner->canal) == 0) && (strcmp(keyNodo, runner->nodo) == 0) && 
			 (keyRegistro == runner->registro) && (strcmp(keyItem, runner->item) == 0) && 
			 (keyTramo == runner->tramo) && (keyAtributo == runner->atributo) ){
				//Encontrado => Modificacion
				contextSize -= runner->stringSize;
				//actualizacion del tamaño del elemento de la lista enlazada
				runner->stringSize = strSize;
				contextSize += strSize;
				//actualizacion del valor del elemento de la lista enlazada
				runner->valor = valor;
				if (runner->numRefArgs > 0){
					runner->numRefArgs = numRefArgs;
					if (runner->refArgs)
						delete []runner->refArgs;
					runner->AssignVectorVariants(runner->refArgs, refArgs, numRefArgs);	//runner->refArgs = refArgs;
				}
				return 1;
			}
		runner = runner->next;
	}
	//No Encontrado => Alta
	addNew(keyCanal, keyNodo, keyRegistro, keyItem, keyTramo, keyAtributo, valor, strSize, numRefArgs, refArgs);
	return 0;
}


//Borrar la Tabla de Objetos
void LinkedTO::Clean( void )
{
	//Recorremos la lista borrando los nodos hacia adelante
	for (_TO *pTemp=head; pTemp!=NULL; pTemp=head) {
		head=pTemp->next;
		delete pTemp;
	}
	head = NULL;

    contextSize = 0;
}

