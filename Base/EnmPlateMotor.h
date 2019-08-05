#ifndef ENMPLATEMOTOR_H_
#define ENMPLATEMOTOR_H_

#include "EnmMotor.h"

class CEnmPlateMotor  : public CEnmMotor
{
public:
	static CEnmPlateMotor& Instance();
	~CEnmPlateMotor();

protected:
	CEnmPlateMotor();

public:
    S32         IFwOne();
    S32         IBwOne();
    
public:
    BOOLEAN     IMotorOn( BOOLEAN bOn = TRUE );
    BOOLEAN     IMotorFW();
    BOOLEAN     IMotorBW();
	BOOLEAN		IIsReset();

};


#endif ///< ENMPLATEMOTOR_H_
