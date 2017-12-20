#include "dsitem.hpp"
#include "m4stl.hpp"

#include "rm.hpp"
#include "vrb.hpp"
#include "file_misc.hpp"
#include "file_manager.hpp"
#include "sizer.hpp"
#include "chlog.hpp"
#include "csres.hpp"
#include "dsres.hpp"
#include "emd.hpp"


////////////////////////////////////////////////////////////////////////////////
// class BlobData
////////////////////////////////////////////////////////////////////////////////


ClBlobData::ClBlobData(m4uint32_t ai_Size) :
    m_Size(ai_Size),
    m_RefCount(0),
    m_pData(NULL)
{
    if (ai_Size)
    {
        m_pData = new m4uint8_t[ai_Size];
        M4_ASSERT(m_pData);
        if (m_pData == NULL)
            ai_Size = 0;
    }
}

ClBlobData::~ClBlobData(void)
{
    delete m_pData;
}

void ClBlobData::Destroy(void)
{
    if (--m_RefCount == 0)
        delete this;
}

m4return_t ClBlobData::GetUsedMemory(ClSize& ao_size) const
{
    ao_size += sizeof(ClBlobData);

    if (m_pData != NULL)
        ao_size += sizeof(m_pData[0]) * m_Size;

    return M4_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
// class ClBlobWithReferences
////////////////////////////////////////////////////////////////////////////////


ClBlobWithReferences::ClBlobWithReferences() :
    m_pBlobData(NULL)
{
}

ClBlobWithReferences::ClBlobWithReferences(const ClBlobWithReferences& ai_oSource)
{
   // Copio la parte dinamica, aumentando las referencias
    m_pBlobData = ai_oSource.m_pBlobData;

    if (m_pBlobData != NULL)
        m_pBlobData->m_RefCount++;

}

ClBlobWithReferences::~ClBlobWithReferences()
{
    SetpBlobData(NULL);
}

ClBlobWithReferences& ClBlobWithReferences::operator= (const ClBlobWithReferences& ai_oSource)
{
    if (this == &ai_oSource)
        return *this;

    // Copio la parte dinamica, aumentando las referencias
    SetpBlobData(ai_oSource.m_pBlobData);

    return *this;
}

m4int32_t ClBlobWithReferences::SetData(const void* const ai_pData,
                                        const m4uint32_t  ai_Size,
                                        const m4uint32_t  ai_Offset)
{
    ClBlobData* pNewBlobData = NULL;
    if (ai_pData != NULL)
    {
        m4uint32_t iLastOffset = ai_Offset + ai_Size;

        // Getting the memory for the new blob
        m4uint32_t iNewSize;
        if (m_pBlobData)
            iNewSize = max(iLastOffset, m_pBlobData->m_Size);
        else
            iNewSize = iLastOffset;
    
        // Siempre necesito reservar un nuevo blob para no perder las otras referencias
        pNewBlobData = new ClBlobData(iNewSize);
        M4_ASSERT(pNewBlobData && pNewBlobData->m_pData);
        if (pNewBlobData == NULL || pNewBlobData->m_pData == NULL)
            return -1;

        // Copy the old data
        if (m_pBlobData)
            memcpy(pNewBlobData->m_pData, m_pBlobData->m_pData, m_pBlobData->m_Size);
    
        // Copy the new data
        memcpy(pNewBlobData->m_pData + ai_Offset, ai_pData, ai_Size);
    }

    // Assign the new blobData
    SetpBlobData(pNewBlobData);

    return ai_Size;
}

m4int32_t ClBlobWithReferences::GetData(void*      const ao_pData,
                                        const m4uint32_t ai_MaxSize,
                                        const m4uint32_t ai_Offset) const
{
    M4_ASSERT(ao_pData);
    
    if (m_pBlobData == NULL)
        return 0;
    
    if (ai_Offset > m_pBlobData->m_Size)
        return -1;
    
    m4uint32_t iBytesToCopy;
    if (ai_Offset + ai_MaxSize <= m_pBlobData->m_Size)
        iBytesToCopy = ai_MaxSize;
    else
        iBytesToCopy = m_pBlobData->m_Size - ai_Offset;
    
    memcpy(ao_pData, m_pBlobData->m_pData + ai_Offset, iBytesToCopy);
        
    return iBytesToCopy;
}

const void* const ClBlobWithReferences::GetDataAddress(const m4uint32_t ai_Offset) const
{
    if (m_pBlobData == NULL || m_pBlobData->m_pData == NULL)
        return NULL;

    if (ai_Offset > m_pBlobData->m_Size)
        return NULL;

    return m_pBlobData->m_pData + ai_Offset;
}

void ClBlobWithReferences::SetpBlobData(ClBlobData* const ai_pBlobData)
{
    if (m_pBlobData != NULL)
        m_pBlobData->Destroy();

    m_pBlobData = ai_pBlobData;

    if (m_pBlobData != NULL)
        m_pBlobData->m_RefCount++;
}

m4return_t ClBlobWithReferences::SetSize(const m4uint32_t ai_Size)
{
    // Siempre necesito reservar un nuevo blob para no perder el oldValue
    ClBlobData* pNewBlobData = new ClBlobData(ai_Size);
    M4_ASSERT(pNewBlobData && pNewBlobData->m_pData);
    if (pNewBlobData == NULL || pNewBlobData->m_pData == NULL)
        return M4_ERROR;

    // Copy the old data
    if (m_pBlobData)
        memcpy(pNewBlobData->m_pData, m_pBlobData->m_pData, min(m_pBlobData->m_Size, ai_Size));
    
    // Assign the new blobData
    SetpBlobData(pNewBlobData);

    return M4_SUCCESS;
}

m4uint32_t ClBlobWithReferences::GetSize(void) const
{
    if (m_pBlobData == NULL)
        return 0;

    return m_pBlobData->m_Size;
}

m4return_t ClBlobWithReferences::GetUsedMemory(ClSize& ao_size) const
{
    ao_size += sizeof(ClBlobWithReferences);

    // ClSerializableObject;    No es de esta dll

    if (m_pBlobData != NULL)
    {
        if (m_pBlobData->GetUsedMemory(ao_size) != M4_SUCCESS)
            return M4_ERROR;
    }

    return M4_SUCCESS;
}

m4return_t ClBlobWithReferences::Serialize(ClGenericIODriver& IOD)
{
    m4bool_t bExistsPreviously;
    if (IOD.Write(m_pBlobData, &bExistsPreviously) != M4_SUCCESS)
        return M4_ERROR;
        
    // Si todavia no me he serializado, me serializo yo
    if (bExistsPreviously == M4_FALSE)
    return IOD.Write((void*)GetDataAddress(), GetSize());

    return M4_SUCCESS;
}

m4return_t ClBlobWithReferences::DeSerialize(ClGenericIODriver& IOD)
{
    ClBlobData* pNewBlobData;
    m4bool_t bExistsPreviously;

    if (IOD.Read(((void**)&pNewBlobData), &bExistsPreviously) != M4_SUCCESS)
        return M4_ERROR;

    if (pNewBlobData == NULL && bExistsPreviously == M4_FALSE)
    {
        // Es la primera vez
        m4uint32_t AuxOffset = IOD.GetActualOffset();
        
    m4uint32_t iSize;
    if (IOD.ReadSize(iSize) != M4_SUCCESS)
        return M4_ERROR;

    // Siempre necesito reservar un nuevo blob para no perder las otras referencias
        pNewBlobData = new ClBlobData(iSize);
    M4_ASSERT(pNewBlobData && pNewBlobData->m_pData);
    if (pNewBlobData == NULL || pNewBlobData->m_pData == NULL)
        return M4_ERROR;
    
    if (IOD.ReadBuffer(pNewBlobData->m_pData) != M4_SUCCESS)
    {
        pNewBlobData->Destroy();
        return M4_ERROR;
    }

        IOD.StoreAddress(pNewBlobData, AuxOffset);
    }

    // Assign the new blobData
    SetpBlobData(pNewBlobData);

    return M4_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
// class ClDsItemBlob
////////////////////////////////////////////////////////////////////////////////


ClDsItemBlob::ClDsItemBlob(ClItemRecordManager* const ai_pRM,
                           m4uint8_t*           const ai_pRecordBuffer,
                           m4uint16_t           const ai_ItemPos) :
    m_pRM(ai_pRM),
    m_pRecordBuffer(ai_pRecordBuffer),
    m_ItemPos(ai_ItemPos)
{
    M4_ASSERT(ai_pRM);
    M4_ASSERT(ai_pRecordBuffer);
}

ClDsItemBlob::ClDsItemBlob(const ClDsItemBlob& ai_oSource,
                           m4uint8_t*    const ai_pRecordBuffer) :
    ClBlobWithReferences(ai_oSource),
    m_pRM(ai_oSource.m_pRM),
    m_pRecordBuffer(ai_pRecordBuffer),  // OJO no puedo copiar el pRecordBuffer porque estaria apuntando al mismo buffer
    m_ItemPos(ai_oSource.m_ItemPos)
{
    M4_ASSERT(ai_pRecordBuffer);
}

ClDsItemBlob::~ClDsItemBlob(void)
{
    // Dejo que se llame al destructor de la clase base
}

ClDsItemBlob& ClDsItemBlob::operator= (const ClDsItemBlob& ai_oSource)
{
    if (this == &ai_oSource)
        return *this;

    ClBlobWithReferences::operator= (ai_oSource);

    m_pRM           = ai_oSource.m_pRM;
    m_pRecordBuffer = ai_oSource.m_pRecordBuffer;
    m_ItemPos       = ai_oSource.m_ItemPos;

    return *this;
}

m4int32_t ClDsItemBlob::SetValue(const void* const ai_pValue,
                                 const m4uint32_t  ai_Size,
                                 const m4uint32_t  ai_Offset)
{
    m4int32_t iReturnSize = SetData(ai_pValue, ai_Size, ai_Offset);
    
    // Si la operacion se completo con exito, actulizamos el estado del flag de nulo
    if (iReturnSize == (m4int32_t)ai_Size)
        SetNull((ai_pValue == NULL) ? M4_TRUE : M4_FALSE);

    return iReturnSize;
}

m4int32_t ClDsItemBlob::GetValue(void*      const ao_pValue,
                                 m4bool_t&        ao_IsNull,
                                 const m4uint32_t ai_MaxSize,
                                 const m4uint32_t ai_Offset) const
{
    M4_ASSERT(ao_pValue);

    ao_IsNull = GetNull();

    return GetData(ao_pValue, ai_MaxSize, ai_Offset);
}

void const * const ClDsItemBlob::GetValueAddress(const m4uint32_t ai_Offset) const
{
    if (GetNull() == M4_TRUE)
        return NULL;

    return GetDataAddress(ai_Offset);
}

m4return_t ClDsItemBlob::SetSize(const m4uint32_t ai_Size)
{
    m4return_t iRet = ClBlobWithReferences::SetSize(ai_Size);

    if (iRet == M4_SUCCESS)
        SetNull(M4_FALSE);  // El item ya no es null

    return iRet;
}

m4uint32_t ClDsItemBlob::GetSize(void) const
{
    if (GetNull() == M4_TRUE)
        return 0;

    return ClBlobWithReferences::GetSize();
}

size_t ClDsItemBlob::GetStaticSize(const ClHandle&      ai_iItemHandle, 
                                   const ClCompiledMCR& ai_oMetaChannel)
{
    return sizeof(ClDsItemBlob);
}

m4return_t ClDsItemBlob::GetUsedMemory(ClSize& ao_size) const
{
    ao_size += sizeof(ClDsItemBlob);

    ao_size -= sizeof(ClBlobWithReferences);
    if (ClBlobWithReferences::GetUsedMemory(ao_size) != M4_SUCCESS)
        return M4_ERROR;

    return M4_SUCCESS;
}

m4return_t ClDsItemBlob::SetNull(const m4bool_t ai_bIsNull)
{
    if (ai_bIsNull == M4_TRUE)
        SetpBlobData(NULL);
    else
    {
        if (m_pBlobData == NULL)
        {
            // SITUACION ERRONEA !!!!!
            M4_ASSERT(0);
            return M4_ERROR;
        }
    }

    // Pongo la flag de nulo en el registro
    m_pRM->SetItemNullLL(m_pRecordBuffer, m_ItemPos, ai_bIsNull);

    return M4_SUCCESS;
}

m4bool_t ClDsItemBlob::GetNull(void) const
{
    return m_pRM->GetItemNull(m_pRecordBuffer, m_ItemPos);
}

void ClDsItemBlob::SetHasSlices(const m4bool_t ai_bHasSlices)
{
    m_pRM->SetItemHasSlices(m_pRecordBuffer, m_ItemPos, ai_bHasSlices);
}

m4bool_t ClDsItemBlob::GetHasSlices(void) const
{
    return m_pRM->ItemHasSlices(m_pRecordBuffer, m_ItemPos);
}

void ClDsItemBlob::SetPriority(const m4int8_t ai_iPriority)
{
    m_pRM->SetItemPriority(m_pRecordBuffer, m_ItemPos, ai_iPriority);
}

m4int8_t ClDsItemBlob::GetPriority(void) const
{
    return m_pRM->GetItemPriority(m_pRecordBuffer, m_ItemPos);
}

ClItemType ClDsItemBlob::GetType(void) const
{
    return ITEM_TYPE_OTHER;
}


////////////////////////////////////////////////////////////////////////////////
// class ClBlobFileDescription
////////////////////////////////////////////////////////////////////////////////


ClBlobFileDescription::ClBlobFileDescription()
{
    // Dejo que acute el constructor de la clase base
}

ClBlobFileDescription::ClBlobFileDescription(m4pcchar_t const ai_pDescription)
{
	m4uint32_t iDataSize;
	if (ai_pDescription == NULL)
		iDataSize = 0;
	else
		iDataSize = strlen(ai_pDescription) + 1;

    SetData(ai_pDescription, iDataSize);
}

ClBlobFileDescription::~ClBlobFileDescription()
{
    // Dejo que acute el destructor de la clase base
}

ClBlobFileDescription& ClBlobFileDescription::operator= (const ClBlobFileDescription& ai_oSource)
{
    if (this == &ai_oSource)
        return *this;

    ClBlobWithReferences::operator= (ai_oSource);

    return *this;
}

m4return_t ClBlobFileDescription::SetDescription(m4pcchar_t const ai_pDescription)
{
	m4int32_t iDataSize;
	if (ai_pDescription == NULL)
		iDataSize = 0;
	else
		iDataSize = strlen(ai_pDescription) + 1;

    return (SetData(ai_pDescription, iDataSize) == iDataSize) ? M4_SUCCESS : M4_ERROR;
}

m4pcchar_t const ClBlobFileDescription::GetDescription(void)
{
    return (m4pcchar_t)GetDataAddress();
}
   
m4return_t ClBlobFileDescription::Serialize(ClGenericIODriver& IOD)
{
    return ClBlobWithReferences::Serialize(IOD);
}

m4return_t ClBlobFileDescription::DeSerialize(ClGenericIODriver& IOD)
{
    return ClBlobWithReferences::DeSerialize(IOD);
}

m4return_t ClBlobFileDescription::GetUsedMemory(ClSize& ao_size) const
{
    ao_size += sizeof(ClBlobFileDescription);

    ao_size -= sizeof(ClBlobWithReferences);
    if (ClBlobWithReferences::GetUsedMemory(ao_size) != M4_SUCCESS)
        return M4_ERROR;

    return M4_SUCCESS;
}


////////////////////////////////////////////////////////////////////////////////
// class ClDsItemBlobFile
////////////////////////////////////////////////////////////////////////////////


ClDsItemBlobFile::ClDsItemBlobFile(ClVectorRecordBase*  const ai_pVRB,
                                   ClItemRecordManager* const ai_pRM,
                                   m4uint8_t*           const ai_pRecordBuffer,
                                   m4uint16_t           const ai_ItemPos,
                                   m4pcchar_t           const ai_pDescription) :
    ClDsItemBlob(ai_pRM, ai_pRecordBuffer, ai_ItemPos),
    m_Description(ai_pDescription),
    m_pVRB(ai_pVRB)
{
    M4_ASSERT(ai_pVRB);
}

ClDsItemBlobFile::ClDsItemBlobFile(const ClDsItemBlobFile& ai_oSource,
                                   m4uint8_t*        const ai_pRecordBuffer) :
    ClDsItemBlob(ai_oSource, ai_pRecordBuffer),
    m_Description(ai_oSource.m_Description),
    m_pVRB(ai_oSource.m_pVRB)
{
    // Me intereso por el nuevo valor
    SetInterestInFile(GetValueAddress());
}

ClDsItemBlobFile::~ClDsItemBlobFile()
{
    // Dejo de tener interes por el fichero
    UnSetInterestInFile(GetValueAddress());
}

m4return_t ClDsItemBlobFile::CreateTempFile(m4pchar_t const ai_pDirectory, 
                                            m4pchar_t const ai_pExtension)
{
	/* Bug 0201071
    // Dejo de mostrar interes por el fichero que tengo ahora
    if (UnSetInterestInFile(GetValueAddress()) != M4_SUCCESS)
        return M4_ERROR;
	*/

    // Creo un fichero temporal (con esto me intereso por el fichero)
    ClFileName oFileName;
    if (ClFileManager::Instance()->CreateTempFile(oFileName, NULL, ai_pExtension, ai_pDirectory) != M4_SUCCESS)
        return M4_ERROR;

    // Asigno el nombre del fichero temporal al item
    m4char_t sTempName[M4_MAX_PATH];
    
    if (oFileName.GetFileName(sTempName) != M4_SUCCESS)
        return M4_ERROR;

    m4int32_t iFileNameLength = oFileName.GetLength();

    // Para indicarle al registro que he cambiando tengo que buscar mi indice en el VRB
    m4bool_t bIAmOldValue;
    regindex_t iMyIndexInVRB = m_pVRB->GetIndexByRecordAddress(m_pRecordBuffer, bIAmOldValue);
    if (iMyIndexInVRB == -1)
        return M4_ERROR;

    // Tengo que atacar desde arriba para que el registro se entere de que se ha modificado
    // (con esto tomo interes sobre el fichero y sobre los eventos)
    if (m_pVRB->SetItemValueByPosAndOffset(m_ItemPos, sTempName, iMyIndexInVRB, 0, iFileNameLength, NULL, M4_TRUE, M4_FALSE) != M4_SUCCESS)
        return M4_ERROR;

    // Me he interesado dos veces sobre el fichero, asi que tengo que perder un interes
    ClFileManager::Instance()->UnSetInterestInFile(sTempName);

    /*
    // Error si no consigo asignar entero el nuevo valor
    if (ClDsItemBlob::SetValue(sTempName, iFileNameLength) != iFileNameLength)
        return M4_ERROR;

    // Muestro interes por los eventos del fichero
    if (ClFileEventBroker::Instance()->SetInterestInFile(oFileName, this) != M4_SUCCESS)
        return M4_ERROR;

    // Actualizo el estado del fichero
    m_oFileState.SetFileName(sTempName);
    */

    return M4_SUCCESS;
}

m4return_t ClDsItemBlobFile::GetState(ClFileState& ao_oFState) const
{
    m4char_t* pValue = (m4char_t*)GetValueAddress();
    M4_ASSERT(pValue);
    if (pValue == NULL)
        return M4_ERROR;

    ao_oFState.SetFileName(pValue);
	return M4_SUCCESS;
	
}

m4bool_t ClDsItemBlobFile::StateHasChanged(void) const
{
    ClFileState oFileState((m4char_t*)GetValueAddress());
    
    return (m_oFileState != oFileState);
}

void ClDsItemBlobFile::ChangedMask(ClComparisonResultMask& ao_oChangedMask)
{
    m_oFileState.CompareAndSetMask(ClFileState((m4char_t*)GetValueAddress()), ao_oChangedMask);
}

ClDsItemBlobFile& ClDsItemBlobFile::operator= (const ClDsItemBlobFile& ai_oSource)
{
    if (this == &ai_oSource)
        return *this;

    // Dejo de mostrar interes por el fichero que tengo ahora
    UnSetInterestInFile(GetValueAddress());

    ClDsItemBlob::operator= (ai_oSource);

    // Me intereso por el nuevo valor
    SetInterestInFile(GetValueAddress());

    m_Description = ai_oSource.m_Description;

    return *this;
}

m4int32_t ClDsItemBlobFile::SetValue(const void* const ai_pValue,
                                     const m4uint32_t  ai_Size,
                                     const m4uint32_t  ai_Offset)
{
    M4_ASSERT(ai_pValue);

    // Antes de nada tengo que comprobar que el fichero que me pasan existe
    if (ai_pValue == NULL || ClFileState((const m4char_t*)ai_pValue).IsFile() == M4_FALSE)
        return -1;

    // Si el nombre es el mismo no hago nada
    m4char_t* pOldValue = (m4char_t*)GetValueAddress();
    if (pOldValue != NULL)
    {
        // Si me van a asignar el mismo valor no hace falta que haga nada
        if (strcmp((m4char_t*)ai_pValue, pOldValue) == 0)
            return ai_Size;

        // Dejo de mostrar interes por el fichero que tengo ahora
        if (UnSetInterestInFile(pOldValue) != M4_SUCCESS)
        return -1;
    }

    // Error si no consigo asignar entero el nuevo valor
    if (ClDsItemBlob::SetValue(ai_pValue, ai_Size, ai_Offset) != (m4int32_t)ai_Size)
        return -1;

    // Me intereso por el nuevo valor
    if (SetInterestInFile(ai_pValue) != M4_SUCCESS)
        return -1;

    // Actualizo el estado del fichero
    m_oFileState.SetFileName((m4char_t*)ai_pValue);

    return ai_Size;
}

size_t ClDsItemBlobFile::GetRealSize(void)
{
    return 0;
}

size_t ClDsItemBlobFile::GetStaticSize(const ClHandle&      ai_iItemHandle, 
                                       const ClCompiledMCR& ai_oMetaChannel)
{
    return sizeof(ClDsItemBlobFile);
}

m4return_t ClDsItemBlobFile::SetNull(const m4bool_t ai_bIsNull)
{
    // Si me quieren poner a null y yo no soy null, antes dejo de tener interes por el fichero
    if (ai_bIsNull == M4_TRUE && GetNull() == M4_FALSE)
    {
        // Dejo de mostrar interes por el fichero que tengo ahora
        if (UnSetInterestInFile(GetValueAddress()) != M4_SUCCESS)
            return M4_ERROR;
    }

    return ClDsItemBlob::SetNull(ai_bIsNull);
}

ClItemType ClDsItemBlobFile::GetType(void) const
{
    return ITEM_TYPE_BLOB;
}

m4return_t ClDsItemBlobFile::Serialize(ClGenericIODriver &IOD)
{
    m4char_t* pValue = (m4char_t*)GetValueAddress();
    if (ClFileState((const m4char_t*)pValue).IsFile() == M4_FALSE)
    {
		/* Bug 0103787
		Mensaje de error específico
		*/
        DUMP_CHLOG_ERRORF(M4_CH_DS_NO_SUCH_BLOB_FILE, pValue);
        return M4_ERROR;
    }
    
    m4bool_t bExistsPreviously;
    if (IOD.Write(m_pBlobData, &bExistsPreviously) != M4_SUCCESS)
    {
        DUMP_CHLOG_ERROR(M4_CS_ERROR_SERIALIZE_ITEMBLOBFILE);
        return M4_ERROR;
    }
    
    if (bExistsPreviously == M4_FALSE)
    {
        // Me serializo yo
        if (IOD.WriteFromFile(pValue) != M4_SUCCESS)
        {
            DUMP_CHLOG_ERROR(M4_CS_ERROR_SERIALIZE_ITEMBLOBFILE);
            return M4_ERROR;
        }
    }
    
    // Serializo mi descripcion
    if (m_Description.Serialize(IOD) != M4_SUCCESS)
    {
        DUMP_CHLOG_ERROR(M4_CS_ERROR_SERIALIZE_ITEMBLOBFILE);
        return M4_ERROR;
    }

    return M4_SUCCESS;
}

m4return_t ClDsItemBlobFile::DeSerialize(ClGenericIODriver &IOD)
{
    ClBlobData* pBlobData;
    m4bool_t bExistsPreviously;
    
    if (IOD.Read((void**)&pBlobData, &bExistsPreviously) != M4_SUCCESS)
    {
        DUMP_CHLOG_ERROR(M4_CS_ERROR_DESERIALIZE_ITEMBLOBFILE);
        return M4_ERROR;
    }
    
    if (pBlobData == NULL && bExistsPreviously == M4_FALSE)
    {
        // Es la primera vez
        m4uint32_t AuxOffset = IOD.GetActualOffset();
        
        // Me deserializo yo
        m4char_t sTempName[M4_MAX_PATH];
        if (IOD.ReadToFile(sTempName) != M4_SUCCESS)
        {
            DUMP_CHLOG_ERROR(M4_CS_ERROR_DESERIALIZE_ITEMBLOBFILE);
            return M4_ERROR;
        }
        
        if (sTempName[0] == 0)
        {
            // No hay nombre the fichero
            DUMP_CHLOG_ERROR(M4_CS_ERROR_DESERIALIZE_ITEMBLOBFILE);
            return M4_ERROR;
        }
        
        m4int32_t iNameSize = strlen(sTempName) + 1;

		m4int32_t iSize = SetValue(sTempName, iNameSize) ;
        
		/*
		Hay que liberar el interés sobre el fichero, ya que ReadToFile
		nos lo devuelve con un interés
		Sólo debe quedar el interés que hace el setvalue
		*/
		ClFileManager::Instance()->UnSetInterestInFile( sTempName ) ;
        
        if (iSize != iNameSize)
        {
            DUMP_CHLOG_ERROR(M4_CS_ERROR_DESERIALIZE_ITEMBLOBFILE);
            return M4_ERROR;
        }
        
        IOD.StoreAddress(m_pBlobData, AuxOffset);
    }
    else
    {
        SetpBlobData(pBlobData);    // Es nulo o ya existe
        if (SetInterestInFile(pBlobData->m_pData) != M4_SUCCESS)
        {
            DUMP_CHLOG_ERROR(M4_CS_ERROR_DESERIALIZE_ITEMBLOBFILE);
            return M4_ERROR;
        }
    }
    
    // Derializo mi descripcion
    if (m_Description.DeSerialize(IOD) != M4_SUCCESS)
    {
        DUMP_CHLOG_ERROR(M4_CS_ERROR_DESERIALIZE_ITEMBLOBFILE);
        return M4_ERROR;
    }

    return M4_SUCCESS;
}

m4return_t ClDsItemBlobFile::EventHandler(ClEventDescription& ai_oEventDescription)
{
	/* Bug 0202912
	Sólo se tratan eventos en parte cliente
	*/
	if( m_pVRB->m_pSMData->CsType() != M4CL_CSTYPE_FRONT )
	{
		return M4_SUCCESS;
	}

	if( ai_oEventDescription.GetType() == EVENT_TYPE_RM )
	{
		m4bool_t bIAmOldValue = *(m4bool_t*)ai_oEventDescription.GetDetails();

		if (bIAmOldValue)
		{
			// Ahora soy un oldValue,
			// tengo que dejar de mostrar interes en los eventos sobre el fichero
			ClFileEventBroker::Instance()->UnSetInterestInFile((m4char_t*)GetValueAddress(), this);
		}
		else
		{
			// Ahora soy un value normal,
			// tengo que volver a mostrar interes en los eventos sobre el fichero
			ClFileEventBroker::Instance()->SetInterestInFile((m4char_t*)GetValueAddress(), this);
		}
    }

	return M4_SUCCESS;
}

void ClDsItemBlobFile::FileEventHandler(const ClFileEventDescription& ai_oEventDescription)
{
	/* Bug 0202912
	Sólo se tratan eventos en parte cliente
	*/
	if( m_pVRB->m_pSMData->CsType() != M4CL_CSTYPE_FRONT )
	{
		return;
	}

    // Los eventos siempre me van a venir serializados, es decir nunca vendra un envento indicando
    // modificacion del fichero y cambio de nombre, primero vendra uno y luego el otro
    // Algunas flags son prioritarias, estas las debo tratar primero

    // Para actualizar el item siempre accedo desde el VRB. De esta forma se me
    // ya se marca el regsitro como modificado, se crea el oldValue, se activan los deltas, ...

    m4return_t iRet;

    // Para indicarle al registro que he cambiando tengo que buscar mi indice en el VRB
    m4bool_t bIAmOldValue;
    regindex_t iMyIndexInVRB = m_pVRB->GetIndexByRecordAddress(m_pRecordBuffer, bIAmOldValue);
    if (iMyIndexInVRB == -1)
    {
        M4_ASSERT("Record not found in VRB" == NULL);
        return;
    }

    // Solo vamos a tratar los eventos de modificacion
    // Compruebo si el fichero ha cambiado
    if (ai_oEventDescription.m_oDiffMask.Data    == 1 &&
        ai_oEventDescription.m_oDiffMask.Erased  == 0 /*&&
        ai_oEventDescription.m_oDiffMask.Created == 0*/)
    {
        iRet = m_pVRB->SetItemValueByPosAndOffset(m_ItemPos, GetValueAddress(), iMyIndexInVRB, 0, GetSize(), NULL, M4_TRUE, M4_FALSE);

		if (iRet != M4_SUCCESS)
		{
			DUMP_CHLOG_ERRORF(M4_CS_ERROR_DSITEMBLOBFILE_FILE_MODIFIED,  (m4char_t*)GetValueAddress());
		}
    }
}

m4return_t ClDsItemBlobFile::SetInterestInFile(const void* const ai_pFileName)
{
    // Me intereso por un fichero
    if (ClFileManager::Instance()->SetInterestInFile((m4char_t*)ai_pFileName) != M4_SUCCESS)
	{
        return M4_ERROR;
	}

	/* Bug 0202912
	Sólo se tratan eventos en parte cliente
	*/
	if( m_pVRB->m_pSMData->CsType() == M4CL_CSTYPE_FRONT )
	{
	    // Me intereso por sus eventos
		if (ClFileEventBroker::Instance()->SetInterestInFile((m4char_t*)ai_pFileName, this) != M4_SUCCESS)
		{
			return M4_ERROR;
		}
	}

    return M4_SUCCESS;
}

m4return_t ClDsItemBlobFile::UnSetInterestInFile(const void* const ai_pFileName)
{
	/* Bug 0202912
	Sólo se tratan eventos en parte cliente
	*/
	if( m_pVRB->m_pSMData->CsType() == M4CL_CSTYPE_FRONT )
	{
		// Dejo de mostrar interes por sus eventos
		ClFileEventBroker::Instance()->UnSetInterestInFile((m4char_t*)ai_pFileName, this);
	}

    // Dejo de mostrar interes por un fichero
    ClFileManager::Instance()->UnSetInterestInFile((m4char_t*)ai_pFileName);

    return M4_SUCCESS;
}

m4return_t ClDsItemBlobFile::GetUsedMemory(ClSize& ao_size) const
{
    ao_size += sizeof(ClDsItemBlobFile);

    ao_size -= sizeof(ClDsItemBlob);
    if (ClDsItemBlob::GetUsedMemory(ao_size) != M4_SUCCESS)
        return M4_ERROR;

    // ClEventControllableObject;   Pertenece a otra dll
    // ClFileControllableObject;    Pertenece a otra dll

    ao_size -= sizeof(m_Description);
    if (m_Description.GetUsedMemory(ao_size) != M4_SUCCESS)
        return M4_ERROR;

    // m_oFileState;    Pertenece a otra dll

    return M4_SUCCESS;
}
