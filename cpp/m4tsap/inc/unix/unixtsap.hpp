#ifndef _UnixTSAP_hpp
#define _UnixTSAP_hpp

#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#include "m4types.hpp"
#include "m4trace.hpp"
#include "tsap.hpp"
#include "tsapimpl.hpp"
#include "selectimpl.hpp"
#include "tsapimplfactory.hpp"
#include "ssltsapimpl.hpp"
#include "syncro.hpp"

#ifdef M4_POLL_IMPLEMENTATION
#include <poll.h>
#define	M4_READ			1
#define	M4_WRITE		2
#define	M4_EXCEPTIONS	4
#endif

class ClUnixTSAPImpl : public ClTSAPImpl {

public:

  int Accept(ClTSAPImpl* newTSAP);
  int Bind(ClInetAddress* src, int lport);
  void Close();
  int Connect(const char* hostname, int port);
  int Connect(ClInetAddress* dst, int port);
  int Create(m4bool_t stream = M4_TRUE);
  int Listen(int backlog);
  int Write(const void* src, int len);
  int Read(void* dst, int len);
  int SetBlockingIO(m4bool_t blocking = M4_TRUE);
   int GetSystemDescriptor(){return iSocket;}

protected:

  int iSocket;

};

class ClUnixSelectMask {

public:

  ClUnixSelectMask(unsigned int size = 0);
  ~ClUnixSelectMask();
  void Set(unsigned int n);
  void Clear(unsigned int n);
  m4bool_t IsSet(unsigned int n) const;
  int NextSet(unsigned int n = 0) const;
  void Zero();
  unsigned int GetSize() const;
  void Resize(unsigned int newsize = 0);
  void* GetMask();
  ClUnixSelectMask& operator=(const ClUnixSelectMask&);
  m4bool_t IsIn(unsigned int ai_iIdentificador);

private:

  unsigned int iMaskSize;
  fd_set	ulaMask;

};

#ifdef M4_POLL_IMPLEMENTATION
#define CHANGE_ADD		0
#define CHANGE_REMOVE	1

#define MAX_CHANGES		3000
typedef struct
{
	ClTSAP *pClTSAP; 
	int		iMode;
	int		iChange;			
}Changes_t;

class ClUnixPollFds {

public:

  ClUnixPollFds(int ai_iSize = 2048);
  ~ClUnixPollFds();
  void Set(ClTSAP* ai_pClTSAP, int ai_iMode);
  m4return_t Clear(ClTSAP* ai_pClTSAP, m4bool_t ai_bRead = M4_TRUE,m4bool_t ai_bWrite = M4_TRUE,m4bool_t ai_bExceptions = M4_TRUE);
  m4bool_t IsSet(ClTSAP* ai_pClTSAP, int ai_iMode) const;
  ClTSAP *NextSet(int ai_iMode);
  int GetSize() const;
  m4bool_t IsIn(ClTSAP* ai_pClTSAP, int ai_iMode);
  int Poll(ClMutex	*ai_poMutex, int ai_iMillisecs);

private:

  int	m_iPositionRead;
  int	m_iPositionWrite;
  int	m_iPositionExceptions;
  int	m_iSize;
  int	m_iNumFds;
  Changes_t	*m_pChanges;
  int	m_iChanges;
  struct	pollfd	*m_pPollFds;
  ClTSAP** m_pTSAPList;
  void Regenerate();
  void InternalSet(ClTSAP* ai_pClTSAP, int ai_iMode);
  void InternalClear(ClTSAP* ai_pClTSAP, int ai_iMode);
};
#endif

class ClUnixTSAPSelectImpl : public ClTSAPSelectImpl {

public:

  ClUnixTSAPSelectImpl(unsigned int nTSAPs = 0);
  void Add(ClTSAP* s, m4bool_t read, m4bool_t write, m4bool_t exceptions);
  m4return_t Remove(ClTSAP* s);
  m4return_t Remove (ClTSAP* ai_poTSAP, m4bool_t ai_bRead, m4bool_t ai_bWrite,m4bool_t ai_bExceptions);
  int Select(ClMutex *ao_pMutex, unsigned long, unsigned long, 
			m4bool_t ai_bRead = M4_TRUE, m4bool_t ai_bWrite = M4_TRUE, 
					m4bool_t i_bExceptions = M4_TRUE);
  ClTSAP* NextRead();
  ClTSAP* NextWrite();
  ClTSAP* NextExceptions();
  m4bool_t IsInMask (ClTSAP *ai_poTSAP, m4int8_t ai_iTipoMascara);

private:

#ifndef M4_POLL_IMPLEMENTATION
  ClUnixSelectMask* oMasterReadMask;
  ClUnixSelectMask* oMasterWriteMask;
  ClUnixSelectMask* oMasterExceptionsMask;
  ClUnixSelectMask* oReadMask;
  ClUnixSelectMask* oWriteMask;
  ClUnixSelectMask* oExceptionsMask;
  ClTSAP** oaTSAPList;
  unsigned int iNumTSAPs;
  unsigned int iNextRead;
  unsigned int iNextWrite;
  unsigned int iNextExceptions;
#else
  ClUnixPollFds		SetPollFds;
#endif

};

class ClUnixTSAPImplFactory : public ClTSAPImplFactory 
{

public:

  ClTSAPImpl* CreateTSAPImpl() 
  {

    return new ClUnixTSAPImpl;  
  }
  ClTSAPImpl* CreateTSAPSSLImpl(int ai_iMode, void *ai_pData)
  {
    return new ClSSLTSAPImpl(ai_iMode, ai_pData);
  }

  ClTSAPSelectImpl* CreateTSAPSelectImpl() {return new ClUnixTSAPSelectImpl;}
};


#endif // _UnixTSAP_hpp

