
//==============================================================================
//
// (c) Copyright  1991-1997 Meta Software M.S., S.A
// All rights reserved.
//
// File:             Component.cpp
// Project:          Service presentation
// Author:           Meta Software M.S. , S.A
// Language:         C++
// Operating System: WINDOWS , UNIX ( HP, DEC , AIX , SUN)
// Design Document:
//
//
// Definition:
//
//    Implementation of the CComponent class.
//
//
//==============================================================================

#include "asndef.h"
#include "presfac.hpp"
#include "channel.hpp"
#include "chandef.hpp"
#include "m4mdrt.hpp"
#include "m4presres.hpp"
#include "cldbgutil.hpp"
#include "m4unicode.hpp"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComponent::CComponent()
{

	propBag = new CListComponent();
    m_access = new CAccessSeq();
	nameProp = NULL;
	length = 0;
	length_prop=0;
	length_nameProp=0;
	type_prop = ASNINTEGER;
	type_nameProp = ASNINTEGER;
	value_prop.int_value = 0;
	m_iInheritStatus = 0;
	m_bIsOrdered = M4_FALSE;
}

CComponent::CComponent(const CComponent& X)
{
	
	propBag			 = X.propBag;
    m_access		 = X.m_access;
	nameProp		 = X.nameProp;
	length			 = X.length;
	length_prop		 = X.length_prop;
	length_nameProp	 = X.length_nameProp;
	type_nameProp	 = X.type_nameProp;
	type_prop		 = X.type_prop;
	value_prop		 = X.value_prop;
	m_iInheritStatus = X.m_iInheritStatus;
	m_bIsOrdered     = X.m_bIsOrdered;
}

CComponent::~CComponent()
{
	if (m_access!=NULL) delete m_access;
	if (type_prop != ASNSequence)
	{
		if (nameProp!=NULL)
		{
			delete []nameProp;
		}

		if (type_prop == ASNGeneralString)
		{
			delete [](value_prop.char_value);
		}
	}
	if (propBag != NULL)
	{
		delete propBag;
		propBag = NULL;
	}
}

void CComponent::SetInheritStatus(m4uint32_t ai_iInheritStatus)
{
	m_iInheritStatus = ai_iInheritStatus; 		
}

m4uint32_t CComponent::GetInheritStatus()
{
	return m_iInheritStatus; 
}

m4uint32_t CComponent::GetLength()
{
	return length;
}

void  CComponent::SetLength(m4int32_t ai_iLength)
{
	length += ai_iLength;
}

m4uchar_t  CComponent::GetTypeNameProp()
{
	return type_nameProp;
}

m4uint32_t  CComponent::GetLengthNameProp()
{
	return length_nameProp;
}

void CComponent::GetNameProp(m4char_t *name)
{
	memcpy(name,nameProp,length_nameProp);
	name[length_nameProp] = '\0';
}

m4uchar_t  CComponent::GetTypeProp()
{
	return type_prop;
}

m4uint32_t  CComponent::GetLengthProp()
{
	return length_prop;
}

m4int32_t  CComponent::GetIntValueProp()
{
	return value_prop.int_value;
}

m4uchar_t  CComponent::GetBoolValueProp()
{
	return value_prop.byte_value;
}

void CComponent::GetStringValueProp(m4char_t *name)
{
	memcpy(name,value_prop.char_value,length_prop);
	name[length_prop] = '\0';
}

void  CComponent::SetTypeNameProp()
{
	type_nameProp = ASNGeneralString;
	SetLength(1);
}

void  CComponent::SetLengthNameProp(m4uint32_t ai_iLength)
{
	length_nameProp = ai_iLength;
	
	m4char_t n_b = calculateLength(ai_iLength);
	SetLength(n_b);
}

void  CComponent::SetNameProp(const m4char_t *the_name,m4uint32_t the_length)
{

	if (nameProp != NULL)
	{
		delete [](nameProp);
	}

	nameProp = new m4char_t[the_length + 1];
	memcpy(nameProp,the_name,the_length);
	nameProp[the_length] = '\0';

	SetLength(the_length);
}

void  CComponent::SetTypeProp(m4int_t type)
{
	switch (type){
	case T_INT :
		type_prop = ASNINTEGER;
		break;

	case T_BOOL :
		type_prop = ASNBOOLEAN;
		break;

	case T_STRING :
		type_prop = ASNGeneralString;
		value_prop.char_value = NULL;
		break;

	case T_SEQ :
		type_prop = ASNSequence;
		break;
	}
	SetLength(1);
}

m4int32_t  CComponent::RecalculeLength( CListComponent * ai_pLcomp )
{
	m4uchar_t b_T;
	m4int32_t cont = 0;
	CComponent *comp;
	m4uint32_t i = 0;
	

	while(i<ai_pLcomp->GetNumComponent())
	{
		comp = ai_pLcomp->View(i);
		i++;
		b_T = comp->GetTypeProp();
		
 		if (b_T != ASNSequence)
		{
			cont = comp->GetLength();
			SetLength(cont);
		}
		else
		{
			comp->_IniLength(1);
			comp->RecalculeLength(comp->propBag);
			cont = comp->GetLength();
			SetLength(cont);
		}
	}

	length_prop = length-1; // OJOOOO, quito el byte del TAG
	// se suma el numero de bytes que ocupa la longitud
	decodeLength(length_prop);
	return cont;
}

void  CComponent::SetLengthProp(m4bool_t traducir,m4int_t type,m4int32_t value,m4uint32_t ai_iLength)
{
	m4uint32_t longitud = 0, cont = 0;
	m4int32_t m_long = 0;

	switch (type)
	{
	case T_INT : 

		if ((value <= 127) && (value >= -128))
		{
			cont = 1;
		}
		else
		{
			if ((value <= 32767) && (value >= -32768))
			{
				cont = 2;
			}
			else
			{
				if ((value <= 8388607) && (value >= -8388608))
				{
					cont = 3;
				}
				else
				{
					cont = 4;
				}
			}
		}
		// num de bytes que ocupa el entero 
		length_prop = cont; 
		SetLength(1);
		break;

	case T_BOOL :

		length_prop = 1; 
		SetLength(1);
		break;

	case T_STRING :	

		m4uchar_t n_bytes;
		m4uint32_t aux;
		m4int_t i;
		i=0;
		length_prop = ai_iLength;
		if (ai_iLength >127)
		{
			aux = ai_iLength;
			while (aux != 0)
			{
				i++;
				aux >>= 8;
			}
			n_bytes = i;
		}
		else
		{
			n_bytes = 0;
		}

		// n_bytes contiene el numero de bytes que se necesitan para 
		// codificar la longitud
		SetLength(1+n_bytes);
		break;

	case T_SEQ :
		RecalculeLength(propBag);
		break;
	}
}

void  CComponent::SetBoolValueProp( m4uchar_t the_value)
{
	value_prop.byte_value = the_value;
	SetLength(1);
}

void  CComponent::SetIntValueProp( m4int32_t the_value)
{
	value_prop.int_value = the_value;
	SetLength(length_prop);
}

void  CComponent::SetStringValueProp( const m4char_t *the_value, m4uint32_t the_length)
{
	if (value_prop.char_value != NULL)
	{
		delete [](value_prop.char_value);
		value_prop.char_value = NULL;
	}

	//Vamos a permitir que se quede a NULL la propiedad, esto es con cuidadin.
	//En principio solo se empleará sí se han reseteado longitudes.
	if (the_value == NULL)
	{
		return;
	}
	
	// reservar memoria para la variable miembro
	value_prop.char_value = new m4char_t[the_length + 1]; // con \0

	memcpy(value_prop.char_value,the_value,the_length);
	value_prop.char_value[the_length] = '\0';
	// se incrementa la longitud del componente
	SetLength(the_length);
}

m4bool_t CComponent::IsNameProp( m4pcchar_t ai_pName )
{
	//Comprueba si el nombre coincide. El argumento SI es null terminado, pero el miembro NO lo es.

	if ( (ai_pName == NULL) || (nameProp == NULL) )
	{
		return M4_FALSE;
	}

	//Si los tamaños son diferentes es pq no son iguales.
	if (length_nameProp != strlen(ai_pName))
	{
		return M4_FALSE;
	}

	if (strncmp(nameProp, ai_pName, length_nameProp) != 0)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}

m4bool_t CComponent::IsNameProp( CComponent * ai_pCComponent )
{
	//Sobrecarga. Podemos pasar una componente con la que comparar el nombre.

	if ( (ai_pCComponent == NULL) || (nameProp == NULL) )
	{
		return M4_FALSE;
	}

	//Si los tamaños son diferentes es pq no son iguales.
	if (length_nameProp != ai_pCComponent->GetLengthNameProp())
	{
		return M4_FALSE;
	}

	m4char_t * pNameProp = new m4char_t[length_nameProp + 1];
	ai_pCComponent->GetNameProp(pNameProp);

	if (strncmp(nameProp, pNameProp, length_nameProp) != 0)
	{
		delete [] pNameProp;
		return M4_FALSE;
	}
	delete [] pNameProp;

	return M4_TRUE;
}

m4bool_t CComponent::IsValueProp( m4int32_t ai_iValue )
{
	if (type_prop != ASNINTEGER)
	{
		return M4_FALSE;
	}

	if (value_prop.int_value != ai_iValue)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}

m4bool_t CComponent::IsValueProp( m4uchar_t ai_bValue )
{
	if (type_prop != ASNBOOLEAN)
	{
		return M4_FALSE;
	}

	if (value_prop.byte_value != ai_bValue)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}

m4bool_t CComponent::IsValueProp( m4pcchar_t ai_pValue )
{
	//Comprueba si el valor coincide. El argumento SI es null terminado, pero el miembro NO lo es.

	//Esto sólo se aplica a componentes de tipo cadena.
	if (type_prop != ASNGeneralString)
	{
		return M4_FALSE;
	}

	if ( (ai_pValue == NULL) || (value_prop.char_value == NULL) )
	{
		return M4_FALSE;
	}

	//Si los tamaños son diferentes es pq no son iguales.
	if (length_prop != strlen(ai_pValue))
	{
		return M4_FALSE;
	}

	if (strncmp(value_prop.char_value, ai_pValue, length_prop) != 0)
	{
		return M4_FALSE;
	}

	return M4_TRUE;
}

m4bool_t CComponent::IsNameAndValueProp( CComponent * ai_pCComponent )
{
	//Comprobamos que el nombre y el valor es el mismo.
	m4bool_t bIsEqual = M4_FALSE;

	if (IsNameProp(ai_pCComponent) == M4_FALSE)
	{
		return M4_FALSE;
	}

	//Coinciden con nombre, veamos con tipo.
	if (type_prop != ai_pCComponent->GetTypeProp())
	{
		return M4_FALSE;
	}

	//Por último comprobamos si coinciden en valor.
	switch (type_prop)
	{
		case ASNBOOLEAN:
			bIsEqual = IsValueProp(ai_pCComponent->GetBoolValueProp());
			break;

		case ASNINTEGER:
			bIsEqual = IsValueProp(ai_pCComponent->GetIntValueProp());
			break;

		case ASNGeneralString:
			{
			m4uint32_t iValueSize = ai_pCComponent->GetLengthProp();
			m4char_t * pValue = new m4char_t [iValueSize + 1];			
			ai_pCComponent->GetStringValueProp( pValue );
			pValue[iValueSize] = NULL;
			bIsEqual = IsValueProp(pValue);
			delete [] pValue;
			break;
			}

		default: 
			return M4_FALSE;
	}

	return bIsEqual;
}

m4char_t CComponent::calculateLength(m4int32_t ai_iLength)
{
	m4int32_t aux;
	m4int_t i = 0;
	
	if (ai_iLength >127)
	{
		aux = ai_iLength;
		while (aux != 0)
		{
			i++;
			aux >>= 8;
		}

		return(1+i);
	}
	else
	{
		return(1);
	}
}


void CComponent::decodeLength(m4int32_t ai_iLength)
{
	m4char_t n_b = calculateLength(ai_iLength);
	SetLength(n_b);
}
	

void CComponent::writeLength(m4int32_t ai_iLength,COutputStream *outStr)
{
	m4int32_t aux;
	m4uchar_t n_b = 0;
	m4uchar_t acBytes[10];
	m4int_t i = 0,j;

	if (ai_iLength >127)
	{
		aux = ai_iLength;
		while (aux != 0)
		{
			acBytes[i++] = aux & (0x000000ff);
			aux >>= 8;
		}

		n_b = 128 +i;

		/// i representa los bytes que ocupa el numero
		/// que representa la longitud de la SEQUENCE
		outStr->writeByte(n_b);
		for(j=i-1;j>=0;j--)
		{
			outStr->writeByte(acBytes[j]);
		}
	}
	else
	{
		n_b = (m4uchar_t)ai_iLength;
		outStr->writeByte(n_b);	
	}
}

void  CComponent::decode(COutputStream *outStr,m4bool_t traducir,CComponent *padre)
{
	m4uchar_t b_TNP,b_TP,b_V;
	
	
	m4uint32_t v;
	m4int32_t m_length = 0;
	m4uint32_t i = 0, cont = 0;

	m4bool_t firstSeq = M4_TRUE,segurity = M4_FALSE;
	
	if(traducir)
	{
		b_TP = GetTypeProp();     
		if (b_TP != ASNSequence)
		{
			m4uint32_t lengthString;

			m4uint32_t typePosition = outStr->GetLength();
			b_TNP = GetTypeNameProp();
			outStr->writeByte(b_TNP);

			lengthString = GetLengthNameProp();
			writeLength(lengthString, outStr);
			
			m4char_t *THEname=new m4char_t[lengthString+1];
			GetNameProp(THEname);
			outStr->writeString((m4uchar_t*)THEname, lengthString, typePosition);
			delete []THEname;

			typePosition = outStr->GetLength();
			b_TP = GetTypeProp();
			outStr->writeByte(b_TP);

			lengthString = GetLengthProp();
			writeLength(lengthString, outStr);
			
			switch (b_TP)
			{
			case ASNBOOLEAN :
				
				b_V = GetBoolValueProp();
				outStr->writeByte(b_V);
				break;

			case ASNINTEGER : 
							  
				m4uint32_t tmpMask;
				m4uint32_t tmpLength;
				tmpLength= length_prop;

				v = GetIntValueProp();

				while(tmpLength-- > 0)
				{
					tmpMask = 255 << (8 * tmpLength);
					b_V =(m4uchar_t) ((v & tmpMask) >> (8*tmpLength));
					outStr->writeByte(b_V);
				}
				break;

			case ASNGeneralString :
				m4char_t *name = new m4char_t[lengthString + 1];
				GetStringValueProp(name);
				outStr->writeString((m4uchar_t*)name, lengthString, typePosition);
				delete []name;
				break;
			}
		}
		else  /// es SEQUENCE
		{			
			// Lo primero simepre va a ser una SEQ
			b_TP = GetTypeProp();
			outStr->writeByte(b_TP);
			
			writeLength(length_prop,outStr);
			CComponent *m_comp;
			i=0;
			while (i<propBag->GetNumComponent())
			{
				m_comp = propBag->View(i);
				i++;
				m_comp->decode(outStr,traducir);
			}
		}
	
	} // end TRADUCIR
	else
	{
		b_TP = GetTypeProp();
		
		if (b_TP != ASNSequence)
		{
			m4uint32_t lengthString;

			m4uint32_t typePosition = outStr->GetLength();
			b_TNP = GetTypeNameProp();
			outStr->writeByte(b_TNP);

			lengthString = GetLengthNameProp();
			// escribir la longitud
			writeLength(lengthString, outStr);

			m4char_t *THEname = new m4char_t[lengthString + 1];
			GetNameProp(THEname);
			if((lengthString == M4PRESENTATION_GRANTS_SIZE) && (memcmp(THEname, M4PRESENTATION_GRANTS, lengthString) == 0))
			{
				segurity = M4_TRUE;
			}
			outStr->writeString((m4uchar_t*)THEname, lengthString, typePosition);
			delete []THEname;

			typePosition = outStr->GetLength();
			b_TP = GetTypeProp();
			outStr->writeByte(b_TP);

			lengthString = GetLengthProp();
			writeLength(lengthString, outStr);

			switch (b_TP){

			case ASNBOOLEAN:
				b_V = GetBoolValueProp();
				outStr->writeByte(b_V);
				break;

			case ASNINTEGER:				
				//No hacemos la distinción de si es seguridad. Vamos a dejar el reseteo del access por si
				//quisieramos comprobar qué grants se han aplicado y qué grants no.
				if(segurity == M4_TRUE)	{
					//outStr->writeByte((m4uchar_t)((padre->m_access)->grant));
					(padre->m_access)->grant = -1;
				}
				{
					m4uint32_t tmpMask;
					m4uint32_t tmpLength;
					tmpLength= length_prop;
			
					v = GetIntValueProp();

					while(tmpLength-- > 0)
					{
						tmpMask = 255 << (8 * tmpLength);
						b_V =(m4uchar_t) ((v & tmpMask) >> (8*tmpLength));
						outStr->writeByte(b_V);
					}
				}
				break;

			case ASNGeneralString:
				m4char_t *name = new m4char_t[lengthString + 1];
				GetStringValueProp(name);
				outStr->writeString((m4uchar_t*)name, lengthString, typePosition);
				delete []name;
				break;
			}
		}
		else  /// es SEQUENCE
		{
			b_TP = GetTypeProp();
			outStr->writeByte(b_TP);
			writeLength(length_prop,outStr);   /// OJOOOOOOOOO
			CComponent *m_comp;
			i=0;
			while (i<propBag->GetNumComponent()) 
			{
				m_comp = propBag->View(i);
				i++;				
				m_comp->decode(outStr,traducir,this);
				//delete(m_comp);
			}
		} // else  SEQ
	}// else  Traducir
}

void CComponent::_IniLength(m4uint32_t ai_iValue)
{
	length = ai_iValue;
}

m4return_t CComponent::MergeChildIncludeByPos ( m4uint32_t ai_iPosPropBag, CComponent * ai_pNewComponent )
{
	//----------------------------------------
	//Presentacion principal:
	//SEQ
	//	Classname:	Presentation
	//	Alias: ZZ_PEDRO_PRESENTATION
	//	SEQ
	//		Classname:	From
	//		Alias: Form1
	//		SEQ
	//			Classname:	Includecall
	//			Alias: xxxx
	//----------------------------------------

	//----------------------------------------
	//Presentacion include:
	//SEQ
	//	Classname:	Includepanel
	//	Alias: yyyy
	//	SEQ
	//		Classname:	Panel
	//		Alias: PanelId
	//		--- resto de propiedades ---
	//----------------------------------------

	//Cuando se haga el merge debe quedar:

	//----------------------------------------
	//Presentacion definitiva:
	//SEQ
	//	Classname:	Presentation
	//	Alias: ZZ_PEDRO_PRESENTATION
	//	SEQ
	//		Classname:	From
	//		Alias: Form1
	//		SEQ
	//			Classname:	Panel
	//			Alias: PanelId
	//			--- resto de propiedades ---
	//----------------------------------------

	if (propBag != NULL) {
		//Escenario:
		//Vamos a decidir donde hacer el merge.
		//1.- Sustitucion solo del nodo raiz (que es una SEQ) del subarbol, manteniendo el resto
		//de componentes (classname,...)
		return propBag->ReplaceComponentByPos( ai_iPosPropBag, ai_pNewComponent);
	}

	DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
	return M4_ERROR;
}

m4return_t CComponent::ReplaceValueProp( m4char_t * ai_pNewValue )
{
	//El argumento es una cadena NULL terminada.

	m4char_t n_bNew = 0;
	m4char_t n_bOld = 0;
	m4int32_t iSizeNew = 0;
	m4int32_t iSizeOld = GetLengthProp();
	
	//Reemplaza el valor de la componente por otro. Se recalculan tamaños.
	if (type_prop != ASNGeneralString) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return	M4_ERROR;
	}

	if (ai_pNewValue != NULL) {
		iSizeNew = strlen(ai_pNewValue);
	}

	//Ya tenemos la cadena que queremos, ahora necesitamos recalcular el tamaño.
	//Para ello restamos la longitud que tenia y le damos la nueva longitud.
	SetLength(-iSizeOld);

	/* Bug 0107224
	También se ajusta el tamaño de la longitud, restando el viejo y añadiendo el nuevo
	*/
	n_bNew = calculateLength(iSizeNew);
	n_bOld = calculateLength(iSizeOld);
	SetLength(n_bNew-n_bOld);

	//Pasamos una cadena sin el NULL final!. Esto tambien cambia el tamaño de la componente
	//pero NO cambia la longitud del valor.
	SetStringValueProp(ai_pNewValue, iSizeNew);

	//Ahora cambiamos la nueva longitud del valor.
	length_prop = iSizeNew;

	return M4_SUCCESS;
}

m4return_t CComponent::ReplaceValuePropSearchingPattern( const string & ai_sPattern, const string & ai_sNewString )
{
	//Es necesario que sea cadena.
	if (type_prop != ASNGeneralString) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return	M4_ERROR;
	}

	m4uint32_t iLengthBefore = GetLengthProp();	
	m4char_t * pValueBefore = new m4char_t [iLengthBefore + 1];
	GetStringValueProp(pValueBefore);
	pValueBefore[iLengthBefore] = NULL;
	string sValue(pValueBefore);
	delete [] pValueBefore;

	m4int32_t iPos = 0;
	m4int32_t iPosBegin = 0;
	m4int32_t iPosEnd = 0;
	while ((iPosBegin = sValue.find(ai_sPattern, iPos)) != -1)
	{
		iPosEnd = iPosBegin + ai_sPattern.size();
		sValue.replace(iPosBegin, iPosEnd - iPosBegin, ai_sNewString);
	}

	return ReplaceValueProp( (m4char_t *) sValue.c_str() );
}


m4int32_t CComponent::code(ClChannelManager *ai_poChannelManager, CInputStream *ai_pIn, CAsn *ai_pReference, m4bool_t ai_bIsDef, m4uint32_t ai_iReferences, CAsn *ai_paMultiReference, m4pchar_t *ai_ppcLangCodes, CComponent *ai_pParent )
{
		
	// codificar de ASN a estructura de objetos "a.k.a arbolillo". 
	
	m4uint32_t	bytesRead = 0;
	m4uchar_t b_TN, b_T, b_V, b_L, b_LN;
	m4uint32_t lengthString;
	
	m4uint32_t i, iLength = 0;
	m4uint32_t lengthASN = 0;
	m4bool_t acabar = M4_FALSE;
	m4bool_t traduccion;
	m4int32_t length_code;
	m4int_t num_bytes;
		
	
	bytesRead += ai_pIn->readByte(b_TN);

	while( ai_pIn->eof() == 0 )
	{
		if (b_TN == ASNGeneralString || b_TN == ASNUnicodeString)
		{
			bytesRead += ai_pIn->readByte(b_LN);
			num_bytes=0;
			lengthString = 0;

			if (b_LN>128) 
			{
				b_LN -= 128;
				num_bytes=b_LN;
				for (i=b_LN;i>0;i--)
				{
					m4uint32_t byte;
					bytesRead += ai_pIn->readByte(b_V);
					byte=b_V;
					lengthString+=(byte<<8*(i-1));					
				}
			}
			else 
			{
				num_bytes = 1;
				lengthString = b_LN;	
			}

			m4char_t *m_nombre;
			bytesRead += ai_pIn->readString(m_nombre, lengthString, b_TN);

			bytesRead += ai_pIn->readByte(b_T);

			switch (b_T)
			{
			case ASNBOOLEAN:				
				{
				SetTypeNameProp();
				SetLengthNameProp(lengthString);
				SetNameProp(m_nombre, lengthString);
				SetTypeProp(T_BOOL);
				bytesRead += ai_pIn->readByte(b_L);
				SetLengthProp(M4_FALSE, T_BOOL);
				bytesRead += ai_pIn->readByte(b_V);
				SetBoolValueProp(b_V);
				delete []m_nombre;
				return bytesRead;
				}
				
			case ASNINTEGER:
				{
				m4uint32_t mask = 0xFFFFFFFF;
				m4bool_t negative = M4_FALSE;

				SetTypeNameProp();
				SetLengthNameProp(lengthString);
				SetNameProp(m_nombre, lengthString);
				SetTypeProp(T_INT);
				bytesRead += ai_pIn->readByte(b_L);
				
				for (i=b_L;i>0;i--)
				{
					m4uint32_t byte;

					mask <<= 8;
					bytesRead += ai_pIn->readByte(b_V);
					byte=b_V;

					if ((i == b_L) && (byte & 0x80))
					{
						negative = M4_TRUE;
					}
						
					lengthASN+=(byte<<8*(i-1));
				}

				if (negative)
				{
					lengthASN |= mask;
				}

				length_prop = b_L;
				SetLength(1);
				SetIntValueProp(lengthASN);
				lengthASN = 0;
				delete []m_nombre;
				return bytesRead;
				}

			case ASNGeneralString:
			case ASNUnicodeString:
				{
				// añadir las referencias
				SetTypeNameProp();
				SetLengthNameProp(lengthString);
				SetNameProp(m_nombre, lengthString);

				iLength = lengthString; // keep the length of the name to pass to the refSolver

				bytesRead += ai_pIn->readByte(b_L);
				num_bytes = 0; lengthString = 0;
				if(b_L>128)
				{
					b_L -= 128;
					num_bytes=b_L;
					for (i=b_L;i>0;i--)
					{
						bytesRead += ai_pIn->readByte(b_V);
						m4uint32_t byte=b_V;
						lengthString+=(byte<<8*(i-1));
					}
				}
				else 
				{
					num_bytes = 1;
					lengthString = b_L;	
				}
				
				m4char_t *nombre;
				bytesRead += ai_pIn->readString(nombre, lengthString, b_T);

				// Begin Juanma. 
				//	Bug 53253 IMPACTO: S2G:Aparece la referencia como texto en una Label con Value_BRA="X"
				//
				// El modelo de campos traducibles ha cambiado para que la gente de traducciones encuentre
				// siempre los mismos Id.
				//
				// Modelo de traducciones de versión <= Mind 3.2
				//	Text = "R_Ref1"
				//	BEGIN TraductProp R_Ref1

				// Modelo de traducciones de versiones >= Mind 4.0
				//	Text = "_55C0258"
				//	BEGIN TraductProp _55C0258

				traduccion = M4_FALSE;

				/* 					
				La condición para buscar referencias caso de multireference (cuando estamos en tiempo de diseño)
				debe tener en cuenta que los alias no se traducen, aunque empiecen por R o por guión.
				*/

				m4bool_t	bTryTranslate = M4_FALSE ;

				if( ai_bIsDef == M4_FALSE )
				{
					// Dejamos lo de antes
					if	(	lengthString > 0
							&& ( nombre[0] == '_' || nombre[0] == 'R')
							&& ai_pReference != NULL
						)
						// hay una posible referencia, Empieza por _ ó por R
					{
						bTryTranslate = M4_TRUE ;
					}
				}
				else
				{
					if	(	lengthString > 0
							&& ( nombre[0] == '_' || nombre[0] == 'R')
							&& strnicmp( m_nombre, M4PRESENTATION_ALIAS, iLength ) != 0
							&& ai_paMultiReference != NULL
						)
					{
						bTryTranslate = M4_TRUE ;
					}
				}

				if( bTryTranslate == M4_TRUE )
				{
					referenceSolver( iLength, m_nombre, lengthString, nombre, b_T, traduccion, 
						ai_pReference, ai_bIsDef, ai_iReferences, ai_paMultiReference, ai_ppcLangCodes, ai_pParent ) ; 
				} 
				else
				{
					traduccion = M4_FALSE;
					SetTypeProp(T_STRING);
					SetLengthProp(M4_FALSE, T_STRING, 0, lengthString);
					SetStringValueProp(nombre, lengthString);
				}

				delete []nombre;
				delete []m_nombre; 
				
				} // end case
			
				return bytesRead;

			}// endSwitch	
		} //end then
		else // es sequence
		{		
			if (b_TN == ASNSequence)
			{
				SetTypeProp(T_SEQ);
				bytesRead += ai_pIn->readByte(b_L);
				num_bytes=0; lengthASN = 0;
				if(b_L>128)
				{
					b_L -= 128;
					num_bytes=b_L;
					for (i=b_L;i>0;i--)
					{
						bytesRead += ai_pIn->readByte(b_V);
						m4uint32_t byte=b_V;
						lengthASN+=(byte<<8*(i-1));
					}
				}
				else
				{
					lengthASN = b_L;
				}

				i = 0;
 				while( i < lengthASN && !acabar )
				{
					CComponent *comp = new CComponent();					
					length_code = comp->code(ai_poChannelManager, ai_pIn, ai_pReference, ai_bIsDef, ai_iReferences, ai_paMultiReference, ai_ppcLangCodes, this);
					
					/////////////////////////////////////////
					if( length_code > 0 )
					{
						i += length_code;
						bytesRead += length_code;
					}
					else
					{
						acabar = M4_TRUE;
					}

					if (!acabar)
					{
						propBag->Add(comp);
					}
				}
				return bytesRead;
			}	
			else
			{
				return M4_ERROR;
			}

		}// end else
	} // end while  

	return M4_ERROR;
}


m4int32_t CComponent::codeReferences(CInputStream *m_in)
{
	m4uint32_t	bytesRead = 0;
	m4uchar_t b_TN,b_LN,b_T,b_L,b_V;
	m4uint32_t i;
	m4uint32_t lengthASN = 0;
	m4uint32_t lengthString = 0;
	m4bool_t acabar = M4_FALSE;
	m4int32_t length_code;
	m4uchar_t num_bytes;
	
	bytesRead += m_in->readByte(b_TN);

	while( m_in->eof() == 0 )
	{
		if(b_TN == ASNGeneralString || b_TN == ASNUnicodeString)
		{
			bytesRead += m_in->readByte(b_LN);
			num_bytes=0; lengthString = 0;
			if(b_LN>128)
			{
				b_LN -= 128;
				num_bytes=b_LN;
				for (i=b_LN;i>0;i--)
				{
					bytesRead += m_in->readByte(b_V);
					m4uint32_t byte=b_V;
					lengthString+=(byte<<8*(i-1));
				}
			}
			else
			{
				lengthString = b_LN;
			}

			m4char_t *m_nombre;
			bytesRead += m_in->readString(m_nombre, lengthString, b_TN);

			bytesRead += m_in->readByte(b_T);
			switch (b_T)
			{
			case ASNBOOLEAN:
			{	
				SetTypeNameProp();
				SetLengthNameProp(lengthString);
				SetNameProp(m_nombre, lengthString);
				SetTypeProp(T_BOOL);
				bytesRead += m_in->readByte(b_L);
				SetLengthProp(M4_FALSE, T_BOOL);
				bytesRead += m_in->readByte(b_V);
				SetBoolValueProp(b_V);
				delete []m_nombre;
				return bytesRead;
				break;
			}
			case ASNINTEGER:
			{
				m4uint32_t mask = 0xFFFFFFFF;
				m4bool_t negative = M4_FALSE;

				SetTypeNameProp();
				SetLengthNameProp(lengthString);
				SetNameProp(m_nombre, lengthString);
				SetTypeProp(T_INT);
				bytesRead += m_in->readByte(b_L);
				
				for (i=b_L;i>0;i--)
				{
					m4uint32_t byte;

					mask <<= 8;
					bytesRead += m_in->readByte(b_V);
					byte=b_V;

					if ((i == b_L) && (byte & 0x80))
					{
						negative = M4_TRUE;
					}
						
					lengthASN+=(byte<<8*(i-1));
				}

				if (negative)
				{
					lengthASN |= mask;
				}

				length_prop = b_L;
				SetLength(1);
				SetIntValueProp(lengthASN);
				lengthASN = 0;
				delete []m_nombre;
				return bytesRead;
				break;
			}

			case ASNGeneralString:
			case ASNUnicodeString:
			{
				SetTypeNameProp();
				SetLengthNameProp(lengthString);
				SetNameProp(m_nombre, lengthString);
				delete []m_nombre;

				bytesRead += m_in->readByte(b_L);
				num_bytes = 0; lengthString = 0;
				if(b_L>128)
				{
					b_L -= 128;
					num_bytes=b_L;
					for (i=b_L;i>0;i--)
					{
						bytesRead += m_in->readByte(b_V);
						m4uint32_t byte=b_V;
						lengthString+=(byte<<8*(i-1));
					}
				}
				else
				{
					lengthString = b_L;
				}
				
				m4char_t *nombre;
				bytesRead += m_in->readString(nombre, lengthString, b_T);
				SetTypeProp(T_STRING);
				SetLengthProp(M4_FALSE, T_STRING, 0, lengthString);
				SetStringValueProp(nombre, lengthString);
				delete []nombre;
				return bytesRead;
			}
			} // end switch 
		} //end then
		else // es sequence
		{	
			SetTypeProp(T_SEQ);
			bytesRead += m_in->readByte(b_L);
			num_bytes = 0;
			if(b_L>128)
			{
				b_L -= 128;
				num_bytes = b_L;
				for (i=b_L;i>0;i--)
				{
					bytesRead += m_in->readByte(b_V);
					m4uint32_t byte=b_V;
					lengthASN+=(byte<<8*(i-1));
				}
			}
			else
			{
				lengthASN = b_L;
			}

			SetLength(lengthASN);
			length_prop=lengthASN;

			i = 0;
			while( i < lengthASN && !acabar )
			{
				CComponent *comp = new CComponent();
				length_code = comp->codeReferences(m_in);

				if( length_code > 0 )
				{
					i += length_code;
					bytesRead += length_code;
				}
				else
				{
					acabar = M4_TRUE;
				}
				if( !acabar )
				{
					propBag->Add(comp);
				}
			}
			return bytesRead;	
		}// end else
	} // end while

	return M4_ERROR;
}


m4int_t CComponent::FindReference(CComponent *seq,m4char_t *nombre,m4uint32_t *aio_pLength,m4uchar_t *type,TYPE_COMP *c)
{
	// Hay que añadir las traducciones que sean una SEQ
	
	m4uint32_t i=0,j;
	m4bool_t acabar = M4_FALSE;
	m4char_t *item=NULL;
	CComponent *compon = NULL;

	// Esta funcion devuelve:
	//		1 si la traduccion se ha encontrado
	//      0 si no se ha encontrado traduccion
	
	m4char_t name[255 + 1];
	
	if ((seq->GetTypeProp()) == ASNSequence)
	{
		while((i<(seq->propBag)->GetNumComponent()) && (!acabar))
		{	
			j=0;
			compon = (seq->propBag)->View(i);
			if ((compon->GetTypeProp()) != ASNSequence)
			{
				compon->GetNameProp(name);
				while ((j<(*aio_pLength)) && (name[j] == nombre[j]))
				{
					j++;
				}
				//delete []name;
				if ((compon->GetLengthNameProp() == (*aio_pLength)) && (j == (*aio_pLength)))
				{
					acabar = M4_TRUE;
				}
			}
			else
			{
				FindReference(compon,nombre,aio_pLength,type,c);
			}

			i++;
		}

		if (!acabar)
		{
			// no se ha encontrado correspondencia
			return 0;
		}
		else
		{
			// buscar el objeto que se corresponde.
			// Se devuelve el objeto porque asi se permite correspondencia con:
			//		integer, bool, string y sequence
					
			CComponent *find = NULL; 
			m4uchar_t indice = 0;
			find = (seq->propBag)->View(i-1); // COMPONENTE CON LA REFERENCIA
			(*aio_pLength) = find->GetLengthProp();
			m4char_t *value_name = new m4char_t[*aio_pLength + 1];
			
			(*type) = find->GetTypeProp();
			switch(*type)
			{
			 case ASNBOOLEAN:
						c->byte_value = find->GetBoolValueProp();
						break;

			 case ASNINTEGER:
						c->int_value = find->GetIntValueProp();
						break;

			 case ASNGeneralString:
						find->GetStringValueProp(value_name);
						c->char_value = new m4char_t[*aio_pLength + 1];
						memcpy(c->char_value,value_name,*aio_pLength);
						c->char_value[ *aio_pLength ] = '\0' ;
						break;
									
			}
			delete []value_name;
			return 1;
		}
	}
	else
	{
		return 0;
	}
}



m4int_t CComponent::analiceIndirection(m4char_t *org,m4uint32_t *indice,m4uint32_t ai_iLength, m4uint32_t *beginInd)
{
	m4int_t res = -1;

	*beginInd = 0;

	while (((*indice)<ai_iLength) && (org[*indice] != '#'))
	{
		(*indice)+=1;
	}

	if ((*indice) == ai_iLength)
	{
		res = -1;
	}
	else
	{
		(*indice)++;
		if (((*indice) == ai_iLength) || org[*indice] != '#')
		{
			res = -1;
		}
		else // ##
		{
			if ( org[*indice] == '#')
			{
				(*indice)++;
				if (((*indice)+1<ai_iLength) && (org[*indice] == 'T') && (org[*indice+1] == 'M'))
				{
					if (*indice >2)
					{
						*beginInd = *indice - 2;
					}

					(*indice) += 2;
					res = 0;
				}
				else
				{
					if (((*indice)+1<ai_iLength) && (org[*indice] == 'N') && (org[*indice+1] == 'D'))
					{
						if (*indice >2)
						{
							*beginInd = *indice - 2;
						}
						(*indice) += 2;
						res = 1;
					}
					else
					{
						if (((*indice)+4<ai_iLength) && (org[*indice] == 'C') && (org[*indice+1] == 'H') &&
							(org[*indice+2] == 'N') && (org[*indice+3] == 'N') &&
							(org[*indice+4] == 'L'))
						{
							if (*indice >2)
							{
								*beginInd = *indice - 2;
							}
							(*indice) += 5;
							res = 2;
						}
					}
				}
			}
		}
	}
	return res;
}

m4int_t CComponent::FindIndirectionItem(ClChannelManager *ai_poChannelManager, ClCompiledMCR *poCompiled,m4char_t *org,m4char_t *&dst,m4uint32_t *indice,m4uint32_t *b_L,m4char_t *name_node,m4uint32_t ai_iLength)
{
	// en org llega el ID_ITEM y en b_L la longitud
	// La funcion devuelve en dst el nombre del ITEM y en b_L la longitud del nombre
	
	m4uint32_t	m_ItemHandle,m_NodeHandle;
	m4char_t *name = NULL;
	m4char_t *name_aux = NULL;
	
	m4uint32_t i=0;

	(*b_L) = 0;
	
	if (poCompiled!=NULL)
	{
		name = new m4char_t[40 + 1]; // reserva espacio con el nombre de item

		(*indice)++;
		while(((*indice)<ai_iLength) && (i<40) && (org[*indice] != ']'))
		{
			name[i] = org[*indice];
			(*indice)++;
			i++;
		}

		if (((*indice) == ai_iLength) || (i==40))
		{
			if (name!= NULL)
			{
				delete []name;
			}
			return 1;
		}

		name[i] = '\0';

		if (name_node == NULL) 
		{
			if (name!= NULL)
			{
				delete []name;
			}
			return 1;
		}

		m_NodeHandle = poCompiled->GetChannelNodeHandleById(name_node);
		if (m_NodeHandle==0)
		{
			if (name!= NULL)
			{
				delete []name;
			}
			return 1;
		}
		else
		{
			m_ItemHandle = poCompiled->GetNodeItemHandleById(m_NodeHandle,name);
			if (m_ItemHandle==0)
			{
				if (name!= NULL)
				{
					delete []name;
				}
				return 1;
			}
		}

		name_aux = (m4char_t *)poCompiled->GetItemName(m_ItemHandle);
		if (name_aux == NULL)
		{
			if (name!= NULL)
			{
				delete []name;
			}
			return 1;
		}
		else
		{
			if (strcmp(name_aux,"")==0)
			{
				*b_L = strlen(name);
				dst = new m4char_t[*b_L+1];
				memcpy(dst,name,*b_L+1);
			}
			else 
			{
				*b_L = strlen(name_aux);
				dst = new m4char_t[*b_L+1];
				memcpy(dst,name_aux,*b_L+1);
			}
			
			if (name!= NULL)
			{
				delete []name;
			}
			return 0;
		}
	}
	else
	{
		return M4_ERROR;
	}
}


m4int_t CComponent::FindIndirectionNode(ClChannelManager *ai_poChannelManager, ClCompiledMCR *poCompiled,m4char_t *org,m4char_t *&dst,m4uint32_t *indice,m4uint32_t *b_L,m4uint32_t ai_iLength)
{
	// en m_nombre llega el ID_NODE y en b_L la longitud
	// La funcion devuelve en m_nombre el nombre del NODE y en b_L la longitud del nombre
	
	m4uint32_t	m_NodeHandle;
	m4char_t *name = NULL;
	m4char_t *name_aux = NULL;
	m4int_t i=0;

	(*b_L) = 0;

	if (poCompiled!=NULL)
	{
		name = new m4char_t[40 + 1];
		(*indice)++;
		while(((*indice)<ai_iLength) && (i<40) && (org[*indice] != ']'))
		{
			name[i] = org[*indice];
			(*indice)++;
			i++;
		}

		if (((*indice) == ai_iLength) || (i==40))
		{
			if (name!= NULL)
			{
				delete []name;
			}
			return 1;
		}

		name[i] = '\0';

		m_NodeHandle = poCompiled->GetChannelNodeHandleById(name);
		
		if (m_NodeHandle == 0)
		{
			if (name!= NULL)
			{
				delete []name;
			}
			return 1;
		}
		name_aux = (m4char_t *)poCompiled->GetNodeName(m_NodeHandle);
		
		if (name_aux == NULL)
		{
			if (name!= NULL)
			{
				delete []name;
			}
			return 1;
		}
		else
		{
			if (strcmp(name_aux,"")==0)
			{
				*b_L = strlen(name);
				dst = new m4char_t[*b_L+1];
				memcpy(dst,name,*b_L+1);
			}
			else
			{
				*b_L = strlen(name_aux);
				dst = new m4char_t[*b_L+1];
				memcpy(dst,name_aux,*b_L+1);
			}
			
			if (name!= NULL)
			{
				delete []name;
			}
			return 0;
		}
	}
	else
	{
		return M4_ERROR;
	}
}


m4int_t CComponent::FindIndirectionChannel(ClChannelManager *ai_poChannelManager, ClCompiledMCR *poCompiled,m4char_t *org,m4char_t *&dst,m4uint32_t *indice,m4uint32_t *b_L,m4uint32_t ai_iLength, CInternalPresentationFactory *aux, ClPresentationFactory * ai_pPresentationFactory, const ClPresentationAttributes & ai_ClPresentationAttributes)
{
	// en m_nombre llega el ID_CHANNEL y en ai_iLength la longitud
	// La funcion devuelve en m_nombre el nombre del CHANNEL y en ai_iLength la longitud del nombre
	
	m4char_t *name_aux = NULL;
	m4char_t name[40];
	m4uchar_t i = 0;
	m4bool_t insert;
	ClChannel *ptoChannel;
	m4uint32_t cont = 0;
	m4return_t res;

	*b_L = 0;
	(*indice)++;
	insert = M4_TRUE;

	while (((*indice)<ai_iLength) && (i<40) && (org[*indice] != ']'))
	{
		if (insert)
		{
			name[cont++] = org[*indice];
		}

		if (org[*indice] == '[')
		{
			insert = M4_TRUE;
		}
		(*indice)++;
		i++;
	}
	
	insert = M4_FALSE;
	if (((*indice) == ai_iLength) || (i==40))
	{
		return 1;
	}

	name[cont] = '\0';

// jcr para el res
	ptoChannel = aux->FindChannel(name, ai_ClPresentationAttributes, &res);

	if (res == M4_ERROR)
	{
		return 1;
	}
// fin jcr

	if (ptoChannel != NULL)
	{
		poCompiled = (ptoChannel->GetpChannelDef())->GetpCMCR();
	}

	if (poCompiled != NULL)
	{
		name_aux = (m4char_t *)poCompiled->GetChannelName();
	}
	else
	{
		return 1;
	}

	if (name_aux == NULL)
	{
		return 1;
	}
	else
	{
		*b_L = strlen(name_aux);
		dst = new m4char_t[*b_L+1];
		memcpy(dst,name_aux,*b_L+1);
		return 0;
	}
}


void CComponent::AddSegurity()
{	
	// Siempre se entra con una sequence que
	// contiene una presentacion.
	// Añade la propiedad Grant 

	// En esta version todas las SEQ van a tener esta propiedad
	
	m4uint32_t i = 0;
	m4bool_t add = M4_FALSE;

	if ((GetTypeProp() == ASNSequence) && ((m_access->grantsByUser) == M4_FALSE))
	{
		propBag->SetGrant(m_access->grant);
	}

	while (i<propBag->GetNumComponent())
	{
		CComponent *comp;
		
		comp = propBag->View(i);
		if ((comp->GetTypeProp()) == ASNSequence)
		{
			comp->AddSegurity();
		}
		i++;
	}
}


void CComponent::AnalizarSequence( m4bool_t ai_bFirstTime, CBufferASN * ai_pInfPresent )
{
	
	// Siempre se entra con una sequence que
	// contiene una presentacion.
	// Añade a cada sequence la jerarquia channel-node-item
	// Si no aparecen explicitamente las hereda de su padre.
	// Tambien analiza si aparece explicitamente la propiedad Grant
	
	m4uint32_t i = 0;
	m4bool_t findGrant = M4_FALSE;
	m4bool_t firstSeq = M4_TRUE;
	m4bool_t extend = M4_FALSE;
	CComponent *comp;
	while (i<propBag->GetNumComponent())
	{
		comp = propBag->View(i);
		if ((comp->GetTypeProp()) != ASNSequence)
		{
			m4char_t *name = new m4char_t[comp->GetLengthNameProp() + 1];
			m4char_t *prop = new m4char_t[comp->GetLengthProp() + 1];

			comp->GetNameProp(name);

			if ((comp->GetLengthNameProp() == M4PRESENTATION_ID_CHANNEL_SIZE) && (memcmp(name, M4PRESENTATION_ID_CHANNEL, M4PRESENTATION_ID_CHANNEL_SIZE) == 0))
			{
				if (!extend)
				{
					if (m_access->channel) delete [](m_access->channel);
					m_access->channel = new m4char_t[comp->GetLengthProp() + 1];
					comp->GetStringValueProp(prop);
					memcpy(m_access->channel,prop,comp->GetLengthProp());
					m_access->channel[ comp->GetLengthProp() ] = '\0';
					m_access->lengthChannel = comp->GetLengthProp();

					//nuevo para la ListMeta4Obj
					prop[comp->GetLengthProp()] = '\0';
					if (ai_pInfPresent->Exist(prop) == -1)
					{
						// el T3 no esta almacenado
						
						ai_pInfPresent->listT3[ai_pInfPresent->numT3] = new char[strlen(prop)+1];
						strcpy(ai_pInfPresent->listT3[ai_pInfPresent->numT3], prop);
						ai_pInfPresent->listVersionT3[ai_pInfPresent->numT3] = M4CL_MINUS_INFINITE_IN_JULIAN;
						ai_pInfPresent->numT3 ++;
					}
				}
				else
				{
					m_access->lengthChannel = 0;
					m_access->lengthNode = 0;
					m_access->lengthItem = 0;
				}
			}
			else 
				if ((comp->GetLengthNameProp() == M4PRESENTATION_ID_NODE_SIZE) && (memcmp(name, M4PRESENTATION_ID_NODE, M4PRESENTATION_ID_NODE_SIZE) == 0))
				{
					if (!extend)
					{
						if (m_access->node) delete [](m_access->node);
						m_access->node = new m4char_t[comp->GetLengthProp() + 1];
						comp->GetStringValueProp(prop);
						memcpy(m_access->node,prop,comp->GetLengthProp());
						m_access->node[ comp->GetLengthProp() ] = '\0';
						m_access->lengthNode = comp->GetLengthProp();
					}
					else
					{
						m_access->lengthChannel = 0;
						m_access->lengthNode = 0;
						m_access->lengthItem = 0;
					}
				}
				else
					if ((comp->GetLengthNameProp() == M4PRESENTATION_ID_ITEM_SIZE) && (memcmp(name, M4PRESENTATION_ID_ITEM, M4PRESENTATION_ID_ITEM_SIZE) == 0))
					{
						if (!extend)
						{
							if (m_access->item) delete [](m_access->item);
							m_access->item = new m4char_t[comp->GetLengthProp() + 1];
							comp->GetStringValueProp(prop);
							memcpy(m_access->item,prop,comp->GetLengthProp());
							m_access->item[ comp->GetLengthProp() ] = '\0';
							m_access->lengthItem = comp->GetLengthProp();
						}
						else
						{
							m_access->lengthChannel = 0;
							m_access->lengthNode = 0;
							m_access->lengthItem = 0;
						}

					}
					else
						if ((comp->GetLengthNameProp() == M4PRESENTATION_GRANTS_SIZE) && (memcmp(name, M4PRESENTATION_GRANTS, M4PRESENTATION_GRANTS_SIZE) == 0))
						{
							m_access->grant = comp->GetIntValueProp();
							m_access->grantsByUser = M4_TRUE;
							findGrant = M4_TRUE;
						}
						else
						{
							if (comp->GetTypeProp() == ASNGeneralString)
							{
								comp->GetStringValueProp(prop);
								if ((comp->GetLengthProp() == M4PRESENTATION_EXTENDS_SIZE) && (memcmp(prop, M4PRESENTATION_EXTENDS, M4PRESENTATION_EXTENDS_SIZE) == 0))
								{
									extend = M4_TRUE;
								}

							}
						}
			if (prop != NULL) delete []prop;
			if (name != NULL) delete []name;
			
		}
		else
		{
			if ((m_access->lengthChannel) > 0)
			{
				if ((comp->m_access)->channel != NULL)
				{
					delete []((comp->m_access)->channel);
				}

				(comp->m_access)->channel = new m4char_t[m_access->lengthChannel + 1];
				(comp->m_access)->lengthChannel = m_access->lengthChannel;
				memcpy((comp->m_access)->channel,m_access->channel,m_access->lengthChannel);
				(comp->m_access)->channel[ m_access->lengthChannel ] = '\0';
			}
			if ((m_access->lengthNode) > 0)
			{
				if ((comp->m_access)->node!= NULL)
				{
					delete []((comp->m_access)->node);
				}

				(comp->m_access)->node = new m4char_t[m_access->lengthNode + 1];
				(comp->m_access)->lengthNode = m_access->lengthNode;
				memcpy((comp->m_access)->node,m_access->node,m_access->lengthNode);
				(comp->m_access)->node[ m_access->lengthNode ] = '\0';
			}

			if ((m_access->lengthItem) > 0)
			{
				if ((comp->m_access)->item != NULL)
				{
					delete []((comp->m_access)->item);
				}

				(comp->m_access)->item = new m4char_t[m_access->lengthItem + 1];
				(comp->m_access)->lengthItem = m_access->lengthItem;
				memcpy((comp->m_access)->item,m_access->item,m_access->lengthItem);
				(comp->m_access)->item[ m_access->lengthItem ] = '\0';
			}

			if ((ai_bFirstTime)&&(!findGrant)&&(firstSeq))
			{
				firstSeq = M4_FALSE;
				m_access->grant = 31;
				(comp->m_access)->grant = 31;
			}
			else
			{
				(comp->m_access)->grant = m_access->grant;	
			}
			comp->AnalizarSequence(M4_FALSE, ai_pInfPresent);	
		}
        i++;

	}// fWhile
}

//-----------------------------------------------------------------------------------------------
//----------------------------- SOLO DEPURACION -------------------------------------------------
void CComponent::DumpInfoRoot( m4bool_t ai_bTranslateToObl, m4pcchar_t ai_pTitle, m4int32_t ai_iLevel )
{
	static FILE * pFile = NULL;

	if (pFile == NULL)
	{
		// UNICODE FILE
		eUniFileEncodingType_t eEncoding = M4UniNative ;

		pFile = M4Fopen("C:\\TEMP\\trace_present.txt", M4UniWrite, eEncoding);
	}

	//Vamos a volcar el arbol pq esto es un follon!.
	m4char_t * pName = NULL;
	m4char_t * pValue = NULL;

	string sTab("");
	for (m4int32_t i = 0; i<= ai_iLevel; i++) {
		if (ai_bTranslateToObl == M4_TRUE) {
			//Su tabulador es de 4 blancos.
			sTab.append("    ");
		}
		else {
			sTab.append("\t");
		}
	}
	
	if (ai_iLevel == 0)  {
		fprintf(pFile, "**** New Presentation");
		if (ai_pTitle != NULL) {
			fprintf(pFile, ": %s ****\n", ai_pTitle);
		}
		else {
			fprintf(pFile, "****\n");
		}		
	}
	
	if (type_prop == ASNSequence) {
		//Si es secuencia.
		if (ai_bTranslateToObl == M4_TRUE) {
			fprintf(pFile, "%sBEGIN", sTab.c_str());			
		}
		else {
			fprintf(pFile, "%sSEQ\n", sTab.c_str());			
		}
		
		if (propBag != NULL) {
			CComponent * pView = NULL;
			m4uint32_t iNumComponent = propBag->GetNumComponent();
			for (m4uint32_t i=0; i<iNumComponent; i++) {
				pView = propBag->View(i);
				pView->DumpInfoRoot ( ai_bTranslateToObl, NULL, ai_iLevel + 1);
			}
		}

		if (ai_bTranslateToObl == M4_TRUE) {
			fprintf(pFile, "%sEND\n", sTab.c_str());			
		}
	}

	if ((type_prop == ASNGeneralString) 
		|| (type_prop == ASNINTEGER) 
		|| (type_prop == ASNBOOLEAN))
	{
		if (nameProp != NULL) {
			pName = new m4char_t [length_nameProp + 1];
			pName[length_nameProp] = NULL;
			memcpy(pName, nameProp, length_nameProp);
		}
		else {
			pName = new m4char_t [strlen("NULL_NAME") + 1];		
			pName[strlen("NULL_NAME")] = NULL;
			memcpy(pName, "NULL_NAME", strlen("NULL_NAME"));
		}
		
		switch (type_prop)
		{		
		case ASNGeneralString:
			pValue = new m4char_t [length_prop + 1];
			pValue[length_prop] = NULL;
			memcpy(pValue, value_prop.char_value, length_prop);

			if (ai_bTranslateToObl == M4_TRUE) 
			{
				if ((strcmp(pName, M4PRESENTATION_CLASSNAME) == 0) 
				|| (strcmp(pName, M4PRESENTATION_ALIAS) == 0) )
				{
					if (strcmp(pName, M4PRESENTATION_CLASSNAME) == 0) {
						//Ojo pq el obl final no entiende de include panel, por lo tanto, habría que sustituir
						//Includepanel por Panel para que el obl puediera pegarse en el editor de obls!.
						//Tambien tendríamos que eliminar la parte de Includeevent y dejar solo su contenido.
						fprintf(pFile, " %s " , pValue);
					}
					else {
						fprintf(pFile, "%s\n" , pValue);
					}
				}
				else {
					fprintf(pFile, "%s%s=\"%s\"\n" ,sTab.c_str(), pName, pValue);	
				}
			}
			else {
				fprintf(pFile, "%snameProp:\t%s\tvalue_prop:\t%s\n" ,sTab.c_str(), pName, pValue);	
			}
			
			delete [] pValue;
			break;

		case ASNINTEGER:
			if (ai_bTranslateToObl == M4_TRUE) {
				fprintf(pFile, "%s%s=%d\n" ,sTab.c_str(), pName, value_prop.int_value);	
			}
			else {
				fprintf(pFile, "%snameProp:\t%s\tvalue_prop:\t%d\n" ,sTab.c_str(), pName, value_prop.int_value);
			}
			break;

		case ASNBOOLEAN:
			if (ai_bTranslateToObl == M4_TRUE) {
				fprintf(pFile, "%s%s=%s\n" ,sTab.c_str(), pName, (value_prop.byte_value != 0) ? "True" : "False");
			}
			else {
				fprintf(pFile, "%snameProp:\t%s\tvalue_prop:\t%s\n" ,sTab.c_str(), pName, (value_prop.byte_value != 0) ? "True" : "False");
			}
			break;
		}

		delete [] pName;	
	}

	fflush(pFile);
}

//------------------ Volcado de memoria -----------------------------------------------------------
	#ifdef _M4PRESENTATION_DEBUG_DUMP_MEMORY_INFO_
		#include "malloc.h"
		void CComponent::DumpMemoryInfo( m4pcchar_t ai_pIdPresentation, m4bool_t ai_bStartToCount )
		{
			//Nos da la cantidad de memoria que queda entre el inicio y fin de la llamada.
			class ClMemDelta{
			public:
				m4int32_t m_iBefore;		
				ClMemDelta ( void ) { m_iBefore = 0; }
			};
			static ClMemDelta oMemDelta;

			static FILE * pFile = NULL;

			if (pFile == NULL) {
				// UNICODE FILE
				eUniFileEncodingType_t eEncoding = M4UniNative ;

				pFile = M4Fopen("C:\\TEMP\\trace_present_memory.txt", M4UniWrite, eEncoding);
			}

			fprintf(pFile, "Presentacion: %s (%s)\n", ai_pIdPresentation, (ai_bStartToCount == M4_TRUE) ? "ANTES" : "DESPUES");

			//Memoria usada.
			m4int32_t iUsedMemory = 0;

		   _HEAPINFO hinfo;
		   int heapstatus;
		   hinfo._pentry = NULL;
		   while( ( heapstatus = _heapwalk( &hinfo ) ) == _HEAPOK )
		   { 
			   if (hinfo._useflag == _USEDENTRY) {				   
				   iUsedMemory += hinfo._size;
			   }
		   }

		   if (ai_bStartToCount == M4_TRUE) {
			   oMemDelta.m_iBefore = iUsedMemory;
		   }
		   else {
			   //Mostramos la resta!.
			   fprintf(pFile, "\tMemoria usada: %d\n", iUsedMemory - oMemDelta.m_iBefore);
		   }
   
		   fflush(pFile);
		}
	#else
		void CComponent::DumpMemoryInfo( m4pcchar_t ai_pIdPresentation, m4bool_t ai_bStartToCount )
		{}
	#endif
//------------------ FIN Volcado de memoria -----------------------------------------------------------

//----------------------------- FIN SOLO DEPURACION ---------------------------------------------
//-----------------------------------------------------------------------------------------------

//--------------------------------------------
//Punto de entrada.
//--------------------------------------------
m4return_t CComponent::MergeInheritPresentationRoot( CComponent * & aio_pCComponentContainerParent )
{
	m4return_t iRet = M4_SUCCESS;

	//El this y el otro son dos secuencias por narices.
	if (aio_pCComponentContainerParent == NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	if ((type_prop != ASNSequence) 
		|| (type_prop != aio_pCComponentContainerParent->GetTypeProp()) ) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//***** if (aio_pCComponentContainerParent != NULL) {
	//***** 	DumpInfoRoot(M4_TRUE, "HIJA: --- Inicio ---", 0);
	//*****		aio_pCComponentContainerParent->DumpInfoRoot(M4_TRUE, "PADRE: --- Inicio ---", 0);
	//***** }

	//Empieza el lio, van a fusionarse dos pedazo arbolillos.
	iRet = _MergeInheritPresentation( aio_pCComponentContainerParent );

	//***** DumpInfoRoot(M4_TRUE, "HIJA: --- Fin ---", 0);

	return iRet;
}

//----------------------------------------------------------------------------------------
//Control opcional:

//Escenario:
//Si una sequencia no está en el padre la herencia no analiza el subarbol, por lo tanto es posible
//pq VB no lo controle, que haya Deletedobject. Nosotros no trataríamos el subarbol y estariamos devolviendo un
//arbolillo donde hay Deletedobject y también podría haber ##DEFAULT_VALUE##.
//En principio, cuando nosotros devolvemos el merge a VB ellos deberían eliminar o no considerar
//estos objetos, pero porsiaca, vamos a tratar este tema.
//Estas funciones recorren una presentacion, buscan objetos a borrar y los eliminan.
//----------------------------------------------------------------------------------------
m4return_t CComponent::AnalizeChildToDeleteObjectNotExistsInParentRoot( void )
{
	return _AnalizeChildToDeleteObjectNotExistsInParent();
}

m4return_t CComponent::_AnalizeChildToDeleteObjectNotExistsInParent( void )
{
	//Este paso se realiza cuando ya tengo la presentacion hija resultado.
	//Recursivamente se buscarán objetos que sobran.

	m4return_t iRet = M4_SUCCESS;

	if (type_prop != ASNSequence) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4int32_t i=(iNumComponent-1); i>=0; i--) {
		CComponent * pView = NULL;
		m4bool_t bIsComponentToDelete = M4_FALSE;
		m4pchar_t pIdComponentToDelete = NULL;

		pView = propBag->View(i);
		if (pView->GetTypeProp() == ASNSequence) {
			//Comprobemos si es una sequencia de tipo Deletedobject.
			iRet = pView->_IsComponentToDelete( bIsComponentToDelete, pIdComponentToDelete );
			if (pIdComponentToDelete != NULL) {
				//De momento no lo utilizo.
				delete [] pIdComponentToDelete;
				pIdComponentToDelete = NULL;
			}
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}
			if (bIsComponentToDelete == M4_FALSE) {
				//Si no hay que borrarla puede ser que haya alguna subsequencia que si tenga que borrarse,
				//necesitamos recursion!!!.
				iRet = pView->_AnalizeChildToDeleteObjectNotExistsInParent();
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
			}
		}
		else {
			//Comprobamos si el atributo debería eliminarse.
			iRet = pView->_IsComponentToDelete( bIsComponentToDelete, pIdComponentToDelete );
			if (pIdComponentToDelete != NULL) {
				//De momento no lo utilizo.
				delete [] pIdComponentToDelete;
				pIdComponentToDelete = NULL;
			}
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}
		}

		//Si hay que borrarla y liberarla pues se hace y ya está!.
		if (bIsComponentToDelete == M4_TRUE) {
			iRet = _RemoveComponentInListByPos(i, M4_TRUE);
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}
		}
	}
	return iRet;
}
//FIN Control opcional:
//----------------------------------------------------------------------------------------

//--------------------------------------------
//Se invocará en la recursion.
//--------------------------------------------
m4return_t CComponent::_MergeInheritPresentation( CComponent * & aio_pCComponentContainerParent )
{
	//-------------------------------------------------------------------
	//Se distingue si estamos en una componente sequencia o en un atributo.
	//-------------------------------------------------------------------

	m4return_t iRet = M4_SUCCESS;

	if (aio_pCComponentContainerParent == NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//En funcion del tipo se hara de una forma u otra.
	if (type_prop == ASNSequence) {
		iRet = _MergeChildSequenceAndParentSequence( aio_pCComponentContainerParent );
		if (iRet != M4_ERROR) {
			//Si se hace el merge podemos eliminar la componente padre.
			if (aio_pCComponentContainerParent != NULL) {
				delete aio_pCComponentContainerParent;
				aio_pCComponentContainerParent = NULL;
			}
		}
	}
	else {
		// Por aqui no he visto que se entre 
		m4bool_t bDummy = M4_FALSE; 
		iRet = _MergeChildAttributeAndParentSequence( aio_pCComponentContainerParent , bDummy);
	}
	
	return iRet;
}


//-------------------------------------------------------------
//Llamada para el tratamiento de atributos.
//-------------------------------------------------------------
m4return_t CComponent::_MergeChildAttributeAndParentSequence( CComponent * ai_pCComponentContainerParent , m4bool_t & ao_bIsOverwritten )
{
	//El algoritmo de mezcla a este nivel es:
	//Si el valor del atributo del hijo es ##DEFAULT_VALUE## significa que queremos eliminar la propiedad del padre.
	//Si el valor del atributo del hijo es Valor_xx nos quedaremos con la propiedad del hijo.
	//Si un atributo NO esta en el hijo, el del padre se queda, habrá que transferirlo al hijo.

	//Por lo tanto:
	//Toda propiedad que esté en el hijo se elimina del padre!, la diferencia está en si tambien tiene que
	//eliminarse del hijo o se deja!.

	//El argumento ai_pCComponentContainerParent siempre es una secuencia.

	m4return_t iRet = M4_SUCCESS;

	ao_bIsOverwritten = M4_FALSE;

	//El this NUNCA será una sequencia pq es un atributo que buscaremos en el padre.
	if (type_prop == ASNSequence) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	if ((ai_pCComponentContainerParent == NULL) 
		|| ( ai_pCComponentContainerParent->propBag == NULL)
		|| ( ai_pCComponentContainerParent->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	m4int32_t iAttributePositionInParent = -1;

	//Buscamos la propiedad en la presentacion padre!.
	if (_ExistsAttributeInContainer(ai_pCComponentContainerParent, iAttributePositionInParent) == M4_ERROR) {
		return M4_ERROR;
	}

	//Segun algoritmo de mezcla, pero ojo, por ahora solo tenemos en cuenta cuando propiedad SI está en hijo.
	//Despues trataremos el caso de transferir propiedades del arbol padre al arbol hijo (pq el arbol hijo
	//es el que al final tendrá el resultado).
	//Importante!!:
	//En este caso no importa si hay que sobreescribir, pq la propiedad SI está en el hijo, y en herencia
	//el hijo siempre tiene prioridad (sobreescribe la propiedad del padre).
	
	//Si el atributo está en el padre, tengo que eliminarlo y liberarlo del padre pq nos quedaremos con la del hijo!.
	if (iAttributePositionInParent != -1) {	

		ao_bIsOverwritten = M4_TRUE; 

		iRet = ai_pCComponentContainerParent->_RemoveComponentInListByPos( iAttributePositionInParent, M4_TRUE );
		if (iRet == M4_ERROR) {
			return M4_ERROR;
		}
	}




	return iRet;
}

//-------------------------------------------------------------
//FIN Llamada para el tratamiento de atributos.
//-------------------------------------------------------------

//-------------------------------------------------------------
//Llamadas para el tratamiento de sequencias.
//-------------------------------------------------------------
m4return_t CComponent::_MergeChildSequenceAndParentSequence( CComponent * ai_pCComponentContainerParent )
{
	m4return_t iRet = M4_SUCCESS;
	
	//-------------------------------------------------------------------------------------------------
	//Comprobacion previa:
	//La entrada aqui debe garantizar que los roots son del mismo tipo y alias, sino vamos a considerar
	//que estamos mezclando peras con manzanas. Podríamos eliminar la comprobacion pero así evitamos que
	//puedan mezclar pe un include con una presentacion.	
	m4bool_t bIsSameObject = M4_FALSE;
	iRet = _CheckIsSameObject( ai_pCComponentContainerParent, bIsSameObject );
	if (iRet == M4_ERROR){
		return M4_ERROR;
	}
	if (bIsSameObject == M4_FALSE) {
		//Damos un error funcional.
		m4pchar_t pClassNameChild = NULL;
		m4pchar_t pAliasChild = NULL;
		m4pchar_t pClassNameParent = NULL;
		m4pchar_t pAliasParent = NULL;

		//Me da lo mismo el retorno.
		iRet = GetClassNameAndAlias( pClassNameChild, pAliasChild );
		iRet = ai_pCComponentContainerParent->GetClassNameAndAlias( pClassNameParent, pAliasParent );

		if ((pClassNameChild != NULL) 			
			&& (pClassNameParent != NULL)			
			&& (pAliasChild != NULL)
			&& (pAliasParent != NULL) ) 
		{
			DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_MERGE_DIFFERENT_OBJECTS, pClassNameChild << pAliasChild << pClassNameParent << pAliasParent ) ;
		}
		else {
			DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		}

		if (pClassNameChild != NULL) delete [] pClassNameChild;
		if (pAliasChild != NULL) delete [] pAliasChild;
		if (pClassNameParent != NULL) delete [] pClassNameParent;
		if (pAliasParent != NULL) delete [] pAliasParent;

		return M4_ERROR;
	}
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	//Paso1:
	//Dado un atributo del hijo se busca si existe en el padre. Si está en el padre lo tenemos
	//que liberar y eliminar del padre.
	iRet = _AnalizeChildAttributes( ai_pCComponentContainerParent );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	//Paso2:
	//Tratamiento de atributos y secuencias de borrado que hay que eliminar en la presentacion hija.
	//Tambien tendrán que borrandose de la padre.
	iRet = _AnalizeChildToDeleteObjects( ai_pCComponentContainerParent );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	//Paso3:
	//Tratamiento de todo lo que está en el padre y NO está en el hijo.	
	//No hay que liberar, sólo transferir.
	//Este paso tambien tiene en cuenta el posicionamiento.
	iRet = _AnalizeTransferComponentFromParentSequenceAndNotExistsInChild( ai_pCComponentContainerParent );		
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}
	//-------------------------------------------------------------------------------------------------

	//***** DumpInfoRoot(M4_TRUE, "HIJA: Resultado", 0);
	//***** ai_pCComponentContainerParent->DumpInfoRoot(M4_TRUE, "PADRE: Resultado", 0);

	//-------------------------------------------------------------------------------------------------
	//Paso4:
	//Tratamiento de sequencias en el hijo. Porque una sequencia puede contener subsequencias.
	//Aqui va a haber recursion.	
	iRet = _AnalizeChildSequences( ai_pCComponentContainerParent );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}
	//-------------------------------------------------------------------------------------------------

	//Hemos terminado con la sequencia.
	//Del padre sólo ha debido quedar la sequencia, el classname y el alias. El resto o se ha pasado al hijo
	//o se ha eliminado!. Pero por si acaso se eliminará más tarde.

	return iRet;
}

m4return_t CComponent::_CheckIsSameObject( CComponent * ai_pCComponentContainerParent, m4bool_t & ao_bIsSameObject )
{	
	m4return_t iRet = M4_SUCCESS;

	ao_bIsSameObject = M4_FALSE;

	//Tenemos que ser secuencia.
	if ((propBag == NULL) 
		|| (type_prop != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//El padre tambien tiene que ser secuencia.
	if ((ai_pCComponentContainerParent == NULL) 
		|| ( ai_pCComponentContainerParent->propBag == NULL)
		|| ( ai_pCComponentContainerParent->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//-------------------------------------------------------------------------------------------------
	m4pchar_t pClassNameChild = NULL;
	m4pchar_t pAliasChild = NULL;

	iRet = GetClassNameAndAlias( pClassNameChild, pAliasChild );
	if (iRet == M4_ERROR) {
		if (pClassNameChild != NULL) {
			delete pClassNameChild;
		}
		if (pAliasChild != NULL) {
			delete pAliasChild;
		}
		return M4_ERROR;
	}

	//Ahora obtenemos el del padre.
	m4pchar_t pClassNameParent = NULL;
	m4pchar_t pAliasParent = NULL;

	iRet = ai_pCComponentContainerParent->GetClassNameAndAlias( pClassNameParent, pAliasParent );
	if (iRet != M4_ERROR) {
		if ((pClassNameChild != NULL) 
			&& (pClassNameParent != NULL)
			&& (pAliasChild != NULL)
			&& (pAliasParent != NULL) ) 
		{
			//-----------------------------------------------------------
			//Se han encontrado, vamos a comparar!.
			if ((strcmp(pClassNameChild, pClassNameParent) == 0) 
				&& (strcmp(pAliasChild, pAliasParent) == 0) )
			{
				ao_bIsSameObject = M4_TRUE;
			}			
			//-----------------------------------------------------------
		}
		else {
			DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
			iRet = M4_ERROR;
		}
	}

	//Ya no se necesitan.
	if (pClassNameChild != NULL) {
		delete pClassNameChild;
	}
	if (pAliasChild != NULL) {
		delete pAliasChild;
	}
	if (pClassNameParent != NULL) {
		delete pClassNameParent;
	}
	if (pAliasParent != NULL) {
		delete pAliasParent;
	}

	return iRet;
}

m4return_t CComponent::_AnalizeChildAttributes( CComponent * ai_pCComponentContainerParent )
{
	m4return_t iRet = M4_SUCCESS;

	//Tenemos que ser secuencia.
	if ((propBag == NULL) 
		|| (type_prop != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//El padre tambien tiene que ser secuencia.
	if ((ai_pCComponentContainerParent == NULL) 
		|| ( ai_pCComponentContainerParent->propBag == NULL)
		|| ( ai_pCComponentContainerParent->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//-------------------------------------------------------------------------------------------------
	//Tratamiento de atributos.
	m4bool_t bIsSequenceOverwritten = M4_FALSE; 

	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4int32_t i=0; i<iNumComponent; i++) {
		CComponent * pView = propBag->View(i);

		//Dada una componente hija, necesitamos saber con qué componente padre puede haber matching.
		//Le pasamos el contenedor padre y buscaremos la componente e indice del padre que hace matching con
		//la componente hija.

		//Tratamos atributos que no sean subarboles.
		if (pView->GetTypeProp() != ASNSequence) {
			//Veamos si coinciden con classname y alias.
			if ((pView->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE)
				|| (pView->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) )
			{					
				// Se asume coherencia en arboles, estamos intentando hacer merge de dos arboles del mismo tipo.
				// Mismo classname y mismo alias..

				continue;
			}
			else {
				// Estamos en un atributo normal y corriente.				
				m4bool_t bIsAttributeOverwritten = M4_FALSE; 
				iRet = pView->_MergeChildAttributeAndParentSequence( ai_pCComponentContainerParent, bIsAttributeOverwritten );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				// Si al menos uno de los atributos está sobreescrito, entonces la secuencia lo está. 
				if (bIsAttributeOverwritten)
				{
					bIsSequenceOverwritten = M4_TRUE;
				}
			}
		}
	}
			
	// Para el tema de la mezcla en tiempo de diseño, se marca el objeto "hijo" como sobreescrito. 
	if (bIsSequenceOverwritten)
	{
		SetInheritStatus(M4PRESENTATION_INHERITSTATUS_VALUE_OVERWRITTEN);
	}
	else
	{
		// Si el hijo no está sobreescrito, es que está heredado 
		SetInheritStatus(M4PRESENTATION_INHERITSTATUS_VALUE_INHERITED);
	}
	
	
	//FIN Tratamiento de atributos.
	//-------------------------------------------------------------------------------------------------

	return iRet;
}

m4return_t CComponent::_AnalizeChildSequences( CComponent * ai_pCComponentContainerParent )
{
	m4return_t iRet = M4_SUCCESS;

	//Tenemos que ser secuencia.
	if ((propBag == NULL) 
		|| (type_prop != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//El padre tambien tiene que ser secuencia.
	if ((ai_pCComponentContainerParent == NULL) 
		|| ( ai_pCComponentContainerParent->propBag == NULL)
		|| ( ai_pCComponentContainerParent->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//-------------------------------------------------------------------------------------------------
	//Tratamiento de sequencias. dentro de la propia secuencia.
	//Aqui por narices habrá recursion.
	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4int32_t i=0; i<iNumComponent; i++) {
		CComponent * pView = propBag->View(i);

		if (pView->GetTypeProp() == ASNSequence) {
			//Es una secuencia, vamos a buscar esa secuencia en el arbol del padre.
			m4int32_t iSequencePositionInParent = -1;
			iRet = pView->_ExistsSequenceInContainer(ai_pCComponentContainerParent, iSequencePositionInParent);
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}

			//-------------------------------------------
			if (iSequencePositionInParent != -1) {
				//Hemos encontrado una secuencia en la presentacion padre que coincide con la sequencia
				//de la presentación hija. Tenemos que lanzar recursion.
				CComponent * pCComponentParentNextSeq = ai_pCComponentContainerParent->propBag->View(iSequencePositionInParent);

				iRet = pView->_MergeInheritPresentation( pCComponentParentNextSeq );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				//Cuando volvemos de la recursion el pCComponentParentNextSeq ha debido de eliminarse!!.
				if (pCComponentParentNextSeq != NULL) {
					//No se pudo hacer merge.
					DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
					return M4_ERROR;
				}
				iRet = ai_pCComponentContainerParent->_RemoveComponentInListByPos( iSequencePositionInParent, M4_FALSE );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
			}
			else {
				//No está en el padre por lo tanto no tocamos la sequencia del hijo y seguimos recorriendo el hijo.
				continue;
			}
			//-------------------------------------------
		}
	}
	//FIN Tratamiento de sequencias. dentro de la propia secuencia.
	//-------------------------------------------------------------------------------------------------

	return iRet;
}

m4return_t CComponent::_AnalizeChildToDeleteObjects( CComponent * ai_pCComponentContainerParent )
{
	m4return_t iRet = M4_SUCCESS;

	//Tenemos que ser secuencia.
	if ((propBag == NULL) 
		|| (type_prop != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//El padre tambien tiene que ser secuencia.
	if ((ai_pCComponentContainerParent == NULL) 
		|| ( ai_pCComponentContainerParent->propBag == NULL)
		|| ( ai_pCComponentContainerParent->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//-------------------------------------------------------------------------------------------------
	//Tratamiento de sequencias de borrado (Deltedobject) del hijo.
	//Si el hijo tiene un Deletedobject:
	//IMPORTANTE: Es necesario que el objeto se encuentre en la presentacion padre pq la padre es el merge
	//de toda la ascendencia. Por lo tanto se eliminara y liberará del padre y la sequencia de Deletedobject
	//se eliminara y liberara del hijo.
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	//Tratamiento de atributos y secuencias de borrado que hay que eliminar en la presentacion hija.
	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4int32_t i=(iNumComponent-1); i>=0; i--) {
		CComponent * pView = propBag->View(i);

		m4bool_t bIsComponentToDelete = M4_FALSE;
		m4pchar_t pIdComponentToDelete = NULL;
		iRet = pView->_IsComponentToDelete( bIsComponentToDelete, pIdComponentToDelete );
		if (pIdComponentToDelete != NULL) {
			//De momento no lo utilizo.
			delete [] pIdComponentToDelete;
			pIdComponentToDelete = NULL;
		}
		if (iRet == M4_ERROR) {
			return M4_ERROR;
		}
		if (bIsComponentToDelete == M4_TRUE) {
			//Eliminamos y liberamos del padre.
			m4int32_t iAttributePositionInParent = -1;
			if (pView->GetTypeProp() != ASNSequence) {
				iRet = pView->_ExistsAttributeInContainer( ai_pCComponentContainerParent, iAttributePositionInParent );
			}
			else {
				iRet = pView->_ExistsSequenceInContainer( ai_pCComponentContainerParent, iAttributePositionInParent );
			}
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}

			//Si en el padre existe hay que elimiarlo y liberarlo del padre.
			if (iAttributePositionInParent != -1) {
				iRet = ai_pCComponentContainerParent->_RemoveComponentInListByPos( iAttributePositionInParent, M4_TRUE );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
			}

			//Eliminamos y libreramos del hijo.
			iRet = _RemoveComponentInListByPos(i, M4_TRUE);
			if (iRet == M4_ERROR) {
				return M4_ERROR;
			}
		}
	}
	//FIN Tratamiento de atributos que hay que eliminar.
	//-------------------------------------------------------------------------------------------------

	return iRet;
}

m4return_t CComponent::_AnalizeTransferComponentFromParentSequenceAndNotExistsInChild( CComponent * ai_pCComponentContainerParent )
{
	m4return_t iRet = M4_SUCCESS;

	//Tenemos que ser secuencia.
	if ((propBag == NULL) 
		|| (type_prop != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//El padre tambien tiene que ser secuencia.
	if ((ai_pCComponentContainerParent == NULL) 
		|| ( ai_pCComponentContainerParent->propBag == NULL)
		|| ( ai_pCComponentContainerParent->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//-------------------------------------------------------------------------------------------------
	//Transferencia de objetos de la presentacion padre y que NO están en la hija.
	//No hay que liberar, sólo transferir.
	//Objetivo: Transferir componentes del padre y que no están en el hijo. Respetando el orden del padre!.
	//El resultado se dejará en la sequencia hijo.

	VcCComponent vLimbo;

	VcCComponent vMergeParentChild;
	m4int32_t i = 0;
	
	//-------------------------------------------------------------------------------------------------
	//Paso1:
	//Pasamos componentes: Las del padre que NO están en el hijo y las del hijo que SI están en el padre.
	CListComponent * propBagContainerParent = ai_pCComponentContainerParent->propBag;
	m4int32_t iNumComponentParent = propBagContainerParent->GetNumComponent();	
	for (i=0; i<iNumComponentParent; i++) {
		CComponent * pViewParent = propBagContainerParent->View(i);

		m4int32_t iAttributePositionInChild = -1;

		//-----------------------------------------------------------------------------
		//Buscamos si existe la componente en el hijo.
		if (pViewParent->GetTypeProp() != ASNSequence) {
			//Buscamos si el atributo del padre está en el hijo!!.
			iRet = pViewParent->_ExistsAttributeInContainer( this, iAttributePositionInChild );			
		}
		else {
			//Es una secuencia.
			//Buscamos si la sequencia del padre está en el hijo!!.
			iRet = pViewParent->_ExistsSequenceInContainer( this, iAttributePositionInChild );
		}
		if (iRet == M4_ERROR) {
			return M4_ERROR;
		}
		//-----------------------------------------------------------------------------

		if (iAttributePositionInChild == -1) {			
			//La componente no está en la hija: Me quedo con la del padre.
			vMergeParentChild.push_back(pViewParent);
		}
		else {
			//La componente está en el hijo: Me quedo con la del hijo.
			CComponent * pViewChild = propBag->View(iAttributePositionInChild);

			//Solo vale para sequencias!.
			if (pViewChild->GetTypeProp() == ASNSequence) {
				//Pero antes vamos a hacer una comprobación, vemos si la componente hijo especifica una Positionafter.
				//Significa que un hijo le pone una Positionafter de una componente que todavía no se ha tratado.				
				m4bool_t bHasPositionafter = M4_FALSE;
				m4int32_t iPositionFound = -1;
				iRet = pViewChild->FindPositionInVectorSearchingByIdSequence( &vMergeParentChild, bHasPositionafter, iPositionFound );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				//---------------------------------
				if (bHasPositionafter == M4_TRUE) {
					//La componente tiene Positionafter.
					if (iPositionFound != -1) {
						//La componente hija la ponemos ya en situación correcta.
						itVcCComponent it = vMergeParentChild.begin() + iPositionFound;
						vMergeParentChild.insert(it, pViewChild);
						
					}
					else {
						//No existe la componente especificada en el atributo Positionafter en la lista de merge.
						//Pero puede ser pq todavía no se haya metido en la lista!. Hay que meter antes el que busco.
						//El que busco puede estar en el hijo o en el padre. Es prioritario hijo.
						//Es decir, pViewChild (Boton2) tiene Positionafter y busca al Boton4 en la lista y no lo 
						//ha encontrado. Por lo tanto tenemos que ver donde está el Boton4, prioridad en hijo y 
						//despues en padre!.
						//vMergeParentChild.push_back(pViewChild);
						vLimbo.push_back(pViewChild);
					}
				}
				else {
					//La componente NO tiene Positionafter.
					//Se inserta y ya esta.
					vMergeParentChild.push_back(pViewChild);
				}
				//---------------------------------
			}
			else {
				//Es atributo.
				vMergeParentChild.push_back(pViewChild);
			}
		}		
	}
	//FIN Paso1.
	//-------------------------------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------------------------------
	//Paso2:
	//Meto en la lista merge las componentes del hijo que no se han metido (probablemente pq eran nuevas).
	m4int32_t iPosPositionToInsertSeq = vMergeParentChild.size();
	itVcCComponent itPositionToInsertSeq = vMergeParentChild.begin() + iPosPositionToInsertSeq;
	m4int32_t iNumComponentChild = propBag->GetNumComponent();	
	for (i=0; i<iNumComponentChild; i++) {
		CComponent * pViewChild = propBag->View(i);

		//-------------------------------------
		//Ojo, si la componente se metio en vLimbo se tratará mas tarde!.
		m4bool_t bFoundInLimbo = M4_FALSE;
		for (itVcCComponent itLimbo = vLimbo.begin(); itLimbo != vLimbo.end(); itLimbo++)
		{
			CComponent * pViewInLimbo = (*itLimbo);
			if (pViewChild == pViewInLimbo) {
				bFoundInLimbo = M4_TRUE;
				break; 
			}
		}
		if (bFoundInLimbo == M4_TRUE) {
			continue;
		}
		//-------------------------------------
	
		m4bool_t bFoundChildInMerge = M4_FALSE;

		//Buscamos la componente, el puntero!.
		for (itVcCComponent it = vMergeParentChild.begin(); it != vMergeParentChild.end(); it++)
		{
			CComponent * pViewInMerge = (*it);
			if (pViewChild == pViewInMerge) {
				//Hemos encontrado el hijo en la lista merge. Salimos y NO tendremos que insertar el hijo.
				bFoundChildInMerge = M4_TRUE;
				break;
			}
		}
		
		//Si despues de recorrer la lista merge NO hemos encontrado el hijo lo tenemos que insertar en lista merge.
		//Se trata de una componente nueva del hijo (no estaba en el padre).
		if (bFoundChildInMerge == M4_FALSE) {		
			if (pViewChild->GetTypeProp() != ASNSequence) {
				//Si es un atributo debería meterlo al final de los atributos que tenemos.
				m4int32_t iPosFirstSequenceInVector = CListComponent::FindFirstSequenceInVector( &vMergeParentChild );
				//Obtengo el iterador.
				itVcCComponent itFirstSequenceInVector = vMergeParentChild.begin() + iPosFirstSequenceInVector;
				vMergeParentChild.insert(itFirstSequenceInVector, pViewChild);
				//Hace falta reposicionar el last de sequencias.
				if (iPosFirstSequenceInVector <= iPosPositionToInsertSeq) {
					iPosPositionToInsertSeq++;
				}
			}
			else {				
				//Es sequencia que NO se encontró, vamos a ver dónde debería meterse. Puede ir donde diga el 
				//Positionafter de la componente o detras de lo ultimo que se insertó.
				//Comprobamos si pViewChild tiene Positionafter. Si lo tiene buscamos el indice 
				//de la componente cuyo IdSequence sea el que pone en el after.
				m4bool_t bHasPositionafter = M4_FALSE;
				m4int32_t iPositionFound = -1;
				iRet = pViewChild->FindPositionInVectorSearchingByIdSequence( &vMergeParentChild, bHasPositionafter, iPositionFound );
				if (iRet == M4_ERROR) {
					return M4_ERROR;
				}
				//---------------------------------
				if (bHasPositionafter == M4_TRUE) {
					//La componente tiene Positionafter.
					if (iPositionFound != -1) {
						itPositionToInsertSeq = vMergeParentChild.begin() + iPositionFound;
						vMergeParentChild.insert(itPositionToInsertSeq, pViewChild);
						iPosPositionToInsertSeq = iPositionFound + 1;
					}
					else {
						//Por aqui pasaríamos si la componente hijo especifica un Positionafter que todavía no 
						//se ha metido. Es decir, como estamos en paso2 significa que el hijo es nuevo y su Positionafter
						//hace referencia a una componente tambien nueva del hijo pero que se ha pintado despues.						
						//vMergeParentChild.push_back(pViewChild);
						vLimbo.push_back(pViewChild);
					}
				}
				else {
					//---- Cambio de comportamiento ---
					////La componente NO tiene position after.
					////Se inserta en la ultima posicion de insercion.
					//itPositionToInsertSeq = vMergeParentChild.begin() + iPosPositionToInsertSeq;
					//vMergeParentChild.insert(itPositionToInsertSeq, pViewChild);
					//iPosPositionToInsertSeq++;
					//---- FIN Cambio de comportamiento ---
					
					//Vamos a meterlo al final!. Inicialmente este era el algoritmo pero no gustaba... en fin, como
					//dice el refrán "sin un tapujo ...".
					//Vamos a meterlo al final de las componentes del padre.
					vMergeParentChild.push_back(pViewChild);
				}
				//---------------------------------
			}
		}
		else {
			//Si se encuentra es pq lo insertó el hijo.			
		}
	}
	//FIN Paso2.
	//-------------------------------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------------------------------
	//Paso2 (Anexo1):	
	//Las componentes que estan en el Limbo tienen que poderse pasar a la lista Merge!!!.
	iRet = CListComponent::MergeUnknownInList( vMergeParentChild, vLimbo );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}		
	//En realidad debe estar vacia!.
	vLimbo.clear();
	//FIN Paso2 (Anexo1).
	//-------------------------------------------------------------------------------------------------

	/* Bug 0165819
	La ordenación se debe hacer después de que se hayan metido los objetos del limbo
	*/
	//-------------------------------------------------------------------------------------------------
	//Paso2 (Anexo2):	
	//Escenario:
	//-- Hijo --								-- Padre --
	//	Boton4 (begin)							Boton1
	//	Boton2 (despues de boton4)				Boton2
	//	Boton3 (despues de boton2)				Boton3
	//	Boton1									Boton4
	//Con el Paso1 se recorren objetos del padre y se insertan en la lista vMergeParentChild. Después del Paso1
	//la lista NO está ordenada. Primero se mete Boton1 y cuando se mete el Boton2 (que tiene Positionafter Boton4) 
	//se busca el Boton4 que todavía NO se ha insertado. Es decir, el Boton2 acaba añadiendose al final.
	//Se mete el Boton2 despues del Boton4, se mete el Boton3 despues del Boton2 y por ultimo el Boton4 que se
	//mete al principio: Quedando un orden erroneo: Boton4, Boton1, Boton2, Boton3.
	//Es necesario reorganizar la lista. Con lo que diga el Positionafter.
	iRet = CListComponent::SortListByPositionAfterBegin( vMergeParentChild );
	if (iRet == M4_ERROR) {
		return M4_ERROR;
	}		
	//FIN Paso2 (Anexo2).
	//-------------------------------------------------------------------------------------------------

	//Ahora, en la lista merge tenemos la lista definitiva asociada al hijo y en el orden que estaban en padre!.

	//-------------------------------------------------------------------------------------------------
	//Paso3:
	//Borramos los elementos del hijo SIN LIBERAR. Hacemos que la lista merge sea la nueva sequencia hijo.
	propBag->RemoveAllComponent( M4_FALSE );
	propBag->AddComponentFromComponentVector( &vMergeParentChild );
	//-------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------
	//Paso4:
	//Los elementos del padre que no se han pasado a la lista merge deben quedarse (pq despues tenemos la recursion).
	//Es importante que empecemos por el final pq vamos a quitar lo que sí se ha pasado al merge.
	for (i=(iNumComponentParent-1); i>=0; i--) {
		CComponent * pViewParent = propBagContainerParent->View(i);

		m4bool_t bFoundParentInMerge = M4_FALSE;

		//Buscamos la componente, el puntero!.
		for (itVcCComponent it = vMergeParentChild.begin(); it != vMergeParentChild.end(); it++)
		{
			CComponent * pViewInMerge = (*it);
			if (pViewParent == pViewInMerge) {
				//Hemos encontrado el padre en la lista merge. Tendremos que eliminar, sin liberar, de la seq padre.
				bFoundParentInMerge = M4_TRUE;
				break;
			}
		}

		//Si despues de recorrer la lista merge SI hemos encontrado el padre lo tenemos eliminar sin liberar de
		//la sequencia padre.
		if (bFoundParentInMerge == TRUE) {
			propBagContainerParent->Remove(i);
		}
	}
	//FIN Paso4.
	//-------------------------------------------------------------------------------------------------
	
	//En la sequencia hijo están las componentes del padre que NO estaban en el hijo. Si una componente
	//estaba en padre e hijo estará la del hijo. Por ultimo tenemos las componentes que sólo estaban en hijo.
	//En la sequencia padre solo quedan componentes que estaban en padre e hijo.
	//Importante: Si un atributo estuviera en padre e hijo no se estaría eliminando de padre pero esto no puede darse
	//pq los atributos comunes se eliminaron y liberaron del padre (cuando se analizaron atributos del hijo)!!.
	//Los únicos atributos que están en ambos son el tipo de objeto (classname) y el id (Alias), que son obligatorios 
	//que estén en el padre.

	//Terminamos con la lista temporal.
	vMergeParentChild.clear();

	return iRet;
}
//-------------------------------------------------------------
//FIN Llamadas para el tratamiento de sequencias.
//-------------------------------------------------------------

m4return_t CComponent::GetIdSequenceMoveAttribute( m4bool_t & ao_bExistsMoveAttribute, m4pchar_t & ao_pIdSequenceToMove )
{
	m4return_t iRet = M4_SUCCESS;

	//SI tengo que ser sequencia.
	if (type_prop != ASNSequence) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Retornaremos una cadena con el identificador.
	//IMPORTANTE: Es responsabilidad del invocante liberar la cadena!.
	if (ao_pIdSequenceToMove != NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}	
	ao_bExistsMoveAttribute = M4_FALSE;

	//Buscamos el atributo que nos indica movimiento de sequencias.
	//Tambien extraemos el identificador de la sequencia sobre la que queremos ir DESPUES.

	CComponent * pView = NULL;
	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4uint32_t i=0; i<iNumComponent; i++) {
		pView = propBag->View(i);
		if (pView->GetTypeProp() != ASNSequence) {
			//Sólo me interesa el classname.
			if (pView->IsNameProp(M4PRESENTATION_INHERIT_PROP_POSITIONAFTER) == M4_TRUE) {				
				ao_bExistsMoveAttribute = M4_TRUE;

				//Ahora extraemos el identificador de la componente.
				ao_pIdSequenceToMove = new m4char_t[pView->GetLengthProp() + 1];
				pView->GetStringValueProp(ao_pIdSequenceToMove);
				ao_pIdSequenceToMove[pView->GetLengthProp()] = NULL;
			
				return M4_SUCCESS;
			}
		}
	}

	return M4_SUCCESS;
}

m4return_t CComponent::GetClassNameAndAlias( m4pchar_t & ao_pClassName, m4pchar_t & ao_pAlias )
{
	if ((ao_pClassName != NULL) || (ao_pAlias != NULL)) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}
	
	m4pchar_t pClassName = NULL;
	m4pchar_t pAlias = NULL;

	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4int32_t i=0; i<iNumComponent; i++) {
		CComponent * pView = propBag->View(i);

		//Tratamos atributos que no sean subarboles.
		if (pView->GetTypeProp() != ASNSequence) {
			//Veamos si coinciden con classname y alias.
			if (pView->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE) {
				pClassName = new m4char_t[pView->GetLengthProp() + 1];
				pView->GetStringValueProp(pClassName);
				pClassName[pView->GetLengthProp()] = NULL;
			}
			else {
				if (pView->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {
					pAlias = new m4char_t[pView->GetLengthProp() + 1];
					pView->GetStringValueProp(pAlias);
					pAlias[pView->GetLengthProp()] = NULL;
				}			
			}
		}
	}

	if ((pClassName == NULL) || (pAlias == NULL)) {
		if (pClassName != NULL) {
			delete pClassName;
		}
		if (pAlias != NULL) {
			delete pAlias;
		}
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	ao_pClassName = pClassName;
	ao_pAlias = pAlias;

	return M4_SUCCESS;
}

m4return_t CComponent::_ExistsAttributeInContainer( CComponent * ai_pCComponentContainer, m4int32_t & ao_iAttributePositionInContainer )
{
	m4return_t iRet = M4_SUCCESS;

	ao_iAttributePositionInContainer = -1;

	//NO tengo que ser sequencia.
	if (type_prop == ASNSequence) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//El contenedor tambien tiene que ser secuencia.
	if ((ai_pCComponentContainer == NULL) 
		|| ( ai_pCComponentContainer->propBag == NULL)
		|| ( ai_pCComponentContainer->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//------------------------------------------------------
	CListComponent * pCListComponent = ai_pCComponentContainer->propBag;	
	CComponent * pView = NULL;
	m4uint32_t iNumComponent = pCListComponent->GetNumComponent();
	for (m4uint32_t i=0; i<iNumComponent; i++) {
		pView = pCListComponent->View(i);

		if (pView->GetTypeProp() != ASNSequence) {
			if (IsNameProp(pView) == M4_TRUE) {
				ao_iAttributePositionInContainer = i;
				return M4_SUCCESS;
			}
		}
	}
	//------------------------------------------------------

	return M4_SUCCESS;
}

m4return_t CComponent::_ExistsSequenceInContainer( CComponent * ai_pCComponentContainer, m4int32_t & ao_iSequencePositionInContainer )
{
	//Si estamos aqui es pq somos una secuencia, ademas, el contenedor argumento tambien es una secuencia.
	//Estamos buscando que subsequencia del contenedor argumento que sea igual (classname y alias) que nosotros.

	m4return_t iRet = M4_SUCCESS;

	ao_iSequencePositionInContainer = -1;

	//SI tengo que ser sequencia.
	if (type_prop != ASNSequence) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//El padre tambien tiene que ser secuencia.
	if ((ai_pCComponentContainer == NULL) 
		|| ( ai_pCComponentContainer->propBag == NULL)
		|| ( ai_pCComponentContainer->GetTypeProp() != ASNSequence)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//------------------------------------------------------
	//Extraemos el alias y el class name asociado a la secuencia
	CComponent * pCComponentThisClassName = NULL;
	CComponent * pCComponentThisAlias = NULL;
	m4bool_t bIsDeletedObjectSequence = M4_FALSE;

	if (propBag != NULL) {
		CComponent * pViewChild = NULL;
		m4uint32_t iNumComponent = propBag->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pViewChild = propBag->View(i);
			
			if (pViewChild->GetTypeProp() != ASNSequence) {
				if (pViewChild->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE) {
					pCComponentThisClassName = pViewChild;

					//Comprobamos si somos un elemento de borrado.
					if (IsValueProp(M4PRESENTATION_INHERIT_TYPE_PRESENTATION_DELETEDOBJECT) == M4_TRUE) {
						bIsDeletedObjectSequence = M4_TRUE;
					}
				}
				else {
					//Comprobamos si es el mismo ID.
					if (pViewChild->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {
						pCComponentThisAlias = pViewChild;
					}
				}
			}

			//Si ya tenemos la clase y el alias podemos salir del bucle.
			if ((pCComponentThisClassName != NULL) && (pCComponentThisAlias != NULL)) {
				break;
			}
		}
	}

	//Si no tenemos alias y classname, mal rollo!!.
	if ((pCComponentThisClassName == NULL) || (pCComponentThisAlias == NULL)) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}
	//------------------------------------------------------

	//------------------------------------------------------
	CListComponent * pCListComponent = ai_pCComponentContainer->propBag;	
	if (pCListComponent != NULL) {
		CComponent * pView = NULL;
		m4uint32_t iNumComponent = pCListComponent->GetNumComponent();
		for (m4uint32_t i=0; i<iNumComponent; i++) {
			pView = pCListComponent->View(i);
			if (pView == NULL) {
				DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
				return M4_ERROR;
			}

			//-----------------------------------------
			//Solo me interesa compararme con sequencias!.
			if (pView->GetTypeProp() == ASNSequence) {
				//Necesito obtener el classname y alias de la sequencia.
				CListComponent * pCListComponentInside = pView->propBag;
				if (pCListComponentInside != NULL) {					
					CComponent * pViewInside = NULL;
					m4uint32_t iNumComponentInside = pCListComponentInside->GetNumComponent();
					for (m4uint32_t j=0; j<iNumComponentInside; j++) {
						pViewInside = pCListComponentInside->View(j);					
						if (pViewInside->GetTypeProp() != ASNSequence) {
							if (pViewInside->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE) {
								//Si coinciden classname tengo que continuar para el alias.
								//Si hay que eliminarlo tambien tengo que continuar para comprobar el alias!!.
								if ((pViewInside->IsNameAndValueProp(pCComponentThisClassName) == M4_TRUE) 
									|| (bIsDeletedObjectSequence == M4_TRUE))
								{
									//Continuamos para el alias.									
									continue;
								}
								//Si no coinciden con classname tambien contiuaré, pero podríamos salir del bucle para
								//controlar el escenario de mismo alias y classname diferentes.
								//Ahora no se controla y si coinciden con alias ya nos vale para determinar encontrado.
							}
							else {
								//Comprobamos si es el mismo ID.
								if (pViewInside->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {
									//No obligamos a que coincidan tipos, solo identificadores.
									if (pViewInside->IsNameAndValueProp(pCComponentThisAlias) == M4_TRUE) {
										//Encontrado!!!.
										ao_iSequencePositionInContainer = i;
										return M4_SUCCESS;
									}
								}
							}
						}
					}
				}				
			}
			//-----------------------------------------
		}
	}
	//------------------------------------------------------

	return iRet;
}

m4return_t CComponent::_IsComponentToDelete( m4bool_t & ao_bIsComponentToDelete, m4pchar_t & ao_pIdComponentToDelete )
{
	//Retornaremos una cadena con el identificador.
	//IMPORTANTE: Es responsabilidad del invocante liberar la cadena!.
	if (ao_pIdComponentToDelete != NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}	
	ao_bIsComponentToDelete = M4_FALSE;

	//El this puede ser una sequencia, en cuyo caso comprobaremos si es una sequencia de Deletedobject.
	//Tambien puede ser una componente de tipo atributo, en cuyo caso comprobaremos si tiene la palabra reservada.
	if (type_prop == ASNSequence) {
		if (propBag != NULL) {
			CComponent * pView = NULL;
			m4uint32_t iNumComponent = propBag->GetNumComponent();
			for (m4uint32_t i=0; i<iNumComponent; i++) {
				pView = propBag->View(i);

				if (pView->GetTypeProp() != ASNSequence) {
					//Sólo me interesa el classname.
					if (pView->IsNameProp(M4PRESENTATION_CLASSNAME) == M4_TRUE) {
						if (pView->IsValueProp(M4PRESENTATION_INHERIT_TYPE_PRESENTATION_DELETEDOBJECT) == M4_TRUE) {						
							ao_bIsComponentToDelete = M4_TRUE;
						}
						else {
							return M4_SUCCESS;
						}
					}
					else {
						if (pView->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {
							//Ahora extraemos el identificador. Solo tiene sentido si realmente es una componente a
							//eliminar.
							//Antes de obtener el alias YA debo saber si es una sequencia de borrado.
							if (ao_bIsComponentToDelete == M4_TRUE) {
								ao_pIdComponentToDelete = new m4char_t[pView->GetLengthProp() + 1];
								pView->GetStringValueProp(ao_pIdComponentToDelete);
								ao_pIdComponentToDelete[pView->GetLengthProp()] = NULL;
							}

							//Despues de obtener el alias nos vamos, los valores de retorno han debido quedar
							//resueltos.
							return M4_SUCCESS;
						}
					}
				}
			}
		}
	}

	if (type_prop == ASNGeneralString) {		
		if (IsValueProp(M4PRESENTATION_INHERIT_PROP_DEFAULT_VALUE) == M4_TRUE) {
			ao_bIsComponentToDelete = M4_TRUE;
		}
	}
	
	return M4_SUCCESS;
}

m4return_t CComponent::_RemoveComponentInListByPos( m4int32_t ai_iPosPropBag, m4bool_t ai_bDeleteComponent )
{
	if ((propBag != NULL) 
		&& (ai_iPosPropBag >= 0)
		&& (ai_iPosPropBag < propBag->GetNumComponent()) ) 
	{		 
		if (ai_bDeleteComponent == M4_TRUE) {
			CComponent * pCComponent = propBag->View(ai_iPosPropBag);
			if (pCComponent != NULL) {
				delete pCComponent;
			}
		}
		propBag->Remove(ai_iPosPropBag);
		return M4_SUCCESS;		
	}

	DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
	return M4_ERROR;	
}

m4return_t CComponent::_TransferComponentFromParentComponent( CComponent * ai_pCComponentContainerParent, m4int32_t ai_iComponentPositionInParent, m4int32_t ai_iLastInsertPositionInChildSequence )
{
	//Se transfiere componente pero no se libera.
	//La componente iesima del contenedor padre la queremos transferir al hijo.
	//El que invoca ya ha controlado que el contenedor es una secuencia...

	//Veamos donde insertar la componente.
	
	//---------------------------------------------------------------
	//Lo comentamos pq vamos a cambiar la política.
	
	//m4int32_t iPosToInsert = 0;
	//if (type_prop != ASNSequence) {
	//	if (_FindPositionFirstAttributeAfterAlias( iPosToInsert ) == M4_ERROR) {
	//		return M4_ERROR;
	//	}	
	//}
	//else {
	//	if (_FindPositionFirstSequence( iPosToInsert ) == M4_ERROR) {
	//		return M4_ERROR;
	//	}
	//}
	//---------------------------------------------------------------

	//Nueva politica:
	//Las componentes del padre tienen que transferirse en el mismo orden al hijo!!.
	//Importante: El padre se está recorriendo de (NumComp - 1) a 0. Tenemos que calcular en qué posicion del hijo
	//se va a insertar la componente del padre.

	//Veamos cuantas componentes tiene el hijo.
	m4uint32_t iNumComponentChild = propBag->GetNumComponent();
	m4int32_t iPosToInsert = 0;
	if (ai_iLastInsertPositionInChildSequence == -1) {
		//Si no sabemos donde meterlo lo transferimos al final.
		iPosToInsert = iNumComponentChild;
	}
	else {
		//Si ya se ha transferido algo del padre al hijo, lo dejamos en esa posición
		//(por lo tanto se estará insertando antes que la ultima posicion donde se insertó).
		iPosToInsert = ai_iLastInsertPositionInChildSequence;
	}

	CListComponent * propBagContainerParent = ai_pCComponentContainerParent->propBag;
	CComponent * pView = propBagContainerParent->View( ai_iComponentPositionInParent );

	//Insertamos la componente. Las del padre van a estar al principio		
	if (propBag->InsertComponentInPosition(iPosToInsert, pView) == M4_ERROR) {
		return M4_ERROR;
	}

	propBagContainerParent->Remove(ai_iComponentPositionInParent);

	return M4_SUCCESS;
}

m4return_t CComponent::_FindPositionFirstAttributeAfterAlias( m4int32_t & ao_iPositionFound )
{
	//Buscamos la posicion del atributo que no sea alias.
	//Voy a insertarla justo detras del alias!.

	//SI tengo que ser sequencia.
	if ((type_prop != ASNSequence) 
		|| (propBag == NULL) 
		|| (propBag->GetNumComponent() <= 0) )
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	ao_iPositionFound = -1;

	CComponent * pView = NULL;
	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4uint32_t i=0; i<iNumComponent; i++) {
		pView = propBag->View(i);
		if (pView->GetTypeProp() != ASNSequence) {
			//Sólo me interesa el classname.
			if (pView->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {	
				//Retornamos la siguiente al alias.
				ao_iPositionFound = i + 1;
				return M4_SUCCESS;
			}
		}
	}

	//Si estamos aqui es pq no se ha encontrado una la posicion, mal rollito!!.

	//Seguro de la muerte que debe encontrarse un alias pq el this es una secuencia.	
	DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
	return M4_ERROR;
}

m4return_t CComponent::_FindPositionFirstSequence( m4int32_t & ao_iPositionFound )
{
	//Buscamos la posicion de la primera sequencia que encontremos.
	//Si no se encuentra devolveremos la posicion del ultimo atributo (antes de sequencia).
	
	if ((type_prop != ASNSequence) 
		|| (propBag == NULL) 
		|| (propBag->GetNumComponent() <= 0) )
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//Ojo pq puede ser que no haya sequencias!!.
	ao_iPositionFound = - 1;

	CComponent * pView = NULL;
	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4uint32_t i=0; i<iNumComponent; i++) {
		pView = propBag->View(i);
		if (pView->GetTypeProp() == ASNSequence) {			
			ao_iPositionFound = i;
			return M4_SUCCESS;
		}
		else {
			//Interesa retornar la siguiente posicion del ultimo atributo.			
			ao_iPositionFound = i + 1;
		}
	}

	if (ao_iPositionFound == -1) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	return M4_SUCCESS;
}

m4return_t CComponent::_FindPositionLastSequence( m4int32_t & ao_iPositionFound )
{
	//Empezaremos a buscar por el final!!.
	//Si no hay sequencia consideraremos el último atributo.
	if ((type_prop != ASNSequence) 
		|| (propBag == NULL) 
		|| (propBag->GetNumComponent() <= 0) )
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}
	
	m4uint32_t iLastComponent = propBag->GetNumComponent() - 1;	
	CComponent * pView = NULL;	
	for (m4uint32_t i=iLastComponent; i>=0; i--) {
		pView = propBag->View(i);
		if (pView->GetTypeProp() == ASNSequence) {
			ao_iPositionFound = i;
			return M4_SUCCESS;
		}
	}

	//Si no hay sequencias tomaremos el último atributo.
	ao_iPositionFound = iLastComponent;
	return M4_SUCCESS;
}

m4return_t CComponent::_FindPositionByIdSequence( CComponent * ai_pCComponentContainer, m4int32_t & ao_iPositionFound )
{
	//Nos pasan un contenedor que contiene un alias con un id.
	//Vamos a comprobar si ese id está en nuestra lista de sequencias.
	if ((type_prop != ASNSequence) 
		|| (propBag == NULL) 
		|| (propBag->GetNumComponent() <= 0) )
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	if ((ai_pCComponentContainer == NULL) 
		|| (ai_pCComponentContainer->propBag == NULL) 
		|| (ai_pCComponentContainer->propBag->GetNumComponent() <= 0)) 
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	ao_iPositionFound = - 1;
	
	m4char_t * pIdSequence = NULL;
	m4uint32_t i = 0;	
	CComponent * pView = NULL;

	//Vamos a buscar una sequencia cuyo id coincida con el id de la secuencia argumento.
	CListComponent * propBagParent = ai_pCComponentContainer->propBag;
	m4uint32_t iNumComponentParent = propBagParent->GetNumComponent();
	
	for (i=0; i<iNumComponentParent; i++) {
		pView = propBagParent->View(i);	
		if (pView->GetTypeProp() == ASNGeneralString) {
			//Comprobamos que estamos en la seccion de declaracion de argumentos.
			if (pView->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE)	{
				pIdSequence = new m4char_t[pView->GetLengthProp() + 1];
				pView->GetStringValueProp(pIdSequence);
				pIdSequence[pView->GetLengthProp()] = NULL;
			
				//Ya tenemos el id a buscar!.
				break;
			}
		}
	}

	if (pIdSequence == NULL) {
		//Que ha pasado que nos han dado una sequencia sin identificador en el alias!!.
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}
	
	//------------------------------------------------------	
	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (i=0; i<iNumComponent; i++) {
		pView = propBag->View(i);

		//Solo me interesa comparame con sequencias!.
		if (pView->GetTypeProp() == ASNSequence) {
			//Necesito obtener el classname y alias de la sequencia.
			CListComponent * pCListComponentInside = pView->propBag;
			if (pCListComponentInside != NULL) {
				CComponent * pViewInside = NULL;
				m4uint32_t iNumComponentInside = pCListComponentInside->GetNumComponent();
				for (m4uint32_t j=0; j<iNumComponentInside; j++) {
					pViewInside = pCListComponentInside->View(j);					
					if (pViewInside->GetTypeProp() != ASNSequence) {
						if (pViewInside->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {
							if (pViewInside->IsValueProp( pIdSequence ) == M4_TRUE) {
								//Encontrado!!!.
								ao_iPositionFound = i;								
								delete [] pIdSequence;
								return M4_SUCCESS;
							}
						}
					}
				}
			}			
		}
	}
	//------------------------------------------------------

	//Puede ser que retornemos ok pero no se haya encontrado.
	delete [] pIdSequence;	
	return M4_SUCCESS;
}

m4return_t CComponent::_FindPositionByIdSequence( m4pchar_t ai_pIdSequenceToMove, m4int32_t & ao_iPositionFound )
{
	//Vamos a buscar la posición de un id sequencia.
	//Nosotros somos un contenedor!.

	//SI tengo que ser sequencia.
	if ((type_prop != ASNSequence) 
		|| (propBag == NULL) 
		|| (propBag->GetNumComponent() <= 0) )
	{
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	if (ai_pIdSequenceToMove == NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	ao_iPositionFound = -1;	

	//------------------------------------------------------	
	m4uint32_t iNumComponent = propBag->GetNumComponent();
	for (m4uint32_t i=0; i<iNumComponent; i++) {
		CComponent * pView = propBag->View(i);

		//Solo me interesa comparame con sequencias!.
		if (pView->GetTypeProp() == ASNSequence) {
			//Necesito obtener el classname y alias de la sequencia.
			CListComponent * pCListComponentInside = pView->propBag;
			if (pCListComponentInside != NULL) {
				CComponent * pViewInside = NULL;
				m4uint32_t iNumComponentInside = pCListComponentInside->GetNumComponent();
				for (m4uint32_t j=0; j<iNumComponentInside; j++) {
					pViewInside = pCListComponentInside->View(j);					
					if (pViewInside->GetTypeProp() != ASNSequence) {
						if (pViewInside->IsNameProp(M4PRESENTATION_ALIAS) == M4_TRUE) {
							if (pViewInside->IsValueProp( ai_pIdSequenceToMove ) == M4_TRUE) {
								//Encontrado!!!. Se encuentra en la iesima posicion.
								ao_iPositionFound = i;								
								return M4_SUCCESS;
							}
						}
					}
				}
			}			
		}
	}
	//------------------------------------------------------
	return M4_SUCCESS;
}


m4return_t CComponent::FindPositionInVectorSearchingByIdSequence( VcCComponent * ai_pVcCComponent, m4bool_t & ao_bHasPositionafter, m4int32_t & ao_iPositionFound )
{
	//Comprobamos si la componente (this) tiene Positionafter. Si tiene Positionafter
	//obtengo el IdSequencia del Positionafter y lo busco en la lista que me pasan como argumento.
	//Se devolverá la posición en la lista.

	m4return_t iRet = M4_SUCCESS;

	ao_iPositionFound = -1;
	ao_bHasPositionafter = M4_FALSE;

	/* Bug 0173297
	Si ya está ordenado es como si no tuviese position after
	*/
	if( m_bIsOrdered == M4_TRUE )
	{
		return M4_SUCCESS;
	}

	if (ai_pVcCComponent == NULL) {
		DUMP_CHLOG_ERRORF( M4_CH_PRES_INHERIT_BUILD_INTERNAL_ERROR, __FILE__ << __LINE__ ) ;
		return M4_ERROR;
	}

	//----------------------- Comprobar si tiene Positionafter -----------------------------
	//Es una seq pero veamos si tiene Positionafter.
	//Únicamente se mueven sequencias. En realidad se trata de subsequencias.
	m4bool_t bExistsMoveAttribute = M4_FALSE;
	m4pchar_t pIdSequenceToMove = NULL;
	iRet = GetIdSequenceMoveAttribute( bExistsMoveAttribute, pIdSequenceToMove );
	if (iRet == M4_ERROR) {
		//Si hay error tampoco nos habrán dado un id que tengamos que liberar.
		return M4_ERROR;
	}

	if (bExistsMoveAttribute == M4_TRUE) {
		m4int32_t iPosDestination = -1;

		//----------------------------------------------------------------------------
		//Ojo: Si queremos moverlo al principio, lo moveremos a la primera sequencia!!.
		//Entendemos que si no hay id o es la palabra reservada movemos al principio.
		if ((pIdSequenceToMove == NULL) 
			|| strcmp(pIdSequenceToMove, M4PRESENTATION_INHERIT_PROP_POSITIONAFTER_BEGINVALUE) == 0)
		{
			//Lo queremos mover al principio, como primera sequencia!.
			iPosDestination = CListComponent::FindFirstSequenceInVector( ai_pVcCComponent );					
		}
		else {
			m4int32_t iPosComponentByIdSequence = CListComponent::FindComponentInVectorByIdSequence( ai_pVcCComponent, pIdSequenceToMove );
			if (iPosComponentByIdSequence == ai_pVcCComponent->size()) 
			{
				//No se ha encontrado el id.

				//Vaya!!, nos han puesto un id de objeto que no existe!, pero no vamos a dar error, no no no.
				//Los gurús dicen que no deberíamos dar error. Como dice la canción: "Fenómeno, eeeres un fenómeno".
				//Ahora les mola al final, pero solo ahora.

				//No lo movemos al final, el invocante decidirá donde ponerlo.
				//iPosDestination = ai_pVcCComponent->size();
				iPosDestination = -1;
			}
			else {
				//Se encontró el id, y tenemos la posicion exacta donde se encontro, pero queremos insertar
				//justo después (AFTER) de esa posicion!.
				iPosDestination = iPosComponentByIdSequence;
				iPosDestination++;				

				/* Bug 0173297
				Si se encuentra el componente se marca como ordenado
				*/
				m_bIsOrdered = M4_TRUE;
			}					
		}
		ao_bHasPositionafter = M4_TRUE;
		ao_iPositionFound = iPosDestination;
		//----------------------------------------------------------------------------
	} // if (bExistsMoveAttribute == M4_TRUE)

	if (pIdSequenceToMove != NULL) {
		delete pIdSequenceToMove;
		pIdSequenceToMove = NULL;
	}
	//----------------------- FIN Comprobar si tiene Positionafter -----------------------------

	return iRet;
}

m4return_t CComponent::HasPositionAfterBegin( m4bool_t & ao_bHasPositionafterBegin )
{
	//Comprobamos si la componente (this) tiene Positionafter = Begin		
	m4return_t iRet = M4_SUCCESS;

	ao_bHasPositionafterBegin = M4_FALSE;

	//----------------------- Comprobar si tiene Positionafter -----------------------------
	//Es una seq pero veamos si tiene Positionafter.
	//Únicamente se mueven sequencias. En realidad se trata de subsequencias.
	m4bool_t bExistsMoveAttribute = M4_FALSE;	
	m4pchar_t pIdSequenceToMove = NULL;
	iRet = GetIdSequenceMoveAttribute( bExistsMoveAttribute, pIdSequenceToMove );
	if (iRet == M4_ERROR) {
		//Si hay error tampoco nos habrán dado un id que tengamos que liberar.
		return M4_ERROR;
	}

	if (bExistsMoveAttribute == M4_TRUE) {
		//----------------------------------------------------------------------------		
		//Entendemos que si no hay id o es la palabra reservada movemos al principio.
		if ((pIdSequenceToMove == NULL) 
			|| strcmp(pIdSequenceToMove, M4PRESENTATION_INHERIT_PROP_POSITIONAFTER_BEGINVALUE) == 0)
		{
			ao_bHasPositionafterBegin = M4_TRUE;
		}
		//----------------------------------------------------------------------------
	} // if (bExistsMoveAttribute == M4_TRUE)

	if (pIdSequenceToMove != NULL) {
		delete [] pIdSequenceToMove;
		pIdSequenceToMove = NULL;
	}
	//----------------------- FIN Comprobar si tiene Positionafter -----------------------------

	return iRet;
}


m4int32_t CComponent::referenceSolver (m4uint32_t ai_iLenName, m4pchar_t ai_pcPropName,
		m4uint32_t ai_iLenValue, m4pchar_t ai_pcPropValue, m4uchar_t ai_cPropValueType, m4bool_t &ao_bIsTranslation, 
		CAsn *ai_pReference, m4bool_t ai_bIsDef, m4uint32_t ai_iReferences, CAsn *ai_paMultiReference, m4pchar_t *ai_ppcLangCodes, CComponent *ai_pParent )

{
	
	if (ai_bIsDef == M4_FALSE) 
	{

		//  Begin Run Time
		//  -----------------
		//  This code is resolving _EA538B8 to its text value in the runtime language. 

		TYPE_COMP cRef;
		CComponent *seq;
		m4uchar_t vBool;
		m4uint32_t vInt = 0;

		seq = ai_pReference->present;
		ao_bIsTranslation = M4_TRUE; 

		m4int_t m_reference = FindReference(seq,ai_pcPropValue,&ai_iLenValue,&ai_cPropValueType,&cRef);
		if (m_reference==0)
		{
			// es una cadena que empieza con "_" pero no es traducible
			SetTypeProp(T_STRING);
			SetLengthProp(M4_FALSE, T_STRING, 0, ai_iLenValue);
			SetStringValueProp(ai_pcPropValue, ai_iLenValue);			
		}
		else
		{
			// compruebo que se ha encontrado una referencia
			if (m_reference==1)	
			{
				switch(ai_cPropValueType)
				{
				case ASNBOOLEAN:
					
						SetTypeProp(T_BOOL);
						SetLengthProp(M4_FALSE,T_BOOL);
						vBool = cRef.byte_value;
						SetBoolValueProp(vBool);
						break;

				case ASNINTEGER:
					
						SetTypeProp(T_INT);
						vInt = cRef.int_value;
						SetLengthProp(M4_FALSE,T_INT,vInt);
						SetIntValueProp(vInt);
						break;

				case ASNGeneralString:
					
						SetTypeProp(T_STRING);
						SetLengthProp(M4_FALSE, T_STRING, 0, ai_iLenValue);
						SetStringValueProp(cRef.char_value, ai_iLenValue);
						delete [](cRef.char_value);
						break;
				}
			}
		}
		// End Run Time
		// -----------------
	}
	else
	{
		// Begin Design Time 
		// -----------------
		// This code is adding _EA538B8 as a value
		// And creating a new component/sequence with the language values. 
		//		Text = "_EA538B8"
		//      BEGIN TraductProp _EA538B8
		//           Name = "Text"
		//           ...
		//           Value_ENG = "TextoIdiomaIngles"
		//           ...
		//      END


		// Add the value of Text or Tooltip ("_EA538B8") 
		SetTypeProp(T_STRING);
		SetLengthProp(M4_FALSE,T_STRING, 0, ai_iLenValue);
		SetStringValueProp(ai_pcPropValue, ai_iLenValue);

		ao_bIsTranslation = M4_TRUE;
		
		// Create component for sequence BEGIN TraductProp = _EA538B8 ... END
		// ------------------------------------------------------------------
		// When building the ASN for SEQUENCES, you can see that an OBL containing
		//	BEGIN NAME VALUE
		// expands in the buffer to 
		//	Classname NAME Alias VALUE

		CComponent *oReferenceSeq = new CComponent(); // 
		oReferenceSeq->SetTypeProp(T_SEQ);
				
		CComponent *oCompTraductProp = new CComponent();
		oCompTraductProp->SetTypeNameProp(); // String				
		oCompTraductProp->SetLengthNameProp(M4PRESENTATION_CLASSNAME_SIZE);
		oCompTraductProp->SetNameProp(M4PRESENTATION_CLASSNAME, M4PRESENTATION_CLASSNAME_SIZE);

		oCompTraductProp->SetTypeProp(T_STRING);		
		oCompTraductProp->SetLengthProp(M4_FALSE, T_STRING, 0, M4PRESENTATION_TRADUCTPROP_SIZE);	
		oCompTraductProp->SetStringValueProp(M4PRESENTATION_TRADUCTPROP, M4PRESENTATION_TRADUCTPROP_SIZE);

		oReferenceSeq->propBag->Add(oCompTraductProp);

		CComponent *oCompAlias = new CComponent();
		oCompAlias->SetTypeNameProp(); // String				
		oCompAlias->SetLengthNameProp(M4PRESENTATION_ALIAS_SIZE);
		oCompAlias->SetNameProp(M4PRESENTATION_ALIAS,M4PRESENTATION_ALIAS_SIZE);

		oCompAlias->SetTypeProp(T_STRING);		
		oCompAlias->SetLengthProp(M4_FALSE, T_STRING, 0, ai_iLenValue);	
		oCompAlias->SetStringValueProp(ai_pcPropValue, ai_iLenValue);

		oReferenceSeq->propBag->Add(oCompAlias);
		
		// --------------------------------------------------------------- 
		// Create the component Name = "Text", "Tooltip"... 
				
		CComponent *oCompNameText = new CComponent(); 
		oCompNameText->SetTypeNameProp();
		oCompNameText->SetLengthNameProp(M4PRESENTATION_TRADUCTPROPNAME_SIZE);
		oCompNameText->SetNameProp(M4PRESENTATION_TRADUCTPROPNAME,M4PRESENTATION_TRADUCTPROPNAME_SIZE);		
		
		oCompNameText->SetTypeProp(T_STRING);
		oCompNameText->SetLengthProp(M4_FALSE, T_STRING, 0, ai_iLenName);
		oCompNameText->SetStringValueProp(ai_pcPropName, ai_iLenName);	

		oReferenceSeq ->propBag->Add(oCompNameText);

		m4bool_t	bAnyLanguage = M4_FALSE ;

		// Create the component Value_XXX = "Language_XXX"
		// --------------------------------------------------------------- 
		for (m4uint32_t i = 0; i < ai_iReferences; i++) 
		{			
			// Value Substitutive of VALUE_XXX
			TYPE_COMP cRef; 
			m4uint32_t iLenValue = ai_iLenValue;			
			if (ai_paMultiReference[i].present != NULL) 
			{
				m4int_t m_reference = m_reference = FindReference(ai_paMultiReference[i].present, ai_pcPropValue, &iLenValue, &ai_cPropValueType, &cRef);

				if (m_reference != 0) 
				{
					bAnyLanguage = M4_TRUE ;

					/* Bug 0134523
					Sólo se debe crear la componente si se inserta en el árbol. Sino es un memory leak.
					Lo mismo sucede para el nombre
					*/
					CComponent *oReference = new CComponent(); 

					// Name "Value_XXX"
					string sName = "Value_";
					sName.append( ai_ppcLangCodes[ i ] );

					// I assume that it is always a STRING, unlike the original code.	
					oReference->SetTypeNameProp();
					oReference->SetLengthNameProp(sName.length());
					oReference->SetNameProp(sName.c_str(), sName.length());
							
					oReference->SetTypeProp(T_STRING);
					oReference->SetLengthProp(M4_FALSE, T_STRING, 0, iLenValue);
					oReference->SetStringValueProp(cRef.char_value, iLenValue);	

					oReferenceSeq->propBag->Add(oReference);
				}				
			}
		} // end for loop

		// -- If no language is found, the sequence is not added.
		if( bAnyLanguage == M4_TRUE )
		{
			ai_pParent->propBag->Add(oReferenceSeq);
		}
		else
		{
			delete oReferenceSeq;
		}
				
		// -----------------
		// End Design Time
	}
	return M4_TRUE; 						
}

