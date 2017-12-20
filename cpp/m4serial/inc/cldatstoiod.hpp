//## begin module%3911AA2302E5.cm preserve=no
//## end module%3911AA2302E5.cm

//## begin module%3911AA2302E5.cp preserve=no
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
//## end module%3911AA2302E5.cp

//## Module: cldatstoiod%3911AA2302E5; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: d:\new\m4serial\inc\cldatstoiod.hpp

#ifndef cldatstoiod_h
#define cldatstoiod_h 1

//## begin module%3911AA2302E5.additionalIncludes preserve=no
//## end module%3911AA2302E5.additionalIncludes

//## begin module%3911AA2302E5.includes preserve=yes

#include "m4serial_dll.hpp"
#include "m4serial.hpp"
#include "clgeniod.hpp"

#include "cltadof.hpp"
#include <datastorages.hpp>


#define MAX_LENG_BLOCK (16*1024)

//## end module%3911AA2302E5.includes

// m4types
#include <m4types.hpp>
//## begin module%3911AA2302E5.declarations preserve=no
//## end module%3911AA2302E5.declarations

//## begin module%3911AA2302E5.additionalDeclarations preserve=yes
//## end module%3911AA2302E5.additionalDeclarations


//## begin ClDataStoreIODriver%3911AA23034A.preface preserve=yes
//## end ClDataStoreIODriver%3911AA23034A.preface

//## Class: ClDataStoreIODriver%3911AA23034A
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClDataStoreIODriver : public ClGenericIODriver  //## Inherits: <unnamed>%3911AA240231
{
  //## begin ClDataStoreIODriver%3911AA23034A.initialDeclarations preserve=yes
  //## end ClDataStoreIODriver%3911AA23034A.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDataStoreIODriver%2012976625; C++
      //	Ojo m4date_t	= m4double_t	en la Serializacion
      //	Una fecha se serializa como un double
      ClDataStoreIODriver (M4DataStorage* ai_poDataStor = 0);

      //## Operation: ClDataStoreIODriver%-1934284263; C++
      //	-- {AddDecl: 128} region.unprotectedFunction [1192..1402]
      ClDataStoreIODriver (m4uint32_t ai_iMode, M4DataStorage* ai_poDataStor, m4uint16_t ai_iServerArchitect = M4_ARCHITECTURE);

    //## Destructor (specified)
      //## Operation: ~ClDataStoreIODriver%-290893492; C++
      //	Write
      //	-- {AddDecl: 130} region.unprotectedFunction [4357..4416]
      ~ClDataStoreIODriver ();


    //## Other Operations (specified)
      //## Operation: Init%-207712432; C++
      //	-- {AddDecl: 129} region.unprotectedFunction [1403..4356]
      m4return_t Init (m4uint32_t ai_iMode, M4DataStorage* ai_poDataStor = 0, m4uint16_t ai_iServerArchitect = M4_ARCHITECTURE);

      //## Operation: Write%-291530675; C++
      m4return_t Write (m4uint32_t ai_iNumber);

      //## Operation: Write%1146694729; C++
      //	-- {AddDecl: 135} region.unprotectedFunction [6893..7125]
      m4return_t Write (m4int32_t ai_iNumber);

      //## Operation: Write%2114694442; C++
      //	-- {AddDecl: 136} region.unprotectedFunction [7126..7359]
      m4return_t Write (m4uint16_t ai_iNumber);

      //## Operation: Write%-1297262124; C++
      //	-- {AddDecl: 137} region.unprotectedFunction [7360..7592]
      m4return_t Write (m4int16_t ai_iNumber);

      //## Operation: Write%-1261357684; C++
      //	-- {AddDecl: 138} region.unprotectedFunction [7593..7764]
      m4return_t Write (m4uint8_t ai_cChar);

      //## Operation: Write%411937274; C++
      //	-- {AddDecl: 139} region.unprotectedFunction [7765..7935]
      m4return_t Write (m4int8_t ai_cChar);

      //## Operation: Write%-1907242843; C++
      //	-- {AddDecl: 140} region.unprotectedFunction [7936..8168]
      m4return_t Write (m4float_t ai_fNumber);

      //## Operation: Write%890730920; C++
      //	-- {AddDecl: 141} region.unprotectedFunction [8169..8402]
      m4return_t Write (m4double_t ai_dNumber);

      //## Operation: Write%1417622219; C++
      //	-- {AddDecl: 142} region.unprotectedFunction [8403..8836]
      m4return_t Write (m4bool_t ai_iBoolean);

      //## Operation: Write%-1825450731; C++
      //	-- {AddDecl: 144} region.unprotectedFunction [10200..10370]
      m4return_t Write (void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: Write%-874616728; C++
      //	-- {AddDecl: 146} region.unprotectedFunction [10563..12329]
      m4return_t Write (void* ai_pPtr, m4uint32_t ai_iSize);

      //## Operation: Write%-971968883; C++
      //	-- {AddDecl: 147} region.unprotectedFunction [12330..12694]
      m4return_t Write (m4pchar_t ai_pString);

      //## Operation: Write%-1525880265; C++
      //	Files...
      //	This method stores the data of a file in the stream
      //	-- {AddDecl: 148} region.unprotectedFunction [12695..12810]
      m4return_t Write (m4pcchar_t ai_pConstString);

      //## Operation: WritePrivate%-315363350; C++
      //	-- {AddDecl: 145} region.unprotectedFunction [10371..10562]
      m4return_t WritePrivate (void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: WriteFromFile%686724163; C++
      //	Read
      //	-- {AddDecl: 149} region.unprotectedFunction [12811..13087]
      //	Files...
      //	This method stores the data of a file in the stream
      m4return_t WriteFromFile (m4pcchar_t ai_pFileName);

      //## Operation: Read%-1688627031; C++
      m4return_t Read (m4uint32_t& ai_iNumber);

      //## Operation: Read%-73745664; C++
      //	-- {AddDecl: 152} region.unprotectedFunction [15451..15759]
      m4return_t Read (m4int32_t& ai_iNumber);

      //## Operation: Read%1634679727; C++
      //	-- {AddDecl: 153} region.unprotectedFunction [15760..16069]
      m4return_t Read (m4uint16_t& ai_iNumber);

      //## Operation: Read%1050031673; C++
      //	-- {AddDecl: 154} region.unprotectedFunction [16070..16378]
      m4return_t Read (m4int16_t& ai_iNumber);

      //## Operation: Read%-1048403713; C++
      //	-- {AddDecl: 155} region.unprotectedFunction [16379..16626]
      m4return_t Read (m4uint8_t& ai_cChar);

      //## Operation: Read%946191249; C++
      //	-- {AddDecl: 156} region.unprotectedFunction [16627..16873]
      m4return_t Read (m4int8_t& ai_cChar);

      //## Operation: Read%558489178; C++
      //	-- {AddDecl: 157} region.unprotectedFunction [16874..17181]
      m4return_t Read (m4float_t& ai_fNumber);

      //## Operation: Read%-674859141; C++
      //	-- {AddDecl: 158} region.unprotectedFunction [17182..17490]
      m4return_t Read (m4double_t& ai_dNumber);

      //## Operation: Read%-2135133207; C++
      //	-- {AddDecl: 159} region.unprotectedFunction [17491..17955]
      m4return_t Read (m4bool_t& ai_iBoolean);

      //## Operation: Read%-35277620; C++
      //	-- {AddDecl: 161} region.unprotectedFunction [19380..19547]
      m4return_t Read (void** ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: Read%-1744485436; C++
      //	-- {AddDecl: 171} region.unprotectedFunction [23063..23341]
      m4return_t Read (void* ao_pPtr, m4uint32_t& ao_iSize);

      //## Operation: Read%1710873989; C++
      //	-- {AddDecl: 174} region.unprotectedFunction [23798..24199]
      m4return_t Read (m4pchar_t ao_pString, m4uint32_t ai_iMaxLen = 0);

      //## Operation: Read%-1845492164; C++
      //	Files...
      //	This method creates a file from the data in the stream
      //	If FileName is a void string (""), the method returns the name generated
      //	inside,
      //	otherwise the name passed is used...
      //	-- {AddDecl: 175} region.unprotectedFunction [24200..24625]
      m4return_t Read (m4pcchar_t ao_pConstString, m4uint32_t ai_iMaxLen = 0);

      //## Operation: ReadPrivate%524631985; C++
      //	-- {AddDecl: 162} region.unprotectedFunction [19548..19736]
      m4return_t ReadPrivate (void** ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: ReadSize%-1712516114; C++
      //	-- {AddDecl: 172} region.unprotectedFunction [23342..23564]
      m4return_t ReadSize (m4uint32_t& ao_iSize);

      //## Operation: ReadBuffer%1743208874; C++
      //	-- {AddDecl: 173} region.unprotectedFunction [23565..23797]
      m4return_t ReadBuffer (void* ao_pPtr);

      //## Operation: ReadToFile%1635854610; C++
      //	Others
      //	-- {AddDecl: 176} region.unprotectedFunction [24626..25200]
      //	Files...
      //	This method creates a file from the data in the stream
      //	This method assumes tat the string holds the needed space...
      m4return_t ReadToFile (m4pchar_t ao_pFileName);

      //## Operation: Eof%-1753696491; C++
      //	-- {AddDecl: 178} region.unprotectedFunction [25622..25682]
      m4bool_t Eof (void );

      //## Operation: Flush%-1309285846; C++
      //	-- {AddDecl: 179} region.unprotectedFunction [25683..25953]
      m4bool_t Flush (void );

      //## Operation: Reset%1265143797; C++
      //	-- {AddDecl: 177} region.unprotectedFunction [25201..25287]
      void Reset (void );

      //## Operation: StoreAddress%-957016152; C++
      //	-- {AddDecl: 164} region.unprotectedFunction [20328..20439]
      m4return_t StoreAddress (void* ai_pPtr);

      //## Operation: StoreAddress%447084782; C++
      //	-- {AddDecl: 167} region.unprotectedFunction [21105..21300]
      m4return_t StoreAddress (void* ai_pPtr, m4uint32_t ai_iOffset);

      //## Operation: StoreAddressPrivate%-1412875484; C++
      //	-- {AddDecl: 165} region.unprotectedFunction [20440..20565]
      m4return_t StoreAddressPrivate (void* ai_pPtr);

      //## Operation: StoreAddressPrivate%217573661; C++
      //	-- {AddDecl: 168} region.unprotectedFunction [21301..21517]
      m4return_t StoreAddressPrivate (void* ai_pPtr, m4uint32_t ai_iOffset);

      //## Operation: GetActualOffset%1235932099; C++
      //	-- {AddDecl: 169} region.unprotectedFunction [21518..21596]
      m4uint32_t GetActualOffset (void );

      //## Operation: ResetPrivateInfo%-1813344334; C++
      //	-- {AddDecl: 80} region.unprotectedFunction [4033..4096]
      //	-- {AddDecl: 180} region.unprotectedFunction [25954..26063]
      m4return_t ResetPrivateInfo (void );

      //## Operation: GetDataStorage%-1656260526; C++
      M4DataStorage* GetDataStorage ();

      //## Operation: HasToSwap%-193875813; C++
      m4bool_t HasToSwap (void ) const;

      //## Operation: End%-161200245; C++
      //	-- {AddDecl: 131} region.unprotectedFunction [4417..4692]
      void End (void );

    // Additional Public Declarations
      //## begin ClDataStoreIODriver%3911AA23034A.public preserve=yes
      //## end ClDataStoreIODriver%3911AA23034A.public

  protected:
    // Additional Protected Declarations
      //## begin ClDataStoreIODriver%3911AA23034A.protected preserve=yes
      //## end ClDataStoreIODriver%3911AA23034A.protected

  private:

    //## Other Operations (specified)
      //## Operation: WritePointer%-1690783765; C++
      //	-- {AddDecl: 143} region.unprotectedFunction [8837..10199]
      m4return_t WritePointer (ClTableAddressOffset* ai_pTable, void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: ReadPointer%71432984; C++
      //	-- {AddDecl: 160} region.unprotectedFunction [17956..19379]
      m4return_t ReadPointer (ClTableAddressOffset* ai_pTable, void** ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: StoreAddressPointer%311046174; C++
      //	-- {AddDecl: 163} region.unprotectedFunction [19737..20327]
      m4return_t StoreAddressPointer (ClTableAddressOffset* ai_pTable, void* ai_pPtr);

      //## Operation: StoreAddressPointer%-145059750; C++
      //	-- {AddDecl: 166} region.unprotectedFunction [20566..21104]
      m4return_t StoreAddressPointer (ClTableAddressOffset* ai_pTable, void* ai_pPtr, m4uint32_t ai_iOffset);

      //## Operation: ReadPtr%1677070765; C++
      //	-- {AddDecl: 170} region.unprotectedFunction [21597..23062]
      void ReadPtr (void* ao_pPtr, m4uint32_t ai_iSize);

      //## Operation: _Interchange%-1345979075; C++
      //	-- {AddDecl: 132} region.unprotectedFunction [4693..5077]
      //	////////////////////////////	ARCHITECTURE TRANSLATION
      //	///////////////////////////
      void _Interchange (m4char_t* ai_vector, m4int32_t ai_tamano);

    // Data Members for Associations

      //## Association: M4Serial::<unnamed>%3911AA24020A
      //## Role: ClDataStoreIODriver::m_oTAO%3911AA24020C
      //## begin ClDataStoreIODriver::m_oTAO%3911AA24020C.role preserve=no  private: ClTableAddressOffset {1 -> 0..1RHAPN}
      ClTableAddressOffset *m_oTAO;
      //## end ClDataStoreIODriver::m_oTAO%3911AA24020C.role

      //## Association: M4Serial::<unnamed>%3911AA24020D
      //## Role: ClDataStoreIODriver::m_oPrivateTAO%3911AA24020F
      //## begin ClDataStoreIODriver::m_oPrivateTAO%3911AA24020F.role preserve=no  private: ClTableAddressOffset {1 -> 0..1RHAPN}
      ClTableAddressOffset *m_oPrivateTAO;
      //## end ClDataStoreIODriver::m_oPrivateTAO%3911AA24020F.role

      //## Association: M4Serial::<unnamed>%3911AA2400A1
      //## Role: ClDataStoreIODriver::m_poDataStor%3911AA2400B5
      //## begin ClDataStoreIODriver::m_poDataStor%3911AA2400B5.role preserve=no  private: M4DataStorage {1 -> 0..1RHAPN}
      M4DataStorage *m_poDataStor;
      //## end ClDataStoreIODriver::m_poDataStor%3911AA2400B5.role

      //## Association: M4Serial::<unnamed>%3911AA2400CB
      //## Role: ClDataStoreIODriver::m_bHasToSwap%3911AA2400CD
      //## begin ClDataStoreIODriver::m_bHasToSwap%3911AA2400CD.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bHasToSwap;
      //## end ClDataStoreIODriver::m_bHasToSwap%3911AA2400CD.role

      //## Association: M4Serial::<unnamed>%3911AA2400E9
      //## Role: ClDataStoreIODriver::m_pInitialPtr%3911AA2400EB
      //## begin ClDataStoreIODriver::m_pInitialPtr%3911AA2400EB.role preserve=no  private: m4char_t {1 -> 0..1RHAPN}
      m4char_t *m_pInitialPtr;
      //## end ClDataStoreIODriver::m_pInitialPtr%3911AA2400EB.role

      //## Association: M4Serial::<unnamed>%3911AA2400EC
      //## Role: ClDataStoreIODriver::m_pPtr%3911AA2400EE
      //## begin ClDataStoreIODriver::m_pPtr%3911AA2400EE.role preserve=no  private: m4char_t {1 -> 0..1RHAPN}
      m4char_t *m_pPtr;
      //## end ClDataStoreIODriver::m_pPtr%3911AA2400EE.role

      //## Association: M4Serial::<unnamed>%3911AA2400F0
      //## Role: ClDataStoreIODriver::m_iLastSize%3911AA2400F2
      //## begin ClDataStoreIODriver::m_iLastSize%3911AA2400F2.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_iLastSize;
      //## end ClDataStoreIODriver::m_iLastSize%3911AA2400F2.role

      //## Association: M4Serial::<unnamed>%3911AA240211
      //## Role: ClDataStoreIODriver::m_iMode%3911AA240213
      //## begin ClDataStoreIODriver::m_iMode%3911AA240213.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iMode;
      //## end ClDataStoreIODriver::m_iMode%3911AA240213.role

      //## Association: M4Serial::<unnamed>%3911AA240214
      //## Role: ClDataStoreIODriver::m_uiOffset%3911AA240216
      //## begin ClDataStoreIODriver::m_uiOffset%3911AA240216.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_uiOffset;
      //## end ClDataStoreIODriver::m_uiOffset%3911AA240216.role

      //## Association: M4Serial::<unnamed>%3911AA240217
      //## Role: ClDataStoreIODriver::m_iBlockSize%3911AA240219
      //## begin ClDataStoreIODriver::m_iBlockSize%3911AA240219.role preserve=no  private: m4uint32_t {1 -> 1VHAPN}
      m4uint32_t m_iBlockSize;
      //## end ClDataStoreIODriver::m_iBlockSize%3911AA240219.role

      //## Association: M4Serial::<unnamed>%3911AA24021A
      //## Role: ClDataStoreIODriver::m_bProp%3911AA24021C
      //## begin ClDataStoreIODriver::m_bProp%3911AA24021C.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bProp;
      //## end ClDataStoreIODriver::m_bProp%3911AA24021C.role

      //## Association: M4Serial::<unnamed>%3911AA24021E
      //## Role: ClDataStoreIODriver::m_pDataUnitInterface%3911AA240220
      //## begin ClDataStoreIODriver::m_pDataUnitInterface%3911AA240220.role preserve=no  private: ClDataUnitInterface {1 -> 0..1RHAPN}
      ClDataUnitInterface *m_pDataUnitInterface;
      //## end ClDataStoreIODriver::m_pDataUnitInterface%3911AA240220.role

      //## Association: M4Serial::<unnamed>%3911AA240222
      //## Role: ClDataStoreIODriver::m_pDataUnit%3911AA240224
      //## begin ClDataStoreIODriver::m_pDataUnit%3911AA240224.role preserve=no  private: ClDataUnit {1 -> 0..1RHAPN}
      ClDataUnit *m_pDataUnit;
      //## end ClDataStoreIODriver::m_pDataUnit%3911AA240224.role

      //## Association: M4Serial::<unnamed>%3911AA240226
      //## Role: ClDataStoreIODriver::m_iLastType%3911AA240228
      //## begin ClDataStoreIODriver::m_iLastType%3911AA240228.role preserve=no  private: m4int16_t {1 -> 1VHAPN}
      m4int16_t m_iLastType;
      //## end ClDataStoreIODriver::m_iLastType%3911AA240228.role

      //## Association: M4Serial::<unnamed>%3911AA240229
      //## Role: ClDataStoreIODriver::m_iEOF%3911AA24022B
      //## begin ClDataStoreIODriver::m_iEOF%3911AA24022B.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_iEOF;
      //## end ClDataStoreIODriver::m_iEOF%3911AA24022B.role

    // Additional Private Declarations
      //## begin ClDataStoreIODriver%3911AA23034A.private preserve=yes
      //## end ClDataStoreIODriver%3911AA23034A.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDataStoreIODriver%3911AA23034A.implementation preserve=yes
      //## end ClDataStoreIODriver%3911AA23034A.implementation

};

//## begin ClDataStoreIODriver%3911AA23034A.postscript preserve=yes
//## end ClDataStoreIODriver%3911AA23034A.postscript

// Class ClDataStoreIODriver 


//## Other Operations (inline)
inline M4DataStorage* ClDataStoreIODriver::GetDataStorage ()
{
  //## begin ClDataStoreIODriver::GetDataStorage%-1656260526.body preserve=yes
 return m_poDataStor ; 
  //## end ClDataStoreIODriver::GetDataStorage%-1656260526.body
}

inline m4bool_t ClDataStoreIODriver::HasToSwap (void ) const
{
  //## begin ClDataStoreIODriver::HasToSwap%-193875813.body preserve=yes
		  return( m_bHasToSwap ) ;
	  
  //## end ClDataStoreIODriver::HasToSwap%-193875813.body
}

//## begin module%3911AA2302E5.epilog preserve=yes
//## end module%3911AA2302E5.epilog


#endif
