//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           bookmark.cpp
// File:             bookmark.cpp
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

#include "bookmark.hpp"

#include "access.hpp"
#include "node.hpp"
#include "arecset.hpp"
#include "aregister.hpp"
#include "node_knl.hpp"
#include "compst_common.hpp"
#include "bnihil.hpp"

#include "blockref.hpp"

#include "cldbgutil.hpp"
#include "cp_find.hpp"
#include "rthierdes.hpp"
#include "m4mdrt.hpp"

////////////////////////////////////////////////////////////////////////////////
//
// ClNode_PK_Info
//

ClNode_PK_Info::~ClNode_PK_Info()
{
    if (m_iItemCount)
    {
        delete [] m_pArguments;
        delete [] m_pNodeName;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// ClAccessBookMark
//

ClAccessBookMark::ClAccessBookMark ()
	: m_nodeHandle(0)
	, m_pRegisters_Info(0)
	, m_pCurrents_Info(0)
	, m_plNodeBookMark(0)
{}

m4return_t ClAccessBookMark::GetFrom(ClAccess& ai_access)
{
	/*
	 * Creo una lista donde los nodos hoja están al principio, y los nodos padre están al final.
	 * De esta forma si luego me recorro la lista del final al principio, puedo ir colocando el
	 * acceso. Esta forma de recorrerme la lista me aseguar que los nodos padres se van a colocar
	 * primero, con loque el bloque del hijo estará bien seleccionado por el current.
	 */
	Reset();
	m_plNodeBookMark = new list<ClNodeBookMark>;

    m4uint16_t n = ai_access.Node.Count ();
    for (m4uint16_t i = 0; i < n; i++)
		InsertInNodeBookMarkList(ai_access.Node.Get((ClNodeIndex)i), ai_access);

	m_nodeHandle = ai_access.Node->GetpNodeDef()->GetHandle();

    return M4_SUCCESS;
}

/**
 * Busca un nodo de un acceso en la lista de NodeBookMarks.
 *
 * @param ai_pNode    Nodo del acceso que se quiere buscar en la lista.
 * @return Un iterador apuntantdo al NodeBookMark. Si no se encuentra se devuelve el iterador end().
 */
list<ClNodeBookMark>::iterator ClAccessBookMark::FindNodeInNodeBookMarkList(const ClNode* const ai_pNode)
{
	list<ClNodeBookMark>::iterator it;
	for (it = m_plNodeBookMark->begin(); it != m_plNodeBookMark->end(); it++) {
		if (ai_pNode == (*it).pNode)
			break;	// Lo he encontrado
	}
	return it;
}

/**
 * Inserta un nuevo NodeBookMark. No se permiten repeticiones.
 * Un NodeBookMark siempre se inserta delanta de su nodo padre. Si este todavía no está en la lista
 * se llamara recursivamente para insertar primero al padre y luego al hijo.
 *
 * @param ai_pNode    Nodo del acceso que se quiere insertar en la lista de NodeBookMarks.
 * @param ai_access    Acceso donode se busca el padre del nodo que queremos insertar.
 * @return un iterador apuntando al NodeBookMark insertado. Si este ya existía se devuelve el iterador end().
 */
list<ClNodeBookMark>::iterator ClAccessBookMark::InsertInNodeBookMarkList(ClNode* const ai_pNode, ClAccess& ai_access)
{
	// Miro que todavía no estoy en la lista.
	list<ClNodeBookMark>::iterator it = FindNodeInNodeBookMarkList(ai_pNode);
	if (it != m_plNodeBookMark->end())
		return it;		// Ya estoy en la lista.

	ClNodeBookMark nodeBookMark;
	
	nodeBookMark.pNode     = ai_pNode;
	nodeBookMark.nodeIndex = nodeBookMark.pNode->GetpExtInfo()->Index;
	
	nodeBookMark.currentIndex     = nodeBookMark.pNode->RecordSet.Current.Index;
	nodeBookMark.currentMMIndex   = nodeBookMark.pNode->RecordSet.Current.MMIndex;
	nodeBookMark.currentItemIndex = nodeBookMark.pNode->RecordSet.Current.Item.GetIndex();
	
	nodeBookMark.registerIndex     = nodeBookMark.pNode->RecordSet.Register.Index;
	nodeBookMark.registerMMIndex   = nodeBookMark.pNode->RecordSet.Register.MMIndex;
	nodeBookMark.registerItemIndex = nodeBookMark.pNode->RecordSet.Register.Item.GetIndex();

	// Obtengo el padre del nodo, para insertarme delante de él.
	// Compruebo que mi padre no es de otro canal (caso de estar encarrilado).
	ClNode_Connector* pNodeConector = ai_pNode->GetpExtInfo()->Parent.GetpNotFree();
	if (pNodeConector && ai_pNode->GetpExtInfo()->IsFree_ForgetingLanes() == M4_FALSE) {
		// Compruebo si el padre ya está en la lista de NodeBookMarks.
		ClNode_Data* pParentNodeData = pNodeConector->GetAddress();
		ClNodeIndex  nodeIndex       = pParentNodeData->GetpExtInfo()->Index;
		ClNode*      pParentNode     = ai_access.Node.Get(nodeIndex);

		/*
		// Compruebo que no se trate de una conexión auto. En este caso soy mi propio padre.
		if (ai_pNode->GetpNode_Data() == pParentNodeData) {
			// Como no tengo padre (soy yo mismo), me inserto al final de la lista.
			it = m_plNodeBookMark->end();
		}
		*/

		it = InsertInNodeBookMarkList(pParentNode, ai_access);	// Llamada recursiva. Si es necesario se inserta a mi padre.
		M4_ASSERT(it != m_plNodeBookMark->end());
	}
	else {
		// Si no tengo padre me inserto al final de la lista.
		it = m_plNodeBookMark->end();
	}

	return m_plNodeBookMark->insert(it, nodeBookMark);
}

m4return_t ClAccessBookMark::GetFrom_PK (ClAccess &ai_access)
{
	Reset ();

	m4uint32_t i;
	m4uint32_t n = ai_access.Node.Count();

    m_pRegisters_Info = new ClNode_PK_Info[n];
    m_pCurrents_Info = new ClNode_PK_Info[n];

    for (i = 0; i < n; i ++)
    {
        Get_PK(&ai_access.Node.Get((ClNodeIndex)i)->RecordSet.Current, m_pCurrents_Info[i]);
        Get_PK(&ai_access.Node.Get((ClNodeIndex)i)->RecordSet.Register, m_pRegisters_Info[i]);
    }
    return M4_SUCCESS;
}

/**
 * Coloco el acceso según está guardado en el bookmark.
 *
 * @param ai_access    Acceso que se quiere recolocar.
 * @return
 */
m4return_t ClAccessBookMark::ApplyTo (ClAccess &ai_access)
{
	// Si hicimos un GetFrom().
    if (m_plNodeBookMark) {
		ClNode* pNode = 0;

		/*
		 * Nos recorremos la lista de atras a delante. De esta forma nos aseguramos que se van
		 * a colocar priemro los nodos padre. Así el current del padre determina el bloque del hijo.
		 */
		list<ClNodeBookMark>::reverse_iterator it;
		for (it = m_plNodeBookMark->rbegin(); it != m_plNodeBookMark->rend(); it++) {
			pNode = ai_access.Node.Get((ClNodeIndex)((*it).nodeIndex));

			/*
			 * Hay que hacer algunas comprobaciones para ver si realmente nos
			 * podemos mover donde indica el bookmark.
			 * Pero faltarían más comprobaciones. Ej. si tenemos filtros, podemos estar fuera de rango
			 * y no nos damos cuenta.
			 */

			// Cuidado pq si el currentblock es un nihil y pedimos el count, lanzamos un warning nosotros mismos:
			ClBlock_Channel_Proxy* pCurrentBlockProxy = pNode->RecordSet.Current.GetCurrentBlock();
			if (pCurrentBlockProxy->IsNihil()) {
				pNode->RecordSet.Current.MoveToEOF();
				pNode->RecordSet.Register.MoveToEOF();
			}
			else {
				// m4uint32_t n = ai_reg.Count();
				// ^----- no! : dispara autoload, o ejecuta filtros dinámicos, lo que nos puede meter en bucles infinitos.
				m4int32_t n = pCurrentBlockProxy->Count();

				// Comprobación rudimentadia para saber si nos vamos a ir fuera de rango.
				if ((m4int32_t)((*it).currentMMIndex) >= n || (m4int32_t)((*it).registerMMIndex) >= n) {
					pNode->RecordSet.Current.MoveToEOF();
					pNode->RecordSet.Register.MoveToEOF();

					// Tengo un bookmark que no puedo aplicar sobre el acceso.
					// Ej. is entre el GetFrom y el ApplyTo se ha hecho una reordenacion.
					return M4_ERROR;
				}
				else {
					// MoveTo del current para que se me colquen los hijos.
					pNode->RecordSet.Current.MoveToMM   ((*it).currentMMIndex);
					pNode->RecordSet.Current.Index     = (*it).currentIndex;
					pNode->RecordSet.Current.Item.MoveTo((*it).currentItemIndex);
					
					pNode->RecordSet.Register.Index     = (*it).registerIndex;
					pNode->RecordSet.Register.MMIndex   = (*it).registerMMIndex;
					pNode->RecordSet.Register.Item.MoveTo((*it).registerItemIndex);
				}
			}
		}

	    ai_access.Node[m_nodeHandle];

        return M4_SUCCESS;
    }

	// Si hicimos un GetFrom_PK().
    if (m_pRegisters_Info && m_pCurrents_Info) {
        _ApplyTo_PK (ai_access);
        return M4_SUCCESS;
    }

    return M4_ERROR;
}

m4return_t ClAccessBookMark::_ApplyTo_PK (ClAccess &ai_access)
{
    ClChanProc_FindRegister oClCPE (m_pCurrents_Info, m_pRegisters_Info, &ai_access);
    ClChanRoutHierarchicalDescendent oRoute;

    oRoute.AddProcess(&oClCPE);
    return oRoute.Begin(ai_access.GetpChannel(), 0);
/*
    m4uint32_t i, n = ai_access.Node.Count();
    ClAccessRecordSet * pRS;

    for (i = 0; i < n; i ++)
    {
        pRS = &ai_access.Node.Get((ClNodeIndex)i)->RecordSet;

        if (m_pCurrents_Info[i].m_iItemCount)
            pRS->Current.FindSec (m_pCurrents_Info[i].m_pArguments, (m4uint16_t) (3 * m_pCurrents_Info[i].m_iItemCount));
        else
            pRS->Current.MoveToEOF();

        if (m_pRegisters_Info[i].m_iItemCount)
            pRS->Register.FindSec (m_pRegisters_Info[i].m_pArguments, (m4uint16_t) (3 * m_pRegisters_Info[i].m_iItemCount));
        else
            pRS->Register.MoveToEOF();
    }
    return M4_SUCCESS;
*/
}

m4return_t ClAccessBookMark::ApplyToBranch (ClAccess &ai_access, m4pchar_t ai_pCutNode, m4pcchar_t & ao_pLastNode)
{
    m4int32_t i, n = ai_access.Node.Count(), count;
    ClNodeIndex * pNodeIndex = new ClNodeIndex[n];
    ClAccessRecordSet * pRS;

    if (ai_access.Node.Get(ai_pCutNode))
    {
        ClNode_Data * pNodeIter = ai_access.Node.Get(ai_pCutNode)->GetpNode_Data();
        i = 0;

        while (pNodeIter)
        {
            pNodeIndex[i] = pNodeIter->GetpExtInfo()->Index;
            i ++;

            if (pNodeIter->GetpExtInfo()->Parent.GetpNotFree())
                pNodeIter = pNodeIter->GetpExtInfo()->Parent.GetpNotFree()->GetAddress();
            else
                pNodeIter = 0;
        }

        count = i;

        ao_pLastNode = 0;

        for (i = count - 1; i >= 0 ; -- i)
        {
            pRS = &ai_access.Node.Get(pNodeIndex[i])->RecordSet;

            if (m_pCurrents_Info[pNodeIndex[i]].m_iItemCount)
                pRS->Current.FindSec (m_pCurrents_Info[pNodeIndex[i]].m_pArguments, (m4uint16_t) (3 * m_pCurrents_Info[pNodeIndex[i]].m_iItemCount));
            else
                pRS->Current.MoveToEOF();

            if (m_pRegisters_Info[pNodeIndex[i]].m_iItemCount)
                pRS->Register.FindSec (m_pRegisters_Info[pNodeIndex[i]].m_pArguments, (m4uint16_t) (3 * m_pRegisters_Info[pNodeIndex[i]].m_iItemCount));
            else
                pRS->Register.MoveToEOF();

            if (pRS->Current.Index == M4DM_EOF_INDEX)
            {
                delete [] pNodeIndex;
                return M4_ERROR;
            }
            ao_pLastNode = pRS->GetpNode_Data()->GetpNodeDef()->Id();
        }

        ao_pLastNode = ai_pCutNode;
        delete [] pNodeIndex;
        return M4_SUCCESS;
    }

    else
    {
        ao_pLastNode = 0;
        delete [] pNodeIndex;
        return M4_ERROR;
    }
}

void ClAccessBookMark::Get_PK(_ClRegister_Base * ai_pReg, ClNode_PK_Info &ao_PK_Info)
{
    if (ai_pReg->Index != M4DM_EOF_INDEX)
    {
        ClItem auxItem = ai_pReg->Item;
        auxItem.SetMode(M4DM_SCOPE_ALL, M4DM_TYPE_PROPERTY | M4DM_TYPE_FIELD | M4DM_TYPE_CONCEPT);

        m4uint32_t n = 0, l, i;

        auxItem.Begin();
        while (auxItem.GetIndex() != M4DM_EOF_INDEX)
        {
            if (auxItem.ItemDef.IsPK())
                n ++;
            auxItem.Next();
        }

        if (n)
        {
            ao_PK_Info.m_iItemCount = n;
            ao_PK_Info.m_pArguments = new m4VariantType [3 * n];

            l = strlen(ai_pReg->GetpNode_Data()->GetpNodeDef()->Id()) + 1;
            ao_PK_Info.m_pNodeName = new m4char_t[l];
            memcpy (ao_PK_Info.m_pNodeName, ai_pReg->GetpNode_Data()->GetpNodeDef()->Id(), l);

            i = 0;
            auxItem.Begin();
            while (auxItem.GetIndex() != M4DM_EOF_INDEX)
            {
                if (auxItem.ItemDef.IsPK())
                {
                    //Nombre del Item
                    ao_PK_Info.m_pArguments[3 * i].SetString((const m4pchar_t)auxItem.ItemDef.Id());
                    //Valor
                    auxItem.Value.Get(ao_PK_Info.m_pArguments[(3 * i) + 1]);
                    if (ao_PK_Info.m_pArguments[(3 * i) + 1].Type == M4CL_CPP_TYPE_STRING_VAR)
                    {
                        m4pchar_t auxpointer = ao_PK_Info.m_pArguments[(3 * i) + 1].Data.PointerChar;
                        ao_PK_Info.m_pArguments[(3 * i) + 1].SetString(auxpointer);
                    }

                    //Operacion: M4CL_CASE_EQUAL_FUNCTION
                    ao_PK_Info.m_pArguments[(3 * i) + 2].SetDouble(M4CL_CASE_EQUAL_FUNCTION);

                    i ++;
                }
                auxItem.Next();
            }
        }
    }
}

void ClAccessBookMark::Reset(void)
{
	delete m_plNodeBookMark;
	m_plNodeBookMark = 0;

    if (m_pRegisters_Info)
	{
        delete[] m_pRegisters_Info;
		m_pRegisters_Info = 0;
	}

    if (m_pCurrents_Info)
	{
        delete[] m_pCurrents_Info;
		m_pCurrents_Info = 0;
	}
}

ClAccessBookMark::~ClAccessBookMark ()
{
	Reset ();
}

/**
 * Deja el acceso "to" apuntando al mismo sitio que el acceso "from".
 *
 * @param ai_accessfrom    Acceso origen.
 * @param ai_accessto    Acceso que se va a cambiar y se va a dejar apuntando al mismo sitio que "ai_accessfrom".
 * @return M4_SUCCESS.
 */
m4return_t ClAccessBookMark::Follow(ClAccess& ai_accessfrom, ClAccess &aio_accessto)
{
    m4uint16_t n = ai_accessfrom.Node.Count ();
	M4_ASSERT (n == aio_accessto.Node.Count ());

    for (m4uint32_t i = 0; i < n; i++) {
		ClNodeIndex ni(i);

		// Recordset.
		ClAccessRecordSet& arsFrom = ai_accessfrom.Node[ni].RecordSet;
		ClAccessRecordSet& arsTo = aio_accessto.Node[ni].RecordSet;

		arsTo._SetCurrentBlock(arsFrom.GetCurrentBlockRef());

		// Current.
		_ClRegister_Base& rbFrom = arsFrom.Current;
		_ClRegister_Base& rbTo = arsTo.Current;
		for (m4uint8_t j = 0; j < 2; j++) {
			M4_ASSERT (rbFrom.GetpNode_Data() == rbTo.GetpNode_Data());

			rbTo.Index = rbFrom.Index;
			rbTo.MMIndex = rbFrom.MMIndex;

				// Item.
				ClNoCallableItem& nciFrom = rbFrom.Item;
				ClNoCallableItem& nciTo = rbTo.Item;
				M4_ASSERT(nciFrom.GetpNode_ExtInfo() == rbTo.GetpNode_ExtInfo());

				rbTo.Index = rbFrom.Index;

			// Ahora lo hacemos con el Register.
			rbFrom = arsFrom.Register;
			rbTo = arsTo.Register;
		}
	}

    return M4_SUCCESS;
}

/**
 * Comprueba que el acceso "to" está apuntando al mismo sitio que el acceso "from".
 *
 * @param ai_accessfrom    Acceso origen.
 * @param ai_accessto    Acceso destino.
 */
void ClAccessBookMark::Verify (ClAccess& ai_accessfrom, ClAccess& ai_accessto)
{
    m4uint16_t n = ai_accessfrom.Node.Count ();
	M4_ASSERT(n == ai_accessto.Node.Count());

    for (m4uint16_t i = 0; i < n; i++) {
		ClNodeIndex ni(i);

		// Recordset.
		ClAccessRecordSet& arsFrom = ai_accessfrom.Node[ni].RecordSet;
		ClAccessRecordSet& arsTo = ai_accessto.Node[ni].RecordSet;

	    M4_ASSERT (arsFrom.GetCurrentBlock() == arsTo.GetCurrentBlock());

		// Current.
		_ClRegister_Base& rbFrom = arsFrom.Current;
		_ClRegister_Base& rbTo = arsTo.Current;
		for (m4uint8_t j = 0; j < 2; j++) {
			M4_ASSERT(rbFrom.GetpNode_Data() == rbTo.GetpNode_Data());

			M4_ASSERT(rbFrom.Index == rbTo.Index);
			M4_ASSERT(rbFrom.MMIndex == rbTo.MMIndex);
		
				// Item.
				ClNoCallableItem& nciFrom = rbFrom.Item;
				ClNoCallableItem& nciTo = rbTo.Item;
				M4_ASSERT(nciFrom.GetpNode_ExtInfo() == nciTo.GetpNode_ExtInfo());

				M4_ASSERT(nciFrom.GetIndex() == nciTo.GetIndex());

			// Ahora lo hacemos con el Register.
			rbFrom = arsFrom.Register;
			rbTo = arsTo.Register;
		}
    }
}

/**
 * Obtener un Token.
 * Es equivalente al char *strtok_r(char *s1, const char *s2, char **lasts);
 *
 * @param ai_pcString    Puntero al Inicio de la cadena a parsear.
 * @param ai_pcDelim    Cadena de caracteres (cualquiera de los caracteres indica el final de Token)+'\0'.
 * @param ai_pc    Puntero al siguiente caracter a procesar (para una segunda llamada con ai_pcString=NULL).
 * @return Puntero al token.
 */
m4char_t *ClAccessBookMark::_GetToken(const m4char_t* ai_pcString, const m4char_t* ai_pcDelim, m4char_t **ai_pc)
{
	m4bool_t bEncontrado = M4_FALSE;	//Token encontrado S/N
	const m4char_t *pcTemp;				//Puntero temporal
	m4char_t *pcGet;					//Puntero a la copia del Token
	m4char_t *pc;						//Puntero de parseo
	m4char_t *pcIni;					//Puntero al inicio del parseo

	//Mirar si es la 1a.llamada o subsiguientes, con NULL
	if (ai_pcString)
		pc = (m4char_t *)ai_pcString;	//Llamada inicial
	else
		pc = *ai_pc;					//Subsiguientes llamadas

	//Buscamos el Token, usando el delimitador
	for (pcGet=NULL, pcIni=pc; pc && !bEncontrado && *pc!='\0'; ++pc) {
		//Comparamos con todos los delimitadores indicados
		for (pcTemp=ai_pcDelim; pcTemp && *pcTemp!='\0'; ++pcTemp) {
			if (*pc == *pcTemp) {
				*pc = 0;	//Null terminated
				pcGet=pcIni;
				bEncontrado = M4_TRUE;
				break;
			}
		}
	}	
	*ai_pc = pc;	//Actualizamos el puntero, para las subsiguientes llamadas
	return pcGet;	//Devolvemos puntero a la copia del token dinamico
}


// Funcion:	Deserializa el bookmark. Parsea un path, y modifica el estado del bookmark (Convert PathToBookmark)
// Entrada:	ai_poChannel	=	Puntero al Canal
//			ai_pcString		=	Puntero al string del Path
// Salida:	return	= M4_ERROR | M4_SUCCESS | M4_WARNING (un index de registro, no existe)
m4return_t ClAccessBookMark::DeserializeBranch(ClChannel *ai_poChannel, const m4char_t* ai_pcString)
{
	m4return_t ret=M4_SUCCESS;
	//* Input  = String Path
	//* Input	 = Channel
	//* Output = (this) Bookmark
	//- poAccess = CreateAccess()
	//- Parsear string, y posicionar
	//- this->GetFrom(poAccess)
	//- Destroy poAccess

	ClAccess*	poAccess;
	ClRegisterIndex oRegisterIndex;

	//Verificacion
	if (!ai_poChannel || !ai_pcString)
		return M4_ERROR;
	
	//Creamos un Access al Canal
	poAccess = ai_poChannel->CreateAccess();
	if (!poAccess)
		return M4_ERROR;
	
	m4bool_t	bIsEOF;
	//m4uint32_t	iIndex;
	m4uint32_t	iIdReg;
	//m4uint32_t	iCount;

	//Cabecera de Path - Ahora no se va a usar
	//m4char_t *pcIdChannel			= NULL;
	//m4char_t *pcHandleAccesClient	= NULL;
	//m4char_t *pcHandleAccesServer	= NULL;
	//Path
	m4char_t *pcIdNode	= NULL;
	m4char_t *pcIdReg	= NULL;
	m4char_t *pcIndex	= NULL;
	m4char_t *pcCount	= NULL;
	m4pchar_t pcExistsItem = 0;
	m4pchar_t pcIdItem = 0;

	m4char_t *pc = NULL;

	//Parseamos string, y posicionamos el access
	m4char_t *pcString = new m4char_t[strlen(ai_pcString)+1];	//Copia local, para modificar
	strcpy(pcString, ai_pcString);
	pc=pcString;

	/*
	pcIdChannel			= _GetToken(pcString, "#", &pc);
	pcHandleAccesClient	= _GetToken(NULL, "#", &pc);
	pcHandleAccesServer	= _GetToken(NULL, "#", &pc);

	//Validamos la cabecera del Path
	if (!pcIdChannel || !pcHandleAccesClient || !pcHandleAccesServer) {
		//Destruimos el Access
		poAccess->Destroy();
		poAccess = NULL;
		if (pcString)
			delete [] pcString;
		return M4_ERROR;
	}
	*/

	//Parseamos el Path
	for ( ; pc && *pc!='\0'; ) {
		//Parser		
		pcIdNode	= _GetToken(NULL, "(", &pc);
		pcIdReg		= _GetToken(NULL, "/", &pc);
		pcIndex		= _GetToken(NULL, "/", &pc);
		pcCount		= _GetToken(NULL, ")", &pc);
		
		//Get IdReg
		if (pcIdReg) {
			if ( strcmp(pcIdReg, "EOF") == 0 )
				bIsEOF = M4_TRUE;	//EOF
			else {
				bIsEOF = M4_FALSE;
				iIdReg = (m4uint32_t)atoi(pcIdReg);
			}
		}
		//Get Index
		/*
		if (pcIndex) {
			if ( strcmp(pcIndex, "EOF") == 0 )
				bIsEOF = M4_TRUE;	//EOF
			else {
				bIsEOF = M4_FALSE;
				iIndex = (m4uint32_t)atoi(pcIndex);
			}
		}
		*/
		//Get Count
		//if (pcCount)
		//	iCount = atoi(pcCount);
		
		//Validamos el Id. de Nodo
		if (pcIdNode && pcIdReg && pcIndex && pcCount) {
			if ( poAccess->GetpCMCR()->GetChannelNodeHandleById(pcIdNode) != 0 ) {
				//Nos posicionamos, usando el Access	(ojo, que se puede disparar la autoload)
				if (bIsEOF) {
					//Nos movemos a EOF
					poAccess->Node[pcIdNode].RecordSet.Current.MoveToEOF();
				} else {
					//Nos movemos al indice de Registro indicado
					if ( poAccess->Node[pcIdNode].RecordSet.FindRegisterByID(iIdReg, oRegisterIndex) == M4_SUCCESS) {
						if (poAccess->Node[pcIdNode].RecordSet.Current.MoveTo(oRegisterIndex) != M4_TRUE) {
							ret=M4_WARNING;
							break;
						}
					} else {	// Record with ID doesn't exist
						poAccess->Node[pcIdNode].RecordSet.Current.MoveToEOF(); // Move to EOF
						ret=M4_WARNING;
						break;
					}
				}
			}else{
				ret=M4_WARNING;	//Id. de Nodo invalido
				break;
			}
		} else {
			ret=M4_ERROR;	//Error en el parser
			break;
		}

		//Next sub-Path
		if (*pc == '\\')
			++pc;	//salta '\'
		if (*pc == '&') {
			if (*(pc+1) == '&')
				break;	// "&&" encontrado => No seguir parseando
		}
	}
	// Miramos a ver si viene Item
	// #IdItem&&NItem....
	pcExistsItem = _GetToken(NULL,"#",&pc);
	if (pcExistsItem){
		pcIdItem = _GetToken(NULL,"&",&pc);
		if (pcIdItem){
			poAccess->Node->RecordSet.Current.Item.MoveTo(pcIdItem);
		}
		else{
			// Formato incorrecto.
			ret = M4_ERROR;
		}

	}

	//Asignamos el Bookmark a partir del Access
	this->GetFrom(*poAccess);

	//Destruimos el Access
	poAccess->Destroy();
	poAccess = NULL;
	if (pcString)
		delete [] pcString;

	return ret;
}

// Funcion:	Serializa un Bookmark. Escribe la rama del nodo indicado (llama al GetTreePath)
// Entrada:	ai_poChannel	=	Puntero al Canal
//			ai_poAccess		=	Puntero al Access (que deseamos utilizar) o NULL (si deseamos crear uno temporal)
//			ai_hNode		=	Handle del Nodo, cuya rama queremos serializar
// Salida:	ao_pcBuffer		=	Puntero al Buffer de salida, donde se volcara el path. Si NULL => Devolvemos el tamaño de buffer necesario en "aio_uiLongBuffer"
//			aio_uiLongBuffer=	Longitud del Buffer disponible. Si "ao_pcBuffer" = NULL o aio_uiLongBuffer<Tamaño necesario => Devolvemos el tamaño necesario.
//			return	= M4_ERROR | M4_SUCCESS | M4_WARNING(Long.del buffer insuficiente)
m4return_t ClAccessBookMark::SerializeBranch(ClChannel *ai_poChannel, ClAccess *ai_poAccess, m4uint32_t ai_hNode, m4char_t *ao_pcBuffer, m4uint32_t *aio_uiLongBuffer)
{
	m4return_t ret=M4_SUCCESS;

	//Input  = (this) Bookmark
	//Input	 = Channel
	//Input	 = Access o Null
	//Output = String
	//Dump String = GetTreePath

	ClAccess*	poAccess;
	string		stringPath;
	//ClAccessBookMark *poTempBookMark;	//BookMark temporal
	ClAccessBookMark oTempBookMark;	//BookMark temporal

	//Verificacion
	if (!ai_poChannel || !aio_uiLongBuffer)
		return M4_ERROR;

	if (!ai_poAccess) {
		//Creamos un Access al Canal (temporal)
		poAccess = ai_poChannel->CreateAccess();
		if (!poAccess)
			return M4_ERROR;
	} else {
		//Utilizamos el Access
		//poTempBookMark = new ClAccessBookMark();
		//if (!poTempBookMark)
		//	return M4_ERROR;
		oTempBookMark.GetFrom(*ai_poAccess);	//Nos guardamos la posicion original
		poAccess = ai_poAccess;
	}
	
	//Aplicamos el Bookmark al Access
	this->ApplyTo(*poAccess);
	
	//if ( poAccess->Node.Get(ClHandle(ai_hNode)) != NULL ) {
		//Volcamos el Path: IdChannel#hAccessClient#hAccessServer#PATH (antes)
		//Volcamos el Path: PathIds&&PathNames (ahora)
		stringPath = GetTreePath(poAccess, ai_hNode);
		//Copiamos resultado
		if (ao_pcBuffer) {
			strncpy(ao_pcBuffer, stringPath.c_str(), *aio_uiLongBuffer);
			ao_pcBuffer[*aio_uiLongBuffer-1]='\0';
			if (*aio_uiLongBuffer<=stringPath.length()) {
				*aio_uiLongBuffer = stringPath.length()+1;	//Devolvemos el Tamaño necesario de buffer
				ret=M4_WARNING;	//Longitud de buffer insuficiente
			}
		}else {
			*aio_uiLongBuffer = stringPath.length()+1;	//Nos piden el Tamaño necesario de buffer
		}
	//} else {
	//	//El Handle de nodo no es valido
	//	ret = M4_ERROR;
	//}
	
	if (!ai_poAccess) {
		//Destruimos el Access (temporal)
		poAccess->Destroy();
		poAccess = NULL;
	} else {
		oTempBookMark.ApplyTo(*ai_poAccess);		//Restauramos la posicion original
		//delete [] poTempBookMark;
	}

	return ret;
}

/**
 * Operador <<. Vuelca el contenido del AccessBookMark en un stream.
 *
 * @param aio_os    Stream de salida.
 * @param ai_accessBookMark    AccessBookMark que queremos volcar al stream.
 * @return ostream    Stream de salida.
 */
M4_DECL_M4DM ostream& operator<< (ostream& aio_os, const ClAccessBookMark& ai_accessBookMark)
{
	// Si hicimos un GetFrom().
    if (ai_accessBookMark.m_plNodeBookMark) {
		/*
		 * Nos recorremos la lista de atras a delante para que salgan primero los padres.
		 */
		list<ClNodeBookMark>::reverse_iterator it;
		for (it = ai_accessBookMark.m_plNodeBookMark->rbegin(); it != ai_accessBookMark.m_plNodeBookMark->rend(); it++) {
			aio_os << (*it) << endl;
		}

		aio_os << "Current Node = " << ai_accessBookMark.m_nodeHandle << endl;
    }
	else
		aio_os << "No se hizo GetFrom()" << endl;

	return aio_os;
}


////////////////////////////////////////////////////////////////////////////////
//
// ClNodeBookMark
//

/**
 * Operador de igualdad.
 * Dos ClAccessBookMark son iguales si todos sus miembros son iguales.
 *
 * @ai_param ai_left    El operador izquierdo (el ClAccessBookMark que está a la izquierda del ==).
 * @return M4_TRUE  = si todos los miembros son iguales,
 *         M4_FALSE = en caso contrario.
 */
m4bool_t ClNodeBookMark::operator == (const ClNodeBookMark& ai_left)const
{
	if (currentIndex != ai_left.currentIndex)
		return M4_FALSE;

	if (currentMMIndex != ai_left.currentMMIndex)
		return M4_FALSE;

	if (currentItemIndex != ai_left.currentItemIndex)
		return M4_FALSE;

	if (registerIndex != ai_left.registerIndex)
		return M4_FALSE;

	if (registerMMIndex != ai_left.registerMMIndex)
		return M4_FALSE;

	if (registerItemIndex != ai_left.registerItemIndex)
		return M4_FALSE;

	if (pNode != ai_left.pNode)
		return M4_FALSE;

	if (nodeIndex != ai_left.nodeIndex)
		return M4_FALSE;

	return M4_TRUE;
}

/**
 * Operador menor.
 * Este operador puede servir para insertar los elementos en la lista. Por esto vamos a definir
 * que el ClAccessBookMark derecho es menor que el ClAccessBookMark izquierdo cuando el nodo del
 * ClAccessBookMark derecho es hijo del nodeo del ClAccessBookMark izquierdo.
 *
 * @param ai_left    El operador izquierdo (el ClAccessBookMark que está a la izquierda del ==).
 * @return M4_TRUE  = el nodo del ClAccessBookMark derecho es hijo del ClAccessBookMark izquierdo,
 *         M4_FALSE = en caso contrario.
 */
m4bool_t ClNodeBookMark::operator < (const ClNodeBookMark& ai_left)const
{
	ClNode_Data*      pParentNodeData = 0;
	ClNode_Connector* pNodeConector   = pNode->GetpNode_Data()->GetpExtInfo()->Parent.GetpNotFree();

	if (pNodeConector)
		pParentNodeData = pNodeConector->GetAddress();

	if (pParentNodeData == ai_left.pNode->GetpNode_Data())
		return M4_TRUE;

	return M4_FALSE;
}

/**
 * Operador <<. Vuelca el contenido del bookmark en un stream.
 *
 * @param aio_os    Stream de salida.
 * @param ai_nodeBookMark    NodeBookMark que queremos volcar al stream.
 * @return ostream    Stream de salida.
 */
M4_DECL_M4DM ostream& operator<< (ostream& aio_os, const ClNodeBookMark& ai_nodeBookMark)
{
	ClNode_Kernel* pNodeKernel = ai_nodeBookMark.pNode->GetpNode_Data()->GetpNodeDef();

	// Obtengo el padre del nodo.
	ClNode_Connector* pNodeConector = pNodeKernel->Parent.GetpNotFree();
	if (pNodeConector) {
		ClNode_Kernel* pParentpNodeKernel = pNodeConector->GetAddress()->GetpNodeDef();
		aio_os << "Nodo padre = " << pParentpNodeKernel->Id() << " (" << pParentpNodeKernel->GetHandle() << ")" << endl;
	}

	// Vuelco la información sobre mi mismo.
	aio_os << "Nodo       = " << pNodeKernel->Id() << " (" << pNodeKernel->GetHandle() << ")" << endl;
	aio_os << "Node Index = " << ai_nodeBookMark.nodeIndex << endl;
	aio_os << endl;
	aio_os << "Current MMIndex = " << ai_nodeBookMark.currentMMIndex   << endl;
	aio_os << "Current Index   = " << ai_nodeBookMark.currentIndex     << endl;
	aio_os << "Current Item    = " << ai_nodeBookMark.currentItemIndex << endl;
	aio_os << endl;
	aio_os << "Current MMIndex = " << ai_nodeBookMark.registerMMIndex   << endl;
	aio_os << "Current Index   = " << ai_nodeBookMark.registerIndex     << endl;
	aio_os << "Current Item    = " << ai_nodeBookMark.registerItemIndex << endl;
	aio_os << endl;

	// Vuelco el id del nodo.
	ClBlock_Channel_Proxy* pBlockPx = ai_nodeBookMark.pNode->Item.GetCurrentBlock();
	aio_os << "ID del bloque:" << endl;
	aio_os << "Block Id              = " << pBlockPx->ID.BlockId              << endl;
	aio_os << "Block Unique Key      = " << pBlockPx->ID.BlockUniqueKey       << endl;
	aio_os << "Parent Block Id       = " << pBlockPx->ID.Parent_BlockId       << endl;
	aio_os << "Parent Register Index = " << pBlockPx->ID.Parent_RegisterIndex << endl;
	aio_os << "Register Id           = " << pBlockPx->ID.RegisterId           << endl;
	return aio_os;
}


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


////////////////////////////////////////////////////////////////////////////////
//
// ClOldAccessBookMark
//

ClOldAccessBookMark::ClOldAccessBookMark()
	: ClAccessBookMark()
	, m_pnbm(0)
	, m_nnodes(0)
{
}

ClOldAccessBookMark::~ClOldAccessBookMark()
{
    if (m_pnbm)
        delete[] m_pnbm;
}

m4return_t ClOldAccessBookMark::GetFrom(ClAccess& ai_access)
{
    m4uint16_t n = ai_access.Node.Count ();

    if (n != m_nnodes) {
        if (m_pnbm)
			delete []m_pnbm;
        m_pnbm = new ClOldNodeBookMark [n];
        m_nnodes = n;

        M4_ASSERT (m_pnbm);
    }

    for (m4uint16_t i = 0; i < n; i++) {
        m_pnbm [i].GetFrom ( *(ai_access.Node.Get ((ClNodeIndex)i)) );
    }

	m_nodeHandle = ai_access.Node->GetpNodeDef()->GetHandle();

    return M4_SUCCESS;
}

m4return_t ClOldAccessBookMark::ApplyTo(ClAccess& ai_access)
{
    if (m_pnbm)
    {
        for (m4uint16_t i = 0; i < m_nnodes; i++) {
            m_pnbm [i].ApplyTo ( *(ai_access.Node.Get ((ClNodeIndex)i)) );
        }

	    ai_access.Node[m_nodeHandle];

        return M4_SUCCESS;
    }
    if ((m_pRegisters_Info)&&(m_pCurrents_Info))
    {
        _ApplyTo_PK (ai_access);
        return M4_SUCCESS;
    }

    return M4_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
//
// ClOldNodeBookMark
//

m4return_t ClOldNodeBookMark::GetFrom (ClNode &ai_node)
{
    RecordSet.GetFrom (ai_node.RecordSet);

    return M4_SUCCESS;
}

m4return_t ClOldNodeBookMark::ApplyTo (ClNode &ai_node)
{
    RecordSet.ApplyTo (ai_node.RecordSet);

    return M4_SUCCESS;
}

m4return_t ClOldNodeBookMark::Follow (ClNode &ai_nodefrom, ClNode &ai_nodeto)
{
    ClOldAccessRecordSetBookMark::Follow (ai_nodefrom.RecordSet, ai_nodeto.RecordSet);

    return M4_SUCCESS;
}

void ClOldNodeBookMark::Verify (ClNode &ai_nodefrom, ClNode &ai_nodeto)
{
    ClOldAccessRecordSetBookMark::Verify (ai_nodefrom.RecordSet, ai_nodeto.RecordSet);
}

////////////////////////////////////////////////////////////////////////////////
//
// ClOldAccessRecordSetBookMark
//

ClOldAccessRecordSetBookMark::ClOldAccessRecordSetBookMark()
{
}

ClOldAccessRecordSetBookMark::~ClOldAccessRecordSetBookMark()
{
}

m4return_t ClOldAccessRecordSetBookMark::GetFrom (ClAccessRecordSet &ai_rs)
{
//    pBlock = ai_rs.GetCurrentBlock ();

    m_bIsNihil = ai_rs.GetCurrentBlock()->IsNihil();

    m_oID = ai_rs.GetCurrentBlock ()->ID;

    Register.GetFrom (ai_rs.Register);
    Current.GetFrom (ai_rs.Current);

    return M4_SUCCESS;
}

m4return_t ClOldAccessRecordSetBookMark::ApplyTo (ClAccessRecordSet &ai_rs)
{
	ClBlockReference br;
    if (!m_bIsNihil)
    {
        //ClBlock_Channel_Proxy *pBlock;
        m4return_t res = ai_rs.GetpNode_Data ()->GetpBlockStore ()->Get (m_oID.BlockId, m_oID.RegisterId, br);
		    //que no lo cree

        if (res != M4_SUCCESS) {
		    br.Set (&g_oNihilBlock);
	    }
    }
    else
		br.Set (&g_oNihilBlock);

	ai_rs._SetCurrentBlock (br);

    Register.ApplyTo (ai_rs.Register);
    Current.ApplyTo (ai_rs.Current);

    return M4_SUCCESS;
}

m4return_t ClOldAccessRecordSetBookMark::Follow (ClAccessRecordSet &ai_rsfrom, ClAccessRecordSet &ai_rsto)
{
    ai_rsto._SetCurrentBlock (ai_rsfrom.GetCurrentBlockRef ());

    ClOldRegisterBookMark::Follow (ai_rsfrom.Register, ai_rsto.Register);
    ClOldRegisterBookMark::Follow (ai_rsfrom.Current, ai_rsto.Current);

    return M4_SUCCESS;
}

void ClOldAccessRecordSetBookMark::Verify (ClAccessRecordSet &ai_rsfrom, ClAccessRecordSet &ai_rsto)
{
    M4_ASSERT (ai_rsto.GetCurrentBlock () == ai_rsfrom.GetCurrentBlock ());

    ClOldRegisterBookMark::Verify (ai_rsfrom.Register, ai_rsto.Register);
    ClOldRegisterBookMark::Verify (ai_rsfrom.Current, ai_rsto.Current);
}

////////////////////////////////////////////////////////////////////////////////
//
// ClOldRegisterBookMark
//

m4return_t ClOldRegisterBookMark::GetFrom (_ClRegister_Base &ai_reg)
{
    Index = ai_reg.Index;
    MMIndex = ai_reg.MMIndex;

    Item.GetFrom (ai_reg.Item);
    
    return M4_SUCCESS;
}

m4return_t ClOldRegisterBookMark::ApplyTo (_ClRegister_Base &ai_reg)
{
    //m4uint32_t n = ai_reg.Count();
		// ^----- no! : dispara autoload, o ejecuta filtros dinámicos, lo que nos puede
		//meter en bucles infinitos

	//comprobación rudimentadia para saber si nos vamos a ir fuera de rango
    //cuidado pq si el currentblock es un nihil y pedimos el count, lanzamos
    //un warning nosotros mismos:
    if (ai_reg.GetCurrentBlock ()->IsNihil ()) {
        Index = M4DM_EOF_INDEX;
        MMIndex = ClMMIndex_EOF;
    }
    else {
	    m4uint32_t n = ai_reg.GetCurrentBlock ()->Count();
        if (((m4uint32_t)MMIndex) >= n)
        {
            Index = M4DM_EOF_INDEX;
            MMIndex = ClMMIndex_EOF;
        }
    }

	//pero faltarían más comprobaciones, si tenemos filtros, podemos estar fuera de rango
	//y no nos damos cuenta:
    ai_reg._SetIndex (Index);
    ai_reg.MMIndex = MMIndex;

    Item.ApplyTo (ai_reg.Item);

    return M4_SUCCESS;
}

m4return_t ClOldRegisterBookMark::Follow (_ClRegister_Base &ai_regfrom, _ClRegister_Base &ai_regto)
{
    M4_ASSERT (ai_regfrom.GetpNode_Data () == ai_regto.GetpNode_Data ());

    ai_regfrom.Index = ai_regto.Index;
    ai_regfrom.MMIndex = ai_regto.MMIndex;

    ClOldItemBookMark::Follow (ai_regfrom.Item, ai_regto.Item);

    return M4_SUCCESS;
}

void ClOldRegisterBookMark::Verify (_ClRegister_Base &ai_regfrom, _ClRegister_Base &ai_regto)
{
    M4_ASSERT (ai_regfrom.GetpNode_Data () == ai_regto.GetpNode_Data ());

    M4_ASSERT (ai_regfrom.Index == ai_regto.Index);
    M4_ASSERT (ai_regfrom.MMIndex == ai_regto.MMIndex);

    ClOldItemBookMark::Verify (ai_regfrom.Item, ai_regto.Item);
}

////////////////////////////////////////////////////////////////////////////////
//
// ClOldItemBookMark
//

m4return_t ClOldItemBookMark::GetFrom (ClNoCallableItem &ai_item)
{
    Index = ai_item.GetIndex();
    
    return M4_SUCCESS;
}

m4return_t ClOldItemBookMark::ApplyTo (ClNoCallableItem &ai_item)
{
    ai_item.MoveTo (Index);

    return M4_SUCCESS;
}

m4return_t ClOldItemBookMark::Follow (ClNoCallableItem &ai_itemfrom, ClNoCallableItem &ai_itemto)
{
    M4_ASSERT (ai_itemfrom.GetpNode_ExtInfo () == ai_itemto.GetpNode_ExtInfo ());

    ai_itemfrom.Index = ai_itemto.GetIndex();

    return M4_SUCCESS;
}

void ClOldItemBookMark::Verify (ClNoCallableItem &ai_itemfrom, ClNoCallableItem &ai_itemto)
{
    M4_ASSERT (ai_itemfrom.GetpNode_ExtInfo () == ai_itemto.GetpNode_ExtInfo ());

    M4_ASSERT (ai_itemfrom.GetIndex() == ai_itemto.GetIndex());
}
