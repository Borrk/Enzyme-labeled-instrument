#include "EnmPlateMotor.h"

#ifdef ENM_VER_TARGET
	#include  "key&motor.h"
#else
	void Plate_Motor_On( char bOn ){}
	void Plate_Motor_CW_ONE_STEP(void){}
	void Plate_Motor_CCW_ONE_STEP(void){}
	unsigned char Is_Plate_Motor_Reset(void){ return 1; }
#endif

CEnmPlateMotor::CEnmPlateMotor()
{
}		

CEnmPlateMotor::~CEnmPlateMotor()
{
}

CEnmPlateMotor& CEnmPlateMotor::Instance()
{
	static CEnmPlateMotor aInst;
	return aInst;
}

S32   CEnmPlateMotor::IFwOne()
{
	for ( U16 i = 0; i < (U16)( 0.5 + mMotorInfo.fHoleDist / mMotorInfo.fStepDist); i++ )
	{
		Plate_Motor_CW_ONE_STEP();
	}
    
    return mCurPos;
}

S32   CEnmPlateMotor::IBwOne()
{
	for ( U16 i = 0; i < (U16)( 0.5 + mMotorInfo.fHoleDist / mMotorInfo.fStepDist); i++ )
	{
		Plate_Motor_CCW_ONE_STEP();
	}

    return mCurPos;
}

BOOLEAN     CEnmPlateMotor::IMotorOn( BOOLEAN bOn )
{
	Plate_Motor_On( bOn ? 1 : 0 );
	return TRUE;
}

BOOLEAN     CEnmPlateMotor::IMotorFW()
{
	Plate_Motor_CW_ONE_STEP();
	return TRUE;
}

BOOLEAN     CEnmPlateMotor::IMotorBW()
{
	Plate_Motor_CCW_ONE_STEP();
	return TRUE;
}

BOOLEAN     CEnmPlateMotor::IIsReset()
{
	return Is_Plate_Motor_Reset() ? TRUE : FALSE;
}
