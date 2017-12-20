//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             AsnDef.h
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Interface of the CAccessSeq class, CAsn class,
//    CBufferASN class, CComponent class, CInputStream class,
//	  CInternalPresentationFactory class, CListComponent class,
//    COutputStream class.
//
//
//==============================================================================

#ifndef __ASN_H
#define __ASN_H

#ifdef M4_USING_DLL_ASN
	#ifdef M4_IMPL_ASN
		#define M4_DECL_ASN __declspec(dllexport)
	#else
		#define M4_DECL_ASN __declspec(dllimport)
	#endif
#else
	#define M4_DECL_ASN 
#endif

#define T_BOOL		0
#define T_INT		1
#define T_STRING	2
#define T_SEQ		3

#define ASNBOOLEAN			0x1				// cod BOOLEAN
#define ASNINTEGER			0x2				// cod INTEGER
#define ASNGeneralString	0x1B			// cod STRING
#define ASNUnicodeString	0x1F			// cod STRING in Unicode utf8
#define ASNSequence			0x30			// cod SEQUENCE

#define MAX_NUM				100

// Before use Templates include m4stl.hpp
#include "m4stl.hpp"
#include "attributes.hpp"
#include "cllstr.hpp"

//-----------------------------------------------------------------------------------------------
//Esta macro la utilizamos si se pierde memoria en el heap. De momento queda indefinida pq solo
//la utilizare en caso de depuracion.
#undef _M4PRESENTATION_DEBUG_DUMP_MEMORY_INFO_
//-----------------------------------------------------------------------------------------------

class ClChannelManager;
class ClChannel;
class ClCompiledMCR;

//////////////////////////////////////////////////////////////////////
// OutputStream.h: interface for the COutStream class.
//
//////////////////////////////////////////////////////////////////////

class M4_DECL_ASN COutputStream  
{
public:
	COutputStream();
	virtual ~COutputStream();

	m4uint32_t GetLength() const;
	void writeString( m4uchar_t *ai_pcString, m4uint32_t ai_iLength, m4uint32_t ai_iTypePosition );
	void writeByte( m4uchar_t ai_iByte );

public: // solo de momento despues sera privada.
	vector<m4uchar_t> m_stream;
private:
	m4uint32_t m_cont;
};

///////////////////////////////////////////////////
// InputStream.h: interface for the CInputStream class.
//
//////////////////////////////////////////////////////////////////////

class M4_DECL_ASN CInputStream  
{

private:

	m4uint32_t m_cont;
	m4uint32_t m_iLength;
	m4puchar_t m_stream;
	m4uchar_t  m_iStringBaseType;

public:

	CInputStream( m4puchar_t asn, m4uint32_t length );

	m4uchar_t  StringBaseType( void ) const
	{
		return m_iStringBaseType;
	}

	m4bool_t	eof( void ) const;
	m4uchar_t	readByte( m4uchar_t& ao_rbByte );
	m4uint32_t	readLength( m4uint32_t& ao_riLength );
	m4uint32_t	readInteger( m4uint32_t& ao_riInteger );
	m4uint32_t	readString( m4pchar_t &ao_rpcString, m4uint32_t &ao_riLength, m4uchar_t ai_iType );
	m4uint32_t	readString( m4pchar_t &ao_rpcString, m4uint32_t &ao_riLength, m4pchar_t &ao_rpcCopy, m4return_t &ao_riResult );
	m4uint32_t	readStringReference(  m4pchar_t &ao_rpcString, m4uint32_t &ao_riLength, m4uchar_t ai_iType, m4pchar_t &ao_rpcCopy );
};



///////////////////////////////////////////////////
// Component.h: interface for the CComponent class.
//
//////////////////////////////////////////////////////////////////////

union TYPE_COMP {
	m4uchar_t byte_value;
	m4int32_t int_value;
	m4char_t *char_value;
};


class CListComponent;
class CAccessSeq;
class CAsn;
class CBufferASN;
class CInternalPresentationFactory;
class ClPresentationFactory;

class CComponent;
typedef vector<CComponent *>				VcCComponent;
typedef VcCComponent::iterator				itVcCComponent;
typedef VcCComponent::reverse_iterator		itRVcCComponent;

class M4_DECL_ASN CComponent
{
	m4uchar_t type_nameProp;
	m4uint32_t length_nameProp;
	m4char_t *nameProp;
	m4uchar_t type_prop;
	TYPE_COMP value_prop;
	

private:
	m4char_t calculateLength(m4int32_t ai_iLength);
	void decodeLength(m4int32_t ai_iLength);
	void writeLength(m4int32_t ai_iLength,COutputStream *outStr);
	m4bool_t Exist(m4char_t *idChannel, m4int_t i_numMeta4Obj, m4char_t **lst_listMeta4Obj);

	m4int32_t referenceSolver (m4uint32_t ai_iLenName, m4pchar_t ai_pcPropName, 
		m4uint32_t ai_iLenValue, m4pchar_t ai_pcPropValue, m4uchar_t ai_cPropValueType, m4bool_t &ao_bIsTranslation, 
		CAsn *ai_pReference, m4bool_t ai_bIsDef, m4uint32_t ai_iReferences, CAsn *ai_paMultiReference, m4pchar_t *ai_ppcLangCodes, CComponent *ai_pParent );

public:
	m4uint32_t m_iInheritStatus; // nuevo
	m4uint32_t length;
	CListComponent	* propBag; // Si es SEQ: Los dos primeros elmtos de esta lista son siempre ClassName y Alias.
	m4uint32_t length_prop;
	CAccessSeq *m_access;
	m4bool_t m_bIsOrdered;

public:
	CComponent();
	virtual ~CComponent();
	CComponent(const CComponent& X);

	m4uint32_t GetLength();
	m4uchar_t GetTypeNameProp();
	m4uint32_t GetLengthNameProp();
	void GetNameProp(m4char_t *name);
	m4uchar_t GetTypeProp();
	m4uint32_t GetLengthProp();
	m4int32_t GetIntValueProp();
	m4uchar_t GetBoolValueProp();

	void GetStringValueProp(m4char_t *name);
	void SetLength(m4int32_t ai_iLength);
	void SetTypeNameProp();
	void SetLengthNameProp(m4uint32_t ai_iLength);
	void SetNameProp(const m4char_t *the_name,m4uint32_t the_length);
	void SetTypeProp(m4int_t type);
	void SetLengthProp(m4bool_t traducir,m4int_t type,m4int32_t value = 0,m4uint32_t ai_iLength = 0);
	void SetIntValueProp(m4int32_t the_value);
	void SetBoolValueProp(m4uchar_t the_value);
	void SetStringValueProp(const m4char_t *the_value,m4uint32_t the_length);	

	m4bool_t IsNameProp( m4pcchar_t ai_pName );	
	m4bool_t IsNameProp( CComponent * ai_pCComponent );
	m4bool_t IsValueProp( m4int32_t ai_iValue );
	m4bool_t IsValueProp( m4uchar_t ai_bValue );
	m4bool_t IsValueProp( m4pcchar_t ai_pValue );
	m4bool_t IsNameAndValueProp( CComponent * ai_pCComponent );

	void SetInheritStatus(m4uint32_t ai_iInheritStatus);
	m4uint32_t GetInheritStatus(); 

public:	
	void AnalizarSequence( m4bool_t ai_bFirstTime, CBufferASN * ai_pInfPresent );
	void AddSegurity();
	m4int32_t RecalculeLength( CListComponent * ai_pLcomp );
	void decode(COutputStream *outStr,m4bool_t traducir,CComponent *padre = NULL);

	m4int32_t code(ClChannelManager *ai_poChannelManager, CInputStream *ai_pIn, CAsn *ai_pReference, m4bool_t ai_bIsDef, m4uint32_t ai_iReferences, CAsn *ai_paMultiReference, m4pchar_t *ai_ppcLangCodes, CComponent *ai_pParent);
	m4int32_t codeReferences(CInputStream *m_in);
	m4int_t FindReference(CComponent *seq,m4char_t *nombre,m4uint32_t *aio_pLength,m4uchar_t *type,TYPE_COMP *c);
	
	m4int_t FindIndirectionItem(ClChannelManager *ai_poChannelManager, ClCompiledMCR *poCompiled,m4char_t *org,m4char_t *&dst,m4uint32_t *indice,m4uint32_t *b_L,m4char_t *name_node,m4uint32_t ai_iLength);
	m4int_t FindIndirectionNode(ClChannelManager *ai_poChannelManager, ClCompiledMCR *poCompiled,m4char_t *org,m4char_t *&dst,m4uint32_t *indice,m4uint32_t *b_L,m4uint32_t ai_iLength);
	m4int_t FindIndirectionChannel(ClChannelManager *ai_poChannelManager, ClCompiledMCR *poCompiled,m4char_t *org,m4char_t *&dst,m4uint32_t *indice,m4uint32_t *b_L,m4uint32_t length, CInternalPresentationFactory *aux, ClPresentationFactory * ai_pPresentationFactory, const ClPresentationAttributes & ai_ClPresentationAttributes);
	m4int_t analiceIndirection(m4char_t *org,m4uint32_t *indice,m4uint32_t ai_Length,m4uint32_t *beginInd);

	m4return_t GetClassNameAndAlias( m4pchar_t & ao_pClassName, m4pchar_t & ao_pAlias );

	//-----------------------------------------------------------------------------------------------
	public: 	
	void DumpInfoRoot( m4bool_t ai_bTranslateToObl, m4pcchar_t ai_pTitle, m4int32_t ai_iLevel );
	static void DumpMemoryInfo( m4pcchar_t ai_pIdPresentation, m4bool_t ai_bStartToCount );
	//-----------------------------------------------------------------------------------------------

public:
	m4return_t MergeChildIncludeByPos( m4uint32_t ai_iPosPropBag, CComponent * ai_pNewComponent );
	m4return_t ReplaceValueProp( m4char_t * ai_pNewValue );
	m4return_t ReplaceValuePropSearchingPattern( const string & ai_sPattern, const string & ai_sNewString );

	m4return_t AnalizeChildToDeleteObjectNotExistsInParentRoot( void );
	m4return_t MergeInheritPresentationRoot( CComponent * & aio_pCComponentContainerParent );	

	m4return_t _RemoveComponentInListByPos( m4int32_t ai_iPosPropBag, m4bool_t ai_bDeleteComponent );

protected:	
	

	m4return_t _AnalizeChildToDeleteObjectNotExistsInParent( void );

	m4return_t _MergeInheritPresentation( CComponent * & aio_pCComponentContainerParent );
	m4return_t _MergeChildAttributeAndParentSequence( CComponent * ai_pCComponentContainerParent, m4bool_t & ao_bIsOvewritten );
	m4return_t _MergeChildSequenceAndParentSequence( CComponent * ai_pCComponentContainerParent );	

	m4return_t _CheckIsSameObject( CComponent * ai_pCComponentContainerParent, m4bool_t & ao_bIsSameObject );
	m4return_t _AnalizeChildAttributes( CComponent * ai_pCComponentContainerParent );	
	m4return_t _AnalizeChildToDeleteObjects( CComponent * ai_pCComponentContainerParent );	
	m4return_t _AnalizeTransferComponentFromParentSequenceAndNotExistsInChild( CComponent * ai_pCComponentContainerParent );
	m4return_t _AnalizeMoveChildSequences( void );
	m4return_t _AnalizeChildSequences( CComponent * ai_pCComponentContainerParent );

	m4return_t _ExistsAttributeInContainer( CComponent * ai_pCComponentContainer, m4int32_t & ao_iAttributePositionInContainer );
	m4return_t _ExistsSequenceInContainer( CComponent * ai_pCComponentContainer, m4int32_t & ao_iSequencePositionInContainer );
	m4return_t _IsComponentToDelete( m4bool_t & ao_bIsComponentToDelete, m4pchar_t & ao_pIdComponentToDelete );
	

	m4return_t _TransferComponentFromParentComponent( CComponent * ai_pCComponentContainerParent, m4int32_t ai_iComponentPositionInParent, m4int32_t ai_iLastInsertPositionInChildSequence );
	m4return_t _FindPositionFirstAttributeAfterAlias( m4int32_t & ao_iPositionFound );
	m4return_t _FindPositionFirstSequence( m4int32_t & ao_iPositionFound );
	m4return_t _FindPositionLastSequence( m4int32_t & ao_iPositionFound );
	m4return_t _FindPositionByIdSequence( CComponent * ai_pCComponentContainer, m4int32_t & ao_iPositionFound );	
	m4return_t _FindPositionByIdSequence( m4pchar_t ai_pIdSequenceToMove, m4int32_t & ao_iPositionFound );

public:
	m4return_t GetIdSequenceMoveAttribute( m4bool_t & ao_bExistsMoveAttribute, m4pchar_t & ao_pIdSequenceToMove );
	m4return_t FindPositionInVectorSearchingByIdSequence( VcCComponent * ai_pVcCComponent, m4bool_t & ao_bHasPositionafter,m4int32_t & ao_iPositionFound );
	m4return_t HasPositionAfterBegin( m4bool_t & ao_bHasPositionafterBegin );

protected:	
	void	_IniLength( m4uint32_t ai_iValue );
};

/////////////////////////////////////////////////////////////
// ListComponent.h: interface for the CListComp class.
//
//////////////////////////////////////////////////////////////////////

class M4_DECL_ASN CListComponent  
{
	m4uint32_t m_iNumComp;
	VcCComponent m_oVcComponent;

public:
	CListComponent();
	virtual ~CListComponent();

	m4uint32_t GetNumComponent();
	CComponent * View( m4uint32_t i );
	void SetGrant( m4uchar_t value );
	void Remove( m4int32_t i );
	void Add( CComponent *comp );

	m4return_t ReplaceComponentByPos ( m4uint32_t ai_iPosPropBag, CComponent * ai_pNewComponent );
	m4return_t InsertComponentInPosition( m4int32_t ai_iPosition, CComponent * ai_pNewComponent );
	m4return_t InsertComponentInLastPosition( CComponent * ai_pNewComponent );
	m4return_t InsertComponentAfterPosition( m4int32_t ai_iPosition, CComponent * ai_pNewComponent );

	//------------------------------------------------------------------------------
	static void RemoveInListAllComponent( VcCComponent & aio_vCComponent, m4bool_t ai_bDeleteElement );	
	void RemoveAllComponent( m4bool_t ai_bDeleteElement );	
	static m4return_t SortListByPositionAfterBegin( VcCComponent & aio_vCComponent );
	static m4return_t MergeUnknownInList( VcCComponent & aio_vToFill, VcCComponent & aio_vToClear );

	void AddComponentFromComponentVector( VcCComponent * ai_pVcCComponent );
	static m4int32_t FindComponentInVector( VcCComponent * ai_pVcCComponent, const CComponent * ai_pCComponentToFind);
	static m4int32_t FindComponentInVectorByIdSequence( VcCComponent * ai_pVcCComponent, m4pcchar_t ai_pIdSequenceToMove );
	static m4int32_t FindFirstSequenceInVector( VcCComponent * ai_pVcCComponent );
	//------------------------------------------------------------------------------
};

//////////////////////////////////////////////////////////////////////
// Asn.h: interface for the CAsn class.
//
//////////////////////////////////////////////////////////////////////

class CAsn;
typedef vector<CAsn *>					VcInheritCAsn;
typedef VcInheritCAsn::iterator			itVcInheritCAsn;
typedef VcInheritCAsn::const_iterator	constitVcInheritCAsn;

class M4_DECL_ASN CAsn  
{
public:
	CComponent			*present;

public:
	CAsn();
	virtual ~CAsn();

	void decode(m4bool_t traducir);

	m4return_t code(ClChannelManager *ai_poChannelManager, m4bool_t traducir, CInputStream *ai_pIn, CAsn *ai_pReference, m4bool_t ai_bIsDef, m4uint32_t ai_iReferences, CAsn *ai_paMultiReference, m4pchar_t *ai_ppcLangCodes, CComponent *ai_oParent);
	m4return_t codeXml(CInputStream *ai_pInput, m4uint32_t ai_iBytesToRead, ClLongString &ao_roXML, m4pcchar_t ai_pccOrganization, ClChannelManager *ai_poChannelManager);
	m4return_t MergeInheritPresentationRoot( CAsn * & aio_pCAsnParent );
};



//////////////////////////////////////////////////////////////////////
// BufferASN.h: interface for the CBufferASN class.
//
//////////////////////////////////////////////////////////////////////

class CBufferASN;
typedef vector<CBufferASN *>					VcInheritCBufferASN;
typedef VcInheritCBufferASN::iterator			itVcInheritCBufferASN;
typedef VcInheritCBufferASN::const_iterator		constitVcInheritCBufferASN;


class M4_DECL_ASN CBufferASN  
{
private:

	VcIdInclude			m_oVcIdInclude;
	VcVersionInclude	m_oVcVersionInclude;
	SetIdInclude		m_oSetIdInclude;

public:
	m4uint32_t	length;
	m4puchar_t	asn;

	m4uint32_t	lengthRef;
	m4puchar_t	ref;

	m4uint32_t	references;
	m4uint32_t	*ppiLengthRefs;
	m4puchar_t	*ppcRefs;
	m4pchar_t	*ppcCodes;

	m4date_t	version;
	m4uint16_t	style;
	m4int_t		numT3;
	m4pchar_t	listT3[ MAX_NUM ];
	m4date_t	listVersionT3[ MAX_NUM ];


	CBufferASN(m4int32_t ai_iReferences = 0);
	virtual ~CBufferASN();

	m4date_t	GetVersionT3ByIndex(m4int_t index);
	m4pchar_t	GetIdT3ByIndex(m4int_t index);
	m4int_t		Exist(m4pcchar_t idT3);
	m4int_t		ExistVersionOfT3(m4pcchar_t idT3);
	m4int32_t	GetNumInclude( void ) const;
	string		GetIdIncludeByPos( m4int32_t ai_iPos ) const;
	m4date_t	GetVersionIncludeByPos( m4int32_t ai_iPos );
	m4return_t	AddInfoIncludeInBufferASN( const string & ai_sIdInclude, m4date_t ai_dVersionInclude );
	m4return_t	CheckInfiniteRecursionPush( const string & ai_sIdInclude );
	m4return_t	CheckInfiniteRecursionPop( void );
};



//////////////////////////////////////////////////////////////////////
// InternalPresentationFactory.h: interface for the CBufferASN class.
//
//////////////////////////////////////////////////////////////////////

typedef map<string, ClChannel*, less<string> > tMap;

class ClPresentationFactory;
class ClPresentation;

typedef map<string, CComponent *, less<string> >	MpArguments;
typedef MpArguments::iterator						itMpArguments;
typedef MpArguments::const_iterator					constitMpArguments;

typedef set<string, less<string> >					SetIdArguments;
typedef SetIdArguments::iterator					itSetIdArguments;
typedef SetIdArguments::const_iterator				constitSetIdArguments;

class M4_DECL_ASN CInternalPresentationFactory
{
private:
	ClPresentationFactory * m_pPresentationFactory;
	ClChannelManager	*m_poChannelManager;
	ClChannel			*m_poChannel;
	tMap*               m_pmChannel_map;

public:
	CInternalPresentationFactory(ClPresentationFactory * ai_pPresentationFactory, ClChannelManager *poCManager);
	virtual ~CInternalPresentationFactory();

	m4return_t BuildBasicPresentationFromId( const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, CBufferASN * & ao_pBasicPresentation );
	m4return_t BuildPresentation( m4pcchar_t ai_pIdPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes, CBufferASN  * ai_pBasicPresentation, m4bool_t ai_bDefinition, CBufferASN * & ao_pParticularPresentation );
	ClChannel *FindChannel(m4pcchar_t name_channel, const ClPresentationAttributes & ai_ClPresentationAttributes, m4return_t *ret);
	
private:
	//Includes.
	m4bool_t _IsPresentationRoot( CAsn * ai_pCAsn );
	m4return_t _AnalizeIncludesRoot( CBufferASN * ai_pCBufferASNRoot, CAsn * ai_pCAsnParent, CAsn * ai_pCAsnIncludeToAnalize, const string & ai_sIdInclude, const ClPresentationAttributes & ai_ClPresentationAttributes );
	m4return_t _AnalizeIncludes( CBufferASN * ai_pCBufferASNRoot, CAsn * ai_pCAsn, CComponent * ai_pCComponent, const ClPresentationAttributes & ai_ClPresentationAttributes );
	m4return_t _AnalizeIncludeBlock( CBufferASN * ai_pCBufferASNRoot, CAsn * ai_pCAsn, CComponent * ai_pCComponent, const ClPresentationAttributes & ai_ClPresentationAttributes, CComponent * & ao_pIncludeRootComponent );
	m4return_t _MergeFatherAndChildIncludeRoot( CAsn * ai_pCAsn, CComponent * ai_pIncludeRootComponent );
	m4return_t _MergeFatherAndChildInclude( CComponent * ai_pParentComponent, m4uint32_t ai_iChildPosition, CComponent * ai_pCComponent, CComponent * ai_pIncludeRootComponent, m4bool_t & ao_bPruned );
	m4return_t _AnalizeIncludeArgument( CComponent * ai_pCComponent, MpArguments & ao_mArguments );
	m4return_t _InsertIncludeArgument( MpArguments & ao_mArguments, const string & ai_sIdArgument, CComponent * ai_pCComponent );
	m4return_t _AnalizeLoadIncludePresentation( CBufferASN * ai_pCBufferASNRoot, m4char_t * ai_pIdPresentation, const MpArguments & ai_mArguments, CAsn * ai_pCAsn, const ClPresentationAttributes & ai_ClPresentationAttributes, CComponent * & ao_pIncludeRootComponent );
	m4return_t _LoadIncludePresentation( CBufferASN * ai_pCBufferASNRoot, ClPresentation * & ao_pPresentation, m4pcchar_t ai_pIdPresentation, const ClPresentationAttributes & ai_ClPresentationAttributes );	
	m4return_t _SolveIncludeArgumentsRoot( CAsn * ai_pCAsn, const MpArguments & ai_mArguments );
	m4return_t _SolveIncludeArguments( CComponent * ai_pCComponent, const MpArguments & ai_mArguments );
	m4return_t _SolveIncludeOneArgument( const SetIdArguments & ai_oSetIdArguments, CComponent * ai_pCComponent, const MpArguments & ai_mArguments );
	m4return_t _GetIdArgument( m4char_t * ai_pValue, SetIdArguments & ao_oSetIdArguments );
	m4return_t _RenameAliasIncludeRoot( CComponent * ai_pIncludeRootComponent, m4char_t * ai_pIdAlias );
	m4return_t _ClearIncludeTreeComponent( CAsn * ai_pCAsn, CComponent * & ao_pIncludeRootComponent );
	m4return_t _ClearSubTreeComponentByRoot( CComponent * ai_pIncludeRootComponent );
	m4return_t _FreeSubTreeComponentByRoot( CComponent * & aio_pCComponent );
		
	//Herencia.
	m4return_t _CreateCAsnInheritancePresentationChain( const VcInheritCBufferASN & ai_vInheritCBufferASN, m4bool_t ai_bDefinition, VcInheritCAsn & ao_vInheritCAsn );	
	m4return_t _CreateCAsn( CBufferASN * ai_pCBufferASN, m4bool_t ai_bDefinition, CAsn * & ao_pCAsn );

	m4return_t _AnalizeInheritancePresentationChain( VcInheritCAsn & aio_vInheritCAsn, m4bool_t ai_bDefinition, CBufferASN * & ao_pMergedBufferASN );	
	m4return_t _AnalizeInheritancePresentationRootByAsn( CAsn * & aio_pCAsnParent, CAsn * & aio_pCAsnChild );	

	m4return_t _SetInheritanceFlag ( CComponent * ai_oComponent, m4int8_t ai_bInheritStatus ) ; 
	m4return_t _ResetInheritanceMarks ( CComponent * ai_oComponent ) ; 

	//------------------------------------------------------------------------------
	static void RemoveInListAllCAsn( VcInheritCAsn & aio_vInheritCAsn, m4bool_t ai_bDeleteElement );
	static void RemoveInListAllCBufferASN( VcInheritCBufferASN & aio_vInheritCBufferASN, m4bool_t ai_bDeleteElement );
	//------------------------------------------------------------------------------
	
	m4uint32_t know_length(m4uchar_t *asn);
	m4bool_t Decode(m4pchar_t ai_pcPresentation, m4puchar_t& ao_rpBuffer, m4uint32_t& ao_riLength);
	m4return_t CreateChannel();
	m4return_t GetPresentation(const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, CBufferASN * & ao_pBasicPresentation);	
	m4return_t _GetPresentation(const ClPresentationAttributes & ai_ClPresentationAttributes, m4bool_t ai_bDefinition, CBufferASN * & ao_pBasicPresentation);
	m4int_t AskSegurityItem(ClCompiledMCR *poCompiled,m4char_t *name_item,m4char_t *name_node);
	m4int_t AskSegurityNode(ClCompiledMCR *poCompiled,m4char_t *name_node);
	m4return_t AskSegurity_Indirection(CComponent *seq,CAccessSeq *anterior,ClCompiledMCR *poCompiled, const ClPresentationAttributes & ai_ClPresentationAttributes, CBufferASN *infPresent);
	void Destroy_map();
	m4return_t TranslateIndirection(m4char_t *strIni, m4char_t *strFin,ClCompiledMCR *poCompiled, CComponent *comp, m4uint32_t length,m4char_t *name_node, m4uint32_t *lenTotal, m4bool_t copy,CInternalPresentationFactory *aux, const ClPresentationAttributes & ai_ClPresentationAttributes);
};

//////////////////////////////////////////////////////////////////////
// Clase CAccessSeq
//////////////////////////////////////////////////////////////////////

class M4_DECL_ASN CAccessSeq  
{
public:
	m4char_t *channel;
	m4char_t *node;
	m4char_t *item;
	m4int_t  lengthChannel; 
	m4int_t  lengthNode;
	m4int_t  lengthItem;
	m4int_t  grant;
	m4bool_t grantsByUser;

public:
	CAccessSeq();
	virtual ~CAccessSeq();
};

#endif //ASN_H
