//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:  CVM - DM
// File: Slices_fixed.cpp
// Project: CVM
// Author:  Meta Software M.S. , S.A
// Date: 29/12/97
// Language:C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
// This module defines...
//
//
//==============================================================================
#include "chlog.hpp"
#include "dmres.hpp"
#include "slices_fixed.hpp"
#include "node_knl.hpp"
#include "gm_intrf.hpp"
#include "m4define.hpp"
#include "chlog.hpp"
#include "v_contxt.hpp"

#include "m4mdrt.hpp"	// M4MDrt


//EDU:
// includes para que reconozca ItemDef_Interface y DatesInfo_Interface
 #include "itemdef.hpp"
 #include "chan_knl.hpp"

#define MinSizeBlock 40 //tamaño minimo del bloque
#define MinSize 8 //tamaño minimo del bloque
#define MinFS 5 //minimo espacio libre permitido
#define BlockFrac 2 //crecimento del bloque
#define Int32Size 4 //tamano del entero de 32 bits (4bytes)
#define DateSize 8  // tamaño del tipo m4date_t (8bytes)
#define HeadPlusEnd 16  // Cabecera de 8 y fecha de 8 simpre presentes cuando hay tramos
#define HeadPlusDate 16  // Cabecera de 8 y fecha de 8 simpre presentes cuando hay tramos
#define FlagSize 1  // Cabecera de 8 y fecha de 8 simpre presentes cuando hay tramos

//******************************************************************
// Funciones que pueden ser llamadas direectamente desde Item.Value

m4return_t _ClSlices_Fixed::Count (const StKernelContext &ai_context, m4uint32_t &ao_count, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;

    SliceHeader = *(m4uint32_t **)ao_valueadd;
	ao_count = SliceHeader[0];

	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::GetDescription (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address)
{
	return M4_ERROR;
}

m4return_t _ClSlices_Fixed::GetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t *date;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	date = (m4date_t *)(SliceHeader);
	ao_date = date[(ai_index * ((DateSize + SliceHeader[1]) / DateSize)) + 1];

	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::GetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t unitdate, *date;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	ai_context.GetUnitDate(unitdate);

    date = (m4date_t *)(SliceHeader);
	ao_date = date[((ai_index + 1) * ((DateSize + SliceHeader[1]) / DateSize)) + 1] - unitdate;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::Size (const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t &ao_size, void **ao_valueadd)
{
	m4return_t res;
	m4bool_t isnull;

	ao_size = 0;

    res = IsNull(ai_context, ai_index, isnull, ao_valueadd);

	if (!isnull)
    {
        m4int32_t isize;
		res = ai_context.pItemDef_Interface->MaxSize(isize);
        ao_size = isize;
	}

	return res;
}

m4return_t _ClSlices_Fixed::IsNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t &ao_isnull, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, NS;
	m4uint8_t *Offset, mask, NewOffset;
	m4return_t res;

    ao_isnull = M4_TRUE;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NS = SliceHeader[0];
	Offset = (m4uint8_t *)(SliceHeader);
	NewOffset = Offset[NS * (DateSize + SliceHeader[1]) + HeadPlusEnd + ai_index];
	mask = 4; //mask = 0000 0100

	NewOffset = NewOffset & mask;
	ao_isnull = ( NewOffset ? M4_TRUE : M4_FALSE );

	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::GetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t &ao_priority, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, NS;
	m4uint8_t *Offset, mask, NewOffset;
	m4return_t res;

    ao_priority = 3;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NS = SliceHeader[0];
	Offset = (m4uint8_t *)(SliceHeader);
	NewOffset = Offset[NS * (DateSize + SliceHeader[1]) + HeadPlusEnd + ai_index];
	mask = 3; //mask = 0000 0011 solo afecto al segundo trio
	NewOffset = NewOffset & mask;
	ao_priority = NewOffset;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::SetPriority (const StKernelContext &ai_context, m4uint32_t ai_index, priority_t ao_priority, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, NS;
	m4uint8_t *Offset, mask, *NewOffset;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	Offset = (m4uint8_t *)(SliceHeader);
	NS = SliceHeader[0];
	NewOffset = &Offset[NS * (DateSize + SliceHeader[1]) + HeadPlusEnd + ai_index];
	mask = (m4uint8_t)252; //mask = 1111 1100
	*NewOffset = ((*NewOffset) & mask) + ao_priority;
	
	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::SetNull (const StKernelContext &ai_context, m4uint32_t ai_index, m4bool_t ai_bSetSize, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, NS;
	m4uint8_t *Offset, mask, *NewOffset;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	Offset = (m4uint8_t *)(SliceHeader);

	NS = SliceHeader[0];
	NewOffset = &Offset[NS * (DateSize + SliceHeader[1]) + HeadPlusEnd + ai_index];
	mask = 4; //mask = 0000 0100
	*NewOffset = (*NewOffset) | mask;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::SetValue(const StKernelContext &ai_context, 
									  m4uint32_t ai_index, const void *ai_paddress, 
									  m4uint32_t ai_size,  m4uint32_t ai_offset, 
									  m4uint32_t &ao_retSize, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, size_to_set, MaxSize;
	m4int32_t ItemMaxSize;
	m4return_t res, resout;
    m4bool_t bhs;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

// De momento no se me ocurre nada mejor
    if (ai_paddress == 0)
        return SetNull (ai_context, ai_index, M4_TRUE, ao_valueadd);

	res = ai_context.pItemDef_Interface->MaxSize(ItemMaxSize);

	MaxSize = m4uint32_t(ItemMaxSize);
	if (ai_offset > MaxSize) { return M4_ERROR; }

	if (MaxSize < (ai_size + ai_offset)) {
		resout = M4_WARNING;
		size_to_set = MaxSize - ai_offset;
	}
	else {
		resout = M4_SUCCESS;
		size_to_set = ai_size;
	}
	res = ai_context.pMemory_Interface->SetValueByOffset(ai_context, 
		ai_paddress, size_to_set, 
		(ai_index * (DateSize + SliceHeader[1]) + ai_offset + HeadPlusDate),
		NULL, M4_TRUE, M4_TRUE);
	if (res != M4_SUCCESS)
		return M4_ERROR;

    ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, ao_valueadd, bhs);

    res = SetNoNull(ai_context, ai_index, ao_valueadd);
	ao_retSize = size_to_set;


	return resout;
}

m4return_t _ClSlices_Fixed::SetDescription(const StKernelContext &ai_context,
                                           m4uint32_t ai_index, const void *ai_paddress)
{
    return M4_ERROR;
}

m4return_t _ClSlices_Fixed::SetStartDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t *StartDate;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	StartDate = (m4date_t *)(SliceHeader);
    StartDate[(ai_index * ((DateSize + SliceHeader[1]) / DateSize)) + 1] = ai_date;


    return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::SetEndDate (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t ai_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t NewDate, *EndDate, unitdate;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	ai_context.GetUnitDate(unitdate);
	
	NewDate = ai_date + unitdate;
	EndDate = (m4date_t *)(SliceHeader);
	EndDate[((ai_index + 1) * ((DateSize + SliceHeader[1]) / DateSize)) + 1] = NewDate;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::Delete (const StKernelContext &ai_context, m4uint32_t &ai_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, SliceToDelete, NS, ai_size, NextSlice, NextFlag, FlagToDelete, flagsize;
	m4uint8_t *FlagsHeader;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	NS = SliceHeader[0];
	--NS;

	if (NS == 0) {
		return DeleteAll(ai_context, ao_valueadd);
	}

	FlagsHeader = (m4uint8_t *) SliceHeader;
	SliceToDelete = (ai_index * (DateSize + SliceHeader[1])) + DateSize;
	NextSlice = ((ai_index + 1) * (DateSize + SliceHeader[1])) + DateSize;
	ai_size = (NS - ai_index) * (DateSize + SliceHeader[1]) + DateSize + NS + 1;//ya se le ha restado uno a NS
	memmove((void *)&SliceHeader[(SliceToDelete)/Int32Size],
		(void *)&SliceHeader[(NextSlice)/Int32Size], ai_size);
	NextFlag = (NS * (DateSize + SliceHeader[1])) + HeadPlusEnd + ai_index + 1;
	FlagToDelete = (NS * (DateSize + SliceHeader[1])) + HeadPlusEnd + ai_index;
	flagsize = NS - ai_index + 1;
	memmove((void *)&FlagsHeader[FlagToDelete],	(void *)&FlagsHeader[NextFlag], flagsize);

    if (ai_index == (SliceHeader[0] - 1)) ai_index--;
	SliceHeader[0] = NS;

	return M4_SUCCESS;
}

m4return_t  _ClSlices_Fixed::DeleteAll (const StKernelContext &ai_context, void **ao_valueadd)
{
	m4bool_t hasslices = M4_FALSE;
	m4return_t res;

	res = ai_context.pMemory_Interface->SetSize(ai_context, MinSize);
    //despues de SetSize
	res = ai_context.pMemory_Interface->SetHasSlices(ai_context, hasslices);
	res = ai_context.pMemory_Interface->SetNull(ai_context, M4_TRUE);

	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::Insert (const StKernelContext &ai_context, m4uint32_t ai_index, m4uint32_t ai_count, void **ao_valueadd)
{
	m4bool_t hasslices, bhs;
	m4uint32_t Size, AddSize,FS, *SliceHeader, NS, i;
	m4return_t res;
	m4date_t *date;
	m4uint32_t n;
    void * paddress;

    res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, &paddress, hasslices);

    if  (!hasslices)
    {
		hasslices = M4_TRUE;
		res = ai_context.pMemory_Interface->SetSize(ai_context, MinSizeBlock);
		    res = ai_context.pMemory_Interface->SetHasSlices(ai_context, hasslices);    //despues de SetSize
		res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, (void**)&SliceHeader, hasslices);
		SliceHeader[0] = 0;
		SliceHeader[1] = M4_ROUND_TO_8 (ai_context.pItemDef_Interface->MaxSize());
		date = (m4date_t *) SliceHeader;
		date[1] = 0; //pongo la fecha final a cero (no me fio, puede afectar a los flags si uso el GM de Edu)
		date[2] = 0; //pongo los primeros 8 bytes para flags a cero, me aseguro.
	}
    else
    {
        SliceHeader = (m4uint32_t *)paddress;
    }



	res = Count(ai_context, n, (void **) &SliceHeader);
	if (ai_index > n)
    {
        m4uint32_t ItemHandle  = ai_context.pItemDef_Interface->GetHandle();
        ClCompiledMCR * pmcr   = ai_context.pItemDef_Interface->GetpCMCR ();

        DUMP_CHLOG_WARNINGF (M4_CH_DM_SLICEISEOF, M4ObjNodeItemI(pmcr, ItemHandle));

        return M4_ERROR;
    }


	res = ai_context.pMemory_Interface->GetSize(ai_context, Size);

	NS = SliceHeader[0];
	NS = NS + ai_count;
	AddSize = ai_count*(DateSize + SliceHeader[1]);
	AddSize = AddSize + ai_count;// hay que agregar ai_count bytes para flags
	FS = FreeSpace(ai_context, (void **)&SliceHeader);
	if (FS < AddSize)
    {
		res = ai_context.pMemory_Interface->SetSize(ai_context, (BlockFrac * (AddSize + Size)));
		res = ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, (void**)&SliceHeader, bhs);
	}

	i = ai_count;
	for ( ; i!=0; --i)  {
		res = SingleInsert(ai_context, SliceHeader, ai_index);
	}

    if (ao_valueadd)
        * ao_valueadd = SliceHeader;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::AllocSlices (const StKernelContext &ai_context, ClDateStack *ai_stack, void **ao_valueadd)
{
	m4uint32_t maxsize, blocksize, PosInBlock8, PosInBlock1, incr, *block4;
	m4uint16_t ai_pos, length;
	m4bool_t hasslices = M4_TRUE;
	m4return_t res = 0;
	m4date_t date, *block8;
	m4uint8_t *block1;
    m4bool_t bhs;

	maxsize = M4_ROUND_TO_8 (ai_context.pItemDef_Interface->MaxSize());
	length  = ai_stack->GetLength();
	blocksize = m4uint32_t ((((maxsize + DateSize + FlagSize) * (length - 1)) + HeadPlusEnd) * BlockFrac);
	
	res |= ai_context.pMemory_Interface->SetSize(ai_context, blocksize);
    res |= ai_context.pMemory_Interface->SetHasSlices(ai_context, hasslices);    //despues de SetSize
	res |= ai_context.pMemory_Interface->GetAddress_andhasslices(ai_context, (void**)&block8, bhs);

	if (res != M4_SUCCESS) {return M4_ERROR;}

	block1 = (m4uint8_t *)block8;
	block4 = (m4uint32_t *)block8;
	ai_pos = 0;
	PosInBlock8 = 1; //se comienza en los segundos 8 bytes
	PosInBlock1 = ((maxsize + DateSize) * (length - 1)) + HeadPlusEnd;
	incr = (maxsize / 8) + 1;

	while (ai_pos < (length - 1)) {
		res = ai_stack->GetElement(ai_pos, date);
		block8[PosInBlock8] = date;
		block1[PosInBlock1] = 7;
		PosInBlock8 += incr;
		PosInBlock1 ++;
		ai_pos ++;
	}

	res = ai_stack->GetElement(length - 1, date);
	block8[(incr * (length -1)) + 1] = date; //fecha final
	block4[0] = m4uint32_t (length - 1);
	block4[1] = m4uint32_t (maxsize);

	return M4_SUCCESS;
}

//******************************************************************
// Funciones llamdas indirectamente

m4return_t _ClSlices_Fixed::Serialize (ClGenericIODriver& IOD, m4pcchar_t ai_poPtrSlice, m4char_t ai_iCppType, void **ao_valueadd)
{
	m4uint32_t iNumSlices, iIndex=0;

	iNumSlices=*(m4uint32_t*)&ai_poPtrSlice[iIndex];
	iIndex+=sizeof(m4uint32_t);

	m4uint32_t iRoundedSize;

	iRoundedSize=*(m4uint32_t*)&ai_poPtrSlice[iIndex];

	iIndex+=sizeof(m4uint32_t);

	// Size of all the slices
	if (IOD.Write((m4uint32_t)(sizeof(m4uint32_t)+ // Num Slices
		sizeof(m4uint32_t)+ // Size
		iNumSlices*(iRoundedSize+sizeof(m4double_t))+ // Values
		sizeof(m4double_t)+ // Last date
		iNumSlices) // Flags
		)!=M4_SUCCESS)

		return M4_ERROR;

	if (IOD.Write(iNumSlices)!=M4_SUCCESS)
		return M4_ERROR;

	if (IOD.Write(iRoundedSize)!=M4_SUCCESS)
		return M4_ERROR;

	for (m4uint32_t i=0; i<iNumSlices; i++)
	{
		// Date
		if (IOD.Write(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
		return M4_ERROR;
		iIndex+=sizeof(m4double_t);

		// Data of item
		switch (ai_iCppType)
		{
			case M4CL_CPP_TYPE_STRING :
				if (IOD.Write((m4pchar_t)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
					return M4_ERROR;
				break;
 
			case M4CL_CPP_TYPE_NUMBER :
			case M4CL_CPP_TYPE_DATE :
				if (IOD.Write(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
					return M4_ERROR;
				break;
		}

		iIndex+=iRoundedSize;
	}

	// Last date
	if (IOD.Write(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
		return M4_ERROR;
	iIndex+=sizeof(m4double_t);

	// Flags
	if (IOD.Write((void*)&ai_poPtrSlice[iIndex],iNumSlices)!=M4_SUCCESS)
		return M4_ERROR;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::DeSerializedSize (ClGenericIODriver& IOD, m4uint32_t &ao_iSize)
{
	if (IOD.Read(ao_iSize)!=M4_SUCCESS)
		return M4_ERROR;

	// m_Size=ao_iSize;
 
	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::DeSerialize (ClGenericIODriver& IOD, m4pcchar_t ai_poPtrSlice, 
										 m4char_t ai_iCppType, m4uint32_t m_Size)
{
	m4uint32_t iNumSlices, iIndex=0;

	if (IOD.Read(iNumSlices)!=M4_SUCCESS)
		return M4_ERROR;

	*(m4uint32_t*)&ai_poPtrSlice[iIndex]=iNumSlices;
	iIndex+=sizeof(m4uint32_t);

	m4uint32_t iRoundedSize;

	if (IOD.Read(iRoundedSize)!=M4_SUCCESS)
		return M4_ERROR;

	*(m4uint32_t*)&ai_poPtrSlice[iIndex]=iRoundedSize;

	iIndex+=sizeof(m4uint32_t);

	for (m4uint32_t i=0; i<iNumSlices; i++)
	{
		// Date
		if (IOD.Read(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
			return M4_ERROR;
		iIndex+=sizeof(m4double_t);

// Data of item
		switch (ai_iCppType)
		{
			case M4CL_CPP_TYPE_STRING :
				if (IOD.Read((m4pchar_t)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
					return M4_ERROR;
				break;
 
			case M4CL_CPP_TYPE_NUMBER :
			case M4CL_CPP_TYPE_DATE :
				if (IOD.Read(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
					return M4_ERROR;
				break;
		}

		iIndex+=iRoundedSize;
	}

	// Last date
	if (IOD.Read(*(m4double_t*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
		return M4_ERROR;
	iIndex+=sizeof(m4double_t);

	// Flags
	m4uint32_t iAuxSize;

	if (IOD.ReadSize(iAuxSize)!=M4_SUCCESS)
		return M4_ERROR;

	if (iAuxSize!=iNumSlices)
		return M4_ERROR;

	if (IOD.ReadBuffer((void*)&ai_poPtrSlice[iIndex])!=M4_SUCCESS)
		return M4_ERROR;

	return M4_SUCCESS;
}
/*
m4return_t _ClSlices_Fixed::CopyItem(const StKernelContext &ai_Dest, StKernelContext &ai_Source, void **ao_valueadd)
{
    return M4_ERROR;
}
*/
m4return_t _ClSlices_Fixed::GetAddress_withstartaddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **aio_address, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, SliceAddr;
	m4return_t res;
	m4bool_t isnull;	

    SliceHeader = *(m4uint32_t **)aio_address;
    *aio_address = 0; //inicialmente a cero

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF_GETVALUE,
                        aio_address);
    if (res != M4_SUCCESS)
        return res;

	res = IsNull(ai_context, ai_index, isnull, (void **)&SliceHeader);

	if (!isnull) {
		SliceAddr = (ai_index * ((DateSize + SliceHeader[1]) / Int32Size)) + Int32Size;
		*aio_address = (void *)(&SliceHeader[SliceAddr]);
	}

	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::GetAddress (const StKernelContext &ai_context, m4uint32_t ai_index, void **ao_address, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, SliceAddr;
	m4return_t res;
	m4bool_t isnull;

	*ao_address = 0; //inicialmente a cero

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF_GETVALUE);
    if (res != M4_SUCCESS)
        return res;

	res = IsNull(ai_context, ai_index, isnull, ao_valueadd);

	if (!isnull) {
		SliceAddr = (ai_index * ((DateSize + SliceHeader[1]) / Int32Size)) + Int32Size;
		*ao_address = (void *)(&SliceHeader[SliceAddr]);
	}

	return M4_SUCCESS;
}
	
m4return_t _ClSlices_Fixed::GetEndDatePlusOne (const StKernelContext &ai_context, m4uint32_t ai_index, m4date_t &ao_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	m4date_t *date;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF,
                        ao_valueadd);
    if (res != M4_SUCCESS)
        return res;

	date = (m4date_t *)(SliceHeader);
	ao_date = date[((ai_index + 1) * ((DateSize + SliceHeader[1]) / DateSize)) + 1];

	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::GetFirstIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd)
{
	ao_index = 0;

	return M4_SUCCESS;
}

m4return_t _ClSlices_Fixed::GetLastIndex (const StKernelContext &ai_context, m4uint32_t &ao_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	
    SliceHeader = *(m4uint32_t **)ao_valueadd;
	ao_index = SliceHeader[0] - 1;

	return M4_SUCCESS;
}


m4return_t _ClSlices_Fixed::GetFirstStartDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd)
{
	return GetStartDate (ai_context, 0, ao_date, ao_valueadd);
}

m4return_t _ClSlices_Fixed::GetLastEndDate (const StKernelContext &ai_context, m4date_t &ao_date, void **ao_valueadd)
{
	m4uint32_t *SliceHeader;
	
    SliceHeader = *(m4uint32_t **)ao_valueadd;
	return GetEndDate (ai_context, SliceHeader[0] - 1, ao_date, ao_valueadd);
}


m4return_t _ClSlices_Fixed::FindByDate (const StKernelContext &ai_context, m4date_t ai_date, m4uint32_t &ao_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, index;
	m4date_t date;
	m4return_t res;
	
    SliceHeader = *(m4uint32_t **)ao_valueadd;

	index = 0;
	res = GetFirstStartDate(ai_context, date, ao_valueadd);
	if (ai_date < date) {
		ao_index = M4DM_EOF_INDEX;
		return M4_WARNING;
	}

	while (index < SliceHeader[0])
	{
		GetEndDate(ai_context, index, date, ao_valueadd);
        if (ai_date <= date)
            break;
		index++ ;
	}

 //---------->>
 // EDU: en caso de no encontrar el elemento
 // el comportamiento es: devolver en ao_index -1 y como retorno un warning
	//if (index >= (SliceHeader[0])) return M4_ERROR;
 if (index >= (SliceHeader[0])) {
  ao_index = M4DM_EOF_INDEX;
  return M4_WARNING;
 }
 //----------<<
	else ao_index = index;

	return M4_SUCCESS;
}

//******************************************************************
// Funciones internas de la clase

m4return_t _ClSlices_Fixed::SingleInsert(const StKernelContext &ai_context,
                                         m4uint32_t *SliceHeader, m4uint32_t ai_index,
                                         void **ao_valueadd)
{
	m4uint32_t NS, *SliceToInsert, ai_size, ActualSlice, NextSlice, length, i;
	m4uint32_t ActualFlag, NextFlag, sizeflag, ValueInt32;
	m4date_t *date, newdate;
	m4uint8_t *Flags;
	m4return_t res;
	
	NS = SliceHeader[0];
	
	SliceToInsert = (m4uint32_t *)(SliceHeader + (ai_index * ((DateSize + SliceHeader[1]) / Int32Size)) + 2);
	Flags = (m4uint8_t *)SliceHeader;
	length = M4_ROUND_TO_8(SliceHeader[0]); //Redondeo a 8 Bytes, por si acaso (no me fio)
	ai_size = (NS - ai_index) * (DateSize + SliceHeader[1]) + DateSize + length;
	ActualSlice = (ai_index * (DateSize + SliceHeader[1])) + DateSize;
	NextSlice = ((ai_index + 1) * (DateSize + SliceHeader[1])) + DateSize;
	memmove((void *)&SliceHeader[(NextSlice)/Int32Size],
		(void *)&SliceHeader[(ActualSlice)/Int32Size], ai_size);

	++NS;
	ActualFlag = (NS * (DateSize + SliceHeader[1])) + HeadPlusEnd + ai_index;
	NextFlag = ((NS * (DateSize + SliceHeader[1])) + HeadPlusEnd + ai_index + 1);
	sizeflag = NS - ai_index;
	memmove((void *)&Flags[NextFlag], (void *)&Flags[ActualFlag], sizeflag);

	Flags[ActualFlag] = 3; //prioridad 3
	SliceHeader[0] = NS;

	i = 0;
	ai_size = SliceHeader[1] / Int32Size;
	ValueInt32 = Int32Size + (ai_index * ((DateSize + SliceHeader[1]) / Int32Size));
	while (i < ai_size) {
		SliceHeader[ValueInt32 + i++] = 0;
	}
	newdate = 0;
	date = (m4date_t *)(SliceHeader);
	date[(ai_index * ((DateSize + SliceHeader[1]) / DateSize)) + 1] = newdate;//pongo la fecha a cero
	res = SetNull(ai_context, ai_index, M4_FALSE, (void **)&SliceHeader);

	return M4_SUCCESS;
}


m4uint32_t _ClSlices_Fixed::FreeSpace(const StKernelContext &ai_context, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, Size;
	m4return_t res;
	m4uint32_t FS;

    SliceHeader = *(m4uint32_t **)ao_valueadd;

	res = ai_context.pMemory_Interface->GetSize(ai_context, Size);
	FS = Size - SliceHeader[0] * (DateSize + SliceHeader[1]) - SliceHeader[0] - HeadPlusEnd; 
	return FS;
}

m4return_t _ClSlices_Fixed::SetNoNull (const StKernelContext &ai_context,
                                       m4uint32_t ai_index, void **ao_valueadd)
{
	m4uint32_t *SliceHeader, NS;
	m4uint8_t *Offset, mask, *NewOffset;
	m4return_t res;

    SliceHeader = *(m4uint32_t **)ao_valueadd;
    res = ValidateIndex(SliceHeader, ai_index, ai_context, M4_CH_DM_SLICEISEOF);
    if (res != M4_SUCCESS)
        return res;

	Offset = (m4uint8_t *)(SliceHeader);

	NS = SliceHeader[0];
	NewOffset = &Offset[NS * (DateSize + SliceHeader[1]) + HeadPlusEnd + ai_index];
	mask = (m4uint8_t) 251; //mask = 1111 1011 solo afecto al segundo trio

	*NewOffset = (*NewOffset) & mask;

	return M4_SUCCESS;
}
