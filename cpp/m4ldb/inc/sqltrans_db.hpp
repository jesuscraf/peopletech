#ifndef _SQLTRANS_DB_HPP_
#define _SQLTRANS_DB_HPP_

#include "sqltrans.hpp"
#include "m4stl.hpp"

// Oracle. Traductor por defecto.
class ClTranslatorUtils;
class	ClSQLTranslator_Oracle : public _AClSQLTranslator
{
public:
	ClSQLTranslator_Oracle (ClMetaSentence *ai_pMtSent,ClGlobVars *ai_pGlobVars,ClTranslatorUtils * ai_pTrUtils, m4bool_t ai_bIsSecurityView, string* ai_psTranslatedSQL)
		:	_AClSQLTranslator(ai_pMtSent, ai_pGlobVars, ai_pTrUtils, ai_bIsSecurityView, ai_psTranslatedSQL){}

	virtual	m4return_t		TranslateTable(string &ao_stTrans,m4int32_t ai_TableIndex,m4int32_t ai_precIndex){
		// Se traducen en el From.
		return M4_ERROR;
	}

	//Cada gestor sabrá como traducir los hints.
	m4return_t	TranslateAutomaticSysHints( eClauseType_t ai_eClause, m4int32_t ai_iLevel, string & aio_sTrans );
	m4return_t	TranslateHintBySysHintId( m4int32_t ai_iFilter, const string & ai_sSysHintId, string & aio_sTrans );
	m4return_t	TranslateSysHintsBegin( string & aio_sTrans );
	m4return_t	TranslateSysHintsEnd( string & aio_sTrans );
	m4return_t	ArrangeSysHintsBeginEnd( string & aio_sTrans, string & aio_sRealHint );

	m4return_t	MergeSysHints( const string & ai_sSourceString, string & aio_sSysHintTranslated, m4int32_t & aio_iPosBegin);

protected:
	m4return_t _RemoveSysHintsBegin( string & aio_sTrans, m4bool_t & a_bRemoved );
	m4return_t _RemoveSysHintsEnd( string & aio_sTrans, m4bool_t & ao_bRemoved );

protected:
	virtual m4return_t		_RemoveOuterSigns	(string &ao_stTrans){return M4_SUCCESS;}

};
/***********************************************************************************
	SQLServer. Tratamiento de :
	- Outer Joins	--> TranslateFrom, TranslateRelations, Translate RealJoins y Final (~F(+)=?)
	- NVL			--> Se traduce por ISNULL
						-->	TranslateField.
	- For Update	--> Se traduce por ROWLOCK UPDLOCK y se pone junto a cada tabla.
						-->	TranslateFrom.
***********************************************************************************/

class	ClSQLTranslator_SQLServer : public _AClSQLTranslator
{
public:
	ClSQLTranslator_SQLServer (ClMetaSentence *ai_pMtSent,ClGlobVars *ai_pGlobVars,ClTranslatorUtils* ai_pTrUtils, m4bool_t ai_bIsSecurityView, string* ai_psTranslatedSQL)
		:	_AClSQLTranslator(ai_pMtSent, ai_pGlobVars, ai_pTrUtils, ai_bIsSecurityView, ai_psTranslatedSQL){}

	virtual m4return_t		TranslateSentenceAfter(string &ao_stTrans);
	virtual	m4return_t		TranslateTable(string &ao_stTrans,m4int32_t ai_TableIndex,m4int32_t ai_precIndex);
	virtual	m4return_t		TranslateCycleRelations		(string &ai_stTrans,m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2);
	virtual	void			AppendOJinTables_begin(string &ao_stTrans);
	virtual	void			AppendOJinTables_end(string &ao_stTrans);
	virtual	void			AppendIJinTables_begin(string &ao_stTrans);
	virtual	void			AppendIJinTables_end(string &ao_stTrans);

	//Bug 0077247. Una coma de mas en informix
	virtual	void			EraseOJinTables_begin(string &ao_stTrans);
	virtual	void			EraseOJinTables_end(string &ao_stTrans);
	virtual	void			EraseIJinTables_begin(string &ao_stTrans);
	virtual	void			EraseIJinTables_end(string &ao_stTrans);

protected:
	virtual m4return_t		_TranslateFrom			(string &ai_stTrans,m4int32_t ai_iLevel);
	virtual	m4return_t		_TranslateRelations		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere);
	virtual m4return_t		_TranslateRealJoins		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere);
	virtual m4return_t		_TranslateRealJoins		(string &ai_stTrans,ClMetaObject *ai_pObj,m4bool_t &bAddedWhere);
	virtual	m4return_t		_TranslateSpecialRelations		(string &ai_stTrans,m4int32_t ai_iLevel,m4bool_t &bAddedWhere);


private:
	m4return_t		_TranslateFrom_SQLServer(string &ai_stTrans, m4int32_t ai_iLevel);
	m4return_t		_TranslateRealJoins		(string &ai_stTrans, m4int32_t ai_ObjectIndex, m4int32_t ai_RealTable1, m4int32_t ai_RealTable2, m4uint32_t ai_iIsTranslation);
	m4return_t		_TranslateRelations		(string &ao_stTrans, m4int32_t ai_TableIndex, m4int32_t ai_precIndex);
	m4return_t		_RemoveOuterFromWhere	(string &ao_stTrans);
	m4int32_t		_FindFisicTableWithRelation(m4int32_t ai_TableIndex1, m4int32_t ai_TableIndex2);

};

/***********************************************************************************
	Informix. Tratamiento de :
	- Outer Joins	--> TranslateFrom, TranslateRelations, Translate RealJoins y Final (~F(+)=?)
	- For Update	--> No se permite junto con Order By. Separarlo en dos sentencias.
						--> Final.
	- Order By /	--> Deben estar como columnas, y hay que indicarlo por indice de columna, por el caso en que haya funciones.
	  Group By			--> Final.
								

***********************************************************************************/
class	ClSQLTranslator_Informix : public _AClSQLTranslator
{
public:
	ClSQLTranslator_Informix (ClMetaSentence *ai_pMtSent,ClGlobVars *ai_pGlobVars,ClTranslatorUtils	* ai_pTrUtils, m4bool_t ai_bIsSecurityView, string* ai_psTranslatedSQL)
		:	_AClSQLTranslator(ai_pMtSent, ai_pGlobVars, ai_pTrUtils, ai_bIsSecurityView, ai_psTranslatedSQL){}

	virtual m4return_t		TranslateSentenceAfter(string &ao_stTrans);
	virtual	m4return_t		TranslateTable(string &ao_stTrans,m4int32_t ai_TableIndex,m4int32_t ai_precIndex);
	virtual	m4return_t		TranslateCycleRelations	(string &ai_stTrans,m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2);
	virtual	void			AppendOJinTables_begin(string &ao_stTrans);
	virtual	void			AppendOJinTables_end(string &ao_stTrans);
	virtual	void			AppendIJinTables_begin(string &ao_stTrans);
	virtual	void			AppendIJinTables_end(string &ao_stTrans);

	//Bug 0077247. Una coma de mas en informix
	virtual	void			EraseOJinTables_begin(string &ao_stTrans);
	virtual	void			EraseOJinTables_end(string &ao_stTrans);
	virtual	void			EraseIJinTables_begin(string &ao_stTrans);
	virtual	void			EraseIJinTables_end(string &ao_stTrans);

protected:
	virtual m4return_t		_TranslateFrom					(string &ai_stTrans,m4int32_t ai_iLevel);
	virtual m4return_t		_TranslateFrom_Informix			(string &ai_stTrans,m4int32_t ai_iLevel);
};

// Tools.

enum eTableType_t{
	LDB_NO_JOIN=1,
	LDB_INNER,
	LDB_OUTER_DOMINATOR,
	LDB_OUTER_DOMINATED
};

class TableRel{

	typedef set<TableRel *, less <TableRel *> > TableRelSet_t;
	typedef TableRelSet_t::iterator TableRelSetIt_t;

public:
	TableRel(){

		m_TableIndex = -1;
		m_bAdded = M4_FALSE;
		m_eType = LDB_NO_JOIN;
		
		m_nInner = 0;
		m_nOuter = 0;

		m_ppInner = 0;
		m_ppOuter = 0;

		m_bExistsDominator = M4_FALSE;

		m_psetTableRelSet = new TableRelSet_t;

	}
	~TableRel();
	
	m4return_t Init(m4int32_t ai_NumTables,m4int32_t ai_Tableindex);
	m4return_t AddInnerJoin(TableRel *ai_InnerTable);
	m4return_t AddOuterJoinDominator(TableRel *ai_OuterDominated);
	m4return_t AddOuterJoinDominated(TableRel *ai_OuterDominator);
	m4return_t DumpTable(string &ao_stStmSQL,VcClMtObject *ai_pstTbl,_AClSQLTranslator	*ai_pTrans,m4int32_t ai_precIndex);
	m4return_t PropagateOuterJoins(void);
	//Bug 0080087
	m4return_t CheckCircularOuterJoins (void);
	m4return_t _CheckCircularOuterJoins (TableRel * ai_pTableRel);

	eTableType_t m_eType;
	m4bool_t m_bAdded;
protected:
	m4return_t ClearEntrance(TableRel *ai_toclean,m4bool_t ai_bIsOuter);
	m4int32_t	m_TableIndex;

	m4int32_t m_nInner;
	m4int32_t m_nOuter;

	TableRel **m_ppInner;
	TableRel **m_ppOuter;
	
	m4bool_t	m_bExistsDominator;

	TableRelSet_t * m_psetTableRelSet;

};

class  TableAnalyzer{
public:
	TableAnalyzer(_AClSQLTranslator	*ai_pTranslator,VcClMtObject	*ai_pObjects,VcClMtRelat	*ai_pRelat,ClMetaSentence	*ai_pMtSent,m4int32_t ai_Level);
	~TableAnalyzer();

	//************************
	//Pedro.
	m4return_t	AnalyzePreviousCheck(m4int32_t ai_iLevel);
	m4return_t	ReduceSecurityAlias(m4int32_t ai_iLevel);
	m4return_t	DumpInfoRelations(m4int32_t ai_iLevel);	
	void		EraseDummyRelations (void);
	//************************

	m4return_t	Analyze(m4int32_t ai_iLevel);
	m4return_t	DumpResults(string &ao_stStmSQL);
protected :
	m4return_t	AddInnerJoin(m4int32_t ai_TableIndex1,m4int32_t ai_TableIndex2);
	m4return_t	AddOuterJoin(m4int32_t ai_TabIxDominator,m4int32_t ai_TabIxDominated);
private:
	m4return_t	_InitTableRel(m4int32_t ai_iLevel);
	m4return_t	_PropagateOuterJoins(void);
	m4return_t	_CheckCircularOuterJoins (void);


	VcClMtObject		*m_pObjects;
	VcClMtRelat			*m_pRelat;
	ClMetaSentence	*m_pMtSent;
	_AClSQLTranslator	*m_pTranslator;
	m4int32_t			m_nTables;
	m4int32_t			m_nInitTable;
	m4int32_t			m_iLevel;

	TableRel			*m_pTableRel;
};

//***********************************************************************************
//Se realiza un chequeo de las relaciones por si existen relaciones circulares.
//Si existe un Inner join de seguridad, propagaremos el inner a todas las relaciones
//que se vean involucradas en el ciclo.

//Ejemplo 1:
//A=B(+)
//B=C(+)
//A=C		Seg
//Este caso daba un error en la propagación.
//Reduciremos todos a inner poque todas están involucradas.

//Ejemplo 2:
//A.id=B.id(+)
//B.org=C.org(+)	Seg.
//En este caso se daba un error antes de la propagación. Ahora podemos reducirlas.

//La idea es que antes de realizarse el el proceso inicial de análisis de relaciones en
//TableAnalyzer::Analyze(ai_iLevel) tendremos este algoritmo de propagación de inners
//AnalyzePreviousCheck(ai_iLevel).



//*** Definimos punteros a objetos, vectores e iteradores.***

//Puntero a un objeto de la clase ClPreviousCheckTable.
typedef class ClPreviousCheckTable*					PClPreviousCheckTable;
//Vector de punteros a objetos.
typedef vector<PClPreviousCheckTable>				VcClPreviousCheckTable;
//Puntero al vector de punteros a objetos.
typedef VcClPreviousCheckTable*						PVcClPreviousCheckTable;
//Iterador del vector de punteros. Al definir una variable de este tipo, en realidad
//será un puntero a la casilla i-esima del vector.
//De forma que (*it) será el elemento i-ésimo.
//En nuestro caso, los elementos son vectores a objetos de la clase ClPreviousCheckTable.
typedef VcClPreviousCheckTable::iterator			itVcClPreviousCheckTable;

//ClPreviousCheckRelation.
//Se ha definido una lista de punteros a objetos relacion. La razon de utilizar una lista es
//porque tenemos una funcion sort que ordenará la lista de elementos según un criterio.
typedef class ClPreviousCheckRelation*					PClPreviousCheckRelation;
typedef list<PClPreviousCheckRelation>					VcClPreviousCheckRelation;
typedef VcClPreviousCheckRelation*						PVcClPreviousCheckRelation;
typedef VcClPreviousCheckRelation::iterator				itVcClPreviousCheckRelation;



//Clases para la comprobación previa de joins de APISQL y de Seguridad.

//Ejemplo:
//A.id = B.id(+)
//A.org = B.org   Seg.
//Son dos metarelaciones:				A.id = B.id.
//										A.org = B.org.
//Son dos objetos tabla					A, B
//Son cuatro relaciones relaciones:		Base: A.id	--	Next: B.id -- Outer	--	Principal -- No seg.
//										Base: B.id	--	Next: A.id -- Outer	--	No Princi -- No seg.
//										Base: A.org	--	Next: B.org-- Inner	--	Principal -- Seg.
//										Base: B.org	--	Next: A.org-- Inner	--	No Princi -- Seg.
//Despues de la ultima modificacion, se ha optado por eliminar el concepto de principal.
//--------------------------
//Por parejas: Si son diferentes dos relaciones totalmente --> Convierto a Outer.
//En la segunda iteración. Comparo tablas (sin campos) son diferentes algunas --> convierto a inner.
//Es decir, el primer paso convierte a OJ, y en el segundo paso, si hay ciclo y hay un inner
//de seguridad en el ciclo, se convierte a inners.
//--------------------------


//*** La clase ClPreviousCheckRelationsGroup es el punto de entrada.
//Tendremos una llamada inicial que declarará un objeto de esta clase, posteriormente
//se irán añadiendo las metarelaciones en un bucle y después tendremos la llamada a
//la reducción de joins:
//Para toda MetaRelacion:	oGrupo.AddRelation(pMetaRel)
//							oGrupo.ReduceJoins()
//***

class ClPreviousCheckRelation;
class ClPreviousCheckTable;
class ClPreviousCheckRelationsGroup;

class ClPreviousCheckRelationsGroup {

	//Ambas clases puedan usar los miembros privados.
	friend class ClPreviousCheckRelation;
	friend class ClPreviousCheckTable;

	//--------------------------------------------------
	//Parte publica, utilizada en la clase TableAnalyzer.
public:
	//En la metarelacion tenemos toda la informacion.
	//Con este miembro iremos insertando las MetaRelaciones. Cada inserción provocará
	//la creación de objetos Tabla (si no estaban) y objetos Relación.
	//Cada tabla llamara a su metodo de inserción, que a su vez insertara en el vector
	//de tablas que tenemos.
	m4return_t AddRelation(ClMetaRelat * ai_pMetaRel);

	//Inicialización de los vectores.
	ClPreviousCheckRelationsGroup();
		
	//Liberamos los punteros que tenemos guardados.
	//El objeto vector se destruye automáticamente.
	~ClPreviousCheckRelationsGroup();


	//Para cada objeto Tabla de la relacion invocaremos a reducir joins de la base.
	m4return_t ReduceJoins(void);
	//--------------------------------------------------

	//Parte privada. Uso interno y de las clases que hemos declarado friend.
private:
	//Son vectores o listas, cuyos elementos son punteros a tablas, relaciones.
	//Se almacenarán todas las Tablas (Alias) y Relaciones implicadas.
	VcClPreviousCheckTable				m_oTables;
	VcClPreviousCheckRelation			m_oRelations;


	//Inserta una Relacion en el vector-lista de relaciones.
	m4return_t AddRelation(PClPreviousCheckRelation ai_Relation);

	//Si encontramos la Tabla devolveremos un puntero a ella.
	//En otro caso devolvemos NULL.
	PClPreviousCheckTable FindTable(ClMetaRelat * ai_pMetaRel, m4bool_t ai_bIsLeftAlias);

	//Marca cada Relacion como no visitada.
	m4return_t ResetRelations(void);

	//Reduce joins, partiendo de la tabla que le pasamos.
	//A partir de esta Tabla se iniciará la recursion.
	m4return_t ReduceJoins(PClPreviousCheckTable ai_pTable);

	//Le pasamos un puntero a la Tabla y el join.
	//Una Tabla tiene un vector de Relaciones, donde se indica con qué otras
	//ablas hay relacion.
	//Es recursiva.
	m4bool_t FindAndReduce(PClPreviousCheckTable ai_pTable, m4bool_t ai_bInner);
	
	m4return_t PromoteToOuterAndSetClones(void);
	m4return_t PromoteToInner(void);
	m4return_t GenerateTableRelations(void);

	m4return_t PropagateToClones(void);	
};


//*** La clase ClPreviousCheckTable
//Contiene informacion de cada una de las tablas implicadas.
//***

class ClPreviousCheckTable {

	friend class ClPreviousCheckRelation;
	friend class ClPreviousCheckRelationsGroup;


	//Tenemos objeto vector de de punteros a objetos Relacion.
	VcClPreviousCheckRelation	m_oRelations;

	m4int32_t					m_indAlias;	

	//Tendremos a qué MetaRelacion pertenece esta tabla de chequeo.
	ClMetaRelat *				m_pMetaRel;

	m4bool_t					m_bIsOrigin;		

	//Construimos a partir de la MetaRelacion y el segundo argumento indica si
	//para construir la Tabla tenemos que tomar el Alias izquierdo o el Alias
	//derecho de la MetaRelacion.
	ClPreviousCheckTable(ClMetaRelat * ai_pMetaRel, m4bool_t ai_bIsMainLeft);

	//El vector de punteros a Relaciones, establecemos que el i-ésimo apunte a Null
	//porque la destrucción se realizaré en el GrupoRelaciones (que se insertó).
	//El objeto vector se autodestruirá.
	//Ponemos el puntero de la metarelacion a nulo.
	~ClPreviousCheckTable();
	
	m4return_t AddRelation(ClPreviousCheckRelation * ai_pRelation);
};


//*** La clase ClPreviousCheckRelation
//Contiene informacion de relaciones.
//***

class ClPreviousCheckRelation {

	friend class ClPreviousCheckRelationsGroup;
	friend class ClPreviousCheckTable;
	//friend bool CompareTo(ClPreviousCheckRelation * const &ai_pRelationOrig, ClPreviousCheckRelation * const &ai_pRelationDest);
	
	//En el caso de tener una MetaRelacion:		A.id=B.id
	//tendremos un objeto Relacion:				A.id=B.id
	
	//Indicamos si venimos de una metarelacion de seguridad, es Outer.
	m4bool_t					m_bIsSecurity;
	m4bool_t					m_bIsOuter;

	//Indica si la relacion ha sido visitada. Necesaria para el algoritmo recursivo
	//de recorrido.
	m4bool_t					m_bIsVisited;
	//Informacion extraida de la metarelacion. Base y Next.
	m4int32_t					m_indAliasBase;
	string						m_stFieldBase;
	m4int32_t					m_indAliasNext;
	string						m_stFieldNext;

	//Son dos punteros a objetos Tabla.
	//Tendremos dos punteros a objetos Tabla.	
	//Si la relación es principal:		Base = A, Next = B
	//Si la relación no es principal:	Base = B, Next = A
	PClPreviousCheckTable		m_pBase;
	PClPreviousCheckTable		m_pNext;

	//Tenemos puntero a un objeto grupo. Este puntero no se creará aquí.
	//Relamente, en la llamada inicial creamos un objeto de este tipo y este puntero
	//apuntara a este objeto.
	//No hará falta destruir el puntero aunque sí podemos ponerlo a null cuando
	//se destruya la relacion.	
	ClPreviousCheckRelationsGroup * m_pRelationsGroup;

	//Asociamos la metarelacion con la relación de chequeo previo.
	ClMetaRelat *				m_pMetaRel;


	VcClPreviousCheckRelation	m_oClones;

	ClPreviousCheckRelation(ClPreviousCheckRelationsGroup * ai_pRelationsGroup,
												 ClMetaRelat * ai_pMetaRel,
												 ClPreviousCheckTable * ai_pBase,
												 ClPreviousCheckTable * ai_pNext);
	~ClPreviousCheckRelation();

	//Le pasamos una tabla y si es la base devolvemos next.	
	void AddClone(ClPreviousCheckRelation * ai_pRelation) {m_oClones.push_back(ai_pRelation);}	
	void ResetVisited(void) {m_bIsVisited = M4_FALSE;}
	

	m4bool_t CompareToField(ClPreviousCheckRelation * ai_pRelation);
	m4bool_t CompareToTable(ClPreviousCheckRelation * ai_pRelation);
	m4bool_t CompareToTables(ClPreviousCheckTable * ai_pBase, ClPreviousCheckTable * ai_pNext);
	
	m4return_t SetInner(void);
	m4return_t SetOuter(m4bool_t ai_bIsOuter) {m_bIsOuter = ai_bIsOuter; return M4_TRUE;}
	m4bool_t SetSecurity(void) {m_bIsSecurity = M4_TRUE; return m_bIsSecurity;}	
	//Bug 0073288.
	m4return_t SetVisited(m4bool_t ai_bIsVisited);
	void SetVisited(void)  {m_bIsVisited = M4_TRUE;}

public:
	ClPreviousCheckTable * GetNextTable(ClPreviousCheckTable * ai_pTable);
	ClPreviousCheckTable * GetBase() {return m_pBase;}		
	ClPreviousCheckTable * GetNext() {return m_pNext;}
	
	m4int32_t	GetBaseAlias() {return m_indAliasBase;}
	m4int32_t	GetNextAlias() {return m_indAliasNext;}
	string		GetBaseField() {return m_stFieldBase;}
	string		GetNextField() {return m_stFieldNext;}

	m4bool_t IsOuter(void) {return m_bIsOuter;}
	m4bool_t IsSecurity(void) {return m_bIsSecurity;}
	m4bool_t IsVisited(void) {return m_bIsVisited;}

};

//Esta funcion nos permite comparar dos relaciones.
//Se utilizara en la ordenacion del vector de punteros a relaciones.
//Es importante que el valor de retorno sea bool y los argumentos sean
//punteros a referencias constantes.
bool CompareTo(ClPreviousCheckRelation * const &ai_pRelationOrig, ClPreviousCheckRelation * const &ai_pRelationDest);


//***********************************************************************************


#endif

