// EnmFmWpMeasurePage.h: interface for the CEnmFmWpMeasurePage class.
//
//////////////////////////////////////////////////////////////////////

#ifndef  __EnmFmCmMesaPage_H
#define  __EnmFmCmMesaPage_H


#include "EnmBasePage.hpp"
#include "EnmDef.h"

class CEnmFmCmMeasPage : public CEnmBasePage  
{
	typedef CEnmBasePage inherited;

public:
	CEnmFmCmMeasPage();
	BOOLEAN  OnForward();
	BOOLEAN  OnExit();

protected:
	void	FocusOn();
	void	OnUserMsg( const tagMigMSG& stMsg );
	
	void	CalcConc();
	void	StartupScan( BOOLEAN bFirst = FALSE );
	void	StopScan();

protected:
	void   Draw();
	void   DrawHints();
	U8	   GetColumn( U32 columnMask );

protected:
	FP32		mConcData[ENM_REAGENT_LN_MAX];
	BOOLEAN		mbInScan;
};

#endif ///< __EnmFmCmMesaPage_H
