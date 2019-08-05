
#ifndef __EnmDensityListPage_H
#define __EnmDensityListPage_H

#include "EnmConcListPage.h"
#include "EnmDataProc.h"

class CEnmDensityListPage : public CEnmConcListPage  
{
	typedef CEnmConcListPage  inherited;
	
public:
	CEnmDensityListPage();

protected:
	void	SetPageData( S8 page );
	void	FocusOn();

protected:
	void	Init();
	void	CalculateDensity();
	BOOLEAN Convert2Formulate( tagDateFormulate& rFormulate );

/// member variables
protected:
	FP64	mDensity[ENM_DATA_COUNT*ENM_DATA_GROUP];
	U16		mDataCount;
};

#endif ///< __EnmDensityListPage_H
