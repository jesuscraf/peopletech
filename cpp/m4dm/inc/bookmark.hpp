#ifndef __BOOKMARK_HPP__
#define __BOOKMARK_HPP__

//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           bookmark.hpp
// File:             bookmark.hpp
// Project:          CVM-DM
// Author:           Meta Software M.S. , S.A
// Date:             27/04/98
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    This module defines...
//
//
//==============================================================================
#include "m4dm_dll.hpp"
#include "index.hpp"
#include "m4types.hpp"
#include "mmindex.hpp"
#include "handles.hpp"
#include "m4var.hpp"


class _ClRegister_Base;
class ClAccess;
class ClChannel;
class ClNode;
class ClNoCallableItem;
class ClNode_Data;


/**
 * Guarda los datos necesarios para determinar donde estamos posicionados en un nodo.
 */
class ClNodeBookMark
{
public:
	/** Registro lógico donde está posicionado el current. */
    ClRegisterIndex currentIndex;

	/** Registro físico donde está posicionado el current. Va a deteminar el bloque del hijo. */
    ClMMIndex currentMMIndex;

	/** Item donde está posicionado el curretn. */
	ClItemIndex currentItemIndex;

	/** Registro lógico donde está posicionado el register. */
    ClRegisterIndex registerIndex;

	/** Registro físico donde está posicionado el register. */
    ClMMIndex registerMMIndex;

	/** Item donde está posicionado el register. */
	ClItemIndex registerItemIndex;

	/** Puntero al Node del access. Sirve para ordenar la lista mientras se construye. */
	ClNode* pNode;

	/** Indice del nodo. Sirve para localizar el nodo, rápidamente, cuando estamos aplicando el bookmark. */
	m4uint32_t nodeIndex;

	/** Construye un nuevo ClNodeBookMark */
	ClNodeBookMark(void)
		: currentIndex ((ClRegisterIndex)-1)
		, currentMMIndex ((ClMMIndex)-1)
		, currentItemIndex((ClItemIndex)-1)
		, registerIndex ((ClRegisterIndex)-1)
		, registerMMIndex ((ClMMIndex)-1)
		, registerItemIndex ((ClItemIndex)-1)
		, pNode(0)
		, nodeIndex((m4uint32_t)-1)
	{};

	/** En AIX, ObjectSpace necesita de estos operadores, para instanciar una pila. ¿Por qué?. Ni idea. */
	m4bool_t operator == (const ClNodeBookMark& ai_left)const;
	m4bool_t operator < (const ClNodeBookMark& ai_left)const;
};

M4_DECL_M4DM ostream& operator<< (ostream& aio_os, const ClNodeBookMark& ai_nodeBookMark);


class ClNode_PK_Info
{
public:

    ClNode_PK_Info() : m_pArguments(0), m_pNodeName (0), m_iItemCount (0)
    {
    }

   ~ClNode_PK_Info();

    m4pchar_t m_pNodeName;
    m4VariantType * m_pArguments;

    m4uint32_t m_iItemCount;

    m4uint32_t Count(void)
    {
        return m_iItemCount;
    }

    m4pchar_t GetNodeName(void)
    {
        return m_pNodeName;
    }

    m4VariantType ItemValue(m4uint32_t ai_iItem)
    {
        if (ai_iItem < m_iItemCount)
            return m_pArguments[(3 * ai_iItem) + 1];
        else
        {
            m4VariantType vNull;
            return vNull;
        }
    }

    m4pchar_t ItemName(m4uint32_t ai_iItem)
    {
        if (ai_iItem < m_iItemCount)
            return m_pArguments[3 * ai_iItem].Data.PointerChar;
        else
            return 0;
    }
};


class M4_DECL_M4DM ClAccessBookMark
{
public:
	ClAccessBookMark(void);
    virtual ~ClAccessBookMark(void);

	/**
	 * Prepara el AccessBookMark para que recuerde la posición actual del acceso.
	 * Lo hace de forma jerárquica. Es decir, por cada nodo se guarda (entre otras cosas)
	 * donde esta el current. Al hacer el ApplyTo coloco el current de forma que se selecciona
	 * automáticamente el bloque hijo.
	 *
	 * @param ai_access    Acceso que queremos guardar donde está posicionado.
	 * @return m4return_t
	 */
    m4return_t GetFrom   (ClAccess& ai_access);
    m4return_t GetFrom_PK(ClAccess& ai_access);

	/**
	 * Recoloca un acceso para que quede posicionado según lo que está guardado en el bookmark.
	 *
	 * @param ai_access    Acceso sobre el que queremos aplicar el bookmark.
	 * @return m4return_t
	 */
    m4return_t ApplyTo   (ClAccess& ai_access);
    m4return_t ApplyToBranch(ClAccess& ai_access, m4pchar_t ai_pCutNode, m4pcchar_t& ao_pLastNode);

	m4return_t DeserializeBranch(ClChannel* ai_poChannel, const m4char_t* ai_pcString);
	m4return_t SerializeBranch	(ClChannel* ai_poChannel, ClAccess* ai_poAccess, m4uint32_t ai_hNode, m4char_t *ao_pcBuffer, m4uint32_t* aio_uiLongBuffer);

    static m4return_t Follow(ClAccess& ai_accessfrom, ClAccess &aio_accessto);
    static void Verify(ClAccess& ai_accessfrom, ClAccess& ai_accessto);

protected:
	/** El handle del nodo current donde estba posicionado el acceso al hacer el GetFrom. */
	ClHandle m_nodeHandle;

	/** Vector, a pelo, con la información de los ítems PK para los register. */
    ClNode_PK_Info* m_pRegisters_Info;

	/** Vector, a pelo, con la información de los ítems PK para los current. */
	ClNode_PK_Info* m_pCurrents_Info;

	m4char_t * _GetToken(const m4char_t* ai_pcString, const m4char_t* ai_pcDelim, m4char_t **ai_pc);
    void Get_PK(_ClRegister_Base * ai_pReg, ClNode_PK_Info &ao_PK_Info);
    m4return_t _ApplyTo_PK (ClAccess &ai_access);

protected:
	/** Vector con la informacion de posicionamiento de los nodos del access. */
	list<ClNodeBookMark>* m_plNodeBookMark;

	/** Borra toda la informacion asociada al bookmark. */
	void Reset (void);

	/**
	 * Busca un nodo de un acceso en la lista de NodeBookMarks.
	 *
	 * @param ai_pNode    Nodo del acceso que se quiere buscar en la lista.
	 * @return Un iterador apuntantdo al NodeBookMark. Si no se encuentra se devuelve el iterador end().
	 */
	list<ClNodeBookMark>::iterator FindNodeInNodeBookMarkList(const ClNode* const ai_pNode);

	/**
	 * Inserta un nuevo NodeBookMark. No se permiten repeticiones.
	 * Un NodeBookMark siempre se inserta delanta de su nodo padre. Si este todavía no está en la lista
	 * se llamara recursivamente para insertar primero al padre y luego al hijo.
	 *
	 * @param ai_pNode    Nodo del acceso que se quiere insertar en la lista de NodeBookMarks.
	 * @param ai_access    Acceso donode se busca el padre del nodo que queremos insertar.
	 * @return un iterador apuntando al NodeBookMark insertado. Si este ya existía se devuelve el iterador end().
	 */
	list<ClNodeBookMark>::iterator InsertInNodeBookMarkList(ClNode* const ai_pNode, ClAccess& ai_access);

friend M4_DECL_M4DM ostream& operator<< (ostream& aio_os, const ClAccessBookMark& ai_accessBookMark);
};

M4_DECL_M4DM ostream& operator<< (ostream& aio_os, const ClAccessBookMark& ai_accessBookMark);



/*****************************************************************************************
 *
 * SOPORTE A LOS ANTIGUOS BOOKMARKS.
 * ESTOS SÓLO DEBERIAN UTILIZARSE PARA LA COMPILACIÓN DEL CÓDIGO DEL LN4.
 *
 * Estos bookmark tiene el inconveniente que cuando se aplican sobre un acceso este puede
 * quedar inconsistente, ya que lo que se guarda es el id del bloque. Ej. si borro un
 * bloque y lo vuelvo a crear, no tiene por que tener el mismo ID.
 *
 *****************************************************************************************/

#include "px_block.hpp"

class ClAccessRecordSet;


class ClOldItemBookMark
{
public:
    ClItemIndex Index;

    m4return_t GetFrom (ClNoCallableItem &ai_item);
    m4return_t ApplyTo (ClNoCallableItem &ai_item);

    static m4return_t Follow (ClNoCallableItem &ai_itemfrom, ClNoCallableItem &ai_itemto);
    static void Verify (ClNoCallableItem &ai_itemfrom, ClNoCallableItem &ai_itemto);
};

// ClOldRegisterBookMark es friend de _ClRegister_Base.
class ClOldRegisterBookMark
{
public:
    ClRegisterIndex Index;
    ClMMIndex MMIndex;
    ClOldItemBookMark Item;

    m4return_t GetFrom (_ClRegister_Base &ai_rs);
    m4return_t ApplyTo (_ClRegister_Base &ai_rs);

    static m4return_t Follow (_ClRegister_Base &ai_regfrom, _ClRegister_Base &ai_regto);
    static void Verify (_ClRegister_Base &ai_regfrom, _ClRegister_Base &ai_regto);
};

// ClOldAccessRecordSetBookMark es friend de ClAccessRecordSet.
class ClOldAccessRecordSetBookMark
{
public:
	StBlockId  m_oID;
    m4bool_t m_bIsNihil;

    ClOldAccessRecordSetBookMark();
   ~ClOldAccessRecordSetBookMark();

    ClOldRegisterBookMark Current;
    ClOldRegisterBookMark Register;

    m4return_t GetFrom (ClAccessRecordSet& ai_rs);
    m4return_t ApplyTo (ClAccessRecordSet& ai_rs);

    static m4return_t Follow (ClAccessRecordSet &ai_rsfrom, ClAccessRecordSet &ai_rsto);
    static void Verify (ClAccessRecordSet &ai_rsfrom, ClAccessRecordSet &ai_rsto);
};

class ClOldNodeBookMark
{
public:
    ClOldAccessRecordSetBookMark RecordSet;

    m4return_t GetFrom (ClNode &ai_node);
    m4return_t ApplyTo (ClNode &ai_node);

    static m4return_t Follow (ClNode &ai_nodefrom, ClNode &ai_nodeto);
    static void Verify (ClNode &ai_nodefrom, ClNode &ai_nodeto);
};

class M4_DECL_M4DM ClOldAccessBookMark : public ClAccessBookMark
{
public:
	ClOldAccessBookMark();
    ~ClOldAccessBookMark();

    m4return_t GetFrom    (ClAccess &ai_access);
    m4return_t ApplyTo    (ClAccess &ai_access);

private:
    ClOldNodeBookMark* m_pnbm;
    m4uint16_t m_nnodes;    //auxiliar, se usa para saber si hay que realocar cuando hacen un get
};


#endif //__BOOKMARK_HPP__
