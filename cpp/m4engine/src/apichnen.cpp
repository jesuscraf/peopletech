#include "apichni.h"

#include "apichnen.h"

StMIT_APIChn_TaskEnv::StMIT_APIChn_TaskEnv() 
{
	m_poMVC=NULL ;
	ItemV_m_poCurrChn=NULL;
	Chn_m_poChnCurrency=NULL ;
	bFromWrapper = M4_FALSE;

	Chn_m_oTimerChannelAccess.Reset() ;
	Chn_m_oTimerBuildFromId.Reset() ;
	Chn_m_oTimerChannelDestroy.Reset() ;
	Chn_m_oTimerStartVM.Reset() ;
	Chn_m_oTimerShutDownVM.Reset() ;
	Chn_m_oTimerExecuteMethod.Reset() ;
	Chn_m_oTimerLoad.Reset() ;
    Chn_m_oTimerUnload.Reset() ;
	Chn_m_oTimerStartUpCurrency.Reset() ;
}
