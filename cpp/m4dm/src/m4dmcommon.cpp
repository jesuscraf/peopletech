#include "m4dmcommon.hpp"
#include "access.hpp"

void __SSearch( ClRegister & ai_Reg, m4pchar_t ai_pcVal, ClItemIndex ai_Item)
{
	m4pcchar_t pcTemp;
	ClRegisterIndex i;
	m4uint32_t NumRows = ai_Reg.Count();

	for ( i = 0; i<NumRows; ++i)
	{
			pcTemp = (m4pcchar_t) ai_Reg[i].Item[ai_Item].Value.Get();

            if (pcTemp== 0){
                continue;
            }
                    
			if ( stricmp(pcTemp, ai_pcVal ) == 0 )
			{				
				return;
			}
	}

	ai_Reg.MoveToEOF();
}
