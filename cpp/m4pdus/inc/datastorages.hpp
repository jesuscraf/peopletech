//## begin module%344E3133012D.cm preserve=no
//## end module%344E3133012D.cm

//## begin module%344E3133012D.cp preserve=no
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
//## end module%344E3133012D.cp

//## Module: DataStorages%344E3133012D; Package specification
//## Subsystem: M4PDU::inc%3794593C0398
//## Source file: e:\mybuild\m4pdus\inc\datastorages.hpp

#ifndef datastorages_h
#define datastorages_h 1

//## begin module%344E3133012D.additionalIncludes preserve=no
//## end module%344E3133012D.additionalIncludes

//## begin module%344E3133012D.includes preserve=yes
#define M4PDUS_FILEUNIT_UNDEFINED_SIZE  -1
#define M4PDUS_FILEUNIT_UNDEFINED_FILE  -2

#include "m4stl.hpp"
#include "m4pdus_dll.hpp"
//## end module%344E3133012D.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>

class ClDataUnit;
class ClBlockFormat;

//## begin module%344E3133012D.declarations preserve=yes
#ifdef _KCC
using std::deque;
#endif
//## end module%344E3133012D.declarations

//## begin module%344E3133012D.additionalDeclarations preserve=yes

const m4bool_t	M4_DECRYPTED = M4_FALSE;
const m4bool_t	M4_ENCRYPTED = M4_TRUE;

#define M4_DATA_UNIT 0
#define M4_FILE_UNIT 1
#define M4_CHECK_UNIT 2

//## end module%344E3133012D.additionalDeclarations


//## begin ClFormatQueue%34ED4BB701B7.preface preserve=yes
//## end ClFormatQueue%34ED4BB701B7.preface

//## Class: ClFormatQueue%34ED4BB701B7
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34ED674E01A9;m4return_t { -> }
//## Uses: <unnamed>%34F14FE20037;ClBlockFormat { -> F}

class ClFormatQueue 
{
  //## begin ClFormatQueue%34ED4BB701B7.initialDeclarations preserve=yes
  //## end ClFormatQueue%34ED4BB701B7.initialDeclarations

  public:
    //## Class: iterator%34ED4C110351
    //## Category: M4PDU%34BA420E0375
    //## Subsystem: M4PDU::inc%3794593C0398
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef m4uint32_t iterator;

    //## Constructors (specified)
      //## Operation: ClFormatQueue%887968732
      ClFormatQueue ();

    //## Destructor (specified)
      //## Operation: ~ClFormatQueue%887968733
      ~ClFormatQueue ();


    //## Other Operations (specified)
      //## Operation: push_back%887968726
      m4return_t push_back (ClBlockFormat *ai_data);

      //## Operation: erase%887968730
      m4return_t erase (ClFormatQueue::iterator ai_ulPos);

      //## Operation: begin%888225518
      ClFormatQueue::iterator begin ();

      //## Operation: begin%887968728
      ClFormatQueue::iterator begin (ClBlockFormat * &ao_pData);

      //## Operation: end%888225519
      ClFormatQueue::iterator end ();

      //## Operation: end%887968729
      ClFormatQueue::iterator end (ClBlockFormat * &ao_pData);

      //## Operation: resize%887968731
      m4return_t resize ();

      //## Operation: get%888225514
      m4return_t get (ClFormatQueue::iterator ao_Iterator, ClBlockFormat * &ao_pData);

      //## Operation: get_next%888225515
      m4return_t get_next (ClFormatQueue::iterator ao_Iterator, ClBlockFormat * &ao_pData);

      //## Operation: size%888225516
      m4uint32_t size ();

      //## Operation: empty%888225517
      m4bool_t empty ();

      //## Operation: clear%888685577
      void clear ();

  public:
    // Additional Public Declarations
      //## begin ClFormatQueue%34ED4BB701B7.public preserve=yes
      //## end ClFormatQueue%34ED4BB701B7.public

  protected:
    // Additional Protected Declarations
      //## begin ClFormatQueue%34ED4BB701B7.protected preserve=yes
      //## end ClFormatQueue%34ED4BB701B7.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_ArraypData%34ED5AFF02CB
      //## begin ClFormatQueue::m_ArraypData%34ED5AFF02CB.attr preserve=no  private: ClBlockFormat * * {UA} NULL
      ClBlockFormat * *m_ArraypData;
      //## end ClFormatQueue::m_ArraypData%34ED5AFF02CB.attr

      //## Attribute: m_ulTotalSize%34ED6370014C
      //## begin ClFormatQueue::m_ulTotalSize%34ED6370014C.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulTotalSize;
      //## end ClFormatQueue::m_ulTotalSize%34ED6370014C.attr

      //## Attribute: m_ulFilled%34ED637802DE
      //## begin ClFormatQueue::m_ulFilled%34ED637802DE.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulFilled;
      //## end ClFormatQueue::m_ulFilled%34ED637802DE.attr

    // Additional Private Declarations
      //## begin ClFormatQueue%34ED4BB701B7.private preserve=yes
      //## end ClFormatQueue%34ED4BB701B7.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFormatQueue%34ED4BB701B7.implementation preserve=yes
      //## end ClFormatQueue%34ED4BB701B7.implementation

};

//## begin ClFormatQueue%34ED4BB701B7.postscript preserve=yes
//## end ClFormatQueue%34ED4BB701B7.postscript

//## begin ClDataUnitInterface%35D85D2C012E.preface preserve=yes
//## end ClDataUnitInterface%35D85D2C012E.preface

//## Class: ClDataUnitInterface%35D85D2C012E
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClDataUnitInterface 
{
  //## begin ClDataUnitInterface%35D85D2C012E.initialDeclarations preserve=yes
  //## end ClDataUnitInterface%35D85D2C012E.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDataUnitInterface%903369740
      ClDataUnitInterface ();

      //## Operation: ClDataUnitInterface%903369741
      ClDataUnitInterface (m4pchar_t ai_pBuffer, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypt = M4_TRUE, m4uint32_t ai_ulOffset = 0);

    //## Destructor (specified)
      //## Operation: ~ClDataUnitInterface%903369742
      virtual ~ClDataUnitInterface ();


    //## Other Operations (specified)
      //## Operation: GetSize%903369743
      virtual m4uint32_t GetSize ();

      //## Operation: GetBuffer%903369744
      virtual m4pchar_t GetBuffer ();

      //## Operation: GetCryptState%903369745
      virtual m4bool_t GetCryptState ();

      //## Operation: Clear%903369746
      //	  Pone m_pBuffer =  NULL e inicializa el resto de los argumentos.
      virtual void Clear ();

      //## Operation: Dump%903369747
      virtual void Dump (m4pchar_t  &ao_strBuffer);

      //## Operation: SetSize%903369748; C++
      virtual m4return_t SetSize (m4uint32_t ai_uiSize);

      //## Operation: GetType%903536128
      virtual m4int16_t GetType ();

      //## Operation: GetSerialState%903603130
      m4bool_t GetSerialState ();

      //## Operation: SetSerialState%903603131
      void SetSerialState (m4bool_t ai_SerialState);

      //## Operation: SetCryptState%932487948; C++
      void SetCryptState (m4bool_t ai_bCrypt);

      //## Operation: InitializeSerialState%940592256
      virtual m4return_t InitializeSerialState ();

      //## Operation: SetBufferSize%970483019; C++
      m4return_t SetBufferSize (m4uint_t ai_uiBufferSize);

      //## Operation: GetChunkSize%970483022
      virtual m4uint32_t GetChunkSize ();

    // Additional Public Declarations
      //## begin ClDataUnitInterface%35D85D2C012E.public preserve=yes
      //## end ClDataUnitInterface%35D85D2C012E.public

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_ulSize%35D861E00338
      //	  Atributo que representa al tamaño del bloque que contiene  la clase.
      //## begin ClDataUnitInterface::m_ulSize%35D861E00338.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulSize;
      //## end ClDataUnitInterface::m_ulSize%35D861E00338.attr

      //## Attribute: m_pBuffer%35D861E100A4
      //## begin ClDataUnitInterface::m_pBuffer%35D861E100A4.attr preserve=no  protected: m4pchar_t {UA} NULL
      m4pchar_t m_pBuffer;
      //## end ClDataUnitInterface::m_pBuffer%35D861E100A4.attr

      //## Attribute: m_usEncrypt%35D861E101BD
      //## begin ClDataUnitInterface::m_usEncrypt%35D861E101BD.attr preserve=no  protected: m4bool_t {UA} M4_FALSE
      m4bool_t m_usEncrypt;
      //## end ClDataUnitInterface::m_usEncrypt%35D861E101BD.attr

      //## Attribute: m_usSerialState%35DBE283029B
      //## begin ClDataUnitInterface::m_usSerialState%35DBE283029B.attr preserve=no  protected: m4bool_t {UA} M4_TRUE
      m4bool_t m_usSerialState;
      //## end ClDataUnitInterface::m_usSerialState%35DBE283029B.attr

    // Additional Protected Declarations
      //## begin ClDataUnitInterface%35D85D2C012E.protected preserve=yes
      //## end ClDataUnitInterface%35D85D2C012E.protected

  private:
    // Additional Private Declarations
      //## begin ClDataUnitInterface%35D85D2C012E.private preserve=yes
      //## end ClDataUnitInterface%35D85D2C012E.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClDataUnitInterface%35D85D2C012E.implementation preserve=yes
      //## end ClDataUnitInterface%35D85D2C012E.implementation

};

//## begin ClDataUnitInterface%35D85D2C012E.postscript preserve=yes
//## end ClDataUnitInterface%35D85D2C012E.postscript

//## Class: ClDataUnit%344E067902B6
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClDataUnit : public ClDataUnitInterface  //## Inherits: <unnamed>%35D85DA202A0
{
  //## begin ClDataUnit%344E067902B6.initialDeclarations preserve=yes
public:


  //## end ClDataUnit%344E067902B6.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClDataUnit%885978338
      ClDataUnit ();

      //## Operation: ClDataUnit%877685667
      ClDataUnit (m4pchar_t ai_pBuffer, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypt = M4_TRUE, m4uint32_t ai_ulOffset = 0);

    //## Destructor (specified)
      //## Operation: ~ClDataUnit%878295962
      virtual ~ClDataUnit ();


    //## Other Operations (specified)
      //## Operation: GetSize%877685665
      m4uint32_t GetSize ();

      //## Operation: GetBuffer%877685666
      m4pchar_t GetBuffer ();

      //## Operation: GetCryptState%878377079
      m4bool_t GetCryptState ();

      //## Operation: Dump%878377101
      void Dump (m4pchar_t  &ao_strBuffer);

      //## Operation: SetSize%885978335; C++
      m4return_t SetSize (m4uint32_t ai_uiSize);

      //## Operation: GetType%903536123
      m4int16_t GetType ();

      //## Operation: InitializeSerialState%940592258
      m4return_t InitializeSerialState ();

  protected:
  private:
  private: //## implementation
};

//## Class: ClDataUnitQueue%344F7DA9001E
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%344F7E5902E8;ClDataUnit { -> F}
//## Uses: <unnamed>%35D860310003;ClDataUnitInterface { -> }

class M4_DECL_M4PDUS ClDataUnitQueue 
{
  public:
    //## Class: iterator%34FA6CB001D7
    //## Category: M4PDU%34BA420E0375
    //## Subsystem: M4PDU::inc%3794593C0398
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    typedef m4uint32_t iterator;

    //## Constructors (specified)
      //## Operation: ClDataUnitQueue%888826864
      ClDataUnitQueue ();

    //## Destructor (specified)
      //## Operation: ~ClDataUnitQueue%888826865
      ~ClDataUnitQueue ();


    //## Other Operations (specified)
      //## Operation: push_back%888826866
      m4return_t push_back (ClDataUnitInterface *ai_data);

      //## Operation: begin%888826868
      ClDataUnitQueue::iterator begin ();

      //## Operation: begin%888826869
      ClDataUnitQueue::iterator begin (ClDataUnitInterface * &ao_pData);

      //## Operation: end%888826870
      ClDataUnitQueue::iterator end ();

      //## Operation: end%888826871
      ClDataUnitQueue::iterator end (ClDataUnitInterface * &ao_pData);

      //## Operation: get%888826873
      m4return_t get (ClDataUnitQueue::iterator ao_Iterator, ClDataUnitInterface * &ao_pData);

      //## Operation: size%888826875
      m4uint32_t size ();

      //## Operation: empty%888826876
      //	Testea se esta vacia
      m4bool_t empty ();

      //## Operation: clear%888826877
      //	Deja vacia la DataUnitQueue
      void clear ();

      //## Operation: Delete%963315749; C++
      //	Borra una DataUnit
      m4return_t Delete (ClDataUnitQueue::iterator ai_ulPos);

  public:
  protected:
    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%3961E24F0148
      //## Role: ClDataUnitQueue::m_uiFirstDataUnit%3961E2500050
      //## begin ClDataUnitQueue::m_uiFirstDataUnit%3961E2500050.role preserve=no  protected: m4uint32_t { -> VHAN}
      m4uint32_t m_uiFirstDataUnit;
      //## end ClDataUnitQueue::m_uiFirstDataUnit%3961E2500050.role

      //## Association: M4PDU::<unnamed>%3961E2B70238
      //## Role: ClDataUnitQueue::m_uiLastDataUnit%3961E2B800C7
      //## begin ClDataUnitQueue::m_uiLastDataUnit%3961E2B800C7.role preserve=no  protected: m4uint32_t { -> VHAN}
      m4uint32_t m_uiLastDataUnit;
      //## end ClDataUnitQueue::m_uiLastDataUnit%3961E2B800C7.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_ArraypData%34FA548F0329
      //## begin ClDataUnitQueue::m_ArraypData%34FA548F0329.attr preserve=no  private: ClDataUnitInterface * * {UA} NULL
      ClDataUnitInterface * *m_ArraypData;
      //## end ClDataUnitQueue::m_ArraypData%34FA548F0329.attr

      //## Attribute: m_ulTotalSize%34FA548F032A
      //## begin ClDataUnitQueue::m_ulTotalSize%34FA548F032A.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulTotalSize;
      //## end ClDataUnitQueue::m_ulTotalSize%34FA548F032A.attr

      //## Attribute: m_ulFilled%34FA548F0333
      //## begin ClDataUnitQueue::m_ulFilled%34FA548F0333.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_ulFilled;
      //## end ClDataUnitQueue::m_ulFilled%34FA548F0333.attr

  private: //## implementation
};

//## Class: ClBaseStorage%344DE8C4013E
//	   Clase base que nos sirve para que hereden de ellas las clases especificas
//	de almacenamiento de datos.
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%344F26A60209;deque { -> }
//## Uses: <unnamed>%345B51970231;M4ClString { -> }
//## Uses: <unnamed>%39607E2C0299;ClFileManager { -> F}

class M4_DECL_M4PDUS ClBaseStorage 
{
  public:
    //## Constructors (specified)
      //## Operation: ClBaseStorage%877685670
      ClBaseStorage ();

    //## Destructor (specified)
      //## Operation: ~ClBaseStorage%878295961
      ~ClBaseStorage ();


    //## Other Operations (specified)
      //## Operation: Clear%878893910
      //	  Elimina todos los elementos que contenga el almacén.
      virtual void Clear ();

      //## Operation: AddFileUnit%903536124
      m4return_t AddFileUnit (m4pchar_t ai_FileName, m4bool_t ai_bEncrypted = M4_DECRYPTED);

      //## Operation: SetTempPath%903536125
      void SetTmpPath (m4pchar_t ai_pTempPath);

      //## Operation: GetTempPath%903536126
      m4pchar_t GetTmpPath ();

      //## Operation: SetDefragSize%903536134
      void SetDefragSize (m4uint32_t ai_DefragSize);

      //## Operation: GetDefragSize%903536135
      m4uint32_t GetDefragSize ();

      //## Operation: CheckDataStorage%903715592
      m4return_t CheckDataStorage (m4pchar_t ai_pBlock, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypted = M4_FALSE);

      //## Operation: CheckForFileUnit%903961923; C++
      m4bool_t CheckForFileUnit (m4pchar_t ai_pBlock, m4uint32_t ai_blockSize);

      //## Operation: isDataUnitInterfaceQueueOwner%945078896
      m4bool_t isDataUnitInterfaceQueueOwner ();

      //## Operation: SetDataUnitInterfaceQueueOwner%945078897
      void SetDataUnitInterfaceQueueOwner (m4bool_t ai_bOwner);

      //## Operation: GetDataUnitInterfaceQueue%945078898
      ClDataUnitQueue * GetDataUnitInterfaceQueue ();

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%378C53450185
      //## Role: ClBaseStorage::m_poDataUnitQueue%378C53460104
      //## begin ClBaseStorage::m_poDataUnitQueue%378C53460104.role preserve=no  public: ClDataUnitQueue { -> 1RHAN}
      ClDataUnitQueue *m_poDataUnitQueue;
      //## end ClBaseStorage::m_poDataUnitQueue%378C53460104.role

  protected:

    //## Other Operations (specified)
      //## Operation: Add%877539739
      //	  Introduce un nuevo bloque en nuestro almacén. Dicho bloque estará formado
      //	por los elementos que se pasan como paramétros:
      //
      //	  ai_pBlock  puntero a los datos del bloque.
      //	  ai_ulSize  tamaño del bloque.
      //	  ai_bEncrypted  si true, cifrado. Si false, no cifrado.
      //
      //	 Devuelve M4_SUCCESS
      m4return_t Add (m4pchar_t ai_pBlock, m4uint32_t ai_ulSize, m4bool_t ai_usEncrypted = M4_FALSE);

      //## Operation: Add%885288225
      m4return_t Add (ClDataUnitInterface *ai_pDataUnit);

      //## Operation: Get%877602376
      //	  Extrae la información contenida en el primer bloque de datos de nuestro
      //	almacén. Dicho bloque no es eliminado del almacén.
      //
      //	  ao_pBlock  recibe los datos guardados en el bloque.
      //	  ao_ulSize  tamaño del bloque que se saca.
      //	  ao_bEncrypted  true si esta cifrado, false caso contrario
      //
      //	retorna:
      //	 M4_SUCCESS bloque devuelto correctamente
      //	 M4_ERROR no se ha podido devolver el bloque.
      m4return_t Get (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted, m4uint32_t ai_ulBlockOrder = 0);

      //## Operation: Get%903715593
      m4return_t Get (ClDataUnitInterface *ai_pDataUnit);

      //## Operation: Remove%877602378
      //	  Elimina el bloque ai_ulDataUnit de la cola de unidades de dato almacenadas.
      //	  No borra la memoria que estaba reservada para la cadena de caracteres que
      //	contenía la Data Unit.
      //
      //	Retorna:
      //	  M4_ERROR si no se ha podidi eliminar el bloque.
      //	  M4_SUCCESS todo correcto.
      m4return_t Remove (m4uint32_t ai_ulDataUnit = 0);

      //## Operation: Dump%878295968
      void Dump (m4pchar_t  &ao_strBuffer);

    // Data Members for Class Attributes

      //## Attribute: m_ulOffsetOut%346AF85E0368
      //## begin ClBaseStorage::m_ulOffsetOut%346AF85E0368.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulOffsetOut;
      //## end ClBaseStorage::m_ulOffsetOut%346AF85E0368.attr

      //## Attribute: m_ulRelativeSize%346AF86803B2
      //## begin ClBaseStorage::m_ulRelativeSize%346AF86803B2.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulRelativeSize;
      //## end ClBaseStorage::m_ulRelativeSize%346AF86803B2.attr

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pTempPath%35DADC3A005F
      //## begin ClBaseStorage::m_pTempPath%35DADC3A005F.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pTempPath;
      //## end ClBaseStorage::m_pTempPath%35DADC3A005F.attr

      //## Attribute: m_DefragSize%35DAF6A1032F
      //## begin ClBaseStorage::m_DefragSize%35DAF6A1032F.attr preserve=no  private: m4uint32_t {UA} M4_PDU_FRAGMENTATION_SIZE
      m4uint32_t m_DefragSize;
      //## end ClBaseStorage::m_DefragSize%35DAF6A1032F.attr

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%3854CC1800F1
      //## Role: ClBaseStorage::m_bOwner%3854CC180281
      //## begin ClBaseStorage::m_bOwner%3854CC180281.role preserve=no  private: m4bool_t { -> VHAN}
      m4bool_t m_bOwner;
      //## end ClBaseStorage::m_bOwner%3854CC180281.role

  private: //## implementation

  private:
      m4bool_t m_bHasFiles;
  public:
      m4bool_t HasFileUnits() 
      {
          return m_bHasFiles;
      }
};

//## begin ClFileUnit%35D8607101BD.preface preserve=yes
//## end ClFileUnit%35D8607101BD.preface

//## Class: ClFileUnit%35D8607101BD
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClFileUnit : public ClDataUnitInterface  //## Inherits: <unnamed>%35D8612D0055
{
  //## begin ClFileUnit%35D8607101BD.initialDeclarations preserve=yes
  //## end ClFileUnit%35D8607101BD.initialDeclarations

  public:
    //## Constructors (specified)
      //## Operation: ClFileUnit%903369749
      ClFileUnit ();

      //## Operation: ClFileUnit%903369750
      ClFileUnit (m4pchar_t ai_pFileName, m4bool_t ai_usEncrypt = M4_FALSE, m4uint32_t ai_ulOffset = 0);

    //## Destructor (specified)
      //## Operation: ~ClFileUnit%903369751
      virtual ~ClFileUnit ();


    //## Other Operations (specified)
      //## Operation: GetSize%903369752
      m4uint32_t GetSize ();

      //## Operation: GetBuffer%903369753
      m4pchar_t GetBuffer ();

      //## Operation: GetFileName%903536129
      m4pchar_t GetFileName ();

      //## Operation: GetOnlyFileName%904235866
      m4pchar_t GetOnlyFileName ();

      //## Operation: GetFilePath%904051589
      m4pchar_t GetFilePath ();

      //## Operation: GetFileExtension%903536130
      m4pchar_t GetFileExtension ();

      //## Operation: GetSourceFileName%904575294
      m4pchar_t GetSourceFileName ();

      //## Operation: GetCryptState%903369754
      m4bool_t GetCryptState ();

      //## Operation: Dump%903369756
      void Dump (m4pchar_t  &ao_strBuffer);

      //## Operation: SetFile%903536132
      m4return_t SetFile (m4pchar_t ai_pFileName);

      //## Operation: SetFilePath%904051592
      m4return_t SetFilePath (m4pchar_t ai_pFileName);

      //## Operation: GetType%903536133
      m4int16_t GetType ();

      //## Operation: AttachBuffer%903603132
      m4return_t AttachBuffer (m4pchar_t ai_pBuffer, m4uint32_t ai_size);

      //## Operation: SetDefragSize%903603133
      void SetDefragSize (m4uint32_t ai_uiDefragSize);

      //## Operation: GetFileDescriptorString%903715590
      m4uint32_t GetFileDescriptorString ();

      //## Operation: SetFileDescriptorString%903961924
      m4return_t SetFileDescriptorString (m4pchar_t ao_pbuffer, m4int32_t ao_size);

      //## Operation: GetTotalSize%903715591
      m4int32_t GetTotalSize ();

      //## Operation: SetTotalSize%903961925
      m4return_t SetTotalSize (m4uint32_t ai_ulsize);

      //## Operation: GetRandomName%904320068
      void GetRandomName (m4pchar_t &ao_RandomName);

      //## Operation: IsCompressed%932487949
      m4bool_t IsCompressed ();

      //## Operation: SetCompressed%932487950
      void SetCompressed ();

      //## Operation: InitializeSerialState%940592257
      m4return_t InitializeSerialState ();

      //## Operation: GetFileDescriptorStringSize%970483020
      m4uint_t GetFileDescriptorStringSize ();

      //## Operation: GetFileSize%970483021
      m4uint_t GetFileSize ();

      //## Operation: GetChunkSize%970483023
      virtual m4uint32_t GetChunkSize ();

      //## Operation: GetDefragSize%970563312
      m4uint32_t GetDefragSize ();

    // Additional Public Declarations
      //## begin ClFileUnit%35D8607101BD.public preserve=yes
      //## end ClFileUnit%35D8607101BD.public

  protected:

    //## Other Operations (specified)
      //## Operation: GetFileDescriptor%903536131
      FILE * GetFileDescriptor ();

    // Data Members for Class Attributes

      //## Attribute: m_pFileName%35D9C1C500A0
      //## begin ClFileUnit::m_pFileName%35D9C1C500A0.attr preserve=no  protected: m4pchar_t {UA} NULL
      m4pchar_t m_pFileName;
      //## end ClFileUnit::m_pFileName%35D9C1C500A0.attr

    // Additional Protected Declarations
      //## begin ClFileUnit%35D8607101BD.protected preserve=yes
      //## end ClFileUnit%35D8607101BD.protected

  private:
    // Data Members for Class Attributes

      //## Attribute: m_pFileExtension%35D9C20101A1
      //## begin ClFileUnit::m_pFileExtension%35D9C20101A1.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pFileExtension;
      //## end ClFileUnit::m_pFileExtension%35D9C20101A1.attr

      //## Attribute: m_pFileDescriptor%35D9C2C60262
      //## begin ClFileUnit::m_pFileDescriptor%35D9C2C60262.attr preserve=no  private: FILE * {UA} NULL
      FILE *m_pFileDescriptor;
      //## end ClFileUnit::m_pFileDescriptor%35D9C2C60262.attr

      //## Attribute: m_uiFileIterator%35D9C32D01F2
      //## begin ClFileUnit::m_uiFileIterator%35D9C32D01F2.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uiFileIterator;
      //## end ClFileUnit::m_uiFileIterator%35D9C32D01F2.attr

      //## Attribute: m_uiDefragSize%35DBEBB60310
      //## begin ClFileUnit::m_uiDefragSize%35DBEBB60310.attr preserve=no  private: m4uint32_t {UA} M4_PDU_FRAGMENTATION_SIZE
      m4uint32_t m_uiDefragSize;
      //## end ClFileUnit::m_uiDefragSize%35DBEBB60310.attr

      //## Attribute: m_uiTotalSize%35DDA87C003A
      //## begin ClFileUnit::m_uiTotalSize%35DDA87C003A.attr preserve=no  private: m4uint32_t {UA} M4PDUS_FILEUNIT_UNDEFINED_SIZE
      m4uint32_t m_uiTotalSize;
      //## end ClFileUnit::m_uiTotalSize%35DDA87C003A.attr

      //## Attribute: m_pTempPath%35E2BB480017
      //## begin ClFileUnit::m_pTempPath%35E2BB480017.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pTempPath;
      //## end ClFileUnit::m_pTempPath%35E2BB480017.attr

      //## Attribute: m_pSourceFileName%35EAB8E300D0
      //## begin ClFileUnit::m_pSourceFileName%35EAB8E300D0.attr preserve=no  private: m4pchar_t {UA} NULL
      m4pchar_t m_pSourceFileName;
      //## end ClFileUnit::m_pSourceFileName%35EAB8E300D0.attr

      //## Attribute: m_bCompressed%3794A8300157
      //## begin ClFileUnit::m_bCompressed%3794A8300157.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bCompressed;
      //## end ClFileUnit::m_bCompressed%3794A8300157.attr

      //## Attribute: m_bGetFileNameString%39D88EA2037D
      //## begin ClFileUnit::m_bGetFileNameString%39D88EA2037D.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bGetFileNameString;
      //## end ClFileUnit::m_bGetFileNameString%39D88EA2037D.attr

      //## Attribute: m_bGetFileNameStringSize%39D8957E0346
      //## begin ClFileUnit::m_bGetFileNameStringSize%39D8957E0346.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bGetFileNameStringSize;
      //## end ClFileUnit::m_bGetFileNameStringSize%39D8957E0346.attr

    // Additional Private Declarations
      //## begin ClFileUnit%35D8607101BD.private preserve=yes
      //## end ClFileUnit%35D8607101BD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClFileUnit%35D8607101BD.implementation preserve=yes
      //## end ClFileUnit%35D8607101BD.implementation

};

//## begin ClFileUnit%35D8607101BD.postscript preserve=yes
//## end ClFileUnit%35D8607101BD.postscript


// Class for checking OLTP response time from m4dispatcher application.
class M4_DECL_M4PDUS ClCheckUnit : public ClDataUnitInterface 
{
  public:

      ClCheckUnit();

      virtual ~ClCheckUnit();

	  m4int16_t GetType();

	  m4uint32_t GetSize();

	  m4pchar_t GetBuffer();

	  m4return_t SetSize (m4uint32_t ai_uiSize);
};


//## Class: M4DataStorage%344DCF14019C
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3960812601B8;ClFileUnit { -> }

class M4_DECL_M4PDUS M4DataStorage : public ClBaseStorage  //## Inherits: <unnamed>%344DE8D0007D
{
  public:
    //## Destructor (specified)
      //## Operation: ~M4DataStorage%878295959
      ~M4DataStorage ();


    //## Other Operations (specified)
      //## Operation: Get%877602377
      //	  Extrae la información contenida en el primer bloque de datos de nuestro
      //	almacén. Además, dicho bloque será eliminado del almacén.
      //
      //	  ao_pBlock  recibe los datos guardados en el bloque.
      //	  ao_ulSize  tamaño del bloque que se saca.
      //	  ao_bEncrypted  true si esta cifrado, false caso contrario
      //
      //	retorna:
      //	 M4_SUCCESS bloque devuelto correctamente
      //	 M4_ERROR no se ha podido devolver el bloque.
      //
      //
      //	Importante:
      //	Con este get, no eliminamos el dato de el almacén. Cuando destruyamos el
      //	almacén, este dato también será destruido.
      m4return_t Get (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted, m4uint32_t ai_ulBlock = 0);

      //## Operation: Add%878295969
      //	  Introduce un nuevo bloque en nuestro almacén. Dicho bloque estará formado
      //	por los elementos que se pasan como paramétros:
      //
      //	  ai_pBlock  puntero a los datos del bloque.
      //	  ai_ulSize  tamaño del bloque.
      //	  ai_bEncrypted  si true, cifrado. Si false, no cifrado.
      m4return_t Add (m4pchar_t ai_pBlock, m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted = M4_DECRYPTED);

      //## Operation: Add%885288224
      m4return_t Add (ClDataUnitInterface *ai_pDataUnit);

      //## Operation: Remove%878295970
      //	  Elimina el bloque ai_ulDataUnit de la cola de unidades de dato almacenadas.
      //
      //	  No borra la memoria que estaba reservada para la cadena de caracteres que
      //	contenía la Data Unit.
      //
      //	Retorna:
      //	  M4_ERROR si no se ha podidi eliminar el bloque.
      //	  M4_SUCCESS todo correcto.
      m4return_t Remove (m4uint32_t ai_ulDataUnit = 0);

      //## Operation: Dump%878295965
      void Dump (m4pchar_t  &ao_strBuffer);

      //## Operation: Blocks%878806451
      //	  Devuelve el nº de bloques almacenados en el medio de almacenamiento.
      m4uint32_t Blocks ();

      //## Operation: Size%878806452
      //	   Devuelve el nº de bytes que existen almacenados en el alamacén.
      m4uint32_t Size ();

      //## Operation: GetNext%879336057
      m4return_t GetNext (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulSize, m4bool_t &ao_bEncrypted);

      //## Operation: GetNext%879425090
      m4return_t GetNext (m4uint32_t ai_ulDataOffset, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize, m4bool_t &ao_bEncrypted);

      //## Operation: GetNext%885978336
      m4return_t GetNext (ClDataUnitInterface * &ao_pDataUnit);

      //## Operation: GetNewDataUnit%885978334
      m4return_t GetNewDataUnit (ClDataUnit * &ao_pDataUnit);

      //## Operation: InitializeIteration%879336058
      void InitializeIteration ();

      //## Operation: InitializeSerialState%940592255
      m4return_t InitializeSerialState ();

      //## Operation: Clear%886066130
      void Clear ();

      //## Operation: GetNewDataStorage%886410895
      static M4DataStorage * GetNewDataStorage ();

      //## Operation: GetNewDataStorage%945078895
      static M4DataStorage * GetNewDataStorage (ClDataUnitQueue *ai_pDataUnitInterfaceQueue);

      //## Operation: DeleteDataStorage%886410896
      static m4return_t DeleteDataStorage (M4DataStorage * &aio_pDataStorage);

      //## Operation: GetNewFileUnit%903449685
      m4return_t GetNewFileUnit (ClFileUnit * &ao_pFileUnit);

      //## Operation: AddFileUnit%903536127
      m4return_t AddFileUnit (m4pchar_t ai_FileName, m4bool_t ai_bEncrypted = M4_DECRYPTED);

      //## Operation: GetCurrent%932487947; C++
      m4return_t GetCurrent (ClDataUnitInterface *&ao_pDataUnit);

      //## Operation: SetDefaultPath%962624238; C++
      static m4return_t SetDefaultPath (m4pchar_t ai_DefaultPath);

	  m4return_t CloneFiles (m4pcchar_t ai_pccPrerix);

      //## Operation: NewData%962696603
      //	Nos dice se han añadido/modificado los dataunit por lo que la PDU tendria
      //	que rehacer sus blockFormats.
      //	Resetea esta valor pues se supone que la PDU rehara su blockformat.
      m4bool_t NewData ();

      //## Operation: GetNewDataStorage%963819982; C++
      static M4DataStorage * GetNewDataStorage (M4DataStorage *ai_pDataStorage, m4bool_t ai_bCopyIt = M4_FALSE);

    // Data Members for Class Attributes

      //## Attribute: m_bFinishSerialization%3A012EE50398
      //## begin M4DataStorage::m_bFinishSerialization%3A012EE50398.attr preserve=no  public: m4bool_t {UA} M4_FALSE
      m4bool_t m_bFinishSerialization;
      //## end M4DataStorage::m_bFinishSerialization%3A012EE50398.attr

	  m4return_t GetNewCheckUnit (ClCheckUnit * &ao_pCheckUnit);

  protected:
    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%39608CE803DD
      //## Role: M4DataStorage::m_bObsoleteBlockFormat%39608CE903D5
      //## begin M4DataStorage::m_bObsoleteBlockFormat%39608CE903D5.role preserve=no  protected: m4bool_t { -> VHAN}
      m4bool_t m_bObsoleteBlockFormat;
      //## end M4DataStorage::m_bObsoleteBlockFormat%39608CE903D5.role

  private:
    //## Constructors (specified)
      //## Operation: M4DataStorage%877685668
      M4DataStorage ();

      //## Operation: M4DataStorage%945078894
      M4DataStorage (ClDataUnitQueue *ai_pDataUnitInterfaceQueue);

    // Data Members for Class Attributes

      //## Attribute: m_oIter%34699AC00093
      //## begin M4DataStorage::m_oIter%34699AC00093.attr preserve=no  private: ClDataUnitQueue::iterator {UA} 
      ClDataUnitQueue::iterator m_oIter;
      //## end M4DataStorage::m_oIter%34699AC00093.attr

      //## Attribute: m_uiSize%39D9F3F50324
      //## begin M4DataStorage::m_uiSize%39D9F3F50324.attr preserve=no  private: m4uint32_t {UA} 0
      m4uint32_t m_uiSize;
      //## end M4DataStorage::m_uiSize%39D9F3F50324.attr

  private: //## implementation
};

// Class ClFormatQueue 

// Class ClDataUnitInterface 

// Class ClDataUnit 

// Class ClDataUnitQueue 

// Class ClBaseStorage 

// Class ClFileUnit 

// Class M4DataStorage 

//## begin module%344E3133012D.epilog preserve=yes
//## end module%344E3133012D.epilog


#endif
