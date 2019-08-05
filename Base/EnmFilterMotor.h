#ifndef ENMFILTERMOTOR_H_
#define ENMFILTERMOTOR_H_

#include "EnmMotor.h"

class CEnmFilterMotor  : public CEnmMotor
{
public:
	static CEnmFilterMotor& Instance();
	~CEnmFilterMotor();

protected:
	CEnmFilterMotor();

public:
    BOOLEAN     IMotorOn( BOOLEAN bOn = TRUE );
    BOOLEAN     IMotorFW();
    BOOLEAN     IMotorBW();
	BOOLEAN		IIsReset();

};


#endif ///< ENMFILTERMOTOR_H_
