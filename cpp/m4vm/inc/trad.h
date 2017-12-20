//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:         compilador
// File:                trad.h
// Project:        Calculo
// Author:         Meta Software M.S. , S.A
// Date:            20 may 97
// Language:        C++ 
// Operating System: WINDOWS
// Design Document:  
//
//
// Definition:
//
//    This module defines la clase del traductor a polacas
////
//==============================================================================

#ifndef _CLTRAD_H
#define _CLTRAD_H


#include "cpdefine.hpp"
#include "gram_tab.h"

#include "vmtypes.hpp"
#include "vmdefine.hpp"

#include "m4stl.hpp"
#include "m4string.hpp"
#include "m4mdrt.hpp"



typedef set<string,less<string> > SetDependencias;
typedef SetDependencias::iterator itSetDepend;
//
//SYNONYMS

struct StSynonym
{
	m4char_t m_acOriginal[M4CL_MAX_LEN_ITEM+1];
	m4char_t m_acSynonym[M4CL_MAX_LEN_ITEM+1];
	m4bool_t m_bToChange;
	m4int32_t m_iOffset;
	m4int32_t m_iOffsetEnd;
	// En AIX, ObjectSpace necesita de estos operadores, para instanciar una pila.¿Por qué?. Ni idea.
	m4bool_t operator == (const StSynonym &ai_left)const;
	m4bool_t operator < (const StSynonym &ai_left)const;

};




/* Bug 0086408
Clase para no tener que utilizar la stack de las stl que aloca y desaloca
cuando se queda sin elementos
*/
class ClSynonymStack
{

protected:

	m4uint16_t	m_iSize ;
	m4uint16_t	m_iLength ;
	m4uint16_t	m_iDelta ;
	StSynonym   *m_pstElements ;


	void	_Alloc( void )
	{

		StSynonym   *pstTmp ;


		m_iSize += m_iDelta ;
		pstTmp = new StSynonym[ m_iSize ] ;

		if( m_pstElements != NULL )
		{
			if( m_iLength > 0 )
			{
				memcpy( pstTmp, m_pstElements, sizeof( StSynonym ) * m_iLength ) ;
			}

			delete [] m_pstElements ;
		}

		m_pstElements = pstTmp ;
	}


public:

		ClSynonymStack( m4uint16_t ai_iDelta )
		{
			if( ai_iDelta == 0 )
			{
				ai_iDelta = 1 ;
			}

			m_iSize = 0 ;
			m_iLength = 0 ;
			m_iDelta = ai_iDelta ;
			m_pstElements = NULL ;
		}

		~ClSynonymStack( void )
		{
			if( m_pstElements != NULL )
			{
				delete [] m_pstElements ;
				m_pstElements = NULL ;
			}
		}

	
	m4uint16_t	size( void ) const
	{
		return( m_iSize ) ;
	}

	m4uint16_t	length( void ) const
	{
		return( m_iLength ) ;
	}

	m4bool_t	empty( void ) const
	{
		return( m_iLength == 0 ) ;
	}

    void  pop( void )
    {
		if( m_iLength > 0 )
		{
			m_iLength-- ;
		}
    }

    const StSynonym&	top( void ) const
    {
	    return( m_pstElements[ m_iLength - 1 ] ) ;
    }

	void push( const StSynonym& ai_crstValue )
	{
		if( m_iLength >= m_iSize )
		{
			_Alloc() ;
		}
		m_pstElements[ m_iLength++ ] = ai_crstValue ;
	}

};


typedef map<m4int32_t,StSynonym,less<m4int32_t> > MapSynonym;
typedef MapSynonym::iterator					itMapSynonym;

enum m4DependentType {	DependentExecution=1, 
						DependentRead=2, 
						DependentAssign=3
};

class ClParser;
class ClAccess;
class ClRegister;
class ClRecordSet;
class IntLN4;
class ClAtributeFuncTable;
class ClFinder;
class ClAtributeFuncTable;
class ClFuncTable;

class ClTrad{


	public:
		ClTrad ();
		virtual ~ClTrad();

        m4uint32_t GetSize (void) const;

		m4return_t	Compile (m4char_t *ai_pszFormula);
		m4return_t  SetArguments( void );		
		m4return_t  SetArguments(m4char_t **ai_ppszArguments, m4bool_t *ai_pbReference, m4int8_t ai_iNumArgs, m4bool_t ai_bVar);		
		
		m4return_t	CompileRule (Compile_Type = M4_NO_SOURCE);
		void Reset();

		/*operaciones*/
		virtual m4return_t StoreNumber (char *ai_number);
		virtual m4return_t StoreString (char *ai_string);
		virtual m4return_t StoreDate (char *ai_date);
		virtual m4return_t LogOpAnd(void);
		virtual m4return_t LogOpOr(void);
		virtual m4return_t LogOpNot(void);
		virtual m4return_t LogOpEqual(void);
		virtual m4return_t LogOpNotEqual(void);
		virtual m4return_t LogOpGreater(void);
		virtual m4return_t LogOpGreaterEqual(void);
		virtual m4return_t LogOpLess(void);
		virtual m4return_t LogOpLessEqual(void);
		virtual m4return_t Assign(void);
		virtual m4return_t Return(void);
		virtual m4return_t AritmOpAdd(void);
		virtual m4return_t AritmOpSub(void);
		virtual m4return_t AritmOpMul(void);
		virtual m4return_t AritmOpDiv(void);
		virtual m4return_t AritmOpSigno(void);
		
		/*relativo a nodos,items...*/
		virtual m4return_t AtributoMetodo(char *ai_celda, char* ai_metodo, char *ai_nArgs);
		virtual m4return_t Atributo(char *ai_celda, char *ai_atributo);
		virtual m4return_t Metodo(char *ai_metodo,char* ai_nArgs);
		virtual m4return_t MetodoRegla(char *ai_metodo, char *ai_regla,char* ai_nArgs);
		virtual m4return_t MetodoPadre(char *ai_metodo,char* ai_nArgs);
		virtual m4return_t MetodoReglaPadre(char *ai_metodo, char *ai_regla,char* ai_nArgs);
		virtual m4return_t ChannelFunction (char *ai_channel, char *ai_function, char *ai_nArgs);
		virtual m4return_t ItemValorRead (char *ai_celda);
		
		/*relativo al control*/
		virtual m4return_t If(void);
		virtual m4return_t Then (void);
		virtual m4return_t Else (void);
		virtual m4return_t Elseif (void);
		virtual m4return_t Endif (void);
		virtual m4return_t Do (void);
		virtual m4return_t For (void);
		virtual m4return_t To (void);
		virtual m4return_t Next (void);
		virtual m4return_t While (void);
		virtual m4return_t Wend (void);
		virtual m4return_t Until (void);
		virtual m4return_t StoreFor(m4char_t *ai_identifier);
		virtual m4return_t FinFor(void);
		virtual m4return_t Purge(void);

		virtual m4return_t Target(char *ai_target);
		virtual m4return_t Condition(void);
		virtual m4return_t WriteLine(void);

        //relativo a los argumentos
        virtual m4return_t Arg(void);
        virtual m4return_t InitArgList(void);
        virtual m4uint32_t EndArgList (void);

        void SetError (void){
            bCompileError = M4_TRUE;
        }

		void DumpError   (m4int32_t ai_iNumError, m4pcchar_t ai_ErrorString1 = NULL, m4pcchar_t ai_ErrorString2 = NULL);
		void DumpWarning (m4int32_t ai_iNumError, m4pcchar_t ai_ErrorString1 = NULL, m4pcchar_t ai_ErrorString2 = NULL);

	private:

        m4bool_t        bCompileError;

		ClParser		*Parser;
		m4int8_t		m_iNumberOfArguments;
		m4int8_t		m_iNumOfRefArguments;
		m4int32_t		m_iPolishPos;		//posicion actual en la polaca
		char			*m_pszPolish;		//la polaca
											//OJO__>Para añadir cosas a la polaca SOLO
											// a traves de AddPolish
		m4int32_t		m_iTamPolish;		//tamaño del buffer de la polaca
		m4char_t		m_BufferTemp[M4CL_MAX_LINEA_POLACA];  //buffer temporal

		ClFuncTable		*m_oFuncTable; //Tabla de funciones (la semantica)
		

		m4ItemComp_t	m_oTargetItem;	//Cuando nos llega un TARGET lo descomponemos y lo ponemos aquí
										//para que cuando tengamos que asignar sepamos a que
		MapaSimbolos			m_oSymbolTable;
		MapaSimbolos			m_oAtributeTable; //tabla de atributos que existen
		ClAtributeFuncTable		*m_oAtributeMethodTable; //tabla de atributos-metodo que existen
		MapaSimbolosVariant		m_oMacroTable;
		ListaInt				m_oArgList;
		PilaInt					m_oLabelStack;
        PilaInt					m_oArgCallStack;

		m4int32_t		m_iNumEtiqueta;
		m4int32_t		m_iNumVar;	//Numero de variables definidas hasta el momento
		m4int32_t		m_iNumLine; //numero de linea de codificaión
		m4int32_t		m_iNumLineParser; //numero de linea NL en la que se encuentra el Syntax Error
		m4char_t		m_pszPushRefArg[M4CL_MAX_LINEA_POLACA*M4CL_MAX_PARAM];
		m4char_t		m_pszPushArgRefAtReturn[6*M4CL_MAX_PARAM];
		m4char_t		m_pszErrorString [M4CL_MAX_STRING_ERROR];
		m4int32_t		m_iLastArgRefStrSize;		//Tamaño de la última cadena de asignación de arg. por referencia (m_pszPushRefArg) de después de ejecución de método.
				//DEPENDENCIAS

	
		SetDependencias m_oInternalDepend;
		SetDependencias m_oExternalDepend;
		SetDependencias m_oChannelDepend;


		//SINONIMOS

		ClSynonymStack	m_oSynonymStack;			
		MapSynonym		m_oSynonymMap;			// Utilizo un mapa, por si acaso no me vinieran ordenadas las llamadas del parser. ( La PK es la posicion ).
		Compile_Type 	m_eSourceTo;
		m4bool_t		m_bSynonymError;
		m4int32_t		m_iLenLN4;
		m4int32_t		m_iExtraMemorySynonym;
		m4pchar_t		m_pszLN4FinalCode;
		m4bool_t		m_bSynonymFound;

		//JIT
		ClFinder		*m_poFinder;
	
		//Tablas de símbolos->Desde fuera nos ponen el sitio donde dejar los simbolos si corresponde
		m4char_t		**m_ppszSymbols;
		m4uint32_t		m_iSymbolsTableSize;
		m4uint32_t		m_iSymbolSize;


		
		//***************
		//Utiles diversos
		//***************
	public:
		m4return_t SetAccess (ClAccess * ai_poAccess, m4uint32_t ai_hItem, m4uint8_t ai_iRuleLevel );
		ClFinder * GetpFinder(void) { return m_poFinder;}
		ClParser * GetpParser(void) { return Parser;}
		m4bool_t   ErrorActive(void) { return bCompileError; }

        //establece el RS. Debe tener un bloque "atachado"
		void SetRecordSet ( ClAccessRecordSet * ai_poRecordSet );

		void SetRegIndex ( ClRegisterIndex ai_oRegIndex );

		void SetSourceMode(Compile_Type ai_cSourceMode);

		void SetSymbolsTableSpace(m4char_t **ai_ppszSymbols, m4uint32_t ai_iSymbolsTableSize, m4uint32_t ai_iSymbolSize );

		m4return_t 		PopIdentifier(void);
		void			PushIdentifier(m4pchar_t ai_pOriginal,m4int32_t ai_offset,m4int32_t ai_ilen);
		
		//sinonimos
		m4bool_t SynonymTreatment(void);
		// Item --> Synonym
		m4return_t		NotifyPosibleItem(void);	// Puede ser item o variable.
		m4return_t		AddItemSynonym(ClRegister *oRegItem,m4pchar_t ai_pcItem);
		void			DeleteItemSynonym(m4pchar_t ai_pcItem,m4bool_t ai_bToChange);

		// old_ti -->id_ti
		m4return_t		NotifyPosibleTI(void);
		m4return_t		AddTISynonym(m4pcchar_t ai_pAlias,m4pcchar_t ai_pIdTI);

		Compile_Type	GetSourceMode(void);
		void			UpdateFinder(ClFinder *ai_Finder) { m_poFinder = ai_Finder;}


		m4int32_t	GetNumLineParser (void);
		m4int32_t   GetNumLine (void);

		char *		GetPolishCode (void) { 
			return m_pszPolish;	// Devuelve un puntero al buffer donde esta la Polaca 
		}

		m4return_t	SetCurrentRuleInfo( m4bool_t ai_bTakeInfoFromTiChannel, const m4char_t *ai_pszChannel=0, const m4char_t *ai_pszTi=0,const m4char_t *ai_pszItem=0, const m4char_t *ai_pszRule=0 );
		const m4char_t	*GetCurrentItem(void){
			return m_acCurrentItem;
		}
		const m4char_t	*GetCurrentRule(void){
			return m_acCurrentRule;
		}
		const m4char_t	*GetCurrentTi(void){
			return m_acCurrentNode;
		}
		const m4char_t	*GetCurrentChannel(void){
			return m_acCurrentChannel;
		}

	private:
		//establecer argumentos
		m4return_t  AddArgument         (m4char_t *ai_argument,m4bool_t ai_bReference);

		//scannear celda
		m4return_t  ScanCell            (m4char_t *ai_item,m4ItemComp_t	&ao_oRet);
		//simbolos
		m4return_t  AddSymbol           (m4char_t *ai_symbol,m4bool_t ai_bIsSymbol=M4_TRUE);
		m4int32_t   GetSymbol           (m4char_t *ai_symbol);
		
		//macros
		m4return_t		AddMacro        (m4char_t *ai_pszName,m4VariantType ai_iVal);
		m4return_t		AddMacro        (m4char_t *ai_pszName,m4int32_t ai_iVal);
		m4bool_t		IsMacro         (m4char_t *ai_pszName);
		m4bool_t		GetMacro        (m4char_t *ai_pszName,m4VariantType &ao_iVal);
		
		//atributos
		m4return_t  AddAtribute         (m4char_t *ai_pszName,m4int32_t ai_iVal);
		m4bool_t    IsAtribute          (m4char_t *ai_pszName);
		m4int32_t   GetAtribute         (m4char_t *ai_pszName);
		
		//atributos-metodo
		m4return_t  AddAtributeMethod   (m4char_t *ai_pszName, m4int8_t ai_iNumArg, m4uint16_t ai_iId, m4char_t *ai_acParamDesc, m4bool_t ai_bGenDep);
		m4bool_t    IsAtributeMethod    (m4char_t *ai_pszName);
		m4int32_t   GetAtributeMethod   (m4char_t *ai_pszName);
		m4return_t  CheckAtributeMethod (m4char_t *ai_Atribute, m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t ai_nArgs, m4int8_t &ai_iRealArgs,m4pcchar_t &ao_pRealItem);

		//checks
		m4return_t  CheckFunction       (m4char_t *ai_acName,m4uint16_t &ai_id, m4int8_t ai_iNumArgsPassed,m4int8_t &ai_iRealArgs, m4bool_t &ao_rbIsChannel);
		m4return_t  CheckArgs           (m4int8_t ai_iNumArgsReal,m4int8_t ai_iNumArgsPassed, m4char_t * ai_acArgDesc, m4char_t * ai_pszErrorString);
		m4return_t	CheckSimpleArg      (m4char_t ai_cType);
		m4return_t  CheckChannelFunction(m4char_t *ai_acName,m4uint16_t &ai_id, m4int8_t ai_iNumArgsPassed, m4int8_t &ao_iRealArgs);
		
		//existencia
		m4bool_t    IsItem              (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4pcchar_t &ao_RealItem);
		m4bool_t    IsMethodItem        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem);
		m4bool_t    IsRule              (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule);
		m4bool_t    IsFatherMethod      (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel, m4char_t *ao_acArgDes,m4pcchar_t &ao_pcRealItem);
		m4bool_t    IsFatherRule        (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel, m4char_t *ai_Rule);

		//checkeos varios
		m4return_t  CheckMethodItem     (m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4int8_t ai_nArgs, m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pcRealItem, m4bool_t &ao_ItemFound);
										//si ao_ItemFound==FALSE  a la salida, es que el item no existía, para poder sacar el mansaje adecuado
		m4return_t  CheckMethodRule     (m4char_t *ai_Rule, m4char_t *ai_Item, m4char_t *ai_Node, m4char_t *ai_Channel,m4int8_t ai_nArgs,m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pcRealItem);
		m4return_t  CheckFatherMethod   (m4char_t *ai_Item, m4char_t *ai_Node , m4char_t *ai_Channel,m4int8_t ai_nArgs,m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pcRealItem);
		m4return_t  CheckFatherMethodRule(m4char_t *ai_Rule, m4char_t *ai_Item, m4char_t *ai_Node , m4char_t *ai_Channel, m4int8_t ai_nArgs,m4int8_t &ai_iRealArgs, m4int32_t &ao_iLevel,m4pcchar_t &ao_pcRealItem);

		//establecer dependencias

		m4return_t	AddDepend(m4pcchar_t ai_pszItemId,m4ItemComp_t &ai_oItemId, m4DependentType ai_eDependentType);
		m4return_t	UpdateInternalDepend(void);
		m4return_t	UpdateExternalDepend(void);
		m4return_t  UpdateChannelDepend(void);
		
		//sinonimos

		m4return_t UpdateRealSynonym(m4pcchar_t ai_acPosSynon,m4pcchar_t ai_acSyonym);
		m4return_t UpdateSynoynms(m4pchar_t);
		m4return_t CheckToUpdateSynonyms(m4pchar_t ai_InCode,m4bool_t ai_bSetItemHasSynonym);
		m4return_t ChangeToSynonym(m4pchar_t ai_pszSource,m4int32_t ai_Offset,m4pchar_t ai_pszSynonym,m4int32_t m_iOffsetEnd);
		void	   _DeleteLastSynonym(m4pcchar_t ai_pitem,m4bool_t ai_bToChange);

		


	private:		
		
		//manejo de polaca
		m4return_t  ResizePolish		(void);
		m4return_t  AddPolish           (m4char_t *ai_buffer);
		m4return_t  ReadLine            (m4char_t *ai_pszString,m4char_t *ao_pszLine, m4int32_t ai_iTam);
		m4return_t  SetPolishInfo       (void);
		m4return_t  BasicItem2String    (m4ItemComp_t ai_oItem, m4pcchar_t ai_RealItem,m4char_t *ao_pszItem, m4int32_t ai_iLong  );

		m4return_t  GenerateBasicFunctionCall (m4char_t *ai_pszString);
        friend class IntLN4;
		
		m4char_t m_acCurrentItem[M4CL_MAX_LEN_ITEM+1];
		m4char_t m_acCurrentNode[M4CL_MAX_LEN_NODE+1];
		m4char_t m_acCurrentChannel[M4CL_MAX_LEN_CHANNEL+1];
		m4char_t m_acCurrentRule[M4CL_MAX_LEN_RULE+1];



};



#endif
