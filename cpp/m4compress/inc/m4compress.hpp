//## begin module%373AE1580219.cm preserve=no
//## end module%373AE1580219.cm

//## begin module%373AE1580219.cp preserve=no
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
//## end module%373AE1580219.cp

//## Module: m4compress%373AE1580219; Package specification
//## Subsystem: M4Compress::inc%37C2BE980107
//## Source file: e:\source\m4compress\version\inc\m4compress.hpp

#ifndef m4compress_h
#define m4compress_h 1

//## begin module%373AE1580219.additionalIncludes preserve=no
//## end module%373AE1580219.additionalIncludes

//## begin module%373AE1580219.includes preserve=yes
#include <m4zlib.hpp>
#include <m4compress_dll.hpp>
#include <m4trace.hpp>
#include <m4types.hpp>
//## end module%373AE1580219.includes

//## begin module%373AE1580219.declarations preserve=no
//## end module%373AE1580219.declarations

//## begin module%373AE1580219.additionalDeclarations preserve=yes

m4return_t M4_DECL_M4COMPRESS M4DecompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile, m4pchar_t ai_Mode = "Z");
m4return_t M4_DECL_M4COMPRESS M4CompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile, m4pchar_t ai_Mode = "Z");

#ifndef M4_COMPRESSION_TYPE_NULL

#   define M4_COMPRESSION_TYPE_NULL  1
#   define M4_COMPRESSION_TYPE_CLASSIC  2
#   define M4_COMPRESSION_TYPE_ZDELTA  3

// #	define M4_COMPRESSION_LEVEL_LOW 1
// #	define M4_COMPRESSION_LEVEL_MEDIUM 2
// #	define M4_COMPRESSION_LEVEL_HIGH 3
#   define M4_COMPRESSION_CLASSIC_DEFAULT_LEVEL  3

#endif //M4_COMPRESSION_TYPE_NULL

#ifdef USE_PKWARE
typedef struct PassedParam
{
   char *pSource;                   /* Pointer to source buffer           */
   char *pDestination;              /* Pointer to destination buffer      */
   unsigned long SourceOffset;      /* Offset into the source buffer      */
   unsigned long DestinationOffset; /* Offset into the destination buffer */
   unsigned long CompressedSize;    /* Need this for extracting!          */
   unsigned long UnCompressedSize;  /* Size of uncompressed data file     */
   unsigned long BufferSize;
} PARAM;


/*
** BufferSize defines the maximum size allowed for output of compressed
** data from implode(), or uncompressed data from explode().  Notice that
** if you are compressing files that compress to a size greater than 
** BufferSize, an error message will result.  You can modify the value of
** BufferSize if you need a bigger buffer.  If you know your maximum file 
** size, you can adjust BUFFERSIZE.  This will allow for larger, or smaller
** maximum values of BufferSize.  
*/

/* Routine to read uncompressed data.  Used only by implode().
** This routine reads the data that is to be compressed.
*/


unsigned int 
ReadUnCompressed(char *buff, unsigned int *size, void *Param);

/* Routine to read compressed data.  Used only by explode(). 
** This routine reads the compressed data that is to be uncompressed.
*/

unsigned int
ReadCompressed(char *buff, unsigned int *size, void *Param);

/* Routime to write compressed data.  Used only by implode(). 
** This routine writes the compressed data to a memory buffer.
*/

void 
WriteCompressed(char *buff, unsigned int *size, void *Param);


/* Routine to write uncompressed data. Used only by explode().
** This routine writes the uncompressed data to a memory buffer.
*/

void 
WriteUnCompressed(char *buff, unsigned int *size, void *Param);
#endif // USE_PKWARE
//## end module%373AE1580219.additionalDeclarations


//## begin ClCompress%373AA67F0393.preface preserve=yes
//## end ClCompress%373AA67F0393.preface

//## Class: ClCompress%373AA67F0393
//## Category: M4Compress%373AA654017F
//## Subsystem: M4Compress::inc%37C2BE980107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%373FDDFC014E;m4return_t { -> }

class M4_DECL_M4COMPRESS ClCompress
{
  //## begin ClCompress%373AA67F0393.initialDeclarations preserve=yes
  //## end ClCompress%373AA67F0393.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCompress%926606772
      ClCompress ();

    //## Destructor (specified)
      //## Operation: ~ClCompress%926606773
      virtual ~ClCompress ();


    //## Other Operations (specified)
      //## Operation: Compress%926606774; C++
      virtual m4return_t Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Decompress%926606775; C++
      virtual m4return_t Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Init%926606776; C++
      virtual m4return_t Init (m4int_t ai_iCompressionLevel);

      //## Operation: SetCompressionLevel%926685383; C++
      virtual m4return_t SetCompressionLevel (m4int_t ai_iCompresseionLevel);

      //## Operation: GetCompressionLevel%926685384; C++
      m4return_t GetCompressionLevel (m4int_t &ao_iCompresseionLevel);

      //## Operation: Free%926948284; C++
      virtual m4return_t Free ();

      //## Operation: CompressFile%935144291
      virtual m4return_t CompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile);

      //## Operation: DecompressFile%935144292
      virtual m4return_t DecompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile);

    // Additional Public Declarations
      //## begin ClCompress%373AA67F0393.public preserve=yes
      //## end ClCompress%373AA67F0393.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_pcName%373AA7120359
      //## begin ClCompress::m_pcName%373AA7120359.attr preserve=no  protected: m4pchar_t {UA} NULL
      m4pchar_t m_pcName;
      //## end ClCompress::m_pcName%373AA7120359.attr

      //## Attribute: m_pcVersion%373AA72900EF
      //## begin ClCompress::m_pcVersion%373AA72900EF.attr preserve=no  protected: m4pchar_t {UA} NULL
      m4pchar_t m_pcVersion;
      //## end ClCompress::m_pcVersion%373AA72900EF.attr

      //## Attribute: m_iCompressionLevel%373C1887006A
      //## begin ClCompress::m_iCompressionLevel%373C1887006A.attr preserve=no  protected: m4int_t {UA} 0
      m4int_t m_iCompressionLevel;
      //## end ClCompress::m_iCompressionLevel%373C1887006A.attr

    // Additional Protected Declarations
      //## begin ClCompress%373AA67F0393.protected preserve=yes
      //## end ClCompress%373AA67F0393.protected

  private:
    // Additional Private Declarations
      //## begin ClCompress%373AA67F0393.private preserve=yes
      //## end ClCompress%373AA67F0393.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCompress%373AA67F0393.implementation preserve=yes
      //## end ClCompress%373AA67F0393.implementation

};

//## begin ClCompress%373AA67F0393.postscript preserve=yes
//## end ClCompress%373AA67F0393.postscript

//## begin ClCompressFactory%373AAC6E01E9.preface preserve=yes
//## end ClCompressFactory%373AAC6E01E9.preface

//## Class: ClCompressFactory%373AAC6E01E9
//	Es una factoria que nos va a proporcionar un objeto compresor YA OPERATIVO
//## Category: M4Compress%373AA654017F
//## Subsystem: M4Compress::inc%37C2BE980107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%373AAF130016;ClCompress { -> }

class M4_DECL_M4COMPRESS ClCompressFactory
{
  //## begin ClCompressFactory%373AAC6E01E9.initialDeclarations preserve=yes
  //## end ClCompressFactory%373AAC6E01E9.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClCompressFactory%926606779
      ClCompressFactory ();

    //## Destructor (specified)
      //## Operation: ~ClCompressFactory%926606780
      ~ClCompressFactory ();


    //## Other Operations (specified)
      //## Operation: GetCompress%926606781; C++
      ClCompress * GetCompress (m4pchar_t ai_pcName = "Z", m4int_t ai_iCompressionLevel = 6);

    // Additional Public Declarations
      //## begin ClCompressFactory%373AAC6E01E9.public preserve=yes
      //## end ClCompressFactory%373AAC6E01E9.public

  protected:
    // Additional Protected Declarations
      //## begin ClCompressFactory%373AAC6E01E9.protected preserve=yes
      //## end ClCompressFactory%373AAC6E01E9.protected

  private:
    // Additional Private Declarations
      //## begin ClCompressFactory%373AAC6E01E9.private preserve=yes
      //## end ClCompressFactory%373AAC6E01E9.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClCompressFactory%373AAC6E01E9.implementation preserve=yes
      //## end ClCompressFactory%373AAC6E01E9.implementation

};

//## begin ClCompressFactory%373AAC6E01E9.postscript preserve=yes
//## end ClCompressFactory%373AAC6E01E9.postscript

//## begin ClPKWareCompress%373AAEBF01C3.preface preserve=yes
#ifdef USE_PKWARE
//## end ClPKWareCompress%373AAEBF01C3.preface

//## Class: ClPKWareCompress%373AAEBF01C3
//## Category: M4Compress%373AA654017F
//## Subsystem: M4Compress::inc%37C2BE980107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClPKWareCompress : public ClCompress  //## Inherits: <unnamed>%373AAED303D5
{
  //## begin ClPKWareCompress%373AAEBF01C3.initialDeclarations preserve=yes
  //## end ClPKWareCompress%373AAEBF01C3.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClPKWareCompress%926929990
      ClPKWareCompress ();

    //## Destructor (specified)
      //## Operation: ~ClPKWareCompress%926929991
      ~ClPKWareCompress ();


    //## Other Operations (specified)
      //## Operation: Compress%926929992; C++
      virtual m4return_t Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Decompress%926929993; C++
      virtual m4return_t Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Init%926929994; C++
      virtual m4return_t Init (m4int_t ai_iCompressionLevel);

      //## Operation: SetCompressionLevel%926948283; C++
      m4return_t SetCompressionLevel (m4int_t ai_iCompressionLevel);

      //## Operation: Free%926948285; C++
      virtual m4return_t Free ();

    // Additional Public Declarations
      //## begin ClPKWareCompress%373AAEBF01C3.public preserve=yes
      //## end ClPKWareCompress%373AAEBF01C3.public

  protected:
    // Additional Protected Declarations
      //## begin ClPKWareCompress%373AAEBF01C3.protected preserve=yes
      //## end ClPKWareCompress%373AAEBF01C3.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pcWorkBuffer%37401BA70090
      //## begin ClPKWareCompress::m_pcWorkBuffer%37401BA70090.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pcWorkBuffer;
      //## end ClPKWareCompress::m_pcWorkBuffer%37401BA70090.attr

      //## Attribute: m_uiType%37401CDE028C
      //## begin ClPKWareCompress::m_uiType%37401CDE028C.attr preserve=no  private: unsigned int {UA} CMP_BINARY
      unsigned int m_uiType;
      //## end ClPKWareCompress::m_uiType%37401CDE028C.attr

      //## Attribute: m_piDicSize%37401DC4028D
      //## begin ClPKWareCompress::m_piDicSize%37401DC4028D.attr preserve=no  private: unsigned int * {UA} 
      unsigned int *m_piDicSize;
      //## end ClPKWareCompress::m_piDicSize%37401DC4028D.attr

      //## Attribute: m_pcInternalBuffer%374034DE0196
      //## begin ClPKWareCompress::m_pcInternalBuffer%374034DE0196.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pcInternalBuffer;
      //## end ClPKWareCompress::m_pcInternalBuffer%374034DE0196.attr

      //## Attribute: m_uiInternalBufferSize%3740351303AF
      //## begin ClPKWareCompress::m_uiInternalBufferSize%3740351303AF.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uiInternalBufferSize;
      //## end ClPKWareCompress::m_uiInternalBufferSize%3740351303AF.attr

    // Additional Private Declarations
      //## begin ClPKWareCompress%373AAEBF01C3.private preserve=yes
	  PARAM m_Param;
      //## end ClPKWareCompress%373AAEBF01C3.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClPKWareCompress%373AAEBF01C3.implementation preserve=yes
      //## end ClPKWareCompress%373AAEBF01C3.implementation

};

//## begin ClPKWareCompress%373AAEBF01C3.postscript preserve=yes
#endif // USE_PKWARE
//## end ClPKWareCompress%373AAEBF01C3.postscript

//## begin ClZlibCompress%373AAEE70095.preface preserve=yes
//## end ClZlibCompress%373AAEE70095.preface

//## Class: ClZlibCompress%373AAEE70095
//## Category: M4Compress%373AA654017F
//## Subsystem: M4Compress::inc%37C2BE980107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClZlibCompress : public ClCompress  //## Inherits: <unnamed>%373AAEEA0071
{
  //## begin ClZlibCompress%373AAEE70095.initialDeclarations preserve=yes
  //## end ClZlibCompress%373AAEE70095.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClZlibCompress%926685388
      ClZlibCompress ();

    //## Destructor (specified)
      //## Operation: ~ClZlibCompress%926685389
      ~ClZlibCompress ();


    //## Other Operations (specified)
      //## Operation: Compress%926685385; C++
      virtual m4return_t Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Decompress%926685386; C++
      virtual m4return_t Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Init%926685387; C++
      virtual m4return_t Init (m4int_t ai_iCompressionLevel);

      //## Operation: Free%927022005
      m4return_t Free ();

      //## Operation: CompressFile%935144293
      m4return_t CompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile);

      //## Operation: DecompressFile%935144294
      m4return_t DecompressFile (m4pchar_t ai_pSourceFile, m4pchar_t ai_pDestinationFile);

    // Additional Public Declarations
      //## begin ClZlibCompress%373AAEE70095.public preserve=yes
      //## end ClZlibCompress%373AAEE70095.public

  protected:
    // Additional Protected Declarations
      //## begin ClZlibCompress%373AAEE70095.protected preserve=yes
      //## end ClZlibCompress%373AAEE70095.protected

  private:
    // Additional Private Declarations
      //## begin ClZlibCompress%373AAEE70095.private preserve=yes
	  M4Zlib * m_poCompressor;
      //## end ClZlibCompress%373AAEE70095.private
  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClZlibCompress%373AAEE70095.implementation preserve=yes
      //## end ClZlibCompress%373AAEE70095.implementation

};

//## begin ClZlibCompress%373AAEE70095.postscript preserve=yes
//## end ClZlibCompress%373AAEE70095.postscript

//## begin ClNULLCompress%373AAF2A0272.preface preserve=yes
//## end ClNULLCompress%373AAF2A0272.preface

//## Class: ClNULLCompress%373AAF2A0272
//## Category: M4Compress%373AA654017F
//## Subsystem: M4Compress::inc%37C2BE980107
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClNULLCompress : public ClCompress  //## Inherits: <unnamed>%373AAF37025C
{
  //## begin ClNULLCompress%373AAF2A0272.initialDeclarations preserve=yes
  //## end ClNULLCompress%373AAF2A0272.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClNULLCompress%926685390
      ClNULLCompress ();

    //## Destructor (specified)
      //## Operation: ~ClNULLCompress%926685391
      ~ClNULLCompress ();


    //## Other Operations (specified)
      //## Operation: Compress%926670135; C++
      virtual m4return_t Compress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Decompress%926670136; C++
      virtual m4return_t Decompress (m4pchar_t ai_pcBufferIn, m4uint32_t ai_uiBufferInSize, m4pchar_t &ao_pcBufferOut, m4uint32_t &ao_uiBufferOutSize);

      //## Operation: Init%926670137; C++
      virtual m4return_t Init (m4int_t ai_iCompressionLevel);

    // Additional Public Declarations
      //## begin ClNULLCompress%373AAF2A0272.public preserve=yes
      //## end ClNULLCompress%373AAF2A0272.public

  protected:
    // Additional Protected Declarations
      //## begin ClNULLCompress%373AAF2A0272.protected preserve=yes
      //## end ClNULLCompress%373AAF2A0272.protected

  private:
    // Additional Private Declarations
      //## begin ClNULLCompress%373AAF2A0272.private preserve=yes
      //## end ClNULLCompress%373AAF2A0272.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClNULLCompress%373AAF2A0272.implementation preserve=yes
      //## end ClNULLCompress%373AAF2A0272.implementation

};

//## begin ClNULLCompress%373AAF2A0272.postscript preserve=yes
//## end ClNULLCompress%373AAF2A0272.postscript

// Class ClCompress 

// Class ClCompressFactory 

// Class ClPKWareCompress 

// Class ClZlibCompress 

// Class ClNULLCompress 

//## begin module%373AE1580219.epilog preserve=yes
//## end module%373AE1580219.epilog


#endif
