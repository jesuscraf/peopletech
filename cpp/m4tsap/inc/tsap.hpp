//## begin module%3777684302B8.cm preserve=no
//## end module%3777684302B8.cm

//## begin module%3777684302B8.cp preserve=no
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
//## end module%3777684302B8.cp

//## Module: tsap%3777684302B8; Package specification
//## Subsystem: M4Tsap::inc%37776840008D
//	d:\compon\m4tsap\version\inc
//## Source file: c:\Componentes\m4tsap\inc\tsap.hpp

#ifndef xtsap_h
#define xtsap_h 1

//## begin module%3777684302B8.additionalIncludes preserve=no
//## end module%3777684302B8.additionalIncludes

//## begin module%3777684302B8.includes preserve=yes
#include "m4tsap_dll.hpp"
//include con las definiciones necesarias para el uso de la librería TSAP
//## end module%3777684302B8.includes

// m4types
#include <m4types.hpp>
// syncro
#include <syncro.hpp>

class ClTSAPImplFactory;
class ClTSAPImpl;
class ClTSAPSelectImpl;

//## begin module%3777684302B8.declarations preserve=no
//## end module%3777684302B8.declarations

//## begin module%3777684302B8.additionalDeclarations preserve=yes



#define	M4TSAP_OK								0
#define	M4TSAP_NOT_INITIALIZED					1
#define	M4TSAP_NOT_IMPLEMENTED					2		
#define	M4TSAP_NOT_KNOWN						3		
#define	M4TSAP_SOCKET_FAILED					4		
#define	M4TSAP_BIND_FAILED						5 	
#define	M4TSAP_ACCEPT_FAILED					6		
#define	M4TSAP_CONNECT_FAILED					7		
#define	M4TSAP_LISTEN_FAILED					8		
#define	M4TSAP_WRITE_FAILED						9	
#define	M4TSAP_READ_FAILED						10
#define	M4TSAP_INVALID_HOSTNAME					11	
#define	M4TSAP_INVALID_INET_ADDRESS				12

#define	M4TSAP_CERTIFICATE_CA					13
#define	M4TSAP_INVALID_PRIVATE_KEY				14		
#define	M4TSAP_INVALID_CERTIFICATE				15	

#define	M4TSAP_SSL_CLIENT_NOT_INITIALIZED		16
#define	M4TSAP_SSL_SERVER_NOT_INITIALIZED		17
#define	M4TSAP_SSL_SSL_NEW_FAILED				18
#define	M4TSAP_SSL_SET_CIPHER_LIST_FAILED		19
#define	M4TSAP_SSL_ACCEPT_FAILED				20
#define	M4TSAP_SSL_CONNECT_FAILED				21
#define	M4TSAP_SSL_GET_PEER_CERTIFICATE_FAILED	22
#define	M4TSAP_SSL_X509_NAME_ONLINE_FAILED		23
#define	M4TSAP_SSL_M_SSL_NOT_INITIALIZED		24
#define	M4TSAP_SSL_WRITE_FAILED					25
#define	M4TSAP_SSL_READ_FAILED					26

#define M4_SIZE_INET_ADDRESS	4

#define M4SOCK_INVALIDSOCKET  -1
#define M4SOCK_ERROR  -1

//Defines para SocketInit con parámetros (SSL)
#define M4_SSL_CLIENT					1
#define M4_SSL_SERVER					2
#define M4_SSL_CLIENTSERVER				3
#define M4_SSL_CLIENT_CERTIFICATE		4
#define M4_SSL_CLIENTSERVER_CERTIFICATE	5

//Defines para constructor de TSAP con parámetros int, void * (SSL)
#define	M4_SSL_SET_PREFERED_CIPHER	101

#ifndef IS_IN_READ
#define IS_IN_READ			0
#define IS_IN_WRITE			1
#define IS_IN_EXCEPTIONS	2
#endif



int M4_DECL_M4TSAP	SocketInit(void);
void M4_DECL_M4TSAP	SocketFinish();
int M4_DECL_M4TSAP	SocketInit(int ai_iModeSSL, void *ai_pData);
//## end module%3777684302B8.additionalDeclarations


//## Class: ClInetAddress%3777684400D1
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37776844022E;m4pchar_t { -> }

class M4_DECL_M4TSAP ClInetAddress 
{
  public:
    //## Constructors (specified)
      //## Operation: ClInetAddress%40079955; C++
      ClInetAddress (const char* ai_pcHostname);

      //## Operation: ClInetAddress%1461341715; C++
      ClInetAddress (const m4uint8_t* ai_puiAddr);


    //## Other Operations (specified)
      //## Operation: GetAddress%-1928642072; C++
      const m4uint8_t* GetAddress () const;

      //## Operation: IsRight%928100026; C++
      m4bool_t IsRight () const;

      //## Operation: GetIP%-1379364228; C++
      m4bool_t GetIP (m4pchar_t ai_szAddress, int ai_iMaxLen);

      //## Operation: GetHostname%1323407581; C++
      m4bool_t GetHostname (m4pchar_t ai_szHostName, int ai_iMaxLen);

    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%37776844022A
      //## Role: ClInetAddress::m_s_iAddrLen%37776844022C
      //## begin ClInetAddress::m_s_iAddrLen%37776844022C.role preserve=no  public: static m4size_t {1 -> 1VHAPN}
      static m4size_t m_s_iAddrLen;
      //## end ClInetAddress::m_s_iAddrLen%37776844022C.role

  protected:
    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%37776844023E
      //## Role: ClInetAddress::m_aAddress%377768440240
      //## begin ClInetAddress::m_aAddress%377768440240.role preserve=no  protected: m4uint8_t {1 -> 16VHAPN}
      m4uint8_t m_aAddress[16];
      //## end ClInetAddress::m_aAddress%377768440240.role

  private:
    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%377768440242
      //## Role: ClInetAddress::m_bCorrecta%377768440244
      //## begin ClInetAddress::m_bCorrecta%377768440244.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bCorrecta;
      //## end ClInetAddress::m_bCorrecta%377768440244.role

  private: //## implementation
};

//## Class: ClTSAP%3777684301F1
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37776844030F;ClInetAddress { -> }
//## Uses: <unnamed>%377768440338;m4return_t { -> }
//## Uses: <unnamed>%377B369301DE;m4pchar_t { -> }

class M4_DECL_M4TSAP ClTSAP 
{
  public:
    //## Constructors (specified)
      //## Operation: ClTSAP%-1259939195; C++
      ClTSAP (const char* ai_pcHostname, int ai_iPort, m4bool_t ai_bStream = M4_TRUE);

      //## Operation: ClTSAP%70042277; C++
      ClTSAP (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream = M4_TRUE);

      //## Operation: ClTSAP%1469968033; C++
      ClTSAP ();

      //## Operation: ClTSAP%-1779220893; C++
      //	Constructor para TSAP en modo SSL
      ClTSAP (int ai_iMode, void* ai_pData, const char* ai_pcHostname, int ai_iPort, m4bool_t ai_bStream = M4_TRUE);

      //## Operation: ClTSAP%682838846; C++
      //	Constructor para uso desde ClServerTSAP, a partir de un Accept, no usa host
      //	ni puerto
      ClTSAP (int ai_iMode, void* ai_pData);

    //## Destructor (specified)
      //## Operation: ~ClTSAP%760283972; C++
      ~ClTSAP ();


    //## Other Operations (specified)
      //## Operation: Init%-1334151515; C++
      m4bool_t Init (const char* ai_pcHostname, int ai_iPort, m4bool_t ai_bStream = M4_TRUE);

      //## Operation: Init%1585032072; C++
      m4bool_t Init (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream = M4_TRUE);

      //## Operation: Close%-840728080; C++
      void Close ();

      //## Operation: Write%-824776902; C++
      int Write (const void* ai_pcSrc, int ai_iLen);

      //## Operation: Read%732633365; C++
      int Read (void* ao_pcDst, int ai_iLen, m4bool_t ai_bTotal = M4_FALSE);

      //## Operation: SetBlockingIO%61045530; C++
      //	-- {AddDecl: 160} region.unprotectedFunction [3618..3777]
      //	Función que determina el comportamiento del TSAP.
      //	Si se lanza con parámetro M4_TRUE, el TSAP tiene un
      //	comportamiente bloqueante. Si M4_FALSE, no bloqueante.
      //	Por defecto, su comportamiento es bloqueante.
      //	Atención: Si se usa en modo bloqueante, puede tardar
      //	mucho en devolver control.Si se usa en modo no
      //	bloqueante, hay que tener en cuenta que puede  escribir
      //	menos bytes de los que se le indique sin que ello
      //	signifique un error, y que Read devuelva 0.
      int SetBlockingIO (m4bool_t ai_bBlocking = M4_TRUE);

      //## Operation: GetLocalPort%1039449503; C++
      int GetLocalPort () const;

      //## Operation: GetPort%635553501; C++
      int GetPort () const;

      //## Operation: GetImplementation%-1922567987; C++
      ClTSAPImpl* GetImplementation ()
      {
        //## begin ClTSAP::GetImplementation%-1922567987.body preserve=yes
return m_poTSAPImpl;
        //## end ClTSAP::GetImplementation%-1922567987.body
      }

      //## Operation: IsConnected%-284801115; C++
      m4return_t IsConnected ()
      {
        //## begin ClTSAP::IsConnected%-284801115.body preserve=yes
 return m_bConectado;
        //## end ClTSAP::IsConnected%-284801115.body
      }

      //## Operation: SetConnected%783446814; C++
      void SetConnected ()
      {
        //## begin ClTSAP::SetConnected%783446814.body preserve=yes
 m_bConectado = M4_TRUE;
        //## end ClTSAP::SetConnected%783446814.body
      }

      //## Operation: ToString%-1942123768; C++
      char* ToString () const;

      //## Operation: Error%502218168; C++
      m4bool_t Error () const;

      //## Operation: GetError%52443838; C++
      int GetError () const;

      //## Operation: GetStringError%-1391374849; C++
      char* GetStringError () const;

      //## Operation: SetTSAPImplFactory%1300616214; C++
      static void SetTSAPImplFactory (ClTSAPImplFactory* ai_poImplFactory)
      {
        //## begin ClTSAP::SetTSAPImplFactory%1300616214.body preserve=yes
		m_s_poTSAPImplFactory = ai_poImplFactory;
	
        //## end ClTSAP::SetTSAPImplFactory%1300616214.body
      }

      //## Operation: SetId%1141040247; C++
      m4int32_t SetId (m4int32_t ai_lId);

      //## Operation: GetId%-901954985; C++
      m4int32_t GetId ();

      //## Operation: GetSystemDescriptor%2114005834; C++
      int GetSystemDescriptor ();

      //## Operation: GetRemoteIP%1432512714; C++
      m4bool_t GetRemoteIP (m4pchar_t ai_szAddress, int ai_iMaxLen);

      //## Operation: GetRemoteHostname%-1762171823; C++
      m4bool_t GetRemoteHostname (m4pchar_t ai_szAddress, int ai_iMaxLen);

      //## Operation: GetRemoteInetAddress%218218125; C++
      const ClInetAddress* GetRemoteInetAddress () const;

      //## Operation: Factory%1489073769; C++
      static ClTSAPImplFactory* Factory ()
      {
        //## begin ClTSAP::Factory%1489073769.body preserve=yes
		return m_s_poTSAPImplFactory;
	
        //## end ClTSAP::Factory%1489073769.body
      }

	  inline m4bool_t IsSSL()
	  {
		  return m_bTipoSSL;
	  }
  protected:
    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%377768440335
      //## Role: ClTSAP::m_poTSAPImpl%377768440337
      //## begin ClTSAP::m_poTSAPImpl%377768440337.role preserve=no  protected: ClTSAPImpl {1 -> 1RFHAPN}
      ClTSAPImpl *m_poTSAPImpl;
      //## end ClTSAP::m_poTSAPImpl%377768440337.role

      //## Association: M4Communication::M4CTL::<unnamed>%38E9C68601EB
      //## Role: ClTSAP::m_Mutex%38E9C68701CE
      //## begin ClTSAP::m_Mutex%38E9C68701CE.role preserve=no  protected: ClMutex { -> VHAN}
      ClMutex m_Mutex;
      //## end ClTSAP::m_Mutex%38E9C68701CE.role

      //## Association: M4Tsap::<unnamed>%37776844035E
      //## Role: ClTSAP::m_bError%377768440360
      //## begin ClTSAP::m_bError%377768440360.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bError;
      //## end ClTSAP::m_bError%377768440360.role

  private:
    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%37776844033A
      //## Role: ClTSAP::m_bConectado%37776844033C
      //## begin ClTSAP::m_bConectado%37776844033C.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bConectado;
      //## end ClTSAP::m_bConectado%37776844033C.role

      //## Association: M4Tsap::<unnamed>%377768440348
      //## Role: ClTSAP::m_s_poTSAPImplFactory%37776844034A
      //## begin ClTSAP::m_s_poTSAPImplFactory%37776844034A.role preserve=no  private: static ClTSAPImplFactory {1 -> 1RFHAPN}
      static ClTSAPImplFactory *m_s_poTSAPImplFactory;
      //## end ClTSAP::m_s_poTSAPImplFactory%37776844034A.role

      //## Association: M4Tsap::<unnamed>%37776844035B
      //## Role: ClTSAP::m_lId%37776844035D
      //## begin ClTSAP::m_lId%37776844035D.role preserve=no  private: m4int32_t {1 -> 1VHAPN}
      m4int32_t m_lId;
      //## end ClTSAP::m_lId%37776844035D.role

      //## Association: M4Tsap::<unnamed>%377768440361
      //## Role: ClTSAP::m_bTipoSSL%377768440364
      //## begin ClTSAP::m_bTipoSSL%377768440364.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bTipoSSL;
      //## end ClTSAP::m_bTipoSSL%377768440364.role

  private: //## implementation
};


// Bug 0130800
class M4_DECL_M4TSAP ClControlWriteTSAP : public ClTSAP
{
public:
	ClControlWriteTSAP();

	  //## Operation: Write%-824776902; C++
	int Write (const void* ai_pcSrc, int ai_iLen);

    int Read (ClTSAP *ai_poTSAP, void* ao_pcDst, int ai_iLen, m4bool_t ai_bTotal = M4_FALSE);

protected:

	m4bool_t m_bIsSet;

};

class M4_DECL_M4TSAP ClControlReadTSAP : public ClTSAP
{
public:
    ClControlReadTSAP (int ai_iMode, void* ai_pData);

	ClControlReadTSAP();

    int Read (void* ao_pcDst, int ai_iLen, m4bool_t ai_bTotal = M4_FALSE);

	void SetWriteTSAP(ClControlWriteTSAP *ai_poWriteTSAP);

protected:

	ClControlWriteTSAP *m_poWriteTSAP;

};

//## Class: ClServerTSAP%377768440365
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%3777684403F0;ClTSAP { -> }

class M4_DECL_M4TSAP ClServerTSAP 
{
  public:
    //## Constructors (specified)
      //## Operation: ClServerTSAP%1113354433; C++
      ClServerTSAP (int ai_iPort, m4bool_t ai_bStream = M4_TRUE, int ai_iBacklog = 64);

      //## Operation: ClServerTSAP%-1603296603; C++
      ClServerTSAP (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream = M4_TRUE, int ai_bBacklog = 64);

      //## Operation: ClServerTSAP%-326820959; C++
      ClServerTSAP (int ai_iMode, void* ai_pData, int ai_iPort, m4bool_t ai_bStream = M4_TRUE, int ai_iBacklog = 64);

    //## Destructor (specified)
      //## Operation: ~ClServerTSAP%-1476029477; C++
      ~ClServerTSAP ();


    //## Other Operations (specified)
      //## Operation: Close%-76419223; C++
      void Close ();

      //## Operation: GetLocalPort%1085902291; C++
      int GetLocalPort () const;

      //## Operation: ToString%1569814343; C++
      char* ToString () const;

      //## Operation: Error%-496976369; C++
      m4bool_t Error () const;

      //## Operation: GetError%1686994143; C++
      int GetError () const;

      //## Operation: GetStringError%-626908055; C++
      char* GetStringError () const;

      //## Operation: Accept%-12515548; C++
      ClTSAP* Accept (m4bool_t ai_bInitialize = M4_FALSE);

      // Bug 0130800
      ClTSAP* AcceptControl (m4bool_t ai_bInitialize = M4_FALSE);

      //## Operation: GetInetAddress%531080583; C++
      const ClInetAddress* GetInetAddress () const;

      //## Operation: GetSystemDescriptor%939982877
      int GetSystemDescriptor ();

  protected:
    // Data Members for Class Attributes

      //## Attribute: m_iBackLog%3777684403F5
      //## begin ClServerTSAP::m_iBackLog%3777684403F5.attr preserve=no  protected: int {VA} 
      int m_iBackLog;
      //## end ClServerTSAP::m_iBackLog%3777684403F5.attr

    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%3777684403F6
      //## Role: ClServerTSAP::m_poInetAddress%3777684403F8
      //## begin ClServerTSAP::m_poInetAddress%3777684403F8.role preserve=no  protected: ClInetAddress {1 -> 1RHAPN}
      ClInetAddress *m_poInetAddress;
      //## end ClServerTSAP::m_poInetAddress%3777684403F8.role

      //## Association: M4Tsap::<unnamed>%377881EA03DC
      //## Role: ClServerTSAP::m_bStream%377881EB022F
      //## begin ClServerTSAP::m_bStream%377881EB022F.role preserve=no  protected: m4bool_t {1 -> 1UHAN}
      m4bool_t m_bStream;
      //## end ClServerTSAP::m_bStream%377881EB022F.role

  private:
    // Data Members for Class Attributes

      //## Attribute: m_iMode%377768440402
      //## begin ClServerTSAP::m_iMode%377768440402.attr preserve=no  private: int {VA} 
      int m_iMode;
      //## end ClServerTSAP::m_iMode%377768440402.attr

      //## Attribute: m_pData%377768440403
      //## begin ClServerTSAP::m_pData%377768440403.attr preserve=no  private: void* {VA} 
      void* m_pData;
      //## end ClServerTSAP::m_pData%377768440403.attr

    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%3777684403E6
      //## Role: ClServerTSAP::m_poTSAPImpl%3777684403E8
      //## begin ClServerTSAP::m_poTSAPImpl%3777684403E8.role preserve=no  private: ClTSAPImpl {1 -> 1RFHAPN}
      ClTSAPImpl *m_poTSAPImpl;
      //## end ClServerTSAP::m_poTSAPImpl%3777684403E8.role

      //## Association: M4Tsap::<unnamed>%3777684403ED
      //## Role: ClServerTSAP::m_bError%3777684403EF
      //## begin ClServerTSAP::m_bError%3777684403EF.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bError;
      //## end ClServerTSAP::m_bError%3777684403EF.role

      //## Association: M4Tsap::<unnamed>%3777684403FC
      //## Role: ClServerTSAP::oImpl%3777684403FE
      //## begin ClServerTSAP::oImpl%3777684403FE.role preserve=no  private: ClTSAPImpl {1 -> 1RFHAPN}
      ClTSAPImpl *oImpl;
      //## end ClServerTSAP::oImpl%3777684403FE.role

      //## Association: M4Tsap::<unnamed>%3777684403FF
      //## Role: ClServerTSAP::m_bSSLMode%377768440401
      //## begin ClServerTSAP::m_bSSLMode%377768440401.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bSSLMode;
      //## end ClServerTSAP::m_bSSLMode%377768440401.role

  private: //## implementation
};

//## Class: ClTSAPSelect%377768440404
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37776845001C;m4bool_t { -> }
//## Uses: <unnamed>%377768450026;ClTSAP { -> }
//## Uses: <unnamed>%37776845002C;m4return_t { -> }
//## Uses: <unnamed>%377768450043;m4int8_t { -> }

class M4_DECL_M4TSAP ClTSAPSelect 
{
  public:
    //## Constructors (specified)
      //## Operation: ClTSAPSelect%289367655; C++
      ClTSAPSelect (m4bool_t ai_bUseMutex = M4_TRUE);

    //## Destructor (specified)
      //## Operation: ~ClTSAPSelect%1272558795; C++
      virtual ~ClTSAPSelect ();


    //## Other Operations (specified)
      //## Operation: Add%-864637872; C++
      //	Añade un TSAP a la lista del select, especificando con
      //	booleanos si se trata de un socket en el que se pretende
      //	atender la lectura, escritura y/o especial.
      void Add (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite, m4bool_t ai_bExceptions);

      //## Operation: Remove%-2076839024; C++
      m4return_t Remove (ClTSAP* ai_poTSAP);
      m4return_t Remove (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite,m4bool_t ai_bExceptions);

      //## Operation: Select%-2055469014; C++
      //	Cuando recibe como parámetros 0 segundos y 0
      //	microsegundos, no tiene tiempo máximo de espera, es
      //	decir, devuelve control únicamente cuando en alguno de
      //	los TSAPs de su lista se cumple la condición que se
      //	testea.
      //	Los parámetros booleanos indican si se quieren testear
      //	las condiciones de lectura, escritura y/o excepciones.
      //	Es decir, si uso Add para añadir un TSAP en modo lectura
      //	y escritura, y uso Select con parámetros booleanos
      //	M4_TRUE, M4_FALSE y M4_FALSE, Select sólo testea si el
      //	TSAP tiene algo para leer.
      int Select (unsigned long ai_Segundos = 0L, unsigned long ai_Microsegundos = 0L, m4bool_t ai_bRead = M4_TRUE, m4bool_t ai_bWrite = M4_TRUE, m4bool_t ai_bExceptions = M4_TRUE);

      //## Operation: NextRead%1656925455; C++
      ClTSAP* NextRead ();

      //## Operation: NextWrite%312378509; C++
      ClTSAP* NextWrite ();

      //## Operation: NextExceptions%-318900140; C++
      ClTSAP* NextExceptions ();

      //## Operation: IsInRead%2121007519; C++
      m4bool_t IsInRead (ClTSAP* ai_poTSAP);

      //## Operation: IsInWrite%987484626; C++
      m4bool_t IsInWrite (ClTSAP* ai_poTSAP);

      //## Operation: IsInExceptions%1391337547; C++
      m4bool_t IsInExceptions (ClTSAP* ai_poTSAP);

      //## Operation: IsInMask%1431804889; C++
      m4bool_t IsInMask (ClTSAP* ai_poTSAP, m4int8_t ai_iTipoMascara = IS_IN_READ);

  protected:
    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%377768450047
      //## Role: ClTSAPSelect::m_poMutex%377768450049
      //## begin ClTSAPSelect::m_poMutex%377768450049.role preserve=no  protected: ClMutex {1 -> 1RFHAPN}
      ClMutex *m_poMutex;
      //## end ClTSAPSelect::m_poMutex%377768450049.role

  private:
    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%37776845004A
      //## Role: ClTSAPSelect::m_poTSAPSelectImpl%37776845004C
      //## begin ClTSAPSelect::m_poTSAPSelectImpl%37776845004C.role preserve=no  private: ClTSAPSelectImpl {1 -> 1RFHAPN}
      ClTSAPSelectImpl *m_poTSAPSelectImpl;
      //## end ClTSAPSelect::m_poTSAPSelectImpl%37776845004C.role

  private: //## implementation
};

//## Class: ClServerTSAPSelectable%37776845004D
//## Category: M4Tsap%3777684001C4
//## Subsystem: M4Tsap::inc%37776840008D
//## Persistence: Transient
//## Cardinality/Multiplicity: n

//## Uses: <unnamed>%37776845009A;ClTSAP { -> }
//## Uses: <unnamed>%3777684500A3;m4return_t { -> }

class M4_DECL_M4TSAP ClServerTSAPSelectable : public ClTSAP  //## Inherits: <unnamed>%3777684500A5
{
  public:
    //## Constructors (specified)
      //## Operation: ClServerTSAPSelectable%1968226705; C++
      ClServerTSAPSelectable (int ai_iPort, m4bool_t ai_bStream = M4_TRUE, int ai_iBacklog = 64);

      //## Operation: ClServerTSAPSelectable%713160224; C++
      ClServerTSAPSelectable (ClInetAddress* ai_poInetAddress, int ai_iPort, m4bool_t ai_bStream = M4_TRUE, int ai_bBacklog = 64);
      ClServerTSAPSelectable (int ai_iMode, void* ai_pData, int ai_iPort, m4bool_t ai_bStream = M4_TRUE, int ai_iBacklog = 64);


    //## Other Operations (specified)
      //## Operation: Accept%1231405844; C++
      ClTSAP* Accept ();

      //## Operation: Listen%610999058; C++
      m4return_t Listen ();

  protected:
  private:
    // Data Members for Class Attributes

      //## Attribute: m_iBackLog%37776845009C
      //## begin ClServerTSAPSelectable::m_iBackLog%37776845009C.attr preserve=no  private: int {VA} 
      int m_iBackLog;
      int m_iMode;
      void* m_pData;
	  m4bool_t	m_bSSL;
      //## end ClServerTSAPSelectable::m_iBackLog%37776845009C.attr

    // Data Members for Associations

      //## Association: M4Tsap::<unnamed>%37776845009D
      //## Role: ClServerTSAPSelectable::m_poInetAddress%37776845009F
      //## begin ClServerTSAPSelectable::m_poInetAddress%37776845009F.role preserve=no  private: ClInetAddress {1 -> 1RHAPN}
      ClInetAddress *m_poInetAddress;
      //## end ClServerTSAPSelectable::m_poInetAddress%37776845009F.role

      //## Association: M4Tsap::<unnamed>%3777684500A0
      //## Role: ClServerTSAPSelectable::m_bStream%3777684500A2
      //## begin ClServerTSAPSelectable::m_bStream%3777684500A2.role preserve=no  private: m4bool_t {1 -> 1VHAPN}
      m4bool_t m_bStream;
      //## end ClServerTSAPSelectable::m_bStream%3777684500A2.role

  private: //## implementation
};

//## begin module%3777684302B8.epilog preserve=yes
//## end module%3777684302B8.epilog


#endif
