
#ifndef __EnmDensityCurvePage_H
#define __EnmDensityCurvePage_H

#include "EnmBasePage.hpp"
#include "EnmDataProc.h"

class CEnmDensityCurvePage : public CEnmBasePage  
{
	typedef CEnmBasePage  inherited;
	
public:
	CEnmDensityCurvePage();

protected:
	void	Draw();
	void	FocusOn();
	
protected:
	void	Init();

protected:
	void	DrawCurve();
	void	DrawAxis();
	void	DrawAxisUnit();
	void	DrawCurveLinear();
	void	DrawCurveSegment();

protected:
	U16		CalculateDensity();
	BOOLEAN Convert2Formulate( tagDateFormulate& rFormulate );
	void	PickMax();

/// member variable
protected:
	S16		mX0, mY0;		/// axis origonal point
	S16		mLenX, mLenY;

	FP64	mStepX;
	FP64	mStepY;

	FP64	mConcMax, mDensityMax;
	FP64	mK, mC;
	FP64	mDensity[ENM_DATA_COUNT*ENM_DATA_GROUP];
	U16		mDataCount;
};

#endif ///< __EnmDensityCurvePage_H
