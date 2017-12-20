//## begin module%3739996A029F.cm preserve=no
//## end module%3739996A029F.cm

//## begin module%3739996A029F.cp preserve=no
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
//## end module%3739996A029F.cp

//## Module: parsehttp%3739996A029F; Package specification
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Source file: e:\source\m4appclient\inc\parsehttp.hpp

#ifndef parsehttp_h
#define parsehttp_h 1

//## begin module%3739996A029F.additionalIncludes preserve=no
//## end module%3739996A029F.additionalIncludes

//## begin module%3739996A029F.includes preserve=yes
#include <string.h>
#include <ctype.h>
#ifdef _WIN32 
//	#include <windows.h>
#ifndef min
	#define min(a,b) ((a)<(b) ? (a) : (b))
#endif
#else
	#define min(a,b) ((a)<(b) ? (a) : (b))
	#define max(a,b) ((a)>(b) ? (a) : (b))
#endif

#include "m4types.hpp"
//## end module%3739996A029F.includes

//## begin module%3739996A029F.declarations preserve=no
//## end module%3739996A029F.declarations

//## begin module%3739996A029F.additionalDeclarations preserve=yes
#define MAX_TWR_VALUES 50

// Definitions
#define ACTION_CONNECT       "connect"
#define ACTION_SENDDATA      "sendData"
#define ACTION_DISCONNECT    "disconnect"
#define ACTION_CANCEL		 "cancel"

// Limits for the token PKs and values...
#define MAX_TOKEN_KEY_SIZE       128
#define MAX_TOKEN_DATA_SIZE      256

#define MAX_PENDING_BUFFER_SIZE  (2*1024)

#define MAX_PENDING_TOKEN_ID_SIZE 256

#define CR_CHAR                   '\r'
#define LF_CHAR                   '\n'
#define CRLF_STRING               "\r\n"
#define END_OF_LINE_STRING        CRLF_STRING
#define END_OF_LINE_STRING_LENGTH 2

#define DEBUG 1 // Para que tenga info


// MAX LENGTHS
#define MAX_HOST_SIZE         256
#define MAX_SERVLET_SIZE      256
#define MAX_COOKIE_SIZE       256
#define MAX_ACTION_SIZE       256

// Used to define request strings...
#define MAX_POST_REQUEST_SIZE (256+MAX_HOST_SIZE+MAX_SERVLET_SIZE+MAX_COOKIE_SIZE+MAX_COOKIE_SIZE+MAX_ACTION_SIZE)

/*
#define CONTENT_LENGTH_HEADER           "Content-length"
#define CONTENT_LENGTH_HEADER_WP        "Content-length: "
#define OPERATION_TIMEOUT_HEADER        "operationTimeout"
#define OPERATION_TIMEOUT_HEADER_WP     "operationTimeout: "
#define COOKIE_HEADER                   "Cookie"
#define COOKIE_HEADER_WP                "Cookie: "
#define COOKIE_HEADER_WP_LENGTH 8
#define USE_SSL_HEADER                   "useSSL"
#define USE_SSL_HEADER_WP                "useSSL: "
#define USE_SSL_HEADER_WP_LENGTH 8
*/
#define CONTENT_LENGTH_HEADER           "Content-length"
#define CONTENT_LENGTH_HEADER_WP        "Content-length: "
#define OPERATION_TIMEOUT_ID            "operationTimeout"
#define OPERATION_TIMEOUT_ID_LENGTH     16
#define COOKIE_HEADER                   "Cookie"
#define COOKIE_HEADER_WP                "Cookie: "
#define COOKIE_HEADER_WP_LENGTH         8
#define PRIV_KEY_HEADER					"M4Key"
#define PRIV_KEY_HEADER_WP				"M4Key: "
#define PRIV_KEY_HEADER_WP_LENGTH		7
#define USE_SSL_ID                      "useSSL"
#define USE_SSL_ID_LENGTH               6
#define USE_SSL_ID_WEQ                  "useSSL="
#define USE_SSL_ID_WEQ_LENGTH           7
#define REQUEST_ID						"requestID"
#define REQUEST_ID_LENGHT				9
#define REQUEST_ID_WEQ					"requestID="
#define REQUEST_ID_WEQ_LENGHT			10
#define CANCEL_REQUEST_ID				"canceledRequestID"
#define CANCEL_REQUEST_ID_LENGTH		17		
#define CANCEL_REQUEST_ID_WEQ			"canceledRequestID="
#define CANCEL_REQUEST_ID_WEQ_LENGTH	18
#define USE_SSL_FALSE                   0
#define USE_SSL_TRUE                    1
#define USE_SSL_NO                      -1


// Parsing states (states)
#define HTTP_PS_HTTP_IDENTIFICATION_STR    0
#define HTTP_PS_HTTP_FIRST_VERSION_NUMBER  1
#define HTTP_PS_HTTP_SECOND_VERSION_NUMBER 2
#define HTTP_PS_NUMERIC_ERROR_CODE         3
#define HTTP_PS_STRING_ERROR               4
#define HTTP_PS_LOOKING_FOR_LF_CHAR1       5
#define HTTP_PS_LOOKING_FOR_TOKEN_ID       6
#define HTTP_PS_LOOKING_FOR_LF_CHAR2       7
#define HTTP_PS_GETTING_TOKEN_DATA         8
#define HTTP_PS_VERIFYING_BLANK_LINE       9  // Loking for the end of headers
#define HTTP_PS_IN_THE_BODY                10

#define HTTP_END_OF_TOKEN_ID               ':'

#define HTTP_INVALID_ERROR                 (-99)

// Return codes from HTTP
#define HTTP_ERROR_CODE_OK                 200

#define HTTP_IDENTIFIER_STRING            "HTTP/"
#define HTTP_IDENTIFIER_STRING_SIZE       5

// For a limited use, of course...
#define MAX_TOKENS                        16

// Maximum of the error description (HTTP level)
#define MAX_HTTP_ERROR_STRING             256


// Returns of Parse
#define HTTP_PARSE_BUFFER_SIZE_ERROR         -4 // Buffer too long before the body
#define HTTP_PARSE_HTTP_ERROR                -3 // Invalid from the HTTP level
#define HTTP_PARSE_END_OF_HEADERS            -2 // End of the headers (not all found)
#define HTTP_PARSE_ERROR                     -1 // For more info, get it from the details
#define HTTP_PARSE_CONTINUE                  0  // Try it again...
#define HTTP_PARSE_ALL_THE_HEADERS_FOUND     1  // All the tokens specified were found
                                                // (and the next position is correctly
                                                // found)

m4return_t BuildHTTPPOSTRequest(m4pcchar_t  ai_pzHost,
								        m4uint32_t  ai_iPort,
									     m4pcchar_t  ai_pzServlet,
									     m4uint32_t  ai_iContentLength,
									     m4pcchar_t  ai_pzPrivCookie, // Empty string if not needed
										 m4pcchar_t  ai_pzPublicCookie, // Empty string if not needed
									     m4pcchar_t  ai_pzAction,
                                m4uint32_t  ai_iTimeout, // in msec
                                m4int32_t   ai_iUseSSL, // Trtue, False or Nothing
                                m4pchar_t   ao_pzResult,
                                m4uint32_t& ao_iResultSize);

m4return_t BuildReConnectRequest(m4pcchar_t  ai_pzHost,
								 m4uint32_t  ai_pzPort,
								 m4pcchar_t  ai_pzServlet,
								 m4uint32_t  ai_iContentLength,
								 m4pcchar_t  ai_pzPrivCookie,
								 m4pcchar_t  ai_pzPublicCookie,
                                 m4uint32_t  ai_iTimeout, // in msec
								 m4bool_t    ai_bUseSSL, // True or False
                                 m4pchar_t   ao_pzResult,
                                 m4uint32_t& ao_iResultSize);

m4return_t BuildConnectRequest(m4pcchar_t  ai_pzHost,
								       m4uint32_t  ai_pzPort,
									    m4pcchar_t  ai_pzServlet,
									    m4uint32_t  ai_iContentLength,
                               m4uint32_t  ai_iTimeout, // in msec
                               m4bool_t    ai_bUseSSL, // True or False
                               m4pchar_t   ao_pzResult,
                               m4uint32_t& ao_iResultSize);


m4return_t BuildSendDataRequest(m4pcchar_t  ai_pzServlet,
									     m4uint32_t  ai_iContentLength,
									     m4pcchar_t  ai_pzPrivCookie,
										 m4pcchar_t  ai_pzPublicCookie,
                                m4uint32_t  ai_iTimeout, // in msec
                                m4pchar_t   ao_pzResult,
                                m4uint32_t& ao_iResultSize);
m4return_t BuildDisconnectRequest(m4pcchar_t  ai_pzServlet,
									       m4uint32_t  ai_iContentLength,
									     m4pcchar_t  ai_pzPrivCookie,
										 m4pcchar_t  ai_pzPublicCookie,
                                  m4uint32_t  ai_iTimeout, // in msec
                                  m4pchar_t   ao_pzResult,
                                  m4uint32_t& ao_iResultSize);



//## end module%3739996A029F.additionalDeclarations


//## begin ClHTTPParser%373991F201CD.preface preserve=yes
//## end ClHTTPParser%373991F201CD.preface

//## Class: ClHTTPParser%373991F201CD
//## Category: M4AppClient%34E17F280252
//## Subsystem: M4AppClient::inc%3796F8B303AC
//## Persistence: Transient
//## Cardinality/Multiplicity: n

class ClHTTPParser 
{
  //## begin ClHTTPParser%373991F201CD.initialDeclarations preserve=yes
  //## end ClHTTPParser%373991F201CD.initialDeclarations

  public:
    //## begin ClHTTPParser::TokenWithResults%373991F202D5.preface preserve=yes
    //## end ClHTTPParser::TokenWithResults%373991F202D5.preface

    //## Class: TokenWithResults%373991F202D5
    //## Category: M4AppClient%34E17F280252
    //## Subsystem: M4AppClient::inc%3796F8B303AC
    //## Persistence: Transient
    //## Cardinality/Multiplicity: n

    class TokenWithResults 
    {
      //## begin ClHTTPParser::TokenWithResults%373991F202D5.initialDeclarations preserve=yes
      //## end ClHTTPParser::TokenWithResults%373991F202D5.initialDeclarations

      public:
        // Additional Public Declarations
          //## begin ClHTTPParser::TokenWithResults%373991F202D5.public preserve=yes
           TokenWithResults(void)
            { 
               m_pzId=NULL;

               for (m4uint32_t iAux=0; iAux<MAX_TWR_VALUES; iAux++)
                  m_pzValue[iAux]=NULL; 
            }

            m4return_t Reset(void) 
            { 
               if (m_pzId!=NULL)
               {
                  delete m_pzId;

				  m_pzId=NULL;
               }

               ResetValues();

               return M4_SUCCESS;
            }

            m4return_t ResetValues(void) 
            { 
               for (m4uint32_t iAux=0; iAux<MAX_TWR_VALUES; iAux++)
                  if (m_pzValue[iAux]!=NULL)
                  {
				      delete m_pzValue[iAux];

					  m_pzValue[iAux]=NULL;
                  }
				
               return M4_SUCCESS;
            }

            ~TokenWithResults(void) 
            {
               Reset();
            }

            m4return_t SetId(m4pcchar_t ai_pzId)
            {
               if (m_pzId!=NULL)
                  delete m_pzId;

               m_pzId=new char[strlen(ai_pzId)+1];

               if (m_pzId==NULL)
                  return M4_ERROR;

               strcpy(m_pzId,ai_pzId);

               return M4_SUCCESS;
            }

            m4return_t AddValue(m4pcchar_t ai_pzValue)
            {
               for (m4uint32_t iAux=0; iAux<MAX_TWR_VALUES; iAux++)
                  if (m_pzValue[iAux]==NULL)
                  {
                     m_pzValue[iAux]=new char[strlen(ai_pzValue)+1];

                     if (m_pzValue[iAux]==NULL)
                        return M4_ERROR;

                     strcpy(m_pzValue[iAux],ai_pzValue);

                     return M4_SUCCESS;
                  }

               return M4_ERROR;
            }

            m4return_t GetId(m4pchar_t ao_pzId)
            {
               if (m_pzId==NULL)
                  return M4_WARNING; // Is null
               else
               {
                  strcpy(ao_pzId,m_pzId);

                  return M4_SUCCESS;
               }
            }

            m4return_t GetValue(m4pchar_t  ao_pzValue,
                                m4uint32_t ai_iIndex=0)
            {
               if (m_pzValue[ai_iIndex]==NULL)
                  return M4_WARNING; // Is null
               else
               {
                  strcpy(ao_pzValue,m_pzValue[ai_iIndex]);

                  return M4_SUCCESS;
               }
            }

          //## end ClHTTPParser::TokenWithResults%373991F202D5.public
      protected:
        // Additional Protected Declarations
          //## begin ClHTTPParser::TokenWithResults%373991F202D5.protected preserve=yes
          //## end ClHTTPParser::TokenWithResults%373991F202D5.protected

      private:
        // Additional Private Declarations
          //## begin ClHTTPParser::TokenWithResults%373991F202D5.private preserve=yes
			m4pchar_t m_pzId;
            m4char_t* m_pzValue[MAX_TWR_VALUES];
          //## end ClHTTPParser::TokenWithResults%373991F202D5.private
      private: //## implementation
        // Additional Implementation Declarations
          //## begin ClHTTPParser::TokenWithResults%373991F202D5.implementation preserve=yes
          //## end ClHTTPParser::TokenWithResults%373991F202D5.implementation

    };

    //## begin ClHTTPParser::TokenWithResults%373991F202D5.postscript preserve=yes
    //## end ClHTTPParser::TokenWithResults%373991F202D5.postscript

    //## Constructors (specified)
      //## Operation: ClHTTPParser%-301252683; C++
      ClHTTPParser (void );

    //## Destructor (specified)
      //## Operation: ~ClHTTPParser%-1403232529; C++
      ~ClHTTPParser (void );


    //## Other Operations (specified)
      //## Operation: GetEndOfHeaders%363925001; C++
      //	Have we met the end of the header ?
      m4bool_t GetEndOfHeaders (void );

      //## Operation: GetEndOfHeadersAddress%1286452346; C++
      //	Have we met the end of the header ?
      m4pcchar_t GetEndOfHeadersAddress (void );

      //## Operation: AddNewTokenToLookFor%1971719787; C++
      //	We hope that the token wasn't already added
      m4return_t AddNewTokenToLookFor (m4pcchar_t ai_pzNewToken);

      //## Operation: DeleteTokenToLookFor%1763134730; C++
      m4return_t DeleteTokenToLookFor (m4pcchar_t ai_pzToken);

      //## Operation: GetTokenResult%83523101; C++
      //	For getting the first line...
      //	Only 200 is interpreted as a good return value. Any other value aborts the
      //	processing..
      m4return_t GetTokenResult (m4pcchar_t ai_pzToken, m4pchar_t ao_pzData, m4uint32_t ai_iIndex = 0);

      //## Operation: ThereIsAnyFirstLineResult%1255519494; C++
      m4bool_t ThereIsAnyFirstLineResult (void );

      //## Operation: GetFirstLineResult%-1565266923; C++
      //	Numeric one
      m4return_t GetFirstLineResult (m4uint32_t& ao_iResult);

      //## Operation: GetFirstLineResult%-1855119664; C++
      //	String, if any
      m4return_t GetFirstLineResult (m4pchar_t ao_pzResult);

      //## Operation: SetHeadersSizeLimit%-1983018110; C++
      void SetHeadersSizeLimit (m4uint32_t ai_iSizeLimit);

      //## Operation: Parse%193691917; C++
      m4int32_t Parse (m4pcchar_t ai_pzBuffer, m4uint32_t ai_iBufferSize);

      //## Operation: ResetParse%-128425510; C++
      //	A new parsing ...
      void ResetParse (m4int32_t ai_iNewHeaderSizeLimit = -1);

      //## Operation: ResetTokenList%-1250033715; C++
      void ResetTokenList (void );

      //## Operation: ResetTokenResults%376502623; C++
      //	Only the results
      void ResetTokenResults (void );

      //## Operation: ResetParseAndTokenResults%884729305; C++
      void ResetParseAndTokenResults (void );

  public:
    // Additional Public Declarations
      //## begin ClHTTPParser%373991F201CD.public preserve=yes
      //## end ClHTTPParser%373991F201CD.public

  protected:
    // Additional Protected Declarations
      //## begin ClHTTPParser%373991F201CD.protected preserve=yes
      //## end ClHTTPParser%373991F201CD.protected

  private:

    //## Other Operations (specified)
      //## Operation: InitParse%1094424258; C++
      //	Inititates the first parsing
      void InitParse (void );

      //## Operation: InitTokenLists%1683790101; C++
      //	Returns the index
      void InitTokenLists (void );

      //## Operation: FindTokenById%254489228; C++
      //	Returns the index or -1 if an error occurs
      m4int32_t FindTokenById (m4pcchar_t ai_pzToken);

    // Additional Private Declarations
      //## begin ClHTTPParser%373991F201CD.private preserve=yes
      //## end ClHTTPParser%373991F201CD.private

  private: //## implementation
    // Additional Implementation Declarations
      //## begin ClHTTPParser%373991F201CD.implementation preserve=yes
      m4uint16_t  m_iState;

      // To control the pending buffer (when needed)
      m4uint32_t  m_iPendingPointer;
      m4char_t    m_pPendingBuffer[MAX_PENDING_BUFFER_SIZE];

      // The last token ID used
      m4char_t    m_pPendingTokenID[MAX_PENDING_TOKEN_ID_SIZE];

      // Errors returned by the HTTP level
      m4int32_t   m_iHTMErrorCode;
      m4char_t    m_pzHTMErrorString[MAX_HTTP_ERROR_STRING];

      m4uint16_t  m_iNumberOfTokens;
      TokenWithResults *m_Token[MAX_TOKENS];

      m4pchar_t   m_EndOfHeaders;

      m4uint32_t  m_iSizeLimit;
      m4uint32_t  m_iHeaderSize;

      //## end ClHTTPParser%373991F201CD.implementation
};

//## begin ClHTTPParser%373991F201CD.postscript preserve=yes
//## end ClHTTPParser%373991F201CD.postscript

// Class ClHTTPParser::TokenWithResults 

// Class ClHTTPParser 


//## Other Operations (inline)
inline m4bool_t ClHTTPParser::ThereIsAnyFirstLineResult (void )
{
  //## begin ClHTTPParser::ThereIsAnyFirstLineResult%1255519494.body preserve=yes
 return (m_iHTMErrorCode!=HTTP_INVALID_ERROR)?M4_TRUE:M4_FALSE;
  //## end ClHTTPParser::ThereIsAnyFirstLineResult%1255519494.body
}

inline void ClHTTPParser::SetHeadersSizeLimit (m4uint32_t ai_iSizeLimit)
{
  //## begin ClHTTPParser::SetHeadersSizeLimit%-1983018110.body preserve=yes
         m_iSizeLimit=ai_iSizeLimit;
      
  //## end ClHTTPParser::SetHeadersSizeLimit%-1983018110.body
}

//## begin module%3739996A029F.epilog preserve=yes
//## end module%3739996A029F.epilog


#endif
