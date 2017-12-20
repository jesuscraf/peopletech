// DinArray.h: interface for the DinArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DINARRAY_H__F63BDCC3_9FF5_11D2_BCA7_00A0248A69E8__INCLUDED_)
#define AFX_DINARRAY_H__F63BDCC3_9FF5_11D2_BCA7_00A0248A69E8__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//#include <afx.h>
// Clase de Array dinamico
const int tama = 10;

//##ModelId=3973073D0268
class DinArray  
{
private:
	//##ModelId=3973073D027D
	void** array;
	//##ModelId=3973073D027C
	int size;
	//##ModelId=3973073D0272
	int cursor;

public:
//	static DinArray* instance();

	//##ModelId=3973073D02BB
	DinArray();
	//##ModelId=3973073D02B9
	virtual ~DinArray();
	//##ModelId=3973073D02B1
	int anade(void *);
	//##ModelId=3973073D02A6
	int borra(void *);
	//##ModelId=3973073D02AF
	int borra(int);
	//##ModelId=3973073D02A5
	void inicializa();
	//##ModelId=3973073D02A4
	void * next();
	//##ModelId=3973073D0293
	int index(){return cursor;}
	//##ModelId=3973073D0292
	void * actual(){return array[cursor];}
	//##ModelId=3973073D0288
	void modifica(int ri,void * new_element);
	//##ModelId=3973073D0286
	void *operator[](int);

	//##ModelId=3973073D027E
	int count();
};

#endif // !defined(AFX_DINARRAY_H__F63BDCC3_9FF5_11D2_BCA7_00A0248A69E8__INCLUDED_)
