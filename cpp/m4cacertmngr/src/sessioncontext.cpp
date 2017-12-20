//## begin module%3CF4A901037D.cm preserve=no
//	  %X% %Q% %Z% %W%
//## end module%3CF4A901037D.cm

//## begin module%3CF4A901037D.cp preserve=no
//## end module%3CF4A901037D.cp

//## Module: sessioncontext%3CF4A901037D; Package body
//## Subsystem: M4ServerBase::Session Context::src%3CF48A9C0248
//## Source file: F:\integration\m4serverbase\src\sessioncontext.cpp

//## begin module%3CF4A901037D.additionalIncludes preserve=no
//## end module%3CF4A901037D.additionalIncludes

//## begin module%3CF4A901037D.includes preserve=yes
#include <sesscontextserial.hpp>
#include <sessioncontext.hpp>
#include <smplsesscontextserial.hpp>
#include <_base64.h>
//## end module%3CF4A901037D.includes

//## begin module%3CF4A901037D.declarations preserve=no
//## end module%3CF4A901037D.declarations

//## begin module%3CF4A901037D.additionalDeclarations preserve=yes
#define LINELEN 64
//## end module%3CF4A901037D.additionalDeclarations


// Class ClSessionContext 



ClSessionContext::ClSessionContext()
  //## begin ClSessionContext::ClSessionContext%.hasinit preserve=no
  //## end ClSessionContext::ClSessionContext%.hasinit
  //## begin ClSessionContext::ClSessionContext%.initialization preserve=yes
  //## end ClSessionContext::ClSessionContext%.initialization
{
  //## begin ClSessionContext::ClSessionContext%.body preserve=yes
	m_poSessionSerializer = new ClSimpleSessContextSerial ;

	if ( ! m_poSessionSerializer )
	{
		m_iError = M4_SESSION_CONTEXT_SERIALIZER_UNINITIALIZED ;
	}
  //## end ClSessionContext::ClSessionContext%.body
}


ClSessionContext::~ClSessionContext()
{
  //## begin ClSessionContext::~ClSessionContext%.body preserve=yes
	if ( m_poSessionSerializer )
	{
		delete m_poSessionSerializer ;
	}
  //## end ClSessionContext::~ClSessionContext%.body
}

//## Other Operations (implementation)
m4return_t ClSessionContext::SetAtribute (m4string_t ai_strAtribute, m4string_t ai_strValue)
{
  //## begin ClSessionContext::SetAtribute%1022692987.body preserve=yes
	if ( ! m_poSessionSerializer )
	{
		return M4_ERROR ;
	}

	m_iError = m_poSessionSerializer -> AddAtribute (ai_strAtribute, ai_strValue) ;

	return m_iError ;
  //## end ClSessionContext::SetAtribute%1022692987.body
}

m4return_t ClSessionContext::Serialize (m4pchar_t & ao_pBuffer, m4uint32_t & ao_iBufLen)
{
  //## begin ClSessionContext::Serialize%1022692988.body preserve=yes
	m4uint32_t asn1size=0, numBlocks=0 , totalsize=0 ;
	m4pchar_t asn1buf = NULL, base64buf = NULL ;

	if ( ! m_poSessionSerializer )
	{
		return M4_ERROR ;
	}

	asn1buf = ao_pBuffer ;
	asn1size = ao_iBufLen ;

	m_iError = m_poSessionSerializer -> Serialize ( asn1buf , asn1size ) ;

	if ( m_iError == M4_ERROR )
		return M4_ERROR ;

	if ((asn1size %3)==0)
		numBlocks=(asn1size/3);
	else
		numBlocks=(asn1size/3)+1;
	
	totalsize = (numBlocks*4+(numBlocks*4/LINELEN)) + 1;
		
	if ( ! ao_pBuffer )
	{
		ao_iBufLen = totalsize ;
		return M4_SUCCESS ;
	}
	else 
	{
		C_Base64 oEncoder ;

		if ( ao_iBufLen < totalsize )
			return M4_ERROR ;
		

		oEncoder.Code (asn1buf , asn1size , base64buf) ;

		// bg 119288
		if( base64buf != NULL && ao_iBufLen > strlen( base64buf ) )
		{
			memcpy ( ao_pBuffer , base64buf , strlen( base64buf ) + 1  ) ;
		}
		else
		{
			return M4_ERROR ;
		}
		// bg 119288
	}

	return M4_SUCCESS  ;
  //## end ClSessionContext::Serialize%1022692988.body
}

m4return_t ClSessionContext::Deserialize (m4pchar_t ai_pBuffer )
{
  //## begin ClSessionContext::Deserialize%1022692989.body preserve=yes
	C_Base64 oEncoder ;
	m4pchar_t p ;
	m4int32_t length ;

	
	
	if ( ! m_poSessionSerializer )
	{
		return M4_ERROR ;
	}

	oEncoder.Decode ( ai_pBuffer , p , &length ) ;

	m_iError = m_poSessionSerializer -> Deserialize (p , length ) ;

	return m_iError ;
  //## end ClSessionContext::Deserialize%1022692989.body
}

m4return_t ClSessionContext::GetAtribute (m4string_t ai_strAtribute, m4string_t &ao_strValue)
{
  //## begin ClSessionContext::GetAtribute%1022692990.body preserve=yes
	if ( ! m_poSessionSerializer )
	{
		return M4_ERROR ;
	}

	m_iError = m_poSessionSerializer -> GetAtribute (ai_strAtribute , ao_strValue ) ;

	return m_iError ;
  //## end ClSessionContext::GetAtribute%1022692990.body
}

// Additional Declarations
  //## begin ClSessionContext%3CF4BEA5021A.declarations preserve=yes

m4return_t ClSessionContext::GetError ( )
{
	return m_iError ;
}

SessionIterator ClSessionContext::GetIterator() {

	if (m_poSessionSerializer == NULL) {
		return NULL;
	}

	return m_poSessionSerializer->GetIterator();
}

// --------------------------------------------------------
// class SessionIterator
// interface default implementation
// --------------------------------------------------------
SessionIterator::~SessionIterator() {
	Reset();
}

SessionIterator::SessionIterator (const SessionIterator &ai_oIterator) {
	m_poIterator = ai_oIterator.m_poIterator;
	AddRef();
}

SessionIterator::SessionIterator (ISessionIterator *ai_poIterator) {
	m_poIterator = ai_poIterator;
	AddRef();
}

SessionIterator & SessionIterator::operator = (SessionIterator &ai_oIterator) {
	Reset();
	m_poIterator = ai_oIterator.m_poIterator;

	AddRef();

	return *this;
}

void SessionIterator::AddRef() {
	
	if (m_poIterator != NULL) {
		m_poIterator->AddRef();
	}
}

void SessionIterator::Reset() {

	ISessionIterator *poRef = m_poIterator;

	m_poIterator = NULL;

	if (poRef != NULL && poRef->DelRef() == 0) {
		delete poRef;
	}
}

m4pcchar_t SessionIterator::GetKey() {

	if (m_poIterator == NULL) {
		return NULL;
	}
	
	return m_poIterator->GetKey();
}

m4pcchar_t SessionIterator::GetValue() {
	
	if (m_poIterator == NULL) {
		return NULL;
	}
	
	return m_poIterator->GetValue();
}

void SessionIterator::Next() {
	
	if (m_poIterator == NULL) {
		return;
	}
	
	m_poIterator->Next();
}

//## end ClSessionContext%3CF4BEA5021A.declarations

//## begin module%3CF4A901037D.epilog preserve=yes
//## end module%3CF4A901037D.epilog
