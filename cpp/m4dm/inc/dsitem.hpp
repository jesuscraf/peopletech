
#ifndef __DSITEM__HPP__
#define __DSITEM__HPP__

//==============================================================================
//
// (c) Copyright 1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:
// File:                dsitem.hpp
// Project:             Canales
// Author:              Meta Software M.S. , S.A
// Date:                24/2/99
// Language:            C++
// Operating System:    WINDOWS, ??
// Design Document:     XXXX.DOC
//
//
// Definition:
//
//
//
//==============================================================================

#include "dstypes.hpp"
#include "clsrlzbl.hpp"
#include "generic_event.hpp"
#include "file_event.hpp"


class ClVectorRecordBase;
class ClItemRecordManager;
class ClHandle;
class ClCompiledMCR;
class ClFileState;
class ClSize;

typedef m4uint32_t ClItemType; // WARNING: definitions already in mch

const ClItemType ITEM_TYPE_BLOB           = 0;
const ClItemType ITEM_TYPE_DOUBLE         = 1;
const ClItemType ITEM_TYPE_FIXED_STRING   = 2;
const ClItemType ITEM_TYPE_OTHER          = 3;


////////////////////////////////////////////////////////////////////////////////
// class ClBlobData
////////////////////////////////////////////////////////////////////////////////


class ClBlobData
{
public:
    ClBlobData(m4uint32_t ai_Size = 0);
    void Destroy(void);

    m4return_t GetUsedMemory(ClSize& ao_size) const;
    
    m4uint32_t m_Size;
    m4uint8_t  m_RefCount;
    m4uint8_t* m_pData;
#ifndef _AIX
// Para que si estamos en AIX sea público y no de warnings
private:
#endif
    ~ClBlobData(void);
};


////////////////////////////////////////////////////////////////////////////////
// class ClBlobWithReferences
////////////////////////////////////////////////////////////////////////////////


// Esta clase tiene todos sus miembros protegidos, de forma que las
// clases derivadas publiquen la parte que les interesa.
// No debería haber instancias de esta clase.
class ClBlobWithReferences : public ClSerializableObject
{
protected:
    // Constructores y destructor
    ClBlobWithReferences();

    ClBlobWithReferences(const ClBlobWithReferences& ai_oSource);

    virtual ~ClBlobWithReferences();

    // Operadores
    ClBlobWithReferences& operator= (const ClBlobWithReferences& ai_oSource);

    // Miembros
    // Asigna los datos, a partir de un offser indicado
    // Devuleve el tamaño de bytes copiados, -1 == error
    m4int32_t SetData(const void* const ai_pData,
                      const m4uint32_t  ai_Size,
                      const m4uint32_t  ai_Offset = 0);

    // Devuelve los datos, a partir de un offset indicado
    // Devuleve el tamaño de bytes copiados, -1 == error
    m4int32_t GetData(void*      const ao_pData,
                      const m4uint32_t ai_MaxSize,
                      const m4uint32_t ai_Offset = 0) const;

    const void* const GetDataAddress(const m4uint32_t ai_Offset = 0) const;

    m4return_t SetSize(const m4uint32_t ai_Size);

    m4uint32_t GetSize(void) const;

    virtual m4return_t GetUsedMemory(ClSize& ao_size) const;

    void SetpBlobData(ClBlobData* const ai_pBlobData);

    // Miembros heredados de ClSerializableObject
    virtual m4return_t Serialize  (ClGenericIODriver& IOD);
    virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

    ClBlobData* m_pBlobData;
};


////////////////////////////////////////////////////////////////////////////////
// class ClDsItemBlob
////////////////////////////////////////////////////////////////////////////////


// This is an item in a channel (see GetStaticObjectSize)
// We can change it to a generic and a specific in a channel CRG
class ClDsItemBlob : public ClBlobWithReferences
{
public:
    // Constructores y destructores
    ClDsItemBlob(ClItemRecordManager* const ai_pRM,
             m4uint8_t*           const ai_pRecordBuffer,
             m4uint16_t           const ai_ItemPos);

    ClDsItemBlob(const ClDsItemBlob& ai_oSource,
                 m4uint8_t*    const ai_pRecordBuffer);

    virtual ~ClDsItemBlob(void);

    // Operadores
    void* operator new  (size_t, void* p) { return p; } // Calls constructor
    // void* operator new[](size_t, void* p) { return p; } // Calls constructor

    void operator delete  (void*, size_t) {} // Calls destructor
    // void operator delete[](void*, unsigned int) {} // Calls destructor

    ClDsItemBlob& operator= (const ClDsItemBlob& ai_oSource);

    // Miembros
    // Asigna un valor del item, a partir del offset indicado
    // Devuleve el tamaño de bytes asignados, -1 == error
    virtual m4int32_t SetValue(const void* const ai_pValue,
                               const m4uint32_t  ai_Size,
                               const m4uint32_t  ai_Offset = 0);

    // Devuelve el valor del item, a partir de un offset indicado
    // Devuleve el tamaño de bytes copiados, -1 == error
    virtual m4int32_t GetValue(void*      const ao_pValue,
                               m4bool_t&        ao_IsNull,
                               const m4uint32_t ai_MaxSize,
                               const m4uint32_t ai_Offset = 0) const;

    // CRG AL THE ITEMS ????
    virtual void const * const GetValueAddress(const m4uint32_t ai_Offset = 0) const;
    
    // CRG AL THE ITEMS ????
    virtual m4return_t SetSize(const m4uint32_t ai_iNewSize);
    virtual m4uint32_t GetSize(void) const;

    // We are assuming that the static size is only dependent of the attributes 
    // of the item in the metachannel. That's why the method is static, it 
    // doesn't depend on the state of the item, only on the meta-item
    static size_t GetStaticSize(const ClHandle&      ai_oItemHandle, 
                                const ClCompiledMCR& ai_oMetaChannel);

    virtual m4return_t GetUsedMemory(ClSize& ao_size) const;

    // Pone el item a null
    virtual m4return_t SetNull(const m4bool_t ai_bIsNull);
    virtual m4bool_t GetNull(void) const;
    
    // CRG AL THE ITEMS ????
    virtual void SetHasSlices(const m4bool_t ai_bHasSlices);
    virtual m4bool_t GetHasSlices(void) const;
    
    // CRG AL THE ITEMS ????
    virtual void SetPriority(const m4int8_t ai_iPriority);
    virtual m4int8_t GetPriority(void) const;
    
    virtual ClItemType GetType(void) const;


protected:
    // Miembros para poder acceder a mi padre
    ClItemRecordManager* m_pRM;             // Para acceder al EMD;
    m4uint8_t*           m_pRecordBuffer;   // Para poder pasarle a las funciones del RM el buffer
    m4uint16_t           m_ItemPos;         // Posicion que ocupa este item
};


////////////////////////////////////////////////////////////////////////////////
// class ClBlobFileDescription
////////////////////////////////////////////////////////////////////////////////


class ClBlobFileDescription : public ClBlobWithReferences
{
public:
    // Constructores y destructor
    ClBlobFileDescription();

    ClBlobFileDescription(m4pcchar_t const ai_pDescription);

    virtual ~ClBlobFileDescription();

    // Operadores
    ClBlobFileDescription& operator= (const ClBlobFileDescription& ai_oSource);

    // Miembros
    virtual m4return_t SetDescription(m4pcchar_t const ai_pDescription);
    virtual m4pcchar_t const GetDescription(void);

    // Miembros heredados de ClBlobWithReferences
    virtual m4return_t GetUsedMemory(ClSize& ao_size) const;

    // Miembros heredados de ClSerializableObject
    virtual m4return_t Serialize  (ClGenericIODriver& IOD);
    virtual m4return_t DeSerialize(ClGenericIODriver& IOD);
};


////////////////////////////////////////////////////////////////////////////////
// class ClDsItemBlobFile
////////////////////////////////////////////////////////////////////////////////


class ClDsItemBlobFile : public ClDsItemBlob,
                         public ClEventControllableObject,
                         public ClFileControllableObject
{
public:
    // Constructores y destructor
    ClDsItemBlobFile(ClVectorRecordBase*  const ai_pVRB,
                     ClItemRecordManager* const ai_pRM,
                     m4uint8_t*           const ai_pRecordBuffer,
                     m4uint16_t           const ai_ItemPos,
                     m4pcchar_t           const ai_pDescription = NULL);

	ClDsItemBlobFile(const ClDsItemBlobFile& ai_oSource,
                     m4uint8_t*        const ai_pRecordBuffer);

    virtual ~ClDsItemBlobFile();
    
    // Operadores
    void* operator new  (size_t, void* p) { return p; } // Calls constructor
    // void* operator new[](size_t, void* p) { return p; } // Calls constructor

    void operator delete  (void*, size_t) {} // Calls destructor
    // void operator delete[](void*, unsigned int) {} // Calls destructor

    ClDsItemBlobFile& operator= (const ClDsItemBlobFile& ai_oSource);

    // Miembros
    // Este metodo crea un fichero temporal y lo asigna al item
    // Es lo mismo que crear el fichero y hacer un SetValue
    virtual m4return_t CreateTempFile(m4pchar_t const ai_pDirectory, 
                                      m4pchar_t const ai_pExtension);
    
    // Devuelve el estado actual del fichero
    virtual m4return_t GetState(ClFileState& ao_oFState) const;

    // Nos dice si ha habido cambios en el estado del fichero desde la ultima asignacion
    virtual m4bool_t StateHasChanged(void) const;

    // Devuelve los cambios que ha habido en el estado del fichero desde la ultima asignacion
    virtual void ChangedMask(ClComparisonResultMask& ao_oChangedMask);

    // Miembros heredados de ClDsItemBlob
    virtual m4int32_t SetValue(const void* const ai_pValue,
                               const m4uint32_t  ai_Size,
                               const m4uint32_t  ai_Offset = 0);

    virtual m4return_t SetNull(const m4bool_t ai_bIsNull);

    virtual ClItemType GetType(void) const;

    virtual size_t GetRealSize(void);

    static size_t GetStaticSize(const ClHandle&      ai_iItemHandle, 
                                const ClCompiledMCR& ai_oMetaChannel);

    virtual m4return_t GetUsedMemory(ClSize& ao_size) const;

    ClBlobFileDescription m_Description;

    // Miembros heredados de ClSerializableObject
    virtual m4return_t Serialize  (ClGenericIODriver& IOD);
    virtual m4return_t DeSerialize(ClGenericIODriver& IOD);

     // Miembros heredados de ClEventControllableObject
    virtual m4return_t EventHandler(ClEventDescription& ai_oEventDescription);

    // Miembros heredados de ClFileControllableObject
    virtual void FileEventHandler(const ClFileEventDescription& ai_oEventDescription);


private:
    // Para mostrar interes en un fichero y los eventos sobre este fichero
    m4return_t SetInterestInFile(const void* const ai_pFileName);
    m4return_t UnSetInterestInFile(const void* const ai_pFileName);

    ClFileState m_oFileState;
    
    ClVectorRecordBase* m_pVRB; // Lo necesito para poder activar los deltas de registro 
                                // cuando se produce un evento de modificacion de fichero
};


#endif
