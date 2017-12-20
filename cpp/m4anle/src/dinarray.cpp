// DinArray.cpp: implementation of the DinArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "dinarray.h"
#include <afx.h>
#include <stdlib.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DinArray::DinArray()
{

	array = new void*[tama];

	for(int i=0;i<tama;i++)
		array[i]= (void*)0;
	size = tama;
	cursor=0;
}

DinArray::~DinArray()
{
	delete array;
}

//static DinArray* m_pDinArray = NULL;
/**
* Metodo static del Singleton
*/
//DinArray* DinArray::instance()
//{
	// SI hay que crearlo
//	if ( m_pDinArray == NULL ) {
//		m_pDinArray = new DinArray();
//	}
//	return m_pDinArray;
//}


int DinArray::anade(void * new_element){
	int i = 0;

	for(i=0; i<size; i++)
		if (array[i]==(void*)0){
			array[i]=new_element;
			return i;
		}
		int tempsize = size + tama;
		void **temp = new void*[tempsize];
		for (i=0;i<size;i++)
			temp[i]=array[i];
		for (;i<tempsize;i++)
			temp[i]=(void*)0;
		temp[i=size]=new_element;
		delete array;
		array = temp;
		size = tempsize;
		return i;
}

int DinArray::borra(void * valor){
	for (int i=0;i<size;i++)
		if(array[i]==valor) {
			array[i]=(void*)0;
			return 1;
		}
		return 0;
}

int DinArray::borra(int ri){
	for (int i=0;i<ri; i++)
		array[i]=(void*)0;
	size = tama;
	cursor=0;
	return 1;
}
void DinArray::inicializa(){
	cursor=0;
	while((array[cursor]==(void*)0)&&(cursor<size-1))
		cursor++;
}

void * DinArray::next(){
	if(cursor==size-1)
		return (void*)0;
	while(array[++cursor]==(void*)0)
		if(cursor==size-1)
			return (void*)0;
		return array[cursor];
}

void * DinArray::operator[](int x){
	if(x<0||x>=size)
		return 0;
	return array[x];
}

int DinArray::count(){
	int cnt=0;
	for(int x=0;x<size;x++)
		if (array[x])
			cnt++;
		return cnt;
}

void DinArray::modifica(int ri,void * new_element)
{
	array[ri]=new_element;

}