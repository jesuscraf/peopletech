// external calls
#include "stdafx.h"
#include "m4csdll.h"
#include "clcstimeout.h"
#include "clcstimeoutch.h"
#include "csexternal.hpp"
#include "cleventdial.h"

long TimeoutDisplay(const char *ai_pcMessage, const long ai_iCurrent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	long iCurrent = ai_iCurrent / 1000;

	// shows the dialog
	ClCSTimeOut oWindow;

	oWindow.SetTimeOut(iCurrent);

	long ret = -1;
	int loop = 1;

	while (loop)
	{
		loop = 0;
		switch(oWindow.DoModal())
		{
		case IDABORT:
			ret = -10;
			break;
		case IDRETRY:
		case IDC_CS_CHANGE:
			ret = oWindow.GetTimeOut();
			if (ret < 0 || ret > SECS_IN_A_DAY)
				loop = 1;
			break;
		default:
			ret = -1;
		}
	}

	return (ret < 0) ? ret : (ret*1000);
}



long ServerEventDisplay(const long ai_lServerEvent, const void * ai_pParam )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	ClEventDial oEventDial ;

	oEventDial.SetEvent ( ai_lServerEvent ) ;



	oEventDial.SetParam ( ai_pParam ) ;


	oEventDial.DoModal () ;


return 0 ;
}
