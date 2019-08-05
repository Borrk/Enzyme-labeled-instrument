// EnmScanEngine.h: interface for the CEnmScanEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENMMOTOR_H__E3F55576_CCCD_4B20_B58E_3738C3178EF3__INCLUDED_)
#define AFX_ENMMOTOR_H__E3F55576_CCCD_4B20_B58E_3738C3178EF3__INCLUDED_

#include "EnmBase.hpp"


class CEnmMotor
{
public:
    CEnmMotor();
    virtual ~CEnmMotor();

public:
    tagMotorInfo GetMotorInfo() const;
    void SetMotorInfo(tagMotorInfo& info );
    S32  GetCurStep() const;
    S32  GetSpareStep() const;
	S32  GetTotalStep() const;
  
public:
    virtual S32         IMoveTo( S32 pos );
	virtual S32			IToHead();
	virtual S32			IToHole( U32 idx );
	virtual S32			IReset( BOOLEAN bBack );
    virtual S32         IFwOne(){ return 1; }
    virtual S32         IBwOne(){ return 1; }

public:
    virtual BOOLEAN     IMotorOn( BOOLEAN bOn = TRUE ) = 0;
    virtual BOOLEAN     IMotorFW() = 0;
    virtual BOOLEAN     IMotorBW() = 0;
	virtual BOOLEAN		IIsReset() = 0;

protected:
    tagMotorInfo    mMotorInfo;
    S32             mCurPos;
};


inline tagMotorInfo CEnmMotor::GetMotorInfo() const
{
	return mMotorInfo;
}

inline
void CEnmMotor::SetMotorInfo(tagMotorInfo& info )
{
	mMotorInfo = info;
	IReset( TRUE );
}

inline S32  CEnmMotor::GetCurStep() const
{
	return mCurPos;
}

inline S32  CEnmMotor::GetSpareStep() const
{
	S32 aTotal = (mMotorInfo.fPostDist + mMotorInfo.fPreDist) / mMotorInfo.fStepDist;
	return ( aTotal - mCurPos);
}

inline S32  CEnmMotor::GetTotalStep() const
{
	S32 aTotal = (mMotorInfo.fPostDist + mMotorInfo.fPreDist) / mMotorInfo.fStepDist;
	return aTotal;
}


#endif // !defined(AFX_ENMMOTOR_H__E3F55576_CCCD_4B20_B58E_3738C3178EF3__INCLUDED_)
