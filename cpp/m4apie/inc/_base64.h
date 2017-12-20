// _Base64.h: interface for the C_Base64 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__BASE64_H__951C8215_FA2E_11D1_95FD_00C04FB7147A__INCLUDED_)
#define AFX__BASE64_H__951C8215_FA2E_11D1_95FD_00C04FB7147A__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000





typedef unsigned char byte;	      /* Byte type */


//##ModelId=396B329801C7
class C_Base64  
{
public:
	//##ModelId=396B32980221
	short Decode(BSTR input,BSTR *output);
	//##ModelId=396B329801FB
	byte decodeTable[256];	    /* Decode table */
	//##ModelId=396B329801E7
	byte encodeTable[256];	    /* Encode table */

	//##ModelId=396B3298020F
	short Code(unsigned char * input,BSTR *output, long longitud);
	//##ModelId=396B3298020E
	C_Base64();
	//##ModelId=396B32980203
	virtual ~C_Base64();

};

#endif // !defined(AFX__BASE64_H__951C8215_FA2E_11D1_95FD_00C04FB7147A__INCLUDED_)
