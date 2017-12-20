//## begin module%3912A6BB0330.cm preserve=no
//## end module%3912A6BB0330.cm

//## begin module%3912A6BB0330.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.hpp
//	 Project:            M4ComSrv
//	 Author:            Meta Software M.S. , S.A
//	 Date:
//	 Language:          C++
//	 Operating System:  WINDOWS , UNIX ( HP, DEC , AIX , SUN)
//	 Design Document:
//
//	 Definition:
//
//	    This module defines...
//
//
//	=============================================================================
//	=
//## end module%3912A6BB0330.cp

//## Module: clpstack%3912A6BB0330; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\clpstack.hpp

#ifndef __CLPSTACK_HPP__
#define __CLPSTACK_HPP__ 1

//## begin module%3912A6BB0330.additionalIncludes preserve=no
//## end module%3912A6BB0330.additionalIncludes

//## begin module%3912A6BB0330.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                clpstack.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                11-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    Este módulo define un array de punteros a void
//    también define una array de referencias a punteros
//
//
//==============================================================================



#include "m4objglb_dll.hpp"
#include "m4glbres.hpp"

#include "cltypes.hpp"
#include "cldefine.hpp"
#include "chlog.hpp"



//## end module%3912A6BB0330.includes

// m4types
#include <m4types.hpp>
//## begin module%3912A6BB0330.declarations preserve=no
//## end module%3912A6BB0330.declarations

//## begin module%3912A6BB0330.additionalDeclarations preserve=yes
//## end module%3912A6BB0330.additionalDeclarations


//## begin ClReferenceStack%3912A6BB0331.preface preserve=yes
//## end ClReferenceStack%3912A6BB0331.preface

//## Class: ClReferenceStack%3912A6BB0331
//	//===========================================================================
//	======
//	//
//	// ClReferenceStack
//	//
//	// Clase de referencia a punteros a void
//	//
//	//===========================================================================
//	======
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJGLB ClReferenceStack 
{
  //## begin ClReferenceStack%3912A6BB0331.initialDeclarations preserve=yes
  //## end ClReferenceStack%3912A6BB0331.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClReferenceStack%1029112197; C++
      ClReferenceStack (void )
        //## begin ClReferenceStack::ClReferenceStack%1029112197.hasinit preserve=no
        //## end ClReferenceStack::ClReferenceStack%1029112197.hasinit
        //## begin ClReferenceStack::ClReferenceStack%1029112197.initialization preserve=yes
        //## end ClReferenceStack::ClReferenceStack%1029112197.initialization
      {
        //## begin ClReferenceStack::ClReferenceStack%1029112197.body preserve=yes
			m_iLength = 0 ;
			m_ppvElements = NULL ;
		
        //## end ClReferenceStack::ClReferenceStack%1029112197.body
      }

    //## Destructor (specified)
      //## Operation: ~ClReferenceStack%55797044; C++
      ~ClReferenceStack (void )
      {
        //## begin ClReferenceStack::~ClReferenceStack%55797044.body preserve=yes
			m_iLength = 0 ;
			m_ppvElements = NULL ;
		
        //## end ClReferenceStack::~ClReferenceStack%55797044.body
      }


    //## Other Operations (specified)
      //## Operation: GetLength%-1973806649; C++
      m4uint16_t GetLength (void ) const
      {
        //## begin ClReferenceStack::GetLength%-1973806649.body preserve=yes
		return( m_iLength ) ;
	
        //## end ClReferenceStack::GetLength%-1973806649.body
      }

      //## Operation: GetElement%-2071418194; C++
      m4return_t GetElement (m4uint16_t ai_iPosition, m4pvoid_t& ao_rpvElement) const
      {
        //## begin ClReferenceStack::GetElement%-2071418194.body preserve=yes

#ifdef _DEBUG
		m4return_t  iResult ;


		if( ai_iPosition >= m_iLength )
		{
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << ai_iPosition << LogCat << " - " << LogCat << m_iLength ) ;
			iResult = M4_ERROR ;
		}
		else
		{
			ao_rpvElement = m_ppvElements[ ai_iPosition ] ;
			iResult = M4_SUCCESS ;
		}

   		return( iResult ) ;
#else
		ao_rpvElement = m_ppvElements[ ai_iPosition ] ;
		return( M4_SUCCESS ) ;
#endif

	
        //## end ClReferenceStack::GetElement%-2071418194.body
      }

      //## Operation: Init%1807896950; C++
      void Init (m4pvoid_t* ai_ppvElement)
      {
        //## begin ClReferenceStack::Init%1807896950.body preserve=yes
		m_iLength = 0 ;
		m_ppvElements = ai_ppvElement ;
	
        //## end ClReferenceStack::Init%1807896950.body
      }

      //## Operation: AddElement%1770912665; C++
      void AddElement (void )
      {
        //## begin ClReferenceStack::AddElement%1770912665.body preserve=yes
		m_iLength++ ;
	
        //## end ClReferenceStack::AddElement%1770912665.body
      }

      //## Operation: Find%1404830808; C++
      //	=============================================================================
      //	====
      //
      //	ClReferenceStack
      //
      //	Clase de referencia a punteros a void
      //
      //	=============================================================================
      //	====
      m4pcvoid_t Find (m4comparef_t ai_fFunction, m4pcvoid_t ai_pcvElement) const;

      //## Operation: GetTotalSize%1244502316; C++
      //	-- {AddDecl: 255} region.unprotectedFunction [2333..2564]
      m4uint32_t GetTotalSize (m4uint32_t& ao_riStatic);

    // Additional Public Declarations
      //## begin ClReferenceStack%3912A6BB0331.public preserve=yes
      //## end ClReferenceStack%3912A6BB0331.public

  protected:
    // Data Members for Associations

      //## Association: m4objglb::<unnamed>%3912A6BB033A
      //## Role: ClReferenceStack::m_iLength%3912A6BB033C
      //## begin ClReferenceStack::m_iLength%3912A6BB033C.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iLength;
      //## end ClReferenceStack::m_iLength%3912A6BB033C.role

      //## Association: m4objglb::<unnamed>%3912A6BB034F
      //## Role: ClReferenceStack::m_ppvElements%3912A6BB0351
      //## begin ClReferenceStack::m_ppvElements%3912A6BB0351.role preserve=no  protected: m4pvoid_t {1 -> 0..1RHAPN}
      m4pvoid_t *m_ppvElements;
      //## end ClReferenceStack::m_ppvElements%3912A6BB0351.role

    // Additional Protected Declarations
      //## begin ClReferenceStack%3912A6BB0331.protected preserve=yes
      //## end ClReferenceStack%3912A6BB0331.protected

  private:
    // Additional Private Declarations
      //## begin ClReferenceStack%3912A6BB0331.private preserve=yes
      //## end ClReferenceStack%3912A6BB0331.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClReferenceStack%3912A6BB0331.implementation preserve=yes
      //## end ClReferenceStack%3912A6BB0331.implementation

};

//## begin ClReferenceStack%3912A6BB0331.postscript preserve=yes
//## end ClReferenceStack%3912A6BB0331.postscript

//## begin ClPointerStack%3912A6BB0364.preface preserve=yes
//## end ClPointerStack%3912A6BB0364.preface

//## Class: ClPointerStack%3912A6BB0364
//	-- {AddDecl: 95} class [2388..7051]
//	=============================================================================
//	====
//
//	ClPointerStack
//
//	Clase de punteros a void
//
//	=============================================================================
//	====
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJGLB ClPointerStack 
{
  //## begin ClPointerStack%3912A6BB0364.initialDeclarations preserve=yes
  //## end ClPointerStack%3912A6BB0364.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPointerStack%-1633119007; C++
      //	-- {AddDecl: 257} region.unprotectedFunction [3407..3573]
      ClPointerStack (void );

    //## Destructor (specified)
      //## Operation: ~ClPointerStack%1586451529; C++
      //	-- {AddDecl: 258} region.unprotectedFunction [3574..3713]
      ~ClPointerStack (void );


    //## Other Operations (specified)
      //## Operation: GetLength%-1847253600; C++
      m4uint16_t GetLength (void ) const
      {
        //## begin ClPointerStack::GetLength%-1847253600.body preserve=yes
		return( m_iLength ) ;
	
        //## end ClPointerStack::GetLength%-1847253600.body
      }

      //## Operation: GetElement%2120348958; C++
      m4return_t GetElement (m4uint16_t ai_iPosition, m4pvoid_t& ao_rpvElement) const
      {
        //## begin ClPointerStack::GetElement%2120348958.body preserve=yes

#ifdef _DEBUG
        m4return_t  iResult ;


	    if( ai_iPosition >= m_iLength )
	    {
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << ai_iPosition << LogCat << " - " << LogCat << m_iLength ) ;
		    iResult = M4_ERROR ;
	    }
        else
        {
            ao_rpvElement = m_ppvElements[ ai_iPosition ] ;
		    iResult = M4_SUCCESS ;
        }

   	    return( iResult ) ;
#else
	    ao_rpvElement = m_ppvElements[ ai_iPosition ] ;
	    return( M4_SUCCESS ) ;
#endif

    
        //## end ClPointerStack::GetElement%2120348958.body
      }

      //## Operation: Init%-896158974; C++
      //	Funciones de lectura
      //	-- {AddDecl: 97} region.unprotectedFunction [3044..3104]
      //	-- {AddDecl: 259} region.unprotectedFunction [3714..4244]
      m4return_t Init (m4uint16_t ai_iSize, m4uint16_t ai_iDelta = M4CL_DEFAULT_DELTA);

      //## Operation: AddElement%1033999823; C++
      //	-- {AddDecl: 260} region.unprotectedFunction [4245..4618]
      m4return_t AddElement (m4pvoid_t ai_pvElement);

      //## Operation: Find%-1539410701; C++
      //	-- {AddDecl: 264} region.unprotectedFunction [5963..6881]
      m4pcvoid_t Find (m4comparef_t ai_fFunction, m4pcvoid_t ai_pcvElement) const;

      //## Operation: Find%-1866465103; C++
      //	-- {AddDecl: 265} region.unprotectedFunction [6882..7312]
      m4pcvoid_t Find (m4comparef_t ai_fFunction, m4pcvoid_t ai_pcvElement, m4uint16_t& ao_riElement) const;

      //## Operation: GetTotalSize%-760774687; C++
      //	-- {AddDecl: 267} region.unprotectedFunction [7533..8033]
      m4uint32_t GetTotalSize (m4uint32_t& ao_riStatic, m4uint32_t& ao_riPointers);

      //## Operation: GetSize%-723212187; C++
      m4uint16_t GetSize (void ) const
      {
        //## begin ClPointerStack::GetSize%-723212187.body preserve=yes
		return( m_iSize ) ;
	
        //## end ClPointerStack::GetSize%-723212187.body
      }

      //## Operation: GetElementPointer%-1071939142; C++
      m4return_t GetElementPointer (m4uint16_t ai_iPosition, m4pvoid_t*& ao_rppvElement) const
      {
        //## begin ClPointerStack::GetElementPointer%-1071939142.body preserve=yes

#ifdef _DEBUG
		m4return_t  iResult ;


		if( ai_iPosition >= m_iLength )
		{
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << ai_iPosition << LogCat << " - " << LogCat << m_iLength ) ;
			iResult = M4_ERROR ;
		}
		else
		{
			ao_rppvElement = &( m_ppvElements[ ai_iPosition ] ) ;
			iResult = M4_SUCCESS ;
		}

   		return( iResult ) ;
#else
		ao_rppvElement = &( m_ppvElements[ ai_iPosition ] ) ;
		return( M4_SUCCESS ) ;
#endif

	
        //## end ClPointerStack::GetElementPointer%-1071939142.body
      }

      //## Operation: Pop%885826127; C++
      m4return_t Pop (m4pvoid_t& ao_rpvElement)
      {
        //## begin ClPointerStack::Pop%885826127.body preserve=yes

#ifdef _DEBUG
        m4return_t  iResult ;


	    if( m_iLength == 0 )
	    {
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		    iResult = M4_ERROR ;
	    }
        else
        {
            ao_rpvElement = m_ppvElements[ --m_iLength ] ;
		    iResult = M4_SUCCESS ;
        }

   	    return( iResult ) ;
#else
	    ao_rpvElement = m_ppvElements[ --m_iLength ] ;
	    return( M4_SUCCESS ) ;
#endif

    
        //## end ClPointerStack::Pop%885826127.body
      }

      //## Operation: Top%-2000701226; C++
      m4return_t Top (m4pvoid_t& ao_rpvElement)
      {
        //## begin ClPointerStack::Top%-2000701226.body preserve=yes

#ifdef _DEBUG
        m4return_t  iResult ;


	    if( m_iLength == 0 )
	    {
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		    iResult = M4_ERROR ;
	    }
        else
        {
            ao_rpvElement = m_ppvElements[ m_iLength ] ;
		    iResult = M4_SUCCESS ;
        }

   	    return( iResult ) ;

#else
	    ao_rpvElement = m_ppvElements[ m_iLength ] ;
	    return( M4_SUCCESS ) ;
#endif

    
        //## end ClPointerStack::Top%-2000701226.body
      }

	void	SetLength (m4uint16_t ai_iLength)
	{
		if( ai_iLength < m_iLength )
		{
			m_iLength = ai_iLength  ;
		}
	}

      //## Operation: SetDelta%1023428538; C++
      void SetDelta (m4uint16_t ai_iDelta)
      {
        //## begin ClPointerStack::SetDelta%1023428538.body preserve=yes
        m_iDelta = ai_iDelta ;
    
        //## end ClPointerStack::SetDelta%1023428538.body
      }

      //## Operation: AddDelta%-1802493044; C++
      void AddDelta (m4uint16_t ai_iDelta)
      {
        //## begin ClPointerStack::AddDelta%-1802493044.body preserve=yes
        m_iDelta += ai_iDelta ;
    
        //## end ClPointerStack::AddDelta%-1802493044.body
      }

      //## Operation: SetElement%-304347995; C++
      m4return_t SetElement (m4uint16_t ai_iPosition, m4pvoid_t ai_pvElement) const
      {
        //## begin ClPointerStack::SetElement%-304347995.body preserve=yes

#ifdef _DEBUG
        m4return_t  iResult ;


	    if( ai_iPosition >= m_iLength )
	    {
			DUMP_CHLOG_DEBUGF( M4_OBJGLB_INTERNAL_ERROR_WITH_INFO, __LINE__ << __FILE__ << ai_iPosition << LogCat << " - " << LogCat << m_iLength ) ;
		    iResult = M4_ERROR ;
	    }
        else
        {
			m_ppvElements[ ai_iPosition ] = ai_pvElement ;
		    iResult = M4_SUCCESS ;
        }

   	    return( iResult ) ;
#else
		m_ppvElements[ ai_iPosition ] = ai_pvElement ;
	    return( M4_SUCCESS ) ;
#endif

    
        //## end ClPointerStack::SetElement%-304347995.body
      }

      //## Operation: AddElementIfNotIn%2002713089; C++
      //	-- {AddDecl: 261} region.unprotectedFunction [4619..4964]
      m4return_t AddElementIfNotIn (m4pvoid_t ai_pvElement);

      //## Operation: InsertElement%1880403762; C++
      //	-- {AddDecl: 107} region.unprotectedFunction [6381..6468]
      //	-- {AddDecl: 262} region.unprotectedFunction [4965..5689]
      m4return_t InsertElement (m4pvoid_t ai_pvElement, m4uint16_t ai_iPosition, m4uint8_t ai_iOrdered = 1);

      //## Operation: Reset%-2041215853; C++
      void Reset (void )
      {
        //## begin ClPointerStack::Reset%-2041215853.body preserve=yes
	    m_iLength = 0 ;
        m_bSorted = M4_TRUE ;
    
        //## end ClPointerStack::Reset%-2041215853.body
      }

      //## Operation: Reset%126581305; C++
      void Reset (m4uint16_t ai_iLength)
      {
        //## begin ClPointerStack::Reset%126581305.body preserve=yes
		if( ai_iLength < m_iLength )
		{
			m_iLength = ai_iLength ;

			if( m_iLength == 0 )
			{
				m_bSorted = M4_TRUE ;
			}
		}
    
        //## end ClPointerStack::Reset%126581305.body
      }

      //## Operation: Sort%-1846020125; C++
      //	-- {AddDecl: 263} region.unprotectedFunction [5690..5962]
      m4return_t Sort (m4comparef_t ai_fFunction);

      m4return_t Sort (m4comparef_t ai_fFunction, m4uint16_t ai_iStart, m4uint16_t ai_iEnd );

      //## Operation: ReAllocatePointers%-1908955308; C++
      //	-- {AddDecl: 266} region.unprotectedFunction [7313..7532]
      void ReAllocatePointers (size_t ai_iOffset);

    // Additional Public Declarations
      //## begin ClPointerStack%3912A6BB0364.public preserve=yes
      //## end ClPointerStack%3912A6BB0364.public

  protected:

    //## Other Operations (specified)
      //## Operation: _Increment%1205121754; C++
      //	-- {AddDecl: 256} region.unprotectedFunction [2565..3406]
      //	=============================================================================
      //	====
      //
      //	ClPointerStack
      //
      //	Clase de punteros a void
      //
      //	=============================================================================
      //	====
      m4return_t _Increment (void );

    // Data Members for Associations

      //## Association: m4objglb::<unnamed>%3912A6BB038A
      //## Role: ClPointerStack::m_iLength%3912A6BB038C
      //## begin ClPointerStack::m_iLength%3912A6BB038C.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iLength;
      //## end ClPointerStack::m_iLength%3912A6BB038C.role

      //## Association: m4objglb::<unnamed>%3912A6BB038D
      //## Role: ClPointerStack::m_ppvElements%3912A6BB038F
      //## begin ClPointerStack::m_ppvElements%3912A6BB038F.role preserve=no  protected: m4pvoid_t {1 -> 0..1RHAPN}
      m4pvoid_t *m_ppvElements;
      //## end ClPointerStack::m_ppvElements%3912A6BB038F.role

      //## Association: m4objglb::<unnamed>%3912A6BB03A8
      //## Role: ClPointerStack::m_iSize%3912A6BB03AA
      //## begin ClPointerStack::m_iSize%3912A6BB03AA.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iSize;
      //## end ClPointerStack::m_iSize%3912A6BB03AA.role

      //## Association: m4objglb::<unnamed>%3912A6BB03AB
      //## Role: ClPointerStack::m_iDelta%3912A6BB03AD
      //## begin ClPointerStack::m_iDelta%3912A6BB03AD.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iDelta;
      //## end ClPointerStack::m_iDelta%3912A6BB03AD.role

      //## Association: m4objglb::<unnamed>%3912A6BB03AE
      //## Role: ClPointerStack::m_bSorted%3912A6BB03B0
      //## begin ClPointerStack::m_bSorted%3912A6BB03B0.role preserve=no  protected: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bSorted;
      //## end ClPointerStack::m_bSorted%3912A6BB03B0.role

    // Additional Protected Declarations
      //## begin ClPointerStack%3912A6BB0364.protected preserve=yes
      //## end ClPointerStack%3912A6BB0364.protected

  private:
    // Additional Private Declarations
      //## begin ClPointerStack%3912A6BB0364.private preserve=yes
      //## end ClPointerStack%3912A6BB0364.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPointerStack%3912A6BB0364.implementation preserve=yes
      //## end ClPointerStack%3912A6BB0364.implementation

};

//## begin ClPointerStack%3912A6BB0364.postscript preserve=yes
//## end ClPointerStack%3912A6BB0364.postscript

//## begin module%3912A6BB0330.epilog preserve=yes
//## end module%3912A6BB0330.epilog


#endif
