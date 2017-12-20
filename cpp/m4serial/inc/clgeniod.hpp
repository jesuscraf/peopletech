//## begin module%3911AA240230.cm preserve=no
//## end module%3911AA240230.cm

//## begin module%3911AA240230.cp preserve=no
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
//## end module%3911AA240230.cp

//## Module: clgeniod%3911AA240230; Package specification
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Source file: D:\evol\m4serial\inc\clgeniod.hpp

#ifndef clgeniod_h
#define clgeniod_h 1

//## begin module%3911AA240230.additionalIncludes preserve=no
//## end module%3911AA240230.additionalIncludes

//## begin module%3911AA240230.includes preserve=yes

#include "m4serial_dll.hpp"
#include "m4types.hpp"
//## end module%3911AA240230.includes

//## begin module%3911AA240230.declarations preserve=no
//## end module%3911AA240230.declarations

//## begin module%3911AA240230.additionalDeclarations preserve=yes

// Generic constants
const m4uint32_t M4_SERIAL_MAX_LONG_BUFFER = 0x7fff; // 32Kb

// Using modes (por ahora son exclusivos CRG)
const m4uint32_t GEN_IO_MODE_READ   = 1; // Reading from the IODriver (de-serializing
const m4uint32_t GEN_IO_MODE_WRITE  = 2; // Writing (serializing)
const m4uint32_t GEN_IO_MODE_APPEND = 4; // Adding to an exixting Driver (adding to a serialized object)
const m4uint32_t GEN_IO_MODE_NONE   = 8; // We are calculating sizes and offsets only...
const m4uint32_t GEN_IO_MODE_REWRITE=16; // Writting at any position

//## end module%3911AA240230.additionalDeclarations


//## begin ClGenericIODriver%3911AA24022F.preface preserve=yes
//## end ClGenericIODriver%3911AA24022F.preface

//## Class: ClGenericIODriver%3911AA24022F
//	We are calculating sizes and offsets only...
//## Category: M4Serial%3911AA230349
//## Subsystem: M4Serial::inc%3911B0FA01FC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4SERIAL ClGenericIODriver 
{
  //## begin ClGenericIODriver%3911AA24022F.initialDeclarations preserve=yes
public:
	  enum GenErrors { GEN_ERR_OK=0, GEN_ERR_OUT_OF_MEMORY, GEN_ERR_UNKNOWN, GEN_ERR_INIT_WRONG_PARAMS, GEN_ERR_N_ERRORS };
  //## end ClGenericIODriver%3911AA24022F.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClGenericIODriver%-493449199; C++
      ClGenericIODriver ();

    //## Destructor (specified)
      //## Operation: ~ClGenericIODriver%137094710; C++
      //	--- Error support ---
      //	-- {AddDecl: 358} region.unprotectedFunction [776..939]
      virtual ~ClGenericIODriver ();


    //## Other Operations (specified)
      //## Operation: GetLastErrorDescr%181559525; C++
      //	-- {AddDecl: 359} region.unprotectedFunction [940..1143]
      //	////////////////////////////   ERRORS   //////////////////////////////
      virtual const char* GetLastErrorDescr ();

      //## Operation: GetLastErrorCode%2108973108; C++
      //	-- {AddDecl: 10} module.vulnerableDeclarations [1467..1596]
      //	-- {AddDecl: 360} region.unprotectedFunction [1144..1229]
      virtual const m4uint32_t GetLastErrorCode ();

      //## Operation: Write%-1355858342; C++
      //	-- {AddDecl: 362} region.unprotectedFunction [1732..1890]
      //	////////////////////////////   WRITE   //////////////////////////////
      virtual m4return_t Write (m4uint32_t ai_iNumber);

      //## Operation: Write%-1152310899; C++
      //	-- {AddDecl: 363} region.unprotectedFunction [1891..1975]
      virtual m4return_t Write (m4int32_t ai_iNumber);

      //## Operation: Write%-16723042; C++
      //	-- {AddDecl: 364} region.unprotectedFunction [1976..2061]
      virtual m4return_t Write (m4uint16_t ai_iNumber);

      //## Operation: Write%-1089248297; C++
      //	-- {AddDecl: 365} region.unprotectedFunction [2062..2146]
      virtual m4return_t Write (m4int16_t ai_iNumber);

      //## Operation: Write%-101039236; C++
      //	-- {AddDecl: 366} region.unprotectedFunction [2147..2229]
      virtual m4return_t Write (m4uint8_t ai_cChar);

      //## Operation: Write%1698443656; C++
      //	-- {AddDecl: 367} region.unprotectedFunction [2230..2311]
      virtual m4return_t Write (m4int8_t ai_cChar);

      //## Operation: Write%1558274491; C++
      //	-- {AddDecl: 368} region.unprotectedFunction [2312..2396]
      virtual m4return_t Write (m4float_t ai_fNumber);

      //## Operation: Write%1821767255; C++
      //	-- {AddDecl: 369} region.unprotectedFunction [2397..2482]
      virtual m4return_t Write (m4double_t ai_dNumber);

      //## Operation: Write%924562132; C++
      //	Vectors of fixed size
      //	-- {AddDecl: 370} region.unprotectedFunction [2483..2567]
      virtual m4return_t Write (m4bool_t ai_iBoolean);

      //## Operation: Write%-1836580144; C++
      //	-- {AddDecl: 371} region.unprotectedFunction [2568..2706]
      virtual m4return_t Write (void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: Write%-456901579; C++
      //	Asciiz strings
      //	-- {AddDecl: 373} region.unprotectedFunction [2860..2999]
      virtual m4return_t Write (void* ai_pPtr, m4uint32_t ai_iSize);

      //## Operation: Write%808989820; C++
      //	-- {AddDecl: 374} region.unprotectedFunction [3000..3084]
      virtual m4return_t Write (m4pchar_t ai_pString);

      //## Operation: Write%1472642975; C++
      //	Files...
      //	This method stores the data of a file in the stream
      //	-- {AddDecl: 375} region.unprotectedFunction [3085..3175]
      virtual m4return_t Write (m4pcchar_t ai_pConstString);

      //## Operation: WritePrivate%-943504583; C++
      //	A buffer of a known size
      //	-- {AddDecl: 372} region.unprotectedFunction [2707..2859]
      virtual m4return_t WritePrivate (void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: WriteFromFile%1907835107; C++
      //	--- Read ---
      //	-- {AddDecl: 385} region.unprotectedFunction [5118..5213]
      virtual m4return_t WriteFromFile (m4pcchar_t ai_pFileName);

      //## Operation: Read%-863581831; C++
      //	-- {AddDecl: 386} region.unprotectedFunction [5214..5371]
      //	////////////////////////////   READ   //////////////////////////////
      virtual m4return_t Read (m4uint32_t& ai_iNumber);

      //## Operation: Read%74911768; C++
      //	-- {AddDecl: 387} region.unprotectedFunction [5372..5456]
      virtual m4return_t Read (m4int32_t& ai_iNumber);

      //## Operation: Read%-142653055; C++
      //	-- {AddDecl: 388} region.unprotectedFunction [5457..5542]
      virtual m4return_t Read (m4uint16_t& ai_iNumber);

      //## Operation: Read%-480822432; C++
      //	-- {AddDecl: 389} region.unprotectedFunction [5543..5627]
      virtual m4return_t Read (m4int16_t& ai_iNumber);

      //## Operation: Read%1846969027; C++
      //	-- {AddDecl: 390} region.unprotectedFunction [5628..5710]
      virtual m4return_t Read (m4uint8_t& ai_cChar);

      //## Operation: Read%1603485412; C++
      //	-- {AddDecl: 391} region.unprotectedFunction [5711..5792]
      virtual m4return_t Read (m4int8_t& ai_cChar);

      //## Operation: Read%814359385; C++
      //	-- {AddDecl: 392} region.unprotectedFunction [5793..5877]
      virtual m4return_t Read (m4float_t& ai_fNumber);

      //## Operation: Read%-235357315; C++
      //	-- {AddDecl: 393} region.unprotectedFunction [5878..5963]
      virtual m4return_t Read (m4double_t& ai_dNumber);

      //## Operation: Read%-193158775; C++
      //	Vectors
      //	-- {AddDecl: 394} region.unprotectedFunction [5964..6048]
      virtual m4return_t Read (m4bool_t& ai_iBoolean);

      //## Operation: Read%-877100280; C++
      //	-- {AddDecl: 395} region.unprotectedFunction [6049..6185]
      virtual m4return_t Read (void** ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: Read%-522227279; C++
      //	MaxSize on input, relasize on output
      //	-- {AddDecl: 397} region.unprotectedFunction [6337..6476]
      virtual m4return_t Read (void* ai_pPtr, m4uint32_t& ao_iSize);

      //## Operation: Read%-243869449; C++
      //	Size of the actual buffer
      //	-- {AddDecl: 400} region.unprotectedFunction [6648..6790]
      virtual m4return_t Read (m4pchar_t ao_pString, m4uint32_t ai_iMaxLen = 0);

      //## Operation: Read%280845154; C++
      //	Size of the actual buffer
      //	Files...
      //	This method creates a file from the data in the stream
      //	If FileName is a void string (""), the method returns the name generated
      //	inside,
      //	otherwise the name passed is used...
      //	-- {AddDecl: 401} region.unprotectedFunction [6791..6938]
      virtual m4return_t Read (m4pcchar_t ao_pConstString, m4uint32_t ai_iMaxLen = 0);

      //## Operation: ReadPrivate%-417107426; C++
      //	Buffer
      //	-- {AddDecl: 396} region.unprotectedFunction [6186..6336]
      virtual m4return_t ReadPrivate (void** ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: ReadSize%-430691484; C++
      //	First read the size, to allocate memory
      //	-- {AddDecl: 398} region.unprotectedFunction [6477..6564]
      virtual m4return_t ReadSize (m4uint32_t& ao_iSize);

      //## Operation: ReadBuffer%-797567939; C++
      //	Store the buffer
      //	Asciiz strings
      //	-- {AddDecl: 399} region.unprotectedFunction [6565..6647]
      virtual m4return_t ReadBuffer (void* ai_pPtr);

      //## Operation: ReadToFile%1479812601; C++
      //	--- Sizes ---
      //	Methods used to know the real number of bytes used to write the specified
      //	type
      //	(can be used for some operations, such as calculating the total size of a
      //	serialized
      //	object, group of objects or data structures). It also constructs the address
      //	to offset
      //	relation, in the cases when it is needed (depends on implementation).
      //	Basic (simple) types
      //	-- {AddDecl: 411} region.unprotectedFunction [8854..8945]
      virtual m4return_t ReadToFile (m4pchar_t ao_pFileName);

      //## Operation: SizeOf%-508808661; C++
      //	-- {AddDecl: 412} region.unprotectedFunction [8946..9097]
      //	////////////////////////////   SIZEOF   //////////////////////////////
      virtual m4uint32_t SizeOf (m4uint32_t ai_iNumber);

      //## Operation: SizeOf%-1588301982; C++
      //	-- {AddDecl: 413} region.unprotectedFunction [9098..9174]
      virtual m4uint32_t SizeOf (m4int32_t ai_iNumber);

      //## Operation: SizeOf%-544320935; C++
      //	-- {AddDecl: 414} region.unprotectedFunction [9175..9252]
      virtual m4uint32_t SizeOf (m4uint16_t ai_iNumber);

      //## Operation: SizeOf%-1399257039; C++
      //	-- {AddDecl: 415} region.unprotectedFunction [9253..9329]
      virtual m4uint32_t SizeOf (m4int16_t ai_iNumber);

      //## Operation: SizeOf%-172172140; C++
      //	-- {AddDecl: 416} region.unprotectedFunction [9330..9404]
      virtual m4uint32_t SizeOf (m4uint8_t ai_cChar);

      //## Operation: SizeOf%903564943; C++
      //	-- {AddDecl: 417} region.unprotectedFunction [9405..9478]
      virtual m4uint32_t SizeOf (m4int8_t ai_cChar);

      //## Operation: SizeOf%1155894597; C++
      //	-- {AddDecl: 418} region.unprotectedFunction [9479..9555]
      virtual m4uint32_t SizeOf (m4float_t ai_fNumber);

      //## Operation: SizeOf%653334044; C++
      //	-- {AddDecl: 419} region.unprotectedFunction [9556..9633]
      virtual m4uint32_t SizeOf (m4double_t ai_dNumber);

      //## Operation: SizeOf%-1434413233; C++
      //	A fixed number of a simple type
      //	-- {AddDecl: 420} region.unprotectedFunction [9634..9710]
      virtual m4uint32_t SizeOf (m4bool_t ai_iBoolean);

      //## Operation: SizeOf%-1879628015; C++
      //	-- {AddDecl: 421} region.unprotectedFunction [9711..9846]
      virtual m4uint32_t SizeOf (m4uint32_t ai_iNumber, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%-696095098; C++
      //	-- {AddDecl: 422} region.unprotectedFunction [9847..9981]
      virtual m4uint32_t SizeOf (m4int32_t ai_iNumber, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%-509598442; C++
      //	-- {AddDecl: 423} region.unprotectedFunction [9982..10117]
      virtual m4uint32_t SizeOf (m4uint16_t ai_iNumber, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%-1175665006; C++
      //	-- {AddDecl: 424} region.unprotectedFunction [10118..10252]
      virtual m4uint32_t SizeOf (m4int16_t ai_iNumber, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%660741921; C++
      //	-- {AddDecl: 425} region.unprotectedFunction [10253..10385]
      virtual m4uint32_t SizeOf (m4uint8_t ai_cChar, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%1822153809; C++
      //	-- {AddDecl: 426} region.unprotectedFunction [10386..10517]
      virtual m4uint32_t SizeOf (m4int8_t ai_cChar, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%-1455118132; C++
      //	-- {AddDecl: 427} region.unprotectedFunction [10518..10652]
      virtual m4uint32_t SizeOf (m4float_t ai_fNumber, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%1818636905; C++
      //	Any pointer
      //	-- {AddDecl: 428} region.unprotectedFunction [10653..10788]
      virtual m4uint32_t SizeOf (m4double_t ai_dNumber, m4uint32_t ai_iNumb);

      //## Operation: SizeOf%2072738788; C++
      //	Array of pointers to objects wich cant be multi-pointed
      //	-- {AddDecl: 429} region.unprotectedFunction [10789..10920]
      virtual m4uint32_t SizeOf (void* ai_pPtr, m4bool_t* ao_pExist = 0);

      //## Operation: SizeOf%101527962; C++
      //	Ascciz
      //	-- {AddDecl: 431} region.unprotectedFunction [11005..11137]
      virtual m4uint32_t SizeOf (void* ai_pPtr, m4uint32_t ai_iSize);

      //## Operation: SizeOf%248480791; C++
      //	-- {AddDecl: 432} region.unprotectedFunction [11138..11215]
      virtual m4uint32_t SizeOf (m4pchar_t ai_pString);

      //## Operation: SizeOf%-382869546; C++
      //	Accumulated size of all the previous calls
      //	-- {AddDecl: 433} region.unprotectedFunction [11216..11299]
      virtual m4uint32_t SizeOf (m4pcchar_t ai_pConstString);

      //## Operation: SizeOfArrayPtr%-835836625; C++
      //	Buffer of specific size (symulating write)
      //	-- {AddDecl: 430} region.unprotectedFunction [10921..11004]
      virtual m4uint32_t SizeOfArrayPtr (m4uint32_t ai_iNumb);

      //## Operation: TotalSize%459220464; C++
      //	Resets the total size for a new calc
      //	-- {AddDecl: 434} region.unprotectedFunction [11300..11363]
      virtual m4uint32_t TotalSize (void );

      //## Operation: Reset%-1079325739; C++
      //	--- Other ---
      //	-- {AddDecl: 435} region.unprotectedFunction [11364..11410]
      virtual void Reset (void );

      //## Operation: Eof%-829403883; C++
      //	-- {AddDecl: 436} region.unprotectedFunction [11411..11546]
      //	////////////////////////////   OTHER   //////////////////////////////
      virtual m4bool_t Eof (void );

      //## Operation: Flush%-454387608; C++
      //	--- Status ---
      //	Status of last operation...
      //	-- {AddDecl: 437} region.unprotectedFunction [11547..11610]
      virtual m4bool_t Flush (void );

      //## Operation: Status%2054196245; C++
      //	-- {AddDecl: 12} region.unprotectedFunction [10339..10406]
      //	-- {AddDecl: 438} region.unprotectedFunction [11611..11745]
      //	////////////////////////////   STATUS   //////////////////////////////
      virtual m4uint32_t Status (void );

      //## Operation: HasToSwap%1406317973; C++
      virtual m4bool_t HasToSwap (void ) const
      {
        //## begin ClGenericIODriver::HasToSwap%1406317973.body preserve=yes
 return( M4_FALSE ) ; 
        //## end ClGenericIODriver::HasToSwap%1406317973.body
      }

      //## Operation: StoreAddress%675300645; C++
      //	-- {AddDecl: 439} region.unprotectedFunction [11746..11906]
      //	////////////////////////////   SPECIAL   //////////////////////////////
      virtual m4return_t StoreAddress (void* ai_pPtr);

      //## Operation: StoreAddress%-2089641674; C++
      //	-- {AddDecl: 441} region.unprotectedFunction [11999..12154]
      virtual m4return_t StoreAddress (void* ai_pPtr, m4uint32_t ai_iOffset);

      //## Operation: StoreAddressPrivate%-118756418; C++
      //	-- {AddDecl: 440} region.unprotectedFunction [11907..11998]
      virtual m4return_t StoreAddressPrivate (void* ai_pPtr);

      //## Operation: StoreAddressPrivate%-617548594; C++
      //	-- {AddDecl: 442} region.unprotectedFunction [12155..12324]
      virtual m4return_t StoreAddressPrivate (void* ai_pPtr, m4uint32_t ai_iOffset);

      //## Operation: GetActualOffset%1582231088; C++
      //	-- {AddDecl: 443} region.unprotectedFunction [12325..12394]
      virtual m4uint32_t GetActualOffset (void );

      //## Operation: GetTempBuffer%-791740797; C++
      //	-- {AddDecl: 444} region.unprotectedFunction [12395..12721]
      virtual void* GetTempBuffer (const m4uint32_t ai_iSize);

      //## Operation: GetArchitect%752440951; C++
      //	-- {AddDecl: 445} region.unprotectedFunction [12722..12788]
      virtual m4uint16_t GetArchitect (void );

      //## Operation: ResetPrivateInfo%361153502; C++
      //	-- {AddDecl: 446} region.unprotectedFunction [12789..12868]
      virtual m4return_t ResetPrivateInfo (void );

    // Additional Public Declarations
      //## begin ClGenericIODriver%3911AA24022F.public preserve=yes
      //## end ClGenericIODriver%3911AA24022F.public

  protected:

    //## Other Operations (specified)
      //## Operation: SetLastErrorDescr%1729258322; C++
      //	-- {AddDecl: 361} region.unprotectedFunction [1230..1731]
      virtual void SetLastErrorDescr (const char* ai_pcDescr);

    // Data Members for Class Attributes

      //## Attribute: m_iLastErrorCode%39C510D903BC
      //## begin ClGenericIODriver::m_iLastErrorCode%39C510D903BC.attr preserve=no  protected: m4uint32_t {UA} GEN_ERR_OK
      m4uint32_t m_iLastErrorCode;
      //## end ClGenericIODriver::m_iLastErrorCode%39C510D903BC.attr

      //## Attribute: m_pcLastErrorDescr%39C511920119
      //## begin ClGenericIODriver::m_pcLastErrorDescr%39C511920119.attr preserve=no  protected: m4char_t* {UA} 0
      m4char_t* m_pcLastErrorDescr;
      //## end ClGenericIODriver::m_pcLastErrorDescr%39C511920119.attr

    // Additional Protected Declarations
      //## begin ClGenericIODriver%3911AA24022F.protected preserve=yes
      //## end ClGenericIODriver%3911AA24022F.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pTempBuffer%3911AA2603DF
      //## begin ClGenericIODriver::m_pTempBuffer%3911AA2603DF.attr preserve=no  private: void* {VA} 0
      void* m_pTempBuffer;
      //## end ClGenericIODriver::m_pTempBuffer%3911AA2603DF.attr

      //## Attribute: m_iTempBufferSize%39C51000005C
      //## begin ClGenericIODriver::m_iTempBufferSize%39C51000005C.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_iTempBufferSize;
      //## end ClGenericIODriver::m_iTempBufferSize%39C51000005C.attr

    // Additional Private Declarations
      //## begin ClGenericIODriver%3911AA24022F.private preserve=yes
      //## end ClGenericIODriver%3911AA24022F.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClGenericIODriver%3911AA24022F.implementation preserve=yes
      //## end ClGenericIODriver%3911AA24022F.implementation

};

//## begin ClGenericIODriver%3911AA24022F.postscript preserve=yes
//## end ClGenericIODriver%3911AA24022F.postscript

//## begin module%3911AA240230.epilog preserve=yes
//## end module%3911AA240230.epilog


#endif
