#include "EnmBeeper.hpp"

#ifdef ENM_VER_TARGET
#include "44blib.h"
#define	 	BEEP( tm )  beep( (tm) )
#else
#define		BEEP( tm ) 
#endif 

void CEnmBeeper::Beep( INT16U tm )
{
	BEEP( tm );
}

void CEnmBeeper::ParttenBeep( eBEEPTYPE type )
{
	switch ( type )
	{
	case BEEP_ALARM:
		BeepAlarm();
		break;
	case BEEP_HINT:
		BeepHint();
		break;
	case BEEP_INVALID:
		BeepInvalid();
		break;
	default:
		break;	
	}
}

void CEnmBeeper::BeepAlarm()
{
	INT16U i;

	for ( i = 0; i < 4; i++ )
	{
		BEEP( 150 );
		EnmDelay(100);
	}
}

void CEnmBeeper::BeepHint()
{
	INT16U i;

	for ( i = 0; i < 1; i++ )
	{
		BEEP( 4000 );
		//EnmDelay(1000);
	}
}

void CEnmBeeper::BeepInvalid()
{
	INT16U i;

	for ( i = 0; i < 2; i++ )
	{
		BEEP( 50 );
		EnmDelay(100);
	}
}

