//## begin module%3912C58501F4.cm preserve=no
//## end module%3912C58501F4.cm

//## begin module%3912C58501F4.cp preserve=no
//	=============================================================================
//	=
//
//	 (c) Copyright  1991-1997 Meta Software M.S., S.A
//	 All rights reserved.
//
//	 Module:            <module>
//	 File:                  <Ffile>.cpp
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
//	=============================================================================
//## end module%3912C58501F4.cp

//## Module: _base64%3912C58501F4; Package body
//## Subsystem: m4sysall::src%3912C5DD0060
//## Source file: d:\new\m4sysall\src\_base64.cpp

//## begin module%3912C58501F4.additionalIncludes preserve=no
//## end module%3912C58501F4.additionalIncludes

//## begin module%3912C58501F4.includes preserve=yes
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             _Base64.cpp.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the C_Base64 class.
//
//
//==============================================================================

#include <string.h>
#include <stdlib.h>
#include "_base64.h"
//## end module%3912C58501F4.includes

//## begin module%3912C58501F4.declarations preserve=no
//## end module%3912C58501F4.declarations

//## begin module%3912C58501F4.additionalDeclarations preserve=yes



#define LINELEN 64		      /* Encoded line length (max 76) */

typedef unsigned char byte;	      /* Byte type */
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//## end module%3912C58501F4.additionalDeclarations


// Class C_Base64 



C_Base64::C_Base64 ()
  //## begin C_Base64::C_Base64%-1127064404.hasinit preserve=no
  //## end C_Base64::C_Base64%-1127064404.hasinit
  //## begin C_Base64::C_Base64%-1127064404.initialization preserve=yes
  //## end C_Base64::C_Base64%-1127064404.initialization
{
  //## begin C_Base64::C_Base64%-1127064404.body preserve=yes

	 /*	Fill decodeTable with character encodings.  */
	int i;

    for (i = 0; i < 255; i++) {
		decodeTable[i] = 0x80;
    }
    for (i = 'A'; i <= 'Z'; i++) {
        decodeTable[i] = 0 + (i - 'A');
    }
    for (i = 'a'; i <= 'z'; i++) {
        decodeTable[i] = 26 + (i - 'a');
    }
    for (i = '0'; i <= '9'; i++) {
        decodeTable[i] = 52 + (i - '0');
    }
    decodeTable['+'] = 62;
    decodeTable['/'] = 63;
    decodeTable['='] = 0;



	for (i = 0; i < 26; i++) {
        encodeTable[i] = 'A' + i;
        encodeTable[26 + i] = 'a' + i;
    }
    for (i = 0; i < 10; i++) {
        encodeTable[52 + i] = '0' + i;
    }
    encodeTable[62] = '+';
    encodeTable[63] = '/';
  //## end C_Base64::C_Base64%-1127064404.body
}


C_Base64::~C_Base64 ()
{
  //## begin C_Base64::~C_Base64%-1393573843.body preserve=yes
  //## end C_Base64::~C_Base64%-1393573843.body
}



//## Other Operations (implementation)
short C_Base64::Decode (char* input, char*& output, long* length)
{
  //## begin C_Base64::Decode%-1539693195.body preserve=yes
/*  DECODE  --	Decode base64 stream into binary.  */


	long inputLength=(long)strlen(input);
	
	output=(char *) malloc (inputLength);
	
	long inputIndex=0;
	long outputIndex=0;
	
	/*CONSTANTCONDITION*/
    while (inputLength>inputIndex) {

		//printf("inputLength:%lu inputIndex:%lu\r\n",inputLength,inputIndex);
		byte a[4], b[4], o[3];
		int i=0;
		for (i = 0; i < 4; i++) {
			
			char c;

			do {
				c = input[inputIndex++];
				
			} while ((decodeTable[c] & 0x80)&&(inputIndex <inputLength));
			if ((inputIndex >= inputLength)&&(i!=3)&&!(decodeTable[c] & 0x80)) {
				//printf("Error:Tamaño de entrada no es multiplo de 3");
				return 0;
			}
			
			a[i] = (byte) c;
			b[i] = (byte) decodeTable[c];
		}
		o[0] = (b[0] << 2) | (b[1] >> 4);
		o[1] = (b[1] << 4) | (b[2] >> 2);
		o[2] = (b[2] << 6) | b[3];
        i = a[2] == '=' ? 1 : (a[3] == '=' ? 2 : 3);
		
		
		//printf("C_Base64--- I=%lu b[0]=%lu b[1]=%lu b[2]=%lu b[3]=%lu\r\no[0]=%lu,o[1]=%lu,o[2]=%lu\r\n",i,b[0],b[1],b[2],b[3],o[0],o[1],o[2]);
		
		
		for (int j=0;j<i;j++){
			output[outputIndex++]=o[j];			
		}
		if (i<3)
			break;
	
					
	}
	
	*length=outputIndex;
	char *temp=(char *)malloc ((*length)+1);
	memcpy(temp,output,*length);
	temp[*length]=0;
	free(output);
	output=temp;
	
	return 1;
  //## end C_Base64::Decode%-1539693195.body
}

short C_Base64::Code (char* input, long length, char*& output)
{
  //## begin C_Base64::Code%-1270952636.body preserve=yes

	/*  ENCODE  --	Encode binary stream into base64.  */



	/* outputLength -- Length of the output buffer */
	int numBlocks=0;
	if ((length%3)==0)
		numBlocks=(length/3);
	else
		numBlocks=(length/3)+1;

	long outputLength= (numBlocks*4+(numBlocks*4/LINELEN));
	
	//printf("C_BASE64--inLen:%lu outLen:%lu\r\n",length,outputLength);
	
	output=(char *) malloc (outputLength+1);

	long inputIndex=0;
	long outputIndex=0;
	int lineLength=0;


    while (length>inputIndex) {
		
		byte igroup[3], ogroup[4];
		int n;
		char c;

		igroup[0] = igroup[1] = igroup[2] = 0;
		for (n = 0; n < 3; n++) {
			
			c = input[inputIndex++];
			igroup[n] = (byte) c;
			if (inputIndex >= length) {
				n++;
				break;
			}
			
		}
		if (n > 0) {
			ogroup[0] = encodeTable[igroup[0] >> 2];
		    ogroup[1] = encodeTable[((igroup[0] & 3) << 4) | (igroup[1] >> 4)];
		    ogroup[2] = encodeTable[((igroup[1] & 0xF) << 2) | (igroup[2] >> 6)];
		    ogroup[3] = encodeTable[igroup[2] & 0x3F];	

            /* Replace characters in output stream with "=" pad
	       characters if fewer than three characters were
	       read from the end of the input stream. */

			if (n < 3) {
			    ogroup[3] = '=';
				if (n < 2) {
                    ogroup[2] = '=';
				}
			}
			
			//printf("Bytes:%lu\r\nCodificados:o[0]=%c,o[1]=%c,o[2]=%c,o[3]=%c\r\nDecodificados:i[0]=%lu,i[1]=%lu,i[2]=%lu\r\n",n,ogroup[0],ogroup[1],ogroup[2],ogroup[3],igroup[0],igroup[1],igroup[2]);
			
			for (int i = 0; i < 4; i++) {
				
				if (lineLength==LINELEN){
					output[outputIndex++]=10;
					lineLength=0;
				}
				output[outputIndex++]=ogroup[i];
				lineLength++;
			}
		}
    }
	output[outputIndex]=0;
	
    return 1;
  //## end C_Base64::Code%-1270952636.body
}

void C_Base64::FreeBuffer (char* input)
{
  //## begin C_Base64::FreeBuffer%708457139.body preserve=yes
/*  FREEBUFFER  --	release external referenced resouces  */

	free(input);
  //## end C_Base64::FreeBuffer%708457139.body
}

// Additional Declarations
  //## begin C_Base64%3912C5830152.declarations preserve=yes
  //## end C_Base64%3912C5830152.declarations

//## begin module%3912C58501F4.epilog preserve=yes
//## end module%3912C58501F4.epilog


