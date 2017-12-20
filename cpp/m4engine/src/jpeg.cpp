//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           <modulo>
// File:             rasterg.cpp
// Project:
// Author:           Meta Software M.S. , S.A
// Date:18/09/97
// Language:         C++
// Operating System: ALL
// Design Document:  
//
//	Propietary:		Fernando Abad Garcia
//	Modifications:	Who && Date
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================

#include <math.h>

#include "m4stl.hpp"
#include "eng.h"
#include "engdf.inl"

#include "m4clbuf.h"
#include "rasterg.h"
#include "clfile.h"

#include "jpeg.h"

#define ENG_JPEG_ROUND(a) ((m4int64_t)(floor((m4double_t)a+0.5)))

#define ENG_JPEG_COMMENT "Meta 4 JPEG encoder"

#define GROUP_ID    ENG_TGI_RASTERG

//TEST
//MIT_File *FBuffIt ;
//


//==============================================================================ClENG_DCT

m4double_t const ClENG_DCT::m_AAN_ScaleFactor[ClENG_DCT::N] = { 
	1.0, 1.387039845, 1.306562965, 1.175875602,
    1.0, 0.785694958, 0.541196100, 0.275899379
};



ClENG_DCT::ClENG_DCT()
{
}

void ClENG_DCT::End()
{
}

void ClENG_DCT::Init(m4int32_t ai_Quality) 
{
	m4int32_t i, j, index, temp ;
	m4int16_t Proc ;

	ClENG_DCT::End();

	if (ai_Quality <= 0) ai_Quality = 1;
    else if (ai_Quality > 100) ai_Quality = 100;

	if (ai_Quality < 50) {
		ai_Quality = 5000 / ai_Quality ;
	} else {
		ai_Quality = 200 - ai_Quality * 2;
	} ;
	m_Quality = ai_Quality ;

	
	// Matriz de luminosidad

	m_Qu[LUMIN][0]=16; m_Qu[LUMIN][1]=11; m_Qu[LUMIN][2]=10; m_Qu[LUMIN][3]=16;
	m_Qu[LUMIN][4]=24; m_Qu[LUMIN][5]=40; m_Qu[LUMIN][6]=51; m_Qu[LUMIN][7]=61;
	m_Qu[LUMIN][8]=12; m_Qu[LUMIN][9]=12; m_Qu[LUMIN][10]=14; m_Qu[LUMIN][11]=19;
	m_Qu[LUMIN][12]=26; m_Qu[LUMIN][13]=58; m_Qu[LUMIN][14]=60; m_Qu[LUMIN][15]=55;
	m_Qu[LUMIN][16]=14; m_Qu[LUMIN][17]=13; m_Qu[LUMIN][18]=16; m_Qu[LUMIN][19]=24;
	m_Qu[LUMIN][20]=40; m_Qu[LUMIN][21]=57; m_Qu[LUMIN][22]=69; m_Qu[LUMIN][23]=56;
	m_Qu[LUMIN][24]=14; m_Qu[LUMIN][25]=17; m_Qu[LUMIN][26]=22; m_Qu[LUMIN][27]=29;
	m_Qu[LUMIN][28]=51; m_Qu[LUMIN][29]=87; m_Qu[LUMIN][30]=80; m_Qu[LUMIN][31]=62;
	m_Qu[LUMIN][32]=18; m_Qu[LUMIN][33]=22; m_Qu[LUMIN][34]=37; m_Qu[LUMIN][35]=56;
	m_Qu[LUMIN][36]=68; m_Qu[LUMIN][37]=109; m_Qu[LUMIN][38]=103; m_Qu[LUMIN][39]=77;
	m_Qu[LUMIN][40]=24; m_Qu[LUMIN][41]=35; m_Qu[LUMIN][42]=55; m_Qu[LUMIN][43]=64;
	m_Qu[LUMIN][44]=81; m_Qu[LUMIN][45]=104; m_Qu[LUMIN][46]=113; m_Qu[LUMIN][47]=92;
	m_Qu[LUMIN][48]=49; m_Qu[LUMIN][49]=64; m_Qu[LUMIN][50]=78; m_Qu[LUMIN][51]=87;
	m_Qu[LUMIN][52]=103; m_Qu[LUMIN][53]=121; m_Qu[LUMIN][54]=120; m_Qu[LUMIN][55]=101;
	m_Qu[LUMIN][56]=72; m_Qu[LUMIN][57]=92; m_Qu[LUMIN][58]=95; m_Qu[LUMIN][59]=98;
	m_Qu[LUMIN][60]=112; m_Qu[LUMIN][61]=100; m_Qu[LUMIN][62]=103; m_Qu[LUMIN][63]=99;


	// Matriz de cromacidad

	m_Qu[CHRO][0]=17; m_Qu[CHRO][1]=18; m_Qu[CHRO][2]=24; m_Qu[CHRO][3]=47;
	m_Qu[CHRO][4]=99; m_Qu[CHRO][5]=99; m_Qu[CHRO][6]=99; m_Qu[CHRO][7]=99;
	m_Qu[CHRO][8]=18; m_Qu[CHRO][9]=21; m_Qu[CHRO][10]=26; m_Qu[CHRO][11]=66;
	m_Qu[CHRO][12]=99; m_Qu[CHRO][13]=99; m_Qu[CHRO][14]=99; m_Qu[CHRO][15]=99;
	m_Qu[CHRO][16]=24; m_Qu[CHRO][17]=26; m_Qu[CHRO][18]=56; m_Qu[CHRO][19]=99;
	m_Qu[CHRO][20]=99; m_Qu[CHRO][21]=99; m_Qu[CHRO][22]=99; m_Qu[CHRO][23]=99;
	m_Qu[CHRO][24]=47; m_Qu[CHRO][25]=66; m_Qu[CHRO][26]=99; m_Qu[CHRO][27]=99;
	m_Qu[CHRO][28]=99; m_Qu[CHRO][29]=99; m_Qu[CHRO][30]=99; m_Qu[CHRO][31]=99;
	m_Qu[CHRO][32]=99; m_Qu[CHRO][33]=99; m_Qu[CHRO][34]=99; m_Qu[CHRO][35]=99;
	m_Qu[CHRO][36]=99; m_Qu[CHRO][37]=99; m_Qu[CHRO][38]=99; m_Qu[CHRO][39]=99;
	m_Qu[CHRO][40]=99; m_Qu[CHRO][41]=99; m_Qu[CHRO][42]=99; m_Qu[CHRO][43]=99;
	m_Qu[CHRO][44]=99; m_Qu[CHRO][45]=99; m_Qu[CHRO][46]=99; m_Qu[CHRO][47]=99;
	m_Qu[CHRO][48]=99; m_Qu[CHRO][49]=99; m_Qu[CHRO][50]=99; m_Qu[CHRO][51]=99;
	m_Qu[CHRO][52]=99; m_Qu[CHRO][53]=99; m_Qu[CHRO][54]=99; m_Qu[CHRO][55]=99;
	m_Qu[CHRO][56]=99; m_Qu[CHRO][57]=99; m_Qu[CHRO][58]=99; m_Qu[CHRO][59]=99;
	m_Qu[CHRO][60]=99; m_Qu[CHRO][61]=99; m_Qu[CHRO][62]=99; m_Qu[CHRO][63]=99;
	
	
	for ( Proc=0; Proc < 2; ++Proc ) {
		for (j = 0; j < 64; j++) {
			temp = (m_Qu[Proc][j] * ai_Quality + 50) / 100;
			if ( temp <= 0) temp = 1;
			if (temp > 255) temp = 255;
			m_Qu[Proc][j] = temp;
        }
        index = 0;
        for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				m_Dv[Proc][index] = (m4double_t) (m_Qu[Proc][index] << 3);
				m_Dv[Proc][index] = (m4double_t) ((m4double_t)1.0/
					((m4double_t) m_Qu[Proc][index] * 
					m_AAN_ScaleFactor[i] * m_AAN_ScaleFactor[j] * (m4double_t) 8.0));
				index++;
			}
        }
	} ;

}


void ClENG_DCT::ForwardDCT(m4float_t ai_In[N][N], m4double_t ao_Out[N][N] ) 
{
	m4double_t tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7;
	m4double_t tmp10, tmp11, tmp12, tmp13;
	m4double_t z1, z2, z3, z4, z5, z11, z13;
    m4int32_t  i, j ;


	// e resta 128 a los valores de la entrada
	for (i = 0; i < 8; i++) {
		for(j = 0; j < 8; j++) {
			ao_Out[i][j] = ((m4double_t)ai_In[i][j] - (m4double_t)128.0);
		}
	}

	for (i = 0; i < 8; i++) {
		tmp0 = ao_Out[i][0] + ao_Out[i][7];
		tmp7 = ao_Out[i][0] - ao_Out[i][7];
		tmp1 = ao_Out[i][1] + ao_Out[i][6];
		tmp6 = ao_Out[i][1] - ao_Out[i][6];
		tmp2 = ao_Out[i][2] + ao_Out[i][5];
		tmp5 = ao_Out[i][2] - ao_Out[i][5];
		tmp3 = ao_Out[i][3] + ao_Out[i][4];
		tmp4 = ao_Out[i][3] - ao_Out[i][4];

		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;

		ao_Out[i][0] = tmp10 + tmp11;
		ao_Out[i][4] = tmp10 - tmp11;

		z1 = (tmp12 + tmp13) * (m4double_t) 0.707106781;
		ao_Out[i][2] = tmp13 + z1;
		ao_Out[i][6] = tmp13 - z1;

		tmp10 = tmp4 + tmp5;
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		z5 = (tmp10 - tmp12) * (m4double_t) 0.382683433;
		z2 = ((m4double_t) 0.541196100) * tmp10 + z5;
		z4 = ((m4double_t) 1.306562965) * tmp12 + z5;
		z3 = tmp11 * ((m4double_t) 0.707106781);

		z11 = tmp7 + z3;
		z13 = tmp7 - z3;

		ao_Out[i][5] = z13 + z2;
		ao_Out[i][3] = z13 - z2;
		ao_Out[i][1] = z11 + z4;
		ao_Out[i][7] = z11 - z4;
	}

	for (i = 0; i < 8; i++) {
		tmp0 = ao_Out[0][i] + ao_Out[7][i];
		tmp7 = ao_Out[0][i] - ao_Out[7][i];
		tmp1 = ao_Out[1][i] + ao_Out[6][i];
		tmp6 = ao_Out[1][i] - ao_Out[6][i];
		tmp2 = ao_Out[2][i] + ao_Out[5][i];
		tmp5 = ao_Out[2][i] - ao_Out[5][i];
		tmp3 = ao_Out[3][i] + ao_Out[4][i];
		tmp4 = ao_Out[3][i] - ao_Out[4][i];

		tmp10 = tmp0 + tmp3;
		tmp13 = tmp0 - tmp3;
		tmp11 = tmp1 + tmp2;
		tmp12 = tmp1 - tmp2;

		ao_Out[0][i] = tmp10 + tmp11;
		ao_Out[4][i] = tmp10 - tmp11;

		z1 = (tmp12 + tmp13) * (m4double_t) 0.707106781;
		ao_Out[2][i] = tmp13 + z1;
		ao_Out[6][i] = tmp13 - z1;

		tmp10 = tmp4 + tmp5;
		tmp11 = tmp5 + tmp6;
		tmp12 = tmp6 + tmp7;

		z5 = (tmp10 - tmp12) * (m4double_t) 0.382683433;
		z2 = ((m4double_t) 0.541196100) * tmp10 + z5;
		z4 = ((m4double_t) 1.306562965) * tmp12 + z5;
		z3 = tmp11 * ((m4double_t) 0.707106781);

		z11 = tmp7 + z3;
		z13 = tmp7 - z3;

		ao_Out[5][i] = z13 + z2;
		ao_Out[3][i] = z13 - z2;
		ao_Out[1][i] = z11 + z4;
		ao_Out[7][i] = z11 - z4;
	}

}


void ClENG_DCT::QuantizeBlock(m4double_t ai_In[N][N], m4int32_t ai_Code, m4int32_t ao_Out[N*N] ) 
{
	m4int32_t i, j, index ;
    
	index = 0;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			ao_Out[index] = (m4int32_t)(ENG_JPEG_ROUND(ai_In[i][j] * m_Dv[ai_Code][index]));
			ao_Out[index] = (m4int32_t)( ai_In[i][j] * m_Dv[ai_Code][index] + 
				16384.5 -16384);
			index++;
		}
	}
}


//===========================================================================ClENG_BlkEncoder

const m4uint8_t ClENG_BlkEncoder::m_Bits[ClENG_BlkEncoder::AC_DC_ARR_SIZE][17] = 
{
	{0x00,0, 1, 5, 1, 1,1,1,1,1,0,0,0,0,0,0,0},		//bits DC luminance
	{0x10,0,2,1,3,3,2,4,3,5,5,4,4,0,0,1,0x7d },		//bits AC luminance
	{0x01,0,3,1,1,1,1,1,1,1,1,1,0,0,0,0,0 },		//bits DC chro
	{0x11,0,2,1,2,4,4,3,4,7,5,4,4,0,1,2,0x77}		//bits AC chro
} ;

const m4uint8_t ClENG_BlkEncoder::m_Val[ ClENG_BlkEncoder::AC_DC_ARR_SIZE][162] =
{
	{ 0,1,2,3,4,5,6,7,8,9,10,11 },							//val DC luminance
	{														//val AC luminance	
		0x01, 0x02, 0x03, 0x00, 0x04, 0x11, 0x05, 0x12,	
		0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07,
		0x22, 0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08,
		0x23, 0x42, 0xb1, 0xc1, 0x15, 0x52, 0xd1, 0xf0,
		0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16,
		0x17, 0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28,
		0x29, 0x2a, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
		0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,
		0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
		0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
		0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
		0x7a, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89,
		0x8a, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
		0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
		0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6,
		0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3, 0xc4, 0xc5,
		0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
		0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2,
		0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea,
		0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
		0xf9, 0xfa 
	},
	{ 0,1,2,3,4,5,6,7,8,9,10,11	 },							//val DC chro
	{														//val AC chro
		0x00, 0x01, 0x02, 0x03, 0x11, 0x04, 0x05, 0x21,
		0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71,
		0x13, 0x22, 0x32, 0x81, 0x08, 0x14, 0x42, 0x91,
		0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33, 0x52, 0xf0,
		0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34,
		0xe1, 0x25, 0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26,
		0x27, 0x28, 0x29, 0x2a, 0x35, 0x36, 0x37, 0x38,
		0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
		0x49, 0x4a, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
		0x59, 0x5a, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
		0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
		0x79, 0x7a, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
		0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95, 0x96,
		0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5,
		0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4,
		0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2, 0xc3,
		0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2,
		0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda,
		0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9,
		0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
		0xf9, 0xfa 
	}
} ;


ClENG_BlkEncoder::ClENG_BlkEncoder()
{
}

void ClENG_BlkEncoder::End()
{
}

void ClENG_BlkEncoder::Init() 
{
	m4int32_t p, l, i, lastp, si, code;
	m4int32_t huffsize[257];
	m4int32_t huffcode[257];
	m4int_t B, DC_Index, AC_Index, MatrixIndex ;

	ClENG_BlkEncoder::End();

	m_BufferPutBuffer = m_BufferPutBits = 0 ;

	for (B=0;B<2;++B) {

		//Indices a procesar en pasadas 0-1

		if (B==0) {
			DC_Index=DC_CHRO ;
			AC_Index=AC_CHRO ;
			MatrixIndex=1;
		} else {
			DC_Index=DC_LUMIN ;
			AC_Index=AC_LUMIN ;
			MatrixIndex=0;
		} ;

		//Init de valores DC y AC

		p = 0;
		for (l = 1; l <= 16; l++) {
			for (i = 1; i <= m_Bits[DC_Index][l]; i++) {
				huffsize[p++] = l;
			} ;
		} ;
		huffsize[p] = 0;
		lastp = p;

		code = 0;
		si = huffsize[0];
		p = 0;
		while(huffsize[p] != 0) {
			while(huffsize[p] == si) {
				huffcode[p++] = code;
				code++;
			} ;
			code <<= 1;
			si++;
		} ;

		for (p = 0; p < lastp; p++) {
			m_DC_Matrix[MatrixIndex][ m_Val[DC_Index][p]][0] = huffcode[p];
			m_DC_Matrix[MatrixIndex][ m_Val[DC_Index][p]][1] = huffsize[p];
		} ;

		p = 0;
		for (l = 1; l <= 16; l++) {
			for (i = 1; i <= m_Bits[AC_Index][l]; i++) {
				huffsize[p++] = l;
			} ;
		} ;
		huffsize[p] = 0;
		lastp = p;

		code = 0;
		si = huffsize[0];
		p = 0;
		while(huffsize[p] != 0) {
			while(huffsize[p] == si) {
				huffcode[p++] = code;
				code++;
			} ;
			code <<= 1;
			si++;
		} ;

		for (p = 0; p < lastp; p++) {
			m_AC_Matrix[MatrixIndex][m_Val[AC_Index][p]][0] = huffcode[p];
			m_AC_Matrix[MatrixIndex][m_Val[AC_Index][p]][1] = huffsize[p];
		} ;

	} ;
}

void ClENG_BlkEncoder::BeginEncode(M4ClBuffer *ao_OutStream) 
{
	m_BufferPutBuffer = m_BufferPutBits = 0 ;

	m_poOut = ao_OutStream ;

	//TEST
	//FBuffIt = new ClMIT_File ;
	//FBuffIt->Open( "c:\\kk6\\TEST.txt", ClMIT_File::OVERWRITE, ClMIT_File::WRITE)  ;
	//
}



void ClENG_BlkEncoder::BufferIt(m4int32_t ai_Code, m4int32_t ai_Size) 
{
	m4int32_t PutBuffer = ai_Code;
	m4int32_t PutBits = m_BufferPutBits;
	m4uint8_t c ;

	//TEST
	//m4char_t StrAux[200] ;
	//sprintf(StrAux,"Code %li Size %li\n", ai_Code, ai_Size );
	//FBuffIt->WStr(StrAux) ;
	//

	//PutBuffer=ai_Code ...=m_BufferPutBuffer, puede ser negativo
	//ai_Size es positivo (<=23?)

	PutBuffer &= (1 << ai_Size) - 1;
	PutBits += ai_Size ;
	PutBuffer <<= (24 - PutBits);
	PutBuffer |= m_BufferPutBuffer;

	//Put Bits debe ser >=0 && <=23.

	while(PutBits >= 8) {
		c = ((PutBuffer >> 16) & 0xFF);
		m_poOut->Append(&c,1) ;
		if (c == 0xFF) {
			c=0 ;
			m_poOut->Append(&c,1) ;
		}
		PutBuffer <<= 8;
		PutBits -= 8;
	} ;
	m_BufferPutBuffer = PutBuffer;
	m_BufferPutBits = PutBits;
}


void ClENG_BlkEncoder::FlushBuffer() 
{
	m4int32_t PutBuffer = m_BufferPutBuffer;
	m4int32_t PutBits = m_BufferPutBits;
	m4uint8_t c ;

	while (PutBits >= 8) {
		c = ((PutBuffer >> 16) & 0xFF);
		m_poOut->Append(&c,1) ;
		if (c == 0xFF) {
			c=0 ;
			m_poOut->Append(&c,1) ;
		} ;
		PutBuffer <<= 8;
		PutBits -= 8;
	} ;
	if (PutBits > 0) {
		c = ((PutBuffer >> 16) & 0xFF);
		m_poOut->Append(&c,1) ;
	} ;

}

void ClENG_BlkEncoder::Encode(m4int32_t ai_ZigZag[ClENG_DCT::N*ClENG_DCT::N], 
	m4int32_t ai_Prec, m4int32_t ai_DC_Code, m4int32_t ai_AC_Code) 
{
	m4int32_t temp, temp2, nbits, k, r, i;


	//TEST
	//m4char_t StrAux[200] ;
	//sprintf(StrAux,"ZigZag ? Prec %li DCCode %li ACCode %li \n", ai_Prec, ai_DC_Code, ai_AC_Code );
	//FBuffIt->WStr(StrAux) ;
	//

	//Parte DC

	temp = temp2 = ai_ZigZag[0] - ai_Prec;
	if(temp < 0) {
		temp = -temp;
		temp2--;
	} ;
	nbits = 0;
	while (temp != 0) {
		nbits++;
		temp >>= 1;
	} ;
	BufferIt( 
		m_DC_Matrix[ai_DC_Code][nbits][0], 
		m_DC_Matrix[ai_DC_Code][nbits][1] );
	if (nbits != 0) {
		BufferIt(temp2, nbits);
	} ;

	//Parte AC

	r = 0;

	for (k = 1; k < 64; k++) {
		if ((temp = ai_ZigZag[ClENG_JpgEncoder::m_JpegNaturalOrder[k]]) == 0) {
			r++;
		} else {
			while (r > 15) {
				BufferIt(
					m_AC_Matrix[ai_AC_Code][0xF0][0], 
					m_AC_Matrix[ai_AC_Code][0xF0][1]);
				r -= 16;
			} ;
			temp2 = temp;
			if (temp < 0) {
				temp = -temp;
				temp2--;
			} ;
			nbits = 1;
			while ((temp >>= 1) != 0) {
				nbits++;
			} ;
			i = (r << 4) + nbits;
			BufferIt(
				m_AC_Matrix[ai_AC_Code][i][0], 
				m_AC_Matrix[ai_AC_Code][i][1]);
			BufferIt(temp2, nbits);
            r = 0;
		} ;
	} ;

	if (r > 0) {
		BufferIt(
			m_AC_Matrix[ai_AC_Code][0][0], 
			m_AC_Matrix[ai_AC_Code][0][1]);
	} ;
}

//===========================================================================ClENG_JpgEncoder

const m4int32_t ClENG_JpgEncoder::m_CompID[ClENG_JpgEncoder::NUMBER_OF_COMPONENTS] =
{
	1,2,3
} ;

const m4int32_t ClENG_JpgEncoder::m_QtableNumber[ClENG_JpgEncoder::NUMBER_OF_COMPONENTS] =
{
	0,1,1
} ;

const m4int32_t ClENG_JpgEncoder::m_DCtableNumber[ClENG_JpgEncoder::NUMBER_OF_COMPONENTS] = 
{
	0,1,1
} ;

const m4int32_t ClENG_JpgEncoder::m_ACtableNumber[ClENG_JpgEncoder::NUMBER_OF_COMPONENTS] = 
{
	0,1,1
} ;

const m4int32_t ClENG_JpgEncoder::m_HsampFactor[ClENG_JpgEncoder::NUMBER_OF_COMPONENTS] =
{
	1,1,1
} ;

const m4int32_t ClENG_JpgEncoder::m_VsampFactor[ClENG_JpgEncoder::NUMBER_OF_COMPONENTS] = 
{
	1,1,1
} ;

const m4int32_t ClENG_JpgEncoder::m_MaxHsampFactor = 1 ;

const m4int32_t ClENG_JpgEncoder::m_MaxVsampFactor = 1 ;

const m4int32_t ClENG_JpgEncoder::m_JpegNaturalOrder[ClENG_DCT::N*ClENG_DCT::N] =
{
	0,  1,  8, 16,  9,  2,  3, 10,
	17, 24, 32, 25, 18, 11,  4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13,  6,  7, 14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63
} ;



ClENG_JpgEncoder::ClENG_JpgEncoder()
{
	m4int16_t C ;

	for (C=0; C<NUMBER_OF_COMPONENTS; ++C) {
		m_Component[C]=NULL ;
	} ;
}

void ClENG_JpgEncoder::End()
{
	m_DCT.End() ;
	m_BlkEncoder.End() ;
	DeleteComponent() ;
}

void ClENG_JpgEncoder::Init(m4int32_t ai_Quality) 
{
	ClENG_JpgEncoder::End();

	m_DCT.Init(ai_Quality) ;

	m_BlkEncoder.Init() ;
	m_Foreground.Init(0,"",0,0,0) ;
	m_Background.Init(0,"",0xFF,0xFF,0xFF) ;
}

void ClENG_JpgEncoder::ChangeQuality(m4int32_t ai_Quality)
{
	if ( m_DCT.GetQuality() != ai_Quality ) {
		m_DCT.Init(ai_Quality) ;
	} ;

}

void ClENG_JpgEncoder::DeleteComponent() 
{
	m4int16_t C ;

	for (C=0; C<NUMBER_OF_COMPONENTS; ++C) {
		if ( m_Component[C] ) {
			M4_DELETE_ARRAY( m_poInsp->m_poMemProf, MEM_GRP_RASTERG, 
				m_Component[C], m4float_t , m_compHeight[C]*m_compWidth[C] ) ;
		} ;
	} ;

}


void ClENG_JpgEncoder::WMarker(m4uint8_t * ai_Data) 
{
	m_poOut->Append(ai_Data,2) ;
}

void ClENG_JpgEncoder::WArray(m4uint8_t * ai_Data) 
{
	m_poOut->Append(ai_Data, 
		(((m4int32_t) (ai_Data[2] & 0xFF)) << 8) + (m4int32_t) (ai_Data[3] & 0xFF) + 2  ) ;
}


m4bool_t ClENG_JpgEncoder::Encode(ClENG_RasterMap *ai_Image, M4ClBuffer *ao_OutStream ) 
{
	m4bool_t SwOk=M4_TRUE ;
	m4int16_t y,c,x ;

	m_poOut = ao_OutStream ;

	if (!ai_Image->IsMapLoaded()) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	DeleteComponent() ;

	m_BlkEncoder.BeginEncode(m_poOut) ;

	//****Obtener info de la imagen

	m_ImageHeight	= ai_Image->GetRasterY()  ;
	m_ImageWidth	= ai_Image->GetRasterX()  ;


	//Hayar compWidth, compHeight, BlockWidth. BlockHeight, lastColumnIsDummy y lastRowIsDummy

	for (y = 0; y < NUMBER_OF_COMPONENTS; y++) {
		m_lastColumnIsDummy[y]=M4_FALSE;
		m_lastRowIsDummy[y]=M4_FALSE;
	} ;
	for (y = 0; y < NUMBER_OF_COMPONENTS; y++) {
		m_compWidth[y] = (((m_ImageWidth%8 != 0) ? ((m4int32_t) ceil(
			(m4double_t) m_ImageWidth/8.0))*8 : m_ImageWidth) / 
			m_MaxHsampFactor)* m_HsampFactor[y];
		if (m_compWidth[y] != ((m_ImageWidth/m_MaxHsampFactor)*m_HsampFactor[y])) {
			m_lastColumnIsDummy[y] = M4_TRUE;
		} ;
		m_BlockWidth[y] = (m4int32_t) ceil((m4double_t) m_compWidth[y]/8.0);

		m_compHeight[y] = (((m_ImageHeight%8 != 0) ? ((m4int32_t) ceil(
			(m4double_t) m_ImageHeight/8.0))*8: m_ImageHeight)/
			m_MaxVsampFactor) * m_VsampFactor[y];
		if (m_compHeight[y] != ((m_ImageHeight/m_MaxVsampFactor)*m_VsampFactor[y])) {
			m_lastRowIsDummy[y] = M4_TRUE;
		} ;
		m_BlockHeight[y] = (m4int32_t) ceil((m4double_t) m_compHeight[y]/8.0);
	} ;

	//Cargar datos grafico en formato YCbCr

	for (y = 0; y < NUMBER_OF_COMPONENTS; y++) {
		M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			m_Component[y], m4float_t, m_compHeight[y]*m_compWidth[y] ) ;
	} ;

	ai_Image->GrabYCbCrPixels(m_Component[0],m_Component[1],m_Component[2],
		0, m_compWidth[0], &m_Foreground, &m_Background ) ;

	//Reseteo los valores no rellenados de m_Component (sobrante de redondeos a N)

	for (c = 0; c < NUMBER_OF_COMPONENTS; c++) {
		for (y = m_ImageHeight; y < m_compHeight[c]; y++) {
			for (x = 0; x < m_compWidth[c]; x++) {
				SetValue( c, y, x, 0 ) ;
			} ;
		} ;
		for (x = m_ImageWidth; x < m_compWidth[c]; x++) {
			for (y = 0; y < m_compHeight[c]; y++) {
				SetValue( c, y, x, 0 ) ;
			} ;
		} ;
	} ;


	/*TEST->
	m4char_t StrAux[200] ;
	m4int16_t icomp;
	m4uint32_t NumberCount ;
	NumberCount=0;
	for (icomp= 0; icomp < NUMBER_OF_COMPONENTS; icomp++) {
		sprintf(StrAux,"Component %i", (int)icomp );
		FBuffIt->WStr(StrAux);
		for (y = 0; y < ai_Image->GetRasterY() ; y++) {
			sprintf(StrAux,"\nLinea %li\n", (long int)y );
			FBuffIt->WStr(StrAux);
			for (x = 0; x < ai_Image->GetRasterX() ; x++) {
				sprintf(StrAux,"%li ", (long int)GetValue(icomp,y,x));
				FBuffIt->WStr(StrAux) ;
				++NumberCount ;
			} ;
		} ;
		sprintf(StrAux,"\nNumberCount %li\n", NumberCount );
		FBuffIt->WStr(StrAux);
	} ;
	FBuffIt->Flush() ;
	//<-TEST*/
	
	//Al ataquerrl

	WHeaders() ;
	WData() ;
	WEOI() ;

	//TEST
	//FBuffIt->Close() ;
	//delete FBuffIt ;
	//FBuffIt=NULL ;
	//

exit:

	DeleteComponent() ;

	return SwOk ;
}


m4bool_t ClENG_JpgEncoder::Encode(ClENG_RasterMap *ai_Image, m4char_t *ai_pcFile ) 
{
	m4bool_t SwOk=M4_TRUE ;
	M4ClBuffer buffer ;
	ClMIT_File file ;
	

	buffer.Init(1024*4) ;
	if ( ! Encode(ai_Image,&buffer) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! file.Open(ai_pcFile, ClMIT_File::OVERWRITE, ClMIT_File::WRITE, ClMIT_File::TRY_OPEN, M4_FALSE) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	buffer.Flush( M4ClBuffer::WFileManip, &file )  ;
	file.Close() ;

exit:
	return SwOk ;
}

m4bool_t ClENG_JpgEncoder::Encode(m4char_t *ai_pcSourceImageFile, M4ClBuffer *ai_poBuff ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_RasterMap Image ;
	
	Image.Init() ;
	if ( ! Image.LoadFromFile(ai_pcSourceImageFile) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! Encode(&Image,ai_poBuff) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
exit:
	return SwOk ;
}


m4bool_t ClENG_JpgEncoder::Encode(m4char_t *ai_pcSourceImageFile, m4char_t *ai_pcFile ) 
{
	m4bool_t SwOk=M4_TRUE ;
	ClENG_RasterMap Image ;

	Image.Init() ;

	if ( ! Image.LoadFromFile(ai_pcSourceImageFile) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;

	if ( ! Encode(&Image,ai_pcFile) ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
exit:
	return SwOk ;
}



void ClENG_JpgEncoder::WHeaders() 
{
	m4int32_t i, j, index, offset, length ;
	ClENG_DCT::INDEX_DATA IndexData ;

	// Marcador SOI
	m4uint8_t SOI[] = { 0xFF, 0xD8};
	WMarker(SOI);

	//Cabecera

	m4uint8_t JFIF[] = {
        0xff, 0xe0, 0x00, 0x10, //Segmento (0xff) tipo 0xe0 (APP0 marker),size 0x00-0x10
		'J', 'F', 'I', 'F', 0x00, //Cadena fija que identifica JFIF
		0x01, 0x00,  //Major / minor rev numbers (major must be 1, minor 0..2)
		0x00, //units for x/y density (0-units in aspect ratio, 1-dots/inch, 2-dots/cm) 
		0x00, 0x01, //x density (0x00-0x01) must by <>0
		0x00, 0x01, //y density (0x00-0x01) must by <>0
		0x00, 0x00  //thumbnail width/height
					//n bytes for thumbnail, n=width*height*3 (RGB 24 bits)
	} ;
	WArray(JFIF);

	//Cadena comentario

	m4uint8_t *COM ;

	length = strlen( ENG_JPEG_COMMENT ) ;
	M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			COM, m4uint8_t, length + 4 ) ;
	COM[0] = 0xFF ; //Segmento ...
	COM[1] = 0xFE ; // ... tipo coment
	COM[2] = ((length >> 8) & 0xFF);  //... size ...
	COM[3] = (length & 0xFF) ; // ... = length ...
	memcpy( &COM[4], ENG_JPEG_COMMENT, length );
    WArray(COM);
	M4_DELETE_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			COM, m4uint8_t, length + 4 ) ;

	//DQT

	m4uint8_t DQT[ ClENG_DCT::N*ClENG_DCT::N*2 + 6 ] ;
	DQT[0] = (m4uint8_t) 0xFF; //Segmento ....
	DQT[1] = (m4uint8_t) 0xDB; // ... tipo DQT
	DQT[2] = (m4uint8_t) 0x00; // ... size ...
	DQT[3] = (m4uint8_t) 0x84; // ... = 0x00-0x84 
	for (i = 0, offset = 4 ; i < 2; i++) {
		if (i==0) {
			IndexData=ClENG_DCT::LUMIN ;
		} else {
			IndexData=ClENG_DCT::CHRO ;
		} ;
		DQT[offset++] = ((0 << 4) + i);
		for (j = 0; j < 64; j++) {
			DQT[offset++] = m_DCT.GetQu( IndexData, m_JpegNaturalOrder[j] ) ;
		}
	}
	WArray(DQT);

	//Comienzo de cabecera de trama

	m4uint8_t SOF[19] ;
	SOF[0] = 0xFF;  //Segmento ...
	SOF[1] = 0xC0;  // ... tipo SOF (Start Of Frame)
	SOF[2] = 0x00;  // size ...
	SOF[3] = 17; // ... = 17 bytes including this 2 size bytes
	SOF[4] = ClENG_JpgEncoder::PRECISION ;
	SOF[5] = ((m_ImageHeight >> 8) & 0xFF);
	SOF[6] = ((m_ImageHeight) & 0xFF);
	SOF[7] = ((m_ImageWidth >> 8) & 0xFF);
	SOF[8] = ((m_ImageWidth) & 0xFF);
	SOF[9] = ClENG_JpgEncoder::NUMBER_OF_COMPONENTS;
	index = 10;
	for (i = 0; i < SOF[9]; i++) {
		SOF[index++] = m_CompID[i];
		SOF[index++] = ((m_HsampFactor[i] << 4) + m_VsampFactor[i]);
		SOF[index++] = m_QtableNumber[i];
	} ;
	WArray(SOF);

	//DHT

	m4uint8_t DHT1[17], *DHT2=NULL, *DHT3=NULL, *DHT4=NULL;
	m4int32_t szDHT2, szDHT3, szDHT4 ;
	m4int32_t bytes, temp, oldindex, intermediateindex;
	length = 2;
	index = 4;
	oldindex = 4;

	szDHT4=4 ;
	M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			DHT4, m4uint8_t, szDHT4 ) ;

	DHT4[0] = 0xFF;
	DHT4[1] = 0xC4;
	for (i = 0; i < ClENG_BlkEncoder::AC_DC_ARR_SIZE; i++ ) {
		bytes = 0;
		DHT1[index++ - oldindex] = m_BlkEncoder.m_Bits[i][0];
		for (j = 1; j < 17; j++) {
			temp = m_BlkEncoder.m_Bits[i][j];
			DHT1[index++ - oldindex] =temp;
			bytes += temp;
		}
		intermediateindex = index;
		szDHT2=bytes ;
		M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			DHT2, m4uint8_t, szDHT2 ) ;
		for (j = 0; j < bytes; j++) {
			DHT2[index++ - intermediateindex] = m_BlkEncoder.m_Val[i][j];
		}
		szDHT3=index ;
		M4_NEW_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			DHT3, m4uint8_t, szDHT3 ) ;
		memcpy(&DHT3[0],				DHT4,	oldindex) ;
		memcpy(&DHT3[oldindex],			DHT1,	17 ) ;
		memcpy(&DHT3[oldindex + 17],	DHT2,	bytes ) ;
		M4_DELETE_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			DHT4, m4uint8_t, szDHT4 ) ;
		DHT4 = DHT3; szDHT4=szDHT3 ;
		oldindex = index;
		M4_DELETE_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
			DHT2, m4uint8_t, szDHT2 ) ;
	}
	DHT4[2] = (((index - 2) >> 8)& 0xFF);
	DHT4[3] = ((index -2) & 0xFF);
	WArray(DHT4);
	M4_DELETE_ARRAY( m_poInsp->m_poMemProf,MEM_GRP_RASTERG, 
		DHT4, m4uint8_t, szDHT4 ) ;


	//Comienzo cabecera Scan

	m4uint8_t SOS[14] ;
	SOS[0] = 0xFF;
	SOS[1] = 0xDA;
	SOS[2] = 0x00;
	SOS[3] = 12;
	SOS[4] = ClENG_JpgEncoder::NUMBER_OF_COMPONENTS;
	index = 5;
	for (i = 0; i < SOS[4]; i++) {
		SOS[index++] = m_CompID[i];
		SOS[index++] = (m_DCtableNumber[i] << 4) + m_ACtableNumber[i];
	}
	SOS[index++] = 0 ;	//Ss
	SOS[index++] = 63 ;	//Se
	SOS[index++] = (0 << 4) + 0 ;	//Ah, Al
	WArray(SOS);
}

void ClENG_JpgEncoder::WEOI() 
{
	m4uint8_t EOI[2] = { 0xFF, 0xD9};
	WMarker(EOI);
}

void ClENG_JpgEncoder::WData() 
{
	m4int32_t	offset, i, j, r, c,a ,b, temp = 0, iaux;
	m4int32_t	comp, xpos, ypos, xblockoffset, yblockoffset;
	m4float_t	dctArray1[ClENG_DCT::N][ClENG_DCT::N] ;
	m4double_t	dctArray2[ClENG_DCT::N][ClENG_DCT::N] ;
	m4int32_t	dctArray3[ClENG_DCT::N*ClENG_DCT::N] ;

	//Comienza en la esquina superior izquierda, comprime bloques de datos de 8x8
    //hasta terminar con la esquina inferior derecha

	m4int32_t	lastDCvalue[ClENG_JpgEncoder::NUMBER_OF_COMPONENTS] ;
	m4int32_t	zeroArray[ClENG_DCT::N*ClENG_DCT::N] ;
	m4int32_t	Width = 0, Height = 0;
	m4int32_t	nothing = 0, _not;
	m4int32_t	MinBlockWidth, MinBlockHeight;

	for(i=0;i<ClENG_DCT::N*ClENG_DCT::N;++i) zeroArray[i]=0 ;
	for(i=0;i<ClENG_JpgEncoder::NUMBER_OF_COMPONENTS;++i) lastDCvalue[i]=0 ;

		
	// Se incicializa MinBlockWidth y MinBlockHeight para asegurarse de que comienzan 
	// con valores mayores que los que realmente tendrá por el caso.

	MinBlockWidth = ((m_ImageWidth%8 != 0) ? (m4int32_t) (floor( 
		(m4double_t) m_ImageWidth/8.0) + 1)*8 : m_ImageWidth);
	MinBlockHeight = ((m_ImageHeight%8 != 0) ? (m4int32_t) (floor(
		(m4double_t) m_ImageHeight/8.0) + 1)*8: m_ImageHeight);
	for (comp = 0; comp < ClENG_JpgEncoder::NUMBER_OF_COMPONENTS; comp++) {
		if ( m_BlockWidth[comp] < MinBlockWidth ) MinBlockWidth = m_BlockWidth[comp] ;
		if ( m_BlockHeight[comp] < MinBlockHeight ) MinBlockHeight = m_BlockHeight[comp] ; 
	} ;

	xpos = 0;
	for (r = 0; r < MinBlockHeight; r++) {
		for (c = 0; c < MinBlockWidth; c++) {
			xpos = c*8;
			ypos = r*8;
			for (comp = 0; comp < ClENG_JpgEncoder::NUMBER_OF_COMPONENTS; comp++) {
				Width = m_BlockWidth[comp];
				Height = m_BlockHeight[comp];
				for(i = 0; i < m_VsampFactor[comp]; i++) {
					for(j = 0; j < m_HsampFactor[comp]; j++) {
						xblockoffset = j * 8;
						yblockoffset = i * 8;
						//TEST
						//m4char_t StrAux[200] ;
						//FBuffIt->WStr("DCT1 ") ;
						//
						for (a = 0; a < 8; a++) {
							for (b = 0; b < 8; b++) {
								dctArray1[a][b] =  GetValue( comp, 
									ypos + yblockoffset + a, xpos + xblockoffset + b ) ;
								//TEST
								//sprintf(StrAux,"%li ", (long int)dctArray1[a][b] );
								//FBuffIt->WStr(StrAux) ;
								//
							} ;
						} ;
						//TEST
						//FBuffIt->WStr("\n") ;
						//


// 
//						if ((!m_lastColumnIsDummy[comp] || c < Width - 1)
//							&& (!m_lastRowIsDummy[comp] || r < Height - 1))
//						{
							m_DCT.ForwardDCT(dctArray1,dctArray2) ;
							m_DCT.QuantizeBlock(dctArray2, m_QtableNumber[comp], dctArray3 ) ;
//						} else {
//							zeroArray[0] = dctArray3[0];
//							zeroArray[0] = lastDCvalue[comp];
//							for(iaux=0;iaux<ClENG_DCT::N*ClENG_DCT::N;++iaux) {
//								dctArray3[iaux] = zeroArray[iaux] ;
//							} ;
//						}
//
						m_BlkEncoder.Encode( 
							dctArray3, lastDCvalue[comp], 
							m_DCtableNumber[comp], m_ACtableNumber[comp]) ;
						lastDCvalue[comp] = dctArray3[0];
					}
				}
			}
		}
	}
	m_BlkEncoder.FlushBuffer();
}


m4bool_t ClENG_JpgEncoder::LocateJPG_HeaderData(
	ClMIT_File* ai_poFile, m4uint32_t* ai_uiWidth, m4uint32_t* ai_uiHeight, m4int16_t *ao_bpp)
{
	m4uint8_t Byte[10];
	m4uint32_t prevPos, nextSegment ;
	m4uint16_t segLen ;
	m4bool_t SwOk=M4_TRUE, SOF_Readed=M4_FALSE, APP0_Readed=M4_FALSE ;
		
	prevPos = ai_poFile->GetPos() ;
	
	//Lee SOI, obligatorio
	ai_poFile->SetPos(0);
	ai_poFile->R(&Byte[0], sizeof(m4uint8_t)); //Marca FF
	ai_poFile->R(&Byte[1], sizeof(m4uint8_t)); //SOI
	if (Byte[0] != 0xFF || Byte[1] != 0xd8 ) {
		SwOk=M4_FALSE ;
		goto exit ;
	} ;
	
	for( nextSegment=2; !ai_poFile->IsEOF() ; ) {
		ai_poFile->SetPos(nextSegment);
		//Lee marca de segmento, tipo y longitud del mismo
		ai_poFile->R(&Byte[0], sizeof(m4uint8_t)); //Marca FF
		ai_poFile->R(&Byte[1], sizeof(m4uint8_t)); //Tipo
		if( Byte[0] !=0xFF ) {
			//Error, debe ser un comienzo de segmento
			SwOk=M4_FALSE ;
			goto exit ;
		} ;
		//Antes de seguir, ver tipo; hay algunos segmentos sin size, hay que saltarselos!
		if ( Byte[1]==0x01 || Byte[1]==0xD0 || Byte[1]==0xD1 || Byte[1]==0xD2 || 
			 Byte[1]==0xD3 || Byte[1]==0xD4 || Byte[1]==0xD5 || Byte[1]==0xD6 ||
			 Byte[1]==0xD7 )
		{
			nextSegment += 2 ;
			continue ;
		} ;

		ai_poFile->R(&Byte[2], sizeof(m4uint8_t)); //MSB length
		ai_poFile->R(&Byte[3], sizeof(m4uint8_t)); //LSB length
		segLen = (Byte[2]*0x0100) + Byte[3] ;
		nextSegment += segLen+2 ; //+2 porque length no incluye Marca ni Tipo
 		if (Byte[1]==0xE0) {
			//APP0
			APP0_Readed=M4_TRUE ;
			ai_poFile->R(&Byte[0], sizeof(m4uint8_t)); 
			ai_poFile->R(&Byte[1], sizeof(m4uint8_t)); 
			ai_poFile->R(&Byte[2], sizeof(m4uint8_t)); 
			ai_poFile->R(&Byte[3], sizeof(m4uint8_t)); 
			ai_poFile->R(&Byte[4], sizeof(m4uint8_t)); 
			if ( ! ( Byte[0]=='J' && Byte[1]=='F' && Byte[2]=='I' && Byte[3]=='F' && Byte[4]==0x00 )  ) {
				//Error, debe ser esta marca
				SwOk=M4_FALSE ;
				goto exit ;
			} ;
 
		} else if(Byte[1]==0xC0) {
			//Segmento SOF (Start Of Frame)
			SOF_Readed=M4_TRUE ;
			ai_poFile->R(&Byte[0], sizeof(m4uint8_t)); //Data precission
			ai_poFile->R(&Byte[0], sizeof(m4uint8_t)); //Img height MSB/LSB
			ai_poFile->R(&Byte[1], sizeof(m4uint8_t));
			*ai_uiHeight = (Byte[0]*0x0100) + Byte[1];
			ai_poFile->R(&Byte[0], sizeof(m4uint8_t)); //Img width MSB/LSB
			ai_poFile->R(&Byte[1], sizeof(m4uint8_t));
			*ai_uiWidth =  (Byte[0]*0x0100) + Byte[1];
			ai_poFile->R(&Byte[0], sizeof(m4uint8_t)); //Number of components 
			*ao_bpp = Byte[0]==1 ? 8 : 24 ;
		};
		if ( APP0_Readed && SOF_Readed) break ;
	};
exit:
	ai_poFile->SetPos(prevPos) ;

	//PARCHEADO
	/*
	if ( !SOF_Readed || !APP0_Readed ) {
		SwOk=M4_FALSE ;
	} ;
	*/
	SwOk = APP0_Readed ;
	if ( SwOk && !SOF_Readed ) {
		*ao_bpp=-1 ;
	} ;

	if (!SwOk) {
		*GET_INSP()->m_poTrace << ClMIT_Msg(ClMIT_Msg::MSG_ERROR, GROUP_ID, 2163036,
			"The file %0:s is not a JPG file", ClMIT_Msg::MIXED_OUT)
			<< ai_poFile->GetName() << SEND_MSG ;
	} ;
	return SwOk ;

}

