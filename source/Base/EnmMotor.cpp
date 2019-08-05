#include "EnmMotor.h"

CEnmMotor::CEnmMotor()
{
	mCurPos = 0;
}		

CEnmMotor::~CEnmMotor()
{
	
}

S32  CEnmMotor::IMoveTo( S32 pos )
{
	if ( pos >= GetTotalStep() )
		return mCurPos;

	if ( pos > mCurPos )
	{
		for ( U32 i = 0; i < (pos - mCurPos); i++ )
			IMotorFW();
	}
	else
	{
		for ( U32 i = 0; i < (pos - mCurPos); i++ )
			IMotorBW();
	}

	mCurPos = pos;
	return mCurPos;
}

S32	CEnmMotor::IToHead()
{
	S32 aHeadPos = mMotorInfo.fPreDist / mMotorInfo.fStepDist;
	if ( mCurPos >= aHeadPos )
	{
		for ( S32 i = 0; i < (mCurPos - aHeadPos); i++ )
			IMotorBW();
	}
	else
	{
		for ( S32 i = 0; i < (aHeadPos - mCurPos); i++ )
			IMotorFW();	
	}

	mCurPos = aHeadPos;
	return mCurPos;
}

S32	CEnmMotor::IToHole( U32 idx )
{
	if ( idx >= mMotorInfo.iHoleCount )
		return mCurPos;

	S32 aHolePos = (mMotorInfo.fPreDist + mMotorInfo.fHolePre + mMotorInfo.fHoleDist * idx) / mMotorInfo.fStepDist;
	if ( mCurPos >= aHolePos )
	{
		for ( S32 i = 0; i < (mCurPos - aHolePos); i++ )
			IMotorBW();
	}
	else
	{
		for ( S32 i = 0; i < (aHolePos - mCurPos); i++ )
			IMotorFW();	
	}

	mCurPos = aHolePos;
	return mCurPos;
}

S32	CEnmMotor::IReset( BOOLEAN bBack )
{
	S32 i;
	S32 cnt = 0;
	S32 aMax = (mMotorInfo.fPreDist + mMotorInfo.fPostDist) / mMotorInfo.fStepDist;
	
	IMotorOn( TRUE );
	
	/// move forward first
	if ( bBack )
	{
		for ( i = 0; i < 20; i++ )
		{
			IMotorFW();
		}
		EnmDelay( 100 );
	}

	while (1)
	{	
		if ( IIsReset() )
		{		 
		  mCurPos = 0;	
		  break;
		}	
			
		IMotorBW();
		/// if step larger than max step, quit
		if ( (++cnt) > aMax )
		{
			mCurPos = 0; //-1;
			break;
		}				
	}	
	
	return mCurPos;	
}



