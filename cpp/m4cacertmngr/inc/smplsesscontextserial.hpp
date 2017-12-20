

#ifndef smplsesscontextserial_h
#define smplsesscontextserial_h 1


#include <sesscontextserial.hpp>
#include "m4stl.hpp"


class ClSimpleSessContextSerial : public ISessContextSerial 
{


  public:

      ClSimpleSessContextSerial();
	  ~ClSimpleSessContextSerial() ;
   
      m4return_t AddAtribute (m4string_t ai_strAtribute, m4string_t ai_strValue);
   
      m4return_t Serialize (m4pchar_t & ao_pBuffer, m4uint32_t & ao_bufLen);
     
      m4return_t Deserialize (m4pchar_t ai_pBuffer, m4uint32_t ai_iBufLen);

      m4return_t GetAtribute (m4string_t ai_strAtribute, m4string_t &ao_strValue);

	  SessionIterator GetIterator();


  protected:

	  map<m4string_t, m4string_t> m_oContextMap; 

  private:


};


class SimpleSessionIterator : public ISessionIterator {

public:
	SimpleSessionIterator();
	m4pcchar_t GetKey();
	m4pcchar_t GetValue();
	void Next();
	void Init (map<m4string_t, m4string_t> ai_oContextMap);

protected:
	void Reset();

private:
	  map<m4string_t, m4string_t> m_oContextMap; 	  
	  map <m4string_t, m4string_t>::iterator m_oContextIter;
	  m4int32_t m_iPos;
	  m4pcchar_t m_pcKey;
	  m4pcchar_t m_pcValue;
	  m4uint32_t m_iNRef;
};



#endif
