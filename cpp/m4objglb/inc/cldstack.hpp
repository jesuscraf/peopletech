//## begin module%3912A6B703CA.cm preserve=no
//## end module%3912A6B703CA.cm

//## begin module%3912A6B703CA.cp preserve=no
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
//## end module%3912A6B703CA.cp

//## Module: cldstack%3912A6B703CA; Package specification
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Source file: d:\new\m4objglb\inc\cldstack.hpp

#ifndef __CLDSTACK_HPP__
#define __CLDSTACK_HPP__ 1

//## begin module%3912A6B703CA.additionalIncludes preserve=no
//## end module%3912A6B703CA.additionalIncludes

//## begin module%3912A6B703CA.includes preserve=yes

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:              m4objglb.dll
// File:                cldstack.hpp
// Project:             mind3.x
// Author:              Meta Software M.S. , S.A
// Date:                13-06-1997
// Language:            C++
// Operating System:    WINDOWS
// Design Document:     XXX.DOC
//
//
// Definition:
//
//    This module defines the stack for dates slices
//
//
//==============================================================================



#include "m4objglb_dll.hpp"
#include "m4glbres.hpp"

#include "cltypes.hpp"
#include "cldefine.hpp"
#include "chlog.hpp"


//## end module%3912A6B703CA.includes

// m4types
#include <m4types.hpp>
//## begin module%3912A6B703CA.declarations preserve=no
//## end module%3912A6B703CA.declarations

//## begin module%3912A6B703CA.additionalDeclarations preserve=yes
//## end module%3912A6B703CA.additionalDeclarations


//## begin ClDateStack%3912A6B703D5.preface preserve=yes
//## end ClDateStack%3912A6B703D5.preface

//## Class: ClDateStack%3912A6B703D5
//## Category: m4objglb%3912A6B703D4
//## Subsystem: m4objglb::inc%3912A6EC01C8
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4OBJGLB ClDateStack 
{
  //## begin ClDateStack%3912A6B703D5.initialDeclarations preserve=yes
  //## end ClDateStack%3912A6B703D5.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDateStack%-241389089; C++
      //	-- {AddDecl: 211} region.unprotectedFunction [2129..2299]
      ClDateStack (void );

      //## Operation: ClDateStack%-803545528; C++
      //	-- {AddDecl: 212} region.unprotectedFunction [2300..2883]
      ClDateStack (const ClDateStack& ai_oRhs);

    //## Destructor (specified)
      //## Operation: ~ClDateStack%1915633445; C++
      //	-- {AddDecl: 213} region.unprotectedFunction [2884..3010]
      ~ClDateStack (void );


    //## Other Operations (specified)
      //## Operation: Init%2054287195; C++
      //	-- {AddDecl: 214} region.unprotectedFunction [3011..3755]
      m4return_t Init (m4date_t ai_dStartDate, m4date_t ai_dEndDate, m4uint16_t ai_iSize, m4uint16_t ai_iDelta = M4CL_DEFAULT_DELTA);

      //## Operation: Clear%408685202; C++
      //	-- {AddDecl: 215} region.unprotectedFunction [3756..3842]
      m4return_t Clear (void );

      //## Operation: Clear%-530398764; C++
      //	Funciones de lectura
      //	-- {AddDecl: 33} region.unprotectedFunction [1675..1735]
      //	-- {AddDecl: 216} region.unprotectedFunction [3843..4077]
      m4return_t Clear (m4date_t ai_dStartDate, m4date_t ai_dEndDate);

      //## Operation: GetSize%-581353969; C++
      m4uint16_t GetSize (void ) const
      {
        //## begin ClDateStack::GetSize%-581353969.body preserve=yes
		return( m_iSize ) ;
	
        //## end ClDateStack::GetSize%-581353969.body
      }

      //## Operation: GetLength%-1968383317; C++
      m4uint16_t GetLength (void ) const
      {
        //## begin ClDateStack::GetLength%-1968383317.body preserve=yes
		return( m_iLength ) ;
	
        //## end ClDateStack::GetLength%-1968383317.body
      }

      //## Operation: GetEndDate%-409222096; C++
      m4date_t GetEndDate (void ) const
      {
        //## begin ClDateStack::GetEndDate%-409222096.body preserve=yes
        return( m_dEndDate ) ;
    
        //## end ClDateStack::GetEndDate%-409222096.body
      }

      //## Operation: GetStartDate%-81211651; C++
      m4date_t GetStartDate (void ) const
      {
        //## begin ClDateStack::GetStartDate%-81211651.body preserve=yes
        return( m_dStartDate ) ;
    
        //## end ClDateStack::GetStartDate%-81211651.body
      }

      //## Operation: GetElements%957977757; C++
      m4date_t* GetElements (void ) const
      {
        //## begin ClDateStack::GetElements%957977757.body preserve=yes
		  return( m_pdElements ) ;
        //## end ClDateStack::GetElements%957977757.body
      }

      //## Operation: GetElement%-1453061178; C++
      //	Funciones de escritura
      //	-- {AddDecl: 217} region.unprotectedFunction [4078..4431]
      m4return_t GetElement (m4uint16_t ai_iPosition, m4date_t& ao_rdElement) const;

      //## Operation: AddEndDate%43275926; C++
      //	-- {AddDecl: 38} region.unprotectedFunction [2134..2568]
      //	-- {AddDecl: 218} region.unprotectedFunction [4432..4777]
      m4return_t AddEndDate (m4date_t ai_iTimeUnit);

      //## Operation: AddElement%1407685379; C++
      m4return_t AddElement (m4date_t ai_dElement)
      {
        //## begin ClDateStack::AddElement%1407685379.body preserve=yes
		m4return_t	iResult ;
		
		
		if( ai_dElement > m_dEndDate || ai_dElement <= m_dStartDate )
		{
			return( M4_SUCCESS ) ;
		}
		
		if( m_iLength >= m_iSize )
		{
			iResult = _Increment() ;
			
			CHECK_CHLOG_DEBUGF( iResult != M4_SUCCESS, M4_ERROR, M4_OBJGLB_INTERNAL_ERROR, __LINE__ << __FILE__ ) ;
		}
		
		m_pdElements[ m_iLength++ ] = ai_dElement ;
		
		return( M4_SUCCESS ) ;
	
        //## end ClDateStack::AddElement%1407685379.body
      }

      //## Operation: Reduce%-266750812; C++
      //	-- {AddDecl: 219} region.unprotectedFunction [4778..5401]
      m4return_t Reduce (void );

      //## Operation: ClipReduce%383569071; C++
      //	Operators
      //	-- {AddDecl: 220} region.unprotectedFunction [5402..6482]
      m4return_t ClipReduce (m4date_t ai_dStartDate, m4date_t ai_dEndDate);

      //## Operation: Copy%957978193
      m4return_t Copy (m4date_t*& ao_pdDatesArray, m4uint16_t ai_iLength) const;

      //## Operation: operator =%776598467; C++
      //	-- {AddDecl: 221} region.unprotectedFunction [6483..7270]
      //	Operators
      ClDateStack& operator = (const ClDateStack& ai_oRhs);

      //## Operation: operator +=%-1267398125; C++
      //	-- {AddDecl: 222} region.unprotectedFunction [7271..7527]
      ClDateStack& operator += (const ClDateStack& ai_oRhs);

    // Additional Public Declarations
      //## begin ClDateStack%3912A6B703D5.public preserve=yes
      //## end ClDateStack%3912A6B703D5.public

  protected:

    //## Other Operations (specified)
      //## Operation: _Increment%-2053271243; C++
      //	=============================================================================
      //	====
      //
      //	ClDateStack
      //
      //	Clase de array de fechas
      //
      //	=============================================================================
      //	====
      m4return_t _Increment (void );

      //## Operation: _Sort%1889313345; C++
      //	-- {AddDecl: 210} region.unprotectedFunction [1968..2128]
      m4return_t _Sort (void ) const;

    // Data Members for Associations

      //## Association: m4objglb::<unnamed>%3912A6B8006E
      //## Role: ClDateStack::m_dStartDate%3912A6B80070
      //## begin ClDateStack::m_dStartDate%3912A6B80070.role preserve=no  protected: m4date_t {1 -> 1VHAPN}
      m4date_t m_dStartDate;
      //## end ClDateStack::m_dStartDate%3912A6B80070.role

      //## Association: m4objglb::<unnamed>%3912A6B80071
      //## Role: ClDateStack::m_dEndDate%3912A6B80073
      //## begin ClDateStack::m_dEndDate%3912A6B80073.role preserve=no  protected: m4date_t {1 -> 1VHAPN}
      m4date_t m_dEndDate;
      //## end ClDateStack::m_dEndDate%3912A6B80073.role

      //## Association: m4objglb::<unnamed>%3912A6B80075
      //## Role: ClDateStack::m_iLength%3912A6B80077
      //## begin ClDateStack::m_iLength%3912A6B80077.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iLength;
      //## end ClDateStack::m_iLength%3912A6B80077.role

      //## Association: m4objglb::<unnamed>%3912A6B80078
      //## Role: ClDateStack::m_iSize%3912A6B8007A
      //## begin ClDateStack::m_iSize%3912A6B8007A.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iSize;
      //## end ClDateStack::m_iSize%3912A6B8007A.role

      //## Association: m4objglb::<unnamed>%3912A6B8007B
      //## Role: ClDateStack::m_iDelta%3912A6B8007D
      //## begin ClDateStack::m_iDelta%3912A6B8007D.role preserve=no  protected: m4uint16_t {1 -> 1VHAPN}
      m4uint16_t m_iDelta;
      //## end ClDateStack::m_iDelta%3912A6B8007D.role

      //## Association: m4objglb::<unnamed>%3912A6B8007E
      //## Role: ClDateStack::m_pdElements%3912A6B80080
      //## begin ClDateStack::m_pdElements%3912A6B80080.role preserve=no  protected: m4date_t {1 -> 0..1RHAPN}
      m4date_t *m_pdElements;
      //## end ClDateStack::m_pdElements%3912A6B80080.role

    // Additional Protected Declarations
      //## begin ClDateStack%3912A6B703D5.protected preserve=yes
      //## end ClDateStack%3912A6B703D5.protected

  private:
    // Additional Private Declarations
      //## begin ClDateStack%3912A6B703D5.private preserve=yes
      //## end ClDateStack%3912A6B703D5.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDateStack%3912A6B703D5.implementation preserve=yes
      //## end ClDateStack%3912A6B703D5.implementation

};

//## begin ClDateStack%3912A6B703D5.postscript preserve=yes
//## end ClDateStack%3912A6B703D5.postscript

//## begin module%3912A6B703CA.epilog preserve=yes

//otros operadores
ClDateStack  M4_DECL_M4OBJGLB operator+ (const ClDateStack &ai_oRhs);



//## end module%3912A6B703CA.epilog


#endif
