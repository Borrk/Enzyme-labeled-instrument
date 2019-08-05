#ifndef __ENM_BEEPER_H__
#define __ENM_BEEPER_H__

#include "EnmBase.hpp"

enum eBEEPTYPE
{
	BEEP_HINT,
	BEEP_ALARM,
	BEEP_INVALID	
};

class CEnmBeeper
{
/// member routines
public:
	CEnmBeeper(){}
	
	static void Beep( INT16U tm );
	static void ParttenBeep( eBEEPTYPE type );

private:
	static void BeepAlarm();
	static void BeepHint();
	static void BeepInvalid();
	
/// member variables
	
};



#endif  ///< __ENM_BEEPER_H__
