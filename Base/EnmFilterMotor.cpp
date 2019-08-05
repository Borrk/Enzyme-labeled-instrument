#include "EnmFilterMotor.h"

#ifdef ENM_VER_TARGET
	#include  "key&motor.h"
#else
	void Light_Motor_On( char bOn ){}
	void Light_Motor_CW_ONE_STEP(void){}
	void Light_Motor_CCW_ONE_STEP(void){}
	unsigned char Is_Light_Motor_Reset(void){ return 1; }
#endif

CEnmFilterMotor::CEnmFilterMotor()
{
}		

CEnmFilterMotor::~CEnmFilterMotor()
{
}

CEnmFilterMotor& CEnmFilterMotor::Instance()
{
	static CEnmFilterMotor aInst;
	return aInst;
}

BOOLEAN     CEnmFilterMotor::IMotorOn( BOOLEAN bOn )
{
	Light_Motor_On( bOn ? 1 : 0 );
	return TRUE;
}

BOOLEAN     CEnmFilterMotor::IMotorFW()
{
	Light_Motor_CW_ONE_STEP();
	return TRUE;
}

BOOLEAN     CEnmFilterMotor::IMotorBW()
{
	Light_Motor_CCW_ONE_STEP();
	return TRUE;
}

BOOLEAN     CEnmFilterMotor::IIsReset()
{
	return Is_Light_Motor_Reset() ? TRUE : FALSE;
}
