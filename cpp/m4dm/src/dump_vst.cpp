//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// Module:           CVM - DM
// File:             mem_vst.cpp
// Project:          CVM
// Author:           Meta Software M.S. , S.A
// Date:             01/10/97
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Memory Dump 'Visitor'
//
//
//==============================================================================

#include "dump_vst.hpp"

#include "m4stl.hpp"
#include "dm.hpp"
#include "channel.hpp"
#include "chan_knl.hpp"
#include "access.hpp"
#include "px_block.hpp"
#include "node.hpp"
#include "node_knl.hpp"
#include "index.hpp"
#include "nodedef.hpp"
#include "itemdef.hpp"
#include "arecset.hpp"

#include "blockref.hpp"
#include "computil.hpp"
#include "compst_common.hpp"

#include "colitemdefit.hpp"



///////////////////////////////////////////////////////////////////////////////
//
// _AClDump_Visitor
//


_AClDump_Visitor::_AClDump_Visitor (ostream &ai_osf)
{
    m_pofs = &ai_osf;    
}


_AClDump_Visitor::~_AClDump_Visitor ()
{
}


///////////////////////////////////////////////////////////////////////////////
//
// ClSimpleDump_Visitor
//
m4return_t ClSimpleDump_Visitor::Visit_Channel (ClChannel &ai_channel)
{
    return Visit_ChannelData (*(ai_channel.GetpChannel_Data ()));
}

m4return_t ClSimpleDump_Visitor::Visit_ChannelData (ClChannel_Data &ai_channel)
{
    m4uint32_t i;
    for (i = 10; i; i--) {
        (*m_pofs) << "////////////////";
    }

    (*m_pofs) << "Channel:" << ai_channel.GetpChannelDef ()->Id () << endl;
    (*m_pofs) << Visit_Operation (ai_channel.Operation) << endl;

    Visit_NodeDirChannel (ai_channel.Node);

    return M4_SUCCESS;
}

void _dumpindex (ostream *ai_posf, m4pchar_t ai_ptitle, m4uint32_t ai_index)
{
    (*ai_posf) << ai_ptitle;

    if ((m4int32_t)ai_index == -1)
        (*ai_posf) << "<EOF>";
    else
        (*ai_posf) << ai_index;

    (*ai_posf) << endl;
}

m4return_t ClSimpleDump_Visitor::Visit_Access (ClAccess &ai_access)
{
    m4uint32_t i;
    m4uint32_t n = ai_access.Node.Count ();

    //ClBlock_Channel_Proxy *pblock;

    for (i = 0; i < n; i++) {
        ai_access.Node[(ClNodeIndex)i];
        (*m_pofs) << "Node <" << ai_access.Node->GetpNodeDef ()->Id () << ">" << endl;

        const ClBlockReference &br = ai_access.Node->RecordSet.GetCurrentBlockRef ();
        
        (*m_pofs) << "\tCurrent Block: ";
        if (br->IsNihil ()) {
            (*m_pofs) << "<nihil>";
        }
        else {
            (*m_pofs) << "[" << br->ID.BlockUniqueKey << 
                ", (" << br->ID.BlockId << ", " << br->ID.RegisterId << "), (" << 
                br->ID.Parent_BlockId << ", " << br->ID.Parent_RegisterIndex << ")]";

            if (br->IsProxy ()) {
                (*m_pofs) << "; <PROXY>";
            }
            else {
                (*m_pofs) << "; Count: " << br->Count ();
            }
        }
        (*m_pofs) << endl;

        _dumpindex (m_pofs, "\tCurrent.Index: ", ai_access.Node->RecordSet.Current.Index);
        _dumpindex (m_pofs, "\t   Current.Item.Index: ", ai_access.Node->RecordSet.Current.Item.GetIndex());
        _dumpindex (m_pofs, "\tRegister.Index: ", ai_access.Node->RecordSet.Register.Index);
        _dumpindex (m_pofs, "\t   Register.Item.Index: ", ai_access.Node->RecordSet.Register.Item.GetIndex());
    }

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_NodeDirChannel (ClNodeDirectory_Data &ai_nodedirchannel)
{
    ClNodeIndex i;
    m4uint32_t n;

    n = ai_nodedirchannel.Count ();

    ClNode_Data *pnode;

    for (i = 0; i < n; i++) {
        pnode = &(ai_nodedirchannel [i]);

        Visit_NodeData (*pnode);
        Visit_BlockDir (*pnode);
    }

    m_pofs->flush ();

    return M4_SUCCESS;
}
/*
m4return_t ClSimpleDump_Visitor::Visit_NodeChannel (ClNode_Channel &ai_nc)
{
    return Visit_NodeData (*ai_nc.GetpNode_Data ());
}
*/
m4return_t ClSimpleDump_Visitor::Visit_NodeData (ClNode_Data &ai_nc)
{
    (*m_pofs) << "Node:" << ai_nc.GetpExtInfo ()->GetHandle () << "(" << ai_nc.GetpExtInfo ()->Id () << ")" << endl;
    (*m_pofs) << Visit_Operation (ai_nc.Operation) << endl;

    Visit_Methods (ai_nc.GetpExtInfo ());
//    Visit_ItemColInfo (ai_nc.Item, M4CL_ITEM_SCOPE_NODE);
//    Visit_ItemValues (ai_nc.Item, M4CL_ITEM_SCOPE_NODE);

	ClNoCallableItem item (M4CL_ITEM_SCOPE_NODE);
	item.Attach (&ai_nc);

    Visit_ItemColInfo (item, M4CL_ITEM_SCOPE_NODE);
    Visit_ItemValues (item, M4CL_ITEM_SCOPE_NODE);

	Visit_BlockDir (ai_nc);

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_NodeAccess (ClNode &ai_nc)
{
    (*m_pofs) << "Node:" << ai_nc.GetpNodeDef ()->GetHandle () << "(" << ai_nc.GetpNodeDef ()->Id () << ")" << endl;

    Visit_Methods (ai_nc.GetpNode_Data ()->GetpExtInfo ());
    Visit_ItemColInfo (ai_nc.Item, M4CL_ITEM_SCOPE_NODE);
    Visit_ItemValues (ai_nc.Item, M4CL_ITEM_SCOPE_NODE);

    m_pofs->flush ();

    return M4_SUCCESS;
}


static m4return_t s_visit_block (ClBlockReference &ai_br, void *ai_psdv)
{
	return ((ClSimpleDump_Visitor *)ai_psdv)->Visit_Block (ai_br);
}


m4return_t ClSimpleDump_Visitor::Visit_BlockDir (ClNode_Data &ai_chnode)
{    
    m4uint32_t n = ai_chnode.GetpBlockStore()->Count();

    (*m_pofs) << "Count: " << n << endl;

    if (n) {
        //itero por los bloques:
		ClCStoreUtilities::ForEach (ai_chnode.GetpBlockStore (), s_visit_block, this);
		/*
        _AClComponentStore_ForwardIt *pfi;

        pfi = ai_chnode.GetpBlockStore ()->CreateIterator ();
        M4_ASSERT (pfi);

        ClBlock_Channel_Proxy *pb;

        pb = pfi->Begin ();
        while (pb) {
            Visit_Block (*pb);

            pb = pfi->Next ();
        }

        ai_chnode.GetpBlockStore ()->DestroyIterator (pfi);
		*/
    }      
    else {
        // si no tiene bloques intentamos mostrar algo, para que conste:
        ClNoCallableItem item (M4DM_SCOPE_ALL);
        item.Attach (&ai_chnode);
        
        Visit_ItemColInfo (item, M4CL_ITEM_SCOPE_BLOCK);
        Visit_ItemColInfo (item, M4CL_ITEM_SCOPE_REGISTER);
    }

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_RecordSet (ClRecordSet &ai_recordset)
{
    return Visit_Block (ai_recordset.GetCurrentBlockRef ());
}
 
m4return_t ClSimpleDump_Visitor::Visit_Block (const ClBlockReference &ai_br)
{
    m4uint32_t i;

    for (i = 10; i; i--) {
        (*m_pofs) << "----------------";
    }
    (*m_pofs) << endl;     
    
    if (ai_br->IsNihil ()) {
        (*m_pofs) << "<NIHIL>" << endl;
    }
    else {
        if (ai_br->IsValid ()) {
            ClRecordSet rs (0); //sin autoload
            rs.SetCurrentBlock (ai_br);

            (*m_pofs) << "Count: " << rs.Count () << "\t";
            Visit_Operation (rs.Operation);
            (*m_pofs) << endl;
			(*m_pofs) << "Block ID: " << ai_br->ID.BlockUniqueKey << ", Parent Block ID: " << ai_br->ID.Parent_BlockId << ", Parent Register Index: " << ai_br->ID.Parent_RegisterIndex << endl;

            Visit_ItemColInfo (rs.Item, M4CL_ITEM_SCOPE_BLOCK);
            Visit_ItemValues (rs.Item, M4CL_ITEM_SCOPE_BLOCK);

            Visit_ItemColInfo (rs.Register.Item, M4CL_ITEM_SCOPE_REGISTER);

            {
                ClRegisterIndex index = rs.Register.Index;
                
                rs.Register.Begin ();

                while (rs.Register.Index != M4DM_EOF_INDEX) {    

                    /*
                    char buf[7];
                    sprintf (buf, "%03i", rs.Register.Index);
                    buf[3] = (rs.Register.Index == index)? 'R' : ' ';
                    buf[4] = (rs.Register.Index == Current.Index)? 'C' : ' ';
                    buf[5] = '>';
                    buf[6] = '\0';
                    (*m_pofs) << buf;
                    */

                    Visit_Register (rs.Register);

                    rs.Register.Next ();
                }    

                rs.Register.MoveTo (index);
            }
        }
        else {
            (*m_pofs) << "<PROXY>" << endl;
        }
    }

    for (i = 10; i; i--) {
        (*m_pofs) << "----------------";
    }
    (*m_pofs) << endl;

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_Register ( _ClRegister_Base &ai_register)
{
    if (ai_register.Index == M4DM_EOF_INDEX) {
        (*m_pofs) << "(EOF)" << endl;
    }
    else {
        (*m_pofs) << "(" << ai_register.Index << ")";
        Visit_Operation (ai_register.Operation);
        Visit_ItemValues (ai_register.Item, M4CL_ITEM_SCOPE_REGISTER);
    }

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_Methods (ClNode_Kernel *ai_pnk)
{
    (*m_pofs) << "Methods:" << endl;

    ClItemDefIt_Random itemdefit (ai_pnk);

    itemdefit.Begin ();
    while (itemdefit.Index != M4DM_EOF_INDEX) {

        if (itemdefit.ItemDef.Type () == M4CL_ITEM_TYPE_METHOD) {
            (*m_pofs) << "\t" << itemdefit.ItemDef.Id () << " (";

            _dump_scope (itemdefit.ItemDef.Scope ());

            (*m_pofs) << ")" << endl;
        }

        itemdefit.Next ();
    }

    (*m_pofs) << endl;

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_ItemValues (ClNoCallableItem &ai_item, m4uint16_t ai_scope)
{
    ClItemIndex aux = ai_item.GetIndex();

    ai_item.Begin ();

    while (ai_item.GetIndex() != M4DM_EOF_INDEX) {
        if (ai_item.ItemDef.Scope () == ai_scope && ai_item.ItemDef.Type () != M4CL_ITEM_TYPE_METHOD) {
            Visit_Value (ai_item.Value, ai_item.ItemDef);
        }
        
        ai_item.Next ();
    }

    (*m_pofs) << endl;

    ai_item.MoveTo (aux);

    m_pofs->flush ();

    return M4_SUCCESS;
}

void ClSimpleDump_Visitor::_dump_type (m4uint16_t ai_type)
{
    switch (ai_type) {
        case M4CL_ITEM_TYPE_METHOD:
            (*m_pofs) << "M";
            break;

        case M4CL_ITEM_TYPE_PROPERTY:
            (*m_pofs) << "P";
            break;

        case M4CL_ITEM_TYPE_FIELD:
            (*m_pofs) << "F";
            break;

        case M4CL_ITEM_TYPE_CONCEPT:
            (*m_pofs) << "C";
            break;
    }
}

void ClSimpleDump_Visitor::_dump_scope (m4uint16_t ai_scope)
{
    switch (ai_scope) {
        case M4CL_ITEM_SCOPE_NODE:
            (*m_pofs) << "N";
            break;

        case M4CL_ITEM_SCOPE_BLOCK:
            (*m_pofs) << "B";
            break;

        case M4CL_ITEM_SCOPE_REGISTER:
            (*m_pofs) << "R";
            break;
    }
}

m4return_t ClSimpleDump_Visitor::Visit_ItemColInfo (ClNoCallableItem &ai_item, m4uint16_t ai_scope)
{
    ClItemIndex aux = ai_item.GetIndex();

    ai_item.Begin ();

    m4uint32_t l;

    string id;
    string buffer;

    while (ai_item.GetIndex() != M4DM_EOF_INDEX) {

        if (ai_item.ItemDef.Scope () == ai_scope && ai_item.ItemDef.Type () != M4CL_ITEM_TYPE_METHOD) {
            switch (ai_item.ItemDef.CppType ()) {
                case M4CL_CPP_TYPE_NUMBER:
                case M4CL_CPP_TYPE_DATE:
                    l = 16;
                    break;

                case M4CL_CPP_TYPE_STRING:
                    l = ai_item.ItemDef.Precision ();
                    break;
            }

            id = ai_item.ItemDef.Id ();

            m4uint32_t length;
            length = id.length ();
            m4int32_t i = l - length - 4;
            
            if (i > 0) {
                do {
                    buffer.append (" ");
                } while (--i);

                buffer.append (id);
            }
            else {                
                buffer.append (id.substr (0, length - i));
            }            

            (*m_pofs) << buffer << "(";

            _dump_scope (ai_item.ItemDef.Scope ());
            _dump_type (ai_item.ItemDef.Type ());

            (*m_pofs) << ")";

            (*m_pofs) << "|";

            buffer.erase ();
        }

        ai_item.Next ();
    }
    (*m_pofs) << endl;

    ai_item.MoveTo (aux);

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_Value (_ClValue_Read_Slices &ai_value, ClItemDef &ai_itemdef)
{
    m4uint32_t n = ai_value.Count ();

    m4pchar_t p;

    (*m_pofs) << "|";

    if (n) {
        // tiene tramos        
        (*m_pofs) << "(" << n << ")";

        ClSliceIndex si;
        for (si = 0; si < n; si++) {

            ai_value.MoveTo (si);

            (*m_pofs) << "[";
            (*m_pofs) << ai_value.GetStartDate ();
            (*m_pofs) << ",";
            (*m_pofs) << ai_value.GetEndDate ();
            (*m_pofs) << "]:";

            p = (m4pchar_t)ai_value.Get ();

            if (p == 0) {
                (*m_pofs) << "NULL";
            }
            else {
                //Ctrl+C-------------------
                    switch (ai_itemdef.CppType ()) {
                        case M4CL_CPP_TYPE_NUMBER:
                        case M4CL_CPP_TYPE_DATE:
                            // directamente el valor
                            (*m_pofs) << *(m4double_t *)p;
                            break;

                        case M4CL_CPP_TYPE_STRING:
                            (*m_pofs) << (m4pcchar_t)p;
                            break;

                        //default:
                            //res = M4_ERROR; // tipo no válido
                    }
                //---------------------------
            }
            (*m_pofs) << ",";
        }        
    }
    else {
        if (ai_itemdef.Type () != M4CL_ITEM_TYPE_METHOD) {
            p = (m4pchar_t)ai_value.Get ();
            //Ctrl+V-------------------
                switch (ai_itemdef.CppType ()) {
                    case M4CL_CPP_TYPE_NUMBER:
                    case M4CL_CPP_TYPE_DATE:
                        {
                            if (p) {
                                char buf [32];
                                m4int32_t l = 16 - sprintf (buf, "%8.8f", *(m4double_t *)p);
                                for (;l>0 ; l--)
                                    (*m_pofs) << " ";
                                buf[16] = '\0';
                                (*m_pofs) << buf;
                            }
                            else {
                                (*m_pofs) << "            NULL";
                            }
                        }
                        break;

                    case M4CL_CPP_TYPE_STRING:
                        {
                            m4int32_t l = ai_itemdef.Precision ();
                            if (p) {
                                l -= strlen (p);
                                for (;l>0 ; l--)
                                    (*m_pofs) << " ";
                                (*m_pofs) << (m4pcchar_t)p;
                            }
                            else {
                                l -= 4;
                                for (;l>0 ; l--)
                                    (*m_pofs) << " ";
                                (*m_pofs) << "NULL";
                            }
                        }
                        break;
                    //default:
                        //res = M4_ERROR; // tipo no válido
                }
            //---------------------------
        }
    }

    (*m_pofs) << "|";

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClSimpleDump_Visitor::Visit_Operation ( _ClOperation_Base &ai_operation)
{
    char b[7];

    b[0] = '[';
    b[1] = ai_operation.IsModified ()? 'M' : ' ';
    b[2] = ai_operation.IsUpdated  ()? 'U' : ' ';
    b[3] = ai_operation.IsNew      ()? 'N' : ' ';
    b[4] = ai_operation.IsDeleted  ()? 'D' : ' ';
    b[5] = ']';
    b[6] = '\0';

    (*m_pofs) << b;

    m_pofs->flush ();

    return M4_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
//
// ClNodeDirectory_Channel
//

m4return_t ClTreeDump_Visitor::Visit_Channel (ClChannel_Data &ai_channel)
{
    ClNodeIndex i;
    m4uint32_t n;

    n = ai_channel.Node.Count ();

    ClNode_Data *pnode;	
    //ClBlock_Channel_Proxy *pblock;
	ClBlockReference br;

    for (i = 0; i < n; i++) {
        pnode = &(ai_channel.Node [i]);

        if (pnode->GetpExtInfo ()->IsRoot ()) {
            pnode->GetpBlockStore ()->Get (0, 0, br);
            Visit_Block (br);
        }
        else {
            m4uint16_t nic;
            nic = pnode->GetpExtInfo ()->GetNodeNumberOfInvConnectors ();

            if (nic) {
                // se asume que el conector activo es el 0:
                m4uint16_t i = 0;
                ClHandle hcon;
                do {
                    hcon = pnode->GetpExtInfo ()->GetNodeInvConnectorHandleByPosition (i);
                    //?ClHandle hn = pnode->NodeDef.GetNodeConnectorFatherNodeHandle (hcon);
                    //?M4_ASSERT (hn);
                    //?ClNode_Kernel *pfather = (*this)[hn];

                    if (pnode->GetpExtInfo ()->GetNodeConnectorType (hcon) != M4CL_NODE_CONNECTOR_TYPE_FREE) {
                        // no sirve
                        nic--;
                        i++;
                    }
                    else {
                        // parece que el nodo es un 'free'
                        pnode->GetpBlockStore ()->Get (0, 0, br);
                        Visit_Block (br);
                        break;
                    }
                } while (nic);
            }
        }
    }

    m_pofs->flush ();

    return M4_SUCCESS;
}

m4return_t ClTreeDump_Visitor::Visit_Block (const ClBlockReference &ai_br, m4uint32_t ai_k, m4uint32_t ai_level)
{
    m4uint16_t i;
    m4uint16_t n;

    // tabuladores
    for (i=0; i<ai_level; i++)
        (*m_pofs) << "\t";    

    ClNode_Data *pnode;
    pnode = ai_br->GetpNode_Data ();
    (*m_pofs) << ai_k << "." << pnode->GetpExtInfo ()->Id () << " ";

    if (ai_br->IsNihil ()) {
        (*m_pofs) << "<NIHIL>" << endl;
    }
    else {
        
        //ClBlock_Channel_Proxy *pblock = &ai_block;

        if (ai_br->IsValid ()) {

            ClRecordSet rs (0); //sin autoload
            rs.SetCurrentBlock (ai_br);

            (*m_pofs) << rs.Count () << endl;

            // volcado:
            n = pnode->GetpExtInfo ()->GetNodeNumberOfRBConnectors ();

			ClBlockReference brchild;

            for (i = 0; i < n; i++) {

                rs.Register.Begin();

                m4uint32_t k = 0;

                while (rs.Register.Index != M4DM_EOF_INDEX) {					
                    rs.Register.GetChildBlock (i, brchild);

                    Visit_Block (brchild, k, ai_level + 1);
					brchild.Reset ();

                    rs.Register.Next ();

                    k++;
                }

                (*m_pofs) << endl << endl;
            }
        }
        else {
            (*m_pofs) << "<PROXY>" << endl;
        }

    }

    m_pofs->flush ();

    return M4_SUCCESS;
}

ostream & operator<< (ostream &ai_ofs, ClChannel &ai_channel)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_Channel (ai_channel);

    return ai_ofs;
}

ostream & operator<< (ostream &ai_ofs, ClChannel_Data &ai_channel)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_ChannelData (ai_channel);

    return ai_ofs;
}

ostream &operator<< (ostream &ai_ofs, ClAccess &ai_access)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_Access (ai_access);

    return ai_ofs;
}

ostream &operator<< (ostream &ai_ofs, ClNode &ai_node)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_NodeAccess (ai_node);

    return ai_ofs;
}

/*
ostream & operator<< (ostream &ai_ofs, ClNode_Channel &ai_nodech)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_NodeChannel (ai_nodech);

    return ai_ofs;
}
*/

ostream & operator<< (ostream &ai_ofs, ClNode_Data &ai_nodedata)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_NodeData (ai_nodedata);

    return ai_ofs;
}

ostream &operator<< (ostream &ai_ofs, _ClRegister_Base &ai_register)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_Register (ai_register);

    return ai_ofs;
}

ostream &operator<< (ostream &ai_ofs, ClRegister &ai_register)
{
    return ai_ofs << (_ClRegister_Base &)(ai_register);
}

ostream &operator<< (ostream &ai_ofs, ClRegister_Current &ai_register)
{
    return ai_ofs << (_ClRegister_Base &)(ai_register);
}

ostream &operator<< (ostream &ai_ofs, ClNoCallableItem &ai_item)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_ItemValues (ai_item, M4CL_ITEM_SCOPE_REGISTER);

    return ai_ofs;
}

ostream &operator<< (ostream &ai_ofs, ClRecordSet &ai_rs)
{
    ClSimpleDump_Visitor dv (ai_ofs);

    dv.Visit_RecordSet (ai_rs);

    return ai_ofs;
}

ostream &operator<< (ostream &ai_ofs, ClAccessRecordSet &ai_rs)
{
    return ai_ofs << (ClRecordSet &)ai_rs;
}
