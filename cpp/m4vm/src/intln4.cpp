#include "intln4.hpp"

#include "tablfunc.h"
#include "vmachin2.h"
#include "trad.h"
#include "finder.hpp"

#include "register.hpp"
#include "clstringpool.hpp"
#include "m4win2ux.hpp"


// Funcion: Rellenar el Vector de Etiquetas a partir del Vector de Instrucciones (polaca)
// Entrada:	ai_polaca	= Vector de Instrucciones de LN4 (polaca)
// Salida:	ao_labels	= Vector de Etiquetas
//			return	= M4_ERROR | M4_SUCCESS
m4return_t IntLN4::PreLocalTest(const STRINGVECTOR &ai_polaca, INTVECTOR &ao_labels)
{
	if ( ai_polaca.empty() )
		return M4_ERROR;	//No se reciben bien los datos, devolver error

	char *pcIns;			//Instruccion
	int iLabelNum;			//Numero de Etiqueta
	m4uint16_t ui16NumFunc;	//Numero de Funcion
	m4int8_t				fFlags;
	//Recorremos el Vector de Instrucciones
	for (unsigned int i = 0; i<ai_polaca.size(); i++){

		//Obtener la Instruccion
		pcIns = ai_polaca[i];
		
		//Segun el Tipo de Instruccion
		switch( *pcIns ) {
		case 'L': // Sentencia: Lnum	(Etiqueta)
			{
				iLabelNum = atoi(pcIns + 1);
				//Si el No.Etiqueta > tamaño del vector => hay que hacer el vector más grande
				if ( iLabelNum >= (int)ao_labels.size() )
					ao_labels.resize(iLabelNum+1, -1);
				//Asignar a la posición iLabelNum, la posición del Vector de Instrucciones que corresponde a esta etiqueta.
				ao_labels[iLabelNum] = i;
			}
			break;
		
		case 'F': // Sentencia: Fnum nivel	(Funcion)
			{
				//Obtener la Funcion y Comprobar su validez para el LocalTest
				ui16NumFunc = atoi (pcIns+1);
				fFlags=m_poFuncTable->GetObjectFunction(ui16NumFunc).GetFlags();
				if (M4CL_HAS_FLAG(fFlags, M4CL_FLAG_FN_LOCAL_TEST_DISABLED) ){
					DUMP_CHLOG_ERRORF(M4_CH_VM_DISABLED_FUNCTION_IN_LOCAL_TEST, m_poFuncTable->GetObjectFunction(ui16NumFunc).GetName() );
					return M4_ERROR;
				}
			}
			break;
		case 'H': // Sentencia: Hnum	(Funciones de Canal: Nivel 2)
			{
				// En teoria con LocalTest .... no se pueden ejecutar funciones de Nivel 2 ???
				//Obtener la Funcion y Comprobar su validez para el LocalTest
				//ui16NumFunc = atoi (pcIns+1);
				//if ( !m_poFuncTable->IsValidForLocalTest( ui16NumFunc ) )
					return M4_ERROR;
			}
			break;

		} //end-switch
	
	} //end-for

	return M4_SUCCESS;
}

// Funcion: Crear el Vector de Punteros a los Tokens de Instrucciones (polaca) a partir del stream de la polaca
//			y trozear una copia del stream de la Polaca en tokens.
// Entrada:	ai_pcPolishCode = Stream de Instrucciones en formato char (polaca)
// Salida:	ao_polaca		= Vector de Punteros a las Instrucciones de LN4 (polaca)
//			return	= M4_ERROR | M4_SUCCESS
m4return_t IntLN4::PushPolishInVector(const char *ai_pcPolishCode, STRINGVECTOR &ao_polaca)
{
	//Borramos previamente
	if (m_pSourceCode) {
		delete [] m_pSourceCode;
		m_pSourceCode = NULL;
	}

	//No hay polaca
	if (!ai_pcPolishCode)
		return M4_ERROR;

	//Hacemos una copia de la Polaca (para manipular con strtok, y para asignar al vector de "polaca")
	int lSourceCode = strlen(ai_pcPolishCode);
    m_pSourceCode=new char [lSourceCode+1];
	if (!m_pSourceCode)
		return M4_ERROR;
    memcpy(m_pSourceCode,ai_pcPolishCode,lSourceCode+1);

	//Creamos el Vector de punteros a Instrucciones de Polaca
	char *pszLasts;
	char *pcIns = M4_strtok(m_pSourceCode, SEP, &pszLasts);
	while (pcIns != NULL) {
		ao_polaca.push_back(pcIns);
		pcIns = M4_strtok(NULL, SEP, &pszLasts);
	}

	return M4_SUCCESS;
}

// Funcion: Ejecutar una Funcion de VM
// Entrada:	ai_ui16NumFunc	= Numero de Funcion a Ejecutar
//			ai_iLevel		= Nivel de la Funcion (no usado actualmente)
// Salida:	return	= M4_ERROR | M4_SUCCESS
m4return_t IntLN4::ExecuteFunction(m4uint16_t ai_ui16NumFunc, int ai_iLevel)
{
	//if ( !m_poFuncTable->IsValidForLocalTest(ai_ui16NumFunc) )
	//	return M4_ERROR;

	switch (m_poFuncTable->GetType (ai_ui16NumFunc) ){
	case M4CL_PROPAGATION_FUNC:
	case M4CL_SEMANTIC_FUNC1:
		return	((ClVMachine1*)m_poVM)->ExecuteFunctionForLocalTest (ai_ui16NumFunc);

	case M4CL_SEMANTIC_FUNC:
		return	((ClVMachine0*)m_poVM)->ExecuteFunctionForLocalTest (ai_ui16NumFunc);

	case M4CL_SEMANTIC_FUNC2:
	case M4CL_CHANNEL_FUNC:
		return	((ClVMachine2*)m_poVM)->ExecuteFunctionForLocalTest (ai_ui16NumFunc);

	default: 
		return M4_ERROR;
	}
}


// Funcion: Destruir un vector de Variants, eliminando las referencias al Pool.
// Entrada:	aio_pvVariants	=	Vector de Variants
//			ai_iNumElem		=	No. de Elementos del array
// Salida:	aio_pvVariants	=	NULL
//			return	= ninguno
void IntLN4::DestroyVariants(m4VariantType* &aio_pvVariants, int ai_iNumElem)
{
	if (aio_pvVariants) {
		//Borrar todas las cadenas del Pool para el Vector
		for (int i=0; i<ai_iNumElem; i++) {
			if (aio_pvVariants[i].Type == M4CL_CPP_TYPE_STRING_POOL)
				m_pStrPool->DeleteString( aio_pvVariants[i].Data.CharData );
		}
		delete [] aio_pvVariants;
		aio_pvVariants = NULL;
	}
}


//DEFINE para gestion de errores en la siguiente funcion
#define M4_CHECK_RES_INTLN4(A,X,Y,Z,H) { if (A) { DestroyVariants(Y,Z); if (X) delete [] X; if (H) delete [] H; return M4_ERROR; } }

// Funcion: Simular la ejecucion de un Item en la Virtual Machine
// Entrada:	ai_iLevel	= Nivel de VM
//			ai_pcChannel= Nombre del Canal
//			ai_pcNode	= Nombre del Nodo
//			ai_iRegIndex= No. de Registro
//			ai_pcItem	= Nombre del Item
//			ai_iSlice	= No. de Tramo
//			ai_iNumArgs	= No. de Argumentos
//			ai_iAtributo= No. de Atributo
// Salida:	return	= M4_ERROR | M4_SUCCESS
m4return_t IntLN4::ExecuteItem(int ai_iLevel, char *ai_pcChannel, char *ai_pcNode, int ai_iRegIndex, char *ai_pcItem, int ai_iSlice, int ai_iNumArgs, int ai_iAtributo)
{
	m4return_t		ret=M4_SUCCESS;

	int				i, j;								//Auxiliares
	m4VariantType	vTopPila;							//Tope de la Pila de Ejecucion
	char			pcArgDescription[M4CL_MAX_PARAM];	//Descripcion de los Argumentos de una Funcion
	m4int8_t		iNumArgs;							//Num.de Argumentos de una Funcion (segun la descripcion)
	int				iNumRealArgs;						//Num.de Argumentos reales (pasados)
	int				iNumRefArgs;						//Num.de Argumentos por Referencia
	m4int32_t		iLevel;								//Nivel de la Funcion

	m4VariantType	*vPilaArgs;							//Pila de Argumentos
	int				*vPilaFlagArgs;						//Pila de Flag de los Argumentos
	m4pcchar_t		pcRealItem;							//Nombre del Item Real

	//Reset Descripcion de los Argumentos
	memset(pcArgDescription, '\0', M4CL_MAX_PARAM);
	
	if ( !m_poTrad->IsMethodItem(ai_pcItem, ai_pcNode, ai_pcChannel, iNumArgs, iLevel, pcArgDescription,pcRealItem) )
		return M4_ERROR;	//No se encuentra canal!nodo.item

	//Averiguar el No.de argumentos con que se ha llamado para Ejecutar este item.
	if (iNumArgs < 0){
		//No.variable para los argumentos. El numero concreto de argumentos se encuentra en el top de la pila.
		ret=m_pilaEjecucion->Top(vTopPila);	if (ret!=M4_SUCCESS) return M4_ERROR;
		ret=m_pilaEjecucion->Pop();			if (ret!=M4_SUCCESS) return M4_ERROR;
		iNumRealArgs = int(abs((int)vTopPila.Data.DoubleData));
	} else {
		//No es Num.Variable de Argumentos. El numero de argumentos esta almacenado en iNumArgs.
		iNumRealArgs = iNumArgs;
	}

	//Reserva de memoria para almacenar los Argumentos de llamada y los Flags de los argumentos que indican si son Argumentos por Referencia
	vPilaArgs = new m4VariantType[iNumRealArgs];	//Argumentos de llamada
	if (!vPilaArgs)
		return M4_ERROR;
	vPilaFlagArgs = new int[iNumRealArgs];			//Indicador de argumento por Referencia
	if (!vPilaFlagArgs) {
		delete [] vPilaArgs;
		return M4_ERROR;
	}

	//Inicializacion de valores para los arrays....
	for (i = 0; i < iNumRealArgs; i++){
		vPilaArgs[i].Type = NULL;
		vPilaArgs[i].Data.DoubleData = 0.0;
		vPilaFlagArgs[i] = 0;
	}

	//Sacar los Argumentos de la Pila de Ejecucion: vPilaArgs y vPilaFlagArgs
	iNumRefArgs = 0;
	j=strlen(pcArgDescription);
	for (i=0; (i<j) && (i<iNumRealArgs); i++){
		if (pcArgDescription[i] == 'r'){
			iNumRefArgs++;
			vPilaFlagArgs[i] = 1;
		}
		// Hay que dar la vuelta a la pila
		ret=m_pilaEjecucion->Top(vPilaArgs[iNumRealArgs - i - 1]);
		if (ret!=M4_SUCCESS) {
			delete [] vPilaArgs;
			delete [] vPilaFlagArgs;
			return M4_ERROR;
		};
		ret=m_pilaEjecucion->Pop();
		if (ret!=M4_SUCCESS) {
			delete [] vPilaArgs;
			delete [] vPilaFlagArgs;
			return M4_ERROR;
		};
	}

	m4VariantType variantRet;		//Variant de Retorno de la Ejecucion
	m4VariantType oldVariantRet;	//Variant copia para Retorno de la Ejecucion

	//Buscar el Metodo
    //EDU:
	ClRegister objRegistro (0 /*sin autoload*/);
	if (((ClFinder_TICH *)(m_poTrad->m_poFinder))->FindMethod(ai_pcChannel, ai_pcNode, ai_pcItem, &objRegistro) != M4_SUCCESS ){
		delete [] vPilaArgs;
		delete [] vPilaFlagArgs;
		return M4_ERROR; 
	}

	int retSearch;				//Objeto Encontrado S/N
	int numRefArgsLeido;		//No. de Argumentos por Referencia del Objeto encontrado en la Tabla de Objetos
	m4VariantType	*vRefArgs=NULL;			//Vector de Argumentos por Referencia
	m4VariantType	*vRefArgsLeido=NULL;	//Vector de Argumentos por Referencia Leido
	int strSize;

	//Buscar en la Tabla de Objetos
	retSearch = m_tablaObj.searchKey(ai_pcChannel, ai_pcNode, ai_iRegIndex, ai_pcItem, ai_iSlice, ai_iAtributo, variantRet, numRefArgsLeido, vRefArgsLeido);
    
	if (retSearch){		//Referencia Encontrada

		if (numRefArgsLeido == iNumRefArgs){
			//Poner en pila los valores almacenados en la Tabla de Objetos: Ret + ArgRef

			//Primero poner el resultado
			if(variantRet.Type == M4CL_CPP_TYPE_STRING_POOL){
				//Si el Retorno es una cadena, hacemos una copia del Variant
				oldVariantRet.Type = M4CL_CPP_TYPE_STRING_POOL;
				ret=m_pStrPool->AddString(m_pStrPool->GetString(variantRet.Data.CharData), oldVariantRet.Data.CharData);
				M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
				ret=m_pilaEjecucion->Push(oldVariantRet);
			} else {
				ret=m_pilaEjecucion->Push(variantRet);
			}
			M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
			//Meter en Pila de Ejecucion los Argumentos por Referencia de la Tabla de Simbolos
			for (i=0; i < iNumRefArgs; i++){
				if(vRefArgsLeido[i].Type == M4CL_CPP_TYPE_STRING_POOL) {	//Si es cadena => Hacemos una copia
					oldVariantRet.Type = M4CL_CPP_TYPE_STRING_POOL;
					ret=m_pStrPool->AddString(m_pStrPool->GetString(vRefArgsLeido[i].Data.CharData), oldVariantRet.Data.CharData);
					M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
					ret=m_pilaEjecucion->Push(oldVariantRet);
				} else {
					ret=m_pilaEjecucion->Push(vRefArgsLeido[i]);
				}
				M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
			}//end-for
		} else {
			//Modificacion de la Tabla de Objetos.
			//No coincide el No.Argumentos por Referencia => Meter en Tabla de Objetos los valores recibidos 
			//en la Pila de Ejecucion. El resultado sera un valor por defecto.

			//Hay que borrar del Pool de Strings los Argumentos del Elemento leido a sustituir de la Tabla de Objetos
			for (i=0; i<numRefArgsLeido; i++) {
				if (vRefArgsLeido[i].Type == M4CL_CPP_TYPE_STRING_POOL) {
					m_pStrPool->DeleteString( vRefArgsLeido[i].Data.CharData );
				}
			}
			//Borrar el variantRet del Elemento
			if (variantRet.Type == M4CL_CPP_TYPE_STRING_POOL) {
				m_pStrPool->DeleteString( variantRet.Data.CharData );
			}
						
			//Creamos una Copia de Argumentos variables a partir de los Argumentos sacados de la Pila de Ejecucion
			vRefArgs = new m4VariantType[iNumRefArgs];
			M4_CHECK_RES_INTLN4(vRefArgs == NULL, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);

			for (i=0, j=0; i<iNumRealArgs; i++){
				if (vPilaFlagArgs[i] == 1){
					vRefArgs[j] = vPilaArgs[i];
					//Hay que hacer una copia al Pool de Strings
					if (vPilaArgs[i].Type == M4CL_CPP_TYPE_STRING_POOL) {
						m_pStrPool->AddString( m_pStrPool->GetString(vPilaArgs[i].Data.CharData), vRefArgs[i].Data.CharData );
					}
					j++;
				}
			}
			//Establecer valor del Resultado de la Ejecucion, y modificar la Tabla de Objetos
			AssignDefaultVariant(variantRet, objRegistro);
			strSize = StringSizeTS(ai_pcChannel, ai_pcNode, ai_iRegIndex, ai_pcItem, ai_iSlice, -1, variantRet, iNumRefArgs, vRefArgs);

			//Si el Retorno Default es una cadena, hacemos una copia del Variant
			if(variantRet.Type == M4CL_CPP_TYPE_STRING_POOL){
				oldVariantRet.Type = M4CL_CPP_TYPE_STRING_POOL;
				ret=m_pStrPool->AddString(m_pStrPool->GetString(variantRet.Data.CharData), oldVariantRet.Data.CharData);
				m_tablaObj.setValue(ai_pcChannel, ai_pcNode, ai_iRegIndex, ai_pcItem, ai_iSlice, -1, oldVariantRet, strSize, iNumRefArgs, vRefArgs);
			} else {
				m_tablaObj.setValue(ai_pcChannel, ai_pcNode, ai_iRegIndex, ai_pcItem, ai_iSlice, -1, variantRet, strSize, iNumRefArgs, vRefArgs);
			}
			//Meter en la Pila de Ejecucion el Resultado de la ejecucion
			ret=m_pilaEjecucion->Push(variantRet);
			M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);

			//Meter en la Pila de Ejecucion los Argumentos por Referencia que han llegado en la Pila de Ejecucion
			for (i=0; i<iNumRefArgs; i++){
				ret=m_pilaEjecucion->Push(vRefArgs[i]);
				M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
			}//end-for
			if (vRefArgs) {
				delete [] vRefArgs;
				vRefArgs=NULL;
			}
		}
	
	} else {
		//Alta en la Tabla de Objetos
		//No se encuentra en la Tabla de Objetos por lo tanto hay que añadir en la Pila
		//y en la Tabla de Objetos valores por defecto.
		AssignDefaultVariant(variantRet, objRegistro);

		//Para los Argumentos por Referencia hay que poner los valores recibidos en la llamada a la funcion.
		vRefArgs = new m4VariantType[iNumRefArgs];
		M4_CHECK_RES_INTLN4(vRefArgs == NULL, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
		for (i=0, j=0; i<iNumRealArgs; i++){
			if (vPilaFlagArgs[i] == 1){
				vRefArgs[j] = vPilaArgs[i];
				//Hay que hacer una copia al Pool de Strings
				if (vPilaArgs[i].Type == M4CL_CPP_TYPE_STRING_POOL) {
					m_pStrPool->AddString( m_pStrPool->GetString(vPilaArgs[i].Data.CharData), vRefArgs[j].Data.CharData );
				}
				j++;
			}
		}
        strSize = StringSizeTS(ai_pcChannel, ai_pcNode, ai_iRegIndex, ai_pcItem, ai_iSlice, ai_iAtributo, variantRet, iNumRefArgs, vRefArgs);
		//Si el Retorno Default es una cadena, hacemos una copia del Variant
		if(variantRet.Type == M4CL_CPP_TYPE_STRING_POOL){
			oldVariantRet.Type = M4CL_CPP_TYPE_STRING_POOL;
			ret=m_pStrPool->AddString(m_pStrPool->GetString(variantRet.Data.CharData), oldVariantRet.Data.CharData);
			m_tablaObj.addNew(ai_pcChannel, ai_pcNode, ai_iRegIndex, ai_pcItem, ai_iSlice, ai_iAtributo, oldVariantRet,strSize, iNumRefArgs, vRefArgs);
		} else {
			m_tablaObj.addNew(ai_pcChannel, ai_pcNode, ai_iRegIndex, ai_pcItem, ai_iSlice, ai_iAtributo, variantRet,strSize, iNumRefArgs, vRefArgs);
		}

		//Meter en la Pila de Ejecucion el resultado
		ret=m_pilaEjecucion->Push(variantRet);
		M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);

		//Meter en la Pila de Ejecucion los Argumentos por Referencia
		for (i=0; i<iNumRefArgs; i++){
			if(vRefArgs[i].Type == M4CL_CPP_TYPE_STRING_POOL){
				oldVariantRet.Type = M4CL_CPP_TYPE_STRING_POOL;
				ret=m_pStrPool->AddString(m_pStrPool->GetString(vRefArgs[i].Data.CharData), oldVariantRet.Data.CharData);
				M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
				ret=m_pilaEjecucion->Push(oldVariantRet);
			} else {
				ret=m_pilaEjecucion->Push(vRefArgs[i]);
			}
			M4_CHECK_RES_INTLN4(ret!=M4_SUCCESS, vPilaFlagArgs, vPilaArgs, iNumRealArgs, vRefArgs);
		}
		if (vRefArgs) {
			delete [] vRefArgs;
			vRefArgs = NULL;
		}
    }

	//Clean
	DestroyVariants(vPilaArgs, iNumRealArgs);
	if (vPilaFlagArgs)
		delete [] vPilaFlagArgs;
	
	return ret;
}

// Funcion: Construir las Tablas de Simbolos: Etiquetas y Objetos
// Entrada:	variables	=	Stream de Argumentos (Variables y Argumentos)
// Salida:	m_tablaVLoc	=	Tabla de Etiquetas
//			m_tablaObj	=	Tabla de Objetos
//			return	= M4_ERROR | M4_SUCCESS
m4return_t IntLN4::ConstruirTS(const char *ai_pcVariables)
{
	m4return_t ret=M4_SUCCESS;

	char *pcAuxVariables;	//Variables Auxiliares
	char *tkLinea;			//Token de Linea
	char *tkValor;
	char *tkValor2;
	int desp = 0;			//Offset desde la posicion inicial del array de variables
	int numRefArgs = 0;
    int strSize;

	int varId;				//Numero de Variable (en negativo)
	m4VariantType vValue;	//Valor de la Variable
	char *canal=NULL;
	char *nodo=NULL;
	char *item=NULL;
	int registro;
	int tramo;
	int atributo;				

	m4VariantType *refArgs=NULL; //, *refArgsAux;


	if (!ai_pcVariables)
		return M4_ERROR;

	//Copia de la variables, para manipular con strtok
	pcAuxVariables = new char[strlen(ai_pcVariables)+1];
	if (!pcAuxVariables)
		return M4_ERROR;
	strcpy(pcAuxVariables, ai_pcVariables);

	//Proceso del stream de Variables
	char *pszLasts;
	tkLinea = M4_strtok(pcAuxVariables, SEP, &pszLasts);
	
	while(tkLinea != NULL){
		//Actualizamos el Offset respecto del stream de variables original
		desp += strlen(tkLinea)+1;

		if (*tkLinea == 'A'){		//A#num_variable#valor_variable\12
			//Get Numero de Variable
			tkValor = M4_strtok(tkLinea+1, "#", &pszLasts);
			if ((tkValor == NULL) || (strcmp(tkValor, "") == 0)){
				delete [] pcAuxVariables;
				return M4_ERROR;;
			}
			varId=atoi(tkValor);

			//Get Valor de Variable
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if ( (strcmp(tkValor, " ") == 0) || (tkValor == NULL) || (strcmp(tkValor, "") == 0) ){
				//Hay un valor nulo, asignar NULL a la variable
				vValue.Type = M4CL_CPP_TYPE_NULL;
				vValue.Data.DoubleData=0.0;
			} else {
				//El valor que se ha leido de la cadena no es null, asignarlo a la nueva entrada de la TS
				AssignToVariant(vValue, tkValor);
			}
			if (ret == M4_ERROR){
				delete [] pcAuxVariables;
				return ret;
			}

			//Añadir nueva entrada a la Tabla de Simbolos de Variables Locales.
			strSize = StringSizeTS(varId, vValue);
			m_tablaVLoc.setValue(varId, vValue, strSize);	//Asignamos valor (modificamos o añadimos)

		} else if (*tkLinea == 'I'){	//Icanal#nodo#registro#item#tramo#atributo#valor_variant#No.Var.Ref#var1 var2 var3#\12
			canal=NULL;
			nodo=NULL;
			item=NULL;
			refArgs=NULL;

			//Get canal (si existe)
			tkValor = M4_strtok(tkLinea+1, "#", &pszLasts);
			if (tkValor==NULL){
				//Hay algún error con la cadena de entrada
				delete [] pcAuxVariables;
				return M4_ERROR;
			} else if (strcmp(tkValor, " ") != 0){
				canal=new char[strlen(tkValor)+1];
				strcpy(canal, tkValor);
			} else {
				canal=new char[1];
				*canal='\0';
			}
			
			//Get nodo (si existe)
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if (tkValor == NULL){
				//Hay algún error con la cadena de entrada
				delete [] pcAuxVariables;
				return M4_ERROR;
			} else if (strcmp(tkValor, " ") != 0){
				nodo=new char[strlen(tkValor)+1];
				strcpy(nodo, tkValor);
			} else {
				nodo=new char[1];
				*nodo='\0';
			}

			//Get valor de registro
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if (tkValor == NULL){
				//Hay algún error con la cadena de entrada
				delete [] pcAuxVariables;
				return M4_ERROR;
			} else if (strcmp(tkValor, " ") != 0){
				registro = atoi(tkValor);
			} else {
				registro = -2;	//un espacio
			}

			//Get valor del item
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if (tkValor == NULL){
				//Hay algún error con la cadena de entrada
				delete [] pcAuxVariables;
				return M4_ERROR;
			} else if (strcmp(tkValor, " ") != 0){
				item = new char[strlen(tkValor)+1];
				strcpy(item, tkValor);
			} else {
				item=new char[1];
				*item='\0';
			}

			//Get valor del tramo
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if (tkValor == NULL){
				//Hay algún error con la cadena de entrada
				delete [] pcAuxVariables;
				return M4_ERROR;
			} else if (strcmp(tkValor, " ") != 0){
				tramo = atoi(tkValor);
			} else {
				tramo = -1;	//un espacio
			}

			//Get valor del atributo (si esta)
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if (tkValor == NULL){
				//Hay algún error con la cadena de entrada
				delete [] pcAuxVariables;
				return M4_ERROR;
			} else if (strcmp(tkValor, " ") != 0){
				atributo = atoi(tkValor);
			} else {
				atributo = -1;	//un espacio
			}

			//Get Valor
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if ( (tkValor == NULL) || (strcmp(tkValor, " ") == 0) || (strcmp(tkValor, "") == 0) ){
				//Hay un valor nulo, asignar NULL a la variable
				vValue.Type = M4CL_CPP_TYPE_NULL;
				AssignToVariant(vValue, "");
				//AssignToVariant(vValue, NULL);
			} else {
				//El valor que se ha leido de la cadena no es null, asignarlo a la nueva entrada de la TS
				AssignToVariant(vValue, tkValor);
			}
			
			//Tomar informacion de los Argumentos por Referencia
			//Get Num.Arg.Ref
			tkValor = M4_strtok(NULL, "#", &pszLasts);
			if (tkValor == NULL){
				//problemas con la cadena de entrada
				delete [] pcAuxVariables;
				return M4_ERROR;
			} else if (strcmp(tkValor, "") == 0){
				numRefArgs = 0;	//No hay Arg.por Ref.
			} else {
				numRefArgs = atoi(tkValor);
			}

			//Ahora el string con los Argumentos por Referencia
			if (numRefArgs > 0){
				tkValor = M4_strtok(NULL, "#", &pszLasts);
				if (tkValor == NULL){
					//Problemas con la cadena de entrada
					delete [] pcAuxVariables;
					return M4_ERROR;
				}
				tkValor2 = M4_strtok(tkValor, SEP_ARGS, &pszLasts);
				refArgs = new m4VariantType[numRefArgs];
				if (!refArgs) {
					delete [] pcAuxVariables;
					return M4_ERROR;
				}
				for (int j=0; tkValor2!=NULL; j++) {
					AssignToVariant(refArgs[j], tkValor2);
					tkValor2 = M4_strtok(NULL, SEP_ARGS, &pszLasts);
				}
			}

			//Añadir nueva entrada a la Tabla de Simbolos de Objetos
			strSize = StringSizeTS(canal, nodo, registro, item, tramo, atributo, vValue, numRefArgs, refArgs);
			m_tablaObj.setValue(canal, nodo, registro, item, tramo, atributo, vValue, strSize, numRefArgs, refArgs);	//Ponemos valor o Añadimos

			//Liberamos memoria porque es temporal, el setValue NO se la queda (chapuza arreglada!!!)
			if (canal)
				delete [] canal;
			if (nodo)
				delete [] nodo;
			if (item)
				delete [] item;
			if (refArgs)
				delete [] refArgs;

		} else {
			if (*tkLinea != ' ') {
				delete [] pcAuxVariables;
				return M4_ERROR;
			}
			break;
		}

		//Cogemos la Siguiente Linea a procesar
		strcpy(pcAuxVariables, ai_pcVariables + desp * sizeof(char));
		tkLinea = M4_strtok(pcAuxVariables, SEP, &pszLasts);
	} //end-while

    
	delete [] pcAuxVariables;

	return M4_SUCCESS;
}


// Funcion: Escribir las Tablas de Simbolos: Etiquetas y Objetos
// Entrada:	Ninguna
// Salida:	ao_ppcOutput=	Puntero al Stream de Salida
//			return	= M4_ERROR | M4_SUCCESS
m4return_t IntLN4::EscribirTS(char** ao_ppcOutput)
{
	int longitudStr = m_tablaVLoc.contextSize + m_tablaObj.contextSize + 1;	//Longitud del Stream a generar
	
    if (m_pszVar!=NULL){
        delete []m_pszVar;
        m_pszVar=NULL;
    }
	m_pszVar= new char[longitudStr+1];
	
	char buffer[DIGITS*2];

	//Primero escribir las Variables Locales (Tabla de Variables Locales)
	memset(m_pszVar, '\0', longitudStr+1);
	TVL *runnerLoc;
	runnerLoc = m_tablaVLoc.head;
	while (runnerLoc != NULL){
		if (runnerLoc->orden < 0){
			//Variable:	A#orden#valor\12
			strcat(m_pszVar, "A#");
			sprintf(buffer, "%d", runnerLoc->orden);	//itoa(runnerLoc->orden, buffer, 10);
			strcat(m_pszVar, buffer);
			strcat(m_pszVar, "#");
			ConcatVariantIntoString(runnerLoc->valor, m_pszVar);
			strcat(m_pszVar, SEP);
		} //end-if
		runnerLoc = runnerLoc->next;
	} //end-while

	//Ahora los items, Variables Objeto (Tabla de Objetos)
	_TO *runnerObj;
	runnerObj = m_tablaObj.head;
	while (runnerObj != NULL){
		//Poner el Item:  I#canal#nodo#registro#item#tramo#atributo#valor#no.argref#arg1|arg2|...|\12
		strcat(m_pszVar, "I#");

		//Poner el canal
		if (strcmp(runnerObj->canal, "") == 0)
			strcat(m_pszVar, " ");
		else
			strcat(m_pszVar, runnerObj->canal);
		strcat(m_pszVar, "#");

		//Poner el nodo
		if (strcmp(runnerObj->nodo, "") == 0)
			strcat(m_pszVar, " ");
		else
			strcat(m_pszVar, runnerObj->nodo);
		strcat(m_pszVar, "#");

		//Poner el registro
		if (runnerObj->registro == -2){
			strcat(m_pszVar, " ");
		} else {
			sprintf(buffer, "%d", runnerObj->registro);
			strcat(m_pszVar, buffer);
		}
		strcat(m_pszVar, "#");

		//Poner el item. No puede ser vacío
		strcat(m_pszVar, runnerObj->item);
		strcat(m_pszVar, "#");

		//Poner el tramo
		if (runnerObj->tramo == -1){
			strcat(m_pszVar, " ");
		} else {
			sprintf(buffer, "%d", runnerObj->tramo);
			strcat(m_pszVar, buffer);
		}
		strcat(m_pszVar, "#");

		//Poner el atributo
		if (runnerObj->atributo == -1){
			strcat(m_pszVar, " ");
		} else {
			sprintf(buffer, "%d", runnerObj->atributo);
			strcat(m_pszVar, buffer);
		}
		strcat(m_pszVar, "#");

		//Poner valor
		ConcatVariantIntoString(runnerObj->valor, m_pszVar);
		strcat(m_pszVar, "#");

		//Poner el No.argumentos por referencia
		sprintf(buffer, "%d", runnerObj->numRefArgs);
		strcat(m_pszVar, buffer);
		strcat(m_pszVar, "#");

		//Poner los argumentos por referencia
		if (runnerObj->numRefArgs > 0){
			for (int ii=0; ii<runnerObj->numRefArgs; ii++){
				ConcatVariantIntoString(runnerObj->refArgs[ii], m_pszVar);	//Poner ArgRef
				strcat(m_pszVar, SEP_ARGS);									//Poner separador de ArgRef
			}		
		} else {
			//No hay argumentos por referencia se pone un blanco
			strcat(m_pszVar, " ");
		}
		strcat(m_pszVar, SEP);
		
		runnerObj = runnerObj->next;
	} //end-while
	
    *ao_ppcOutput=m_pszVar;
	return M4_SUCCESS;
}


// Funcion:	Convierte Fecha(String) a Fecha(Double)
// Entrada:	ai_pcDate	= Fecha en formato cadena
// Salida:	return		= Fecha (en double)
double IntLN4::DateToNumber(char *ai_pcDate)
{
	m4date_t retDate;
    ClStringToDate (ai_pcDate, strlen(ai_pcDate), retDate);
    return retDate;
}

// Funcion:	Convierte Fecha(Double) a Fecha(String)
// Entrada:	ai_dDate	= Fecha en formato double
// Salida:	ao_pcDate	= Fecha como cadena
//			ai_iLong	= Longitud max. del buffer
void IntLN4::NumberToDate(double ai_dDate, char *ao_pcDate, int ai_iLong)
{
	ClDateToString (ai_dDate,ao_pcDate,ai_iLong);
}

// Funcion:	Obtiene una SubCadena y su Longitud a partir de una Cadena de entrada
// Entrada:	ai_pcStringInput	= Cadena de Entrada a parsear
// Salida:	ao_pcStringOutput	= SubCadena de Salida. Se crea nueva memoria//Si !=NULL se borra
//			return				= Longitud de la subcadena creada
int IntLN4::GetString(const m4char_t *ai_pcStringInput, m4char_t* &ao_pcStringOutput)
{
	m4char_t *pcTemp = (m4char_t *)ai_pcStringInput;
	int i;

	//Destruimos el objeto anterior
	//if (ao_pcStringOutput)
	//	delete [] ao_pcStringOutput;

	//Calcular longitud de la cadena
	i=0;
	for (; pcTemp && pcTemp[i]!='\0' && pcTemp[i]!=' ' && pcTemp[i]!='\n'; i++);

	//Crear objeto
	ao_pcStringOutput = new m4char_t[i+1];
	if (!ao_pcStringOutput)
		return 0;
	if (pcTemp)
		memcpy(ao_pcStringOutput, pcTemp, i * sizeof(m4char_t) );
	ao_pcStringOutput[i] = '\0';

	return i;
}



// Funcion:	Calcula el tamaño de un Variant en el formato de la Tabla de Simbolos
// Entrada:	ai_variant	= Variant
// Salida:	return		= Tamaño del Variant en el formato de la tabla de Simbolos.
int IntLN4::VariantSizeTS(const m4VariantType &ai_variant)
{
	int iTamano = 0;
	char pcBuffer[M4CL_MAX_STRING+DIGITS+3];
	//Segun el Tipo de Variant
	switch(ai_variant.Type){
		case M4CL_CPP_TYPE_NUMBER:
			sprintf(pcBuffer, "%.8lf", ai_variant.Data.DoubleData);	//_gcvt(ai_variant.Data.DoubleData, DIGITS, doubleString);
			iTamano += strlen(pcBuffer);
			break;
		case M4CL_CPP_TYPE_STRING_POOL:
			iTamano += m_pStrPool->GetStringLength(ai_variant.Data.CharData);
			iTamano += 2; // Para las ""
			break;
		case M4CL_CPP_TYPE_DATE:
			NumberToDate(ai_variant.Data.DoubleData, pcBuffer, M4CL_MAX_TIMESTAMP_PRECISION+1);
			iTamano += strlen(pcBuffer);
			iTamano += 2; // Para las {}
			break;
		case M4CL_CPP_TYPE_NULL:
			iTamano ++;	//Cuando valor es null se pone un espacio en blanco
			break;
		}
	return iTamano;
}

// Funcion:	Obtener el Tamaño del Simbolo:	A#num_variable#valor_variable\12
// Entrada: ai_iNumVar	= No. de la variable
//			ai_vValor	= Variant con el valor de la variable
// Salida:	return		= Tamaño necesario para alojar el Simbolo
int IntLN4::StringSizeTS(int ai_iNumVar, const m4VariantType ai_vValor)
{
	int iTamano;
	char pcBuffer[DIGITS*2];
	
	//Contamos los caracteres especiales: 4
	//	A#... # .... \12

	sprintf(pcBuffer, "%d", ai_iNumVar);	//itoa(ai_iNumVar, numeroOrden, 10);

	//Formato del String:	A#num_variable#valor_variable\12
	iTamano = 4 + strlen(pcBuffer);
	iTamano += VariantSizeTS(ai_vValor);
	return (++iTamano);
}

// Funcion:	Obtener el tamaño del Simbolo: I#canal#nodo#registro#item#tramo#atributo#valor_variant#No.Var.Ref#var1|var2|var3\12
// Entrada: canal		= Nombre del Canal
//			nodo		= Nombre del Nodo
//			registro	= Index del Registro
//			item		= Nombre del Item
//			atributo	= Num.Atributo
//			valor		= Variant con el valor
//			numRefArgs	= Num.Arg. por Referencia
//			refArgs		= Array de Argumentos por Referencia
// Salida:	return		= Tamaño necesario para alojar el Simbolo
int IntLN4::StringSizeTS(const char *canal, const char* nodo, int registro, const char *item, int tramo, int atributo, const m4VariantType &valor, int numRefArgs, const m4VariantType *refArgs)
{
	int iTamano;
    char pcBuffer[M4CL_MAX_TIMESTAMP_PRECISION+DIGITS+3];

	iTamano = 9; //Contamos los caracteres especiales: 9
				//	I#  #   #   #   #   #   #   \12

	//Formato del String:	I#canal#nodo#registro#item#tramo#atributo#valor_variant#No.Var.Ref#var1|var2|var3\12
	if (!canal || strcmp(canal,"")==0){
		iTamano++;	//un espacio
	} else {
		iTamano += strlen(canal);
	}
	
	if (!nodo || strcmp(nodo,"")==0){
		iTamano++;	//un espacio
	} else {
		iTamano += strlen(nodo);
	}

	if (registro == -2){
		iTamano++;	//un espacio
	} else {
		sprintf(pcBuffer, "%d", registro);	//itoa(registro, pcBuffer, 10);
		iTamano += strlen(pcBuffer);
	}

	if (item)
		iTamano += strlen(item);

	if (tramo == -1){
		iTamano++;	//un espacio
	} else {
		sprintf(pcBuffer, "%d", tramo);		//itoa(registro, pcBuffer, 10);
		iTamano += strlen(pcBuffer);
	}

	if (atributo == -1){
		iTamano++;	//un espacio
	} else {
		sprintf(pcBuffer, "%d", atributo);	//itoa(registro, pcBuffer, 10);
		iTamano += strlen(pcBuffer);	
	}

	iTamano += VariantSizeTS(valor);

	//Añadir la parte de argumentos por referencia
	iTamano += 2;		//Esto es por los #
	iTamano += DIGITS;	//Esto es por el Num.Total de Var.Referencia
	if (numRefArgs > 0){
		for (int j=0; j<numRefArgs; j++)
			iTamano += VariantSizeTS(refArgs[j]);
	} else {
		iTamano++; //Si no hay variables por referencia se pondrá un blanco.
	}
	return (++iTamano);
}

// Funcion:	Convertir un Variant a formato Cadena
// Entrada:	ai_variant	= Variant a formatear
// Salida:	ai_pcString	= Buffer donde escribir el variant formateado segun su tipo
void IntLN4::ConcatVariantIntoString(const m4VariantType &ai_variant, char *aio_pcString)
{
	if (!aio_pcString)
		return;
	char buffer[M4CL_MAX_STRING+DIGITS+3];
	//Formatear segun el Tipo de Variant
	switch(ai_variant.Type){
		case M4CL_CPP_TYPE_NULL:
			strcat(aio_pcString, " ");
			break;
		case M4CL_CPP_TYPE_DATE:
			strcat(aio_pcString,"{");
			NumberToDate (ai_variant.Data.DoubleData,buffer,M4CL_MAX_TIMESTAMP_PRECISION);
			strcat(aio_pcString, buffer);
			strcat(aio_pcString,"}");
			break;
		case M4CL_CPP_TYPE_NUMBER:
			sprintf(buffer, "%.8lf", ai_variant.Data.DoubleData);	//_gcvt(runnerLoc->valor.Data.DoubleData, DIGITS, buffer);
			strcat(aio_pcString, buffer);
			break;
		case M4CL_CPP_TYPE_STRING_POOL:
			strcat(aio_pcString, "\"");
            if (m_pStrPool->GetString(ai_variant.Data.CharData)  != NULL )
                strcat(aio_pcString, m_pStrPool->GetString(ai_variant.Data.CharData) );
			strcat(aio_pcString, "\"");
			break;	
	}
}


// Funcion: Convierte un "string" delimitado en Variant
// Entrada:	ai_pcValor	= Valor en formato cadena
// Salida:	ao_vValor	= Valor en formato Variant
void IntLN4::AssignToVariant(m4VariantType &ao_variant, char *ai_pcValor)
{
	if (!ai_pcValor) {
		ao_variant.Type = M4CL_CPP_TYPE_NULL;
		ao_variant.Data.DoubleData = 0;
		return;
	}
	char *pcTemp = new char[strlen(ai_pcValor)+1];	//Hacemos una Copia local, para manipular
	if (!pcTemp)
		return;
	strcpy(pcTemp,ai_pcValor);
	if (pcTemp[0] == '"'){
		ao_variant.Type = M4CL_CPP_TYPE_STRING_POOL;
		pcTemp[strlen(pcTemp)-1] = '\0';							//Eliminar ultima  "
        m_pStrPool->AddString (pcTemp+1, ao_variant.Data.CharData);	//Eliminar primera "
	} else if (pcTemp[0] == '{'){
		ao_variant.Type = M4CL_CPP_TYPE_DATE;
		pcTemp[strlen(pcTemp)-1] = '\0';							//Eliminar ultima  }
		ao_variant.Data.DoubleData = DateToNumber(pcTemp+1);		//Eliminar primera {
	} else {
		ao_variant.Type = M4CL_CPP_TYPE_NUMBER;
		ao_variant.Data.DoubleData = atof(pcTemp);
	}
	delete [] pcTemp;
	return;
}

// Funcion:	Asignar un Variant con un Valor por defecto segun el 
//			Tipo M4 indicado en el Item M4CL_VM_ID_M4_TYPE_ITEM del sistema
// Entrada:	ai_objRegistro	= Registro del Nodo de Items (DesignChannel)
// Salida:	ao_variant		= Variant con Valor por defecto
void IntLN4::AssignDefaultVariant(m4VariantType &ao_variant, ClRegister objRegistro)
{
    m4VariantType vTemp;
    m4return_t ret;
	
	//Obtenemos el Valor del Item
    ret=objRegistro.Item[M4CL_VM_ID_M4_TYPE_ITEM].Value.Get (vTemp);
    if (ret!=M4_SUCCESS){
        ao_variant.Type = M4CL_CPP_TYPE_NULL;
        ao_variant.Data.DoubleData = 0.0;
        return;    
    }
	//Segun el Tipo M4 del Item
	switch((int)vTemp.Data.DoubleData){
		case M4CL_M4_TYPE_NUMBER:
		case M4CL_M4_TYPE_CURRENCY:
		case M4CL_M4_TYPE_NUM_VARIANT:
			ao_variant.Type = M4CL_CPP_TYPE_NUMBER;
			ao_variant.Data.DoubleData = 0.0;
			break;
		case M4CL_M4_TYPE_FIXED_STRING:
        case M4CL_M4_TYPE_VAR_STRING:
        case M4CL_M4_TYPE_UNI_VAR_STRING:
        case M4CL_M4_TYPE_LONG:
        case M4CL_M4_TYPE_UNI_LONG:
        case M4CL_M4_TYPE_BLOB:
        case M4CL_M4_TYPE_VARIANT:
        case M4CL_M4_TYPE_UNI_VARIANT:
        case M4CL_M4_TYPE_BINARY_STRING:
			ao_variant.Type = M4CL_CPP_TYPE_STRING_POOL;
			m_pStrPool->AddString ("",ao_variant.Data.CharData );
			break;
		case M4CL_M4_TYPE_DATE:
        case M4CL_M4_TYPE_TIMESTAMP:
		case M4CL_M4_TYPE_HOUR:
			ao_variant.Type = M4CL_CPP_TYPE_DATE;
			ao_variant.Data.DoubleData = DateToNumber ("1800-01-01");
			break;
		default:
			ao_variant.Type = M4CL_CPP_TYPE_NULL;
			ao_variant.Data.DoubleData = 0.0;
			break;		
	}
	return;
}


// Funcion:	Procesa una Instruccion de Polaca de Tipo Object, y obtiene los valores adecuados
//				void ScanLineObject(ai_pszIns, &ao_pcChannel, &ao_pcNode, &ao_iHasRowIndex, &ao_pcItem, 
//								&ao_iHasSlices, &ao_iAtributte, &ao_iLevel, &ao_iNumArg, &ao_iNumRegla)
// Entrada:	ai_pszIns		=	Cadena de Polaca
// Salida:	ao_pcChannel	=	Nombre del Canal	(se crea memoria)
//			ao_pcNode		=	Nombre del Nodo		(se crea memoria)
//			ao_iHasRowIndex	=	Indicador de Index
//			ao_pcItem		=	Nombre del Item		(se crea memoria)
//			ao_iHasSlices	=	Indicador de Tramos	
//			ao_iAtributte	=	Num.Atributo
//			ao_iLevel		=	Nivel
//			ao_iNumArg		=	Num.Argumentos
//			ao_iNumRegla	=	Num.Regla
//			return	= M4_SUCCESS | M4_ERROR
m4return_t IntLN4::ScanLineObject(const m4char_t *ai_pszIns, m4char_t* &ao_pcChannel, m4char_t* &ao_pcNode, int &ao_iHasRowIndex, m4char_t* &ao_pcItem, int &ao_iHasSlices, int &ao_iAtributte, int &ao_iLevel, int &ao_iNumArg, int &ao_iNumRegla)
{
	m4return_t	ret = M4_SUCCESS;
	m4char_t pszTemp[M4CL_MAX_LEN_NODE];

	//Default Values
	ao_pcChannel = NULL;
	ao_pcNode = NULL;
	ao_iHasRowIndex = -2;
	ao_pcItem = NULL;
	ao_iHasSlices = -1;
	ao_iAtributte = -1;
	ao_iLevel = 0;
	ao_iNumArg = 0;
	ao_iNumRegla = 0;
	
	m4VariantType variant;

	m4char_t *pcAct = (m4char_t *)ai_pszIns;

	//Segun el Tipo de Argumentos del Object
	m4bool_t bGoOn = M4_TRUE;
	while (bGoOn){
		switch (*pcAct) {
			case 'N':	//Nnodo hasRowIndex
				{
					//Nodo
					pcAct += GetString(pcAct+1,ao_pcNode) + 2;	// saltamos el espacio
					//hasRowIndex 0/1
					ao_iHasRowIndex=atoi(pcAct);
					if (ao_iHasRowIndex == 0) {
						ao_iHasRowIndex = -2;
					} else {
						//Numero de registro en la pila
						if ( (ret=m_pilaEjecucion->Top(variant)) == M4_ERROR)
							return ret;
						if ( (ret=m_pilaEjecucion->Pop()) == M4_ERROR)
							return ret;
						ao_iHasRowIndex = int(variant.Data.DoubleData);
					}
					pcAct++;
				}
				break;

			case 'I':	//Iitem hasSlices
				{
					//Item
					pcAct += GetString(pcAct+1,ao_pcItem) + 2;	// saltamos el espacio
					//hasSlices 0/1
					if (*pcAct=='1') {
						//ao_iHasSlices=1;	//Tiene Tramos
						//Numero de tramo en la pila
						if ( (ret=m_pilaEjecucion->Top(variant)) == M4_ERROR)
							return ret;
						if ( (ret=m_pilaEjecucion->Pop()) == M4_ERROR)
							return ret;
						ao_iHasSlices = int(variant.Data.DoubleData);						
					} else {
						ao_iHasSlices = -1;
					}
					pcAct++;
					if ( (*pcAct != '\n') && (*pcAct != '\0') )
						pcAct++;

					/*
					//Tomar Num.argumentos, si exite
					if (*pcAct == ' ') {
						pcAct++;
						if ((!isdigit(pcAct))){
							ao_iNumArg = 0;
						} else {
							ao_iNumArg = atoi(pcAct);
							pcAct++;
						}
					}
					*/
				}
				break;

			case 'A':	//Anumatributo
				{
					ao_iAtributte=atoi(pcAct+1);
					pcAct++; // saltamos la A
				}
				break;

			case 'L':	//Lnivel
				{
					ao_iLevel=atoi(pcAct+1);
					// jesus chapuza para salir al paso
					//if( ao_iLevel == 0 )
					//	ao_iLevel = 1;
					pcAct++; // saltamos la L
				}
				break;

			case 'R':	//Rnumregla
				{
					ao_iNumRegla = atoi(pcAct+1);
					sscanf(pcAct+1,"%s",pszTemp);
					pcAct+=strlen(pszTemp)+1;   //pasamos al final del token
					if ( (*pcAct!='\n') && (*pcAct!='\0') )
						pcAct++;
				}
				break;

			case 'F':	//F ???
				{
					pcAct++;
				}
				break;

			case 'C':	//Ccanal
				{
					pcAct += GetString(pcAct+1,ao_pcChannel) + 2;	// saltamos el espacio
				}
				break;

			case '\0':	//Fin de cadena
			case '\n':	//Fin de instruccion
				{
					bGoOn=M4_FALSE;
				}
				break;

			default:
				{
					pcAct++;
				}
				break;
		} //end-switch
	} //end-while

	//Default Values
	if (!ao_pcChannel) {
		ao_pcChannel = new m4char_t[1];
		*ao_pcChannel = '\0';
	}
	if (!ao_pcNode) {
		ao_pcNode = new m4char_t[1];
		*ao_pcNode = '\0';
	}
	if (!ao_pcItem) {
		ao_pcItem = new m4char_t[1];
		*ao_pcItem = '\0';
	}

	return M4_SUCCESS;
}



// Funcion:	Procesa e Instala el tipo de Instruccion Ox en la Tabla de Objetos, a partir del stream de Polaca
// Entrada:	ai_pszSentence	=	Puntero al stream de la Instruccion
//			aio_tablaTO		=	Tabla de Objetos
// Salida:	return			=	M4_ERROR | M4_SUCCESS
m4return_t IntLN4::InstallObject(const m4char_t *ai_pszSentence)
{
	m4return_t ret = M4_SUCCESS;

	if (!ai_pszSentence)
		return M4_ERROR;

	// Variables locales
    int atributo;
    char *canal=NULL;
	char *nodo=NULL;
    char *item=NULL;
    int level;
    int registro;
    int tramo;
	int numarg;
    int numregla;
	int numRefArgs=0;
	m4VariantType *refArgs=NULL;
	m4VariantType variant;
	m4VariantType oldVariant;
	int strSize;

	// Con O comienzan las sentencias que se refieren a las acciones con items (objetos)
	//		OR, OA, OF, OP, OT

	//		Opcionales:	Ccanal, Nnodo
	//		reg, si 0 no registro
	//			 si 1 registro, num registro en pila
	//		tramo, si 0 no tramo
	//			   si 1, num tramo en pila
	//


	switch(*ai_pszSentence){
		case 'R':
			{
				// Sentencia: OR Ccanal Nnodo reg Iitem tramo
				//	- poner en pila el item:	canal!nodo[reg].item[tramo]
				//
				// Meter copia de item en pila.
				//	1.- mirar en TS si el item esta en pila
				//		1.1.- si esta tomar y meter en pila
				//		1.2.- si no esta
				//			1.2.1.- poner valor por defecto (NULL)
				//			1.2.2.- añadir a TS
				//			1.2.3.- meter en pila

				//Obtiene el Canal, Nodo, Registro,Item y Tramo a partir de la Intruccion de Polaca
				//ret = partir(ai_pszSentence, canal, nodo, registro, item, tramo);
				ret = ScanLineObject(ai_pszSentence+1, canal, nodo, registro, item, tramo, atributo, level, numarg, numregla);
				if (ret == M4_ERROR)
					break;

				// Buscar la clave en la Tabla de Simbolos
				//	numRefArgs = 0;         \
				//							 ======> 2 ultimos parametros de searchKey
				//	refArgs = NO_ARGUMENTS; /
				if (m_tablaObj.searchKey(canal, nodo, registro, item, tramo, -1, variant, numRefArgs, refArgs)){
					if(variant.Type == M4CL_CPP_TYPE_STRING_POOL){
						oldVariant.Type = M4CL_CPP_TYPE_STRING_POOL;
						ret = m_pStrPool->AddString(m_pStrPool->GetString(variant.Data.CharData), oldVariant.Data.CharData);
						if (ret == M4_ERROR)
							break;
						ret = m_pilaEjecucion->Push(oldVariant);	//Push de una copia
					} else {
						ret = m_pilaEjecucion->Push(variant);
					}
					if (ret == M4_ERROR)
						break;
				} else {
					//No esta en la TS => valor por defecto o inputbox
					//EDU:
					ClRegister objRegistro (0 /*sin autoload*/);

					ret=((ClFinder_TICH *)(m_poTrad->m_poFinder))->FindItem(canal, nodo, item, &objRegistro);
					if (ret == M4_ERROR)
						break;
					AssignDefaultVariant(variant, objRegistro);
					strSize = StringSizeTS(canal, nodo, registro, item, tramo, -1, variant, 0, NO_ARGUMENTS);

					//Añadir copia al Pool si es necesario
					if(variant.Type == M4CL_CPP_TYPE_STRING_POOL){
						oldVariant.Type = M4CL_CPP_TYPE_STRING_POOL;
						m_pStrPool->AddString(m_pStrPool->GetString(variant.Data.CharData), oldVariant.Data.CharData);
						m_tablaObj.addNew(canal, nodo, registro, item, tramo, -1, oldVariant, strSize, 0, NO_ARGUMENTS);
					} else {
						m_tablaObj.addNew(canal, nodo, registro, item, tramo, -1, variant, strSize, 0, NO_ARGUMENTS);
					}
					ret = m_pilaEjecucion->Push(variant);
					if (ret == M4_ERROR)
						break;
				}
			}
			break;

		case 'A':
			{
				// Sentencia: OA Ccanal Nnodo reg Iitem tramo
				//	- asignar el valor de top de pila al item:	canal!nodo[reg].item[tramo]
				//
				// Meter top de la pila en item
				//	1.- mirar en TS si esta el item
				//		1.1.- si esta asignarle el valor del top
				//		1.2.- si no esta
				//			1.2.1.- crear nuevo item
				//			1.2.2.- asignarle el valor del top
				//			1.2.3.- añadir a TS
				//
				//ret = partir(ai_pszSentence, canal, nodo, registro, item, tramo);
				ret = ScanLineObject(ai_pszSentence+1, canal, nodo, registro, item, tramo, atributo, level, numarg, numregla);
				if (ret == M4_ERROR)
					break;
				
				//Sacar top de pila y meter en variant
				ret = m_pilaEjecucion->Top(variant);
				if (ret == M4_ERROR)
					break;
				ret = m_pilaEjecucion->Pop();
				if (ret == M4_ERROR)
					break;
				
				//Intentar asignar valor a un elemento de la TS para items.
				//Si devuelve 0 entonces añadir a la TS una nueva entrada
				
				//Si es un string, aqui hay que eliminar el antiguo valor que tenia en el Pool de strings
				//la variable que se esta asignando.
				strSize = StringSizeTS(canal, nodo, registro, item, tramo, -1, variant, 0, NO_ARGUMENTS);
				if (m_tablaObj.searchKey(canal, nodo, registro, item, tramo, -1, oldVariant, numRefArgs, refArgs)){
					if (oldVariant.Type == M4CL_CPP_TYPE_STRING_POOL){
						ret = m_pStrPool->DeleteString (oldVariant.Data.CharData);
					}
					m_tablaObj.setValue(canal, nodo, registro, item, tramo, -1, variant, strSize, 0, NO_ARGUMENTS);
				} else {
					m_tablaObj.addNew(canal, nodo, registro, item, tramo, -1, variant, strSize, 0, NO_ARGUMENTS);
				}
				if (ret == M4_ERROR)
					break;
				//curSentence++;
			}
			break;
			
		case 'F':
			{
				// Sentencia: OF Lnivel Ccanal Nnodo reg Iitem tramo numArg Rregla
				//	- ejecutar la funcion del item:	canal!nodo[reg].item[tramo]
				//
				//		Si no hay regla entonces la metaregla (regla = -1)
				
				//ret = partir(ai_pszSentence, nivel, canal, nodo, registro, item, tramo, numArg, regla);
				ret = ScanLineObject(ai_pszSentence+1, canal, nodo, registro, item, tramo, atributo, level, numarg, numregla);
				if (ret == M4_ERROR)
					break;

				//Ejecutamos el Item
				ret = ExecuteItem(level, canal, nodo, registro, item, tramo, numarg, -1);
				if (ret == M4_ERROR)
					break;
			}
			break;

		case 'P':
			{
				// Sentencia: OP Ccanal Nnodo reg Iitem tramo Aatributo
				//	- poner en pila el atributo del item canal!nodo[reg].item[tramo]
				//
				// Meter copia de atributo de item en pila.
				//	1.- mirar en TS si el esta atributo
				//		1.1.- si esta tomar y meter en pila
				//		1.2.- si no esta
				//			1.2.1.- poner valor por defecto (NULL)
				//			1.2.2.- añadir a TS
				//			1.2.3.- meter en pila
				
				//ret = partir(ai_pszSentence, canal, nodo, registro, item, tramo, atributo);
				ret = ScanLineObject(ai_pszSentence+1, canal, nodo, registro, item, tramo, atributo, level, numarg, numregla);
				if (ret == M4_ERROR)
					break;

				//Buscar clave en tabla de simbolos
				strSize = StringSizeTS(canal, nodo, registro, item, tramo, atributo, variant, 0, NO_ARGUMENTS);
				if (m_tablaObj.searchKey(canal, nodo, registro, item, tramo, atributo, variant, numRefArgs, refArgs)){
					if(variant.Type == M4CL_CPP_TYPE_STRING_POOL){
						oldVariant.Type = M4CL_CPP_TYPE_STRING_POOL;
						m_pStrPool->AddString(m_pStrPool->GetString(variant.Data.CharData), oldVariant.Data.CharData);
						ret = m_pilaEjecucion->Push(oldVariant);
					} else {
						ret = m_pilaEjecucion->Push(variant);
					}
				} else {
					//EDU:
					ClRegister objRegistro (0 /*sin autoload*/);
					((ClFinder_TICH *)(m_poTrad->m_poFinder))->FindItem(canal, nodo, item, &objRegistro);
					AssignDefaultVariant(variant, objRegistro);
					//Añadir copia al Pool si es necesario
					if(variant.Type == M4CL_CPP_TYPE_STRING_POOL){
						oldVariant.Type = M4CL_CPP_TYPE_STRING_POOL;
						m_pStrPool->AddString(m_pStrPool->GetString(variant.Data.CharData), oldVariant.Data.CharData);
						m_tablaObj.addNew(canal, nodo, registro, item, tramo, atributo, oldVariant, strSize, 0, NO_ARGUMENTS);
					} else {
						m_tablaObj.addNew(canal, nodo, registro, item, tramo, atributo, variant, strSize, 0, NO_ARGUMENTS);
					}
					ret = m_pilaEjecucion->Push(variant);
				}
				if (ret == M4_ERROR)
					break;
			}
			break;

		case 'T':
			{
				// Sentencia: OT Ccanal Nnodo reg Iitem tramo Aatributo
				//	- poner en pila el resultado de ejecutar el atributo-metodo del item:	canal!nodo[reg].item[tramo]

				//ret = partir(ai_pszSentence, nivel, canal, nodo, registro, item, tramo, atributo, numArg, regla);
				ret = ScanLineObject(ai_pszSentence+1, canal, nodo, registro, item, tramo, atributo, level, numarg, numregla);
				if (ret == M4_ERROR)
					break;

				//Ejecuta el Item
				ret = ExecuteItem(level, canal, nodo, registro, item, tramo, numarg, atributo);
				if (ret == M4_ERROR)
					break;
			}
			break;

		default:
			{
				ret = M4_ERROR;
			}
			break;

	}//end-switch de instrucciones del tipo 'Ox'

	//Liberamos memoria creada en "ScanLineObject"
	if (canal)
		delete [] canal;
	if (nodo)
		delete [] nodo;
	if (item)
		delete [] item;

	return ret;
}


// LocalTest:	Implementa la Prueba Local. Interpreta una polaca empleando los argumentos que se le pasan.
// Entrada:	ai_pszSourceCode	= Cadena con la Polaca
//			context				= Array de Argumentos
// Salida:	return				= M4_ERROR o M4_SUCCESS
m4return_t IntLN4::LocalTest(char *ai_pszSourceCode, char **context)
{
	m4return_t ret;

    M4_ASSERT(m_pilaEjecucion);
    M4_ASSERT(m_pStrPool);
	M4_ASSERT(m_poFuncTable);
    M4_ASSERT(m_poVM);
    M4_ASSERT(m_poTrad);

	//Inicializamos y Construimos las Tabla de Simbolos: Variables Locales y Objetos
	//a partir del stream de Argumentos Variables e Items
	Clean();
	ret = ConstruirTS(*context);
	if (ret == M4_ERROR) {
		Clean();
		return ret;
	}

	//La cadena de Polaca está formada por una serie de instrucciones que indican qué hay que realizar.
	//
	//Meto cada una de las instrucciones en un vector, de esta forma tendre mejor acceso 
	//a cada una de las instrucciones, tanto secuencialmente como en saltos.
	STRINGVECTOR polaca;
	ret = PushPolishInVector(ai_pszSourceCode, polaca);
	if (ret == M4_ERROR) {
		Clean();
		return ret;
	}

	//Crear el Vector de Etiquetas y darle un tamaño inicial.
	//Llamar a PreLocalTest para Localizar las Etiquetas,rellenar el vector y
	//chequear que todas las Funciones son validas para LocalTest
	INTVECTOR labelsID;
	labelsID.resize(10, -1);
	ret = PreLocalTest(polaca, labelsID);
	if (ret == M4_ERROR) {
		Clean();
		return ret;
	}

	
	// Analizar las Instrucciones de Polaca que se tienen almacenadas en el vector 'polaca'.
	// Si encontramos instrucciones JXX o jXX habrá que utilizar el vector con las etiquetas.

	int iCurSentence = 0;	//Numero de Instruccion actual
	char *pcSentence;		//Instruccion actual

	int strSize;	//Tamano del Simbolo en formator String
    int label;		//No. de Etiqueta
    int nivel;		//No. de Nivel
    int funcion;	//No. de funcion
    int variable;	//No. de Variable
    int numVeces;	//No. de ocurrencias
    m4VariantType variant;		//Variant aux
    m4VariantType oldVariant;	//Variant aux
    
    int numArg=0;

	//Interpretamos la Polaca
	while (iCurSentence < (int)polaca.size()){
		pcSentence = polaca[iCurSentence];
		switch(*pcSentence){

			case 'V':
				// Sentencia: Vnum	(Numero de variable locales)
				//	- no hacer nada
				//iNumLocalVars=atoi (pcSentence+1);
				iCurSentence++;
				break;

			case 'N':
				// Sentencia: Nnum	(cte.numerica)
				//  - introducir en pila el num
				variant.Type = M4CL_CPP_TYPE_NUMBER;
				variant.Data.DoubleData = atof(pcSentence+1);
				ret = m_pilaEjecucion->Push(variant);
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				iCurSentence++;
				break;

			case 'S':
				// Sentencia: Sstring	(cte.string)
				//	- introducir en pila string
				variant.Type = M4CL_CPP_TYPE_STRING_POOL;
				ret = m_pStrPool->AddString(pcSentence+1, variant.Data.CharData);
				ret = m_pilaEjecucion->Push(variant);
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				iCurSentence++;
				break;

			case 'D':
				// Sentencia: Ddate		(cte.fecha)
				//	-introducir en pila date
				variant.Type = M4CL_CPP_TYPE_DATE;
				variant.Data.DoubleData = atof(pcSentence+1);
				ret = m_pilaEjecucion->Push(variant);
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				iCurSentence++;
				break;

			case'C': 
				// Sentencia: Cnum	(Comentario de num lineas)
				//	- traslada puntero de ejecucion num veces !!! no lo hace
                iCurSentence++;	//numVeces = atoi(pcSentence+1); iCurSentence += numVeces;
				break;

			case 'P':
				// Sentencia: Pnum	(Limpiar pila)
				//	- purga pila de ejecucion num veces.				
				for (numVeces = atoi(pcSentence+1); numVeces>0; numVeces--){
					ret = m_pilaEjecucion->Top(variant);
					if (ret == M4_ERROR) {
						Clean();
						return ret;
					}
					if (variant.Type == M4CL_CPP_TYPE_STRING_POOL){
                        ret = m_pStrPool->DeleteString (variant.Data.CharData);
						if (ret == M4_ERROR) {
							Clean();
							return ret;
						}
					}
					ret = m_pilaEjecucion->Pop();
					if (ret == M4_ERROR) {
						Clean();
						return ret;
					}
				}
				iCurSentence++;
				break;

			case 'R':
				// Sentencia: Rnum
				//	- mete en la pila la variable num
				//
				//	Meter copia de variable en pila.
				//	1.- mirar en TS si esta la variable
				//		1.1.- si esta tomarla y meter en pila
				//		1.2.- si no esta
				//			1.2.1.- poner valor por defecto
				//			1.2.2.- añadir a TS
				//			1.2.3.- meter en pila
				variable = atoi(pcSentence+1);
				if (m_tablaVLoc.searchKey(variable, variant)){
					if (variant.Type == M4CL_CPP_TYPE_STRING_POOL){
						oldVariant.Type = M4CL_CPP_TYPE_STRING_POOL;
						m_pStrPool->AddString(m_pStrPool->GetString(variant.Data.CharData), oldVariant.Data.CharData);
						ret = m_pilaEjecucion->Push(oldVariant);
					} else {
						ret = m_pilaEjecucion->Push(variant);
					}
				} else {
					variant.Type = M4CL_CPP_TYPE_NULL;
                    variant.Data.DoubleData=0.0;
					strSize = StringSizeTS(variable, variant);
					m_tablaVLoc.addNew(variable, variant, strSize);
					ret = m_pilaEjecucion->Push(variant);
				}
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				iCurSentence++;
				break;

			case 'A':
				// Sentencia: Anum
				//	- asigna a la variable num el contenido del top de la pila
				//
				// Meter top de la pila en variable local
				//	1.- mirar en TS si esta la variable
				//		1.1.- si esta asignarle el valor del top
				//		1.2.- si no esta
				//			1.2.1.- crear nueva variable local
				//			1.2.2.- añadir a TS
				variable = atoi(pcSentence+1);

				//sacar top de pila y meter en variant
				ret = m_pilaEjecucion->Top(variant);
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				ret = m_pilaEjecucion->Pop();
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				
				//Si es un string, aqui hay que eliminar el antiguo valor que tenia en el pool de strings, 
				//la variable que se esta asignando.
				strSize = StringSizeTS(variable, variant);
				if (m_tablaVLoc.searchKey(variable, oldVariant)){
					if (oldVariant.Type == M4CL_CPP_TYPE_STRING_POOL){
						ret = m_pStrPool->DeleteString (oldVariant.Data.CharData);
						if (ret == M4_ERROR) {
							Clean();
							return ret;
						}
					}
					m_tablaVLoc.setValue(variable, variant, strSize);
				} else {
					m_tablaVLoc.addNew(variable, variant, strSize);
				}
				iCurSentence++;
				break;

			case 'L':
				// Sentencia: Lnum
				//	- no hace nada, las etiquetas ya estan establecidas
				iCurSentence++;
				break;

			case 'J':
				// Sentencia: Jnum
				//	- salta a etiqueta num si el contenido del top de la pila es 0.

				//Obtener el top de la pila para ver si hay que hacer el salto
				ret=m_pilaEjecucion->Top(variant);
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				ret = m_pilaEjecucion->Pop();
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				if(variant.Data.DoubleData == 0.0){
					//Top de pila tiene 0 => se ejecuta el salto
					label = atoi(pcSentence+1);
					iCurSentence = labelsID[label];
				} else
					iCurSentence++;	//No se ejecuta el salto
				break;

			case 'j':
				// Sentencia: jnum
				//	- salto incondicional a la etiqueta num
				label = atoi(pcSentence+1);
				iCurSentence = labelsID[label];
				break;

			case 'F':
				// Sentencia: Fnum nivel (antes) - Llamada a Funcion
				// Sentencia: Fnum nivel (ahora) - Llamada a Funcion
				//	- se invoca la funcion num de nivel	(el nivel es opcional !!! error)
				int i,j,k;	//Auxiliares
				char strFunc[DIGITS*2], strNivel[DIGITS*2];
				k=strlen(pcSentence);

				//Obtener el Numero de Funcion como string
				for (i = 0; (pcSentence[i]!=' ') && (i<k); i++){
					strFunc[i] = pcSentence[i];
				}
				strFunc[i] = '\0';

				//Obtener el Nivel como string
				for (j = i + 1; j<k; j++){
					strNivel[j - i - 1] = pcSentence[j];
				}
				strNivel[j - i - 1] = '\0';

				//Obtener el No.Funcion y el Nivel como numericos
				funcion = atoi(strFunc+1);
				nivel = atoi(strNivel); //Si *strNivel == '\0' nivel = 0; Nivel es opcional

				/*
				//miramos si viene con numero de argumentos o es el de por defecto de la función
				pszTemp=pcSentence+j;
				while(*pszTemp!=' ' && *pszTemp!='\n') pszTemp ++;
				if (*pszTemp==' '){
					iAuxInt2=atoi (pszTemp);
				}
				else{
					iAuxInt2=iAuxChar;	//Argumentos por defecto para la funcion
				}
				*/

				//Ejecutamos la Funcion
 				ret = ExecuteFunction(funcion, nivel);
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				iCurSentence++;
				break;

			case 'O':
				// Sentencia: Ox
				ret=InstallObject(pcSentence+1);
				if (ret == M4_ERROR) {
					Clean();
					return ret;
				}
				iCurSentence++;
				break;

			case 'E':
				// Sentencia: E
				//	- Return
				iCurSentence = polaca.size();
				break;

			case 'H':	//Funciones del Canal (nivel 2)
			default:
				return ret;	//return M4_ERROR;

		}//end-switch de instrucciones

	} //end-while (pcSentence)

	ret = EscribirTS(context);

	//Limpiar estructuras
	Clean();

    return ret;
}

//EDU:
m4uint32_t IntLN4::GetSize (void) const
{
    m4uint32_t length;

    length = sizeof (IntLN4);

    if (m_pszVar) {
        length += strlen (m_pszVar);
    }
	if (m_pSourceCode) {
        length += strlen (m_pSourceCode);
    }

    return length;
}


void IntLN4::Clean(void)
{
	//Borramos las Cadenas del Pool recorriendo la Lista de Variab.Locales
	for (TVL *poTVL=m_tablaVLoc.head; poTVL!=NULL; poTVL=poTVL->next) {
		if (poTVL->valor.Type == M4CL_CPP_TYPE_STRING_POOL)
			m_pStrPool->DeleteString (poTVL->valor.Data.CharData);
	}
	//Borramos las Cadenas del Pool recorriendo la Lista de Objetos
	for (_TO *poTO=m_tablaObj.head; poTO!=NULL; poTO=poTO->next) {
		if (poTO->valor.Type == M4CL_CPP_TYPE_STRING_POOL)
			m_pStrPool->DeleteString (poTO->valor.Data.CharData);
		//Borrar cadenas del Pool, de la Lista de Argumentos
		for (int i=0; i < poTO->numRefArgs; i++) {
			if (poTO->refArgs[i].Type == M4CL_CPP_TYPE_STRING_POOL)
				m_pStrPool->DeleteString (poTO->refArgs[i].Data.CharData);
		}
	}
	//Borramos las Listas
	m_tablaVLoc.Clean();
	m_tablaObj.Clean();
	//Borramos la Polaca segmentada
	if (m_pSourceCode) {
		delete [] m_pSourceCode;
		m_pSourceCode = NULL;
	}
}

