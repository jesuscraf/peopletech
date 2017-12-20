#ifndef __INTLN4_CPP__
#define __INTLN4_CPP__

#include "m4stl.hpp"
#include "m4string.hpp"
#include "fixstackvm.hpp"
#include "cltypes.hpp"
class ClStringPool;
class ClFuncTable;
class ClVMachine1;
class ClTrad;
#include "tablasimbolos.hpp"
#include "cldates.hpp"

//#ifdef _UNIX
//#include "m4uxwin1.hpp"
//#endif

//using namespace std ;

typedef vector<char*> STRINGVECTOR;
typedef vector<long> LONGVECTOR;
typedef vector<int> INTVECTOR;

#define DIGITS				20
#define SEP					"\12"
#define SEP_ARGS			"|"
#define NUMBER_NO_ARGUMENTS	0
#define NO_ARGUMENTS		NULL

class IntLN4{
	/*
	 * Attributes
	 */
	protected:
		LinkedTVL                   m_tablaVLoc;	// Lista enlazada de Variables Locales
		LinkedTO                    m_tablaObj;		// Lista enlazada de Objetos
		tpFixedStack<m4VariantType> *m_pilaEjecucion;//Pila de Ejecucion (de variants)
        ClStringPool	            *m_pStrPool;	// Pool dinamico
        ClFuncTable                 *m_poFuncTable;	// Tabla de Funciones
        ClVMachine1                 *m_poVM;		// Virtual Machine 1
        char                        *m_pszVar;		// Stream de Argumentos Variables y Objectos formateado
        ClTrad                      *m_poTrad;		// Trad asociado
		char						*m_pSourceCode;	// Polaca delimitada

	/*
	 * Methods
	 */
		void		DestroyVariants(m4VariantType* &aio_pvVariants, int ai_iNumElem);
		m4return_t	PreLocalTest(const STRINGVECTOR &ai_polaca, INTVECTOR &ao_labels);
		m4return_t	PushPolishInVector(const char *ai_pcPolishCode, STRINGVECTOR &ao_polaca);
		m4return_t	ExecuteFunction(m4uint16_t ai_ui16NumFunc, int ai_iLevel);
		m4return_t	ExecuteItem(int ai_iLevel, char *ai_pcChannel, char *ai_pcNode, int ai_iRegIndex, char *ai_pcItem, int ai_iSlice, int ai_iNumArgs, int ai_iAtributo);
		m4return_t	ConstruirTS(const char *ai_pcVariables);
		m4return_t	EscribirTS(char** ao_ppcOutput);
		double		DateToNumber(char *ai_pcDate);
		void		NumberToDate(double ai_dDate, char *ao_pcDate, int ai_iLong);
		int			GetString(const m4char_t *ai_pcStringInput, m4char_t* &ao_pcStringOutput);
		int			VariantSizeTS(const m4VariantType &ai_variant);
		int			StringSizeTS(int ai_iNumVar, const m4VariantType ai_vValor);
		int			StringSizeTS(const char *canal, const char* nodo, int registro, const char *item, int tramo, int atributo, const m4VariantType &valor, int numRefArgs, const m4VariantType *refArgs);
		void		ConcatVariantIntoString(const m4VariantType &ai_variant, char *aio_pcString);
		void		AssignToVariant(m4VariantType &ao_variant, char *ai_pcValor);
		void		AssignDefaultVariant(m4VariantType &ao_variant, ClRegister objRegistro);
		
		m4return_t	ScanLineObject(const m4char_t *ai_pszIns, m4char_t* &ao_pcChannel, m4char_t* &ao_pcNode, int &ao_iHasRowIndex, m4char_t* &ao_pcItem, int &ao_iHasSlices, int &ao_iAtributte, int &ao_iLevel, int &ao_iNumArg, int &ao_iNumRegla);
		m4return_t	InstallObject(const m4char_t *ai_pszSentence);
		void		Clean(void);

	public:
		m4return_t	LocalTest(char *ai_pszSourceCode, char **context);
       
        void SetRTS(tpFixedStack<m4VariantType> *ai_pRTS){
            m_pilaEjecucion=ai_pRTS;
        }

        void SetStrPool (ClStringPool *ai_pStrPool){
            m_pStrPool=ai_pStrPool;
        }

        void SetFuncTable (ClFuncTable  *ai_poFuncTable){
            m_poFuncTable=ai_poFuncTable;
        }

        void SetVM (ClVMachine1 *ai_poVM){
            m_poVM=ai_poVM;
        }

        void SetTrad (ClTrad *ai_poTrad){
            m_poTrad=ai_poTrad;
        }


        IntLN4(void){
            m_pilaEjecucion=NULL;
            m_pStrPool=NULL;
            m_poFuncTable=NULL;
            m_poVM=NULL;
            m_pszVar=NULL;
			m_poTrad=NULL;
			m_pSourceCode=NULL;
        }
        
		~IntLN4(void){           
            if (m_pszVar!=NULL){
                delete [] m_pszVar;
                m_pszVar=NULL;
                
            }
			Clean();
        }
        
        //EDU:
		m4uint32_t	GetSize (void) const;
};

#endif
