//## begin module%3739996C01E3.cm preserve=no
//## end module%3739996C01E3.cm

//## begin module%3739996C01E3.cp preserve=yes
//	=========================================================
//	=====================
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            M4AppClient
//	 File:              parsehttp.cpp
//	 Project:           M4ComSrv
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
//	=========================================================
//	====================
//## end module%3739996C01E3.cp

//## Module: parsehttp%3739996C01E3; Package body
//## Subsystem: M4AppClient::src%3796F8B303AD
//## Source file: e:\source\m4appclient\src\parsehttp.cpp

//## begin module%3739996C01E3.additionalIncludes preserve=no
//## end module%3739996C01E3.additionalIncludes

//## begin module%3739996C01E3.includes preserve=yes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <m4string.hpp>
//## end module%3739996C01E3.includes

// parsehttp
#include <parsehttp.hpp>
//## begin module%3739996C01E3.declarations preserve=no
//## end module%3739996C01E3.declarations

//## begin module%3739996C01E3.additionalDeclarations preserve=yes

// Used to build a POST request...
// All input data, except for the las one, where I put the result
// I am assuming that the neccesary amonut of space is reserved on the Result string
// The resulting string is not zero terminated...
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
                                m4uint32_t& ao_iResultSize)
{
   // Verify the lengths
   if (strlen(ai_pzHost)>MAX_HOST_SIZE)
      return M4_ERROR;
   if (strlen(ai_pzServlet)>MAX_SERVLET_SIZE)
      return M4_ERROR;
   if (strlen(ai_pzPublicCookie)>MAX_COOKIE_SIZE)
      return M4_ERROR;
   if (strlen(ai_pzPrivCookie)>MAX_COOKIE_SIZE)
      return M4_ERROR;
   if (strlen(ai_pzAction)>MAX_ACTION_SIZE)
      return M4_ERROR;

   if (ai_pzHost[0]!=0)
      ao_iResultSize=sprintf(ao_pzResult,"POST %s?action=%s&server=%s&port=%d&%s=%d",
                             ai_pzServlet,
                             ai_pzAction,
                             ai_pzHost,
								     ai_iPort,
                             OPERATION_TIMEOUT_ID,
                             ai_iTimeout);
   else
      ao_iResultSize=sprintf(ao_pzResult,"POST %s?action=%s&%s=%d",
                             ai_pzServlet,
                             ai_pzAction,
                             OPERATION_TIMEOUT_ID,
                             ai_iTimeout);

   if (ai_iUseSSL!=USE_SSL_NO)
   {
      ao_pzResult[ao_iResultSize]='&';
      ao_iResultSize++;
      memcpy(&ao_pzResult[ao_iResultSize],USE_SSL_ID_WEQ,USE_SSL_ID_WEQ_LENGTH);
      ao_iResultSize+=USE_SSL_ID_WEQ_LENGTH;

      if (ai_iUseSSL==USE_SSL_TRUE)
      {
         memcpy(&ao_pzResult[ao_iResultSize],"true",4);
         ao_iResultSize+=4;
      }
      else
      {
         memcpy(&ao_pzResult[ao_iResultSize],"false",5);
         ao_iResultSize+=5;
      }
   }

   // Rest of 1st line
   m4uint32_t iAuxSize;

   iAuxSize=sprintf(&ao_pzResult[ao_iResultSize]," HTTP/1.0%s%s%d%s",
                                                 END_OF_LINE_STRING,
                                                 CONTENT_LENGTH_HEADER_WP,
                                                 ai_iContentLength,
                                                 END_OF_LINE_STRING);

   ao_iResultSize+=iAuxSize;

   if (ai_pzPrivCookie[0]!=0)
   {
      m4uint32_t iCookieLength;
      iCookieLength=strlen(ai_pzPrivCookie);

      memcpy(&ao_pzResult[ao_iResultSize],PRIV_KEY_HEADER_WP,PRIV_KEY_HEADER_WP_LENGTH);
      ao_iResultSize+=PRIV_KEY_HEADER_WP_LENGTH;

      memcpy(&ao_pzResult[ao_iResultSize],ai_pzPrivCookie,iCookieLength);

      ao_iResultSize+=iCookieLength;

      memcpy(&ao_pzResult[ao_iResultSize],END_OF_LINE_STRING,END_OF_LINE_STRING_LENGTH);

      ao_iResultSize+=2;
   }

   if (ai_pzPublicCookie[0]!=0)
   {
      m4uint32_t iCookieLength;
      iCookieLength=strlen(ai_pzPublicCookie);

      memcpy(&ao_pzResult[ao_iResultSize],COOKIE_HEADER_WP,COOKIE_HEADER_WP_LENGTH);
      ao_iResultSize+=COOKIE_HEADER_WP_LENGTH;

      memcpy(&ao_pzResult[ao_iResultSize],ai_pzPublicCookie,iCookieLength);

      ao_iResultSize+=iCookieLength;

      memcpy(&ao_pzResult[ao_iResultSize],END_OF_LINE_STRING,END_OF_LINE_STRING_LENGTH);

      ao_iResultSize+=2;
   }

   memcpy(&ao_pzResult[ao_iResultSize],END_OF_LINE_STRING,END_OF_LINE_STRING_LENGTH);

   ao_iResultSize+=2;
/*
   if (ai_pzHost[0]!=0)
      ao_iResultSize=sprintf(ao_pzResult,"POST %s?action=%s&server=%s&port=%d HTTP/1.0%s%s%d%s%s%d%s",
                             ai_pzServlet,
                             ai_pzAction,
                             ai_pzHost,
								     ai_iPort,
                             END_OF_LINE_STRING,
                             OPERATION_TIMEOUT_HEADER_WP,
                             ai_iTimeout,
                             END_OF_LINE_STRING,
                             CONTENT_LENGTH_HEADER_WP,
                             ai_iContentLength,
                             END_OF_LINE_STRING);
   else
      ao_iResultSize=sprintf(ao_pzResult,"POST %s?action=%s HTTP/1.0%s%s%d%s%s%d%s",
                             ai_pzServlet,
                             ai_pzAction,
                             END_OF_LINE_STRING,
                             OPERATION_TIMEOUT_HEADER_WP,
                             ai_iTimeout,
                             END_OF_LINE_STRING,
                             CONTENT_LENGTH_HEADER_WP,
                             ai_iContentLength,
                             END_OF_LINE_STRING);

   if (ai_iUseSSL!=USE_SSL_NO)
   {
      memcpy(&ao_pzResult[ao_iResultSize],USE_SSL_HEADER_WP,USE_SSL_HEADER_WP_LENGTH);
      ao_iResultSize+=USE_SSL_HEADER_WP_LENGTH;

      if (ai_iUseSSL==USE_SSL_TRUE)
      {
         memcpy(&ao_pzResult[ao_iResultSize],"true",4);
         ao_iResultSize+=4;
      }
      else
      {
         memcpy(&ao_pzResult[ao_iResultSize],"false",5);
         ao_iResultSize+=5;
      }

      memcpy(&ao_pzResult[ao_iResultSize],END_OF_LINE_STRING,END_OF_LINE_STRING_LENGTH);

      ao_iResultSize+=2;
   }

   if (ai_pzCookie[0]!=0)
   {
      m4uint32_t iCookieLength;
      iCookieLength=strlen(ai_pzCookie);

      memcpy(&ao_pzResult[ao_iResultSize],COOKIE_HEADER_WP,COOKIE_HEADER_WP_LENGTH);
      ao_iResultSize+=COOKIE_HEADER_WP_LENGTH;

      memcpy(&ao_pzResult[ao_iResultSize],ai_pzCookie,iCookieLength);

      ao_iResultSize+=iCookieLength;

      memcpy(&ao_pzResult[ao_iResultSize],END_OF_LINE_STRING,END_OF_LINE_STRING_LENGTH);

      ao_iResultSize+=2;
   }

   memcpy(&ao_pzResult[ao_iResultSize],END_OF_LINE_STRING,END_OF_LINE_STRING_LENGTH);

   ao_iResultSize+=2;
*/
   // Por ahora, para test
   #ifdef DEBUG
   ao_pzResult[ao_iResultSize]=0;
   #endif

   return M4_SUCCESS;
}

m4return_t BuildConnectRequest(	m4pcchar_t  ai_pzHost,
							   m4uint32_t  ai_pzPort,
							   m4pcchar_t  ai_pzServlet,
							   m4uint32_t  ai_iContentLength,
							   m4uint32_t  ai_iTimeout, // in msec
							   m4bool_t    ai_bUseSSL, // True or False
							   m4pchar_t   ao_pzResult,
							   m4uint32_t& ao_iResultSize)
{
	return BuildHTTPPOSTRequest(ai_pzHost,
								ai_pzPort,
								ai_pzServlet,
								ai_iContentLength,
								"",
								"",
								ACTION_CONNECT,
								ai_iTimeout,
								ai_bUseSSL==M4_TRUE ? USE_SSL_TRUE : USE_SSL_FALSE,
								ao_pzResult,
								ao_iResultSize);
}

m4return_t BuildReConnectRequest(m4pcchar_t  ai_pzHost,
								 m4uint32_t  ai_pzPort,
								 m4pcchar_t  ai_pzServlet,
								 m4uint32_t  ai_iContentLength,
								 m4pcchar_t  ai_pzPrivCookie,
								 m4pcchar_t  ai_pzPublicCookie,
								 m4uint32_t  ai_iTimeout, // in msec
								 m4bool_t ai_bUseSSL,
								 m4pchar_t   ao_pzResult,
								 m4uint32_t& ao_iResultSize)
{
return BuildHTTPPOSTRequest(ai_pzHost,
							ai_pzPort,
							ai_pzServlet,
							ai_iContentLength,
							ai_pzPrivCookie,
							ai_pzPublicCookie,
							ACTION_CONNECT,
							ai_iTimeout,
							ai_bUseSSL==M4_TRUE ? USE_SSL_TRUE : USE_SSL_FALSE,
							ao_pzResult,
							ao_iResultSize);
}


m4return_t BuildSendDataRequest(m4pcchar_t  ai_pzServlet,
									     m4uint32_t  ai_iContentLength,
									     m4pcchar_t  ai_pzPrivCookie,
										 m4pcchar_t  ai_pzPublicCookie,
                                m4uint32_t  ai_iTimeout, // in msec
                                m4pchar_t   ao_pzResult,
                                m4uint32_t& ao_iResultSize)
{
   return BuildHTTPPOSTRequest("",
								       0,
									    ai_pzServlet,
									    ai_iContentLength,
									    ai_pzPrivCookie,
										ai_pzPublicCookie,
									    ACTION_SENDDATA,
                               ai_iTimeout,
                               USE_SSL_NO,
                               ao_pzResult,
                               ao_iResultSize);
}

m4return_t BuildDisconnectRequest(m4pcchar_t  ai_pzServlet,
									       m4uint32_t  ai_iContentLength,
									       m4pcchar_t  ai_pzPrivCookie,
										   m4pcchar_t  ai_pzPublicCookie,
                                  m4uint32_t  ai_iTimeout, // in msec
                                  m4pchar_t   ao_pzResult,
                                  m4uint32_t& ao_iResultSize)
{
   return BuildHTTPPOSTRequest("",
								       0,
									    ai_pzServlet,
									    ai_iContentLength,
									    ai_pzPrivCookie,
										ai_pzPublicCookie,
									    ACTION_DISCONNECT,
                               ai_iTimeout,
                               USE_SSL_NO,
                               ao_pzResult,
                               ao_iResultSize);
}


//## end module%3739996C01E3.additionalDeclarations


// Class ClHTTPParser::TokenWithResults 

// Additional Declarations
  //## begin ClHTTPParser::TokenWithResults%373991F202D5.declarations preserve=yes
  //## end ClHTTPParser::TokenWithResults%373991F202D5.declarations

// Class ClHTTPParser 


ClHTTPParser::ClHTTPParser (void )
  //## begin ClHTTPParser::ClHTTPParser%-301252683.hasinit preserve=no
  //## end ClHTTPParser::ClHTTPParser%-301252683.hasinit
  //## begin ClHTTPParser::ClHTTPParser%-301252683.initialization preserve=yes
  //## end ClHTTPParser::ClHTTPParser%-301252683.initialization
{
  //## begin ClHTTPParser::ClHTTPParser%-301252683.body preserve=yes
   InitParse();
   InitTokenLists();
  //## end ClHTTPParser::ClHTTPParser%-301252683.body
}


ClHTTPParser::~ClHTTPParser (void )
{
  //## begin ClHTTPParser::~ClHTTPParser%-1403232529.body preserve=yes
   ResetParse();
   ResetTokenList();
  //## end ClHTTPParser::~ClHTTPParser%-1403232529.body
}



//## Other Operations (implementation)
m4bool_t ClHTTPParser::GetEndOfHeaders (void )
{
  //## begin ClHTTPParser::GetEndOfHeaders%363925001.body preserve=yes
   return (m_EndOfHeaders!=NULL)?M4_TRUE:M4_FALSE;
  //## end ClHTTPParser::GetEndOfHeaders%363925001.body
}

m4pcchar_t ClHTTPParser::GetEndOfHeadersAddress (void )
{
  //## begin ClHTTPParser::GetEndOfHeadersAddress%1286452346.body preserve=yes
   return m_EndOfHeaders;
  //## end ClHTTPParser::GetEndOfHeadersAddress%1286452346.body
}

m4return_t ClHTTPParser::AddNewTokenToLookFor (m4pcchar_t ai_pzNewToken)
{
  //## begin ClHTTPParser::AddNewTokenToLookFor%1971719787.body preserve=yes
   if (m_iNumberOfTokens<MAX_TOKENS)
   {
      for (m4uint32_t iAux=0; iAux<MAX_TOKENS; iAux++)
      {
         if (m_Token[iAux]==NULL)
         {
            m_Token[iAux]=new TokenWithResults;

            m_iNumberOfTokens++;

            return m_Token[iAux]->SetId(ai_pzNewToken);
         }
      }

      M4_ASSERT(0);
   }
   else
      return M4_ERROR;

   return M4_SUCCESS;
  //## end ClHTTPParser::AddNewTokenToLookFor%1971719787.body
}

m4return_t ClHTTPParser::DeleteTokenToLookFor (m4pcchar_t ai_pzToken)
{
  //## begin ClHTTPParser::DeleteTokenToLookFor%1763134730.body preserve=yes
	m4int32_t iAux;
	
	for (iAux=0; iAux<MAX_TOKENS; iAux++)
	{
		if (m_Token[iAux]!=NULL)
		{
			m4char_t pzAuxTokenId[MAX_TOKEN_KEY_SIZE+1];

			if ((m_Token[iAux])->GetId(pzAuxTokenId)!=M4_SUCCESS)
			{
				return M4_ERROR;
			}
			else
			{
				if (stricmp(pzAuxTokenId,ai_pzToken)==0)
				{
					delete m_Token[iAux];
					
					m_Token[iAux]=NULL;

					m_iNumberOfTokens--;
					
					return M4_SUCCESS;
				}
//				else
//					cout<<"diferentes"<<endl;
			}
		}
		else
		{
//			cout<<"kk2"<<endl;
			iAux=MAX_TOKENS;
		}
	}
	return M4_WARNING;
  //## end ClHTTPParser::DeleteTokenToLookFor%1763134730.body
}

m4return_t ClHTTPParser::GetTokenResult (m4pcchar_t ai_pzToken, m4pchar_t ao_pzData, m4uint32_t ai_iIndex)
{
  //## begin ClHTTPParser::GetTokenResult%83523101.body preserve=yes
   m4int32_t iAuxIndex;

   if ((iAuxIndex=FindTokenById(ai_pzToken))>=0)
      return m_Token[iAuxIndex]->GetValue(ao_pzData,ai_iIndex);
   else
      return M4_ERROR;
  //## end ClHTTPParser::GetTokenResult%83523101.body
}

m4return_t ClHTTPParser::GetFirstLineResult (m4uint32_t& ao_iResult)
{
  //## begin ClHTTPParser::GetFirstLineResult%-1565266923.body preserve=yes
   ao_iResult=m_iHTMErrorCode;

   return M4_SUCCESS;
  //## end ClHTTPParser::GetFirstLineResult%-1565266923.body
}

m4return_t ClHTTPParser::GetFirstLineResult (m4pchar_t ao_pzResult)
{
  //## begin ClHTTPParser::GetFirstLineResult%-1855119664.body preserve=yes
   strcpy(ao_pzResult,m_pzHTMErrorString);

   return M4_SUCCESS;
  //## end ClHTTPParser::GetFirstLineResult%-1855119664.body
}

m4int32_t ClHTTPParser::Parse (m4pcchar_t ai_pzBuffer, m4uint32_t ai_iBufferSize)
{
  //## begin ClHTTPParser::Parse%193691917.body preserve=yes
  m4uint32_t iInputPointer=0;
   m4uint32_t iAux;


   for (m4bool_t bEnd=M4_FALSE;
        !bEnd;
       )
   {
      if (m_iHeaderSize+iInputPointer>=m_iSizeLimit)
         return HTTP_PARSE_BUFFER_SIZE_ERROR;

      switch (m_iState)
      {
         case HTTP_PS_HTTP_IDENTIFICATION_STR :
            // There is no pending buffer in this state
            for (iAux=0;
                 iAux<min(ai_iBufferSize,HTTP_IDENTIFIER_STRING_SIZE-m_iPendingPointer);
                 iAux++)
               if (ai_pzBuffer[iAux]!=HTTP_IDENTIFIER_STRING[m_iPendingPointer+iAux])
                  return HTTP_PARSE_ERROR;

            m_iPendingPointer+=iAux;
            iInputPointer+=iAux;

            if (m_iPendingPointer==HTTP_IDENTIFIER_STRING_SIZE)
            {
               m_iPendingPointer=0;
               // Next state
               m_iState=HTTP_PS_HTTP_FIRST_VERSION_NUMBER;
            }
            break;

         case HTTP_PS_HTTP_FIRST_VERSION_NUMBER :
            // There is no pending buffer in this state

            // From 1 to Any number of digits allowed...
            for (;
                 iInputPointer<ai_iBufferSize;
                 iInputPointer++)
            {
               if (!isdigit(ai_pzBuffer[iInputPointer]))
               {
                  if (ai_pzBuffer[iInputPointer]=='.') // End of the first version
                  {
                     // If number of digits presviouly==0 error
                     if (m_iPendingPointer==0)
                        return HTTP_PARSE_ERROR; // At least one digit, please

                     if (m_iHeaderSize+iInputPointer<m_iSizeLimit)
                     {
                        iInputPointer++;
                        m_iPendingPointer=0;

                        // Next state
                        m_iState=HTTP_PS_HTTP_SECOND_VERSION_NUMBER;

                        break;
                     }
                     else
                        return HTTP_PARSE_ERROR;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
               else
                  m_iPendingPointer++;
            }
            break;

         case HTTP_PS_HTTP_SECOND_VERSION_NUMBER :
            // There is no pending buffer in this state

            // At lest one number of digits allowed...
            for (;
                 iInputPointer<ai_iBufferSize;
                 iInputPointer++)
            {
               if (!isdigit(ai_pzBuffer[iInputPointer]))
               {
                  if (m_iPendingPointer==0)
                     return HTTP_PARSE_ERROR; // At least one digit, please

                  if (ai_pzBuffer[iInputPointer]==' ') // End of the second version
                  {
                     if (m_iHeaderSize+iInputPointer<m_iSizeLimit)
                     {
                        iInputPointer++;
                        m_iPendingPointer=0;

                        // Next state
                        m_iState=HTTP_PS_NUMERIC_ERROR_CODE;

                        break;
                     }
                     else
                        return HTTP_PARSE_ERROR;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
               else
                  m_iPendingPointer++;
            }
            break;

         case HTTP_PS_NUMERIC_ERROR_CODE :
            // There is no pending buffer in this state

            // At least one number of digits allowed...
            for (;
                 iInputPointer<ai_iBufferSize;
                 iInputPointer++)
            {
               if (!isdigit(ai_pzBuffer[iInputPointer]))
               {
                  if (m_iPendingPointer==0)
                     return HTTP_PARSE_ERROR; // At least one digit, please

                  if (ai_pzBuffer[iInputPointer]==' ') // End of the second version
                  {
                     if (m_iHeaderSize+iInputPointer<m_iSizeLimit)
                     {
                        // Store the results
                        m_pPendingBuffer[m_iPendingPointer]=0;
                        m_iHTMErrorCode=atoi(m_pPendingBuffer);

                        if (m_iHTMErrorCode!=HTTP_ERROR_CODE_OK)
                           return HTTP_PARSE_HTTP_ERROR;

                        iInputPointer++;
                        m_iPendingPointer=0;

                        // Next state
                        m_iState=HTTP_PS_STRING_ERROR;

                        break;
                     }
                     else
                        return HTTP_PARSE_ERROR;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
               else
               {
                  if (m_iPendingPointer<MAX_PENDING_BUFFER_SIZE)
                  {
                     m_pPendingBuffer[m_iPendingPointer]=ai_pzBuffer[iInputPointer];
                     m_iPendingPointer++;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
            }
            break;

         case HTTP_PS_STRING_ERROR :
            // There is no pending buffer in this state

            // Can't be null
            for (;
                 iInputPointer<ai_iBufferSize;
                 iInputPointer++)
            {
               if (ai_pzBuffer[iInputPointer]==CR_CHAR)
               {
                  if (m_iPendingPointer==0)
                     return HTTP_PARSE_ERROR; // At least one digit, please

                  if (m_iHeaderSize+iInputPointer<m_iSizeLimit)
                  {
                     // Store the results
                     if (m_iPendingPointer<MAX_HTTP_ERROR_STRING)
                     {
                        memcpy(m_pzHTMErrorString,
                               m_pPendingBuffer,
                               m_iPendingPointer);
                        m_pzHTMErrorString[m_iPendingPointer]=0;
                     }
                     else
                        return HTTP_PARSE_ERROR;

                     iInputPointer++;
                     m_iPendingPointer=0;

                     // Next state: Look for the LF_CHAR
                     m_iState=HTTP_PS_LOOKING_FOR_LF_CHAR1;

                     break;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
               else
               {
                  if (m_iPendingPointer<MAX_PENDING_BUFFER_SIZE)
                  {
                     m_pPendingBuffer[m_iPendingPointer]=ai_pzBuffer[iInputPointer];
                     m_iPendingPointer++;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
            }
            break;

         case HTTP_PS_VERIFYING_BLANK_LINE :
            if (ai_pzBuffer[iInputPointer]==LF_CHAR)
            {
               iInputPointer++;
               m_iPendingPointer=0;

               m_iState=HTTP_PS_IN_THE_BODY;

               break;
            }
            else
               return HTTP_PARSE_ALL_THE_HEADERS_FOUND;

         case HTTP_PS_IN_THE_BODY :
            m_EndOfHeaders=(m4pchar_t)&ai_pzBuffer[iInputPointer];

            // Verify if all the headers were found
            for (iAux=0; iAux<MAX_TOKENS; iAux++)
            {
               if (m_Token[iAux]!=NULL)
               {
                  m4char_t pzAuxValue[1024];

                  if (m_Token[iAux]->GetValue(pzAuxValue)!=M4_SUCCESS)
                     return HTTP_PARSE_END_OF_HEADERS; // Not solved
               }
            }

            return HTTP_PARSE_ALL_THE_HEADERS_FOUND;

         case HTTP_PS_LOOKING_FOR_LF_CHAR1 :
         case HTTP_PS_LOOKING_FOR_LF_CHAR2 :
            if (ai_pzBuffer[iInputPointer]==LF_CHAR)
            {
                  iInputPointer++;
                  m_iPendingPointer=0;

                  // Next state: Look for the TOKEN_ID
                  m_iState=HTTP_PS_LOOKING_FOR_TOKEN_ID;
            }
            else
               return HTTP_PARSE_ERROR;
            break;

         case HTTP_PS_LOOKING_FOR_TOKEN_ID :
            for (;
                 iInputPointer<ai_iBufferSize;
                 iInputPointer++)
            {
               if (ai_pzBuffer[iInputPointer]==CR_CHAR)
               {
                  // The end of line (error or we get to the end of headers ???)
                  iInputPointer++;
                  m_iPendingPointer=0;

                  // Next state: Look for the LF_CHAR, storing the token data
                  m_iState=HTTP_PS_VERIFYING_BLANK_LINE;

                  break;
               }
               else
               {
                  if (ai_pzBuffer[iInputPointer]==HTTP_END_OF_TOKEN_ID)
                  {
                     if (m_iPendingPointer==0)
                        return HTTP_PARSE_ERROR; // At least one char, please

                     if (m_iHeaderSize+iInputPointer<m_iSizeLimit)
                     {
                        // Store the ID for add it later (when we have the data)
                        if (m_iPendingPointer<MAX_PENDING_TOKEN_ID_SIZE)
                        {
                           memcpy(m_pPendingTokenID,m_pPendingBuffer,m_iPendingPointer);

                           m_pPendingTokenID[m_iPendingPointer]=0;
                        }
                        else
                           return HTTP_PARSE_ERROR;

                        iInputPointer++;
                        m_iPendingPointer=0;

                        // Next state: Look for the LF_CHAR, storing the token data
                        m_iState=HTTP_PS_GETTING_TOKEN_DATA;

                        break;
                     }
                     else
                        return HTTP_PARSE_ERROR;
                  }
                  else
                  {
                     if (m_iPendingPointer<MAX_PENDING_BUFFER_SIZE)
                     {
                        m_pPendingBuffer[m_iPendingPointer]=ai_pzBuffer[iInputPointer];
                        m_iPendingPointer++;
                     }
                     else
                        return HTTP_PARSE_ERROR;
                  }
               }
            }
            break;

         case HTTP_PS_GETTING_TOKEN_DATA :
            // Can be null ?
            for (;
                 iInputPointer<ai_iBufferSize;
                 iInputPointer++)
            {
               if (ai_pzBuffer[iInputPointer]==CR_CHAR)
               {
                  if (m_iHeaderSize+iInputPointer<m_iSizeLimit)
                  {
                     // Store the results
                     if (m_iPendingPointer<MAX_HTTP_ERROR_STRING)
                     {
                        // m_pPendingTokenID

                        // Lets see if we need to store you, man...

                        // Find the Token Id in the current list
                        m4int32_t iAuxIndex;

                        if ((iAuxIndex=FindTokenById(m_pPendingTokenID))>=0)
                        {
                           // Is one of those we control...
                           m_pPendingBuffer[m_iPendingPointer]=0;

                           if (m_Token[iAuxIndex]->AddValue(m_pPendingBuffer)!=M4_SUCCESS)
                              return HTTP_PARSE_ERROR;
                           else
                           {
                              // Verify if the list of tokens is completely solved ???
                           }
                        }
                        else
                           ; // Nothing here, shure
                     }
                     else
                        return HTTP_PARSE_ERROR;

                     iInputPointer++;
                     m_iPendingPointer=0;

                     // Next state: Look for the LF_CHAR
                     m_iState=HTTP_PS_LOOKING_FOR_LF_CHAR2;

                     break;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
               else
               {
                  if (m_iPendingPointer<MAX_PENDING_BUFFER_SIZE)
                  {
                     m_pPendingBuffer[m_iPendingPointer]=ai_pzBuffer[iInputPointer];
                     m_iPendingPointer++;
                  }
                  else
                     return HTTP_PARSE_ERROR;
               }
            }
            break;
      }

      if (iInputPointer==ai_iBufferSize)
      {
         bEnd=M4_TRUE;
      }
   }

   m_iHeaderSize+=iInputPointer;

   return HTTP_PARSE_CONTINUE;
  //## end ClHTTPParser::Parse%193691917.body
}

void ClHTTPParser::ResetParse (m4int32_t ai_iNewHeaderSizeLimit)
{
  //## begin ClHTTPParser::ResetParse%-128425510.body preserve=yes
   m_iState=HTTP_PS_HTTP_IDENTIFICATION_STR;

   m_iPendingPointer=0;

   m_EndOfHeaders=NULL;

   m_iHTMErrorCode=HTTP_INVALID_ERROR;
   m_pzHTMErrorString[0]=0;

   if (ai_iNewHeaderSizeLimit>=0)
      SetHeadersSizeLimit(ai_iNewHeaderSizeLimit);

   m_iHeaderSize=0;
  //## end ClHTTPParser::ResetParse%-128425510.body
}

void ClHTTPParser::ResetTokenList (void )
{
  //## begin ClHTTPParser::ResetTokenList%-1250033715.body preserve=yes
  m4int32_t iAux;

   for (iAux=0; iAux<MAX_TOKENS; iAux++)
      if (m_Token[iAux]!=NULL)
      {
         delete m_Token[iAux];

         m_Token[iAux]=NULL;
      }
  //## end ClHTTPParser::ResetTokenList%-1250033715.body
}

void ClHTTPParser::ResetTokenResults (void )
{
  //## begin ClHTTPParser::ResetTokenResults%376502623.body preserve=yes
   m4int32_t iAux;

   for (iAux=0; iAux<MAX_TOKENS; iAux++)
      if (m_Token[iAux]!=NULL)
          m_Token[iAux]->ResetValues();
  //## end ClHTTPParser::ResetTokenResults%376502623.body
}

void ClHTTPParser::ResetParseAndTokenResults (void )
{
  //## begin ClHTTPParser::ResetParseAndTokenResults%884729305.body preserve=yes
	ResetTokenResults();
	ResetParse();
  //## end ClHTTPParser::ResetParseAndTokenResults%884729305.body
}

void ClHTTPParser::InitParse (void )
{
  //## begin ClHTTPParser::InitParse%1094424258.body preserve=yes
   m_iState=HTTP_PS_HTTP_IDENTIFICATION_STR;

   m_iPendingPointer=0;

   m_EndOfHeaders=NULL;

   m_iHTMErrorCode=HTTP_INVALID_ERROR;
   m_pzHTMErrorString[0]=0;

   m_iSizeLimit=0;
   m_iHeaderSize=0;
  //## end ClHTTPParser::InitParse%1094424258.body
}

void ClHTTPParser::InitTokenLists (void )
{
  //## begin ClHTTPParser::InitTokenLists%1683790101.body preserve=yes
   m4int32_t iAux;

   m_iNumberOfTokens=0;
   for (iAux=0; iAux<MAX_TOKENS; iAux++)
      m_Token[iAux]=NULL;
  //## end ClHTTPParser::InitTokenLists%1683790101.body
}

m4int32_t ClHTTPParser::FindTokenById (m4pcchar_t ai_pzToken)
{
  //## begin ClHTTPParser::FindTokenById%254489228.body preserve=yes
   for (m4uint32_t iAux=0; iAux<MAX_TOKENS; iAux++)
   {
      if (m_Token[iAux]!=NULL)
      {
         m4char_t pzId[512];

         if (m_Token[iAux]->GetId(pzId)!=M4_SUCCESS)
            return M4_ERROR;

         // Case insensitive
         if (stricmp(pzId,ai_pzToken)==0)
            return iAux;
      }
   }

   return -1;
  //## end ClHTTPParser::FindTokenById%254489228.body
}

// Additional Declarations
  //## begin ClHTTPParser%373991F201CD.declarations preserve=yes
  //## end ClHTTPParser%373991F201CD.declarations

//## begin module%3739996C01E3.epilog preserve=yes
//## end module%3739996C01E3.epilog
