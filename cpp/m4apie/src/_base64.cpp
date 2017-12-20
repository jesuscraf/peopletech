// _Base64.cpp: implementation of the C_Base64 class.
//
//////////////////////////////////////////////////////////////////////


//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
#include "stdafx.h"
#include "_base64.h"



#define LINELEN 64		      /* Encoded line length (max 76) */

typedef unsigned char byte;	      /* Byte type */
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//##ModelId=396B3298020E
C_Base64::C_Base64()
{

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
}

//##ModelId=396B32980203
C_Base64::~C_Base64()
{

}

//##ModelId=396B3298020F
short C_Base64::Code(unsigned char *input,BSTR * output, long longitud)
{

	/*  ENCODE  --	Encode binary stream into base64.  */

	//BSTR temp;
	BYTE* outputChar;
    //long length;

	/* outputLength -- Length of the output buffer */
	
	//length = ::SysStringLen(input);

	//while(input[++length]);

	int numBlocks=0;
	if ((longitud%3)==0)
		numBlocks=(longitud/3);
	else
		numBlocks=(longitud/3)+1;

	//long outputLength= (numBlocks*4+(numBlocks*4/LINELEN));
	long outputLength= (numBlocks*4);

	outputChar = new BYTE[outputLength+1];
	
	//printf("C_BASE64--inLen:%lu outLen:%lu\r\n",length,outputLength);
	
	//temp = ::SysAllocStringLen(NULL,outputLength);
	//output=(char *) malloc (outputLength+1);

	long inputIndex=0;
	long outputIndex=0;
	int lineLength=0;


    while (longitud>inputIndex) {
		
		byte igroup[3], ogroup[4];
		int n;
		char c;

		igroup[0] = igroup[1] = igroup[2] = 0;
		for (n = 0; n < 3; n++) {
			
			c = input[inputIndex++];
			igroup[n] = (byte) c;
			if (inputIndex >= longitud) {
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
				
				//if (lineLength==LINELEN){
				//	output[outputIndex++]=10;
				//	lineLength=0;
				//}
				outputChar[outputIndex++]=ogroup[i];
				//lineLength++;
			}
		}
    }
	outputChar[outputIndex]=0;
	
	::SysFreeString(*output);
	
	*output = ::SysAllocStringLen(NULL, outputIndex);
	MultiByteToWideChar(CP_ACP , NULL, (const char *)outputChar, outputIndex, *output, outputIndex);
	delete[] outputChar;

    return 1;
}

//##ModelId=396B32980221
short C_Base64::Decode(BSTR input,BSTR *output)
{
	unsigned char * entrada;
	unsigned char * outputChar;
	long			inputLength;
	int				i = 0;

	inputLength = ::SysStringLen(input);

	entrada = new BYTE[inputLength];
	outputChar = new BYTE[(inputLength*3)/4 + 1];

	WideCharToMultiByte(CP_ACP , NULL, input, inputLength, (char *)entrada, inputLength, NULL, NULL);

	//output=(char *) malloc (inputLength);
	
	long inputIndex=0;
	long outputIndex=0;
	
	/*CONSTANTCONDITION*/
    while (inputLength>inputIndex)
	{

		//printf("inputLength:%lu inputIndex:%lu\r\n",inputLength,inputIndex);
		byte a[4], o[3];
		byte b[4];

		for (i = 0; i < 4; i++)
		{
			
			char c;

			//do {
			c = (char) entrada[inputIndex++];

			if (inputIndex > inputLength)
			{
				// Salida a saco porque no hay coherencia de datos
				delete[] outputChar;
				delete[] entrada;
				::SysFreeString(*output);
				*output = ::SysAllocString(OLESTR(""));
				return 1;
			}
			else if(decodeTable[c] == 0x80)
			{
				// carácter inválido
				delete[] entrada;
				::SysFreeString(*output);
				*output = ::SysAllocStringLen(NULL, inputIndex - 1);
				MultiByteToWideChar(CP_ACP , NULL, (const char *)outputChar, outputIndex, *output, outputIndex);
				delete[] outputChar;
				return 1;
			}
				
				
				//} while ((decodeTable[c] != 0x80)&&(inputIndex <inputLength));
	
/*			if ((inputIndex >= inputLength)||(i!=3))
			{
				// Salida a saco porque no hay coherencia de datos
				delete[] outputChar;
				delete[] entrada;
				::SysFreeString(*output);
				*output = ::SysAllocString(OLESTR(""));
				return 0;
			}
			else if (decodeTable[c] == 0x80)
			{
				// Devolvemos lo que tenemos (sea lo que sea)
				delete[] entrada;
				::SysFreeString(*output);
				*output = ::SysAllocStringLen(NULL, inputIndex - 1);
				MultiByteToWideChar(CP_ACP , NULL, (const char *)outputChar, inputIndex -1, *output, inputIndex-1);
				delete[] outputChar;
			}*/
			a[i] = (byte) c;
			b[i] = (byte) decodeTable[c];
		}

		

		o[0] = (b[0] << 2) | (b[1] >> 4);
		o[1] = (b[1] << 4) | (b[2] >> 2);
		o[2] = (b[2] << 6) | b[3];
        i = a[2] == '=' ? 1 : (a[3] == '=' ? 2 : 3);
		
		for (int j=0;j<i;j++){
			outputChar[outputIndex++]= o[j];			
		}
		if (i<3)
			break;
				
	}
	
	
	delete[] entrada;


	::SysFreeString(*output);
	*output = ::SysAllocStringLen(NULL, outputIndex);
	MultiByteToWideChar(CP_ACP , NULL, (const char *)outputChar, outputIndex, *output, outputIndex);

	delete[] outputChar;
	return 1;
}



