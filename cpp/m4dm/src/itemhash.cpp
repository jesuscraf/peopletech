#include "itemhash.hpp"
#include "item_prx.hpp"
#include "colitemdefit.hpp"
#include "m4mdrt.hpp"


/* #define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
};
*/
#define mix(a,b,c) {   a -= b; a -= c; a ^= (c>>13);  b -= c; b -= a; b ^= (a<<8);  c -= a; c -= b; c ^= (b>>13);  a -= b; a -= c; a ^= (c>>12);   b -= c; b -= a; b ^= (a<<16); c -= a; c -= b; c ^= (b>>5);  a -= b; a -= c; a ^= (c>>3);   b -= c; b -= a; b ^= (a<<10);  c -= a; c -= b; c ^= (b>>15); };

#define TOU(k,a,b)      (((m4uint32_t)(toupper(k[a])))<<b)

m4uint32_t ClItemHash::hash( m4uint8_t *k, m4uint32_t length)
{
   m4uint32_t a,b,c,len;

   /* Set up the internal state */
   len = length;
   a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
   c = M4_HASH_INITVAL;         /* the previous hash value */

   /*---------------------------------------- handle most of the key */
   while (len >= 12)
   {
      a += TOU(k,0,0) + TOU(k,1,8) + TOU(k, 2,16) + TOU(k, 3,24);
      b += TOU(k,4,0) + TOU(k,5,8) + TOU(k, 6,16) + TOU(k, 7,24);
      c += TOU(k,8,0) + TOU(k,9,8) + TOU(k,10,16) + TOU(k,11,24);
      mix(a,b,c);
      k += 12; len -= 12;
   }

   /*------------------------------------- handle the last 11 bytes */
   c += length;
   switch(len)              /* all the case statements fall through */
   {
   case 11: c+= TOU(k,10,24);
   case 10: c+= TOU(k,9,16);
   case 9 : c+= TOU(k,8,8);
      /* the Index byte of c is reserved for the length */
   case 8 : b+= TOU(k,7,24);
   case 7 : b+= TOU(k,6,16);
   case 6 : b+= TOU(k,5,8);
   case 5 : b+= TOU(k,4,0);
   case 4 : a+= TOU(k,3,24);
   case 3 : a+= TOU(k,2,16);
   case 2 : a+= TOU(k,1,8);
   case 1 : a+= TOU(k,0,0);
     /* case 0: nothing left to add */
   }
   mix(a,b,c);
   /*-------------------------------------------- report the result */
   return c;
}


m4uint32_t ClItemHash::hash_AlreadyUpper( m4uint8_t *k, m4uint32_t length)
{
   m4uint32_t a,b,c,len;

   /* Set up the internal state */
   len = length;
   a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
   c = M4_HASH_INITVAL;         /* the previous hash value */

   /*---------------------------------------- handle most of the key */
   while (len >= 12)
   {
      a += (k[0] +((m4uint32_t)k[1]<<8) +((m4uint32_t)k[2]<<16) +((m4uint32_t)k[3]<<24));
      b += (k[4] +((m4uint32_t)k[5]<<8) +((m4uint32_t)k[6]<<16) +((m4uint32_t)k[7]<<24));
      c += (k[8] +((m4uint32_t)k[9]<<8) +((m4uint32_t)k[10]<<16)+((m4uint32_t)k[11]<<24));
      mix(a,b,c);
      k += 12; len -= 12;
   }

   /*------------------------------------- handle the last 11 bytes */
   c += length;
   switch(len)              /* all the case statements fall through */
   {
   case 11: c+=((m4uint32_t)k[10]<<24);
   case 10: c+=((m4uint32_t)k[9]<<16);
   case 9 : c+=((m4uint32_t)k[8]<<8);
      /* the Index byte of c is reserved for the length */
   case 8 : b+=((m4uint32_t)k[7]<<24);
   case 7 : b+=((m4uint32_t)k[6]<<16);
   case 6 : b+=((m4uint32_t)k[5]<<8);
   case 5 : b+=k[4];
   case 4 : a+=((m4uint32_t)k[3]<<24);
   case 3 : a+=((m4uint32_t)k[2]<<16);
   case 2 : a+=((m4uint32_t)k[1]<<8);
   case 1 : a+=k[0];
     /* case 0: nothing left to add */
   }
   mix(a,b,c);
   /*-------------------------------------------- report the result */
   return c;
}


ClItemHash::ClItemHash(ClItemDefIt_Random &ai_itemdefit, _AClItemProxy **ai_ppItemProxy, m4uint32_t ai_iCount)
{
    //m_itemdefit = & ai_itemdefit;
    //m_ppItemProxy = ai_ppItemProxy;

    m4uint32_t i, h;
    m4pcchar_t pccItemId;

//  Busco la primera potencia de 2 mayor que el count que nos pasan.
if (ai_iCount) {

    m_iCount = 1;
    //truncbits = 0;    

    while (m_iCount < ai_iCount)
    {
        //truncbits ++;        
        m_iCount = m_iCount << 1;
    }
    m_hashmask = m_iCount - 1;

    CTable = new ChainTable[m_iCount];

    for (i = 0; i < ai_iCount; i++)
    {
        ai_itemdefit.ItemDef.SetHandle (ai_ppItemProxy [i]->GetHandle ());
		pccItemId = ai_itemdefit.ItemDef.Id() ;

        //contamos con que el caracter anterior a pccItemId contiene su longitud
        M4_ASSERT (strlen (pccItemId) < 256);
        M4_ASSERT ( ((m4char_t)strlen (pccItemId)) == *(pccItemId - 1));

		h = hash((m4uint8_t *) pccItemId, *(pccItemId - 1));
        h = (h & m_hashmask);

        if (CTable[h].Index == M4_HASH_EMPTY)
        {
            CTable[h].Index = i;
        }
        else
        {
            #ifdef _DEBUG
            m4uint32_t iantilooping = h;
            #endif
            //tenemos un choque, mala suerte:
            do {
                h++;    //vamos al siguiente hasta encontrar un hueco
                if (h >= m_iCount)   //cuidado que son unsigned !!
                    h = 0;  //damos la vuelta

                #ifdef _DEBUG
                M4_ASSERT (iantilooping != h);
                #endif

            } while (CTable[h].Index != M4_HASH_EMPTY);

            M4_ASSERT (CTable[h].Index == M4_HASH_EMPTY);

            CTable[h].Index = i;
        }
    }
}
else {
    CTable = 0;
    m_iCount = 0;
    m_hashmask = 0;
}
}


m4uint32_t ClItemHash::Hfind(ClItemDefIt_Random &ai_itemdefit, _AClItemProxy **ai_ppItemProxy, m4pcchar_t name,
                             m4bool_t ai_bIsUpper)
{
    m4bool_t equal;

    if (m_iCount)
    {
        int len = strlen(name);
        m4uint32_t h;

        if (ai_bIsUpper)
            h = hash_AlreadyUpper( (m4uint8_t *) name, len);
        else
            h = hash( (m4uint8_t *) name, len);

        h = (h & m_hashmask);

        m4uint32_t iantilooping = h;

        //tenemos un choque, mala suerte:
        m4pcchar_t pccItemId;
        m4char_t cItemIdLenght;
    
        while (CTable[h].Index != M4_HASH_EMPTY) {
            //contamos con que el caracter anterior a pccItemId contiene su longitud
            pccItemId = GiveString(ai_itemdefit, ai_ppItemProxy, CTable[h].Index);
            cItemIdLenght = pccItemId [-1];
            M4_ASSERT (strlen (pccItemId) < 256);
            M4_ASSERT (((m4char_t)strlen (pccItemId)) == cItemIdLenght);

            if (len == cItemIdLenght)
            {
                if (ai_bIsUpper)
                    equal = (!strcmp(name, pccItemId)) ? M4_TRUE : M4_FALSE;
                else
                    //comparacion no case-sensitive
                    equal = (!strcmpi(name, pccItemId)) ? M4_TRUE : M4_FALSE;

                if (equal)
                    //encontrado !
                    return CTable[h].Index;
            }

            //no encontrado, vamos al siguiente:
            h++;    //vamos al siguiente hasta encontrar un hueco
            if (h >= m_iCount)   //cuidado que son unsigned !!
                h = 0;  //damos la vuelta

            if (iantilooping == h) {
                //no encontrado
                break;
            }

            //seguimos mientras que no encontremos huecos:
        }
    }
    return M4_HASH_EMPTY;
}

m4pcchar_t ClItemHash::GiveString(ClItemDefIt_Random &ai_itemdefit, _AClItemProxy **ai_ppItemProxy, m4uint32_t ai_index)
{
	/* Bug 0278825
	No se puede hacer ai_itemdefit.ItemDef.SetHandle porque no sería multithread
	Se coge el id directamente del metacanal
	*/
	m4uint32_t	iHandle = ai_ppItemProxy [ai_index]->GetHandle() ;

	if( iHandle == 0 )
	{
        return( "" ) ;
    }

	ClCompiledMCR	*poCMCR = ai_itemdefit.ItemDef.GetpCMCR() ;

	if( poCMCR == NULL )
	{
        return( "" ) ;
    }

    return( poCMCR->GetItemId( iHandle ) ) ;
}

m4return_t ClItemHash::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ClItemHash);

	if (CTable){
		for (m4uint32_t i=0;i<m_iCount;++i){
			if (CTable[i].GetSize(ao_size)!=M4_SUCCESS){
				return M4_ERROR;
			}
		}
		// No es necesario porque CTable no tiene destructor.
		// ao_size+=sizeof(int); // array de clases con destructor. Se reserva espacio para el tamaño.
	}
	return M4_SUCCESS;
}
m4return_t 
ChainTable::GetSize(ClSize &ao_size)
{
	ao_size+=sizeof(ChainTable);
	return M4_SUCCESS;
}
