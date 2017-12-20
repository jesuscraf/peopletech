//## begin module%342BBAB400F3.cm preserve=no
//## end module%342BBAB400F3.cm

//## begin module%342BBAB400F3.cp preserve=no
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
//## end module%342BBAB400F3.cp

//## Module: basepdu%342BBAB400F3; Package specification
//## Subsystem: M4PDU::inc%3794593C0398
//## Source file: e:\mybuild\m4pdus\inc\basepdus.hpp

#ifndef basepdu_h
#define basepdu_h 1

//## begin module%342BBAB400F3.additionalIncludes preserve=no
//## end module%342BBAB400F3.additionalIncludes

//## begin module%342BBAB400F3.includes preserve=yes
#include <m4stl.hpp>
//## end module%342BBAB400F3.includes

// m4types
#include <m4types.hpp>
// m4string
#include <m4string.hpp>
// pduserializer
#include <pduserializer.hpp>
// pdutools
#include <pdutools.hpp>
// DataStorages
#include <datastorages.hpp>


//## begin module%342BBAB400F3.declarations preserve=no
//## end module%342BBAB400F3.declarations

//## begin module%342BBAB400F3.additionalDeclarations preserve=yes

#define M4_PDU_40_VERSION 40000
#define M4_PDU_32_VERSION 32000

#define M4_TYPE_PDU_SERVICE 1
#define M4_TYPE_PDU_CONTROL 2

#define M4_PDU_IDPDU_SIZE 8

// Estados de las PDUs.
// Los números pares corresponden al servidor. Los impares al
// cliente

#define	M4_PDUSTATE_HEAD1 0
#define M4_PDUSTATE_HEAD2 2
#define M4_PDUSTATE_HEAD3 3
#define M4_PDUSTATE_HEAD4 5
#define M4_PDUSTATE_HEAD5 6
#define M4_PDUSTATE_HEAD6 7
#define M4_PDUSTATE_HEADFULL 4
#define M4_PDUSTATE_PDUFULL 10
#define M4_PDUSTATE_HEADINI 1
#define M4_PDUSTATE_READSOCKET 5
#define M4_PDUSTATE_BM_RHEAD1 100
#define M4_PDUSTATE_BM_RHEAD2 101
#define M4_PDUSTATE_BM_HEADFULL 102
#define M4_PDUSTATE_HM_RHEAD 103

// Formas de devolver un bloque
const	m4uint8_t	M4_PDUBLOCKMODE_REFERENCE = 0;
const	m4uint8_t	M4_PDUBLOCKMODE_OWN = 1;


const m4int16_t	M4_ID_PDU_CONNECT = 0x0010;
const m4int16_t	M4_ID_PDU_AUTHENTICATION = 0x0012;
const m4int16_t	M4_ID_PDU_REQUEST = 0x0020;
const m4int16_t	M4_ID_PDU_REPLY = 0x0021;
const m4int16_t	M4_ID_PDU_DISCONNECT = 0x0030;
const m4int16_t	M4_ID_PDU_ACK = 0x0032;
const m4int16_t	M4_ID_PDU_ERROR = 0x0033;

const m4int16_t M4_ID_PDU_SETROLE = 0X0035;
const m4int16_t M4_ID_PDU_CHANGEPASSWORD = 0X0036;
const m4int16_t M4_ID_PDU_CANCEL = 0X0037;
const m4int16_t M4_ID_PDU_PING = 0X0038;

const m4int16_t M4_ID_PDU_NEWSETROLE = 0X0235;
const m4int16_t M4_ID_PDU_NEWCHANGEPASSWORD = 0X0236;
const m4int16_t M4_ID_PDU_NEWCANCEL = 0X0237;
const m4int16_t	M4_ID_PDU_NEWERROR = 0x0233;
const m4int16_t	M4_ID_PDU_NEWDISCONNECT = 0x0230;

const m4int16_t	M4_ID_PDU_SKIP = 0x5602;


//## end module%342BBAB400F3.additionalDeclarations


//## Class: ClPDU%3425254B03D2
//	Esta clase base se encarga de mantener las operaciones comunes que se pueden
//	realizar para acceder a los campos de una PDU. Sirve de base para derivar
//	subclases con los distintos tipos de PDU existentes.
//
//
//	Se trata de una clase base de la cual hay que derivar una clase por cada uno
//	de los tipos de PDUs que existen en el sistema.
//	Esta clase deberá completarse con el resto de las operaciones necesarias
//	para obtener una referencia a los diversos campos que forman cada tipo de
//	PDU concreta . También se deben añadir  las funciones complementarias para
//	cambiar los campos de la PDU.
//	Se debe tener en cuenta que para cada tipo de PDU habrá una serie de
//	constructores definidos que permitan construir la PDU de forma flexible,
//	intentando no tener que realizar movimientos de bloques de memoria para
//	construirla, por ejemplo cuando se construye una PDU correspondiente a un
//	resultado (PDU datos) no se pueden copiar los resultados en la zona de datos
//	de la PDU, esto sería demasiado gravoso en rendimiento, en su lugar se debe
//	haber construido previamente la PDU de respuesta y obtener una referencia a
//	la zona de datos a rellenar....
//	Por tanto, toda la gestión interna de la PDU (definición de su estructura)
//	dependerá del sistema gestor de memoria que se este utilizando si se desea
//	optimizar todo el proceso anterior, de aquí la existencia de un constructor
//	específico.
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%34320827033C;deque { -> }
//## Uses: <unnamed>%34335F390399;ClPairIntpChar { -> }
//## Uses: <unnamed>%34575FF202A0;M4ClString { -> }
//## Uses: <unnamed>%348E7D61028C;ClPairLongIntpChar { -> }
//## Uses: <unnamed>%3497BE2502A5;m4uint16_t { -> }
//## Uses: <unnamed>%34CF2CE001E3;ClDataUnit { -> F}

class M4_DECL_M4PDUS ClPDU 
{
  public:
    //## Constructors (specified)
      //## Operation: ClPDU%875692541; C++
      ClPDU (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClPDU%878377084
      virtual ~ClPDU ();


    //## Other Operations (specified)
      //## Operation: GetHeadSize%875278540
      virtual m4uint32_t GetHeadSize ();

      //## Operation: GetIdPDU%876384553
      m4uint16_t GetIdPDU ();

      //## Operation: GetTypePDU%882288136
      virtual m4uint16_t GetTypePDU ();

      //## Operation: GetSize%875556372
      m4uint32_t GetSize ();

      m4uint32_t GetDeltaSize( void ) const ;
      void SetDeltaSize( m4uint32_t ai_iDeltaSize ) ;

      //## Operation: GetTimeMark%875556373
      m4uint64_t GetTimeMark ();

      //## Operation: SetTimeMark%878662601
      void SetTimeMark (m4uint64_t ai_ulTimeMark);

      //## Operation: Dump%876384568
      virtual void Dump (m4pchar_t  &ao_pTexto);

      //## Operation: GetPDUSize%932487951
      virtual m4uint32_t GetPDUSize ();

      //## Operation: Clear%878662608
      //	  Inicializa la iteración sobre los bloque que se proporcionan a la PDU.
      //	  Se debe combinar con GetNextBlockSize y AddBlock.
      virtual void Clear ();

      //## Operation: AddBlock%879761053
      //	    Se usa el la deserialización de la PDU. Siempre en colaboración con Get
      //	NextSizeBlock. GetNextSizeBlock informa del tamaño que espera la PDU que se
      //	le pase con AddBlock.
      //
      //	Entrada:
      //	  m4pchar_t  &ao_pBlock
      //	  Cadena de caracteres que se le pasa a la PDU para que la deserialize. El
      //	tamaño debe coincidir con la respuesta de GetNextSizeBlock
      virtual m4return_t AddBlock (m4pchar_t &ao_pBlock);

      //## Operation: AddDataBlock%885373442
      //	  Añade una DataUnit al DataStorage que contiene la PDU.
      //	  Esta DataUnit se monta internamente en la PDU, se le pasan como parámetros
      //	la información que contendrá esta DataUnit.
      //
      //	m4pchar_t  ai_poDataBlock
      //	Cadena de caracteres que irá en la DataUnit
      //
      //	m4uint32_t ai_ulSize
      //	Tamaño de la cadena de caracteres.
      //
      //	m4bool_t ai_bEncrypted = M4_FALSE
      //	Estado de la encriptación de esta cadena.
      virtual m4return_t AddDataBlock (m4pchar_t ai_poDataBlock, m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted = M4_FALSE);

      //## Operation: GetHeadBlock%878662610
      //	  Devuelve un bloque con la cabecera de la PDU en formato binario.
      //
      //	  ao_pBlock  Bloque que devuelve la PDU.
      //	  ao_ulBlockSize  Tamaño del bloque que se pasa en ao_pBlock.
      //
      //	  Retorna:
      //	  M4_SUCCESS si la el bloque se ha rellenado correctamente.
      //	  M4_ERROR si ha surgido algún problema.
      //
      //	  Detalles:
      //	  Esta función debe usarse en combinación con InitializeBlockIteration y Add
      //	Block.
      virtual m4return_t GetHeadBlock (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize);

      //## Operation: GetBinaryHead%878662611
      //	   Pasa todos los campos significativos de la PDU a una cadena de
      //	caracteres. El formato de los campos numéricos será binario.
      virtual m4return_t GetBinaryHead (m4pchar_t &ao_pBuffer, m4uint32_t &ao_ulBufferSize);

      //## Operation: GetNextBlock%879425087
      //	  Obtiene el siguiente bloque, o parte final de, que corresponde leer a
      //	partir de la cantidad de bytes leidas.
      //
      //	Argumentos:
      //	  ai_ulReadSize, total de bytes leidos, hasta el momento,  por el usuario
      //	  ao_pBlock, bloque leido
      //	  ao_ulBlockSize, tamaño del bloque leido
      //
      //	Salida:
      //	  M4_ERROR, si no quedan más datos que leer
      //	  M4_SUCCESS, exito.
      virtual m4return_t GetNextBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize);

      //## Operation: GetNextSizeBlock%882359184
      virtual m4uint32_t GetNextSizeBlock (m4pchar_t &ao_pBlock);

      //## Operation: GetDataStorage%884343824
      //	  Si la PDU puede tener datos:
      //	  Retorna un puntero al DataStorage que contiene la PDU.
      //
      //	  Si ai_bOwner == M4_TRUE, el DataStorage pasará a ser propiedad de la PDU.
      //	  Si ai_bOwner ==  M4_FALSE, la PDU no será propietaria del DataStorage.
      //
      //
      //	  Si la PDU no puede tener datos:
      //	  Retorna NULL.
      virtual M4DataStorage * GetDataStorage (m4bool_t ai_bOwner = M4_TRUE);

      //## Operation: SetDataStorage%884343825
      //	  Asigna ai_poDataStorage como el DataStorage contenido en la PDU.
      //
      //	  Si ai_bOwner == M4_TRUE, el DataStorage pasará a ser propiedad de la PDU.
      //	  Si ai_bOwner ==  M4_FALSE, la PDU no será propietaria del DataStorage.
      //
      //	  Si la PDU no puede tener datos:
      //	  No hace nada.
      virtual void SetDataStorage (M4DataStorage *ai_poDataStorage, m4bool_t ai_bOwner = M4_TRUE);

      //## Operation: Inicialize%884850778
      virtual void Inicialize (ClPDU *ai_pPDU);

      //## Operation: AddDataUnit%885978329
      //	   Añade una DataUnit al DataStorage que contiene la PDU.
      virtual m4return_t AddDataUnit (ClDataUnit *ai_pDataUnit);

      //## Operation: GetNewDataBuffer%887043276
      //	  Reserva memoria para una cadena de caracteres del tamaño que se le indica.
      //	Tanto la cadena como el tamaño se pasan como parámetros.
      //
      //	m4pchar_t &ao_pBuffer
      //
      //
      //	m4uint32_t ao_ulSize
      virtual m4return_t GetNewDataBuffer (m4pchar_t &ao_pBuffer, m4uint32_t ao_ulSize);

      //## Operation: EnableLogInfo%896370093
      void EnableLogInfo ();

      //## Operation: DisableLogInfo%896370094
      void DisableLogInfo ();

      //## Operation: GetLogInfo%896370095
      m4return_t GetLogInfo (m4pchar_t &ao_pLogInfo, m4uint32_t &ai_lLogInfoSize);

      //## Operation: SetLogInfo%896370096
      m4return_t SetLogInfo (m4pchar_t ai_pLogInfo, m4uint32_t ai_lLogInfoSize);

      //## Operation: LogInfo%896370097
      m4bool_t LogInfo ();

	  m4uint8_t GetHasOverwriteMask( void ) const ;
	  void SetHasOverwriteMask( m4uint8_t ai_iHasOverwriteMask ) ;

	  m4uint8_t GetOverwriteMask( void ) const ;
	  void SetOverwriteMask( m4uint8_t ai_iOverwriteMask ) ;

      //## Operation: GetVersion%964010743
      m4int_t GetVersion ();

      //## Operation: GetRequestId%964010744; C++
      m4uint32_t GetRequestId ();

	  m4uint32_t GetUserInfo();

	  m4void_t SetuserInfo(m4uint32_t ai_uiUserInfo);

      //## Operation: SetRequestId%964010745; C++
      void SetRequestId (m4uint32_t ai_ulRequestId);

      //## Operation: SetCredential%972376846; C++
      virtual m4return_t SetCredential (m4string_t &ai_strCredential);

      //## Operation: GetCredential%972376847; C++
      virtual m4return_t GetCredential (m4string_t &ao_strCredential);



      virtual m4return_t GetCertificate (m4pchar_t &ao_pCertificate , m4uint32_t & ao_iCertificateLen);
      virtual m4return_t SetCertificate (m4pchar_t ai_pCertificate , m4uint32_t ai_iCertificateLen);

      virtual m4return_t SetSessContext (m4pchar_t ao_pSessContex , m4uint32_t ao_iSessContextLen);
      virtual m4return_t GetSessContext (m4pchar_t &ai_pSessContex , m4uint32_t &ai_iSessContextLen);

	  
	  // Data Members for Class Attributes

      //## Attribute: m_uiState%34337FC502BE
      //## begin ClPDU::m_uiState%34337FC502BE.attr preserve=no  public: m4uint16_t {UA} M4_PDUSTATE_HEAD1
      m4uint16_t m_uiState;
      //## end ClPDU::m_uiState%34337FC502BE.attr

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%396D656302EB
      //## Role: ClPDU::m_oSerializer%396D65640292
      //## begin ClPDU::m_oSerializer%396D65640292.role preserve=no  public: ClPDUAdvSerializer { -> VHAN}
      ClPDUAdvSerializer m_oSerializer;
      //## end ClPDU::m_oSerializer%396D65640292.role

	  // UserInfo callback
	  typedef m4uint32_t (*UserInfoCallBackType)();
	  static void RegisterUserInfoCallBack(UserInfoCallBackType ai_pfCallBack);
	  static void UnregisterUserInfoCallBack();

  protected:
    // Data Members for Class Attributes

	  m4uint32_t GetUserInfoFromCallBack();

      //## Attribute: m_uiId%342F9A990365
      //## begin ClPDU::m_uiId%342F9A990365.attr preserve=no  protected: m4uint16_t {UA} 0
      m4uint16_t m_uiId;
      //## end ClPDU::m_uiId%342F9A990365.attr

      //## Attribute: m_ulSize%342F9AB00169
      //## begin ClPDU::m_ulSize%342F9AB00169.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulSize;
      //## end ClPDU::m_ulSize%342F9AB00169.attr

      m4uint32_t m_ulDeltaSize;

      //## Attribute: m_ulTimeMark%342F9AE601FD
      //## begin ClPDU::m_ulTimeMark%342F9AE601FD.attr preserve=no  protected: m4uint64_t {UA} 0
      m4uint64_t m_ulTimeMark;
      //## end ClPDU::m_ulTimeMark%342F9AE601FD.attr

      //## Attribute: m_uiHeadSize%345DA51D0136
      //## begin ClPDU::m_uiHeadSize%345DA51D0136.attr preserve=no  protected: m4uint16_t {UA} 0
      m4uint16_t m_uiHeadSize;
      //## end ClPDU::m_uiHeadSize%345DA51D0136.attr

      //## Attribute: m_pBinaryHead%346AD79B000B
      //## begin ClPDU::m_pBinaryHead%346AD79B000B.attr preserve=no  protected: m4pchar_t {UA} NULL
      m4pchar_t m_pBinaryHead;
      //## end ClPDU::m_pBinaryHead%346AD79B000B.attr

      //## Attribute: m_oFile%34F6D750017F
      //## begin ClPDU::m_oFile%34F6D750017F.attr preserve=no  protected: static fstream {VA} 
      static fstream m_oFile;
      //## end ClPDU::m_oFile%34F6D750017F.attr

      //## Attribute: m_szDump%350956E100B7
      //	  Aquí se guarda la información que se saca con los Dump.
      //
      //	  En el destructor se deberá eliminar. Además, cada vez que se hace un Dump
      //	se deberá eliminar el contenido anterior.
      //## begin ClPDU::m_szDump%350956E100B7.attr preserve=no  protected: m4pchar_t {UA} NULL
      m4pchar_t m_szDump;
      //## end ClPDU::m_szDump%350956E100B7.attr

      //## Attribute: m_uiLogInfo%356D85170054
      //## begin ClPDU::m_uiLogInfo%356D85170054.attr preserve=no  protected: m4uint16_t {UA} 
      m4uint16_t m_uiLogInfo;
      //## end ClPDU::m_uiLogInfo%356D85170054.attr

	  m4uint8_t m_iHasOverwriteMask;
	  m4uint8_t m_iOverwriteMask;

      //## Attribute: m_oLogInformation%356D851702CB
      //## begin ClPDU::m_oLogInformation%356D851702CB.attr preserve=no  protected: ClPairLongIntpChar {UA} 
      ClPairLongIntpChar m_oLogInformation;
      //## end ClPDU::m_oLogInformation%356D851702CB.attr

      //## Attribute: m_iVersion%3975A2D80245
      //## begin ClPDU::m_iVersion%3975A2D80245.attr preserve=no  protected: m4int_t {UA} M4_PDU_40_VERSION
      m4int_t m_iVersion;
      //## end ClPDU::m_iVersion%3975A2D80245.attr

      //## Attribute: m_ulRequestID%3975A4740319
      //## begin ClPDU::m_ulRequestID%3975A4740319.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulRequestID;
      //## end ClPDU::m_ulRequestID%3975A4740319.attr

      //## Attribute: m_uiType%39DB110902F7
      //## begin ClPDU::m_uiType%39DB110902F7.attr preserve=no  protected: m4uint16_t {UA} M4_TYPE_PDU_CONTROL
      m4uint16_t m_uiType;
      //## end ClPDU::m_uiType%39DB110902F7.attr

	  m4uint32_t m_uiUserInfo;

  private:
  private: //## implementation

		static UserInfoCallBackType m_pfUserInfoCallBack;

  private:
      m4uint32_t m_ObjectId;
      m4uint32_t m_CompressionType;
  public:
      void SetObjectId(m4uint32_t ai_objectId) 
      {
          m_ObjectId = ai_objectId;
      }
      m4int32_t GetObjectId()
      {
          return m_ObjectId;
      }

      void SetCompressionType(m4int32_t ai_compressionType) 
      {
          m_CompressionType = ai_compressionType;
      }
      m4int32_t GetCompressionType()
      {
          return m_CompressionType;
      }
};

//## Class: ClPDUWithData%347948EB0030
//## Category: M4PDU%34BA420E0375
//## Subsystem: M4PDU::inc%3794593C0398
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class M4_DECL_M4PDUS ClPDUWithData : public ClPDU  //## Inherits: <unnamed>%34E1D2E4033B
{
  public:
    //## Constructors (specified)
      //## Operation: ClPDUWithData%880389200; C++
      ClPDUWithData (m4int_t ai_iVersion = M4_PDU_40_VERSION);

    //## Destructor (specified)
      //## Operation: ~ClPDUWithData%880389201
      ~ClPDUWithData ();


    //## Other Operations (specified)
      //## Operation: SetNBlocks%880389202
      void SetNBlocks (m4uint32_t ai_ulNBlocks);

      //## Operation: GetNBlocks%880389203
      m4uint32_t GetNBlocks ();

      //## Operation: AddFormatBlocks%880451466
      m4return_t AddFormatBlocks (m4pchar_t ai_poDataBlock, m4uint32_t ai_ulSize);

      //## Operation: GetNextDataBlock%880389207
      m4return_t GetNextDataBlock (m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize);

      //## Operation: GetNextDataBlock%880389208
      m4return_t GetNextDataBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize);

      //## Operation: GetDataSize%880389209
      m4uint32_t GetDataSize ();

      //## Operation: Clear%880389211
      void Clear ();

      //## Operation: Dump%880389212
      void Dump (m4pchar_t &ao_pTexto);

      //## Operation: InitializeIteration%880389213
      void InitializeIteration ();

      //## Operation: InitializeReadIteration%880389214
      void InitializeReadIteration ();

      //## Operation: ActualizeBlockFormats%886605281
      m4return_t ActualizeBlockFormats ();

      //## Operation: SetTempPath%904051590
      void SetTmpPath (m4pchar_t ai_pTempPath);

      //## Operation: GetTempPath%904051591
      m4pchar_t GetTmpPath ();

      //## Operation: GetDataStorage%969437745
      //	  Si la PDU puede tener datos:
      //	  Retorna un puntero al DataStorage que contiene la PDU.
      //
      //	  Si ai_bOwner == M4_TRUE, el DataStorage pasará a ser propiedad de la PDU.
      //	  Si ai_bOwner ==  M4_FALSE, la PDU no será propietaria del DataStorage.
      //
      //
      //	  Si la PDU no puede tener datos:
      //	  Retorna NULL.
      M4DataStorage * GetDataStorage (m4bool_t ai_bOwner = M4_TRUE);

      //## Operation: SetDataStorage%969437746
      //	  Asigna ai_poDataStorage como el DataStorage contenido en la PDU.
      //
      //	  Si ai_bOwner == M4_TRUE, el DataStorage pasará a ser propiedad de la PDU.
      //	  Si ai_bOwner ==  M4_FALSE, la PDU no será propietaria del DataStorage.
      //
      //	  Si la PDU no puede tener datos:
      //	  No hace nada.
      void SetDataStorage (M4DataStorage *ai_poDataStorage, m4bool_t ai_bOwner = M4_TRUE);

      //## Operation: GetPDUSize%970563313
      virtual m4uint32_t GetPDUSize ();

      //## Operation: GetNextBlock%970646985
      virtual m4return_t GetNextBlock (m4uint32_t ai_ulReadSize, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize);

      //## Operation: SetCredential%972376844; C++
      m4return_t SetCredential (m4string_t &ai_strCredential);

      //## Operation: GetCredential%972376845; C++
      m4return_t GetCredential (m4string_t &ao_strCredential);

      //## Operation: SerializeCredential%972376848
      m4return_t SerializeCredential ();


      m4return_t GetCertificate (m4pchar_t &ao_pCertificate , m4uint32_t & ao_iCertificateLen);
      m4return_t SetCertificate (m4pchar_t ai_pCertificate , m4uint32_t ai_iCertificateLen);

      m4return_t SetSessContext (m4pchar_t ai_pSessContex , m4uint32_t ai_iSessContextLen);
      m4return_t GetSessContext (m4pchar_t & ao_pSessContex , m4uint32_t & ao_iSessContextLen);

      m4return_t SerializeCertificate ();
      m4return_t SerializeSessContext ();

      //## Operation: SerializeLog%972376849
      m4return_t SerializeLog ();

      //## Operation: ExtractCredential%972395359
      m4return_t ExtractCredential ();

      m4return_t ExtractCertificate ();
      m4return_t ExtractSessContext ();

      //## Operation: ExtractLog%972395360
      m4return_t ExtractLog ();

      //## Operation: ExtData2DataStorage%972460315
      m4return_t ExtData2DataStorage ();

      //## Operation: DataStorage2ExtData%972460316
      m4return_t DataStorage2ExtData ();

	  // bg 124194
	  m4bool_t GetOwnerState() { return m_bDataStorageOwner; }

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%378C51CF03E0
      //## Role: ClPDUWithData::m_poDataStorage%378C51D003B0
      //## begin ClPDUWithData::m_poDataStorage%378C51D003B0.role preserve=no  public: M4DataStorage { -> 1RHAN}
      M4DataStorage *m_poDataStorage;
      //## end ClPDUWithData::m_poDataStorage%378C51D003B0.role

      //## Association: M4PDU::<unnamed>%378C520F00AD
      //## Role: ClPDUWithData::m_poBlockFormats%378C52100113
      //## begin ClPDUWithData::m_poBlockFormats%378C52100113.role preserve=no  public: ClBlockFormats { -> 1RHAN}
      ClBlockFormats *m_poBlockFormats;
      //## end ClPDUWithData::m_poBlockFormats%378C52100113.role

  protected:

    //## Other Operations (specified)
      //## Operation: WAddDataBlock%880389204
      virtual m4return_t WAddDataBlock (m4pchar_t ai_poDataBlock, m4uint32_t ai_ulSize, m4bool_t ai_bEncrypted = M4_FALSE);

      //## Operation: GetDataBlock1%880389205
      //	Esta función se usa para  aplanar la información que contiene la PDU.
      //
      //	  Recibe tres parametros,  1 de entrada y dos de salida:
      //
      //	  ai_ulBlock: Bloque que se desea leer ( 0 - valuado ).
      //	  ao_pBlock: puntero al  bloque de memoria que devuelve
      //	  ao_ulBlockSize: Tamaño del bloque que se pasa en ao_pBlock.
      //	  ao_usEncrypted: Recibirá información sobre el estado de cifrado del bloque
      //	que se pasa en ao_pBlock.
      //
      //	  Retorna:
      //	  M4_ERROR si no se puede pasar el bloque.
      //	  M4_SUCCESS si se ejecuta correctamente.
      m4return_t GetDataBlock1 (m4uint32_t ai_ulBlock, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize, m4bool_t ao_bEncrypted, m4uint8_t ai_usMode = M4_PDUBLOCKMODE_REFERENCE);

      //## Operation: GetDataBlock1%880389206
      //	Esta función se usa para  aplanar la información que contiene la PDU.
      //
      //	Parámetros:
      //
      //	  ai_ulReadSize: Bloque que se desea leer ( 0 - valuado ).
      //	  ao_pBlock: puntero al nuevo bloque de memoria
      //	  ao_ulBlockSize: Tamaño del bloque que se pasa en ao_pBlock.
      //	  ai_usMode: Informa de la relación que tendrá con la PDU el bloque que se
      //	devuelve, sus posibles valores son:
      //	          M4_PDUBLOCKMODE_REFERENCE, la PDU seguirá teniendo control sobre
      //	la vida del bloque.
      //	         M4_PDUBLOCKMODE_OWN, la PDU pierde todo control sobre este bloque.
      //
      //
      //	  Retorna:
      //	  M4_ERROR si no se puede pasar el bloque.
      //	  M4_SUCCESS si se ejecuta correctamente.
      m4return_t GetDataBlock1 (m4uint32_t ai_ulBlock, m4pchar_t &ao_pBlock, m4uint32_t &ao_ulBlockSize, m4uint8_t ai_usMode);

      //## Operation: WGetDataStorage%884343826
      //	  Retorna un puntero al DataStorage que contiene la PDU.
      //
      //	  Si ai_bOwner == M4_TRUE, el DataStorage pasará a ser propiedad de la PDU.
      //	  Si ai_bOwner ==  M4_FALSE, la PDU no será propietaria del DataStorage.
      M4DataStorage * WGetDataStorage (m4bool_t ai_bOwner = M4_TRUE);

      //## Operation: WSetDataStorage%884343827
      //	  Asigna ai_poDataStorage como el DataStorage contenido en la PDU.
      //
      //	  Si ai_bOwner == M4_TRUE, el DataStorage pasará a ser propiedad de la PDU.
      //	  Si ai_bOwner ==  M4_FALSE, la PDU no será propietaria del DataStorage.
      void WSetDataStorage (M4DataStorage *ai_poDataStorage, m4bool_t ai_bOwner = M4_TRUE);

      //## Operation: WAddDataUnit%885978330
      m4return_t WAddDataUnit (ClDataUnit *ai_pDataUnit);

      //## Operation: SerializeFormatsBlock%963407995; C++
      m4return_t SerializeFormatsBlock ();

      //## Operation: GetNextDataStorageChunkSize%972395361; C++
      m4uint32_t GetNextDataStorageChunkSize (m4pchar_t &ao_pBlock);

    // Data Members for Class Attributes

      //## Attribute: m_ulNBlocks%3479B1910303
      //## begin ClPDUWithData::m_ulNBlocks%3479B1910303.attr preserve=no  protected: m4uint32_t {UA} 0
      m4uint32_t m_ulNBlocks;
      //## end ClPDUWithData::m_ulNBlocks%3479B1910303.attr

    // Data Members for Associations

      //## Association: M4PDU::<unnamed>%39F54AE30076
      //## Role: ClPDUWithData::m_strCredential%39F54AE302ED
      //## begin ClPDUWithData::m_strCredential%39F54AE302ED.role preserve=no  protected: m4string_t { -> VHAN}
      m4string_t m_strCredential;
      //## end ClPDUWithData::m_strCredential%39F54AE302ED.role

	  m4pchar_t m_szCertificate ;

	  m4pchar_t m_szSessContext ;

	  m4uint32_t m_iCertificateLen ;

	  m4uint32_t m_iSessContextLen ;

  private:
    // Data Members for Class Attributes

      //## Attribute: m_bDataStorageOwner%34B612860267
      //## begin ClPDUWithData::m_bDataStorageOwner%34B612860267.attr preserve=no  private: m4bool_t {UA} M4_TRUE
      m4bool_t m_bDataStorageOwner;
      //## end ClPDUWithData::m_bDataStorageOwner%34B612860267.attr

      //## Attribute: m_bIsExtDataInDataStorage%39F6910B038E
      //## begin ClPDUWithData::m_bIsExtDataInDataStorage%39F6910B038E.attr preserve=no  private: m4bool_t {UA} M4_FALSE
      m4bool_t m_bIsExtDataInDataStorage;
      //## end ClPDUWithData::m_bIsExtDataInDataStorage%39F6910B038E.attr

  private: //## implementation
};

// Class ClPDU 

// Class ClPDUWithData 

//## begin module%342BBAB400F3.epilog preserve=yes
//## end module%342BBAB400F3.epilog


#endif
